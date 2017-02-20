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
#include "gaia_thread.h"
#include "gaia_network_asyncsocket.h"

namespace GAIA
{
	namespace NETWORK
	{
		class AsyncDispatcher : public GAIA::Base
		{
			friend class AsyncSocket;

		public:
			static const GAIA::NUM DEFAULT_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_CONNECT_EVENT_COUNT = 4;
			static const GAIA::NUM DEFAULT_ACCEPT_EVENT_COUNT = 4;
			static const GAIA::NUM DEFAULT_SEND_EVENT_COUNT = 100;
			static const GAIA::NUM DEFAULT_RECV_EVENT_COUNT = 100;
			class Desc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					sThreadCount = DEFAULT_THREAD_COUNT;
					sConnectEventCount = DEFAULT_CONNECT_EVENT_COUNT;
					sAcceptEventCount = DEFAULT_ACCEPT_EVENT_COUNT;
					sSendEventCount = DEFAULT_SEND_EVENT_COUNT;
					sRecvEventCount = DEFAULT_RECV_EVENT_COUNT;
				}
				GINL GAIA::BL check() const
				{
					if(sThreadCount <= 0)
						return GAIA::False;
					if(sConnectEventCount <= 0)
						return GAIA::False;
					if(sAcceptEventCount <= 0)
						return GAIA::False;
					if(sSendEventCount <= 0)
						return GAIA::False;
					if(sRecvEventCount <= 0)
						return GAIA::False;
					return GAIA::True;
				}
			public:
				GAIA::NUM sThreadCount;
				GAIA::NUM sConnectEventCount;
				GAIA::NUM sAcceptEventCount;
				GAIA::NUM sSendEventCount;
				GAIA::NUM sRecvEventCount;
			};
			class CallBack : public GAIA::Base
			{
			public:
				virtual GAIA::BL OnCollectListenSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock){return GAIA::False;}
				virtual GAIA::BL OnCollectRecvSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock){return GAIA::False;}
			};
		public:
			AsyncDispatcher();
			~AsyncDispatcher();

			GAIA::BL Create(const GAIA::NETWORK::AsyncDispatcher::Desc& desc);
			GAIA::BL Destroy();
			GAIA::BL IsCreated() const;
			const GAIA::NETWORK::AsyncDispatcher::Desc& GetDesc() const;

			GAIA::BL Begin();
			GAIA::BL End();
			GAIA::BL IsBegin() const;

			GAIA::BL AddListenSocket(const GAIA::NETWORK::Addr& addr);
			GAIA::BL RemoveListenSocket(const GAIA::NETWORK::Addr& addr);
			GAIA::BL RemoveListenSocketAll();
			GAIA::BL IsExistListenSocket(GAIA::NETWORK::Addr& addr);
			GAIA::NUM GetListenSocketCount() const;
			GAIA::BL CollectListenSocket(CallBack& cb) const;

			GAIA::BL AddRecvSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveRecvSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveRecvSocketAll();
			GAIA::BL IsExistRecvSocket(GAIA::NETWORK::AsyncSocket& sock) const;
			GAIA::NUM GetRecvSocketCount() const;
			GAIA::BL CollectRecvSocket(CallBack& cb) const;

		private:
			class Node : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					addrLocal.reset();
					pSock = GNIL;
				}
				GINL GAIA::N32 compare(const Node& src) const
				{
					GAIA::N32 nCmp = addrLocal.compare(src.addrLocal);
					if(nCmp != 0)
						return nCmp;
					if(pSock < src.pSock)
						return -1;
					else if(pSock > src.pSock)
						return +1;
					return 0;
				}
				GCLASS_COMPARE_BYCOMPARE(Node)
			public:
				GAIA::NETWORK::Addr addrLocal;
				GAIA::NETWORK::AsyncSocket* pSock;
			};

		private:
			GAIA::GVOID init();

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::NETWORK::IOCPOverlapped* alloc_iocpoverlapped();
			GAIA::GVOID release_iocpoverlapped(GAIA::NETWORK::IOCPOverlapped* pIOCPOverlapped);
			GAIA::BL attach_socket(GAIA::NETWORK::AsyncSocket& sock);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

		private:
			GAIA::BL m_bCreated;
			GAIA::NETWORK::AsyncDispatcher::Desc m_desc;

			GAIA::SYNC::LockRW m_rwListenSockets;
			GAIA::CTN::Set<Node> m_listen_sockets;
			GAIA::SYNC::LockRW m_rwRecvSockets;
			GAIA::CTN::Set<Node> m_recv_sockets;

			GAIA::CTN::Vector<GAIA::THREAD::Thread*> m_threads;
			
		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::GVOID* m_pIOCP;
			GAIA::CTN::Pool<GAIA::NETWORK::IOCPOverlapped> m_IOCPPool;
			GAIA::SYNC::Lock m_lrIOCPPool;
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			GAIA::N32 m_kqueue;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			GAIA::N32 m_epoll;
		#endif
		};
	}
}

#endif
