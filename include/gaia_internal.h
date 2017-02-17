#ifndef		__GAIA_INTERNAL_H__
#define		__GAIA_INTERNAL_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA_INTERNAL_NAMESPACE
{
	template<typename _SizeType> GAIA::GVOID* gmemcpy(GAIA::GVOID* dst, const GAIA::GVOID* src, const _SizeType& size)
	{
		_SizeType sizet = size;
		GAIA::GVOID* pRet = dst;
		while(sizet > sizeof(GAIA::UM))
		{
			*(GAIA::UM*)dst = *(GAIA::UM*)src;
			dst = ((GAIA::UM*)dst) + 1;
			src = ((GAIA::UM*)src) + 1;
			sizet -= sizeof(GAIA::UM);
		}
		while(sizet > 0)
		{
			*(GAIA::U8*)dst = *(GAIA::U8*)src;
			dst = ((GAIA::U8*)dst) + 1;
			src = ((GAIA::U8*)src) + 1;
			sizet -= sizeof(GAIA::U8);
		}
		return pRet;
	}
	template<typename _DataType> GAIA::NUM gstrlen(const _DataType* p){GAIA::NUM ret = 0; while(p[ret] != 0) ret++; return ret;}
	template<typename _DataType> _DataType* gstrcpy(_DataType* dst, const _DataType* src)
	{
		_DataType* ret = dst;
		while(*src != 0)
		{
			*dst = *src;
			++dst;
			++src;
		}
		*dst = 0;
		return ret;
	}
	template<typename _DataType> GAIA::N32 gstrcmp(const _DataType* p1, const _DataType* p2)
	{
		for(;;)
		{
			if(*p1 < *p2)
				return -1;
			else if(*p1 > *p2)
				return +1;
			else
			{
				if(*p1 == 0)
					return 0;
				++p1;
				++p2;
			}
		}
	#if GAIA_COMPILER != GAIA_COMPILER_VC // For CL C4702 Warning.
		return 0;
	#endif
	}
	template<typename _DstDataType, typename _SizeType> _DstDataType hex2str(const GAIA::U8* pSrc, const _SizeType& size, _DstDataType pDst)
	{
		_SizeType sizet = size;
		_DstDataType p = pDst;
		while(sizet > 0)
		{
			// Calculate high 4 bit.
			GAIA::U8 u0 = ((*pSrc) >> 4) & 0x0F;
			if(u0 < 10)
				*p = '0' + u0;
			else
				*p = 'A' + u0 - 10;
			++p;

			// Calculate low 4 bit.
			GAIA::U8 u1 = (*pSrc) & 0x0F;
			if(u1 < 10)
				*p = '0' + u1;
			else
				*p = 'A' + u1 - 10;
			++p;

			// Combine.
			++pSrc;
			--sizet;
		}
		*p = '\0';
		return pDst;
	}
	template<typename _SrcDataType, typename _SizeType> GAIA::U8* str2hex(_SrcDataType pSrc, const _SizeType& size, GAIA::U8* pDst)
	{
		_SizeType sizet = size;
		GAIA::U8* p = pDst;
		while(sizet > 0)
		{
			// Calculate high 4 bit.
			GAIA::U8 u0;
			if(*pSrc >= '0' && *pSrc <= '9')
				u0 = *pSrc - '0';
			else if(*pSrc >= 'a' && *pSrc <= 'f')
				u0 = *pSrc - 'a' + 10;
			else if(*pSrc >= 'A' && *pSrc <= 'F')
				u0 = *pSrc - 'A' + 10;
			else
				u0 = 0;
			++pSrc;

			// Calculate low 4 bit.
			GAIA::U8 u1;
			if(*pSrc >= '0' && *pSrc <= '9')
				u1 = *pSrc - '0';
			else if(*pSrc >= 'a' && *pSrc <= 'f')
				u1 = *pSrc - 'a' + 10;
			else if(*pSrc >= 'A' && *pSrc <= 'F')
				u1 = *pSrc - 'A' + 10;
			else
				u1 = 0;
			++pSrc;

			// Combine.
			*p = GSCAST(GAIA::U8)(u0 * 16 + u1);
			++p;
			--sizet;
		}
		return pDst;
	}
}

#endif
