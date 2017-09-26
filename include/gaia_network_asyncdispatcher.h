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

				@return Return GAIA::True means need collect more async socket, return GAIA::False means stop collect async socket.

				@remarks If you want use a collected async socket out of current callback, you need add AsyncSocket's reference count(AsyncSocket::rise_ref) manually.
			*/
			virtual GAIA::BL OnCollectAsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock){return GAIA::False;}
		};

		class AsyncDispatcherThread;
		
		/*!
			@brief TCP async socket dispatcher.
		*/
		class AsyncDispatcher : public GAIA::Entity
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

				@remarks If current AsyncDispatcher is created, this metod will destroy it first(by AsyncDispatcher::Destroy).
			*/
			~AsyncDispatcher();

			/*!
				@brief Create current AsyncDispatcher.

				@param desc [in] Specify the AsyncDispatcher's description.

				@return If create successfully, return GAIA::True, or will return GAIA::False.

				@remarks This method will allocate the some internal object, after this function call,
					most of methods in current class could take work.
			*/
			GAIA::BL Create(const GAIA::NETWORK::AsyncDispatcherDesc& desc);

			/*!
				@brief Destroy current AsyncDispatcher.

				@return If destroy successfully, return GAIA::True, or will return GAIA::False.

				@remarks If current AsyncDispatcher is began, this method will end it first(by AsyncDispatcher::End), and then destroy the context.
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

				@return If begin successfully, return GAIA::True, or will return GAIA::False.

				@remarks This method will start the network threads.
			*/
			GAIA::BL Begin();

			/*!
				@brief End current AsyncDispatcher.

				@return If end successfully, return GAIA::True, or will return GAIA::False.

				@remarks This method will stop the network thread.
			*/
			GAIA::BL End();

			/*!
				@brief Check current AsyncDispatcher is began or not.

				@return If current AsyncDispatcher is began, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsBegin() const{return m_bBegin;}

			/*!
				@brief Add a network address for listen.

				@param addrListen [in] Specify the network address.

				@return If add a network address for listen successfully, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL AddListenSocket(const GAIA::NETWORK::Addr& addrListen);

			/*!
				@brief Remove a added network address for stop listen.

				@param addrListen [in] Specify the network address.

				@return If remove a added network address for stop listen successfully, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL RemoveListenSocket(const GAIA::NETWORK::Addr& addrListen);

			/*!
				@brief Remove all of the listening network address.

				@return If exist any listened network address be closed, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL RemoveListenSocketAll();

			/*!
				@brief Check the network address is listening or not.

				@param addrListen [in] Specify the network address.

				@return If the network address is listening, return GAIA::True, or will return GAIA:False.
			*/
			GAIA::BL IsExistListenSocket(const GAIA::NETWORK::Addr& addrListen) const;

			/*!
				@brief Get curernt listening socket count.

				@return Return current listening socket count.
			*/
			GAIA::NUM GetListenSocketCount() const;

			/*!
				@brief Collect current listening socket by a GAIA::NETWORK::AsyncDispatcherCallBack object.

				@param cb [in] Specified the sub class derived from GAIA::NETWORK::AsyncDispatcherCallBack for callback.

				@return If the callback loop be canceled by parameter cb, return GAIA::False, or return GAIA::True.
			*/
			GAIA::BL CollectListenSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			/*!
				@brief Collect current listening network address and save to a list.

				@param listResult [in] Used for save the collected result.

				@return If collected any listening network address, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL CollectListenSocket(GAIA::CTN::Vector<GAIA::NETWORK::Addr>& listResult) const;

			/*!
				@brief Check a socket is a accepting socket or not.

				@param sock [in] Specify the socket to check.

				@return If the socket which specified by parameter sock is a accepting socket, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsExistAcceptingSocket(GAIA::NETWORK::AsyncSocket& sock) const;

			/*!
				@brief Get current accepting socket count.

				@return Return current accepting socket count.
			*/
			GAIA::NUM GetAcceptingSocketCount() const;

			/*!
				@brief Collect current accepting socket by a GAIA::NETWORK::AsyncDispatcherCallBack object.

				@param cb [in] Specified the sub class derived from GAIA::NETWORK::AsyncDispatcherCallBack for callback.

				@return If the callback loop be canceled by parameter cb, return GAIA::False, return GAIA::True.
			*/
			GAIA::BL CollectAcceptingSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			/*!
				@brief Check the socket is a accepted socket or not.

				@param sock [in] Specified the socket to check.

				@return If the socket which specified by parameter sock is a accepted socket, return GAIA::Tre, or will return GAIA::False.
			*/
			GAIA::BL IsExistAcceptedSocket(GAIA::NETWORK::AsyncSocket& sock) const;

			/*!
				@brief Get current accepted socket count.

				@return Return current accepted socket count.
			*/
			GAIA::NUM GetAcceptedSocketCount() const;

			/*!
				@brief Collect current accepted socket by a GAIA::NETWORK::AsyncDispatcherCallback object.

				@param cb [in] Specified the sub class derived from GAIA::NETWORK::AsyncDispatcherCallBack for callback.

				@return If the callback loop be canceled by parameter cb, return GAIA::False, return GAIA::True.
			*/
			GAIA::BL CollectAcceptedSocket(GAIA::NETWORK::AsyncDispatcherCallBack& cb) const;

			/*!
				@brief Check the socket is a connected socket or not.

				@param sock [in] Spcified the socket to check.

				@return If the socket which specified by parameter sock is a connected socket, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsExistConnectedSocket(GAIA::NETWORK::AsyncSocket& sock) const;

			/*!
				@brief Get current connected socket count.

				@return Return current connected socket count.
			*/
			GAIA::NUM GetConnectedSocketCount() const;

			/*!
				@brief Collect current connected socket by a GAIA::NETWORK::AsyncDispatcherCallBack object.

				@param cb [in] Specified the sub class derived from GAIA::NETWORK::AsyncDispatcherCallBack for callback.

				@return If the callback loop be canceled by parameter cb, return GAIA::False, return GAIA::True.
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

				@param addrListen [in] Specify the network address for listen.

				@return Return a GAIA::NETWORK::AsyncSocket or it's sub class.

				@remarks This function is used for obtain a AsyncSocket object instance by AsyncDispatcher, so it is like a factory method.\n
					The user must return a new allocated AsyncSocket, and the socket must not created.\n
			*/
			virtual GAIA::NETWORK::AsyncSocket* OnCreateListenSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				GAIA::NETWORK::AsyncSocket* pListenSocket = gnew GAIA::NETWORK::AsyncSocket(*this, ASYNC_SOCKET_TYPE_LISTEN);
				return pListenSocket;
			}

			/*!
				@brief When AsyncDispatcher need create a accepting socket(AsyncSocket), this function will be callbacked.

				@param addrListen [in] Specify the network addresss that the new accepting socket will be accept.

				@return Return a GAIA::NETWORK::AsyncSocket or it's sub class.

				@remarks This function is used for obtain a AsyncSocket object instance by AsyncDispatcher, so it is like a factory method.\n
					The user must return a new allocated AsyncSocket, and the socket must not created.\n
			*/
			virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptingSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				GAIA::NETWORK::AsyncSocket* pAcceptingSocket = gnew GAIA::NETWORK::AsyncSocket(*this, ASYNC_SOCKET_TYPE_ACCEPTING);
				return pAcceptingSocket;
			}

			/*!
				@brief When AsyncDispatcher accept a AsyncSocket by a listen socket, this function will be callbacked.

				@param sock [in] Specify the new accepted socket.

				@param addrListen [in] Specify the network address where parameter sock come from.

				@return If the socket is ignored by user(function overwriter), the user have responsibility to return GAIA::False, or will return GAIA::True.

				@remarks If this function is called by AsyncDispatcher,
					the user(function overwriter) will be the parameter sock's instance-owner,
					so, the user could use the sock after this function call. \n
					If the user need return GAIA::False to ignore this socket, the user have responsibility to decrease
					the reference count of parameter sock(call AsyncSock::drop_ref).\n
			*/
			virtual GAIA::BL OnAcceptSocket(GAIA::NETWORK::AsyncSocket& sock, const GAIA::NETWORK::Addr& addrListen)
			{
				sock.drop_ref("AsyncDispatcher::OnAcceptSocket:drop ref by base class");
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
