#ifndef		__GAIA_DIGIT_CRYPT_H__
#define		__GAIA_DIGIT_CRYPT_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		GINL GAIA::GVOID crypt(const GAIA::GVOID* pSrc, const GAIA::GVOID* pDst, GAIA::U32 size)
		{
			GAST(pSrc != GNIL);
			GAST(pDst != GNIL);
			GAST(size > 0);
			static const GAIA::U64 L = 0x14EFECCA4A4BEA4C;
			static const GAIA::U8 C = 0xFD;
			GAIA::U32 lsize = size / sizeof(L);
			for(GAIA::U32 x = 0; x < lsize; ++x)
				((GAIA::U64*)pDst)[x] = ((const GAIA::U64*)pSrc)[x] ^ L;
			for(GAIA::U32 x = lsize * sizeof(L); x < size; ++x)
				((GAIA::U8*)pDst)[x] = ((const GAIA::U8*)pSrc)[x] ^ C;
		}
	}
}

#endif
