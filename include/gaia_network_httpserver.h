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
		static const GAIA::CH* HTTP_VERSION_STRING = "HTTP/1.1";

		GAIA_ENUM_BEGIN(HTTP_SERVER_BLACKWHITE_MODE)
			HTTP_SERVER_BLACKWHITE_MODE_BLACK, // Default value.
			HTTP_SERVER_BLACKWHITE_MODE_WHITE,
			HTTP_SERVER_BLACKWHITE_MODE_NONE,
		GAIA_ENUM_END(HTTP_SERVER_BLACKWHITE_MODE)

		class HttpServerDesc : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_MAX_CONN_COUNT = 10000;
			static const GAIA::U64 DEFAULT_MAX_CONN_TIME = (GAIA::U64)1000 * 1000 * 3600 * 24;
			static const GAIA::U64 DEFAULT_MAX_HALFCONN_TIME = 1000 * 1000 * 10;
			static const GAIA::U64 DEFAULT_MAX_DYNAMIC_CACHE_SIZE = 1024 * 1024 * 1024;
			static const GAIA::U64 DEFAULT_MAX_DYNAMIC_CACHE_COUNT = 1024 * 1024;
			static const GAIA::U64 DEFAULT_MAX_STATIC_CACHE_SIZE = 1024 * 1024 * 10;
			static const GAIA::U64 DEFAULT_MAX_STATIC_CACHE_COUNT = 1024 * 1024;
			static const GAIA::U64 DEFAULT_MAX_RESPONSE_COUNT_PER_MINUTE = 100000;

		public:
			GINL GAIA::GVOID reset()
			{
				sThreadCount = DEFAULT_THREAD_COUNT;
				pszRootPath = GNILSTR;
				sMaxConnCount = DEFAULT_MAX_CONN_COUNT;
				uMaxConnTime = DEFAULT_MAX_CONN_TIME;
				uMaxHarfConnTime = DEFAULT_MAX_HALFCONN_TIME;
				uMaxDynamicCacheSize = DEFAULT_MAX_DYNAMIC_CACHE_SIZE;
				uMaxDynamicCacheCount = DEFAULT_MAX_DYNAMIC_CACHE_COUNT;
				uMaxStaticCacheSize = DEFAULT_MAX_STATIC_CACHE_SIZE;
				uMaxStaticCacheCount = DEFAULT_MAX_STATIC_CACHE_COUNT;
				uMaxResponseCountPerMinute = DEFAULT_MAX_RESPONSE_COUNT_PER_MINUTE;
				bEnableAutoResponseStaticFile = GAIA::True;
				GAIA::ALGO::gstrcpy(szHttpVer, GAIA::NETWORK::HTTP_VERSION_STRING);
				sHttpVerLen = GAIA::ALGO::gstrlen(szHttpVer);
			}
			GINL GAIA::BL check() const
			{
				if(sThreadCount <= 0)
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
			GAIA::NUM sThreadCount;
			const GAIA::CH* pszRootPath;
			GAIA::NUM sMaxConnCount;
			GAIA::U64 uMaxConnTime;
			GAIA::U64 uMaxHarfConnTime;
			GAIA::U64 uMaxDynamicCacheSize;
			GAIA::U64 uMaxDynamicCacheCount;
			GAIA::U64 uMaxStaticCacheSize;
			GAIA::U64 uMaxStaticCacheCount;
			GAIA::U64 uMaxResponseCountPerMinute;
			GAIA::BL bEnableAutoResponseStaticFile;
			GAIA::CH szHttpVer[16];
			GAIA::NUM sHttpVerLen;
		};

		class HttpServerStatus : public GAIA::Base
		{
		public:
			GINL GAIA::GVOID reset()
			{
				uRequestAnalyzeFailedCount = 0;
				uRequestDenyByBWCount = 0;
				uRequestDenyByMaxConnCount = 0;
				uRequestDenyByMaxHalfConnCount = 0;

				uRequestPutCount = 0;
				uRequestPostCount = 0;
				uRequestGetCount = 0;
				uRequestHeadCount = 0;
				uRequestDeleteCount = 0;

				uResponsePutCount = 0;
				uResponsePostCount = 0;
				uResponseGetCount = 0;
				uResponseHeadCount = 0;
				uResponseDeleteCount = 0;

				u200Count = 0;
				u400Count = 0;
				u401Count = 0;
				u403Count = 0;
				u404Count = 0;
				u500Count = 0;
				u503Count = 0;
			}

		public:
			GAIA::U64 uRequestAnalyzeFailedCount;
			GAIA::U64 uRequestDenyByBWCount;
			GAIA::U64 uRequestDenyByMaxConnCount;
			GAIA::U64 uRequestDenyByMaxHalfConnCount;

			GAIA::U64 uRequestPutCount;
			GAIA::U64 uRequestPostCount;
			GAIA::U64 uRequestGetCount;
			GAIA::U64 uRequestHeadCount;
			GAIA::U64 uRequestDeleteCount;

			GAIA::U64 uResponsePutCount;
			GAIA::U64 uResponsePostCount;
			GAIA::U64 uResponseGetCount;
			GAIA::U64 uResponseHeadCount;
			GAIA::U64 uResponseDeleteCount;

			GAIA::U64 u200Count;
			GAIA::U64 u400Count;
			GAIA::U64 u401Count;
			GAIA::U64 u403Count;
			GAIA::U64 u404Count;
			GAIA::U64 u500Count;
			GAIA::U64 u503Count;
		};

		class HttpAsyncSocket;
		class HttpAsyncDispatcher;

		class HttpServerLink : public GAIA::Base
		{
			friend class HttpServer;
			friend class HttpAsyncSocket;
			friend class HttpAsyncDispatcher;

		public:
			HttpServerLink(GAIA::NETWORK::HttpServer& svr);
			~HttpServerLink();

			GINL GAIA::NETWORK::HttpServer& GetServer() const{return *m_pSvr;}
			GINL GAIA::NETWORK::HttpAsyncSocket& GetAsyncSocket() const{return *m_pSock;}
			GINL const GAIA::NETWORK::Addr& GetPeerAddr() const{return m_addrPeer;}
			GINL const GAIA::U64& GetAcceptTime() const{return m_uAcceptTime;}
			GAIA::BL Response(GAIA::NETWORK::HTTP_CODE httpcode, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p = GNIL, GAIA::NUM sSize = 0, const GAIA::U64& uCacheTime = GINVALID);
			GAIA::BL Close();

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
				m_uAcceptTime = GINVALID;
			}
			GINL GAIA::GVOID SetAsyncSocket(GAIA::NETWORK::HttpAsyncSocket& sock){m_pSock = &sock;}
			GINL GAIA::GVOID SetPeerAddr(const GAIA::NETWORK::Addr& addrPeer){m_addrPeer = addrPeer;}
			GINL GAIA::GVOID SetAcceptTime(GAIA::U64 uTime){m_uAcceptTime = uTime;}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
			HttpAsyncSocket* m_pSock;
			GAIA::NETWORK::Addr m_addrPeer;
			GAIA::U64 m_uAcceptTime;
		};

		class HttpServerCallBack : public GAIA::RefObject
		{
			friend class HttpServerLink;
			friend class HttpServer;
			friend class HttpAsyncSocket;
			friend class HttpAsyncDispatcher;

		public:
			HttpServerCallBack(GAIA::NETWORK::HttpServer& svr);
			virtual ~HttpServerCallBack();

			GINL GAIA::NETWORK::HttpServer& GetServer() const{return *m_pSvr;}

			GINL const GAIA::NETWORK::HttpServerStatus& GetStatus() const{return m_status;}

		protected:
			virtual GAIA::BL OnRecv(GAIA::NETWORK::HttpServerLink& l, const GAIA::GVOID* p, GAIA::NUM sSize){return GAIA::False;}
			virtual GAIA::BL OnPut(GAIA::NETWORK::HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize){return GAIA::False;}
			virtual GAIA::BL OnPost(GAIA::NETWORK::HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize){return GAIA::False;}
			virtual GAIA::BL OnGet(GAIA::NETWORK::HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead){return GAIA::False;}
			virtual GAIA::BL OnHead(GAIA::NETWORK::HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead){return GAIA::False;}
			virtual GAIA::BL OnDelete(GAIA::NETWORK::HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead){return GAIA::False;}

		protected:
			GINL HttpServerStatus& GetStatus(){return m_status;}

		private:
			GINL GAIA::GVOID init()
			{
				m_pSvr = GNIL;
				m_status.reset();
			}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
			GAIA::NETWORK::HttpServerStatus m_status;
		};

		class HttpServer : public GAIA::Base
		{
			friend class HttpServerLink;
			friend class HttpAsyncSocket;
			friend class HttpAsyncDispatcher;

		public:
			HttpServer();
			~HttpServer();

			GAIA::BL RegistCallBack(GAIA::NETWORK::HttpServerCallBack& cb);
			GAIA::BL UnregistCallBack(GAIA::NETWORK::HttpServerCallBack& cb);
			GAIA::BL UnregistCallBackAll();
			GAIA::BL IsRegistedCallBack(GAIA::NETWORK::HttpServerCallBack& cb);

			GAIA::BL Create(const GAIA::NETWORK::HttpServerDesc& desc);
			GAIA::BL Destroy();
			GAIA::BL IsCreated() const{return m_bCreated;}
			const GAIA::NETWORK::HttpServerDesc& GetDesc() const{return m_desc;}

			GAIA::BL Begin();
			GAIA::BL End();
			GAIA::BL IsBegin() const{return m_bBegin;}

			GAIA::BL OpenAddr(const GAIA::NETWORK::Addr& addr);
			GAIA::BL CloseAddr(const GAIA::NETWORK::Addr& addr);
			GAIA::BL CloseAddrAll();
			GAIA::BL IsOpennedAddr(const GAIA::NETWORK::Addr& addr) const;
			GAIA::NUM GetOpennedAddrCount() const;
			const GAIA::NETWORK::Addr* GetOpennedAddr(GAIA::NUM sIndex) const;

			GAIA::GVOID EnableDynamicResponseCache(GAIA::BL bEnable);
			GAIA::BL IsEnableDynamicResponseCache() const;
			GAIA::GVOID EnableStaticResponseCache(GAIA::BL bEnable);
			GAIA::BL IsEnableStaticResponseCache() const;

			GAIA::GVOID SetBlackWhiteMode(GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE mode);
			GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE GetBlackWhiteMode() const;
			GAIA::GVOID AddBlackList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uTime = GINVALID);
			GAIA::GVOID RemoveBlackList(const GAIA::NETWORK::IP& ip);
			GAIA::GVOID RemoveBlackListAll();
			GAIA::BL IsInBlackList(const GAIA::NETWORK::IP& ip) const;
			GAIA::GVOID AddWhiteList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uTime = GINVALID);
			GAIA::GVOID RemoveWhiteList(const GAIA::NETWORK::IP& ip);
			GAIA::GVOID RemoveWhiteListAll();
			GAIA::BL IsInWhiteList(const GAIA::NETWORK::IP& ip) const;

			GINL const GAIA::NETWORK::HttpServerStatus& GetStatus() const{return m_status;}

		private:
			GAIA::BL RequestCache(const GAIA::CH* pszUrlAndHead, GAIA::NUM sHeadLen, GAIA::GVOID** p, GAIA::NUM& sSize);
			GAIA::BL ReleaseCache(const GAIA::CH* pszUrlAndHead, GAIA::NUM sHeadLen);
			GAIA::BL UpdateCache(const GAIA::CH* pszUrlAndHead, GAIA::NUM sHeadLen, const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::U64 uEffectTime = GINVALID);
			GAIA::BL RecycleCache();

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

			GINL GAIA::NETWORK::HttpServerStatus& GetStatus(){return m_status;}

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
				GINL GAIA::N32 compare(const CacheNode& src) const{return strUrlAndHead.compare(src.strUrlAndHead);}
				GCLASS_COMPARE_BYCOMPARE(CacheNode)

			public:
				GAIA::CTN::AString strUrlAndHead;
				GAIA::CTN::Buffer* buf;
				GAIA::U64 uRegistTime;
				GAIA::U64 uEffectTime;
				GAIA::N64 nRefCount;
			};

		private:
			typedef GAIA::CTN::Set<GAIA::CTN::Ref<GAIA::NETWORK::HttpServerLink> > __LinkSetType;
			typedef GAIA::CTN::Vector<GAIA::NETWORK::HttpServerLink*> __LinkVectorType;

		private:
			GAIA::NETWORK::HttpServerDesc m_desc;
			GAIA::SYNC::LockRW m_rwCBS;
			GAIA::CTN::Vector<GAIA::NETWORK::HttpServerCallBack*> m_cbs;
			GAIA::BL m_bCreated;
			GAIA::BL m_bBegin;

			GAIA::SYNC::LockRW m_rwLinks;
			__LinkSetType m_links_bypeeraddr;

			GAIA::SYNC::LockRW m_rwRCLinks; // RC means request complete.
			__LinkVectorType m_rclinks; // RC means request complete.

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

			HttpAsyncDispatcher* m_disp;
			GAIA::NETWORK::HttpServerStatus m_status;
		};
	}
}

#endif
