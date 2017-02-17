#ifndef	 __GAIA_NETWORK_HTTP_H__
#define	 __GAIA_NETWORK_HTTP_H__

#include "gaia_type.h"
#include "gaia_assert.h"
//#include "gaia_sync_lock.h"
//#include "gaia_sync_autolock.h"
//#include "gaia_sync_lockrw.h"
//#include "gaia_sync_autolockr.h"
//#include "gaia_sync_autolockw.h"
//#include "gaia_ctn_ref.h"
//#include "gaia_ctn_vector.h"
//#include "gaia_ctn_queue.h"
//#include "gaia_ctn_buffer.h"
//#include "gaia_ctn_pool.h"
//#include "gaia_ctn_list.h"
//#include "gaia_ctn_set.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
//#include "gaia_network_base.h"
//#include "gaia_network_socket.h"
#include "gaia_network_asyncsocket.h"
#include "gaia_network_asyncdispatcher.h"
#include "gaia_network_httpbase.h"

namespace GAIA
{
	namespace NETWORK
	{
		class HttpRequest : public GAIA::Base
		{
			friend class Http;

		public:
			HttpRequest();
			~HttpRequest();

			GAIA::GVOID SetAsync(GAIA::BL bAsync);
			GAIA::BL GetAsync() const;
			GAIA::GVOID SetTimeout(const GAIA::U64& uTimeout);
			const GAIA::U64& GetTimeout() const;
			GAIA::GVOID EnableWriteCookicRAM(GAIA::BL bEnable);
			GAIA::BL IsEnableWriteCookicRAM() const;
			GAIA::GVOID EnableWriteCookicFile(GAIA::BL bEnable);
			GAIA::BL IsEnableWriteCookicFile() const;
			GAIA::GVOID SetWriteCookicTime(const GAIA::U64& uTime);
			const GAIA::U64& GetWriteCookicTime() const;
			GAIA::GVOID EnableReadCookicRAM(GAIA::BL bEnable);
			GAIA::BL IsEnableReadCookicRAM() const;
			GAIA::GVOID EnableReadCookicFile(GAIA::BL bEnable);
			GAIA::BL IsEnableReadCookicFile() const;
			GAIA::GVOID SetReadCookicTime(const GAIA::U64& uTime);
			const GAIA::U64& GetReadCookicTime() const;
			GAIA::GVOID SetURL(const GAIA::CH* pszURL);
			const GAIA::CH* GetURL() const;
			GAIA::GVOID SetHead(const GAIA::NETWORK::HttpHead* pHead);
			const GAIA::NETWORK::HttpHead* GetHead() const;

		private:
			GAIA::CH* m_pszURL;
			GAIA::NETWORK::HttpHead* m_pHead;
		};

		class HttpResponse : public GAIA::RefObject
		{
			friend class Http;

		public:
			HttpResponse(){}
			~HttpResponse(){}

			const GAIA::NETWORK::HttpRequest* GetRequest() const;
			const GAIA::NETWORK::HttpHead* GetHead() const;
			GAIA::N16 GetResultCode() const;

		public:
			virtual GAIA::BL OnProgress(const GAIA::GVOID* pData, GAIA::NUM sDataSize){return GAIA::False;}
			virtual GAIA::GVOID OnBegin(){}
			virtual GAIA::GVOID OnComplete(const GAIA::GVOID* pData, GAIA::NUM sDataSize){}
			virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled){}

		private:
			GAIA::GVOID SetRequest(const GAIA::NETWORK::HttpRequest* pRequest);
			GAIA::GVOID SetHead(const GAIA::NETWORK::HttpHead* pHead);
			GAIA::GVOID SetResultCode(GAIA::N64 nResultCode);

		private:
			GAIA::N16 m_nResultCode;
		};

		class HttpDesc : public GAIA::Base
		{
		public:
			GAIA::GVOID reset(){}
			GAIA::BL check() const
			{
				return GAIA::True;
			}
		public:
			GAIA::NUM sMaxThreadCount;
			GAIA::NUM sMaxParallelCount;
			GAIA::NUM sMaxCookicCount;
			GAIA::NUM sMaxCookicSize;
		};

		class Http : public GAIA::Base
		{
		public:
			Http();
			~Http();

			GAIA::BL Create(const GAIA::NETWORK::HttpDesc& desc);
			GAIA::BL Destroy();
			GAIA::BL IsCreated() const;
			const GAIA::NETWORK::HttpDesc& GetDesc() const;

			GAIA::BL Begin();
			GAIA::BL End();
			GAIA::BL IsBegin() const;

			GAIA::GVOID Put(const GAIA::NETWORK::HttpRequest& req, GAIA::NETWORK::HttpResponse& resp, const GAIA::GVOID* pData, GAIA::NUM sDataSize);
			GAIA::GVOID Post(const GAIA::NETWORK::HttpRequest& req, GAIA::NETWORK::HttpResponse& resp, const GAIA::GVOID* pData, GAIA::NUM sDataSize);
			GAIA::GVOID Get(const GAIA::NETWORK::HttpRequest& req, GAIA::NETWORK::HttpResponse& resp);
			GAIA::GVOID Head(const GAIA::NETWORK::HttpRequest& req, GAIA::NETWORK::HttpResponse& resp);
			GAIA::GVOID Delete(const GAIA::NETWORK::HttpRequest& req, GAIA::NETWORK::HttpResponse& resp);
			GAIA::GVOID Cancel(GAIA::NETWORK::HttpResponse& resp);

			GAIA::GVOID EnableWriteCookicRAM(GAIA::BL bEnable);
			GAIA::BL IsEnableWriteCookicRAM() const;
			GAIA::GVOID EnableWriteCookicFile(GAIA::BL bEnable);
			GAIA::BL IsEnableWriteCookicFile() const;
			GAIA::GVOID EnableReadCookicRAM(GAIA::BL bEnable);
			GAIA::BL IsEnableReadCookicRAM() const;
			GAIA::GVOID EnableReadCookicFile(GAIA::BL bEnable);
			GAIA::BL IsEnableReadCookicFile() const;
			GAIA::BL CleanupCookic(GAIA::BL bMem = GAIA::True, GAIA::BL bFile = GAIA::True, const GAIA::U64& uBeyondTime = 0);

		private:
		};
	}
}

#endif
