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

		}

		HttpRequest::~HttpRequest()
		{

		}

		Http& HttpRequest::GetHttp()
		{

		}

		GAIA::GVOID HttpRequest::SetMethod(GAIA::NETWORK::HTTP_METHOD method)
		{

		}

		GAIA::NETWORK::HTTP_METHOD HttpRequest::GetMethod() const
		{

		}

		GAIA::GVOID HttpRequest::SetURL(const GAIA::CH* pszURL)
		{

		}

		const GAIA::CH* HttpRequest::GetURL() const
		{

		}

		GAIA::GVOID HttpRequest::SetHead(const GAIA::NETWORK::HttpHead& pHead)
		{

		}

		const GAIA::NETWORK::HttpHead& HttpRequest::GetHead() const
		{

		}

		GAIA::NETWORK::HttpHead& HttpRequest::GetHead()
		{

		}

		GAIA::BL HttpRequest::BindBuffer(const GAIA::GVOID* p, GAIA::NUM sSize)
		{

		}

		const GAIA::GVOID* HttpRequest::GetBuffer() const
		{

		}

		GAIA::NUM HttpRequest::GetBufferSize() const
		{

		}

		GAIA::BL HttpRequest::Request()
		{

		}

		GAIA::BL HttpRequest::Cancel()
		{

		}

		GAIA::BL HttpRequest::Pause()
		{

		}

		GAIA::BL HttpRequest::Resume()
		{

		}

		GAIA::BL HttpRequest::Wait(GAIA::U32 uMilliSeconds)
		{

		}

		GAIA::BL HttpRequest::IsComplete() const
		{

		}

		GAIA::N64 HttpRequest::GetResultCode() const
		{

		}

		GAIA::NUM HttpRequest::GetResultSize() const
		{

		}

		GAIA::GVOID HttpRequest::SetAsync(GAIA::BL bAsync)
		{

		}

		GAIA::BL HttpRequest::GetAsync() const
		{

		}

		GAIA::GVOID HttpRequest::SetTimeout(const GAIA::U64& uTimeout)
		{

		}

		const GAIA::U64& HttpRequest::GetTimeout() const
		{

		}

		GAIA::GVOID HttpRequest::EnableWriteCookicRAM(GAIA::BL bEnable)
		{

		}

		GAIA::BL HttpRequest::IsEnableWriteCookicRAM() const
		{

		}

		GAIA::GVOID HttpRequest::EnableWriteCookicFile(GAIA::BL bEnable)
		{

		}

		GAIA::BL HttpRequest::IsEnableWriteCookicFile() const
		{

		}

		GAIA::GVOID HttpRequest::SetWriteCookicTime(const GAIA::U64& uTime)
		{

		}

		const GAIA::U64& HttpRequest::GetWriteCookicTime() const
		{

		}

		GAIA::GVOID HttpRequest::EnableReadCookicRAM(GAIA::BL bEnable)
		{

		}

		GAIA::BL HttpRequest::IsEnableReadCookicRAM() const
		{

		}

		GAIA::GVOID HttpRequest::EnableReadCookicFile(GAIA::BL bEnable)
		{

		}

		GAIA::BL HttpRequest::IsEnableReadCookicFile() const
		{

		}

		GAIA::GVOID HttpRequest::SetReadCookicTime(const GAIA::U64& uTime)
		{

		}

		const GAIA::U64& HttpRequest::GetReadCookicTime() const
		{

		}

		Http::Http()
		{
			GAST(HTTP_METHOD_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_METHOD_STRING));
			GAST(HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_STRING));
			GAST(HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_LIST));
			GAST(HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_VALUE));
			GAST(HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_DESCRIPTION));
		}

		Http::~Http()
		{

		}

		GAIA::BL Http::Create(const GAIA::NETWORK::HttpDesc& desc)
		{

		}

		GAIA::BL Http::Destroy()
		{

		}

		GAIA::BL Http::IsCreated() const
		{

		}

		const GAIA::NETWORK::HttpDesc& Http::GetDesc() const
		{

		}

		GAIA::BL Http::Begin()
		{

		}

		GAIA::BL Http::End()
		{

		}

		GAIA::BL Http::IsBegin() const
		{

		}

		GAIA::GVOID Http::EnableWriteCookicRAM(GAIA::BL bEnable)
		{

		}

		GAIA::BL Http::IsEnableWriteCookicRAM() const
		{

		}

		GAIA::GVOID Http::EnableWriteCookicFile(GAIA::BL bEnable)
		{

		}

		GAIA::BL Http::IsEnableWriteCookicFile() const
		{

		}

		GAIA::GVOID Http::EnableReadCookicRAM(GAIA::BL bEnable)
		{

		}

		GAIA::BL Http::IsEnableReadCookicRAM() const
		{

		}

		GAIA::GVOID Http::EnableReadCookicFile(GAIA::BL bEnable)
		{

		}

		GAIA::BL Http::IsEnableReadCookicFile() const
		{

		}

		GAIA::BL Http::CleanupCookic(GAIA::BL bMem, GAIA::BL bFile, const GAIA::U64& uBeyondTime)
		{

		}
	}
}