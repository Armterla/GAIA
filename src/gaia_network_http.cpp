#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_http.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
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

		GAIA::GVOID HttpRequest::SetMethod(GAIA::NETWORK::HTTP_METHOD method)
		{
			m_method = method;
		}

		GAIA::NETWORK::HTTP_METHOD HttpRequest::GetMethod() const
		{
			return m_method;
		}

		GAIA::GVOID HttpRequest::SetURL(const GAIA::NETWORK::HttpURL& url)
		{
			m_url = url;
		}

		const GAIA::NETWORK::HttpURL& HttpRequest::GetURL() const
		{
			return m_url;
		}

		GAIA::NETWORK::HttpURL& HttpRequest::GetURL()
		{
			return m_url;
		}

		GAIA::GVOID HttpRequest::SetHead(const GAIA::NETWORK::HttpHead& head)
		{
			m_head = head;
		}

		const GAIA::NETWORK::HttpHead& HttpRequest::GetHead() const
		{
			return m_head;
		}

		GAIA::NETWORK::HttpHead& HttpRequest::GetHead()
		{
			return m_head;
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

		const GAIA::GVOID* HttpRequest::GetBuffer() const
		{
			return m_buf.fptr();
		}

		GAIA::NUM HttpRequest::GetBufferSize() const
		{
			return m_buf.write_size();
		}

		GAIA::BL HttpRequest::IsBufferOwner() const
		{
			return m_bBufOwner;
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

		GAIA::BL HttpRequest::IsRequestComplete() const
		{
			return m_bRequestComplete;
		}

		GAIA::BL HttpRequest::IsResponseComplete() const
		{
			return m_bResponseComplete;
		}

		GAIA::NETWORK::HTTP_CODE HttpRequest::GetResponseCode() const
		{
			return m_ResponseCode;
		}

		GAIA::NUM HttpRequest::GetResponseSize() const
		{
			return m_sResponseSize;
		}

		GAIA::GVOID HttpRequest::SetAsync(GAIA::BL bAsync)
		{
			m_bAsync = bAsync;
		}

		GAIA::BL HttpRequest::GetAsync() const
		{
			return m_bAsync;
		}

		GAIA::GVOID HttpRequest::SetTimeout(const GAIA::U64& uMilliSeconds)
		{
			m_uTimeout = uMilliSeconds;
		}

		const GAIA::U64& HttpRequest::GetTimeout() const
		{
			return m_uTimeout;
		}

		GAIA::GVOID HttpRequest::EnableWriteCookicRAM(GAIA::BL bEnable)
		{
			m_bEnableWriteCookicRAM = bEnable;
		}

		GAIA::BL HttpRequest::IsEnableWriteCookicRAM() const
		{
			return m_bEnableWriteCookicRAM;
		}

		GAIA::GVOID HttpRequest::EnableWriteCookicFile(GAIA::BL bEnable)
		{
			m_bEnableWriteCookicFile = bEnable;
		}

		GAIA::BL HttpRequest::IsEnableWriteCookicFile() const
		{
			return m_bEnableWriteCookicFile;
		}

		GAIA::GVOID HttpRequest::SetWriteCookicTime(const GAIA::U64& uMilliSeconds)
		{
			m_uWriteCookicTime = uMilliSeconds;
		}

		const GAIA::U64& HttpRequest::GetWriteCookicTime() const
		{
			return m_uWriteCookicTime;
		}

		GAIA::GVOID HttpRequest::EnableReadCookicRAM(GAIA::BL bEnable)
		{
			m_bEnableReadCookicRAM = bEnable;
		}

		GAIA::BL HttpRequest::IsEnableReadCookicRAM() const
		{
			return m_bEnableReadCookicRAM;
		}

		GAIA::GVOID HttpRequest::EnableReadCookicFile(GAIA::BL bEnable)
		{
			m_bEnableReadCookicFile = bEnable;
		}

		GAIA::BL HttpRequest::IsEnableReadCookicFile() const
		{
			return m_bEnableReadCookicFile;
		}

		GAIA::GVOID HttpRequest::SetReadCookicTime(const GAIA::U64& uMilliSeconds)
		{
			m_uReadCookicTime = uMilliSeconds;
		}

		const GAIA::U64& HttpRequest::GetReadCookicTime() const
		{
			return m_uReadCookicTime;
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



			m_bBegin = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL Http::End()
		{
			if(!this->IsBegin())
				return GAIA::False;

			m_bBegin = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL Http::IsBegin() const
		{
			return m_bBegin;
		}

		GAIA::GVOID Http::EnableWriteCookicRAM(GAIA::BL bEnable)
		{
			m_bEnableWriteCookicRAM = bEnable;
		}

		GAIA::BL Http::IsEnableWriteCookicRAM() const
		{
			return m_bEnableWriteCookicRAM;
		}

		GAIA::GVOID Http::EnableWriteCookicFile(GAIA::BL bEnable)
		{
			m_bEnableWriteCookicFile = bEnable;
		}

		GAIA::BL Http::IsEnableWriteCookicFile() const
		{
			return m_bEnableWriteCookicFile;
		}

		GAIA::GVOID Http::EnableReadCookicRAM(GAIA::BL bEnable)
		{
			m_bEnableReadCookicRAM = bEnable;
		}

		GAIA::BL Http::IsEnableReadCookicRAM() const
		{
			return m_bEnableReadCookicRAM;
		}

		GAIA::GVOID Http::EnableReadCookicFile(GAIA::BL bEnable)
		{
			m_bEnableReadCookicFile = bEnable;
		}

		GAIA::BL Http::IsEnableReadCookicFile() const
		{
			return m_bEnableReadCookicFile;
		}

		GAIA::BL Http::CleanupCookic(GAIA::BL bMem, GAIA::BL bFile, const GAIA::U64& uBeyondTime)
		{
			return GAIA::True;
		}
	}
}