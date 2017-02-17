#ifndef		__GAIA_ALGO_HASH_LCG_H__
#define		__GAIA_ALGO_HASH_LCG_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		GINL GAIA::GVOID hash_lcg(const GAIA::GVOID* pSrc, GAIA::GVOID* pDst, GAIA::U32 size)
		{
			GAST(pSrc != GNIL);
			GAST(pDst != GNIL);
			GAST(size > 0);
			static const GAIA::U32 A = 214013;
			static const GAIA::U32 C = 2531011;
			GAIA::U32 ssize = size / sizeof(GAIA::U16);
			for(GAIA::U32 x = 0; x < ssize; ++x)
				((GAIA::U16*)pDst)[x] = (((((const GAIA::U16*)pSrc)[x] * A + C) >> 16) & 0x7FFF) % 65536;
			for(GAIA::U32 x = ssize * sizeof(GAIA::U16); x < size; ++x)
				((GAIA::U8*)pDst)[x] = (((((const GAIA::U8*)pSrc)[x] * A + C) >> 16) & 0x7FFF) % 256;
		}
		GINL GAIA::GVOID hash_lcg(const GAIA::GVOID* pSrc, GAIA::GVOID* pDst, GAIA::U32 size_src, GAIA::U32 size_dst)
		{
		}
	}
}

#endif
