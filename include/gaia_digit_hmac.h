#ifndef		__GAIA_DIGIT_HMAC_H__
#define		__GAIA_DIGIT_HMAC_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"
#include "gaia_digit_sha1.h"

namespace GAIA
{
	namespace DIGIT
	{
		GINL GAIA::GVOID hmacsha1(const GAIA::GVOID* pSrc, GAIA::NUM sSrcLen, const GAIA::GVOID* pKey, GAIA::NUM sKeyLen, GAIA::GVOID* pRes)
		{
			GPCHR_TRUE(pSrc == GNIL);
			GPCHR_TRUE(sSrcLen <= 0);
			GPCHR_TRUE(pKey == GNIL);
			GPCHR_TRUE(sKeyLen <= 0);
			GPCHR_TRUE(pRes == GNIL);
			const GAIA::U8* pSrcPrac = (const GAIA::U8*)pSrc;
			const GAIA::U8* pKeyPrac = (const GAIA::U8*)pKey;
			GAIA::U8* res = (GAIA::U8*)pRes;
			GAIA::U8 kipad[65];
			GAIA::U8 kopad[65];
			GAIA::U8 tk[20];
			GAIA::U8* temp;
			if(sKeyLen > 64)
			{
				GAIA::DIGIT::SHA1 sha1t;
				sha1t.update(pKeyPrac, sKeyLen);
				sha1t.result(tk);
				pKeyPrac = tk;
				sKeyLen = 20;
			}

			zeromems(kipad);
			zeromems(kopad);
			GAIA::ALGO::gmemcpy(kipad, pKeyPrac, sKeyLen);
			GAIA::ALGO::gmemcpy(kopad, pKeyPrac, sKeyLen);

			for(GAIA::NUM i = 0; i < 64; i++)
			{
				kipad[i] ^= 0x36;
				kopad[i] ^= 0x5c;
			}
			temp = gnew GAIA::U8[65 + sSrcLen];
			GAIA::ALGO::gmemset(temp, 0, 65 + sSrcLen);
			GAIA::ALGO::gmemcpy(temp, kipad, 64);
			GAIA::ALGO::gmemcpy(temp + 64, pSrcPrac, sSrcLen);

			GAIA::DIGIT::SHA1 sha1;
			sha1.update(temp, 64 + sSrcLen);
			sha1.result(res);
			for(GAIA::NUM x = 0; x < 20; x += 4)
			{
				GAIA::ALGO::swap(res[x + 0], res[x + 3]);
				GAIA::ALGO::swap(res[x + 1], res[x + 2]);
			}
			gdel[] temp;

			GAIA::U8 buf2[65 + 20];
			temp = buf2;
			GAIA::ALGO::gmemset(temp, 0, 65 + 20);
			GAIA::ALGO::gmemcpy(temp, kopad, 64);
			GAIA::ALGO::gmemcpy(temp + 64, res, 20);
			sha1.reset();
			sha1.update(temp, 64 + 20);
			sha1.result(res);
			for(GAIA::NUM x = 0; x < 20; x += 4)
			{
				GAIA::ALGO::swap(res[x + 0], res[x + 3]);
				GAIA::ALGO::swap(res[x + 1], res[x + 2]);
			}
		}
	}
}

#endif
