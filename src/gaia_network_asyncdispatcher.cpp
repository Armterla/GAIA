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

		GAIA::BL AsyncDispatcher::Create(const GAIA::NETWORK::AsyncDispatcher::Desc& desc)
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
				AsyncDispatcherThread* pThread = (AsyncDispatcherThread*)m_threads[x];
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

			// Create async controller in OS.
		#if GAIA_OS == GAIA_OS_WINDOWS
			m_pIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			m_kqueue = kqueue();
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			m_epoll = epoll_create();
		#endif

			// Start thread.
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
				m_threads[x]->Start();

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::End()
		{
			if(!this->IsBegin())
				return GAIA::False;

			// Notify the thread exit.
			// TODO: IOCP_OVERLAPPED_TYPE_STOP.

			// End thread.
			for(GAIA::NUM x = 0; x < m_threads.size(); ++x)
				m_threads[x]->Wait();

			// Close async controller in OS.
		#if GAIA_OS == GAIA_OS_WINDOWS
			::CloseHandle(m_pIOCP);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			close(m_kqueue);
			m_kqueue = GINVALID;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			close(m_epoll);
			m_epoll = GINVALID;
		#endif

			// Close all sockets.
			{
				// Listen sockets.
				{
					GAIA::SYNC::AutolockW al(m_rwListenSockets);
					for(GAIA::CTN::Set<Node>::it it = m_listen_sockets.frontit(); !it.empty(); ++it)
					{
						Node& n = *it;
						n.pSock->Shutdown();
						n.pSock->Close();
					}
					m_listen_sockets.clear();
				}

				// Accepted sockets.
				{
					GAIA::SYNC::AutolockW al(m_rwAcceptedSockets);
					for(GAIA::CTN::Set<Node>::it it = m_accepted_sockets.frontit(); !it.empty(); ++it)
					{
						Node& n = *it;
						n.pSock->Shutdown();
						n.pSock->Close();
					}
					m_accepted_sockets.clear();
				}

				// Connected sockets.
				{
					GAIA::SYNC::AutolockW al(m_rwConnectedSockets);
					for(GAIA::CTN::Set<Node>::it it = m_connected_sockets.frontit(); !it.empty(); ++it)
					{
						Node& n = *it;
						n.pSock->Shutdown();
						n.pSock->Close();
					}
					m_connected_sockets.clear();
				}
			}

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsBegin() const
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			if(m_pIOCP != GNIL)
				return GAIA::True;
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			if(m_kqueue != GINVALID)
				return GAIA::True;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			if(m_epoll != GINVALID)
				return GAIA::True;
		#endif
			return GAIA::False;
		}

		GAIA::BL AsyncDispatcher::AddListenSocket(const GAIA::NETWORK::Addr& addr)
		{
			GAIA::SYNC::AutolockW al(m_rwListenSockets);

			Node nfinder;
			nfinder.addrLocal = addr;
			nfinder.pSock = GNIL;
			if(m_listen_sockets.find(nfinder) != GNIL)
				return GAIA::False;

			//
			AsyncSocket* pListenSocket = this->OnCreateListenSocket();
			pListenSocket->Create();
			pListenSocket->Bind(addr);

		#if GAIA_OS == GAIA_OS_WINDOWS
			this->attach_socket_iocp(*pListenSocket);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

			nfinder.pSock = pListenSocket;
			m_listen_sockets.insert(nfinder);

			pListenSocket->m_sock.Listen();

		#if GAIA_OS == GAIA_OS_WINDOWS
			for(GAIA::NUM x = 0; x < m_desc.sAcceptEventCount; ++x)
			{
				GAIA::NETWORK::AsyncSocket* pAcceptedSocket = this->OnCreateAcceptedSocket();
				pAcceptedSocket->Create();
				// TODO: Need attach to IOCP?

				GAIA::NETWORK::IOCPOverlapped* pIOCPOverlapped = this->alloc_iocpol();
				pIOCPOverlapped->type = IOCP_OVERLAPPED_TYPE_ACCEPT;
				pIOCPOverlapped->pListenSocket = pListenSocket;
				pIOCPOverlapped->pAcceptedSocket = pAcceptedSocket;
				pListenSocket->rise_ref();
				pAcceptedSocket->rise_ref();

				GAIA::N32 nAddrLen = sizeof(SOCKADDR_IN) + 16;
				DWORD dwRecved = 0;
				if(!((LPFN_ACCEPTEX)pListenSocket->m_pfnAcceptEx)(
					pListenSocket->GetFD(), pAcceptedSocket->GetFD(),
					pIOCPOverlapped->data, sizeof(pIOCPOverlapped->data) - nAddrLen - nAddrLen,
					nAddrLen, nAddrLen, &dwRecved, (OVERLAPPED*)pIOCPOverlapped))
				{
					DWORD err = WSAGetLastError();
					if(err != ERROR_IO_PENDING)
					{
						pListenSocket->drop_ref();
						pAcceptedSocket->drop_ref();
						pAcceptedSocket->drop_ref();
						this->release_iocpol(pIOCPOverlapped);
						GERR << "GAIA AsyncDispatcher IOCP error, cannot AcceptEx, ErrorCode = " << ::WSAGetLastError() << GEND;
					}
				}
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveListenSocket(const GAIA::NETWORK::Addr& addr)
		{
			GAIA::SYNC::AutolockW al(m_rwListenSockets);

			Node nfinder;
			nfinder.addrLocal = addr;
			nfinder.pSock = GNIL;
			Node* pFinded = m_listen_sockets.find(nfinder);
			if(pFinded == GNIL)
				return GAIA::False;

			pFinded->pSock->Shutdown();
			pFinded->pSock->Close();
			pFinded->pSock->drop_ref();

			m_listen_sockets.erase(nfinder);

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveListenSocketAll()
		{
			GAIA::SYNC::AutolockW al(m_rwListenSockets);
			if(m_listen_sockets.empty())
				return GAIA::False;

			for(GAIA::CTN::Set<Node>::it it = m_listen_sockets.frontit(); !it.empty(); ++it)
			{
				Node& n = *it;
				n.pSock->Shutdown();
				n.pSock->Close();
				n.pSock->drop_ref();
			}
			m_listen_sockets.clear();

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsExistListenSocket(const GAIA::NETWORK::Addr& addr) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwListenSockets);
			Node nfinder;
			nfinder.addrLocal = addr;
			nfinder.pSock = GNIL;
			const Node* pFinded = m_listen_sockets.find(nfinder);
			if(pFinded == GNIL)
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::NUM AsyncDispatcher::GetListenSocketCount() const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwListenSockets);
			return m_listen_sockets.size();
		}

		GAIA::BL AsyncDispatcher::CollectListenSocket(CallBack& cb) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwListenSockets);
			for(GAIA::CTN::Set<Node>::const_it it = m_listen_sockets.const_frontit(); !it.empty(); ++it)
			{
				const Node& n = *it;
				if(!cb.OnCollectAsyncSocket(*GCCAST(AsyncDispatcher*)(this), *n.pSock))
					return GAIA::False;
			}
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsExistAcceptedSocket(const GAIA::NETWORK::Addr& addr) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptedSockets);
			Node nfinder;
			nfinder.addrLocal = addr;
			nfinder.pSock = GNIL;
			const Node* pFinded  = m_accepted_sockets.find(nfinder);
			if(pFinded == GNIL)
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::NUM AsyncDispatcher::GetAcceptedSocketCount() const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptedSockets);
			return m_accepted_sockets.size();
		}

		GAIA::BL AsyncDispatcher::CollectAcceptedSocket(CallBack& cb) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwAcceptedSockets);
			for(GAIA::CTN::Set<Node>::const_it it = m_accepted_sockets.const_frontit(); !it.empty(); ++it)
			{
				const Node& n = *it;
				if(!cb.OnCollectAsyncSocket(*GCCAST(AsyncDispatcher*)(this), *n.pSock))
					return GAIA::False;
			}
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsExistConnectedSocket(const GAIA::NETWORK::Addr& addr) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwConnectedSockets);
			Node nfinder;
			nfinder.addrLocal = addr;
			nfinder.pSock = GNIL;
			const Node* pFinded  = m_connected_sockets.find(nfinder);
			if(pFinded == GNIL)
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::NUM AsyncDispatcher::GetConnectedSocketCount() const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwConnectedSockets);
			return m_connected_sockets.size();
		}

		GAIA::BL AsyncDispatcher::CollectConnectedSocket(CallBack& cb) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwConnectedSockets);
			for(GAIA::CTN::Set<Node>::const_it it = m_connected_sockets.const_frontit(); !it.empty(); ++it)
			{
				const Node& n = *it;
				if(!cb.OnCollectAsyncSocket(*GCCAST(AsyncDispatcher*)(this), *n.pSock))
					return GAIA::False;
			}
			return GAIA::True;
		}

		GAIA::GVOID AsyncDispatcher::init()
		{
			m_bCreated = GAIA::False;
			m_desc.reset();
		#if GAIA_OS == GAIA_OS_WINDOWS
			m_pIOCP = GNIL;
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			m_kqueue = GINVALID;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			m_epoll = GINVALID;
		#endif
		}

		GAIA::BL AsyncDispatcher::AddAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwAcceptedSockets);
			Node nfinder;
			GAIA::BL bResult = sock.GetLocalAddress(nfinder.addrLocal);
			GAST(bResult);
			nfinder.pSock = &sock;
			if(m_accepted_sockets.find(nfinder) != GNIL)
				return GAIA::False;
			m_accepted_sockets.insert(nfinder);
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwAcceptedSockets);
			Node nfinder;
			GAIA::BL bResult = sock.GetLocalAddress(nfinder.addrLocal);
			GAST(bResult);
			nfinder.pSock = &sock;
			if(!m_accepted_sockets.erase(nfinder))
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::AddConnectedSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwConnectedSockets);
			Node nfinder;
			GAIA::BL bResult = sock.GetLocalAddress(nfinder.addrLocal);
			GAST(bResult);
			nfinder.pSock = &sock;
			if(m_connected_sockets.find(nfinder) != GNIL)
				return GAIA::False;
			m_connected_sockets.insert(nfinder);
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveConnectedSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwConnectedSockets);
			Node nfinder;
			GAIA::BL bResult = sock.GetLocalAddress(nfinder.addrLocal);
			GAST(bResult);
			nfinder.pSock = &sock;
			if(!m_connected_sockets.erase(nfinder))
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::Execute()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			DWORD dwTrans = 0;
			GAIA::GVOID* pPointer = GNIL;
			GAIA::NETWORK::IOCPOverlapped* pOverlapped = GNIL;
			if(GetQueuedCompletionStatus(m_pIOCP, &dwTrans, (PULONG_PTR)&pPointer, (OVERLAPPED**)&pOverlapped, INFINITE))
			{
				if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_ACCEPT)
				{
					// Dispatch.
					// TODO: Need attach to IOCP?
					GAIA::N32 nAddrLen = sizeof(SOCKADDR_IN) + 16;
					sockaddr_in* saddr_peer = (sockaddr_in*)(pOverlapped->data + nAddrLen);
					GAIA::NETWORK::Addr addrPeer;
					GAIA::NETWORK::saddr2addr(saddr_peer, addrPeer);
					pOverlapped->pAcceptedSocket->SetPeerAddress(addrPeer);
					GAIA::NETWORK::Addr addrListen;
					pOverlapped->pListenSocket->GetBindedAddress(addrListen);
					pOverlapped->pAcceptedSocket->OnAccepted(GAIA::True, addrListen);
					this->OnAcceptSocket(*pOverlapped->pAcceptedSocket, addrListen);

					// Re-request.
					// TODO:
				}
				else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_CONNECT)
				{
					GAIA::NETWORK::Addr addrPeer;
					pOverlapped->pAcceptedSocket->GetPeerAddress(addrPeer);
					pOverlapped->pAcceptedSocket->OnConnected(GAIA::True, addrPeer);
				}
				else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_DISCONNECT)
				{
					pOverlapped->pAcceptedSocket->OnDisconnected(GAIA::True);
					pOverlapped->pAcceptedSocket->SwapBrokenState();
				}
				else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_SEND)
				{
					// TODO: What is pPointer be used for?
					pOverlapped->pAcceptedSocket->OnSent(GAIA::True, pOverlapped->data, dwTrans, pOverlapped->buf.len);
				}
				else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_RECV)
				{
					// Dispatch.
					// TODO: What is pPointer be used for?
					pOverlapped->pAcceptedSocket->OnRecved(GAIA::True, pOverlapped->data, dwTrans);

					// Re-request.
					// TODO:
				}
				else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_STOP)
				{
					GAST(pOverlapped->pListenSocket == GNIL);
					GAST(pOverlapped->pAcceptedSocket = GNIL);
					GAIA::SYNC::Autolock al(m_lrIOCPOLPool);
					m_IOCPOLPool.release(pOverlapped);
					return GAIA::False;
				}
				else
					GASTFALSE;
			}
			else
			{
				if(pOverlapped != GNIL && pOverlapped->pAcceptedSocket->SwapBrokenState())
				{
					if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_RECV)
					{
						if(dwTrans == 0)
						{
							DWORD dwError = WSAGetLastError();
							if(dwError == ERROR_NETNAME_DELETED)
							{
								DWORD dwTrans, dwFlags;
								if(WSAGetOverlappedResult(pOverlapped->pAcceptedSocket->GetFD(), 
									(WSAOVERLAPPED*)pOverlapped, &dwTrans, GAIA::False, &dwFlags))
									dwError = WSAGetLastError();

								if(IsIOCPDisconnected(dwError)) // TODO: Another place to call IsIOCPDisconnected.
									pOverlapped->pAcceptedSocket->OnDisconnected(GAIA::True);
							}
							else
							{
								// TODO:
							}
						}
						else
						{
							// TODO:
						}
					}
					else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_SEND)
					{
						pOverlapped->pAcceptedSocket->OnSent(GAIA::False, pOverlapped->data, dwTrans, pOverlapped->_buf.len);
						pOverlapped->pAcceptedSocket->OnDisconnected(GAIA::True);
					}
					else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_CONNECT)
					{
						GAIA::NETWORK::Addr addrPeer;
						pOverlapped->pAcceptedSocket->GetPeerAddress(addrPeer);
						pOverlapped->pAcceptedSocket->OnConnected(GAIA::False, addrPeer);
					}
					else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_DISCONNECT)
					{
						pOverlapped->pAcceptedSocket->OnDisconnected(GAIA::False);
					}
					else if(pOverlapped->type == GAIA::NETWORK::IOCP_OVERLAPPED_TYPE_ACCEPT)
					{
						GAIA::NETWORK::Addr addrBinded;
						pOverlapped->pListenSocket->GetBindedAddress(addrBinded);
						pOverlapped->pAcceptedSocket->OnAccepted(GAIA::False, addrBinded);
					}
					else
						GASTFALSE;
				}
				else
				{
					// TODO:
				}
			}
			if(pOverlapped != GNIL)
			{
				if(pOverlapped->pListenSocket != GNIL)
					pOverlapped->pListenSocket->drop_ref();
				if(pOverlapped->pAcceptedSocket != GNIL)
					pOverlapped->pAcceptedSocket->drop_ref();
				GAIA::SYNC::Autolock al(m_lrIOCPOLPool);
				m_IOCPOLPool.release(pOverlapped);
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
			return GAIA::True;
		}

	#if GAIA_OS == GAIA_OS_WINDOWS
		GAIA::NETWORK::IOCPOverlapped* AsyncDispatcher::alloc_iocpol()
		{
			GAIA::SYNC::Autolock al(m_lrIOCPOLPool);
			GAIA::NETWORK::IOCPOverlapped* pRet = m_IOCPOLPool.alloc();
			zeromem(&pRet->_ovlp);
			pRet->type = IOCP_OVERLAPPED_TYPE_INVALID;
			pRet->pListenSocket = GNIL;
			pRet->pAcceptedSocket = GNIL;
			pRet->_buf.len = 0;
			pRet->_buf.buf = (GAIA::CH*)pRet->data;
			return pRet;
		}

		GAIA::GVOID AsyncDispatcher::release_iocpol(GAIA::NETWORK::IOCPOverlapped* pIOCPOverlapped)
		{
			GAST(pIOCPOverlapped != GNIL);
			GAIA::SYNC::Autolock al(m_lrIOCPOLPool);
			return m_IOCPOLPool.release(pIOCPOverlapped);
		}

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
	#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

	#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

	#endif
	}
}
