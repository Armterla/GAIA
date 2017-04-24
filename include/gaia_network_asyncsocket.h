#ifndef		__GAIA_NETWORK_ASYNC_SOCKET_H__
#define		__GAIA_NETWORK_ASYNC_SOCKET_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_atomic.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_ctn_bufferrw.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
#include "gaia_network_base.h"
#include "gaia_network_socket.h"

namespace GAIA
{
	namespace NETWORK
	{
		/*!
			@brief Async context type.
		*/
		GAIA_ENUM_BEGIN(ASYNC_CONTEXT_TYPE)
			ASYNC_CONTEXT_TYPE_STOP,
			ASYNC_CONTEXT_TYPE_CONNECT,
			ASYNC_CONTEXT_TYPE_DISCONNECT,
			ASYNC_CONTEXT_TYPE_ACCEPT,
			ASYNC_CONTEXT_TYPE_SEND,
			ASYNC_CONTEXT_TYPE_RECV,
		GAIA_ENUM_END(ASYNC_CONTEXT_TYPE)

		/*!
			@brief Async context.
		*/
		class AsyncContext : public GAIA::Base
		{
		public:
		#if GAIA_OS == GAIA_OS_WINDOWS
			OVERLAPPED _ovlp;
			WSABUF _buf;
			GAIA::NETWORK::AsyncSocket* pListenSocket;
			GAIA::NETWORK::AsyncSocket* pDataSocket;
			GAIA::U8 data[2000];
		#else
			GAIA::NETWORK::AsyncSocket* pSocket;
		#endif
			GAIA::NETWORK::ASYNC_CONTEXT_TYPE type;
		};

		/*!
			@brief Async socket type.
		*/
		GAIA_ENUM_BEGIN(ASYNC_SOCKET_TYPE)
			ASYNC_SOCKET_TYPE_CONNECTED,
			ASYNC_SOCKET_TYPE_ACCEPTING,
			ASYNC_SOCKET_TYPE_ACCEPTED,
			ASYNC_SOCKET_TYPE_LISTEN,
		GAIA_ENUM_END(ASYNC_SOCKET_TYPE)

		/*!
			@brief TCP Async socket.

				This async socket class support stream socket only(TCP).
		*/
		class AsyncSocket : public GAIA::RefObject
		{
			friend class AsyncDispatcher;

		public:

			/*!
				@brief Constructor.
			*/
			AsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::ASYNC_SOCKET_TYPE socktype = GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED);

			/*!
				@brief Destructor.

				@remarks
					If current async socket is created, it will be close automatically.
			*/
			virtual ~AsyncSocket();

			/*!
				@brief Get AsyncDispatcher.

				@return Return current AsyncSocket's AsyncDispatcher.
			*/
			GAIA::NETWORK::AsyncDispatcher& GetDispatcher() const{return *m_pDispatcher;}

			/*!
				@brief Get socket type.
			*/
			GAIA::NETWORK::ASYNC_SOCKET_TYPE GetAsyncSocketType() const{return m_socktype;}

			/*!
				@brief Create async socket.

				@exception
					GAIA::ECT::EctIllegal If async socket is created.

				@remarks
					This function is async call.
			*/
			virtual GAIA::GVOID Create();

			/*!
				@brief Close async socket.

				@remarks
					This function is async call.
			*/
			virtual GAIA::GVOID Close();

			/*!
				@brief Shutdown current async socket.

				@remarks
					This function is async call.
			*/
			virtual GAIA::GVOID Shutdown(GAIA::N32 nShutdownFlag = GAIA::NETWORK::Socket::SSDF_RECV | GAIA::NETWORK::Socket::SSDF_SEND);

			/*!
				@brief Check current socket is create or not.
			*/
			GAIA::BL IsCreated() const{return m_sock.IsCreated();}

			/*!
				@brief Set socket option.

				@exception
					GAIA::ECT::EctInvalidParam If parameter op is a invalid enum.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If set socket option failed.
			*/
			virtual GAIA::GVOID SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, const GAIA::CTN::Vari& v){return m_sock.SetOption(op, v);}

			/*!
				@brief Get socket option.

				@exception
					GAIA::ECT::EctInvalidParam If parameter op is a invalid enum.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.
			*/
			virtual GAIA::GVOID GetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, GAIA::CTN::Vari& v){return m_sock.GetOption(op, v);}

			/*!
				@brief Bind async socket to a network address, include IP and port.

				@remarks
					This function is async call.
			*/
			virtual GAIA::GVOID Bind(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Check the socket is bound or not.

				@return If the socket is bound, return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL IsBinded() const{return m_sock.IsBinded();}

			/*!
				@brief Connect async socket to a network address, include IP and port.

				@remarks
					This function is async call.
			*/
			virtual GAIA::GVOID Connect(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Disconnect from a network address.

				@remarks
					This function is async call.
			*/
			virtual GAIA::GVOID Disconnect();

			/*!
				@brief Check current async socket is connected or not.
			*/
			GAIA::BL IsConnected() const{return m_sock.IsConnected();}

			/*!
				@brief Send data to peer.

				@remarks
					This function is async call.
			*/
			virtual GAIA::N32 Send(const GAIA::GVOID* p, GAIA::N32 nSize);

			/*!
				@brief Get socket file descriptor.
			*/
			GAIA::N32 GetFD() const{return m_sock.GetFD();}

			/*!
				@brief Get socket's bind address.

				@return
					If success return GAIA::True, or return GAIA::False;
			*/
			GAIA::BL GetBindedAddress(GAIA::NETWORK::Addr& addr){return m_sock.GetBindedAddress(addr);}

			/*!
				@brief Get socket's global address.

				@return
					If success return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL GetGlobalAddress(GAIA::NETWORK::Addr& addr){return m_sock.GetGlobalAddress(addr);}

			/*!
				@brief Get socket's local address.

				@return
					If success return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL GetLocalAddress(GAIA::NETWORK::Addr& addr){return m_sock.GetLocalAddress(addr);}

			/*!
				@brief Get socket's peer address.

				@return
					If success return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL GetPeerAddress(GAIA::NETWORK::Addr& addr){return m_sock.GetPeerAddress(addr);}

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
			virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult, GAIA::BL bByRemote){}

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
			virtual GAIA::GVOID OnSent(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nPracticeSize, GAIA::N32 nSize){}

			/*!
				@brief On async socket recv callback.
			*/
			virtual GAIA::GVOID OnRecved(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nSize){}

			/*!
				@brief On async socket shutdown callback.
			*/
			virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag){}

		private:
			GAIA::GVOID init();
			GAIA::BL SetFD(GAIA::N32 nFD){return m_sock.SetFD(nFD);}
			GAIA::BL SetType(GAIA::NETWORK::Socket::SOCKET_TYPE type){return m_sock.SetType(type);}
			GAIA::BL SetBinded(GAIA::BL bBinded){return m_sock.SetBinded(bBinded);}
			GAIA::BL SetConnected(GAIA::BL bConnected){return m_sock.SetConnected(bConnected);}
			GAIA::GVOID SetAsyncSocketType(GAIA::NETWORK::ASYNC_SOCKET_TYPE socktype){m_socktype = socktype;}
			GAIA::GVOID SetPeerAddress(const GAIA::NETWORK::Addr& addr){m_sock.SetPeerAddress(addr);}

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::BL SwapBrokenState()
			{
				GAIA::N64 lNewValue = m_atomBrokenTimes.Increase();
				GAST(lNewValue > 0);
				return lNewValue == 1;
			}
		#endif

		private:
			GAIA::NETWORK::AsyncDispatcher* m_pDispatcher;
			GAIA::NETWORK::ASYNC_SOCKET_TYPE m_socktype;
			GAIA::NETWORK::Socket m_sock;
			GAIA::SYNC::Lock m_lrSend;

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::SYNC::Atomic m_atomBrokenTimes;
			GAIA::GVOID* m_pfnAcceptEx;
			GAIA::GVOID* m_pfnConnectEx;
			GAIA::GVOID* m_pfnDisconnectEx;
		#else
			GAIA::CTN::BufferRW m_sendbuf;
			AsyncContext* m_pReadAsyncCtx;
			AsyncContext* m_pWriteAsyncCtx;
			GAIA::U64 m_uRecycleTime;
			GAIA::N32 m_nBackupSocket;
			GAIA::BL m_bPushForRecycleAble;
		#endif
		};
	}
}

#endif
