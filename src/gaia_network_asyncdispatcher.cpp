#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_asyncdispatcher.h>

#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#include <gaia_network_socket_impl.h>

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <mswsock.h>
#	include <winsock2.h>
#	include <windows.h>
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
				dwError == WSAECONNRESET;
		}
	#endif

		class AsyncDispatcherThread : public GAIA::THREAD::Thread
		{
		public:
			GINL AsyncDispatcherThread(AsyncDispatcher* pDispatcher)
			{
				m_pDispatcher = pDispatcher;

			#if GAIA_OS != GAIA_OS_WINDOWS
				kqep = GINVALID;
			#endif
			}

			virtual GAIA::GVOID Run()
			{
				for(;;)
				{
					if(!m_pDispatcher->Execute())
						break;
				}
			}

		private:
			AsyncDispatcher* m_pDispatcher;

		public:
		#if GAIA_OS != GAIA_OS_WINDOWS
			GAIA::N32 kqep; // kqueue or epoll.
		#endif
		};

		AsyncDispatcher::AsyncDispatcher()
		{
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
				m_threads[x] = gnew AsyncDispatcherThread(this);

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

			// Create async controller in OS.
		#if GAIA_OS == GAIA_OS_WINDOWS
			m_pIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
		#endif

			// Start thread.
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
			{
				AsyncDispatcherThread* pThread = m_threads[x];

			#if GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
				pThread->kqep = kqueue();
			#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
				pThread->kqep = epoll_create();
			#endif

				m_threads[x]->Start();
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

			// Close all listen sockets.
			{
				GAIA::SYNC::AutolockW al(m_rwListenSockets);
				for(GAIA::CTN::Map<GAIA::NETWORK::Addr, GAIA::NETWORK::AsyncSocket*>::it it = m_listen_sockets.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::AsyncSocket* pSock = *it;
					pSock->m_sock.Close();
					pSock->drop_ref();
				}
				m_listen_sockets.clear();
			}

			// Close all accepting sockets.
			{
				GAIA::SYNC::AutolockW al(m_rwAcceptingSockets);
				for(GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*>::it it = m_accepting_sockets.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::AsyncSocket* pSock = *it;
					pSock->m_sock.Close();
					pSock->drop_ref();
				}
				m_accepting_sockets.clear();
			}

			// Notify the thread exit.
		#if GAIA_OS == GAIA_OS_WINDOWS
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
			{
				AsyncContext* pCtx = this->alloc_async_ctx();
				pCtx->type = ASYNC_CONTEXT_TYPE_STOP;
				PostQueuedCompletionStatus(m_pIOCP, sizeof(AsyncContext), 0, (OVERLAPPED*)pCtx);
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

			// End thread.
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
			{
				AsyncDispatcherThread* pThread = m_threads[x];

			#if GAIA_OS != GAIA_OS_WINDOWS
				close(pThread->kqep);
				pThread->kqep = GINVALID;
			#endif

				pThread->Wait();
			}

			// Close async controller in OS.
		#if GAIA_OS == GAIA_OS_WINDOWS
			::CloseHandle(m_pIOCP);
			m_pIOCP = GNIL;
		#endif

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
			pListenSocket->Create();
			pListenSocket->Bind(addrListen);

		#if GAIA_OS == GAIA_OS_WINDOWS
			this->attach_socket_iocp(*pListenSocket);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

			m_listen_sockets.insert(addrListen, pListenSocket);

			pListenSocket->m_sock.Listen();

		#if GAIA_OS == GAIA_OS_WINDOWS
			for(GAIA::NUM x = 0; x < m_desc.sWinIOCPAcceptEventCount; ++x)
				this->request_accept(*pListenSocket, addrListen);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

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
			m_pIOCP = GNIL;
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

		GAIA::BL AsyncDispatcher::Execute()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			DWORD dwTrans = 0;
			GAIA::GVOID* pPointer = GNIL;
			GAIA::NETWORK::AsyncContext* pCtx = GNIL;
			if(GetQueuedCompletionStatus(m_pIOCP, &dwTrans, (PULONG_PTR)&pPointer, (OVERLAPPED**)&pCtx, INFINITE))
			{
				if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_ACCEPT)
				{
					// Dispatch.
					pCtx->pDataSocket->SetAsyncSocketType(GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTED);
					this->RemoveAcceptingSocket(*pCtx->pDataSocket);
					this->AddAcceptedSocket(*pCtx->pDataSocket);

					this->attach_socket_iocp(*pCtx->pDataSocket);
					GAIA::N32 nAddrLen = sizeof(SOCKADDR_IN) + 16;
					sockaddr_in* saddr_peer = (sockaddr_in*)(pCtx->data + nAddrLen);
					GAIA::NETWORK::Addr addrPeer;
					GAIA::NETWORK::saddr2addr(saddr_peer, addrPeer);
					pCtx->pDataSocket->SetPeerAddress(addrPeer);
					GAIA::NETWORK::Addr addrListen;
					pCtx->pListenSocket->GetBindedAddress(addrListen);
					pCtx->pDataSocket->OnAccepted(GAIA::True, addrListen);
					this->OnAcceptSocket(*pCtx->pDataSocket, addrListen);

					// Begin receive.
					for(GAIA::NUM x = 0; x < m_desc.sWinIOCPRecvEventCount; ++x)
						this->request_recv(*pCtx->pDataSocket);

					// Re-request.
					this->request_accept(*pCtx->pListenSocket, addrListen);
				}
				else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_CONNECT)
				{
					// Dispatch.
					GAIA::NETWORK::Addr addrPeer;
					pCtx->pDataSocket->GetPeerAddress(addrPeer);
					pCtx->pDataSocket->OnConnected(GAIA::True, addrPeer);

					// Begin receive.
					for(GAIA::NUM x = 0; x < m_desc.sWinIOCPRecvEventCount; ++x)
						this->request_recv(*pCtx->pDataSocket);
				}
				else if(pCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_DISCONNECT)
				{
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
					}
					else if(dwTrans == 0)
					{
						DWORD dwErr = WSAGetLastError();
						if(dwErr == ERROR_IO_PENDING)
							this->request_recv(*pCtx->pDataSocket);
						else if(IsIOCPDisconnected(dwErr))
							pCtx->pDataSocket->OnDisconnected(GAIA::True);
					}
					else
						GASTFALSE;

					// Re-request.
					this->request_recv(*pCtx->pDataSocket);
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
										pCtx->pDataSocket->OnDisconnected(GAIA::True);
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
			pRet->pDataSocket = GNIL;
			pRet->sThreadIndex = GINVALID;
			pRet->kqep = GINVALID;
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
			if(!this->IsCreated())
				return GAIA::False;
			if(!this->IsBegin())
				return GAIA::False;
			if(!sock.IsCreated())
				return GAIA::False;
			::CreateIoCompletionPort((HANDLE)sock.GetFD(), m_pIOCP, 0, 0);
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
				pCtx->data, sizeof(pCtx->data) - nAddrLen - nAddrLen,
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
			if(!::WSARecv(datasock.GetFD(), &pCtx->_buf, 1, &dwTrans, &dwFlag, (OVERLAPPED*)pCtx, GNIL))
			{
				DWORD err = WSAGetLastError();
				if(err != ERROR_IO_PENDING)
				{
					this->release_async_ctx(pCtx);
					datasock.drop_ref();
				}
			}
		}
	#endif
	}
}
