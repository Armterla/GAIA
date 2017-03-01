#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_ctn_buffer.h>
#include <gaia_network_asyncdispatcher.h>

#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#include <gaia_network_base_impl.h>
#include <gaia_network_socket_impl.h>

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <mswsock.h>
#	include <winsock2.h>
#	include <windows.h>
//#	pragma comment(lib, "mswsock.lib")
#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
#	include <sys/types.h>
#	include <sys/event.h>
#	include <sys/time.h>
#	include <unistd.h>
#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

#endif

namespace GAIA
{
	namespace NETWORK
	{
	#if GAIA_OS == GAIA_OS_WINDOWS
		GINL GAIA::BL IsIOCPDisconnected(DWORD dwError)
		{
			return dwError == ERROR_SUCCESS ||
				dwError == ERROR_NETNAME_DELETED ||
				dwError == ERROR_OPERATION_ABORTED ||
				dwError == ERROR_CONNECTION_ABORTED ||
				dwError == WAIT_TIMEOUT ||
				dwError == WSAECONNABORTED ||
				dwError == WSAENETRESET ||
				dwError == WSAECONNRESET ||
				dwError == WSAESHUTDOWN ||
				dwError == ERROR_ALREADY_EXISTS ||
				dwError == WSAENOTSOCK;
		}
	#endif

		class AsyncDispatcherThread : public GAIA::THREAD::Thread
		{
		public:
			GINL AsyncDispatcherThread(AsyncDispatcher* pDispatcher)
			{
				m_pDispatcher = pDispatcher;

				sIndex = GINVALID;

			#if GAIA_OS == GAIA_OS_WINDOWS
				iocp = (GAIA::GVOID*)(GAIA::U64)GINVALID;
			#else
				kqep = GINVALID;
				bStopCmd = GAIA::False;
			#endif
			}

			virtual GAIA::GVOID Run()
			{
				for(;;)
				{
				#if GAIA_OS != GAIA_OS_WINDOWS
					if(bStopCmd)
						break;
				#endif
					if(!m_pDispatcher->Execute(this))
						break;
				}
			}

		private:
			AsyncDispatcher* m_pDispatcher;

		public:
			GAIA::NUM sIndex;
		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::GVOID* iocp;
		#else
			GAIA::N32 kqep; // kqueue or epoll.
			GAIA::BL bStopCmd;
			GAIA::CTN::Buffer tempbuf;
		#endif
		};

		AsyncDispatcher::AsyncDispatcher()
		{
			GAST(sizeof(GAIA::GVOID*) == sizeof(GAIA::NM));
			GAST(sizeof(GAIA::GVOID*) == sizeof(GAIA::UM));
			this->init();
		}

		AsyncDispatcher::~AsyncDispatcher()
		{
			if(this->IsCreated())
				this->Destroy();
		}

		GAIA::BL AsyncDispatcher::Create(const GAIA::NETWORK::AsyncDispatcherDesc& desc)
		{
			if(this->IsCreated())
				return GAIA::False;
			if(!desc.check())
				return GAIA::False;

			m_threads.resize(desc.sThreadCount);
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
			{
				AsyncDispatcherThread* pThread = gnew AsyncDispatcherThread(this);
				pThread->sIndex = x;
				m_threads[x] = pThread;
			}

			m_desc = desc;
			m_bCreated = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::Destroy()
		{
			if(!this->IsCreated())
				return GAIA::False;

			if(this->IsBegin())
				this->End();

			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
			{
				AsyncDispatcherThread* pThread = m_threads[x];
				GAST(pThread != GNIL);
				gdel pThread;
			}
			m_threads.clear();

			m_desc.reset();
			m_bCreated = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::Begin()
		{
			if(this->IsBegin())
				return GAIA::False;

		#if GAIA_OS == GAIA_OS_WINDOWS
			// Forbid any new accept post.
			{
				GAIA::SYNC::AutolockW al(m_rwPostAcceptAble);
				m_bPostAcceptAble = GAIA::True;
			}
		#endif

			// Start thread.
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
			{
				AsyncDispatcherThread* pThread = m_threads[x];

			#if GAIA_OS == GAIA_OS_WINDOWS
				pThread->iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, GNIL, 0, 0);
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
				pThread->kqep = kqueue();
				GAST(pThread->kqep != GINVALID);
				pThread->bStopCmd = GAIA::False;
			#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
				GAIA::NUM sThreadMaxConnCount = m_desc.sMaxConnectionCount / m_desc.sThreadCount;
				if((m_desc.sMaxConnectionCount % m_desc.sThreadCount) != 0)
					sThreadMaxConnCount += 1;
				pThread->kqep = epoll_create(sThreadMaxCnnCount);
				GAST(pThread->kqep >= 0);
				pThread->bStopCmd = GAIA::False;
			#endif
				pThread->Start();
			}

			m_bBegin = GAIA::True;

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::End()
		{
			if(!this->IsBegin())
				return GAIA::False;

		#if GAIA_OS == GAIA_OS_WINDOWS
			// Forbid any new accept post.
			{
				GAIA::SYNC::AutolockW al(m_rwPostAcceptAble);
				m_bPostAcceptAble = GAIA::False;
			}
		#endif

			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::SYNC::AutolockW al(m_rwAsyncCtxEnding);
			#endif

				// Close all listen sockets.
				{
					GAIA::SYNC::AutolockW al(m_rwListenSockets);
					GAIA::CTN::Vector<GAIA::NETWORK::AsyncSocket*> vecTemp;
					vecTemp.reserve(m_listen_sockets.size());
					for(GAIA::CTN::Map<GAIA::NETWORK::Addr, GAIA::NETWORK::AsyncSocket*>::it it = m_listen_sockets.frontit(); !it.empty(); ++it)
						vecTemp.push_back(*it);
					m_listen_sockets.clear();

					for(GAIA::NUM x = 0; x < vecTemp.size(); ++x)
					{
						GAIA::NETWORK::AsyncSocket* pSock = vecTemp[x];
						pSock->m_sock.Close();
						pSock->drop_ref();
					}
				}

				// Close all accepting sockets.
				{
					GAIA::SYNC::AutolockW al(m_rwAcceptingSockets);
					GAIA::CTN::Vector<GAIA::NETWORK::AsyncSocket*> vecTemp;
					vecTemp.reserve(m_accepting_sockets.size());
					for(GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*>::it it = m_accepting_sockets.frontit(); !it.empty(); ++it)
						vecTemp.push_back(*it);
					m_accepting_sockets.clear();

					for(GAIA::NUM x = 0; x < vecTemp.size(); ++x)
					{
						GAIA::NETWORK::AsyncSocket* pSock = vecTemp[x];
						pSock->m_sock.Close();
						pSock->drop_ref();
					}
				}
			}

			// Notify the thread exit.
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
			{
				AsyncDispatcherThread* pThread = m_threads[x];
			#if GAIA_OS == GAIA_OS_WINDOWS
				AsyncContext* pCtx = this->alloc_async_ctx();
				pCtx->type = ASYNC_CONTEXT_TYPE_STOP;
				PostQueuedCompletionStatus(pThread->iocp, sizeof(AsyncContext), 0, (OVERLAPPED*)pCtx);
			#else
				pThread->bStopCmd = GAIA::True;
			#endif
			}

			// End thread.
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
			{
				AsyncDispatcherThread* pThread = m_threads[x];
				pThread->Wait();

			#if GAIA_OS == GAIA_OS_WINDOWS
				::CloseHandle(pThread->iocp);
				pThread->iocp = (GAIA::GVOID*)(GAIA::U64)GINVALID;
			#else
				close(pThread->kqep);
				pThread->kqep = GINVALID;
				pThread->bStopCmd = GAIA::False;
			#endif
			}

			m_bBegin = GAIA::False;

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::AddListenSocket(const GAIA::NETWORK::Addr& addrListen)
		{
			GAIA::SYNC::AutolockW al(m_rwListenSockets);

			if(m_listen_sockets.find(addrListen) != GNIL)
				return GAIA::False;

			//
			GAIA::NETWORK::AsyncSocket* pListenSocket = this->OnCreateListenSocket(addrListen);
			GTRY
			{
				pListenSocket->Create();
				pListenSocket->Bind(addrListen);
			}
			GCATCHALL
			{
				pListenSocket->drop_ref();
				return GAIA::False;
			}

		#if GAIA_OS == GAIA_OS_WINDOWS
			this->attach_socket_iocp(*pListenSocket);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			GAIA::N32 kqep = this->select_kqep(pListenSocket->GetFD());
			AsyncContext* pCtx = this->alloc_async_ctx();
			pCtx->type = GAIA::NETWORK::ASYNC_CONTEXT_TYPE_ACCEPT;
			pCtx->pSocket = pListenSocket;
			GAST(pListenSocket->m_pReadAsyncCtx == GNIL);
			pListenSocket->m_pReadAsyncCtx = pCtx;
			struct kevent ke;
			EV_SET(&ke, pListenSocket->GetFD(), EVFILT_READ, EV_ADD, 0, 0, pCtx);
			GAIA::NUM sResult = kevent(kqep, &ke, 1, GNIL, 0, GNIL);
			if(sResult == GINVALID)
			{
				this->release_async_ctx(pCtx);
				pListenSocket->drop_ref();
				return GAIA::False;
			}
		// LISTEN SOCKET NOT CALLBACK ON KQUEUE WHEN CLOSED,
		// SO I CAN'T RISE REFERENCE COUNT FOR MULTI-THREAD DISPATCH.
		// [BEGIN]
		//	else
		//		pListenSocket->rise_ref();
		// [END]

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

			m_listen_sockets.insert(addrListen, pListenSocket);

			pListenSocket->m_sock.Listen();
			pListenSocket->OnListened(GAIA::True);

		#if GAIA_OS == GAIA_OS_WINDOWS
			for(GAIA::NUM x = 0; x < m_desc.sWinIOCPAcceptEventCount; ++x)
				this->request_accept(*pListenSocket, addrListen);
		#endif

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveListenSocket(const GAIA::NETWORK::Addr& addrListen)
		{
			GAIA::SYNC::AutolockW al(m_rwListenSockets);
			GAIA::NETWORK::AsyncSocket** ppAsyncSocket = m_listen_sockets.find(addrListen);
			if(ppAsyncSocket == GNIL)
				return GAIA::False;
			GAIA::NETWORK::AsyncSocket* pAsyncSocket = *ppAsyncSocket;
			GAST(pAsyncSocket != GNIL);
			m_listen_sockets.erase(addrListen);
			pAsyncSocket->Shutdown();
			pAsyncSocket->Close();
			pAsyncSocket->drop_ref();
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveListenSocketAll()
		{
			GAIA::SYNC::AutolockW al(m_rwListenSockets);
			if(m_listen_sockets.empty())
				return GAIA::False;

			for(GAIA::CTN::Map<GAIA::NETWORK::Addr, GAIA::NETWORK::AsyncSocket*>::it it = m_listen_sockets.frontit(); !it.empty(); ++it)
			{
				GAIA::NETWORK::AsyncSocket* pSock = *it;
				pSock->Shutdown();
				pSock->Close();
				pSock->drop_ref();
			}
			m_listen_sockets.clear();

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsExistListenSocket(const GAIA::NETWORK::Addr& addrListen) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwListenSockets);
			const GAIA::CTN::Map<GAIA::NETWORK::Addr, GAIA::NETWORK::AsyncSocket*>::_datatype* ppFinded = m_listen_sockets.find(addrListen);
			if(ppFinded == GNIL)
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::NUM AsyncDispatcher::GetListenSocketCount() const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwListenSockets);
			return m_listen_sockets.size();
		}

		GAIA::BL AsyncDispatcher::CollectListenSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwListenSockets);
			for(GAIA::CTN::Map<GAIA::NETWORK::Addr, GAIA::NETWORK::AsyncSocket*>::const_it it = m_listen_sockets.const_frontit(); !it.empty(); ++it)
			{
				GAIA::NETWORK::AsyncSocket* pSock = *it;
				if(!cb.OnCollectAsyncSocket(*GCCAST(AsyncDispatcher*)(this), *pSock))
					return GAIA::False;
			}
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsExistAcceptingSocket(GAIA::NETWORK::AsyncSocket& sock) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptingSockets);
			return m_accepting_sockets.find(&sock) != GNIL;
		}

		GAIA::NUM AsyncDispatcher::GetAcceptingSocketCount() const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptingSockets);
			return m_accepting_sockets.size();
		}

		GAIA::BL AsyncDispatcher::CollectAcceptingSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptingSockets);
			for(GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*>::const_it it = m_accepting_sockets.const_frontit(); !it.empty(); ++it)
			{
				GAIA::NETWORK::AsyncSocket* pSock = *it;
				GAST(pSock != GNIL);
				if(!cb.OnCollectAsyncSocket(*GCCAST(AsyncDispatcher*)(this), *pSock))
					return GAIA::False;
			}
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsExistAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptedSockets);
			return m_accepted_sockets.find(&sock) != GNIL;
		}

		GAIA::NUM AsyncDispatcher::GetAcceptedSocketCount() const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptedSockets);
			return m_accepted_sockets.size();
		}

		GAIA::BL AsyncDispatcher::CollectAcceptedSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptedSockets);
			for(GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*>::const_it it = m_accepted_sockets.const_frontit(); !it.empty(); ++it)
			{
				GAIA::NETWORK::AsyncSocket* pSock = *it;
				GAST(pSock != GNIL);
				if(!cb.OnCollectAsyncSocket(*GCCAST(AsyncDispatcher*)(this), *pSock))
					return GAIA::False;
			}
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsExistConnectedSocket(GAIA::NETWORK::AsyncSocket& sock) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwConnectedSockets);
			return m_connected_sockets.find(&sock) != GNIL;
		}

		GAIA::NUM AsyncDispatcher::GetConnectedSocketCount() const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwConnectedSockets);
			return m_connected_sockets.size();
		}

		GAIA::BL AsyncDispatcher::CollectConnectedSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwConnectedSockets);
			for(GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*>::const_it it = m_connected_sockets.const_frontit(); !it.empty(); ++it)
			{
				GAIA::NETWORK::AsyncSocket* pSock = *it;
				GAST(pSock != GNIL);
				if(!cb.OnCollectAsyncSocket(*GCCAST(AsyncDispatcher*)(this), *pSock))
					return GAIA::False;
			}
			return GAIA::True;
		}

		GAIA::GVOID AsyncDispatcher::init()
		{
			m_bCreated = GAIA::False;
			m_bBegin = GAIA::False;
			m_desc.reset();
		#if GAIA_OS == GAIA_OS_WINDOWS
			m_bPostAcceptAble = GAIA::True;
		#endif
		}

		GAIA::BL AsyncDispatcher::AddAcceptingSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwAcceptingSockets);
			if(m_accepting_sockets.find(&sock) != GNIL)
				return GAIA::False;
			m_accepting_sockets.insert(&sock);
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveAcceptingSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwAcceptingSockets);
			return m_accepting_sockets.erase(&sock) != GNIL;
		}

		GAIA::BL AsyncDispatcher::AddAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwAcceptedSockets);
			if(m_accepted_sockets.find(&sock) != GNIL)
				return GAIA::False;
			m_accepted_sockets.insert(&sock);
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwAcceptedSockets);
			return m_accepted_sockets.erase(&sock) != GNIL;
		}

		GAIA::BL AsyncDispatcher::AddConnectedSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwConnectedSockets);
			if(m_connected_sockets.find(&sock) != GNIL)
				return GAIA::False;
			m_connected_sockets.insert(&sock);
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveConnectedSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwConnectedSockets);
			return m_connected_sockets.erase(&sock) != GNIL;
		}

		GAIA::BL AsyncDispatcher::Execute(GAIA::NETWORK::AsyncDispatcherThread* pThread)
		{
			GAST(pThread != GNIL);

		#if GAIA_OS == GAIA_OS_WINDOWS
			DWORD dwTrans = 0;
			GAIA::GVOID* pPointer = GNIL;
			GAIA::NETWORK::AsyncContext* pCtx = GNIL;
			if(GetQueuedCompletionStatus(pThread->iocp, &dwTrans, (PULONG_PTR)&pPointer, (OVERLAPPED**)&pCtx, INFINITE))
			{
				if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_ACCEPT)
				{
					// Dispatch.
					pCtx->pDataSocket->SetAsyncSocketType(GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTED);
					this->RemoveAcceptingSocket(*pCtx->pDataSocket);
					this->AddAcceptedSocket(*pCtx->pDataSocket);

					GAIA::NETWORK::Addr addrListen;
					pCtx->pListenSocket->GetBindedAddress(addrListen);

					{
						GAIA::SYNC::AutolockR al(m_rwAsyncCtxEnding);
						if(pCtx->pDataSocket->IsCreated())
						{
							this->attach_socket_iocp(*pCtx->pDataSocket);
							pCtx->pDataSocket->SetBinded(GAIA::True);
							pCtx->pDataSocket->SetConnected(GAIA::True);

							GAIA::NM nListenSock = pCtx->pListenSocket->GetFD();
							GAIA::N32 nSetSockOptResult = setsockopt(
									pCtx->pDataSocket->GetFD(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
									(char*)&nListenSock, sizeof(nListenSock));
							GAST(nSetSockOptResult == 0);

							GAIA::NETWORK::Addr addrPeer;
							pCtx->pDataSocket->GetGlobalAddress(addrPeer);
							pCtx->pDataSocket->SetPeerAddress(addrPeer);

							pCtx->pDataSocket->OnAccepted(GAIA::True, addrListen);
							if(this->OnAcceptSocket(*pCtx->pDataSocket, addrListen))
							{
								for(GAIA::NUM x = 0; x < m_desc.sWinIOCPRecvEventCount; ++x)
									this->request_recv(*pCtx->pDataSocket);
							}
						}

						// Re-request.
						if(pCtx->pListenSocket->IsCreated())
							this->request_accept(*pCtx->pListenSocket, addrListen);
					}
				}
				else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_CONNECT)
				{
					// Dispatch.
					GAIA::NETWORK::Addr addrPeer;
					pCtx->pDataSocket->GetPeerAddress(addrPeer);
					pCtx->pDataSocket->SetConnected(GAIA::True);
					pCtx->pDataSocket->OnConnected(GAIA::True, addrPeer);

					// Begin receive.
					for(GAIA::NUM x = 0; x < m_desc.sWinIOCPRecvEventCount; ++x)
						this->request_recv(*pCtx->pDataSocket);
				}
				else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_DISCONNECT)
				{
					pCtx->pDataSocket->SetConnected(GAIA::False);
					pCtx->pDataSocket->OnDisconnected(GAIA::True);
					pCtx->pDataSocket->SwapBrokenState();
				}
				else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_SEND)
				{
					GAST(dwTrans == pCtx->_buf.len);
					pCtx->pDataSocket->OnSent(GAIA::True, pCtx->data, dwTrans, pCtx->_buf.len);
				}
				else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_RECV)
				{
					// Dispatch.
					if(dwTrans > 0)
					{
						pCtx->pDataSocket->OnRecved(GAIA::True, pCtx->data, dwTrans);
						this->request_recv(*pCtx->pDataSocket);
					}
					else if(dwTrans == 0)
					{
						DWORD dwErr = WSAGetLastError();
						if(dwErr == ERROR_IO_PENDING)
							this->request_recv(*pCtx->pDataSocket);
						else if(IsIOCPDisconnected(dwErr))
						{
							if(pCtx->pDataSocket->IsCreated() && pCtx->pDataSocket->IsConnected())
							{
								pCtx->pDataSocket->SetConnected(GAIA::False);
								pCtx->pDataSocket->OnDisconnected(GAIA::True);
							}
						}
						else
							this->request_recv(*pCtx->pDataSocket);
					}
					else
						GASTFALSE;
				}
				else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_STOP)
				{
					GAST(pCtx->pListenSocket == GNIL);
					GAST(pCtx->pDataSocket == GNIL);
					GAIA::SYNC::Autolock al(m_lrAsyncCtxPool);
					m_AsyncCtxPool.release(pCtx);
					return GAIA::False;
				}
				else
					GASTFALSE;
			}
			else
			{
				if(pCtx != GNIL)
				{
					if(pCtx->pDataSocket->SwapBrokenState())
					{
						if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_RECV)
						{
							if(dwTrans == 0)
							{
								DWORD dwError = WSAGetLastError();
								if(dwError == ERROR_NETNAME_DELETED)
								{
									DWORD dwTrans, dwFlags;
									if(WSAGetOverlappedResult(pCtx->pDataSocket->GetFD(),
										(WSAOVERLAPPED*)pCtx, &dwTrans, GAIA::False, &dwFlags))
										dwError = WSAGetLastError();

									if(IsIOCPDisconnected(dwError))
									{
										pCtx->pDataSocket->SetConnected(GAIA::False);
										pCtx->pDataSocket->OnDisconnected(GAIA::True);
									}
								}
								else
									GERR << "GAIA AsyncDispatcher IOCP error, cannot recv, ErrorCode = " << dwError << GEND;
							}
							else
								GASTFALSE;
						}
						else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_SEND)
						{
							pCtx->pDataSocket->OnSent(GAIA::False, pCtx->data, dwTrans, pCtx->_buf.len);
							pCtx->pDataSocket->SetConnected(GAIA::False);
							pCtx->pDataSocket->OnDisconnected(GAIA::True);
						}
						else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_CONNECT)
						{
							GAIA::NETWORK::Addr addrPeer;
							pCtx->pDataSocket->GetPeerAddress(addrPeer);
							pCtx->pDataSocket->OnConnected(GAIA::False, addrPeer);
						}
						else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_DISCONNECT)
						{
							pCtx->pDataSocket->SetConnected(GAIA::False);
							pCtx->pDataSocket->OnDisconnected(GAIA::False);
						}
						else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_ACCEPT)
						{
							GAIA::NETWORK::Addr addrBinded;
							pCtx->pListenSocket->GetBindedAddress(addrBinded);
							pCtx->pDataSocket->OnAccepted(GAIA::False, addrBinded);
						}
						else
							GASTFALSE;
					}
				}
				else
					GASTFALSE;
			}
			if(pCtx != GNIL)
			{
				if(pCtx->pListenSocket != GNIL)
					pCtx->pListenSocket->drop_ref();
				if(pCtx->pDataSocket != GNIL)
					pCtx->pDataSocket->drop_ref();
				GAIA::SYNC::Autolock al(m_lrAsyncCtxPool);
				m_AsyncCtxPool.release(pCtx);
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			struct timespec timeout;
			timeout.tv_sec = 0;
			timeout.tv_nsec = 10 * 1000 * 1000; // 10 MilliSeconds.
			struct kevent elist[128];
			GAIA::NUM sEventCount = kevent(pThread->kqep, GNIL, 0, elist, sizeofarray(elist), &timeout);
			if(sEventCount == GINVALID)
				return GAIA::False; // kqueue closed by AsyncDispatcher::End.
			else if(sEventCount > 0)
			{
				for(GAIA::NUM x = 0; x < sEventCount; ++x)
				{
					struct kevent& e = elist[x];
					GAIA::NETWORK::AsyncContext& ctx = *(GAIA::NETWORK::AsyncContext*)e.udata;
					GAIA::N32 nSocket = (GAIA::N32)e.ident;
					GAST(nSocket != GINVALID);
					GAST(ctx.pSocket == GNIL || ctx.pSocket->GetFD() == GINVALID || nSocket == ctx.pSocket->GetFD());
					GAST(pThread->kqep == this->select_kqep(nSocket));

					GAIA::BL bConnectBroken = GAIA::False;
					if(e.flags & EV_EOF)
						bConnectBroken = GAIA::True;
					else if(e.flags & EV_ERROR)
					{
						GAIA::N32 nErr = (GAIA::N32)e.data;
						if(nErr != EWOULDBLOCK && nErr != EINPROGRESS)
							bConnectBroken = GAIA::True;
					}
					if(bConnectBroken)
					{
						ctx.pSocket->SetConnected(GAIA::False);
						ctx.pSocket->OnDisconnected(GAIA::True);
						struct kevent ke;
						EV_SET(&ke, nSocket, EVFILT_WRITE, EV_DELETE, 0, 0, GNIL);
						kevent(pThread->kqep, &ke, 1, GNIL, 0, GNIL);
						EV_SET(&ke, nSocket, EVFILT_READ, EV_DELETE, 0, 0, GNIL);
						if(kevent(pThread->kqep, &ke, 1, GNIL, 0, GNIL) != GINVALID)
							ctx.pSocket->drop_ref();
						continue;
					}

					if(e.filter == EVFILT_READ)
					{
						switch(ctx.type)
						{
						case GAIA::NETWORK::ASYNC_CONTEXT_TYPE_ACCEPT:
							{
								GAIA::NUM sAcceptAbleCount = (GAIA::NUM)e.data;
								for(GAIA::NUM y = 0; y < sAcceptAbleCount; ++y)
								{
									sockaddr_in addraccept;
									socklen_t newaddrlen = sizeof(addraccept);
									GAIA::N32 nNewSocket = accept(nSocket, (sockaddr*)&addraccept, &newaddrlen);
									if(nNewSocket != GINVALID)
									{
										GAIA::NETWORK::Addr addrListen;
										ctx.pSocket->GetBindedAddress(addrListen);
										GAIA::NETWORK::AsyncSocket* pAcceptedSock = this->OnCreateAcceptingSocket(addrListen);
										pAcceptedSock->SetFD(nNewSocket);
										pAcceptedSock->SetType(GAIA::NETWORK::Socket::SOCKET_TYPE_STREAM);
										pAcceptedSock->SetBinded(GAIA::True);
										pAcceptedSock->SetConnected(GAIA::True);
										GAIA::NETWORK::Addr addrPeer;
										GAIA::NETWORK::saddr2addr(&addraccept, addrPeer);
										pAcceptedSock->SetPeerAddress(addrPeer);

										pAcceptedSock->SetAsyncSocketType(GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTED);
										this->RemoveAcceptingSocket(*pAcceptedSock);
										this->AddAcceptedSocket(*pAcceptedSock);

										AsyncContext* pCtxRecv = this->alloc_async_ctx();
										pCtxRecv->pSocket = pAcceptedSock;
										pCtxRecv->type = GAIA::NETWORK::ASYNC_CONTEXT_TYPE_RECV;
										GAST(pAcceptedSock->m_pReadAsyncCtx == GNIL);
										pAcceptedSock->m_pReadAsyncCtx = pCtxRecv;

										AsyncContext* pCtxSend = this->alloc_async_ctx();
										pCtxSend->pSocket = pAcceptedSock;
										pCtxSend->type = GAIA::NETWORK::ASYNC_CONTEXT_TYPE_SEND;
										GAST(pAcceptedSock->m_pWriteAsyncCtx == GNIL);
										pAcceptedSock->m_pWriteAsyncCtx = pCtxSend;

										pAcceptedSock->OnCreated(GAIA::True);
										pAcceptedSock->OnAccepted(GAIA::True, addrListen);
										if(this->OnAcceptSocket(*pAcceptedSock, addrListen))
										{
											struct kevent ke[2];
											EV_SET(&ke[0], nNewSocket, EVFILT_READ, EV_ADD, 0, 0, pCtxRecv);
											EV_SET(&ke[1], nNewSocket, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, pCtxSend);
											GAIA::N32 kqep = this->select_kqep(nNewSocket);
											GAIA::NUM sResult = kevent(kqep, ke, 2, GNIL, 0, GNIL);
											if(sResult != GINVALID)
											{
												GAST(sResult == 0);
												pAcceptedSock->rise_ref();
											}
										}
									}
									else
										break;
								}
							}
							break;
						case GAIA::NETWORK::ASYNC_CONTEXT_TYPE_RECV:
							{
								GAIA::NUM sRecvAbleSize = (GAIA::NUM)e.data;
								if(sRecvAbleSize > 0)
								{
									pThread->tempbuf.resize(sRecvAbleSize);
									GAIA::NUM sRecved = (GAIA::NUM)recv(nSocket, pThread->tempbuf.fptr(), sRecvAbleSize, 0);
									ctx.pSocket->OnRecved(GAIA::True, pThread->tempbuf.fptr(), sRecved);
								}
							}
							break;
						default:
							GASTFALSE;
							break;
						}
					}
					else if(e.filter == EVFILT_WRITE)
					{
						switch(ctx.type)
						{
						case GAIA::NETWORK::ASYNC_CONTEXT_TYPE_CONNECT:
							{
								AsyncContext* pCtxRecv = this->alloc_async_ctx();
								pCtxRecv->pSocket = ctx.pSocket;
								pCtxRecv->type = GAIA::NETWORK::ASYNC_CONTEXT_TYPE_RECV;
								GAST(ctx.pSocket->m_pReadAsyncCtx == GNIL);
								ctx.pSocket->m_pReadAsyncCtx = pCtxRecv;

								GAST(ctx.pSocket->m_pWriteAsyncCtx != GNIL);
								GAST(ctx.pSocket->m_pWriteAsyncCtx == &ctx);
								ctx.type = GAIA::NETWORK::ASYNC_CONTEXT_TYPE_SEND;

								GAIA::NETWORK::Addr addrPeer;
								ctx.pSocket->GetPeerAddress(addrPeer);
								ctx.pSocket->SetConnected(GAIA::True);
								ctx.pSocket->OnConnected(GAIA::True, addrPeer);

								struct kevent ke[2];
								EV_SET(&ke[0], nSocket, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, &ctx);
								EV_SET(&ke[1], nSocket, EVFILT_READ, EV_ADD, 0, 0, pCtxRecv);
								GAIA::NUM sResult = kevent(pThread->kqep, ke, 2, GNIL, 0, GNIL);
								if(sResult != GINVALID)
								{
									GAST(sResult == 0);
									pCtxRecv->pSocket->rise_ref();
								}
							}
							break;
						case GAIA::NETWORK::ASYNC_CONTEXT_TYPE_SEND:
							{
								GAIA::NUM sSendAbleSize = (GAIA::NUM)e.data;
								if(sSendAbleSize > 0)
								{
									pThread->tempbuf.resize(sSendAbleSize);
									{
										GAIA::SYNC::Autolock al(ctx.pSocket->m_lrSend);
										GAIA::NUM sNeedSendSize = ctx.pSocket->m_sendbuf.read(pThread->tempbuf.fptr(), sSendAbleSize);
										if(sNeedSendSize > 0)
										{
											GAIA::NUM sSendedSize = (GAIA::NUM)send(nSocket, pThread->tempbuf.fptr(), sNeedSendSize, 0);
											GAST(sSendedSize == sNeedSendSize);
											ctx.pSocket->OnSent(GAIA::True, pThread->tempbuf.fptr(), sSendedSize, sSendedSize + pThread->tempbuf.remain());
										}
										if(!ctx.pSocket->m_sendbuf.empty())
										{
											GAST(ctx.pSocket->m_pWriteAsyncCtx != GNIL);
											struct kevent ke;
											EV_SET(&ke, nSocket, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, ctx.pSocket->m_pWriteAsyncCtx);
											GAIA::NUM sResult = kevent(pThread->kqep, &ke, 1, GNIL, 0, GNIL);
											GAST(sResult != GINVALID);
										}
									}
								}
							}
							break;
						default:
							GASTFALSE;
							break;
						}
					}
				}
			}
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
			return GAIA::True;
		}

		GAIA::NETWORK::AsyncContext* AsyncDispatcher::alloc_async_ctx()
		{
			GAIA::SYNC::Autolock al(m_lrAsyncCtxPool);
			GAIA::NETWORK::AsyncContext* pRet = m_AsyncCtxPool.alloc();
		#if GAIA_OS == GAIA_OS_WINDOWS
			zeromem(&pRet->_ovlp);
			pRet->_buf.len = 0;
			pRet->_buf.buf = (GAIA::CH*)pRet->data;
			pRet->pListenSocket = GNIL;
			pRet->pDataSocket = GNIL;
		#else
			pRet->pSocket = GNIL;
		#endif
			pRet->type = GAIA::NETWORK::ASYNC_CONTEXT_TYPE_INVALID;
			return pRet;
		}

		GAIA::GVOID AsyncDispatcher::release_async_ctx(GAIA::NETWORK::AsyncContext* pCtx)
		{
			GAST(pCtx != GNIL);
			GAIA::SYNC::Autolock al(m_lrAsyncCtxPool);
			return m_AsyncCtxPool.release(pCtx);
		}

	#if GAIA_OS == GAIA_OS_WINDOWS
		GAIA::BL AsyncDispatcher::attach_socket_iocp(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAST(this->IsCreated());
			if(!this->IsCreated())
				return GAIA::False;
			GAST(this->IsBegin());
			if(!this->IsBegin())
				return GAIA::False;
			GAST(sock.IsCreated());
			if(!sock.IsCreated())
				return GAIA::False;
			GAIA::NUM sIndex = sock.GetFD() / sizeof(GAIA::GVOID*) % m_threads.size();
			GAIA::GVOID* iocp = m_threads[sIndex]->iocp;
			HANDLE h = ::CreateIoCompletionPort((HANDLE)(GAIA::U64)sock.GetFD(), (HANDLE)iocp, 0, 0);
			GAST(h != GNIL);
			return GAIA::True;
		}

		GAIA::GVOID AsyncDispatcher::request_accept(GAIA::NETWORK::AsyncSocket& listensock, const GAIA::NETWORK::Addr& addrListen)
		{
			GAIA::SYNC::AutolockR al(m_rwPostAcceptAble);
			if(!m_bPostAcceptAble)
				return;

			GAIA::NETWORK::AsyncSocket* pAcceptingSocket = this->OnCreateAcceptingSocket(addrListen);
			pAcceptingSocket->Create();

			GAIA::NETWORK::AsyncContext* pCtx = this->alloc_async_ctx();
			pCtx->type = ASYNC_CONTEXT_TYPE_ACCEPT;
			pCtx->pListenSocket = &listensock;
			pCtx->pDataSocket = pAcceptingSocket;
			listensock.rise_ref();
			pAcceptingSocket->rise_ref();

			GAIA::N32 nAddrLen = sizeof(SOCKADDR_IN) + 16;
			DWORD dwRecved = 0;
			if(!((LPFN_ACCEPTEX)listensock.m_pfnAcceptEx)(
				listensock.GetFD(), pAcceptingSocket->GetFD(),
				pCtx->data, 0,
				nAddrLen, nAddrLen, &dwRecved, (OVERLAPPED*)pCtx))
			{
				DWORD err = WSAGetLastError();
				if(err != ERROR_IO_PENDING)
				{
					listensock.drop_ref();
					pAcceptingSocket->drop_ref();
					pAcceptingSocket->drop_ref();
					this->release_async_ctx(pCtx);
					GERR << "GAIA AsyncDispatcher IOCP error, cannot AcceptEx, ErrorCode = " << ::WSAGetLastError() << GEND;
				}
			}
		}

		GAIA::GVOID AsyncDispatcher::request_recv(GAIA::NETWORK::AsyncSocket& datasock)
		{
			AsyncContext* pCtx = this->alloc_async_ctx();
			pCtx->type = ASYNC_CONTEXT_TYPE_RECV;
			pCtx->pDataSocket = &datasock;
			pCtx->_buf.len = sizeof(pCtx->data);
			datasock.rise_ref();

			DWORD dwTrans = 0;
			DWORD dwFlag = 0;
			if(::WSARecv(datasock.GetFD(), &pCtx->_buf, 1, &dwTrans, &dwFlag, (OVERLAPPED*)pCtx, GNIL) != 0)
			{
				DWORD err = WSAGetLastError();
				if(err != ERROR_IO_PENDING)
				{
					this->release_async_ctx(pCtx);
					datasock.drop_ref();
					GERR << "GAIA AsyncDispatcher IOCP error, cannot WSARecv, ErrorCode = " << ::WSAGetLastError() << GEND;
				}
			}
		}

	#else
		GAIA::N32 AsyncDispatcher::select_kqep(GAIA::N32 nSocket) const
		{
			GAIA::NUM sIndex = nSocket / sizeof(GAIA::GVOID*) % m_threads.size();
			GAIA::N32 kqep = m_threads[sIndex]->kqep;
			return kqep;
		}
	#endif
	}
}
