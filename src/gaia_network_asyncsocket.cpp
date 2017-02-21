#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_base.h>
#include <gaia_network_asyncsocket.h>
#include <gaia_network_asyncdispatcher.h>

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

#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

#endif

namespace GAIA
{
	namespace NETWORK
	{
		AsyncSocket::AsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE socktype)
		{
			this->init();
			m_pDispatcher = &disp;
			m_socktype = socktype;

			if(m_socktype == GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE_ACCEPTED)
				m_pDispatcher->AddAcceptedSocket(*this);
			else if(m_socktype == m_socktype == GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE_CONNECTED)
				m_pDispatcher->AddConnectedSocket(*this);
		}

		AsyncSocket::~AsyncSocket()
		{
			if(m_socktype == GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE_ACCEPTED)
				m_pDispatcher->RemoveAcceptedSocket(*this);
			else if(m_socktype == m_socktype == GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE_CONNECTED)
				m_pDispatcher->RemoveConnectedSocket(*this);
		}

		GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE AsyncSocket::GetAsyncSocketType() const
		{
			return m_socktype;
		}

		GAIA::GVOID AsyncSocket::Create()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::N32 nSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
			if(nSocket == INVALID_SOCKET)
			{
				this->OnCreated(GAIA::False);
				return;
			}
			m_sock.SetFD(nSocket);
			DWORD dwBytes;
			GUID GuidAcceptEx = WSAID_ACCEPTEX;
			GUID GuidConnectEx = WSAID_CONNECTEX;
			GUID GuidDisonnectEx = WSAID_DISCONNECTEX;
			WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
					 &GuidAcceptEx, sizeof(GuidAcceptEx), 
					 &m_pfnAcceptEx, sizeof(m_pfnAcceptEx), 
					 &dwBytes, NULL, NULL);
			WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
					 &GuidConnectEx, sizeof(GuidConnectEx), 
					 &m_pfnConnectEx, sizeof(m_pfnConnectEx), 
					 &dwBytes, NULL, NULL);
			WSAIoctl(nSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
					 &GuidDisonnectEx, sizeof(GuidDisonnectEx), 
					 &m_pfnDisconnectEx, sizeof(m_pfnDisconnectEx), 
					 &dwBytes, NULL, NULL);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			m_sock.Create(GAIA::NETWORK::Socket::SOCKET_TYPE_STREAM);
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			m_sock.Create(GAIA::NETWORK::Socket::SOCKET_TYPE_STREAM);
		#endif
			this->OnCreated(GAIA::True);
		}

		GAIA::GVOID AsyncSocket::Close()
		{
			m_sock.Close();
			this->OnClosed(GAIA::True);
		}

		GAIA::GVOID AsyncSocket::Shutdown(GAIA::N32 nShutdownFlag)
		{
			m_sock.Shutdown(nShutdownFlag);
			this->OnShutdowned(GAIA::True, nShutdownFlag);
		}

		GAIA::BL AsyncSocket::IsCreated() const
		{
			return m_sock.IsCreated();
		}

		GAIA::GVOID AsyncSocket::Bind(const GAIA::NETWORK::Addr& addr)
		{
			m_sock.Bind(addr);
			this->OnBound(GAIA::True, addr);
		}

		GAIA::BL AsyncSocket::IsBinded() const
		{
			return m_sock.IsBinded();
		}

		GAIA::GVOID AsyncSocket::Connect(const GAIA::NETWORK::Addr& addr)
		{
			GAST(addr.check());
			if(!addr.check())
				return;
			GAIA::NETWORK::Addr addrPeer;
			addrPeer.reset();
			this->GetPeerAddress(addrPeer);
			if(addrPeer.check())
				return;

			this->SetPeerAddress(addr);

		#if GAIA_OS == GAIA_OS_WINDOWS
			IOCPOverlapped* pIOCPOverlapped = m_pDispatcher->alloc_iocpol();
			pIOCPOverlapped->type = IOCP_OVERLAPPED_TYPE_CONNECT;
			pIOCPOverlapped->pAcceptedSocket = this;
			this->rise_ref();

			sockaddr_in saddr_in;
			GAIA::NETWORK::addr2saddr(addr, &saddr_in);
			GAIA::N32 nAddrLen = sizeof(SOCKADDR_IN) + 16;
			DWORD dwSent;
			if(!((LPFN_CONNECTEX)m_pfnConnectEx)(this->GetFD(), (sockaddr*)&saddr_in, nAddrLen, GNIL, 0, &dwSent, (OVERLAPPED*)pIOCPOverlapped))
			{
				// TODO:
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Disconnect()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			IOCPOverlapped* pIOCPOverlapped = m_pDispatcher->alloc_iocpol();
			pIOCPOverlapped->type = IOCP_OVERLAPPED_TYPE_DISCONNECT;
			pIOCPOverlapped->pAcceptedSocket = this;
			this->rise_ref();

			if(!((LPFN_DISCONNECTEX)m_pfnDisconnectEx)(this->GetFD(), (OVERLAPPED*)pIOCPOverlapped, 0, 0))
			{
				// TODO:
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::BL AsyncSocket::IsConnected() const
		{
			return m_sock.IsConnected();
		}

		GAIA::GVOID AsyncSocket::Send(const GAIA::GVOID* pData, GAIA::NUM sSize)
		{
			GAIA::SYNC::Autolock al(m_lrSend);

		#if GAIA_OS == GAIA_OS_WINDOWS
			IOCPOverlapped* pIOCPOverlapped = m_pDispatcher->alloc_iocpol();
			pIOCPOverlapped->type = IOCP_OVERLAPPED_TYPE_SEND;
			pIOCPOverlapped->pAcceptedSocket = this;
			pIOCPOverlapped->_buf.len = sizeof(pIOCPOverlapped->data);
			this->rise_ref();

			DWORD dwTrans = 0;
			DWORD dwFlag = 0;
			if(!::WSARecv(this->GetFD(), &pIOCPOverlapped->_buf, 1, &dwTrans, &dwFlag, (OVERLAPPED*)pIOCPOverlapped, GNIL))
			{
				DWORD err = WSAGetLastError();
				if(err != ERROR_IO_PENDING)
				{
					m_pDispatcher->release_iocpol(pIOCPOverlapped);
					this->drop_ref();
				}
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::N32 AsyncSocket::GetFD() const
		{
			return m_sock.GetFD();
		}

		GAIA::BL AsyncSocket::GetBindedAddress(GAIA::NETWORK::Addr& addr)
		{
			return m_sock.GetBindedAddress(addr);
		}

		GAIA::BL AsyncSocket::GetGlobalAddress(GAIA::NETWORK::Addr& addr)
		{
			return m_sock.GetGlobalAddress(addr);
		}

		GAIA::BL AsyncSocket::GetLocalAddress(GAIA::NETWORK::Addr& addr)
		{
			return m_sock.GetLocalAddress(addr);
		}

		GAIA::BL AsyncSocket::GetPeerAddress(GAIA::NETWORK::Addr& addr)
		{
			return m_sock.GetPeerAddress(addr);
		}

		GAIA::GVOID AsyncSocket::init()
		{
			m_pDispatcher = GNIL;
			m_socktype = GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE_CONNECTED;

		#if GAIA_OS == GAIA_OS_WINDOWS
			m_pfnAcceptEx = GNIL;
			m_pfnConnectEx = GNIL;
			m_pfnDisconnectEx = GNIL;
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Recv()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			IOCPOverlapped* pIOCPOverlapped = m_pDispatcher->alloc_iocpol();
			pIOCPOverlapped->type = IOCP_OVERLAPPED_TYPE_RECV;
			pIOCPOverlapped->pAcceptedSocket = this;
			pIOCPOverlapped->_buf.len = sizeof(pIOCPOverlapped->data);
			this->rise_ref();

			DWORD dwTrans = 0;
			DWORD dwFlag = 0;
			if(!::WSARecv(this->GetFD(), &pIOCPOverlapped->_buf, 1, &dwTrans, &dwFlag, (OVERLAPPED*)pIOCPOverlapped, GNIL))
			{
				DWORD err = WSAGetLastError();
				if(err != ERROR_IO_PENDING)
				{
					m_pDispatcher->release_iocpol(pIOCPOverlapped);
					this->drop_ref();
				}
			}
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::SetPeerAddress(const GAIA::NETWORK::Addr& addr)
		{
			m_sock.SetPeerAddress(addr);
		}
	}
}
