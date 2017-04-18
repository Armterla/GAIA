#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_base.h>
#include <gaia_network_asyncsocket.h>
#include <gaia_network_asyncdispatcher.h>

#include <gaia_type_impl.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#include <gaia_network_base_impl.h>
#include <gaia_network_socket_impl.h>

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <ws2tcpip.h>
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
		AsyncSocket::AsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::ASYNC_SOCKET_TYPE socktype)
		{
			this->init();
			m_pDispatcher = &disp;
			m_socktype = socktype;

			if(m_socktype == GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTING)
				m_pDispatcher->AddAcceptingSocket(*this);
			else if(m_socktype == GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTED)
				m_pDispatcher->AddAcceptedSocket(*this);
			else if(m_socktype == GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED)
				m_pDispatcher->AddConnectedSocket(*this);
		}

		AsyncSocket::~AsyncSocket()
		{
			//
			GTRY
			{
				if(this->IsCreated())
					this->Close();
			}
			GCATCH(Network)
			{
				e.SetDispatched(GAIA::True);
			}

			//
		#if GAIA_OS != GAIA_OS_WINDOWS
			if(m_pReadAsyncCtx != GNIL && m_pReadAsyncCtx->type == GAIA::NETWORK::ASYNC_CONTEXT_TYPE_RECV)
				m_pDispatcher->pop_for_recycle(*this);
		#endif

			//
			if(m_socktype == GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTING)
				m_pDispatcher->RemoveAcceptingSocket(*this);
			else if(m_socktype == GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTED)
				m_pDispatcher->RemoveAcceptedSocket(*this);
			else if(m_socktype == GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED)
				m_pDispatcher->RemoveConnectedSocket(*this);

			//
		#if GAIA_OS != GAIA_OS_WINDOWS
			if(m_pReadAsyncCtx != GNIL)
			{
				m_pDispatcher->release_async_ctx(m_pReadAsyncCtx);
				m_pReadAsyncCtx = GNIL;
			}
			if(m_pWriteAsyncCtx != GNIL)
			{
				m_pDispatcher->release_async_ctx(m_pWriteAsyncCtx);
				m_pWriteAsyncCtx = GNIL;
			}
		#endif
		}

		GAIA::GVOID AsyncSocket::Create()
		{
			if(this->IsCreated())
				GTHROW(Illegal);

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::N32 nSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, GNIL, 0, WSA_FLAG_OVERLAPPED);
			GAST(nSocket <= GAIA::N32MAX);
			if(nSocket == INVALID_SOCKET)
			{
				this->OnCreated(GAIA::False);
				return;
			}
			m_sock.SetFD(nSocket);
			m_sock.SetType(GAIA::NETWORK::Socket::SOCKET_TYPE_STREAM);
			DWORD dwBytes;
			GUID GuidAcceptEx = WSAID_ACCEPTEX;
			GUID GuidConnectEx = WSAID_CONNECTEX;
			GUID GuidDisonnectEx = WSAID_DISCONNECTEX;
			WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
					 &GuidAcceptEx, sizeof(GuidAcceptEx), 
					 &m_pfnAcceptEx, sizeof(m_pfnAcceptEx), 
					 &dwBytes, GNIL, GNIL);
			WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
					 &GuidConnectEx, sizeof(GuidConnectEx), 
					 &m_pfnConnectEx, sizeof(m_pfnConnectEx), 
					 &dwBytes, GNIL, GNIL);
			WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
					 &GuidDisonnectEx, sizeof(GuidDisonnectEx), 
					 &m_pfnDisconnectEx, sizeof(m_pfnDisconnectEx), 
					 &dwBytes, GNIL, GNIL);
		#else
			m_sock.Create(GAIA::NETWORK::Socket::SOCKET_TYPE_STREAM);
			m_nBackupSocket = m_sock.GetFD();
		#endif

			this->OnCreated(GAIA::True);
		}

		GAIA::GVOID AsyncSocket::Close()
		{
			if(this->IsConnected())
			{
				this->SetConnected(GAIA::False);
				this->OnDisconnected(GAIA::True, GAIA::False);
			}
		#if GAIA_OS != GAIA_OS_WINDOWS
			m_pDispatcher->push_for_recycle(*this);
		#endif
			m_sock.Close();
			this->OnClosed(GAIA::True);
		}

		GAIA::GVOID AsyncSocket::Shutdown(GAIA::N32 nShutdownFlag)
		{
			if(this->IsConnected())
			{
				this->SetConnected(GAIA::False);
				this->OnDisconnected(GAIA::True, GAIA::False);
			}
		#if GAIA_OS != GAIA_OS_WINDOWS
			m_pDispatcher->push_for_recycle(*this);
		#endif
			m_sock.Shutdown(nShutdownFlag);
			this->OnShutdowned(GAIA::True, nShutdownFlag);
		}

		GAIA::GVOID AsyncSocket::Bind(const GAIA::NETWORK::Addr& addr)
		{
			m_sock.Bind(addr);
			this->OnBound(GAIA::True, addr);
		}

		GAIA::GVOID AsyncSocket::Connect(const GAIA::NETWORK::Addr& addr)
		{
			if(!addr.check())
				GTHROW(InvalidParam);

			if(!this->IsCreated())
				GTHROW(Illegal);
			if(this->IsConnected())
				GTHROW(Illegal);

			GAIA::NETWORK::Addr addrPeer;
			addrPeer.reset();
			this->GetPeerAddress(addrPeer);
			if(addrPeer.check())
				return;

			this->SetPeerAddress(addr);

		#if GAIA_OS == GAIA_OS_WINDOWS
			if(!this->IsBinded())
			{
				GAIA::NETWORK::Addr addrSelf;
				addrSelf.reset();
				this->Bind(addrSelf);
			}
			m_pDispatcher->attach_socket_iocp(*this);

			AsyncContext* pCtx = m_pDispatcher->alloc_async_ctx();
			pCtx->type = ASYNC_CONTEXT_TYPE_CONNECT;
			pCtx->pDataSocket = this;
			this->rise_ref();

			sockaddr_in saddr_in;
			zeromem(&saddr_in);
			GAIA::NETWORK::addr2saddr(addr, &saddr_in, AF_INET);
			GAIA::N32 nAddrLen = sizeof(sockaddr_in);

			DWORD dwSent;
			if(!((LPFN_CONNECTEX)m_pfnConnectEx)(this->GetFD(), (SOCKADDR*)&saddr_in, nAddrLen, GNIL, 0, &dwSent, (OVERLAPPED*)pCtx))
			{
				DWORD err = WSAGetLastError();
				if(err != ERROR_IO_PENDING)
				{
					m_pDispatcher->release_async_ctx(pCtx);
					this->OnConnected(GAIA::False, addr);
					this->drop_ref();
					GERR << "GAIA AsyncSocket IOCP error, cannot ConnectEx, ErrorCode = " << ::WSAGetLastError() << GEND;
					return;
				}
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			AsyncContext* pCtx = m_pDispatcher->alloc_async_ctx();
			pCtx->pSocket = this;
			pCtx->type = GAIA::NETWORK::ASYNC_CONTEXT_TYPE_CONNECT;
			GAST(m_pWriteAsyncCtx == GNIL);
			m_pWriteAsyncCtx = pCtx;

			GAIA::N32 kqep = m_pDispatcher->select_kqep(this->GetFD());

			struct kevent ke;
			EV_SET(&ke, this->GetFD(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, pCtx);
			GAIA::NUM sResult = kevent(kqep, &ke, 1, GNIL, 0, GNIL);
			if(sResult == GINVALID)
			{
				if(m_pDispatcher->IsEnableLog())
				{
					GAIA::N32 nErr = errno;
					GERR << "[AsyncSocket] AsyncSocket::Connect: kevent call failed, errno = " << nErr << GEND;
				}
			}

			m_sock.Connect(addr);
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Disconnect()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			AsyncContext* pCtx = m_pDispatcher->alloc_async_ctx();
			pCtx->type = ASYNC_CONTEXT_TYPE_DISCONNECT;
			pCtx->pDataSocket = this;
			this->rise_ref();

			if(!((LPFN_DISCONNECTEX)m_pfnDisconnectEx)(this->GetFD(), (OVERLAPPED*)pCtx, 0, 0))
			{
				DWORD err = WSAGetLastError();
				if(err != ERROR_IO_PENDING)
				{
					m_pDispatcher->release_async_ctx(pCtx);
					this->OnDisconnected(GAIA::False, GAIA::False);
					this->drop_ref();
					GERR << "GAIA AsyncSocket IOCP error, cannot DisconnectEx, ErrorCode = " << ::WSAGetLastError() << GEND;
					return;
				}
			}
		#else
			this->Shutdown();
		#endif
		}

		GAIA::N32 AsyncSocket::Send(const GAIA::GVOID* p, GAIA::N32 nSize)
		{
			if(p == GNIL)
				GTHROW(InvalidParam);
			if(nSize <= 0)
				GTHROW(InvalidParam);

			if(!this->IsCreated())
				GTHROW(Illegal);

			GAIA::SYNC::Autolock al(m_lrSend);

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::NUM sPieceSize = gsizeofobj(AsyncContext, data);
			GAIA::NUM sPieceCount = nSize / sPieceSize + ((nSize % sPieceSize) != 0 ? 1 : 0);
			GAIA::NUM sOffset = 0;
			for(GAIA::NUM x = 0; x < sPieceCount; ++x)
			{
				GAIA::NUM sCurrentPieceSize = GAIA::ALGO::gmin(nSize - sOffset, sPieceSize);
				GAST(sCurrentPieceSize != 0);

				AsyncContext* pCtx = m_pDispatcher->alloc_async_ctx();
				pCtx->type = ASYNC_CONTEXT_TYPE_SEND;
				pCtx->pDataSocket = this;
				GAIA::ALGO::gmemcpy(pCtx->data, (const GAIA::U8*)p + sOffset, sCurrentPieceSize);
				pCtx->_buf.len = sCurrentPieceSize;
				this->rise_ref();

				DWORD dwTrans = 0;
				DWORD dwFlag = 0;
				if(::WSASend(this->GetFD(), &pCtx->_buf, 1, &dwTrans, dwFlag, (OVERLAPPED*)pCtx, GNIL) != 0)
				{
					DWORD err = WSAGetLastError();
					if(err != ERROR_IO_PENDING)
					{
						this->OnSent(GAIA::False, p, sOffset, nSize);
						m_pDispatcher->release_async_ctx(pCtx);
						this->drop_ref();
						GERR << "GAIA AsyncSocket IOCP error, cannot WSASend, ErrorCode = " << ::WSAGetLastError() << GEND;
						return sOffset;
					}
				}

				sOffset += sCurrentPieceSize;
			}

			GAST(sOffset == nSize);
			return sOffset;
		#else
			m_sendbuf.write(p, nSize);
			GAIA::N32 kqep = m_pDispatcher->select_kqep(this->GetFD());
		#	if GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
				GAST(m_pWriteAsyncCtx != GNIL);
				struct kevent ke;
				EV_SET(&ke, this->GetFD(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, m_pWriteAsyncCtx);
				GAIA::NUM sResult = kevent(kqep, &ke, 1, GNIL, 0, GNIL);
				if(sResult == GINVALID)
				{
					if(m_pDispatcher->IsEnableLog())
					{
						GAIA::N32 nErr = errno;
						GERR << "[AsyncSocket] AsyncSocket::Send: kevent call failed, errno = " << nErr << GEND;
					}
				}
		#	elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#	endif
		#endif
			return nSize;
		}

		GAIA::GVOID AsyncSocket::init()
		{
			m_pDispatcher = GNIL;
			m_socktype = GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED;

		#if GAIA_OS == GAIA_OS_WINDOWS
			m_pfnAcceptEx = GNIL;
			m_pfnConnectEx = GNIL;
			m_pfnDisconnectEx = GNIL;
		#else
			m_pReadAsyncCtx = GNIL;
			m_pWriteAsyncCtx = GNIL;
			m_uRecycleTime = 0;
			m_nBackupSocket = GINVALID;
			m_bPushForRecycleAble = GAIA::True;
		#endif
		}
	}
}
