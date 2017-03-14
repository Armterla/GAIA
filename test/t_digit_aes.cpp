#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_aes(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM BUFFER_SIZE = 1024 * 1024 * 100;
		GAST(BUFFER_SIZE % 16 == 0);
		GAIA::U8* pSrc = gnew GAIA::U8[BUFFER_SIZE];
		for(GAIA::NUM x = 0; x < BUFFER_SIZE; ++x)
			pSrc[x] = (GAIA::U8)x;
		GAIA::U8* pDst = gnew GAIA::U8[BUFFER_SIZE];

		GAIA::MATH::RID128 rid;
		rid.uuid();

		GAIA::DIGIT::AES aes;
		aes.build_key(&rid, sizeof(rid), GAIA::True);
		for(GAIA::NUM x = 0; x < BUFFER_SIZE / 16; ++x)
			aes.update(pSrc + x * 16, pDst + x * 16, 16);

		aes.build_key(&rid, sizeof(rid), GAIA::False);
		for(GAIA::NUM x = 0; x < BUFFER_SIZE / 16; ++x)
			aes.update(pDst + x * 16, pSrc + x * 16, 16);

		for(GAIA::NUM x = 0; x < BUFFER_SIZE; ++x)
		{
			if(pSrc[x] != (GAIA::U8)x)
			{
				TERROR;
				break;
			}
		}

		gdel[] pSrc;
		pSrc = GNIL;
		gdel[] pDst;
		pDst = GNIL;
	}
}
