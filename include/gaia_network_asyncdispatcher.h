#ifndef		__GAIA_NETWORK_ASYNCDISPATCHER_H__
#define		__GAIA_NETWORK_ASYNCDISPATCHER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_sync_lockrw.h"
#include "gaia_sync_autolockr.h"
#include "gaia_sync_autolockw.h"
#include "gaia_algo_memory.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_set.h"
#include "gaia_ctn_map.h"
#include "gaia_thread.h"
#include "gaia_network_asyncsocket.h"

namespace GAIA
{
	namespace NETWORK
	{
		/*!
			@brief AsyncDispatcher's description.
		 
			@remarks It will be used when AsyncDispatcher::Create method.
		*/
		class AsyncDispatcherDesc : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_MAX_CONNECTOIN_COUNT = 1000;

			static const GAIA::NUM DEFAULT_WINIOCP_ACCEPT_EVENT_COUNT = 10;
			static const GAIA::NUM DEFAULT_WINIOCP_RECV_EVENT_COUNT = 1;

		public:

			/*!
				@brief Reset all variables to default value.
			*/
			GINL GAIA::GVOID reset()
			{
				sThreadCount = DEFAULT_THREAD_COUNT;
				sMaxConnectionCount = DEFAULT_MAX_CONNECTOIN_COUNT;

				sWinIOCPAcceptEventCount = DEFAULT_WINIOCP_ACCEPT_EVENT_COUNT;
				sWinIOCPRecvEventCount = DEFAULT_WINIOCP_RECV_EVENT_COUNT;
			}

			/*!
				@brief Check current AsyncDispatcherDesc is valid or not.

				@return If current AsyncDispatcherDesc is valid(all of member variables are valid), return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL check() const
			{
				if(sThreadCount <= 0)
					return GAIA::False;
				if(sMaxConnectionCount <= 0)
					return GAIA::False;

				if(sWinIOCPAcceptEventCount <= 0)
					return GAIA::False;
				if(sWinIOCPRecvEventCount <= 0)
					return GAIA::False;
				return GAIA::True;
			}

		public:
			/*!
				@brief Specify the network thread count. Default is AsyncDispatcherDesc::DEFAULT_THREAD_COUNT.
			*/
			GAIA::NUM sThreadCount;

			/*!
				@brief Specify the max connection count at same time. Default is AsyncDispatcherDesc::DEFAULT_MAX_CONNECTION_COUNT.
			*/
			GAIA::NUM sMaxConnectionCount;

			/*!
				@brief Specify the IOCP accept event count that will be pre-delivered.
			 
				@remarks It used in Windows OS only, Other OS will ignore this parameter.
			*/
			GAIA::NUM sWinIOCPAcceptEventCount;

			/*!
				@brief Specify the IOCP receive event count that will be pre-delivered.
			 
				@remarks It used in Windows OS only, Other OS will ignore this parameter.
			*/
			GAIA::NUM sWinIOCPRecvEventCount;
		};

		/*!
			@brief AsyncDispatcher's AsyncSocket collect callback class.
		*/
		class AsyncDispatcherCallBack : public GAIA::Base
		{
		public:

			/*!
				@brief When collect a AsyncSocket, this function will be callbacked.

				@param disp [in] Specify the AsyncDispatcher.
			 
				@param sock [in] Specify the AsyncSocket which had collected..

				@return

				@remarks
			*/
			virtual GAIA::BL OnCollectAsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock){return GAIA::False;}
		};

		class AsyncDispatcherThread;
		
		/*!
			@brief TCP async socket dispatcher.
		*/
		class AsyncDispatcher : public GAIA::Base
		{
			friend class AsyncSocket;
			friend class AsyncDispatcherThread;

		public:

			/*!
				@brief Constructor.
			*/
			AsyncDispatcher();

			/*!
				@brief Destructor.
			*/
			~AsyncDispatcher();

			/*!
				@brief Create current AsyncDispatcher.

				@param

				@return

				@remarks
			*/
			GAIA::BL Create(const GAIA::NETWORK::AsyncDispatcherDesc& desc);

			/*!
				@brief Destroy current AsyncDispatcher.

				@param

				@return

				@remarks
			*/
			GAIA::BL Destroy();

			/*!
				@brief Check current AsyncDispatcher is created or not.

				@return If current AsyncDispatcher is created, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsCreated() const{return m_bCreated;}

			/*!
				@brief Get current AsyncDispatcher's description which be set by AsyncDispatcher::Create member function.

				@return Return current AsyncDispatcher's description.

				@see GAIA::NETWORK::AsyncDispatcherDesc.
			*/
			const GAIA::NETWORK::AsyncDispatcherDesc& GetDesc() const{return m_desc;}

			/*!
				@brief Begin current AsyncDispatcher.

				@param

				@return

				@remarks
			*/
			GAIA::BL Begin();

			/*!
				@brief End current AsyncDispatcher.

				@param

				@return

				@remarks
			*/
			GAIA::BL End();

			/*!
				@brief Check current AsyncDispatcher is began or not.

				@return If current AsyncDispatcher is began, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsBegin() const{return m_bBegin;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL AddListenSocket(const GAIA::NETWORK::Addr& addrListen);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL RemoveListenSocket(const GAIA::NETWORK::Addr& addrListen);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL RemoveListenSocketAll();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsExistListenSocket(const GAIA::NETWORK::Addr& addrListen) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NUM GetListenSocketCount() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CollectListenSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CollectListenSocket(GAIA::CTN::Vector<GAIA::NETWORK::Addr>& listResult) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsExistAcceptingSocket(GAIA::NETWORK::AsyncSocket& sock) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NUM GetAcceptingSocketCount() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CollectAcceptingSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsExistAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NUM GetAcceptedSocketCount() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CollectAcceptedSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsExistConnectedSocket(GAIA::NETWORK::AsyncSocket& sock) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NUM GetConnectedSocketCount() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CollectConnectedSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			/*!
				@brief Enable output log.

				@param bEnable [in] Specify enable or disable output log.
			*/
			GINL GAIA::GVOID EnableLog(GAIA::BL bEnable){m_bLog = bEnable;}

			/*!
				@brief Check is enable or disable output log.

				@return If enable output log, return GAIA::True, return GAIA::False.\m
					Default is disabled.
			*/
			GINL GAIA::BL IsEnableLog() const{return m_bLog;}

		protected:

			/*!
				@brief When AsyncDispatcher need create a listen socket(AsyncSocket), this function will be callbacked.

				@param

				@return

				@remarks
			*/
			virtual GAIA::NETWORK::AsyncSocket* OnCreateListenSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				GAIA::NETWORK::AsyncSocket* pListenSocket =
						gnew GAIA::NETWORK::AsyncSocket(*this, ASYNC_SOCKET_TYPE_LISTEN);
				return pListenSocket;
			}

			/*!
				@brief When AsyncDispatcher need create a accepting socket(AsyncSocket), this function will be callbacked.

				@param

				@return

				@remarks
			*/
			virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptingSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				GAIA::NETWORK::AsyncSocket* pAcceptingSocket =
						gnew GAIA::NETWORK::AsyncSocket(*this, ASYNC_SOCKET_TYPE_ACCEPTING);
				return pAcceptingSocket;
			}

			/*!
				@brief When AsyncDispatcher accept a AsyncSocket by a listen socket, this function will be callbacked.

				@param

				@return

				@remarks
			*/
			virtual GAIA::BL OnAcceptSocket(GAIA::NETWORK::AsyncSocket& sock, const GAIA::NETWORK::Addr& addrListen)
			{
				sock.drop_ref();
				return GAIA::False;
			}

		private:
			typedef GAIA::CTN::Map<GAIA::NETWORK::Addr, GAIA::NETWORK::AsyncSocket*> __SocketMapType;
			typedef GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*> __SocketSetType;
			typedef GAIA::CTN::Vector<GAIA::NETWORK::AsyncSocket*> __SocketVectorType;

		private:
			GAIA::GVOID init();
			GAIA::BL AddAcceptingSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveAcceptingSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL AddAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL AddConnectedSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveConnectedSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL Execute(GAIA::NETWORK::AsyncDispatcherThread* pThread);

			GAIA::NETWORK::AsyncContext* alloc_async_ctx();
			GAIA::GVOID release_async_ctx(GAIA::NETWORK::AsyncContext* pCtx);

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::BL attach_socket_iocp(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::GVOID request_accept(GAIA::NETWORK::AsyncSocket& listensock, const GAIA::NETWORK::Addr& addrListen);
			GAIA::GVOID request_recv(GAIA::NETWORK::AsyncSocket& datasock);
		#else
			GAIA::N32 select_kqep(GAIA::N32 nSocket) const;
			GAIA::GVOID push_for_recycle(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::GVOID pop_for_recycle(GAIA::NETWORK::AsyncSocket& sock);
		#endif

		private:
			GAIA::BL m_bCreated;
			GAIA::BL m_bBegin;
			GAIA::NETWORK::AsyncDispatcherDesc m_desc;
			GAIA::BL m_bLog;

			GAIA::SYNC::LockRW m_rwListenSockets;
			__SocketMapType m_listen_sockets;
			GAIA::SYNC::LockRW m_rwAcceptingSockets;
			__SocketSetType m_accepting_sockets;
			GAIA::SYNC::LockRW m_rwAcceptedSockets;
			__SocketSetType m_accepted_sockets;
			GAIA::SYNC::LockRW m_rwConnectedSockets;
			__SocketSetType m_connected_sockets;

			GAIA::CTN::Vector<AsyncDispatcherThread*> m_threads;
			
			GAIA::CTN::Pool<GAIA::NETWORK::AsyncContext> m_AsyncCtxPool;
			GAIA::SYNC::Lock m_lrAsyncCtxPool;

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::SYNC::LockRW m_rwPostAcceptAble;
			GAIA::BL m_bPostAcceptAble;
			GAIA::SYNC::LockRW m_rwAsyncCtxEnding;
		#endif
		};
	}
}

#endif
