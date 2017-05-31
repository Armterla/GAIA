#ifndef	 __GAIA_NETWORK_HTTPSERVER_H__
#define	 __GAIA_NETWORK_HTTPSERVER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_sync_lockrw.h"
#include "gaia_sync_autolockr.h"
#include "gaia_sync_autolockw.h"
#include "gaia_sync_event.h"
#include "gaia_ctn_ref.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_queue.h"
#include "gaia_ctn_buffer.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_set.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
#include "gaia_network_asyncsocket.h"
#include "gaia_network_asyncdispatcher.h"
#include "gaia_network_httpbase.h"

namespace GAIA
{
	namespace NETWORK
	{
		/*!
			@brief HttpServer's access mode.

				If you set HttpServer use BLACK mode, all requests in BLACK list will be deny.\n
				If you set HttpServer use WHITE mode, all requests not in WHITE list will be deny.\n
				If you set HttpServer use NONE mode, all requests will be accepted.\n
		*/
		GAIA_ENUM_BEGIN(HTTP_SERVER_BLACKWHITE_MODE)
			/*!
				@brief Specify the BLACK mode, it is the default mode.
			*/
			HTTP_SERVER_BLACKWHITE_MODE_BLACK,
			/*!
				@brief Specify the WHITE mode.
			*/
			HTTP_SERVER_BLACKWHITE_MODE_WHITE,
			/*!
				@brief Specify the NONE mode.
			*/
			HTTP_SERVER_BLACKWHITE_MODE_NONE,
		GAIA_ENUM_END(HTTP_SERVER_BLACKWHITE_MODE)

		/*!
			@brief HttpServer's access mode's string list.

				You can use a enum of HTTP_SERVER_BLACKWHITE_MODE as a array's index
				for current list, then you can get the enum's name.
		*/
		static const GAIA::CH* HTTP_SERVER_BLACKWHITE_MODE_STRING[] =
		{
			"HTTP_SERVER_BLACKWHITE_MODE_INVALID",
			"HTTP_SERVER_BLACKWHITE_MODE_BLACK",
			"HTTP_SERVER_BLACKWHITE_MODE_WHITE",
			"HTTP_SERVER_BLACKWHITE_MODE_NONE",
		};

		/*!
			@brief HttpServer create description.

			@see HttpServer::Create

			@remarks This class didn't have default constructor,
				so allocate a object of current class,
				all member variables are not initialized.
		*/
		class HttpServerDesc : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_NETWORK_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_WORK_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_MAX_CONN_COUNT = 10000;
			static const GAIA::U64 DEFAULT_MAX_CONN_TIME = (GAIA::U64)1000 * 1000 * 3600 * 24;
			static const GAIA::U64 DEFAULT_MAX_HALFCONN_TIME = 1000 * 1000 * 10;
			static const GAIA::U64 DEFAULT_MAX_DYNAMIC_CACHE_SIZE = 1024 * 1024 * 1024;
			static const GAIA::U64 DEFAULT_MAX_DYNAMIC_CACHE_COUNT = 1024 * 100;
			static const GAIA::U64 DEFAULT_MAX_STATIC_CACHE_SIZE = 1024 * 1024 * 10;
			static const GAIA::U64 DEFAULT_MAX_STATIC_CACHE_COUNT = 1024 * 100;
			static const GAIA::U64 DEFAULT_MAX_RESPONSE_COUNT_PER_MINUTE = 100000;

		public:

			/*!
				@brief Reset current structure to default value.
			*/
			GINL GAIA::GVOID reset()
			{
				sNetworkThreadCount = DEFAULT_NETWORK_THREAD_COUNT;
				sWorkThreadCount = DEFAULT_WORK_THREAD_COUNT;
				pszRootPath = GNILSTR;
				sMaxConnCount = DEFAULT_MAX_CONN_COUNT;
				uMaxConnTime = DEFAULT_MAX_CONN_TIME;
				uMaxHarfConnTime = DEFAULT_MAX_HALFCONN_TIME;
				uMaxDynamicCacheSize = DEFAULT_MAX_DYNAMIC_CACHE_SIZE;
				uMaxDynamicCacheCount = DEFAULT_MAX_DYNAMIC_CACHE_COUNT;
				uMaxStaticCacheSize = DEFAULT_MAX_STATIC_CACHE_SIZE;
				uMaxStaticCacheCount = DEFAULT_MAX_STATIC_CACHE_COUNT;
				uMaxResponseCountPerMinute = DEFAULT_MAX_RESPONSE_COUNT_PER_MINUTE;
				GAIA::ALGO::gstrcpy(szHttpVer, GAIA::NETWORK::HTTP_VERSION_STRING);
				sHttpVerLen = GAIA::ALGO::gstrlen(szHttpVer);
				bEnableSocketTCPNoDelay = GAIA::False;
				bEnableSocketNoBlock = GAIA::False;
				bEnableSocketReuseAddr = GAIA::False;
				nSocketSendTimeout = GINVALID;
				nSocketRecvTimeout = GINVALID;
				nListenSocketSendBufferSize = GINVALID;
				nListenSocketRecvBufferSize = GINVALID;
				nAcceptedSocketSendBufferSize = GINVALID;
				nAcceptedSocketRecvBufferSize = GINVALID;
				lSingleCallbackLimitSize = 64 * 1024;
			}

			/*!
				@brief Check current desc is valid or not.

					If not valid, you can't use it for HttpServer::Create's parameter.

				@return If valid, return GAIA::True, or return GAIA::False.
			*/
			GINL GAIA::BL check() const
			{
				if(sNetworkThreadCount <= 0)
					return GAIA::False;
				if(sWorkThreadCount < 0)
					return GAIA::False;
				if(pszRootPath == GNIL)
					return GAIA::False;
				if(sMaxConnCount == 0)
					return GAIA::False;
				if(uMaxConnTime == 0)
					return GAIA::False;
				if(uMaxHarfConnTime == 0)
					return GAIA::False;
				if(uMaxResponseCountPerMinute == 0)
					return GAIA::False;
				if(szHttpVer[0] == '\0')
					return GAIA::False;
				if(sHttpVerLen <= 0)
					return GAIA::False;
				if(GAIA::ALGO::gstrlen(szHttpVer) != sHttpVerLen)
					return GAIA::False;
				if(nSocketSendTimeout != GINVALID && nSocketSendTimeout <= 0)
					return GAIA::False;
				if(nSocketRecvTimeout != GINVALID && nSocketRecvTimeout <= 0)
					return GAIA::False;
				if(nListenSocketSendBufferSize != GINVALID && nListenSocketSendBufferSize < 0)
					return GAIA::False;
				if(nListenSocketRecvBufferSize != GINVALID && nListenSocketRecvBufferSize < 0)
					return GAIA::False;
				if(nAcceptedSocketSendBufferSize != GINVALID && nAcceptedSocketSendBufferSize < 0)
					return GAIA::False;
				if(nAcceptedSocketRecvBufferSize != GINVALID && nAcceptedSocketRecvBufferSize < 0)
					return GAIA::False;
				if(lSingleCallbackLimitSize < 0)
					return GAIA::False;
				return GAIA::True;
			}

		public:

			/*!
				@brief Specify network thread count, default value is DEFAULT_NETWORK_THREAD_COUNT.

				@see DEFAULT_NETWORK_THREAD_COUNT.
			*/
			GAIA::NUM sNetworkThreadCount;

			/*!
				@brief Specify work thread count, default value is DEFAULT_WORK_THREAD_COUNT.

				@see DEFAULT_WORK_THREAD_COUNT.
			*/
			GAIA::NUM sWorkThreadCount;

			/*!
				@brief Specify HttpServer's root path, default value is "".
			*/
			const GAIA::CH* pszRootPath;

			/*!
				@brief Specify HttpServer's max connection count at same time, default value is DEFAULT_MAX_CONN_COUNT.

				@see DEFAULT_MAX_CONN_COUNT.
			*/
			GAIA::NUM sMaxConnCount;

			/*!
				@brief Specify the connection's max transfer time, default value is DEFAULT_MAX_CONN_TIME.

				@see DEFAULT_MAX_CONN_TIME.
			*/
			GAIA::U64 uMaxConnTime;

			/*!
				@brief Specify tht connection's max pending time.(Connected but not send any data to HttpServer)
					Default value is DEFAULT_MAX_HALFCONN_TIME.

				@see DEFAULT_MAX_HALFCONN_TIME.
			*/
			GAIA::U64 uMaxHarfConnTime;

			/*!
				@brief Specify the max dynamic cache size in bytes, default value is DEFAULT_MAX_DYNAMIC_CACHE_SIZE.

				@see DEFAULT_MAX_DYNAMIC_CACHE_SIZE.
			*/
			GAIA::U64 uMaxDynamicCacheSize;

			/*!
				@brief Specify the max dynamic cache count, default value is DEFAULT_MAX_DYNAMIC_CACHE_COUNT.

				@see DEFAULT_MAX_DYNAMIC_CACHE_COUNT.
			*/
			GAIA::U64 uMaxDynamicCacheCount;

			/*!
				@brief Specify the max static cache size in bytes, default value is DEFAULT_STATIC_CACHE_SIZE.

				@see DEFAULT_STATIC_CACHE_SIZE.
			*/
			GAIA::U64 uMaxStaticCacheSize;

			/*!
				@brief Specify the max static cache count, default value is DEFAULT_STATIC_CACHE_COUNT.

				@see DEFAULT_STATIC_CACHE_COUNT.
			*/
			GAIA::U64 uMaxStaticCacheCount;

			/*!
				@brief Specify the max response count per minute, default value is DEFAULT_MAX_RESPONSE_COUNT_PER_MINUTE.

				@see DEFAULT_MAX_RESPONSE_COUNT_PER_MINUTE.
			*/
			GAIA::U64 uMaxResponseCountPerMinute;

			/*!
				@brief Specify the http version, default value is "HTTP/1.1".
			*/
			GAIA::CH szHttpVer[16];

			/*!
				@brief Specify the http version string's length, default value is strlen("HTTP/1.1").
			*/
			GAIA::NUM sHttpVerLen;

			/*!
				@brief Specify the socket use TCP NO-DELAY mode, default is GAIA::False.
			*/
			GAIA::BL bEnableSocketTCPNoDelay;

			/*!
				@brief Specify the socket use NO-BLOCK mode, default is GAIA::False.
			*/
			GAIA::BL bEnableSocketNoBlock;

			/*!
				@brief Specify the socket RE-USE-ADDRESS mode, default is GAIA::False.
			*/
			GAIA::BL bEnableSocketReuseAddr;
			
			/*!
			 	@brief Specify the socket's send timeout in milliseconds, default is GINVALID means use system default setting.
			*/
			GAIA::N32 nSocketSendTimeout;
			
			/*!
			 	@brief Specify the socket's recv timeout in milliseconds, default is GINVALID means use system default setting.
			*/
			GAIA::N32 nSocketRecvTimeout;

			/*!
				@brief Specify the listen socket's send buffer size in bytes, default is GINVALID means use system default setting.
			*/
			GAIA::N32 nListenSocketSendBufferSize;

			/*!
				@brief Specify the listen socket's receive buffer size in bytes, default is GINVALID means use system default setting.
			*/
			GAIA::N32 nListenSocketRecvBufferSize;

			/*!
				@brief Specify the accepted socket send buffer size in bytes, default is GINVALID means use system default setting.
			*/
			GAIA::N32 nAcceptedSocketSendBufferSize;

			/*!
				@brief Specify the accepted socket's receive buffer size in bytes, default is GINVALID means use system default setting.
			*/
			GAIA::N32 nAcceptedSocketRecvBufferSize;

			/*!
				@brief Specify the single callback limit size, default is 64Kb.
					When a request size(include head and body) below equal current variable,
					the HttpServerCallBack::OnRequest will be callbacked when all head and body received complete.
			*/
			GAIA::N64 lSingleCallbackLimitSize;
		};

		/*!
			@brief Http server's status description class.
		*/
		class HttpServerStatus : public GAIA::Base
		{
		public:

			/*!
				@brief Reset the HttpServerStatus to default value.

				@remarks All member variables are be filled by zero.
			*/
			GINL GAIA::GVOID reset()
			{
				uServerStartupTime = 0;

				uLinkLifeTime = 0;
				uLinkLifeCount = 0;

				uCallBackExecuteTime = 0;
				uCallBackExecuteCount = 0;

				uRequestAnalyzeFailedCount = 0;
				uRequestDenyByBWCount = 0;
				uRequestDenyByMaxConnCount = 0;
				uRequestDenyByMaxHalfConnCount = 0;

				uRequestCount = 0;
				uResponseCount = 0;

				uRequestPieceCount = 0;
				uResponsePieceCount = 0;

				uRequestSize = 0;
				uResponseSize = 0;

				GAST(sizeofarray(uRequestCountByMethod) == sizeofarray(uResponseCountByMethod));
				GAST(sizeofarray(uRequestCountByMethod) == sizeofarray(uRequestPieceCountByMethod));
				GAST(sizeofarray(uRequestCountByMethod) == sizeofarray(uResponsePieceCountByMethod));
				GAST(sizeofarray(uRequestCountByMethod) == sizeofarray(uRequestSizeByMethod));
				GAST(sizeofarray(uRequestCountByMethod) == sizeofarray(uResponseSizeByMethod));
				for(GAIA::NUM x = 0; x < sizeofarray(uRequestCountByMethod); ++x)
				{
					uRequestCountByMethod[x] = 0;
					uResponseCountByMethod[x] = 0;
					uRequestPieceCountByMethod[x] = 0;
					uResponsePieceCountByMethod[x] = 0;
					uRequestSizeByMethod[x] = 0;
					uResponseSizeByMethod[x] = 0;
				}

				for(GAIA::NUM x = 0; x < sizeofarray(uResponseCountByCode); ++x)
					uResponseCountByCode[x] = 0;

				uHitResponseCacheCount = 0;
				uHitResponseCacheSize = 0;
				uNotHitResponseCacheCount = 0;
				uNotResponseCount = 0;
			}

		public:

			/*!
				@brief Specify the server startup GMT time in microseconds.

					When you call HttpServer::Begin, this time will be filled.
			*/
			GAIA::U64 uServerStartupTime;

			/*!
				@brief Specify the link's life time.
			*/
			GAIA::U64 uLinkLifeTime;

			/*!
				@brief Specify the link's life dispatch count.
			*/
			GAIA::U64 uLinkLifeCount;

			/*!
				@brief Specify HttpServerCallBack's OnRequest be execute time in microseconds.
			*/
			GAIA::U64 uCallBackExecuteTime;

			/*!
				@brief Specify HttpServerCallBack's OnRequest's execute count.
			*/
			GAIA::U64 uCallBackExecuteCount;

			/*!
				@brief Specify HttpServer analyze request failed count.
			*/
			GAIA::U64 uRequestAnalyzeFailedCount;

			/*!
				@brief Specify HttpServer reject requests count for black white access deny.
			*/
			GAIA::U64 uRequestDenyByBWCount;

			/*!
				@brief Specify the max connection count to deny the request.
			*/
			GAIA::U64 uRequestDenyByMaxConnCount;

			/*!
				@brief Specify the max half connection count to deny the request.
			*/
			GAIA::U64 uRequestDenyByMaxHalfConnCount;

			/*!
				@brief Specify HttpServer's total request count.
			*/
			GAIA::U64 uRequestCount;

			/*!
				@brief Specify HttpServer's total response count.
			*/
			GAIA::U64 uResponseCount;

			/*!
				@brief Specify HttpServer's total request piese count.
			*/
			GAIA::U64 uRequestPieceCount;

			/*!
				@brief Specify HttpServer's total response piece count.
			*/
			GAIA::U64 uResponsePieceCount;

			/*!
				@brief Specify HttpServer's total request size in bytes.
			*/
			GAIA::U64 uRequestSize;

			/*!
				@brief Specify HttpServer's total response size in bytes.
			*/
			GAIA::U64 uResponseSize;

			/*!
				@brief Specify HttpServer's total request count by HTTP_METHOD catagory.
			*/
			GAIA::U64 uRequestCountByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief Specify HttpServer's total response count by HTTP_METHOD catagory.
			*/
			GAIA::U64 uResponseCountByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief Specify HttpServer's total request piese count by HTTP_METHOD catagory.
			*/
			GAIA::U64 uRequestPieceCountByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief Specify HttpServer's total response piece count by HTTP_METHOD catagory.
			*/
			GAIA::U64 uResponsePieceCountByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief Specify HttpServer's total request size in bytes by HTTP_METHOD catagory.
			*/
			GAIA::U64 uRequestSizeByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief Specify HttpServer's total response size in bytes by HTTP_METHOD catagory.
			*/
			GAIA::U64 uResponseSizeByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief Specify HttpServer's total response count by HTTP_CODE catagory.
			*/
			GAIA::U64 uResponseCountByCode[GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT];

			/*!
				@brief Specify hit cache count.
			*/
			GAIA::U64 uHitResponseCacheCount;

			/*!
				@brief Specify hit cache size.
			*/
			GAIA::U64 uHitResponseCacheSize;

			/*!
				@brief Specify not hit cache count.
			*/
			GAIA::U64 uNotHitResponseCacheCount;

			/*!
				@brief Specify not response count.

				@details
					If a request can't be dispatch by any sub class of HttpServerCallBack,
					and there is no default response with error http code,
					it will be record to this variable.
			*/
			GAIA::U64 uNotResponseCount;
		};

		/*!
			@brief HttpServer's black white node.
				It record a ip address info, and used for reject requests.
		*/
		class HttpServerBlackWhiteNode : public GAIA::Base
		{
		public:
			GINL GAIA::N32 compare(const GAIA::NETWORK::HttpServerBlackWhiteNode& src) const{return ip.compare(src.ip);}
			GCLASS_COMPARE_BYCOMPARE(HttpServerBlackWhiteNode)

		public:
			/*!
				@brief Specify the IP address.
			*/
			GAIA::NETWORK::IP ip;

			/*!
				@brief Specify the regist time.
					Regist time is the GMT time when you call HttpServer::AddBlackList or HttpServer::AddWhiteList.
			*/
			GAIA::U64 uRegistTime; // Real time in microseconds.

			/*!
				@brief Specify the effect time.
					Effect time is the BlackWhiteNode take effect time after you call HttpServer::AddBlackList or HttpServer::AddWhiteList.
			*/
			GAIA::U64 uEffectTime; // Relative time in microseconds.
		};

		class HttpServerAsyncSocket;
		class HttpServerAsyncDispatcher;

		/*!
			@brief HttpServer links class.
				It used to query a link state and response a request.
		*/
		class HttpServerLink : public GAIA::RefObject
		{
			friend class HttpServer;
			friend class HttpServerAsyncSocket;
			friend class HttpServerAsyncDispatcher;

		public:

			/*!
				@brief Constructor.

				@param svr [in] Specify the HttpServer.
			*/
			HttpServerLink(GAIA::NETWORK::HttpServer& svr);

			/*!
				@brief Destructor.
			*/
			~HttpServerLink();

			/*!
				@brief Get http server.

				@return Return HttpServer.
			*/
			GINL GAIA::NETWORK::HttpServer& GetServer() const{return *m_pSvr;}

			/*!
				@brief Get peer address.

				@return Return peer address.
			*/
			GINL const GAIA::NETWORK::Addr& GetPeerAddr() const{return m_addrPeer;}

			/*!
				@brief Get which listen socket address the HttpServerLink come from.

				@return Return the listen socket address.
			*/
			GINL const GAIA::NETWORK::Addr& GetListenAddr() const{return m_addrListen;}

			/*!
				@brief Get the time that the HttpServerLink's socket be accepted.

				@return Return the accept time by microseconds.
			*/
			GINL const GAIA::U64& GetAcceptTime() const{return m_uAcceptTime;}

			/*!
				@brief Response peer by http protocol.

				@param httpcode [in] Specify the http code.
					If success, return HTTP_CODE_OK, if failed by resource not found, return HTTP_CODE_NOTFOUND.

				@param pHttpHead [in] Specify the http response head information, if current response not have a header, it will be filled by GNIL.

				@param p [in] Specify the http body's data buffer.

				@param sSize [in] Specify the body's data buffer size.

				@return If response operation is executed successfully, return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL Response(GAIA::NETWORK::HTTP_CODE httpcode, GAIA::NETWORK::HttpHead* pHttpHead = GNIL, const GAIA::GVOID* p = GNIL, GAIA::NUM sSize = 0);

			/*!
				@brief Close the link.

				@return If close success, return GAIA::True, or return GAIA::False.

				@remarks If you response request complete, you must call this function manually.
			*/
			GAIA::BL Close();

			/*!
				@brief Get link's current request times in the link's life.

				@return Return request times.
			*/
			GINL GAIA::NUM GetRequestTimes() const{return m_sRequestTimes;}

			/*!
				@brief Get link's current reponse times in the link's life.

				@return Return response times.
			*/
			GINL GAIA::NUM GetResponseTimes() const{return m_sResponseTimes;}

			/*!
				@brief Check the link is receive all request data(include head and body) or not.

				@return If all request data(include head and body) received complete, return GAIA::True, or return GAIA::False.
			*/
			GINL GAIA::BL IsReqComplete() const{return m_bReqComplete;}

			/*!
				@brief Get http error.

				@return Return http error.
			*/
			GINL GAIA::NETWORK::HTTP_ERROR GetHttpError() const{return m_httperr;}

			/*!
				@brief Compare two HttpServerLink.

				@param src [in] Specify the second operator for compare.

				@return 
					If current HttpServerLink below paramter src, return -1.\n
					If current HttpServerLink equal paramter src, return 0.\n
					If current HttpServerLink above param src, return +1.\n
			*/
			GINL GAIA::N32 compare(const HttpServerLink& src) const
			{
				GAST(m_addrPeer.check());
				GAST(src.m_addrPeer.check());
				return m_addrPeer.compare(src.m_addrPeer);
			}
			GCLASS_COMPARE_BYCOMPARE(HttpServerLink)

		protected:
			virtual GAIA::GVOID RefObjectDestruct();

		private:
			GINL GAIA::GVOID init()
			{
				m_pSvr = GNIL;
				m_pSock = GNIL;
				m_addrPeer.reset();
				m_addrListen.reset();
				m_uAcceptTime = GINVALID;
				m_sRequestTimes = 0;
				m_sResponseTimes = 0;
				m_bReqComplete = GAIA::False;
				m_httperr = GAIA::NETWORK::HTTP_ERROR_OK;
			}
			GINL GAIA::GVOID SetPeerAddr(const GAIA::NETWORK::Addr& addrPeer){m_addrPeer = addrPeer;}
			GINL GAIA::GVOID SetListenAddr(const GAIA::NETWORK::Addr& addrListen){m_addrListen = addrListen;}
			GINL GAIA::GVOID SetAcceptTime(GAIA::U64 uTime){m_uAcceptTime = uTime;}
			GINL GAIA::GVOID SetRequestTimes(GAIA::NUM sRequestTimes){m_sRequestTimes = sRequestTimes;}
			GINL GAIA::GVOID SetResponseTimes(GAIA::NUM sResponseTimes){m_sResponseTimes = sResponseTimes;}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
			HttpServerAsyncSocket* m_pSock;
			GAIA::NETWORK::Addr m_addrPeer;
			GAIA::NETWORK::Addr m_addrListen;
			GAIA::U64 m_uAcceptTime;
			GAIA::NUM m_sRequestTimes;
			GAIA::NUM m_sResponseTimes;
			GAIA::BL m_bReqComplete;
			GAIA::NETWORK::HTTP_ERROR m_httperr;
		};

		/*!
			@brief HttpServer callback base class.

			@remarks
				The HttpServer's user have responsibility to derive a sub class from HttpServerCallBack,
				and the sub class's OnRequest virtual function will be called by HttpServer, the user 
				will get client request information in OnRequest virtual function.\n

				Current class derived from GAIA::RefObject, so the object will be managed by thread-safe 
				reference count.\n

				If you not need use this class's object later, and the class is allocated in Heap,
				You will call GAIA::RefObject::drop_ref() to decrease the reference count.\n
		*/
		class HttpServerCallBack : public GAIA::RefObject
		{
			friend class HttpServerLink;
			friend class HttpServer;
			friend class HttpServerAsyncSocket;
			friend class HttpServerAsyncDispatcher;

		public:
			/*!
				@brief Constructor.

				@param svr [in] Specify the HttpServer.
			*/
			HttpServerCallBack(GAIA::NETWORK::HttpServer& svr);

			/*!
				@brief Destructor.
			*/
			virtual ~HttpServerCallBack();

			/*!
				@brief Get the HttpServer.

				@return Return HttpServer.
			*/
			GINL GAIA::NETWORK::HttpServer& GetServer() const{return *m_pSvr;}

		public:
			/*!
				@brief Get HttpServerCallBack's name.

				@return Return the HttpServerCallBack's name.

				@remarks
					If HttpServer's user not overwrite this method, the system could work well as well.
					But we recommanded overwrite it and return a const string such as class's name,
					because the HttpServer could use it for debug reason.
			*/
			virtual const GAIA::CH* GetName() const{return "HttpServerCallBack";}

		protected:

			/*!
				@brief OnRequst call back.

				@param l [in] Specify the link.
					You could consider it as a http tcp socket.

				@param method [in] Specify the http method, such as GET, HEAD, POST, PUT and etc.

				@param url [in] Specify the url which will be requested, it specified by client.

				@param httphead [in] Specify the http head info which specified by client.

				@param p [in] Specify the body buffer's pointer, it could be GNIL when a request didn't have a body.

				@param sSize [in] Specify the body bufer's size, it could be zero when a request didn't have a body.

				@return
					If the request is be dispatched by the HttpServerCallBack's sub class(HttpServer's user derived),
					HttpServer's user will return GAIA::True, and then HttpServer will 
					not callback next HttpServerCallBack's OnRequest which be registed to 
					HttpServer(By HttpServer::RegistCallBack).\n

					If the request can't be dispatched by current HttpServerCallBack's sub class(HttpServer's user derived),
					HttpServer's user will return GAIA::False, and then HttpServer will
					callback next HttpServerCallBack's OnRequest member function.\n

					If all HttpServerCallBack::OnRequest return GAIA::False, it means this request can't be accepted.
					HttpServer will response with error code by default.\n

				@remarks
					This function will be callbacked on multi thread.
			*/
			virtual GAIA::BL OnRequest(
					GAIA::NETWORK::HttpServerLink& l,
					GAIA::NETWORK::HTTP_METHOD method,
					const GAIA::NETWORK::HttpURL& url,
					const GAIA::NETWORK::HttpHead& httphead,
					const GAIA::GVOID* p,
					GAIA::NUM sSize)
			{
				return GAIA::False;
			}

		private:
			GINL GAIA::GVOID init()
			{
				m_pSvr = GNIL;
			}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
		};

		/*!
			@brief HttpServerCallBack's sub class for response static resource.

			@remarks
				If a request is a pure request, it will be consider as a pure request, 
				and current class will try to load the resource file from disk, and response the file
				to client.
		*/
		class HttpServerCallBackForStaticResource : public GAIA::NETWORK::HttpServerCallBack
		{
		public:
			HttpServerCallBackForStaticResource(GAIA::NETWORK::HttpServer& svr);
			virtual ~HttpServerCallBackForStaticResource();

		public:
			virtual const GAIA::CH* GetName() const{return "HttpServerCallBackForStaticResource";}

		protected:
			virtual GAIA::BL OnRequest(
					GAIA::NETWORK::HttpServerLink& l,
					GAIA::NETWORK::HTTP_METHOD method,
					const GAIA::NETWORK::HttpURL& url,
					const GAIA::NETWORK::HttpHead& httphead,
					const GAIA::GVOID* p,
					GAIA::NUM sSize);
		};

		/*!
			@brief HttpServerCallBack's sub class for response HttpServer informations.

			@remarks
				It support url like these:
					1. http://hostaddress:port/httpinfo. (It used to get the http informations)
					2. http://hostaddress:port/httpblacklist. (It used to get the black list)
					3. http://hostaddress:port/httpwhitelist. (It used to get the white list)
		*/
		class HttpServerCallBackForInfo : public GAIA::NETWORK::HttpServerCallBack
		{
		public:
			HttpServerCallBackForInfo(GAIA::NETWORK::HttpServer& svr);
			virtual ~HttpServerCallBackForInfo();

		public:
			virtual const GAIA::CH* GetName() const{return "HttpServerCallBackForInfo";}

		protected:
			virtual GAIA::BL OnRequest(
					GAIA::NETWORK::HttpServerLink& l,
					GAIA::NETWORK::HTTP_METHOD method,
					const GAIA::NETWORK::HttpURL& url,
					const GAIA::NETWORK::HttpHead& httphead,
					const GAIA::GVOID* p,
					GAIA::NUM sSize);
		};

		class HttpServerWorkThread;

		/*!
			@brief Http server class.

			@remarks HttpServer is a thread-safe class.
		*/
		class HttpServer : public GAIA::Entity
		{
			friend class HttpServerLink;
			friend class HttpServerWorkThread;
			friend class HttpServerAsyncSocket;
			friend class HttpServerAsyncDispatcher;

		public:

			/*!
				@brief Constructor.
			*/
			HttpServer();

			/*!
				@brief Destructor.
			*/
			~HttpServer();

			/*!
				@brief Regist a HttpServerCallBack's object to HttpServer.

				@param cb [in] Specify the HttpServerCallBack's object to registed.

				@return If success, return GAIA::True, or return GAIA::False.

				@remarks
					Regist a same object twice will cause failed.
			*/
			GAIA::BL RegistCallBack(GAIA::NETWORK::HttpServerCallBack& cb);

			/*!
				@brief Unregist a HttpServerCallBack's object from HttpServer.

				@return If success, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL UnregistCallBack(GAIA::NETWORK::HttpServerCallBack& cb);

			/*!
				@brief Unregist all HttpServerCallBack's object from HttpServer.

				@return If succcess, return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL UnregistCallBackAll();

			/*!
				@brief Check a HttpServerCallBack's object is registed or not.

				@param cb [in] Specify a HttpServerCallBack's object to check.

				@return If parameter cb is registed, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsRegistedCallBack(GAIA::NETWORK::HttpServerCallBack& cb);

			/*!
				@brief Collect all regist HttpServerCallBack's objects.

				@param listResult [in] Used to save the collect result.

				@return If collect any HttpServerCallBack's object, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL CollectCallBack(GAIA::CTN::Vector<GAIA::NETWORK::HttpServerCallBack*>& listResult);

			/*!
				@brief Create HttpServer.

				@param desc [in] Specify HttpServer's descripion structure.

				@return If create success, return GAIA::True, or will return GAIA::False.

				@see HttpServerDesc.
			*/
			GAIA::BL Create(const GAIA::NETWORK::HttpServerDesc& desc);

			/*!
				@brief Destroy current HttpServer.

				@return If Destroy success, return GAIA::True, or will return GAIA::False.

				@remarks If current HttpServer is began, currect function call will end it by HttpServer::End function.
			*/
			GAIA::BL Destroy();

			/*!
				@brief Check current HttpServer is created or not.

				@return If current HttpServer is created, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsCreated() const{return m_bCreated;}

			/*!
				@brief Get current HttpServer's description structure.

				@return Return the description structure.

				@see HttpServerDesc.
			*/
			const GAIA::NETWORK::HttpServerDesc& GetDesc() const{return m_desc;}

			/*!
				@brief Begin(Startup) the service of current HttpServer.

				@return If current HttpServer is not begin, and startup service successfully,
					return GAIA::True, or will return GAIA::False.

				@remarks Current function will startup the network threads and work threads.
			*/
			GAIA::BL Begin();

			/*!
				@brief End(Shutdown) the service of current HttpServer.

				@return If end service successfully, return GAIA::True,
					or will return GAIA::False.

				@remarks Current function will shutdown the network threads and work threads.
			*/
			GAIA::BL End();

			/*!
				@brief Check the service of current HttpServer is began(Startuped) or not.

				@return If the service of current HttpServer is began,
					return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsBegin() const{return m_bBegin;}

			/*!
				@brief Execute HttpServer.

			 	@param uWaitMilliSeconds [in] Specify the time for wait a valid request in milliseconds.
			 		0 means no wait, GINVALID means infinite.
			 		Default value is 0.
			 
				@return If exist some task to dispatch, and be dispatched, return GAIA::True,
					or will return GAIA::False.

				@remarks
					Call this function will cause HttpServerCallBack::OnRequest be callback.
			*/
			GAIA::BL Execute(GAIA::U32 uWaitMilliSeconds = 0);

			/*!
				@brief Open a address for Http provide Http service.

				@param addr [in] Specify a network address to open service.

				@return If success return GAIA::True, or will return GAIA::False.

				@remarks
					This function will open a listen tcp socket for http service.
			*/
			GAIA::BL OpenAddr(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Close a openned address which provide Http service.

				@param addr [in] Specify a network address to close service.

				@return If success return GAIA::True, or will return GAIA::False.

				@remarks
					This function will close a listen tcp socket which used for http service.
			*/
			GAIA::BL CloseAddr(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Close all openned network address which provide Http service.

				@return If exist address be closed return GAIA::True, or will return GAIA::False.

				@remarks
					This function will close all listen tcp socket which used for http service.
			*/
			GAIA::BL CloseAddrAll();

			/*!
				@brief Check the network address is openned or not.

				@param addr [in] Specify the address for checkup.

				@return If the network address which provide Http service is openned, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsOpennedAddr(const GAIA::NETWORK::Addr& addr) const;

			/*!
				@brief Get openned network address count. (PS:The network address provide Http service)

				@return Return the address count for openned network address which are used for http service.
			*/
			GAIA::NUM GetOpennedAddrCount() const;

			/*!
				@brief Collect openned network address list. (PS: The network addresses provide Http service)

				@param listResult [out] Used for saving openned addresses.

				@return If collect success, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL CollectOpennedAddr(GAIA::CTN::Vector<GAIA::NETWORK::Addr>& listResult);

			/*!
				@brief Enable or disable response cache.

				@param bEnable [in] Enable or disable.
			*/
			GAIA::GVOID EnableResponseCache(GAIA::BL bEnable);

			/*!
				@brief Check response cache is enabled or disabled.

				@return If enabled, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsEnableResponseCache() const;

			/*!
				@brief Set HttpServer's black white access mode.

				@param mode [in] Specify black white mode.

				@see GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE.
			*/
			GAIA::GVOID SetBlackWhiteMode(GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE mode);

			/*!
				@brief Get HttpServer's black white access mode.

				@return Return black white mode.

				@see GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE.
			*/
			GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE GetBlackWhiteMode() const;

			/*!
				@brief Add a ip to black list.

				@param ip [in] Speicify a IP to add to black list.

				@param uEffectTime [in] Specify the take effect time in microseconds.

				@remarks
					If the node is not exist which specified by parameter ip, the node will be created to black lsit.
			*/
			GAIA::GVOID AddBlackList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uEffectTime = GINVALID);

			/*!
				@brief Remove a ip from black list.

				@param ip [in] Specify a IP to remove.

				@return If the parameter ip is exist in black list, current function call will success and return GAIA::True,
					or will return GAIA::False.
			*/
			GAIA::BL RemoveBlackList(const GAIA::NETWORK::IP& ip);

			/*!
				@brief Remove all ip addresses from black list.
			*/
			GAIA::GVOID RemoveBlackListAll();

			/*!
				@brief Check a ip in black list or not.

				@param ip [in] Specify a ip to check.

				@return If the parameter ip is exist in black list, return GAIA::True,
					or will return GAIA::False.
			*/
			GAIA::BL IsInBlackList(const GAIA::NETWORK::IP& ip) const;

			/*!
				@brief Get count of ip addresses in black list.

				@return Return the count of addresses in black list.
			*/
			GAIA::NUM GetBlackListSize() const;

			/*!
				@brief Collect all ip addresses in black list.

				@param listResult [out] Used for saving the black node list.

				@return If there is exist black node to collect, return GAIA::True,
					or will return GAIA::False.
			*/
			GAIA::BL CollectBlackList(GAIA::CTN::Vector<GAIA::NETWORK::HttpServerBlackWhiteNode>& listResult) const;

			/*!
				@brief Add a ip to white list.

				@param ip [in] Specify a IP to add to white list.

				@param uEffectTime [in] Specify the take effect time in microseconds.

				@remarks
					If the node is not exist which specified by parameter ip, the node will be created to white list.
			*/
			GAIA::GVOID AddWhiteList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uEffectTime = GINVALID);

			/*!
				@brief Remove a ip from white list.

				@param ip [in] Specify a IP to remove.

				@return If the parameter ip is exist in white list, current function call will success and return GAIA::True,
					or will return GAIA::False.
			*/
			GAIA::BL RemoveWhiteList(const GAIA::NETWORK::IP& ip);

			/*!
				@brief Remove all ip addresses from white list.
			*/
			GAIA::GVOID RemoveWhiteListAll();

			/*!
				@brief Check a ip in white list or not.

				@param ip [in] Specify a IP to check.

				@return If the parameter ip is exist in white list, return GAIA::True,
					or will return GAIA::False.
			*/
			GAIA::BL IsInWhiteList(const GAIA::NETWORK::IP& ip) const;

			/*!
				@brief Get count of ip addresses in white list.

				@return Return the count of ip addresses in white list.
			*/
			GAIA::NUM GetWhiteListSize() const;

			/*!
				@brief Collect all ip addresses in white list.

				@param listResult [out] Used for saving the white node list.

				@return If there is exist white node to collect, return GAIA::True,
					or will return GAIA::False.
			*/
			GAIA::BL CollectWhiteList(GAIA::CTN::Vector<GAIA::NETWORK::HttpServerBlackWhiteNode>& listResult) const;

			/*!
				@brief Get http server status.

				@return Return http server status.
			*/
			GINL GAIA::NETWORK::HttpServerStatus& GetStatus(){return m_status;}

			/*!
				@brief Request a cache data by a string key.

				@param pszKey [in] Specify the cache data's string key.

				@param sKeyLen [in] Specify the key's length in characters(With out '\0').
					This parameter could be GINVALID when the parameter pszKey ended with '\0'.

				@param resphead [out] Used for saving the response head.

				@param p [out] Used for saving the body's pointer.
					If the cache did not have a body, this parameter will be filled by GNIL.

				@param sSize [out] Used for saving the body's size in bytes.
					If the cache did not have a body, this parameter will be filled by 0.

				@return If request the cache successfully, return GAIA::True,
					or will return GAIA::False.

				@remarks
					Cache is managed by reference count, call this function will cause reference count increase 1,
					when a cache's reference count decreased to 0, it will be recycled by HttpServer::RecycleCache function.\n

					If current function return GAIA::True, current function will lock cache system's read-lock,
					the HttpServer's user have responsibility to call HttpServer::ReleaseCache to release it(decrease reference count).
					If not do it like above, the HttpServer will deadlock or memory leak.\n
			*/
			GAIA::BL RequestCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen, GAIA::NETWORK::HttpHead& resphead, GAIA::GVOID** p, GAIA::NUM& sSize);

			/*!
				@brief Request a cache data by a url and request head.

				@param url [in] Specify the cache's url.

				@param reqhead [in] Specify the cache's head.

				@param resphead [out] Used for saving the response head.

				@param p [out] Used for saving the body's pointer.
					If the cache did not have a body, this parameter will be filled by GNIL.

				@param sSize [out] Used for saving the body's size in bytes.
					If the cache did not have a body, this parameter will be filled by 0.

				@return If request the cache successfully, return GAIA::True,
					or will return GAIA::False.

				@remarks
					Cache is managed by reference count, call this function will cause reference count increase 1,
					when a cache's reference count decreased to 0, it will be recycled by HttpServer::RecycleCache function.\n

					If current function return GAIA::True, current function will lock cache system's read-lock,
					the HttpServer's user have responsibility to call HttpServer::ReleaseCache to release it(decrease reference count).
					If not do it like above, the HttpServer will deadlock or memory leak.\n
			*/
			GAIA::BL RequestCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead, GAIA::NETWORK::HttpHead& resphead, GAIA::GVOID** p, GAIA::NUM& sSize);

			/*!
				@brief Release a cache by a string key.

				@param pszKey [in] Specify the cache data's string key.

				@param sKeyLen [in] Specify the key's length in characters(With out '\0').
					This parameter could be GINVALID when the parameter pszKey ended with '\0'.

				@return If release the cache successfully, return GAIA::True,
					or will return GAIA::False.

				@remarks
					Cache is managed by reference count, call this function will cause reference count decrease 1,
					When a cache's reference count decreased to 0, it will be recycled by HttpServer::RecycleCache function.\n

					If current function return GAIA::True, current function had release the cache system's read-lock
					which is locked by HttpServer::RequestCache.\n
			*/
			GAIA::BL ReleaseCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen);

			/*!
				@brief Release a cache by url and request head.

				@param url [in] Specify the cache's url.

				@param reqhead [in] Specify the cache's head.

				@return If release the cache successfully, return GAIA::True,
					or will return GAIA::False.

				@remarks
					Cache is managed by reference count, call this function will cause reference count decrease 1,
					When a cache's reference count decreased to 0, it will be recycled by HttpServer::RecycleCache function.\n

					If current function return GAIA::True, current function had release the cache system's read-lock
					which is locked by HttpServer::RequestCache.\n
			*/
			GAIA::BL ReleaseCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead);

			/*!
				@brief Update or insert a cache by a string key.

				@param pszKey [in] Specify the cache data's string key.

				@param sKeyLen [in] Specify the key's length in characters(With out '\0').
					This parameter could be GINVALID when the parameter pszKey ended with '\0'.

				@param resphead [in] Specify the cache's response head.

				@param p [in] Specify the cache's body.

				@param sSize [in] Specify the cache's body size in bytes.

				@param uEffectTime [in] Specify the cache take effect time in microseconds.

				@return If update or insert the cache successfully, return GAIA::True,
					or will return GAIA::False.

				@remarks
					If the cache data is not exist which specified by parameter pszKey,
					current function will insert the cache data.
			*/
			GAIA::BL UpdateCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen, const GAIA::NETWORK::HttpHead& resphead, const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::U64 uEffectTime = GINVALID);

			/*!
				@brief Update or insert a cache by url and request head.

				@param url [in] Specify the cache's url.

				@param reqhead [in] Specify the cache's head.

				@param p [in] Specify the cache's body.

				@param sSize [in] Specify the cache's body size in bytes.

				@param uEffectTime [in] Specify the cache take effect time in microseconds.

				@return If update or insert the cache successfully, return GAIA::True,
					or will return GAIA::False.

				@remarks
					If the cache data is not exist which specified by parameter url and reqhead,
					current function will insert the cache data.
			*/
			GAIA::BL UpdateCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead, const GAIA::NETWORK::HttpHead& resphead, const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::U64 uEffectTime = GINVALID);

			/*!
				@brief Recycle the cache data which are not need to used later.

				@return If there exist some cache data be recycled, return GAIA::True,
					or will return GAIA::False.
			*/
			GAIA::BL RecycleCache();

			/*!
				@brief Get the AsyncDispatcher which derived from GAIA::NETWORK::AsyncDispatcher.

				@return Return the AsyncDispatcher.
			*/
			GAIA::NETWORK::AsyncDispatcher* GetAsyncDispatcher() const{return (GAIA::NETWORK::AsyncDispatcher*)m_disp;}

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

		private:
			GAIA::BL RecycleLink(GAIA::NETWORK::HttpServerLink& l);
			GAIA::CTN::Buffer* RequestBuffer();
			GAIA::GVOID ReleaseBuffer(GAIA::CTN::Buffer* pBuf);

		private:
			GINL GAIA::GVOID init()
			{
				m_desc.reset();
				m_bCreated = GAIA::False;
				m_bBegin = GAIA::False;
				m_bEnableResponseCache = GAIA::True;
				m_blackwhitemode = GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE_BLACK;
				m_disp = GNIL;
				m_status.reset();
				m_bLog = GAIA::False;
			}

		private:

			class CacheNode : public GAIA::Base
			{
			public:
				GINL GAIA::N32 compare(const CacheNode& src) const{return strKey.compare(src.strKey);}
				GCLASS_COMPARE_BYCOMPARE(CacheNode)

			public:
				GAIA::CTN::AString strKey;
				GAIA::NETWORK::HttpHead resphead;
				GAIA::CTN::Buffer* buf;
				GAIA::U64 uRegistTime;
				GAIA::U64 uEffectTime;
				GAIA::N64 nRefCount;
			};

		private:
			typedef GAIA::CTN::Set<GAIA::CTN::Ref<GAIA::NETWORK::HttpServerLink> > __LinkSetType;
			typedef GAIA::CTN::Queue<GAIA::NETWORK::HttpServerLink*> __LinkQueueType;

		private:
			GAIA::NETWORK::HttpServerDesc m_desc;
			GAIA::SYNC::LockRW m_rwCBS;
			GAIA::CTN::Vector<GAIA::NETWORK::HttpServerCallBack*> m_cbs;
			GAIA::BL m_bCreated;
			GAIA::BL m_bBegin;
			
			GAIA::SYNC::Event m_ExecEvent;

			GAIA::SYNC::Lock m_lrLinks;
			__LinkSetType m_links_bypeeraddr;

			GAIA::SYNC::Lock m_lrRCLinks; // RC means request complete.
			__LinkQueueType m_rclinks; // RC means request complete.

			GAIA::BL m_bEnableResponseCache;

			GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE m_blackwhitemode;
			GAIA::SYNC::LockRW m_rwBlackList;
			GAIA::CTN::Set<GAIA::NETWORK::HttpServerBlackWhiteNode> m_BlackList;
			GAIA::SYNC::LockRW m_rwWhiteList;
			GAIA::CTN::Set<GAIA::NETWORK::HttpServerBlackWhiteNode> m_WhiteList;

			GAIA::SYNC::LockRW m_rwCache;
			GAIA::CTN::Set<CacheNode> m_cache;

			GAIA::SYNC::Lock m_lrBufPool;
			GAIA::CTN::Pool<GAIA::CTN::Buffer> m_bufpool;

			GAIA::NETWORK::HttpServerAsyncDispatcher* m_disp;
			GAIA::CTN::Vector<GAIA::NETWORK::HttpServerWorkThread*> m_listWorkThreads;
			GAIA::NETWORK::HttpServerStatus m_status;
			GAIA::BL m_bLog;
		};
	}
}

#endif
