#ifndef	 __GAIA_NETWORK_HTTP_H__
#define	 __GAIA_NETWORK_HTTP_H__

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
		class HttpRequest : public GAIA::RefObject
		{
			friend class Http;

		public:

			// Constructor and destructor.
			HttpRequest(GAIA::NETWORK::Http& http);
			~HttpRequest();
			Http& GetHttp();

			// Base http operation.
			GAIA::GVOID SetMethod(GAIA::NETWORK::HTTP_METHOD method);
			GAIA::NETWORK::HTTP_METHOD GetMethod() const;
			GAIA::GVOID SetURL(const GAIA::CH* pszURL);
			const GAIA::CH* GetURL() const;
			GAIA::GVOID SetHead(const GAIA::NETWORK::HttpHead& pHead);
			const GAIA::NETWORK::HttpHead& GetHead() const;
			GAIA::NETWORK::HttpHead& GetHead();

			// Buffer control.
			GAIA::BL BindBuffer(const GAIA::GVOID* p, GAIA::NUM sSize); // If p == GNIL, will allocate buffer.
			const GAIA::GVOID* GetBuffer() const;
			GAIA::NUM GetBufferSize() const;

			// Work flow control.
			GAIA::BL Request();
			GAIA::BL Cancel();
			GAIA::BL Pause();
			GAIA::BL Resume();
			GAIA::BL Wait(GAIA::U32 uMilliSeconds);

			// Query result.
			GAIA::BL IsComplete() const;
			GAIA::N64 GetResultCode() const;
			GAIA::NUM GetResultSize() const;

			// Mode control.
			GAIA::GVOID SetAsync(GAIA::BL bAsync);
			GAIA::BL GetAsync() const;
			GAIA::GVOID SetTimeout(const GAIA::U64& uTimeout);
			const GAIA::U64& GetTimeout() const;

			// Cookic control.
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

		protected:

			// Stream like read write callback.
			virtual GAIA::GVOID OnBegin(){}
			virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled){}
			virtual GAIA::BL OnWrite(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize){return GAIA::False;}
			virtual GAIA::BL OnRead(GAIA::N64 lOffset, GAIA::GVOID* pData, GAIA::NUM sMaxDataSize, GAIA::NUM& sPracticeDataSize){return GAIA::False;}

		private:
			GAIA::NETWORK::HttpURL m_url;
			GAIA::NETWORK::HttpHead m_head;

		};

		class HttpDesc : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_MAX_PARALLEL_COUNT = 1000;
			static const GAIA::NUM DEFAULT_MAX_COOKIC_COUNT = 10000;
			static const GAIA::NUM DEFAULT_MAX_COOKIC_SIZE = 10000;

		public:
			GAIA::GVOID reset()
			{
				sThreadCount = DEFAULT_THREAD_COUNT;
				sMaxParallelCount = DEFAULT_MAX_PARALLEL_COUNT;
				sMaxCookicCount = DEFAULT_MAX_COOKIC_COUNT;
				sMaxCookicSize = DEFAULT_MAX_COOKIC_SIZE;
			}
			GAIA::BL check() const
			{
				if(sThreadCount <= 0)
					return GAIA::False;
				if(sMaxParallelCount <= 0)
					return GAIA::False;
				if(sMaxCookicCount < 0)
					return GAIA::False;
				if(sMaxCookicSize < 0)
					return GAIA::False;
				return GAIA::True;
			}
		public:
			GAIA::NUM sThreadCount;
			GAIA::NUM sMaxParallelCount;
			GAIA::NUM sMaxCookicCount;
			GAIA::NUM sMaxCookicSize;
		};

		class Http : public GAIA::Base
		{
			friend class HttpRequest;

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
			GAIA::BL m_bCreated;
			HttpDesc m_desc;
		};
	}
}

#endif
