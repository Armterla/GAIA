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
#include "gaia_ctn_set.h"
#include "gaia_thread.h"
#include "gaia_network_asyncsocket.h"

namespace GAIA
{
	namespace NETWORK
	{
		class AsyncDispatcher : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_THREAD_COUNT = 4;
			class Desc : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset(){sThreadCount = DEFAULT_THREAD_COUNT;}
				GINL GAIA::BL check() const
				{
					if(sThreadCount <= 0)
						return GAIA::False;
					return GAIA::True;
				}
			public:
				GAIA::NUM sThreadCount;
			};
			class CallBack : public GAIA::Base
			{
			public:
				virtual GAIA::BL OnCollect(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock) = 0;
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

			GAIA::BL AddAsyncSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveAsyncSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveAsyncSocketAll();
			GAIA::BL IsExistAsyncSocket(GAIA::NETWORK::AsyncSocket& sock) const;
			GAIA::NUM GetAsyncSocketCount() const;
			GAIA::BL CollectSocket(CallBack& cb) const;

		private:
			class Node : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					pSock = GNIL;
					sIndex = GINVALID;
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
				GAIA::NUM sIndex;
			};

		private:
			GAIA::GVOID init();

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::NETWORK::IOCPOverlapped* alloc_iocpoverlapped();
			GAIA::GVOID release_iocpoverlapped(GAIA::NETWORK::IOCPOverlapped* pIOCPOverlapped);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

		private:
			GAIA::BL m_bCreated;
			GAIA::NETWORK::AsyncDispatcher::Desc m_desc;
			GAIA::SYNC::LockRW m_rwSockets;
			GAIA::CTN::Set<Node> m_nodes_byaddrptr;
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
