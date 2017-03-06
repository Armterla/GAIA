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
			@brief

			@remarks
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
				@brief

				@param

				@return

				@remarks
			*/
			GINL GAIA::GVOID reset()
			{
				sThreadCount = DEFAULT_THREAD_COUNT;
				sMaxConnectionCount = DEFAULT_MAX_CONNECTOIN_COUNT;

				sWinIOCPAcceptEventCount = DEFAULT_WINIOCP_ACCEPT_EVENT_COUNT;
				sWinIOCPRecvEventCount = DEFAULT_WINIOCP_RECV_EVENT_COUNT;
			}

			/*!
				@brief

				@param

				@return

				@remarks
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
				@brief
			*/
			GAIA::NUM sThreadCount;

			/*!
				@brief
			*/
			GAIA::NUM sMaxConnectionCount;

			/*!
				@brief
			*/
			GAIA::NUM sWinIOCPAcceptEventCount;

			/*!
				@brief
			*/
			GAIA::NUM sWinIOCPRecvEventCount;
		};

		/*!
			@brief
		*/
		class AsyncDispatcherCallBack : public GAIA::Base
		{
		public:

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			virtual GAIA::BL OnCollectAsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock){return GAIA::False;}
		};

		class AsyncDispatcherThread;
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
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Create(const GAIA::NETWORK::AsyncDispatcherDesc& desc);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Destroy();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsCreated() const{return m_bCreated;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			const GAIA::NETWORK::AsyncDispatcherDesc& GetDesc() const{return m_desc;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Begin();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL End();

			/*!
				@brief

				@param

				@return

				@remarks
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

		protected:

			/*!
				@brief

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
				@brief

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
				@brief

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
		#endif

		private:
			GAIA::BL m_bCreated;
			GAIA::BL m_bBegin;
			GAIA::NETWORK::AsyncDispatcherDesc m_desc;

			GAIA::SYNC::LockRW m_rwListenSockets;
			GAIA::CTN::Map<GAIA::NETWORK::Addr, GAIA::NETWORK::AsyncSocket*> m_listen_sockets;
			GAIA::SYNC::LockRW m_rwAcceptingSockets;
			GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*> m_accepting_sockets;
			GAIA::SYNC::LockRW m_rwAcceptedSockets;
			GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*> m_accepted_sockets;
			GAIA::SYNC::LockRW m_rwConnectedSockets;
			GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*> m_connected_sockets;

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
