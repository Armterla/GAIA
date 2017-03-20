#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_http.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		class HttpAsyncSocket : public GAIA::NETWORK::AsyncSocket
		{
		public:
			HttpAsyncSocket(GAIA::NETWORK::Http& http, GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::ASYNC_SOCKET_TYPE socktype = GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED)
				: GAIA::NETWORK::AsyncSocket(disp, socktype)
			{
				this->init();
				m_pHttp = &http;
			}

			~HttpAsyncSocket(){}

		protected:
			virtual GAIA::GVOID OnCreated(GAIA::BL bResult){}
			virtual GAIA::GVOID OnClosed(GAIA::BL bResult){}
			virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}
			virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}
			virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult){}
			virtual GAIA::GVOID OnListened(GAIA::BL bResult){}
			virtual GAIA::GVOID OnAccepted(GAIA::BL bResult, const GAIA::NETWORK::Addr& addrListen){}
			virtual GAIA::GVOID OnSent(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nPracticeSize, GAIA::N32 nSize){}
			virtual GAIA::GVOID OnRecved(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nSize){}
			virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag){}

		private:
			GINL GAIA::GVOID init()
			{
				m_pHttp = GNIL;
				m_pRequest = GNIL;
			}

		private:
			GAIA::NETWORK::Http* m_pHttp;
			GAIA::NETWORK::HttpRequest* m_pRequest;
		};

		class HttpAsyncDispatcher : public GAIA::NETWORK::AsyncDispatcher
		{
		public:
			HttpAsyncDispatcher(GAIA::NETWORK::Http& http)
			{
				m_pHttp = &http;
			}

			~HttpAsyncDispatcher(){}

		protected:
			virtual GAIA::NETWORK::AsyncSocket* OnCreateListenSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				HttpAsyncSocket* pListenSocket = gnew HttpAsyncSocket(*m_pHttp, *this, ASYNC_SOCKET_TYPE_LISTEN);
				return pListenSocket;
			}

			virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptingSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				HttpAsyncSocket* pAcceptingSocket = gnew HttpAsyncSocket(*m_pHttp, *this, ASYNC_SOCKET_TYPE_ACCEPTING);
				return pAcceptingSocket;
			}

		private:
			GINL GAIA::GVOID init()
			{
				m_pHttp = GNIL;
			}

		private:
			GAIA::NETWORK::Http* m_pHttp;
		};

		HttpRequest::HttpRequest(GAIA::NETWORK::Http& http)
		{
			m_pHttp = &http;
			this->init();
		}

		HttpRequest::~HttpRequest()
		{
			if(m_bBufOwner)
				m_buf.destroy();
			else
				m_buf.proxy(GNIL, GNIL);
		}

		Http& HttpRequest::GetHttp()
		{
			return *m_pHttp;
		}

		GAIA::GVOID HttpRequest::Reset()
		{
			if(m_bBufOwner)
				m_buf.destroy();
			else
				m_buf.proxy(GNIL, GNIL);
			this->init();
		}

		GAIA::BL HttpRequest::BindBuffer(const GAIA::GVOID* p, GAIA::NUM sSize)
		{
			if(p == GNIL)
			{
				if(sSize > 0)
				{
					m_bBufOwner = GAIA::True;
					m_buf.resize(sSize);
				}
				else if(sSize == 0)
				{
					if(m_bBufOwner)
						m_buf.destroy();
					else
						m_buf.proxy(GNIL, GNIL);
				}
				else
				{
					GASTFALSE;
					return GAIA::False;
				}
			}
			else
			{
				if(sSize > 0)
				{
					if(m_bBufOwner)
						m_buf.destroy();
					else
						m_buf.proxy(GNIL, GNIL);
					m_bBufOwner = GAIA::False;
					m_buf.proxy((GAIA::U8*)p, (GAIA::U8*)p + sSize - 1);
				}
				else
				{
					GASTFALSE;
					return GAIA::False;
				}
			}
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Request()
		{
			if(this->GetAsync())
			{

			}
			else
			{

			}
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Cancel()
		{
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Pause()
		{
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Resume()
		{
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Wait(GAIA::U32 uMilliSeconds)
		{
			return GAIA::True;
		}

		Http::Http()
		{
			GAST(GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_METHOD_STRING));
			GAST(GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_STRING));
			GAST(GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_LIST));
			GAST(GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_VALUE));
			GAST(GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_DESCRIPTION));
			this->init();
		}

		Http::~Http()
		{
			if(this->IsCreated())
				this->Destroy();
		}

		GAIA::BL Http::Create(const GAIA::NETWORK::HttpDesc& desc)
		{
			if(!desc.check())
				return GAIA::False;

			if(this->IsCreated())
				return GAIA::False;

			// Create async network.

			m_desc = desc;
			m_bCreated = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL Http::Destroy()
		{
			if(!this->IsCreated())
				return GAIA::False;

			if(this->IsBegin())
				this->End();

			// Destroy async network;

			m_bCreated = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL Http::IsCreated() const
		{
			return m_bCreated;
		}

		const GAIA::NETWORK::HttpDesc& Http::GetDesc() const
		{
			return m_desc;
		}

		GAIA::BL Http::Begin()
		{
			if(!this->IsCreated())
				return GAIA::False;

			if(this->IsBegin())
				return GAIA::False;

			// Create work thread.

			// Begin async network.

			m_bBegin = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL Http::End()
		{
			if(!this->IsBegin())
				return GAIA::False;

			// End async network.

			// Close work thread.

			m_bBegin = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL Http::IsBegin() const
		{
			return m_bBegin;
		}

		GAIA::BL Http::CleanupCookic(GAIA::BL bMem, GAIA::BL bFile, const GAIA::U64& uBeyondTime)
		{
			return GAIA::True;
		}
	}
}