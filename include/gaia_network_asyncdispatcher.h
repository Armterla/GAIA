#ifndef		__GAIA_NETWORK_ASYNCDISPATCHER_H__
#define		__GAIA_NETWORK_ASYNCDISPATCHER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lockrw.h"
#include "gaia_sync_autolockr.h"
#include "gaia_sync_autolockw.h"
#include "gaia_ctn_set.h"
#include "gaia_network_asyncsocket.h"

namespace GAIA
{
	namespace NETWORK
	{
		class AsyncDispatcher : public GAIA::Base
		{
		public:
			class CallBack : public GAIA::Base
			{
			public:
				virtual GAIA::BL OnCollect(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock) = 0;
			};
		public:
			AsyncDispatcher();
			~AsyncDispatcher();

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

		private:
			GAIA::SYNC::LockRW m_rwSockets;
			GAIA::CTN::Set<Node> m_nodes_byaddrptr;

		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			GAIA::N32 m_kqueue;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			GAIA::N32 m_epoll;
		#endif
		};
	}
}

#endif
