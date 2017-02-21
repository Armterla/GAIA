#ifndef	 __GAIA_NETWORK_HTTPSERVER_H__
#define	 __GAIA_NETWORK_HTTPSERVER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_sync_lockrw.h"
#include "gaia_sync_autolockr.h"
#include "gaia_sync_autolockw.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_buffer.h"
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
		class HttpServerDesc : public GAIA::Base
		{
		public:
			GINL GAIA::GVOID reset(){}
			GINL GAIA::BL check() const{return GAIA::True;}

		public:
		};

		class HttpServerStatus : public GAIA::Base
		{
		public:
			GINL GAIA::GVOID reset()
			{
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

		class HttpServerLink : public GAIA::Base
		{
			friend class HttpServer;

		public:
			HttpServerLink(){this->init();}

			const HttpServerStatus& GetStatus() const;

			GINL GAIA::N32 compare(const HttpServerLink& src) const{}
			GCLASS_COMPARE_BYCOMPARE(HttpServerLink)

		private:
			GINL GAIA::GVOID init(){}

		private:
			GAIA::NETWORK::AsyncSocket* pSock;
		};

		class HttpServerCallBack : public GAIA::Base
		{
		public:
			HttpServerCallBack();
			~HttpServerCallBack();

		public:
			const HttpServerStatus& GetStatus() const;

		protected:
			virtual GAIA::BL OnRecv(HttpServerLink& l, const GAIA::GVOID* p, GAIA::NUM sSize){return GAIA::False;}
			virtual GAIA::BL OnPut(HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize){return GAIA::False;}
			virtual GAIA::BL OnPost(HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize){return GAIA::False;}
			virtual GAIA::BL OnGet(HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead){return GAIA::False;}
			virtual GAIA::BL OnHead(HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead){return GAIA::False;}
			virtual GAIA::BL OnDelete(HttpServerLink& l, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead){return GAIA::False;}

		protected:
			HttpServerStatus& GetStatus();

		private:
			GAIA::GVOID init();

		private:
		};

		class HttpServer : public GAIA::Base
		{
		public:
			HttpServer();
			~HttpServer();

			GAIA::BL RegistCallBack(HttpServerCallBack& cb);
			GAIA::BL UnregistCallBack(HttpServerCallBack& cb);
			GAIA::BL UnregistCallBackAll();
			GAIA::BL IsRegistedCallBack(HttpServerCallBack& cb);

			GAIA::BL Create(const HttpServerDesc& desc);
			GAIA::BL Destroy();
			GAIA::BL IsCreated() const;
			const HttpServerDesc& GetDesc() const;

			GAIA::BL Startup();
			GAIA::BL Shutdown();
			GAIA::BL IsStartuped() const;

			GAIA::BL OpenAddr(const GAIA::NETWORK::Addr& addr);
			GAIA::BL CloseAddr(const GAIA::NETWORK::Addr& addr);
			GAIA::BL CloseAddrAll();
			GAIA::BL IsOpennedAddr() const;
			GAIA::NUM GetOpennedAddrCount() const;
			const GAIA::NETWORK::Addr* GetOpennedAddr(GAIA::NUM sIndex) const;

			const HttpServerStatus& GetStatus() const;

		private:
			GAIA::NETWORK::AsyncDispatcher m_disp;
		};
	}
}

#endif
