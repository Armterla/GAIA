#ifndef		__GAIA_NETWORK_ASYNC_SOCKET_H__
#define		__GAIA_NETWORK_ASYNC_SOCKET_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
#include "gaia_network_base.h"
#include "gaia_network_socket.h"

namespace GAIA
{
	namespace NETWORK
	{
		#if GAIA_OS == GAIA_OS_WINDOWS
			#define IODATA_MAXLEN 2000 // SO_MAX_MSG_SIZE
			GAIA_ENUM_BEGIN(IOCP_OVERLAPPED_TYPE)
				IOCP_OVERLAPPED_TYPE_STOP,
				IOCP_OVERLAPPED_TYPE_CONNECT,
				IOCP_OVERLAPPED_TYPE_DISCONNECT,
				IOCP_OVERLAPPED_TYPE_ACCEPT,
				IOCP_OVERLAPPED_TYPE_SEND,
				IOCP_OVERLAPPED_TYPE_RECV,
			GAIA_ENUM_END(IOCP_OVERLAPPED_TYPE)
			class IOCPOverlapped : public GAIA::Base
			{
			public:
				OVERLAPPED _ovlp;
				IOCP_OVERLAPPED_TYPE type;
				AsyncSocket* pListenSocket;
				AsyncSocket* pAcceptedSocket;
				WSABUF _buf;
				GAIA::U8 data[IODATA_MAXLEN];
			};
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif


		/*!
			@brief Async socket.

				This async socket class support stream socket only(TCP).
		*/
		class AsyncSocket : public GAIA::RefObject
		{
			friend class AsyncDispatcher;

		public:
			GAIA_ENUM_BEGIN(ASYNC_SOCKET_TYPE)
				ASYNC_SOCKET_TYPE_CONNECTED,
				ASYNC_SOCKET_TYPE_ACCEPTED,
				ASYNC_SOCKET_TYPE_LISTEN,
			GAIA_ENUM_END(ASYNC_SOCKET_TYPE)

		public:

			/*!
				@brief Constructor.
			*/
			AsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE socktype = GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE_CONNECTED);

			/*!
				@brief Destructor.

				@remarks
					If current async socket is created, it will be close automatically.
			*/
			~AsyncSocket();

			/*!
				@brief Get socket type.
			*/
			GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE GetAsyncSocketType() const;

			/*!
				@brief Create async socket.

				@exception
					GAIA::ECT::EctIllegal If async socket is created.

				@remarks
					This function is async call.
			*/
			GAIA::GVOID Create();

			/*!
				@brief Close async socket.

				@remarks
					This function is async call.
			*/
			GAIA::GVOID Close();

			/*!
				@brief Shutdown current async socket.

				@remarks
					This function is async call.
			*/
			GAIA::GVOID Shutdown(GAIA::N32 nShutdownFlag = GAIA::NETWORK::Socket::SSDF_RECV | GAIA::NETWORK::Socket::SSDF_SEND);

			/*!
				@brief Check current socket is create or not.
			*/
			GAIA::BL IsCreated() const;

			/*!
				@brief Bind async socket to a network address, include IP and port.

				@remarks
					This function is async call.
			*/
			GAIA::GVOID Bind(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Check the socket is bound or not.

				@return If the socket is bound, return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL IsBinded() const;

			/*!
				@brief Connect async socket to a network address, include IP and port.

				@remarks
					This function is async call.
			*/
			GAIA::GVOID Connect(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Disconnect from a network address.

				@remarks
					This function is async call.
			*/
			GAIA::GVOID Disconnect();

			/*!
				@brief Check current async socket is connected or not.
			*/
			GAIA::BL IsConnected() const;

			/*!
				@brief Send data to peer.

				@remarks
					This function is async call.
			*/
			GAIA::GVOID Send(const GAIA::GVOID* pData, GAIA::NUM sSize);

			/*!
				@brief Get socket file descriptor.
			*/
			GAIA::N32 GetFD() const;

			/*!
				@brief Get socket's global address.

				@return
					If success return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL GetGlobalAddress(GAIA::NETWORK::Addr& addr);

			/*!
				@brief Get socket's local address.

				@return
					If success return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL GetLocalAddress(GAIA::NETWORK::Addr& addr);

			/*!
				@brief Get socket's peer address.

				@return
					If success return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL GetPeerAddress(GAIA::NETWORK::Addr& addr);

		protected:

			/*!
				@brief On async socket create callback.
			*/
			virtual GAIA::GVOID OnCreated(GAIA::BL bResult){}

			/*!
				@brief On async socket close callback.
			*/
			virtual GAIA::GVOID OnClosed(GAIA::BL bResult){}

			/*!
				@brief On async socket binded callback.
			*/
			virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}

			/*!
				@brief On async socket connected callback.
			*/
			virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}

			/*!
				@brief On async socket disconnected callback.
			*/
			virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult){}

			/*!
				@brief On async socket started listen.
			*/
			virtual GAIA::GVOID OnListened(GAIA::BL bResult){}

			/*!
				@brief On async socket be accepted by a listen socket.
			*/
			virtual GAIA::GVOID OnAccepted(GAIA::BL bResult, const GAIA::NETWORK::Addr& addrListen){}

			/*!
				@brief On async socket sent callback.
			*/
			virtual GAIA::GVOID OnSent(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::NUM sPracticeSize, GAIA::NUM sSize){}

			/*!
				@brief On async socket recv callback.
			*/
			virtual GAIA::GVOID OnRecved(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::NUM sSize){}

			/*!
				@brief On async socket shutdown callback.
			*/
			virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag){}

		private:
			GAIA::GVOID init();
			GAIA::GVOID Recv();
			GAIA::GVOID SetPeerAddress(GAIA::NETWORK::Addr& addr);

		private:
			GAIA::NETWORK::AsyncDispatcher* m_pDispatcher;
			GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE m_socktype;
			GAIA::NETWORK::Socket m_sock;
			GAIA::SYNC::Lock m_lrSend;
			GAIA::SYNC::Lock m_lrRecv;

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::GVOID* m_pfnAcceptEx;
			GAIA::GVOID* m_pfnConnectEx;
			GAIA::GVOID* m_pfnDisconnectEx;
		#endif
		};
	}
}

#endif
