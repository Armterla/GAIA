#ifndef		__GAIA_LOCALE_H__
#define		__GAIA_LOCALE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_internal.h"
#include "gaia_algo_compare.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <xlocale>
#else
#	include <iconv.h>
#endif

namespace GAIA
{
	namespace LOCALE
	{
		/*!
			@brief multi-bytes encoded chars convert to wide-chars.

			@param pSrc [in] Specify the source multi-bytes encoded chars.

			@param src_size_in_bytes [in] Specify the source multi-bytes encoded chars length(unit by bytes).

			@param pszDst [out] Used for saving the result wide-char list.

			@param dst_size_in_wchar [in] Specify the max size(capacity) of pszDst(unit by wide chars).

			@param charset_type [in] Specify the multi-bytes encoded chars's charset type.

			@return Return the result wide char count(unit by wide-char).

			@see GAIA::CHARSET_TYPE
		*/
		GINL GAIA::NUM m2w(const GAIA::GVOID* pszSrc, GAIA::NUM src_size_in_bytes, GAIA::WCH* pszDst, GAIA::NUM dst_size_in_wchar, GAIA::CHARSET_TYPE charset_type = GAIA::CHARSET_TYPE_UTF8)
		{
			GAST(charset_type > GAIA::CHARSET_TYPE_INVALID && charset_type < GAIA::CHARSET_TYPE_MAXENUMCOUNT);
			if(charset_type <= GAIA::CHARSET_TYPE_INVALID || charset_type >= GAIA::CHARSET_TYPE_MAXENUMCOUNT)
				return 0;
			if(src_size_in_bytes < 0)
				src_size_in_bytes = GAIA_INTERNAL_NAMESPACE::gstrlen((const GAIA::CH*)pszSrc);
			if(charset_type == GAIA::CHARSET_TYPE_ASCII)
			{
				GAIA::NUM ret = GAIA::ALGO::gmin(src_size_in_bytes, dst_size_in_wchar);
				if(ret > 0)
					GAIA::ALGO::copy(pszDst, (const GAIA::CH*)pszSrc, ret);
				return ret;
			}
		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::U32 uCodePage;
			if(charset_type == GAIA::CHARSET_TYPE_SYS)
				uCodePage = CP_OEMCP;
			else
				uCodePage = GAIA::CHARSET_CODEPAGE[charset_type];
			return ::MultiByteToWideChar(uCodePage, CP_ACP, (const GAIA::CH*)pszSrc, src_size_in_bytes, pszDst, dst_size_in_wchar);
		#elif GAIA_OS == GAIA_OS_ANDROID
			return 0;
		#else
			iconv_t cv;
			if(sizeof(GAIA::WCH) == 4)
				cv = iconv_open(GAIA::CHARSET_CODEPAGE_NAMEA[GAIA::CHARSET_TYPE_UTF32LE], GAIA::CHARSET_CODEPAGE_NAMEA[charset_type]);
			else if(sizeof(GAIA::WCH) == 2)
				cv = iconv_open(GAIA::CHARSET_CODEPAGE_NAMEA[GAIA::CHARSET_TYPE_UTF16LE], GAIA::CHARSET_CODEPAGE_NAMEA[charset_type]);
			else
				return 0;
			if(cv == (iconv_t)-1)
				return 0;

			GAIA::CH* pTempSrc = (GAIA::CH*)pszSrc;
			GAIA::CH* pTempDst = (GAIA::CH*)pszDst;
			size_t uSrcSize = src_size_in_bytes;
			size_t uDstSize = dst_size_in_wchar * sizeof(GAIA::WCH);

			if(iconv(cv, &pTempSrc, &uSrcSize, &pTempDst, &uDstSize) == (GAIA::UM)-1)
			{
				iconv_close(cv);
				return 0;
			}

			if(iconv_close(cv) == -1)
				return 0;

			GAIA::NUM sByteCount = (GAIA::NUM)(dst_size_in_wchar * sizeof(GAIA::WCH) - uDstSize);
			GAST(sByteCount % sizeof(GAIA::WCH) == 0);
			GAIA::NUM sCharCount = sByteCount / sizeof(GAIA::WCH);
			return sCharCount;
		#endif
		}

		/*!
			@brief wide-chars convert to encoded multi-bytes chars.

			@param pszSrc [in] Specify the source wide chars.

			@param src_size_in_wchar [in] Specify the source wide chars length(unit by wide chars).

			@param pszDst [out] Used for saving the result encoded multi-bytes chars.

			@param dst_size_in_bytes [in] Specify the max size(capacity) of pszDst(unit by chars).

			@param charset_type [in] Specify the multi-bytes encoded chars's charset type.

			@return Return the result multi-bytes char count(unit by char).

			@see GAIA::CHARSET_TYPE
		*/
		GINL GAIA::NUM w2m(const GAIA::WCH* pszSrc, GAIA::NUM src_size_in_wchar, GAIA::GVOID* pszDst, GAIA::NUM dst_size_in_bytes, GAIA::CHARSET_TYPE charset_type = GAIA::CHARSET_TYPE_UTF8)
		{
			GAST(charset_type > GAIA::CHARSET_TYPE_INVALID && charset_type < GAIA::CHARSET_TYPE_MAXENUMCOUNT);
			if(charset_type <= GAIA::CHARSET_TYPE_INVALID || charset_type >= GAIA::CHARSET_TYPE_MAXENUMCOUNT)
				return 0;
			if(src_size_in_wchar < 0)
				src_size_in_wchar = GAIA_INTERNAL_NAMESPACE::gstrlen(pszSrc);
			if(charset_type == GAIA::CHARSET_TYPE_ASCII)
			{
				GAIA::NUM ret = GAIA::ALGO::gmin(src_size_in_wchar, dst_size_in_bytes);
				if(ret > 0)
					GAIA::ALGO::copy((GAIA::CH*)pszDst, pszSrc, ret);
				return ret;
			}
		#if GAIA_OS == GAIA_OS_WINDOWS
			GAIA::U32 uCodePage;
			if(charset_type == GAIA::CHARSET_TYPE_SYS)
				uCodePage = CP_OEMCP;
			else
				uCodePage = GAIA::CHARSET_CODEPAGE[charset_type];
			return ::WideCharToMultiByte(uCodePage, CP_ACP, pszSrc, src_size_in_wchar, (GAIA::CH*)pszDst, dst_size_in_bytes, GNIL, GNIL);
		#elif GAIA_OS == GAIA_OS_ANDROID
			return 0;
		#else
			iconv_t cv;
			if(sizeof(GAIA::WCH) == 4)
				cv = iconv_open(GAIA::CHARSET_CODEPAGE_NAMEA[charset_type], GAIA::CHARSET_CODEPAGE_NAMEA[GAIA::CHARSET_TYPE_UTF32LE]);
			else if(sizeof(GAIA::WCH) == 2)
				cv = iconv_open(GAIA::CHARSET_CODEPAGE_NAMEA[charset_type], GAIA::CHARSET_CODEPAGE_NAMEA[GAIA::CHARSET_TYPE_UTF16LE]);
			else
				return 0;
			if(cv == (iconv_t)-1)
				return 0;

			GAIA::CH* pTempSrc = (GAIA::CH*)pszSrc;
			GAIA::CH* pTempDst = (GAIA::CH*)pszDst;
			size_t uSrcSize = src_size_in_wchar * sizeof(GAIA::WCH);
			size_t uDstSize = dst_size_in_bytes;

			if(iconv(cv, &pTempSrc, &uSrcSize, &pTempDst, &uDstSize) == (GAIA::UM)-1)
			{
				iconv_close(cv);
				return 0;
			}

			if(iconv_close(cv) == -1)
				return 0;

			GAIA::NUM sByteCount = (GAIA::NUM)(dst_size_in_bytes - uDstSize);
			return sByteCount;
		#endif
		}
	}
}

#endif
