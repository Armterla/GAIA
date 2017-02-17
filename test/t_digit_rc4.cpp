#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_rc4(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM BUFFER_SIZE = 1024 * 3 * 1 + 3;
		GAIA::U8* pSrc = gnew GAIA::U8[BUFFER_SIZE];
		for(GAIA::NUM x = 0; x < BUFFER_SIZE; ++x)
			pSrc[x] = (GAIA::U8)x;
		GAIA::U8* pDst = gnew GAIA::U8[BUFFER_SIZE];

		GAIA::DIGIT::RC4<1023> rc4;
		rc4.build_key("HelloWorld", 10);
		rc4.update(pSrc, pDst, BUFFER_SIZE);

		rc4.build_key("HelloWorld", 10);
		rc4.update(pDst, pSrc, BUFFER_SIZE);

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
