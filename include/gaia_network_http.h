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
			/*!
				@brief Constructor.
			*/
			HttpRequest(GAIA::NETWORK::Http& http);

			/*!
				@brief Destructor.
			*/
			virtual ~HttpRequest();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			Http& GetHttp();

			// Base http operation.
			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetMethod(GAIA::NETWORK::HTTP_METHOD method);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NETWORK::HTTP_METHOD GetMethod() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetURL(const GAIA::CH* pszURL);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			const GAIA::CH* GetURL() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetHead(const GAIA::NETWORK::HttpHead& pHead);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			const GAIA::NETWORK::HttpHead& GetHead() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NETWORK::HttpHead& GetHead();

			// Buffer control.
			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL BindBuffer(const GAIA::GVOID* p, GAIA::NUM sSize); // If p == GNIL, will allocate buffer.

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			const GAIA::GVOID* GetBuffer() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NUM GetBufferSize() const;

			// Work flow control.
			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Request();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Cancel();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Pause();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Resume();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Wait(GAIA::U32 uMilliSeconds);

			// Query result.
			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsComplete() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::N64 GetResultCode() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::NUM GetResultSize() const;

			// Mode control.
			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetAsync(GAIA::BL bAsync);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL GetAsync() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetTimeout(const GAIA::U64& uTimeout);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			const GAIA::U64& GetTimeout() const;

			// Cookic control.
			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableWriteCookicRAM(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableWriteCookicRAM() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableWriteCookicFile(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableWriteCookicFile() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetWriteCookicTime(const GAIA::U64& uTime);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			const GAIA::U64& GetWriteCookicTime() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableReadCookicRAM(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableReadCookicRAM() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableReadCookicFile(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableReadCookicFile() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetReadCookicTime(const GAIA::U64& uTime);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			const GAIA::U64& GetReadCookicTime() const;

		protected:

			// Stream like read write callback.
			/*!
				@brief

				@param

				@return

				@remarks
			*/
			virtual GAIA::GVOID OnBegin(){}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled){}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			virtual GAIA::BL OnWrite(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize){return GAIA::False;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			virtual GAIA::BL OnRead(GAIA::N64 lOffset, GAIA::GVOID* pData, GAIA::NUM sMaxDataSize, GAIA::NUM& sPracticeDataSize){return GAIA::False;}

		private:
			GAIA::NETWORK::HttpURL m_url;
			GAIA::NETWORK::HttpHead m_head;

		};

		/*!
			@brief
		*/
		class HttpDesc : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_MAX_PARALLEL_COUNT = 1000;
			static const GAIA::NUM DEFAULT_MAX_COOKIC_COUNT = 10000;
			static const GAIA::NUM DEFAULT_MAX_COOKIC_SIZE = 10000;

		public:

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID reset()
			{
				sThreadCount = DEFAULT_THREAD_COUNT;
				sMaxParallelCount = DEFAULT_MAX_PARALLEL_COUNT;
				sMaxCookicCount = DEFAULT_MAX_COOKIC_COUNT;
				sMaxCookicSize = DEFAULT_MAX_COOKIC_SIZE;
			}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
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

			/*!
				@brief
			*/
			GAIA::NUM sThreadCount;

			/*!
				@brief
			*/
			GAIA::NUM sMaxParallelCount;

			/*!
				@brief
			*/
			GAIA::NUM sMaxCookicCount;

			/*!
				@brief
			*/
			GAIA::NUM sMaxCookicSize;
		};

		/*!
			@brief
		*/
		class Http : public GAIA::Base
		{
			friend class HttpRequest;

		public:

			/*!
				@brief Constructor.
			*/
			Http();

			/*!
				@brief Destructor.
			*/
			~Http();

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL Create(const GAIA::NETWORK::HttpDesc& desc);

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
			GAIA::BL IsCreated() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			const GAIA::NETWORK::HttpDesc& GetDesc() const;

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
			GAIA::BL IsBegin() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableWriteCookicRAM(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableWriteCookicRAM() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableWriteCookicFile(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableWriteCookicFile() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableReadCookicRAM(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableReadCookicRAM() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableReadCookicFile(GAIA::BL bEnable);

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableReadCookicFile() const;

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CleanupCookic(GAIA::BL bMem = GAIA::True, GAIA::BL bFile = GAIA::True, const GAIA::U64& uBeyondTime = 0);

		private:
			GAIA::BL m_bCreated;
			HttpDesc m_desc;
		};
	}
}

#endif
