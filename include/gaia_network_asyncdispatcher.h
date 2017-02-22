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
		class AsyncDispatcherDesc : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_ACCEPT_EVENT_COUNT = 10;
			static const GAIA::NUM DEFAULT_RECV_EVENT_COUNT = 1;

		public:
			GINL GAIA::GVOID reset()
			{
				sThreadCount = DEFAULT_THREAD_COUNT;
				sAcceptEventCount = DEFAULT_ACCEPT_EVENT_COUNT;
				sRecvEventCount = DEFAULT_RECV_EVENT_COUNT;
			}
			GINL GAIA::BL check() const
			{
				if(sThreadCount <= 0)
					return GAIA::False;
				if(sAcceptEventCount <= 0)
					return GAIA::False;
				if(sRecvEventCount <= 0)
					return GAIA::False;
				return GAIA::True;
			}
		public:
			GAIA::NUM sThreadCount;
			GAIA::NUM sAcceptEventCount;
			GAIA::NUM sRecvEventCount;
		};

		class AsyncDispatcherCallBack : public GAIA::Base
		{
		public:
			virtual GAIA::BL OnCollectAsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock){return GAIA::False;}
		};

		class AsyncDispatcherThread;
		class AsyncDispatcher : public GAIA::Base
		{
			friend class AsyncSocket;
			friend class AsyncDispatcherThread;

		public:
			AsyncDispatcher();
			~AsyncDispatcher();

			GAIA::BL Create(const GAIA::NETWORK::AsyncDispatcherDesc& desc);
			GAIA::BL Destroy();
			GAIA::BL IsCreated() const{return m_bCreated;}
			const GAIA::NETWORK::AsyncDispatcherDesc& GetDesc() const{return m_desc;}

			GAIA::BL Begin();
			GAIA::BL End();
			GAIA::BL IsBegin() const;

			GAIA::BL AddListenSocket(const GAIA::NETWORK::Addr& addrListen);
			GAIA::BL RemoveListenSocket(const GAIA::NETWORK::Addr& addrListen);
			GAIA::BL RemoveListenSocketAll();
			GAIA::BL IsExistListenSocket(const GAIA::NETWORK::Addr& addrListen) const;
			GAIA::NUM GetListenSocketCount() const;
			GAIA::BL CollectListenSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			GAIA::BL IsExistAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock) const;
			GAIA::NUM GetAcceptedSocketCount() const;
			GAIA::BL CollectAcceptedSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			GAIA::BL IsExistConnectedSocket(GAIA::NETWORK::AsyncSocket& sock) const;
			GAIA::NUM GetConnectedSocketCount() const;
			GAIA::BL CollectConnectedSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

		protected:
			virtual GAIA::NETWORK::AsyncSocket* OnCreateListenSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				GAIA::NETWORK::AsyncSocket* pListenSocket = gnew GAIA::NETWORK::AsyncSocket(*this, ASYNC_SOCKET_TYPE_LISTEN);
				return pListenSocket;
			}
			virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptedSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				GAIA::NETWORK::AsyncSocket* pAcceptedSocket = gnew GAIA::NETWORK::AsyncSocket(*this, ASYNC_SOCKET_TYPE_ACCEPTED);
				return pAcceptedSocket;
			}
			virtual GAIA::BL OnAcceptSocket(GAIA::NETWORK::AsyncSocket& sock, const GAIA::NETWORK::Addr& addrListen){return GAIA::False;}

		private:
			GAIA::GVOID init();
			GAIA::BL AddAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL AddConnectedSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL RemoveConnectedSocket(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::BL Execute();

		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::NETWORK::IOCPOverlapped* alloc_iocpol();
			GAIA::GVOID release_iocpol(GAIA::NETWORK::IOCPOverlapped* pOverlapped);
			GAIA::BL attach_socket_iocp(GAIA::NETWORK::AsyncSocket& sock);
			GAIA::GVOID request_accept(GAIA::NETWORK::AsyncSocket& listensock, const GAIA::NETWORK::Addr& addrListen);
			GAIA::GVOID request_recv(GAIA::NETWORK::AsyncSocket& datasock);
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

		private:
			GAIA::BL m_bCreated;
			GAIA::NETWORK::AsyncDispatcherDesc m_desc;

			GAIA::SYNC::LockRW m_rwListenSockets;
			GAIA::CTN::Map<GAIA::NETWORK::Addr, GAIA::NETWORK::AsyncSocket*> m_listen_sockets;
			GAIA::SYNC::LockRW m_rwAcceptedSockets;
			GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*> m_accepted_sockets;
			GAIA::SYNC::LockRW m_rwConnectedSockets;
			GAIA::CTN::Set<GAIA::NETWORK::AsyncSocket*> m_connected_sockets;

			GAIA::CTN::Vector<GAIA::THREAD::Thread*> m_threads;
			
		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::GVOID* m_pIOCP;
			GAIA::CTN::Pool<GAIA::NETWORK::IOCPOverlapped> m_IOCPOLPool;
			GAIA::SYNC::Lock m_lrIOCPOLPool;
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			GAIA::N32 m_kqueue;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			GAIA::N32 m_epoll;
		#endif
		};
	}
}

#endif
