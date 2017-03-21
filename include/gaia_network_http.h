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

				@return If set method successfully, return GAIA::True, or return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL SetMethod(GAIA::NETWORK::HTTP_METHOD method){if(m_bRequesting) return GAIA::False; m_method = method; return GAIA::True;}

			/*!
				@brief Get http request method.

				@return Return http request method.
			*/
			GAIA::NETWORK::HTTP_METHOD GetMethod() const{return m_method;}

			/*!
				@brief Set http request url.

				@param url [in] Specify the url.

				@return If set url successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL SetURL(const GAIA::NETWORK::HttpURL& url){if(m_bRequesting) return GAIA::False; m_url = url; return GAIA::True;}

			/*!
				@brief Get http request url.

				@return Return http request url.
			*/
			const GAIA::NETWORK::HttpURL& GetURL() const{return m_url;}

			/*!
				@brief Get http request url.

				@return Return http request url.
			*/
			GAIA::NETWORK::HttpURL& GetURL(){return m_url;}

			/*!
				@brief Set http request head.

				@param head [in] Specify http head.

				@return If set head successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL SetHead(const GAIA::NETWORK::HttpHead& head){if(m_bRequesting) return GAIA::False; m_head = head; return GAIA::True;}

			/*!
				@brief Get http request head.

				@return Return http request head.
			*/
			const GAIA::NETWORK::HttpHead& GetHead() const{return m_head;}

			/*!
				@brief Get http request head.

				@return Return http request head.
			*/
			GAIA::NETWORK::HttpHead& GetHead(){return m_head;}

			// Buffer control.
			/*!
				@brief Bind a buffer to request.

				@param p [in] Specify the buffer.

				@param sSize [in] Specify the buffer's size in bytes.

				@return If bind buffer successfully, return GAIA::True, or return GAIA::False.

				@remarks The bound buffer will be send to server after send the head, so it could be a http body.\n
					If parameter p is not GNIL and sSize above 0, current function will reference the buffer and the caller must keep the buffer available until http request complete.\n
					If parameter p is GNIL and sSize above 0, current function will allocate buffer internal.\n
					If parameter p is GNIL and sSize equal zero, current function will unreference the buffer or destory the internal buffer.\n
					When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.\n
			*/
			GAIA::BL BindBuffer(const GAIA::GVOID* p, GAIA::NUM sSize);

			/*!
				@brief Get the buffer for request.

				@return If the buffer exist, return a valid pointer, or will return GNIL.
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

				@remarks This method is thread safe.\n
					There are some reasons cause deliver http request failed:\n
					-# Http request's method is not be set.
					-# Http request's url is not be set.
					-# Deliver http request by HttpRequest::Request method twice.
			*/
			GAIA::BL Request();

			/*!
				@brief Cancel the request.

				@return If cancel the request successfully, return GAIA::True, or return GAIA::False.

				@remarks This method is thread safe.\n
					There are some reasons cause cancel http request failed:\n
					-# Http request had not be delivered by HttpRequest::Request method.
					-# Http request is dispatched complete, HttpRequest::OnEnd had be called.
			*/
			GAIA::BL Cancel();

			/*!
				@brief Pause the request.

				@return If pause the request successfully, return GAIA::True, or return GAIA::False.

				@remarks This method is thread safe.\n
					There are some reasons cause pause http request failed:\n
					-# Http request had not be deliverd by HttpRequest::Request method.
					-# Http request had paused.
					-# Http request is dispatched complete, HttpRequest::OnEnd had be called.
			*/
			GAIA::BL Pause();

			/*!
				@brief Resume the request.

				@return If resume the request successfully, return GAIA::True, or return GAIA::False.

				@remarks This method is thread safe.\n
					There are some reasons cause resume http request failed:\n
					-# Http request had not be delivered by HttpRequest::Request method.
					-# Http request had not be paused.
					-# Http request is dispatched complete, HttpRequest::OnEnd had be called.
			*/
			GAIA::BL Resume();

			/*!
				@brief Wait the request complete.

				@param uMilliSeconds [in] Specify the timeout time in milli-seconds.\n
					Caller could pass GINVALID to wait for infinite.

				@return If wait request complete successfully, return GAIA::True, or will return GAIA::False.

				@remarks This method is thread safe.\n
					There are some reasons cause wait http request failed:\n
					-# Http request had not be deliverd by HttpRequest::Request method.
					-# Http request is dispatched complete, HttpRequest::OnEnd had be called.
					-# Wait http request by HttpRequest::Wait twice.
			*/
			GAIA::BL Wait(GAIA::U32 uMilliSeconds);

			// Query result.
			/*!
				@brief Check the request complete or not.

				@return If request completed, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsRequestComplete() const{return m_bRequestComplete;}

			/*!
				@brief Check the response complete or not.

				@return If response completed, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsResponseComplete() const{return m_bResponseComplete;}

			/*!
				@brief Get http result code.

				@return Return http result code.
			*/
			GAIA::NETWORK::HTTP_CODE GetResponseCode() const{return m_ResponseCode;}

			/*!
				@brief Get http response size in bytes.

				@return Return http response size in bytes.
			*/
			GAIA::NUM GetResponseSize() const{return m_sResponseSize;}

			// Mode control.
			/*!
				@brief Set http request timeout time in milliseconds.

				@param uMilliSeconds [in] Specify the timeout time in milliseconds.

				@return If set timeout time successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL SetTimeout(const GAIA::U64& uMilliSeconds){if(m_bRequesting) return GAIA::False; m_uTimeout = uMilliSeconds; return GAIA::True;}

			/*!
				@brief Get http request timeout time in milliseconds.

				@return Return the timeout time in milliseconds.
			*/
			const GAIA::U64& GetTimeout() const{return m_uTimeout;}

			// Cookic control.
			/*!
				@brief Enable or disable write cookic to RAM.

				@param bEnable [in] Specify enable or disable write cookic to RAM.

				@return If enable or disable write cookic to ram successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL EnableWriteCookicRAM(GAIA::BL bEnable){if(m_bRequesting) return GAIA::False; m_bEnableWriteCookicRAM = bEnable; return GAIA::True;}

			/*!
				@brief Check enable write cookic to RAM.

				@return If enable write cookic to RAM, return GAIA::True, or will return GAIA::False.

				@remarks Default value after a HttpRequest construct is GAIA::False.
			*/
			GAIA::BL IsEnableWriteCookicRAM() const{return m_bEnableWriteCookicRAM;}

			/*!
				@brief Enable or disablewrite cookic to file.

				@param bEnable [in] Specify enable or disable write cookic to file.

				@return If enable or disable write cookic to file successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL EnableWriteCookicFile(GAIA::BL bEnable){if(m_bRequesting) return GAIA::False; m_bEnableWriteCookicFile = bEnable; return GAIA::True;}

			/*!
				@brief Check enable write cookic to file.

				@return If enable write cookic to file, return GAIA::True, or will return GAIA::False.

				@remarks Default value after a HttpRequest construct is GAIA::False.
			*/
			GAIA::BL IsEnableWriteCookicFile() const{return m_bEnableWriteCookicFile;}

			/*!
				@brief Set effect time in milliseconds for cookic which will be writen.

				@param uMilliSeconds [in] Specify the effect time in milliseconds.

				@return If set effect time for cookic which will be writen successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL SetWriteCookicTime(const GAIA::U64& uMilliSeconds){if(m_bRequesting) return GAIA::False; m_uWriteCookicTime = uMilliSeconds; return GAIA::True;}

			/*!
				@brief Get effect time in milliseconds for cookic which will be writen.

				@return Return the effect time in milliseconds for cookic which will be writen.

				@remarks Default value after a HttpRequest construct is 0.
			*/
			const GAIA::U64& GetWriteCookicTime() const{return m_uWriteCookicTime;}

			/*!
				@brief Enable or disable read cookic from RAM.

				@param bEnable [in] Specify enable or disable read cookic from RAM.

				@return If enable or disable read cookic from RAM successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL EnableReadCookicRAM(GAIA::BL bEnable){if(m_bRequesting) return GAIA::False; m_bEnableReadCookicRAM = bEnable; return GAIA::True;}

			/*!
				@brief Check enable read cookic from RAM.

				@return If enable read cookic from RAM, return GAIA::True, or will return GAIA::False.

				@remarks Default value after a HttpRequest construct is GAIA::False.
			*/
			GAIA::BL IsEnableReadCookicRAM() const{return m_bEnableReadCookicRAM;}

			/*!
				@brief Enable or disable read cookic from file.

				@param bEnable [in] Specify enable or disable read cookic from file.

				@return If enable or disable read cookic from file successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL EnableReadCookicFile(GAIA::BL bEnable){if(m_bRequesting) return GAIA::False; m_bEnableReadCookicFile = bEnable; return GAIA::True;}

			/*!
				@brief Check enable read cookic from file.

				@return If enable read cookic from file, return GAIA::True, or will return GAIA::False.

				@remarks Default value after a HttpRequest construct is GAIA::False.
			*/
			GAIA::BL IsEnableReadCookicFile() const{return m_bEnableReadCookicFile;}

			/*!
				@brief Set effect time in milliseconds for cookic which will be read.

				@param uMilliSeconds [in] Specify the effect time for cookic which will be read.

				@return If set effect time for cookic which will be read successfully, return GAIA::True, or will return GAIA::False.

				@remarks When the HttpRequest begin to dispatch, the user can't call this method again, so function call will failed.
			*/
			GAIA::BL SetReadCookicTime(const GAIA::U64& uMilliSeconds){if(m_bRequesting) return GAIA::False; m_uReadCookicTime = uMilliSeconds; return GAIA::True;}

			/*!
				@brief Get effect time in milliseconds for cookic which will be read.

				@return Return effect time in milliseconds for cookic which will be read.

				@remarks Default value after a HttpRequest construct is 0.
			*/
			const GAIA::U64& GetReadCookicTime() const{return m_uReadCookicTime;}

		protected:

			// Stream like read write callback.
			/*!
				@brief When a HttpRequest begin to dispatch, this function will be callbacked.

				@remarks This function would be callbacked in multi thread.\n
			*/
			virtual GAIA::GVOID OnBegin(){}

			/*!
				@brief When a HttpRequest end to dispatch,

				@param bCanceled [in] If current request is ended by member function HttpRequest::Cancel, it will be filled by GAIA::True, or will be filled by GAIA::False.

				@remarks This function would be callbacked in multi thread.\n
			*/
			virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled){}

			/*!
				@brief When the request data send complete, this function will be callbacked.

				@remarks This function would be callbacked in multi thread.\n
			*/
			virtual GAIA::GVOID OnRequestComplete(){}

			/*!
				@brief When all response data recv complete, this function will be callbacked.

				@remarks This function would be callbacked in multi thread.\n
			*/
			virtual GAIA::GVOID OnResponseComplete(){}

			/*!
				@brief When the request is paused, this function will be callbacked.

				@remarks This function would be callbacked in multi thread.\n
			*/
			virtual GAIA::GVOID OnPause(){}

			/*!
				@brief When the request is resumed, this function will be callbacked.

				@remarks This function would be callbacked in multi thread.\n
			*/
			virtual GAIA::GVOID OnResume(){}

			/*!
				@brief When send a request buffer piece, this function will be callbacked.

				@param lOffset [in] Specify the offset position of total sent buffer.

				@param pData [in] Specify the data buffer of current sent buffer piece.

				@param sDataSize [in] Specify the data buffer's size of current sent buffer piece.

				@remarks This function would be callbacked in multi thread.\n
					One request could cause multiply callbacks of this method.\n
					This method will be callbacked after send a piece of data by network.\n
			*/
			virtual GAIA::GVOID OnWrite(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize){}

			/*!
				@brief When receive a response buffer piece, this function will be callbacked.

				@param lOffset [in] Specify the offset position of total received buffer.

				@param pData [in] Specify the data buffer of current received buffer piece.

				@param sMaxDataSize [in] Specify the data buffer's max size of current received buffer piece.

				@param sPracticeDataSize [in] Specify the data buffer's size of current received buffer piece.

				@remarks This function would be callbacked in multi thread.\n
					One request could cause multiply callbacks of this method.\n
					This method will be callbacked after receive a piece of data by network.\n
			*/
			virtual GAIA::GVOID OnRead(GAIA::N64 lOffset, GAIA::GVOID* pData, GAIA::NUM sMaxDataSize, GAIA::NUM& sPracticeDataSize){}

		private:
			GINL GAIA::GVOID init()
			{
				m_method = GAIA::NETWORK::HTTP_METHOD_INVALID;
				m_bBufOwner = GAIA::True;
				m_bRequestComplete = GAIA::False;
				m_bResponseComplete = GAIA::False;
				m_ResponseCode = GAIA::NETWORK::HTTP_CODE_INVALID;
				m_sResponseSize = 0;
				m_uTimeout = 16 * 1000 * 1000;
				m_bEnableWriteCookicRAM = GAIA::False;
				m_bEnableWriteCookicFile = GAIA::False;
				m_uWriteCookicTime = 0;
				m_bEnableReadCookicRAM = GAIA::False;
				m_bEnableReadCookicFile = GAIA::False;
				m_uReadCookicTime = 0;
				m_bRequesting = GAIA::False;
				m_pSock = GNIL;
				m_sIndex = GINVALID;
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
			GAIA::U64 m_uTimeout;
			GAIA::BL m_bEnableWriteCookicRAM;
			GAIA::BL m_bEnableWriteCookicFile;
			GAIA::U64 m_uWriteCookicTime;
			GAIA::BL m_bEnableReadCookicRAM;
			GAIA::BL m_bEnableReadCookicFile;
			GAIA::U64 m_uReadCookicTime;
			GAIA::BL m_bRequesting;
			GAIA::NETWORK::AsyncSocket* m_pSock;
			GAIA::NUM m_sIndex; // Is the index in GAIA::NETWORK::Http's variable member m_requests.
		};

		/*!
			@brief Http description for work.
		*/
		class HttpDesc : public GAIA::Base
		{
		public:
			static const GAIA::NUM DEFAULT_NETWORK_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_WORK_THREAD_COUNT = 4;
			static const GAIA::NUM DEFAULT_MAX_CONN_COUNT = 10000;
			static const GAIA::U64 DEFAULT_MAX_CONN_TIME = (GAIA::U64)1000 * 1000 * 3600 * 24;
			static const GAIA::NUM DEFAULT_MAX_COOKIC_COUNT = 10000;
			static const GAIA::NUM DEFAULT_MAX_COOKIC_SIZE = 10000;

		public:

			/*!
				@brief Reset all of the HttpDesc's member variables to default value.
			*/
			GAIA::GVOID reset()
			{
				sNetworkThreadCount = DEFAULT_NETWORK_THREAD_COUNT;
				sWorkThreadCount = DEFAULT_WORK_THREAD_COUNT;
				pszRootPath = GNILSTR;
				sMaxConnCount = DEFAULT_MAX_CONN_COUNT;
				uMaxConnTime = DEFAULT_MAX_CONN_TIME;
				GAIA::ALGO::gstrcpy(szHttpVer, GAIA::NETWORK::HTTP_VERSION_STRING);
				sHttpVerLen = GAIA::ALGO::gstrlen(szHttpVer);
				bEnableSocketTCPNoDelay = GAIA::True;
				bEnableSocketNoBlock = GAIA::False;
				bEnableSocketReuseAddr = GAIA::True;
				nSocketSendBufferSize = GINVALID;
				nSocketRecvBufferSize = GINVALID;
				sMaxCookicCount = DEFAULT_MAX_COOKIC_COUNT;
				sMaxCookicSize = DEFAULT_MAX_COOKIC_SIZE;
			}

			/*!
				@brief Check HttpDesc's member variables is valid or not.

				@return If all of the member variables is valid, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL check() const
			{
				if(sNetworkThreadCount <= 0)
					return GAIA::False;
				if(sWorkThreadCount <= 0)
					return GAIA::False;
				if(pszRootPath == GNIL)
					return GAIA::False;
				if(sMaxConnCount == 0)
					return GAIA::False;
				if(uMaxConnTime == 0)
					return GAIA::False;
				if(szHttpVer[0] == '\0')
					return GAIA::False;
				if(sHttpVerLen <= 0)
					return GAIA::False;
				if(GAIA::ALGO::gstrlen(szHttpVer) != sHttpVerLen)
					return GAIA::False;
				if(nSocketSendBufferSize != GINVALID && nSocketSendBufferSize <= 0)
					return GAIA::False;
				if(nSocketRecvBufferSize != GINVALID && nSocketRecvBufferSize <= 0)
					return GAIA::False;
				if(sMaxCookicCount < 0)
					return GAIA::False;
				if(sMaxCookicSize < 0)
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
				@brief Specify the socket's send buffer size in bytes, default is GINVALID means use system default setting.
			*/
			GAIA::N32 nSocketSendBufferSize;

			/*!
				@brief Specify the socket's receive buffer size in bytes, default is GINVALID means use system default setting.
			*/
			GAIA::N32 nSocketRecvBufferSize;

			/*!
				@brief Specify the max cookic count, default value is DEFAULT_MAX_COOKIC_COUNT.

				@see DEFAULT_MAX_COOKIC_COUNT.
			*/
			GAIA::NUM sMaxCookicCount;

			/*!
				@brief Specify the max cookic size in bytes, default value is DEFAULT_MAX_COOKIC_SIZE.

				@see DEFAULT_MAX_COOKIC_SIZE.
			*/
			GAIA::NUM sMaxCookicSize;
		};

		class HttpWorkThread;

		/*!
			@brief Http request manager class.
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
				@brief Create http.

				@param desc [in] Specify http work description.

				@return If Http create successfully, return GAIA::True, or will return GAIA::False.

				@remarks After call this method, the user should call Http::Begin method to start work thread and network thread.

				@see GAIA::NETWORK::HttpDesc.
			*/
			GAIA::BL Create(const GAIA::NETWORK::HttpDesc& desc);

			/*!
				@brief Destroy http.

				@return If destroy http successfully, return GAIA::True, or return GAIA::False.

				@remarks Call this function will recycle all cookic in RAM.\n
			*/
			GAIA::BL Destroy();

			/*!
				@brief Check http is created or not.

				@return Is http is created, return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL IsCreated() const{return m_bCreated;}

			/*!
				@brief Get http work description which is specified when call Http::Create.

				@return Return the http work description.

				@see GAIA::NETWORK::HttpDesc.
			*/
			const GAIA::NETWORK::HttpDesc& GetDesc() const{return m_desc;}

			/*!
				@brief Begin http work.

				@return If begin http successfully, return GAIA::True, or return GAIA::False.

				@remarks Call this thread will startup the work thread and network thread.\n
			*/
			GAIA::BL Begin();

			/*!
				@brief End http work.

				@return If end http successfully, return GAIA::True, or return GAIA::False.

				@remarks Call this thread will shutdown the work thread and network thread,
					but the RAM cookic wouldn't be recycle in this function call.
			*/
			GAIA::BL End();

			/*!
				@brief Check http is working or not.

				@return If http is working, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsBegin() const{return m_bBegin;}

			/*!
				@brief Execute http.

				@return If there exist a task to execute, pop and executed, return GAIA::True, or return GAIA::False.

				@remarks Call this method only pop a unit task in task queue and execute it, so if user need execute all waiting task,
					call this method many times until GAIA::False be returnned.\n
					This method is thread safe.\n
			*/
			GAIA::BL Execute();

			/*!
				@brief Enable or disable write cookic to RAM.

				@param bEnable [in] Specify enable or disable write cookic to RAM.
			*/
			GAIA::GVOID EnableWriteCookicRAM(GAIA::BL bEnable){m_bEnableWriteCookicRAM = bEnable;}

			/*!
				@brief Check enable write cookic to RAM.

				@return If enable write cookic to RAM, return GAIA::True, or will return GAIA::False.

				@remarks Default value after a Http construct is GAIA::False.
			*/
			GAIA::BL IsEnableWriteCookicRAM() const{return m_bEnableWriteCookicRAM;}

			/*!
				@brief Enable or disable write cookic to file.

				@param bEnable [in] Specify enable or disable write cookic to file.
			*/
			GAIA::GVOID EnableWriteCookicFile(GAIA::BL bEnable){m_bEnableWriteCookicFile = bEnable;}

			/*!
				@brief Check enable write cookic to file.

				@return If enable write cookic to file, return GAIA::True, or will return GAIA::False.

				@remarks Default value after a Http construct is GAIA::False.
			*/
			GAIA::BL IsEnableWriteCookicFile() const{return m_bEnableWriteCookicFile;}

			/*!
				@brief Enable or disable read cookic from RAM.

				@param bEnable [in] Specify enable read cookic from RAM.
			*/
			GAIA::GVOID EnableReadCookicRAM(GAIA::BL bEnable){m_bEnableReadCookicRAM = bEnable;}

			/*!
				@brief Check enable read cookic from RAM.

				@return If enable read cookic from RAM, return GAIA::True, or will return GAIA::False.

				@remarks Default value after a Http construct is GAIA::False.
			*/
			GAIA::BL IsEnableReadCookicRAM() const{return m_bEnableReadCookicRAM;}

			/*!
				@brief Enable or disable read cookic from file.

				@param bEnable [in] Specify enable read cookic from file.
			*/
			GAIA::GVOID EnableReadCookicFile(GAIA::BL bEnable){m_bEnableReadCookicFile = bEnable;}

			/*!
				@brief Check enable read cookic from file.

				@return If enable read cookic from file, return GAIA::True, or will return GAIA::False.

				@remarks Default value after a Http construct is GAIA::False.
			*/
			GAIA::BL IsEnableReadCookicFile() const{return m_bEnableReadCookicFile;}

			/*!
				@brief Cleanup cookic.

				@param bRAM [in] Specify clean cookic from RAM or not. Default is GAIA::True.

				@param bFile [in] Specify clean cookic from file or not. Default is GAIA::True.

				@param uBeyondTime [in] Specify the cookic recycle limit time.\n
					If the cookic life time above equal this parameter, the cookic will be recycled.\n
					Default is zero means all of the cookic would be cleanuped.\n

				@return If exist cookic be cleanuped, return GAIA::True, or return GAIA::False.

				@remarks This method is a sync method, and it is thread safe.
			*/
			GAIA::BL CleanupCookic(GAIA::BL bRAM = GAIA::True, GAIA::BL bFile = GAIA::True, const GAIA::U64& uBeyondTime = 0);

		private:
			typedef GAIA::CTN::Vector<GAIA::NETWORK::HttpRequest*> __RequestVectorType;
			typedef GAIA::CTN::Vector<GAIA::NETWORK::HttpAsyncSocket*> __SockVectorType;

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
			GAIA::NETWORK::HttpAsyncDispatcher* m_disp;
			GAIA::SYNC::Lock m_lrRequests;
			__RequestVectorType m_requests;
			GAIA::SYNC::Lock m_lrSocks;
			__SockVectorType m_socks;
		};
	}
}

#endif
