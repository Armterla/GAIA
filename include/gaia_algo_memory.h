#ifndef		__GAIA_ALGO_MEMORY_H__
#define		__GAIA_ALGO_MEMORY_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType> _DataType swapendian(_DataType v)
		{
			_DataType t;
			GAIA::U8* p = (GAIA::U8*)&v;
			GAIA::U8* q = (GAIA::U8*)&t;
			for(GAIA::NUM x = 0; x < sizeof(v); ++x)
				q[x] = p[sizeof(v) - x - 1];
			return t;
		}
		template<typename _SizeType> GAIA::GVOID* gmemcpy(
				GAIA::GVOID* dst, const GAIA::GVOID* src, const _SizeType& size)
		{
			GAST(!!dst);
			GAST(!!src);
			GAST(size > 0);
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
		template<typename _SizeType> GAIA::GVOID* gmemcpy(
				GAIA::GVOID* dst, const GAIA::GVOID* src,
				const _SizeType& dst_stride, const _SizeType& src_stride,
				const _SizeType& size, const _SizeType& count)
		{
			GAST(!!dst);
			GAST(!!src);
			GAST(dst_stride > 0);
			GAST(src_stride > 0);
			GAST(dst_stride <= src_stride);
			GAST(size > 0);
			GAST(count > 0);
			GAIA::U8* pDst = (GAIA::U8*)dst;
			const GAIA::U8* pSrc = (const GAIA::U8*)src;
			while(count > 0)
			{
				GAIA::ALGO::gmemcpy(pDst, pSrc, size);
				pDst += dst_stride;
				pSrc += src_stride;
				--count;
			}
			return dst;
		}
		template<typename _FillType, typename _SizeType>
		GAIA::GVOID* gmemset(GAIA::GVOID* dst, const _FillType& ch, const _SizeType& size)
		{
			GAST(!!dst);
			GAST(size > 0);
			GAIA::UM clean = 0;
			for(GAIA::U32 c = 0; c < sizeof(GAIA::UM) / sizeof(GAIA::N8); c++)
				clean |= (((GAIA::UM)ch) << (c * 8));
			_SizeType sizet = size;
			GAIA::GVOID* pRet = dst;
			while(sizet > sizeof(GAIA::UM))
			{
				*(GAIA::UM*)dst = (GAIA::UM)clean;
				dst = ((GAIA::UM*)dst) + 1;
				sizet = sizet - (_SizeType)sizeof(GAIA::UM);
			}
			while(sizet > 0)
			{
				*(GAIA::U8*)dst = (GAIA::U8)ch;
				dst = ((GAIA::U8*)dst) + 1;
				sizet = sizet - (_SizeType)sizeof(GAIA::U8);
			}
			return pRet;
		}
		template<typename _FillType, typename _SizeType>
		GAIA::GVOID* gmemset(GAIA::GVOID* dst, const _FillType& ch,
							 const _SizeType& stride, const _SizeType& size,
							 const _SizeType& count)
		{
			GAST(!!dst);
			GAST(stride > 0);
			GAST(size > 0);
			GAST(count > 0);
			GAIA::U8* p = (GAIA::U8*)dst;
			while(count > 0)
			{
				GAIA::ALGO::gmemset(p, ch, size);
				p += stride;
				--count;
			}
			return dst;
		}
		template<typename _CheckType, typename _SizeType>
		GAIA::N32 gmemchr(const GAIA::GVOID* p, const _CheckType& ch, const _SizeType& size)
		{
			GAST(!!p);
			GAST(size > 0);
			GAIA::UM clean = 0;
			for(GAIA::U32 c = 0; c < sizeof(GAIA::UM) / sizeof(GAIA::N8); c++)
				clean |= (((GAIA::UM)ch) << (c * 8));
			_SizeType sizet = size;
			while(sizet > sizeof(GAIA::UM))
			{
				if(*GSCAST(const GAIA::UM*)(p) < GSCAST(GAIA::UM)(clean))
					return -1;
				else if(*GSCAST(const GAIA::UM*)(p) > GSCAST(GAIA::UM)(clean))
					return +1;
				p = (GSCAST(const GAIA::UM*)(p)) + 1;
				sizet = sizet - GSCAST(_SizeType)(sizeof(GAIA::UM));
			}
			while(sizet > 0)
			{
				if(*GSCAST(const GAIA::U8*)(p) < GSCAST(GAIA::U8)(ch))
					return -1;
				else if(*GSCAST(const GAIA::U8*)(p) > GSCAST(GAIA::U8)(ch))
					return +1;
				p = (GSCAST(const GAIA::U8*)(p)) + 1;
				sizet = sizet - GSCAST(_SizeType)(sizeof(GAIA::U8));
			}
			return 0;
		}
		template<typename _CheckType, typename _SizeType>
		GAIA::N32 gmemchr(const GAIA::GVOID* p, const _CheckType& ch,
							const _SizeType& stride, const _SizeType& size, const _SizeType& count)
		{
			GAST(!!p);
			GAST(stride > 0);
			GAST(size > 0);
			GAST(count > 0);
			const GAIA::U8* pTemp = GSCAST(const GAIA::U8*)(p);
			while(count > 0)
			{
				GAIA::N32 res = GAIA::ALGO::gmemchr(pTemp, ch, size);
				if(res != 0)
					return res;
				pTemp += stride;
				--count;
			}
			return 0;
		}
		template<typename _SizeType> GAIA::N32
		gmemcmp(const GAIA::GVOID* p1, const GAIA::GVOID* p2, const _SizeType& size)
		{
			GAST(!!p1);
			GAST(!!p2);
			GAST(size > 0);
			_SizeType sizet = size;
			while(sizet > sizeof(GAIA::UM))
			{
				if(*(GSCAST(const GAIA::UM*)(p1)) < *(GSCAST(const GAIA::UM*)(p2)))
					return +1;
				else if(*(GSCAST(const GAIA::UM*)(p1)) > *(GSCAST(const GAIA::UM*)(p2)))
					return -1;
				p1 = (GSCAST(const GAIA::UM*)(p1)) + 1;
				p2 = (GSCAST(const GAIA::UM*)(p2)) + 1;
				sizet -= sizeof(GAIA::UM);
			}
			while(sizet > sizeof(GAIA::U8))
			{
				if(*(GSCAST(const GAIA::U8*)(p1)) < *(GSCAST(const GAIA::U8*)(p2)))
					return +1;
				else if(*(GSCAST(const GAIA::U8*)(p1)) > *(GSCAST(const GAIA::U8*)(p2)))
					return -1;
				p1 = (GSCAST(const GAIA::U8*)(p1)) + 1;
				p2 = (GSCAST(const GAIA::U8*)(p2)) + 1;
				sizet -= sizeof(GAIA::U8);
			}
			return 0;
		}
		template<typename _SizeType> GAIA::N32 gmemcmp(
				const GAIA::GVOID* p1, const GAIA::GVOID* p2,
				const _SizeType& p1_stride, const _SizeType& p2_stride,
				const _SizeType& size, const _SizeType& count)
		{
			GAST(!!p1);
			GAST(!!p2);
			GAST(p1_stride > 0);
			GAST(p2_stride > 0);
			GAST(size > 0);
			GAST(count > 0);
			const GAIA::U8* pA = GSCAST(const GAIA::U8*)(p1);
			const GAIA::U8* pB = GSCAST(const GAIA::U8*)(p2);
			while(count > 0)
			{
				GAIA::N32 nCmp = GAIA::ALGO::gmemcmp(pA, pB, size);
				if(nCmp != 0)
					return nCmp;
				pA += p1_stride;
				pB += p2_stride;
				--count;
			}
			return 0;
		}
	}
}

#endif
