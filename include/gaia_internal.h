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
	template<typename _DataType1, typename _DataType2, typename _SizeType>
	GAIA::GVOID move_right(_DataType1 dst, _DataType2 src, const _SizeType& size)
	{
		_SizeType sizetemp = size;
		while(sizetemp > 0)
		{
			*dst = *src;
			--dst;
			--src;
			--sizetemp;
		}
	}
	template<typename _DataType1, typename _DataType2, typename _DataType3, typename _DataType4>
	_DataType1 insert(_DataType1 dst, _DataType2 size, _DataType3 src, _DataType4 index)
	{
		if(index != size)
			GAIA_INTERNAL_NAMESPACE::move_right(dst + size, dst + size - 1, size - index);
		dst[index] = src;
		return dst;
	}
	template<typename _DataType> GAIA::GVOID swap(_DataType& t1, _DataType& t2)
	{
		_DataType t = t2;
		t2 = t1;
		t1 = t;
	}
	template<typename _DataType> GAIA::GVOID inverse(_DataType pBegin, _DataType pEnd)
	{
		GAST(!!pBegin);
		GAST(!!pEnd);
		while(pBegin != pEnd)
		{
			GAIA_INTERNAL_NAMESPACE::swap(*pBegin, *pEnd);
			++pBegin;
			if(pBegin == pEnd)
				break;
			--pEnd;
		}
	}
	template<typename _SrcDataType, typename _DstDataType>
	_DstDataType int2str(const _SrcDataType& src, _DstDataType pDst)
	{
		_SrcDataType tsrc = src;
		_DstDataType p = pDst;
		if(src < (_SrcDataType)0)
		{
		#ifdef GAIA_DEBUG_WARNING
		#	if GAIA_COMPILER == GAIA_COMPILER_VC
		#		pragma warning(disable : 4146)
		#	endif
		#endif
			tsrc = -tsrc;
		#ifdef GAIA_DEBUG_WARNING
		#	if GAIA_COMPILER == GAIA_COMPILER_VC
		#		pragma warning(default : 4146)
		#	endif
		#endif
			*p = '-';
			++p;
		}
		while(tsrc > 0)
		{
			*p = (tsrc % 10 + '0');
			tsrc /= 10;
			++p;
		}
		if(p == pDst)
		{
			*p = '0';
			++p;
		}
		else
			GAIA_INTERNAL_NAMESPACE::inverse(*pDst == '-' ? pDst + 1 : pDst, p - 1);
		*p = '\0';
		return p + 1;
	}
	template<typename _DataType1, typename _DataType2>
	_DataType1 gstrbycapacity(_DataType1 dst, _DataType2 storagesize)
	{
		GAIA::CH szUnit[32];
		if(storagesize >= 1024 * 1024 * 1024)
		{
			storagesize = storagesize / 1024 / 1024 * 1000 / 1024;
			GAIA_INTERNAL_NAMESPACE::gstrcpy(szUnit, "(GB)");
		}
		else if(storagesize >= 1024 * 1024)
		{
			storagesize = storagesize / 1024 * 100 / 1024;
			GAIA_INTERNAL_NAMESPACE::gstrcpy(szUnit, "(MB)");
		}
		else if(storagesize >= 1024)
		{
			storagesize = storagesize * 10 / 1024;
			GAIA_INTERNAL_NAMESPACE::gstrcpy(szUnit, "(KB)");
		}
		else
			GAIA_INTERNAL_NAMESPACE::gstrcpy(szUnit, "(Bytes)");
		GAIA_INTERNAL_NAMESPACE::int2str(storagesize, dst);
		GAIA::NUM sLen = GAIA_INTERNAL_NAMESPACE::gstrlen(dst);
		if(szUnit[1] == 'G')
		{
			insert(dst, sLen, '.', sLen - 3);
			++sLen;
		}
		else if(szUnit[1] == 'M')
		{
			insert(dst, sLen, '.', sLen - 2);
			++sLen;
		}
		else if(szUnit[1] == 'K')
		{
			insert(dst, sLen, '.', sLen - 1);
			++sLen;
		}
		GAIA_INTERNAL_NAMESPACE::gstrcpy(dst + sLen, szUnit);
		return dst;
	}
}

#endif
