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
		class HttpAsyncSocket;
		class HttpAsyncDispatcher;

		/*!
			@brief Http request information class.
				It is a reference object.
		*/
		class HttpRequest : public GAIA::RefObject
		{
			friend class HttpAsyncSocket;
			friend class HttpAsyncDispatcher;
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
				@brief Get Http object reference.

				@return Return the Http object reference.
			*/
			Http& GetHttp();

			/*!
				@brief Reset the request, the request will like a new allocated object.
			*/
			GAIA::GVOID Reset();

			// Base http operation.
			/*!
				@brief Set http request method.

				@param method [in] Specify the method.

				@remarks
			*/
			GAIA::GVOID SetMethod(GAIA::NETWORK::HTTP_METHOD method){m_method = method;}

			/*!
				@brief Get http request method.

				@return Return http request method.
			*/
			GAIA::NETWORK::HTTP_METHOD GetMethod() const{return m_method;}

			/*!
				@brief Set http request url.

				@param url [in] Specify the url.

				@remarks
			*/
			GAIA::GVOID SetURL(const GAIA::NETWORK::HttpURL& url){m_url = url;}

			/*!
				@brief Get http request url.

				@return Return http request url.

				@remarks
			*/
			const GAIA::NETWORK::HttpURL& GetURL() const{return m_url;}

			/*!
				@brief Get http request url.

				@return Return http request url.

				@remarks
			*/
			GAIA::NETWORK::HttpURL& GetURL(){return m_url;}

			/*!
				@brief Set http request head.

				@param head [in] Specify http head.

				@remarks
			*/
			GAIA::GVOID SetHead(const GAIA::NETWORK::HttpHead& head){m_head = head;}

			/*!
				@brief Get http request head.

				@return Return http request head.

				@remarks
			*/
			const GAIA::NETWORK::HttpHead& GetHead() const{return m_head;}

			/*!
				@brief Get http request head.

				@return Return http request head.

				@remarks
			*/
			GAIA::NETWORK::HttpHead& GetHead(){return m_head;}

			// Buffer control.
			/*!
				@brief Bind a buffer to request.

				@param p [in] Specify the buffer.

				@param sSize [in] Specify the buffer's size in bytes.

				@return If bind buffer successfully, return GAIA::True, or return GAIA::False.

				@remarks The bound buffer will be send to server after send the head, so it could be a http body.
					If parameter p is not GNIL and sSize above 0, current function will reference the buffer and the caller must keep the buffer available until http request complete.\n
					If parameter p is GNIL and sSize above 0, current function will allocate buffer internal.\n
					If parameter p is GNIL and sSize equal zero, current function will unreference the buffer or destory the internal buffer.\n
			*/
			GAIA::BL BindBuffer(const GAIA::GVOID* p, GAIA::NUM sSize);

			/*!
				@brief Get the buffer for request.

				@return If the buffer exist, return a valid pointer, or will return GNIL.

				@remarks
			*/
			const GAIA::GVOID* GetBuffer() const{return m_buf.fptr();}

			/*!
				@brief Get the buffer size in bytes for request.

				@return Return the buffer size in bytes.\n
					If there is no buffer bound, return GNIL.
			*/
			GAIA::NUM GetBufferSize() const{return m_buf.write_size();}

			/*!
				@brief Check HttpRequest is the buffer's owner or not.

				@return If HttpRequest is the buffer's owner, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsBufferOwner() const{return m_bBufOwner;}

			// Work flow control.
			/*!
				@brief Execute request.

				@return If deliver http request successfully, return GAIA::True, or return GAIA::False.

				@remarks This method is thread safe.
			*/
			GAIA::BL Request();

			/*!
				@brief Cancel the request.

				@return If cancel the request successfully, return GAIA::True, or return GAIA::False.

				@remarks This method is thread safe.
			*/
			GAIA::BL Cancel();

			/*!
				@brief Pause the request.

				@return If pause the request successfully, return GAIA::True, or return GAIA::False.

				@remarks This method is thread safe.
			*/
			GAIA::BL Pause();

			/*!
				@brief Resume the request.

				@return If resume the request successfully, return GAIA::True, or return GAIA::False.

				@remarks This method is thread safe.
			*/
			GAIA::BL Resume();

			/*!
				@brief Wait the request complete.

				@param uMilliSeconds [in] Specify the timeout time in milli-seconds.

				@return If wait request complete successfully, return GAIA::True, or will return GAIA::False.

				@remarks This method is thread safe.
			*/
			GAIA::BL Wait(GAIA::U32 uMilliSeconds);

			// Query result.
			/*!
				@brief Check the request complete or not.

				@return If request completed, return GAIA::True, or will return GAIA::False.

				@remarks
			*/
			GAIA::BL IsRequestComplete() const{return m_bRequestComplete;}

			/*!
				@brief Check the response complete or not.

				@return If response completed, return GAIA::True, or will return GAIA::False.

				@remarks
			*/
			GAIA::BL IsResponseComplete() const{return m_bResponseComplete;}

			/*!
				@brief Get http result code.

				@return Return http result code.

				@remarks
			*/
			GAIA::NETWORK::HTTP_CODE GetResponseCode() const{return m_ResponseCode;}

			/*!
				@brief Get http response size in bytes.

				@return Return http response size in bytes.

				@remarks
			*/
			GAIA::NUM GetResponseSize() const{return m_sResponseSize;}

			// Mode control.
			/*!
				@brief Set async mode.

				@param bAsync [in] Specify the async mode.

				@remarks
			*/
			GAIA::GVOID SetAsync(GAIA::BL bAsync){m_bAsync = bAsync;}

			/*!
				@brief Get async mode.

				@return If in async, return GAIA::True, or will return GAIA::False.

				@remarks
			*/
			GAIA::BL GetAsync() const{return m_bAsync;}

			/*!
				@brief Set http request timeout time in milliseconds.

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetTimeout(const GAIA::U64& uMilliSeconds){m_uTimeout = uMilliSeconds;}

			/*!
				@brief Get http request timeout time in milliseconds.

				@param

				@return

				@remarks
			*/
			const GAIA::U64& GetTimeout() const{return m_uTimeout;}

			// Cookic control.
			/*!
				@brief Enable or disable write cookic to RAM.

				@param bEnable [in] Specify enable or disable write cookic to RAM.

				@remarks
			*/
			GAIA::GVOID EnableWriteCookicRAM(GAIA::BL bEnable){m_bEnableWriteCookicRAM = bEnable;}

			/*!
				@brief Check enable write cookic to RAM.

				@return If enable write cookic to RAM, return GAIA::True, or will return GAIA::False.

				@remarks
			*/
			GAIA::BL IsEnableWriteCookicRAM() const{return m_bEnableWriteCookicRAM;}

			/*!
				@brief Enable or disablewrite cookic to file.

				@param bEnable [in] Specify enable or disable write cookic to file.

				@remarks
			*/
			GAIA::GVOID EnableWriteCookicFile(GAIA::BL bEnable){m_bEnableWriteCookicFile = bEnable;}

			/*!
				@brief Check enable write cookic to file.

				@return If enable write cookic to file, return GAIA::True, or will return GAIA::False.

				@remarks
			*/
			GAIA::BL IsEnableWriteCookicFile() const{return m_bEnableWriteCookicFile;}

			/*!
				@brief Set effect time in milliseconds for cookic which will be writen.

				@param uMilliSeconds [in] Specify the effect time in milliseconds.

				@return

				@remarks
			*/
			GAIA::GVOID SetWriteCookicTime(const GAIA::U64& uMilliSeconds){m_uWriteCookicTime = uMilliSeconds;}

			/*!
				@brief Get effect time in milliseconds for cookic which will be writen.

				@param

				@return

				@remarks
			*/
			const GAIA::U64& GetWriteCookicTime() const{return m_uWriteCookicTime;}

			/*!
				@brief Enable or disable read cookic from RAM.

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableReadCookicRAM(GAIA::BL bEnable){m_bEnableReadCookicRAM = bEnable;}

			/*!
				@brief Check enable read cookic from RAM.

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableReadCookicRAM() const{return m_bEnableReadCookicRAM;}

			/*!
				@brief Enable or disable read cookic from file.

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableReadCookicFile(GAIA::BL bEnable){m_bEnableReadCookicFile = bEnable;}

			/*!
				@brief Check enable read cookic from file.

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableReadCookicFile() const{return m_bEnableReadCookicFile;}

			/*!
				@brief Set effect time in milliseconds for cookic which will be read.

				@param

				@return

				@remarks
			*/
			GAIA::GVOID SetReadCookicTime(const GAIA::U64& uMilliSeconds){m_uReadCookicTime = uMilliSeconds;}

			/*!
				@brief Get effect time in milliseconds for cookic which will be read.

				@param

				@return

				@remarks
			*/
			const GAIA::U64& GetReadCookicTime() const{return m_uReadCookicTime;}

		protected:

			// Stream like read write callback.
			/*!
				@brief When a HttpRequest begin to dispatch, this function will be callbacked.

				@remarks
			*/
			virtual GAIA::GVOID OnBegin(){}

			/*!
				@brief When a HttpRequest end to dispatch,

				@param bCanceled [in] If current request is ended by member function HttpRequest::Cancel, it will be filled by GAIA::True, or will be filled by GAIA::False.

				@remarks
			*/
			virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled){}

			/*!
				@brief When the request data send complete, this function will be callbacked.

				@remarks
			*/
			virtual GAIA::GVOID OnRequestComplete(){}

			/*!
				@brief When all response data recv complete, this function will be callbacked.

				@remarks
			*/
			virtual GAIA::GVOID OnResponseComplete(){}

			/*!
				@brief When the request is paused, this function will be callbacked.

				@remarks
			*/
			virtual GAIA::GVOID OnPause(){}

			/*!
				@brief When the request is resumed, this function will be callbacked.

				@remarks
			*/
			virtual GAIA::GVOID OnResume(){}

			/*!
				@brief When requested and receive a response, this function will be callbacked.

				@param lOffset [in]

				@param pData [in]

				@param sDataSize [in]

				@return

				@remarks One request could cause multiply callbacks.
			*/
			virtual GAIA::BL OnWrite(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize){return GAIA::False;}

			/*!
				@brief When a request try to read request data, this function will be callbacked.

				@param lOffset [in]

				@param pData [in]

				@param sMaxDataSize [in]

				@param sPracticeDataSize [in]

				@return

				@remarks One request could cause multiply callbacks.
			*/
			virtual GAIA::BL OnRead(GAIA::N64 lOffset, GAIA::GVOID* pData, GAIA::NUM sMaxDataSize, GAIA::NUM& sPracticeDataSize){return GAIA::False;}

		private:
			GINL GAIA::GVOID init()
			{
				m_method = GAIA::NETWORK::HTTP_METHOD_INVALID;
				m_bBufOwner = GAIA::True;
				m_bRequestComplete = GAIA::False;
				m_bResponseComplete = GAIA::False;
				m_ResponseCode = GAIA::NETWORK::HTTP_CODE_INVALID;
				m_sResponseSize = 0;
				m_bAsync = GAIA::False;
				m_uTimeout = 16 * 1000 * 1000;
				m_bEnableWriteCookicRAM = GAIA::False;
				m_bEnableWriteCookicFile = GAIA::False;
				m_uWriteCookicTime = 0;
				m_bEnableReadCookicRAM = GAIA::False;
				m_bEnableReadCookicFile = GAIA::False;
				m_uReadCookicTime = 0;
				m_bRequesting = GAIA::False;
			}

		private:
			GAIA::NETWORK::Http* m_pHttp;
			GAIA::NETWORK::HTTP_METHOD m_method;
			GAIA::NETWORK::HttpURL m_url;
			GAIA::NETWORK::HttpHead m_head;
			GAIA::CTN::Buffer m_buf;
			GAIA::BL m_bBufOwner;
			GAIA::BL m_bRequestComplete;
			GAIA::BL m_bResponseComplete;
			GAIA::NETWORK::HTTP_CODE m_ResponseCode;
			GAIA::NUM m_sResponseSize;
			GAIA::BL m_bAsync;
			GAIA::U64 m_uTimeout;
			GAIA::BL m_bEnableWriteCookicRAM;
			GAIA::BL m_bEnableWriteCookicFile;
			GAIA::U64 m_uWriteCookicTime;
			GAIA::BL m_bEnableReadCookicRAM;
			GAIA::BL m_bEnableReadCookicFile;
			GAIA::U64 m_uReadCookicTime;
			GAIA::BL m_bRequesting;
		};

		/*!
			@brief
		*/
		class HttpDesc : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_NETWORK_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_WORK_THREAD_COUNT = 4;
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
				sNetworkThreadCount = DEFAULT_NETWORK_THREAD_COUNT;
				sWorkThreadCount = DEFAULT_WORK_THREAD_COUNT;
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
				if(sNetworkThreadCount <= 0)
					return GAIA::False;
				if(sWorkThreadCount <= 0)
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
				@brief Network thread count.
			*/
			GAIA::NUM sNetworkThreadCount;

			/*!
				@brief
			*/
			GAIA::NUM sWorkThreadCount;

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

		class HttpWorkThread;

		/*!
			@brief
		*/
		class Http : public GAIA::Base
		{
			friend class HttpWorkThread;
			friend class HttpAsyncSocket;
			friend class HttpAsyncDispatcher;
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
			GAIA::GVOID EnableWriteCookicRAM(GAIA::BL bEnable){m_bEnableWriteCookicRAM = bEnable;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableWriteCookicRAM() const{return m_bEnableWriteCookicRAM;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableWriteCookicFile(GAIA::BL bEnable){m_bEnableWriteCookicFile = bEnable;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableWriteCookicFile() const{return m_bEnableWriteCookicFile;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableReadCookicRAM(GAIA::BL bEnable){m_bEnableReadCookicRAM = bEnable;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableReadCookicRAM() const{return m_bEnableReadCookicRAM;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::GVOID EnableReadCookicFile(GAIA::BL bEnable){m_bEnableReadCookicFile = bEnable;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL IsEnableReadCookicFile() const{return m_bEnableReadCookicFile;}

			/*!
				@brief

				@param

				@return

				@remarks
			*/
			GAIA::BL CleanupCookic(GAIA::BL bMem = GAIA::True, GAIA::BL bFile = GAIA::True, const GAIA::U64& uBeyondTime = 0);

		private:
			GINL GAIA::GVOID init()
			{
				m_bCreated = GAIA::False;
				m_desc.reset();
				m_bBegin = GAIA::False;
				m_bEnableWriteCookicRAM = GAIA::False;
				m_bEnableWriteCookicFile = GAIA::False;
				m_bEnableReadCookicRAM = GAIA::False;
				m_bEnableReadCookicFile = GAIA::False;
				m_disp = GNIL;
			}

		private:
			GAIA::BL m_bCreated;
			GAIA::NETWORK::HttpDesc m_desc;
			GAIA::BL m_bBegin;
			GAIA::BL m_bEnableWriteCookicRAM;
			GAIA::BL m_bEnableWriteCookicFile;
			GAIA::BL m_bEnableReadCookicRAM;
			GAIA::BL m_bEnableReadCookicFile;
			GAIA::CTN::Vector<GAIA::NETWORK::HttpWorkThread*> m_listWorkThreads;
			HttpAsyncDispatcher* m_disp;
		};
	}
}

#endif
