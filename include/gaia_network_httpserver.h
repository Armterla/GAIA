#ifndef	 __GAIA_NETWORK_HTTPSERVER_H__
#define	 __GAIA_NETWORK_HTTPSERVER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_sync_lockrw.h"
#include "gaia_sync_autolockr.h"
#include "gaia_sync_autolockw.h"
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
			@brief Current supported HTTP protocal version.
		*/
		static const GAIA::CH* HTTP_VERSION_STRING = "HTTP/1.1";

		/*!
			@brief HttpServer's access mode.

				If you set HttpServer use BLACK mode, all requests in BLACK list will be deny.
				If you set HttpServer use WHITE mode, all requests not in WHITE list will be deny.
				If you set HttpServer use NONE mode, all requests will be accepted.
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
			@brief
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
			static const GAIA::U64 DEFAULT_MAX_DYNAMIC_CACHE_COUNT = 1024 * 1024;
			static const GAIA::U64 DEFAULT_MAX_STATIC_CACHE_SIZE = 1024 * 1024 * 10;
			static const GAIA::U64 DEFAULT_MAX_STATIC_CACHE_COUNT = 1024 * 1024;
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
				bEnableAutoResponseStaticResource = GAIA::True;
				GAIA::ALGO::gstrcpy(szHttpVer, GAIA::NETWORK::HTTP_VERSION_STRING);
				sHttpVerLen = GAIA::ALGO::gstrlen(szHttpVer);
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
				return GAIA::True;
			}

		public:

			/*!
				@brief Specify network thread count.

					Default value is DEFAULT_NETWORK_THREAD_COUNT.
					You can call HttpServerDesc::reset function to reset to default value.

				@see DEFAULT_NETWORK_THREAD_COUNT.
			*/
			GAIA::NUM sNetworkThreadCount;

			/*!
				@brief Specify work thread count.
			*/
			GAIA::NUM sWorkThreadCount;

			/*!
				@brief Specify HttpServer's root path.
			*/
			const GAIA::CH* pszRootPath;

			/*!
				@brief Specify HttpServer's max connection count at same time.
			*/
			GAIA::NUM sMaxConnCount;

			/*!
				@brief
			*/
			GAIA::U64 uMaxConnTime;

			/*!
				@brief
			*/
			GAIA::U64 uMaxHarfConnTime;

			/*!
				@brief
			*/
			GAIA::U64 uMaxDynamicCacheSize;

			/*!
				@brief
			*/
			GAIA::U64 uMaxDynamicCacheCount;

			/*!
				@brief
			*/
			GAIA::U64 uMaxStaticCacheSize;

			/*!
				@brief
			*/
			GAIA::U64 uMaxStaticCacheCount;

			/*!
				@brief
			*/
			GAIA::U64 uMaxResponseCountPerMinute;

			/*!
				@brief
			*/
			GAIA::BL bEnableAutoResponseStaticResource;

			/*!
				@brief
			*/
			GAIA::CH szHttpVer[16];

			/*!
				@brief
			*/
			GAIA::NUM sHttpVerLen;
		};

		/*!
			@brief
		*/
		class HttpServerStatus : public GAIA::Base
		{
		public:

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL GAIA::GVOID reset()
			{
				uServerStartupTime = 0;

				uLinkLifeTime = 0;
				uLinkLifeCount = 0;

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
				@brief
			*/
			GAIA::U64 uServerStartupTime;

			/*!
				@brief
			*/
			GAIA::U64 uLinkLifeTime;

			/*!
				@brief
			*/
			GAIA::U64 uLinkLifeCount;

			/*!
				@brief
			*/
			GAIA::U64 uRequestAnalyzeFailedCount;

			/*!
				@brief
			*/
			GAIA::U64 uRequestDenyByBWCount;

			/*!
				@brief
			*/
			GAIA::U64 uRequestDenyByMaxConnCount;

			/*!
				@brief
			*/
			GAIA::U64 uRequestDenyByMaxHalfConnCount;

			/*!
				@brief
			*/
			GAIA::U64 uRequestCount;

			/*!
				@brief
			*/
			GAIA::U64 uResponseCount;

			/*!
				@brief
			*/
			GAIA::U64 uRequestPieceCount;

			/*!
				@brief
			*/
			GAIA::U64 uResponsePieceCount;

			/*!
				@brief
			*/
			GAIA::U64 uRequestSize;

			/*!
				@brief
			*/
			GAIA::U64 uResponseSize;

			/*!
				@brief
			*/
			GAIA::U64 uRequestCountByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief
			*/
			GAIA::U64 uResponseCountByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief
			*/
			GAIA::U64 uRequestPieceCountByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief
			*/
			GAIA::U64 uResponsePieceCountByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief
			*/
			GAIA::U64 uRequestSizeByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief
			*/
			GAIA::U64 uResponseSizeByMethod[GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT];

			/*!
				@brief
			*/
			GAIA::U64 uResponseCountByCode[GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT];

			/*!
				@brief
			*/
			GAIA::U64 uHitResponseCacheCount;

			/*!
				@brief
			*/
			GAIA::U64 uHitResponseCacheSize;

			/*!
				@brief
			*/
			GAIA::U64 uNotHitResponseCacheCount;

			/*!
				@brief
			*/
			GAIA::U64 uNotResponseCount;
		};

		class HttpAsyncSocket;
		class HttpAsyncDispatcher;

		/*!
			@brief
		*/
		class HttpServerLink : public GAIA::RefObject
		{
			friend class HttpServer;
			friend class HttpAsyncSocket;
			friend class HttpAsyncDispatcher;

		public:

			/*!
				@brief Constructor.
			*/
			HttpServerLink(GAIA::NETWORK::HttpServer& svr);

			/*!
				@brief Destructor.
			*/
			~HttpServerLink();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL GAIA::NETWORK::HttpServer& GetServer() const{return *m_pSvr;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL const GAIA::NETWORK::Addr& GetPeerAddr() const{return m_addrPeer;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL const GAIA::NETWORK::Addr& GetListenAddr() const{return m_addrListen;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL const GAIA::U64& GetAcceptTime() const{return m_uAcceptTime;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Response(GAIA::NETWORK::HTTP_CODE httpcode, GAIA::NETWORK::HttpHead* pHttpHead = GNIL, const GAIA::GVOID* p = GNIL, GAIA::NUM sSize = 0);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Close();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL GAIA::NUM GetRequestTimes() const{return m_sRequestTimes;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL GAIA::NUM GetResponseTimes() const{return m_sResponseTimes;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL GAIA::N32 compare(const HttpServerLink& src) const
			{
				GAST(m_addrPeer.check());
				GAST(src.m_addrPeer.check());
				return m_addrPeer.compare(src.m_addrPeer);
			}
			GCLASS_COMPARE_BYCOMPARE(HttpServerLink)

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
			}
			GINL GAIA::GVOID SetPeerAddr(const GAIA::NETWORK::Addr& addrPeer){m_addrPeer = addrPeer;}
			GINL GAIA::GVOID SetListenAddr(const GAIA::NETWORK::Addr& addrListen){m_addrListen = addrListen;}
			GINL GAIA::GVOID SetAcceptTime(GAIA::U64 uTime){m_uAcceptTime = uTime;}
			GINL GAIA::GVOID SetRequestTimes(GAIA::NUM sRequestTimes){m_sRequestTimes = sRequestTimes;}
			GINL GAIA::GVOID SetResponseTimes(GAIA::NUM sResponseTimes){m_sResponseTimes = sResponseTimes;}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
			HttpAsyncSocket* m_pSock;
			GAIA::NETWORK::Addr m_addrPeer;
			GAIA::NETWORK::Addr m_addrListen;
			GAIA::U64 m_uAcceptTime;
			GAIA::NUM m_sRequestTimes;
			GAIA::NUM m_sResponseTimes;
		};

		class HttpServerCallBack : public GAIA::RefObject
		{
			friend class HttpServerLink;
			friend class HttpServer;
			friend class HttpAsyncSocket;
			friend class HttpAsyncDispatcher;

		public:
			/*!
				@brief Constructor.
			*/
			HttpServerCallBack(GAIA::NETWORK::HttpServer& svr);

			/*!
				@brief Destructor.
			*/
			virtual ~HttpServerCallBack();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL GAIA::NETWORK::HttpServer& GetServer() const{return *m_pSvr;}

		public:
			/*!
				@brief

				@param

				@return

				@remarks
			*/
			virtual const GAIA::CH* GetName() const{return "HttpServerCallBack";}

		protected:

			/*!
				@brief

				@param

				@return

				@remarks
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
			@brief
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
			@brief
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
		class HttpServer : public GAIA::Base
		{
			friend class HttpServerLink;
			friend class HttpServerWorkThread;
			friend class HttpAsyncSocket;
			friend class HttpAsyncDispatcher;

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
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL RegistCallBack(GAIA::NETWORK::HttpServerCallBack& cb);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL UnregistCallBack(GAIA::NETWORK::HttpServerCallBack& cb);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL UnregistCallBackAll();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsRegistedCallBack(GAIA::NETWORK::HttpServerCallBack& cb);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CollectCallBack(GAIA::CTN::Vector<GAIA::NETWORK::HttpServerCallBack*>& listResult);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Create(const GAIA::NETWORK::HttpServerDesc& desc);

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
			const GAIA::NETWORK::HttpServerDesc& GetDesc() const{return m_desc;}

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
			GAIA::BL Execute();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL OpenAddr(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CloseAddr(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CloseAddrAll();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsOpennedAddr(const GAIA::NETWORK::Addr& addr) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NUM GetOpennedAddrCount() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CollectOpennedAddr(GAIA::CTN::Vector<GAIA::NETWORK::Addr>& listResult);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableDynamicResponseCache(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableDynamicResponseCache() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableStaticResponseCache(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableStaticResponseCache() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetBlackWhiteMode(GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE mode);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE GetBlackWhiteMode() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID AddBlackList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uTime = GINVALID);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID RemoveBlackList(const GAIA::NETWORK::IP& ip);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID RemoveBlackListAll();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsInBlackList(const GAIA::NETWORK::IP& ip) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID AddWhiteList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uTime = GINVALID);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID RemoveWhiteList(const GAIA::NETWORK::IP& ip);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID RemoveWhiteListAll();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsInWhiteList(const GAIA::NETWORK::IP& ip) const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GINL GAIA::NETWORK::HttpServerStatus& GetStatus(){return m_status;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL RequestCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen, GAIA::NETWORK::HttpHead& resphead, GAIA::GVOID** p, GAIA::NUM& sSize);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL RequestCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead, GAIA::NETWORK::HttpHead& resphead, GAIA::GVOID** p, GAIA::NUM& sSize);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL ReleaseCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL ReleaseCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL UpdateCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen, const GAIA::NETWORK::HttpHead& resphead, const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::U64 uEffectTime = GINVALID);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL UpdateCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead, const GAIA::NETWORK::HttpHead& resphead, const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::U64 uEffectTime = GINVALID);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL RecycleCache();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NETWORK::HttpAsyncDispatcher* GetAsyncDispatcher() const{return m_disp;}

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
				m_bEnableDynamicResponseCache = GAIA::True;
				m_bEnableStaticResponseCache = GAIA::True;
				m_blackwhitemode = GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE_BLACK;
				m_disp = GNIL;
				m_status.reset();
			}

		private:
			class BWNode : public GAIA::Base // White and black node.
			{
			public:
				GINL GAIA::N32 compare(const BWNode& src) const{return ip.compare(src.ip);}
				GCLASS_COMPARE_BYCOMPARE(BWNode)

			public:
				GAIA::NETWORK::IP ip;
				GAIA::U64 uRegistTime; // Real time in microseconds.
				GAIA::U64 uEffectTime; // Relative time in microseconds.
			};

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

			GAIA::SYNC::LockRW m_rwLinks;
			__LinkSetType m_links_bypeeraddr;

			GAIA::SYNC::LockRW m_rwRCLinks; // RC means request complete.
			__LinkQueueType m_rclinks; // RC means request complete.

			GAIA::BL m_bEnableDynamicResponseCache;
			GAIA::BL m_bEnableStaticResponseCache;

			GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE m_blackwhitemode;
			GAIA::SYNC::LockRW m_rwBlackList;
			GAIA::CTN::Set<BWNode> m_BlackList;
			GAIA::SYNC::LockRW m_rwWhiteList;
			GAIA::CTN::Set<BWNode> m_WhiteList;

			GAIA::SYNC::LockRW m_rwCache;
			GAIA::CTN::Set<CacheNode> m_cache;

			GAIA::SYNC::Lock m_lrBufPool;
			GAIA::CTN::Pool<GAIA::CTN::Buffer> m_bufpool;

			GAIA::NETWORK::HttpAsyncDispatcher* m_disp;
			GAIA::CTN::Vector<GAIA::NETWORK::HttpServerWorkThread*> m_listWorkThreads;
			GAIA::NETWORK::HttpServerStatus m_status;
		};
	}
}

#endif
