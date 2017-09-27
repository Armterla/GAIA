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
		/*!
			@brief Calculate HMAC-SHA1 digest.

			@param pSrc [in] Specify the source data.

			@param sSrcLen [in] Specify the source data length in bytes.

			@param pKey [in] Specify the key data.

			@param sKeyLen [in] Specify the key length in bytes.

			@param pRes [out] Used for saving the HMAC-SHA1 digest result, it's length must 20 bytes at least.

			@return If success return GAIA::True or will return GAIA::False.
		 
			@remarks
				If the parameter pSrc == GNIL, this function will failed and return GAIA::False.
				If sSrcLen <= 0, this function will failed and return GAIA::False.
				If pKey == GNIL, this function will failed and return GAIA::False.
				If sKeyLen <= 0, this function will failed and return GAIA::False.
				If pRes == GNIL, this function will failed and return GAIA::False.
		*/
		GINL GAIA::BL hmacsha1(const GAIA::GVOID* pSrc, GAIA::NUM sSrcLen, const GAIA::GVOID* pKey, GAIA::NUM sKeyLen, GAIA::GVOID* pRes)
		{
			GPCHR_TRUE_RET(pSrc == GNIL, GAIA::False);
			GPCHR_TRUE_RET(sSrcLen <= 0, GAIA::False);
			GPCHR_TRUE_RET(pKey == GNIL, GAIA::False);
			GPCHR_TRUE_RET(sKeyLen <= 0, GAIA::False);
			GPCHR_TRUE_RET(pRes == GNIL, GAIA::False);
			const GAIA::U8* pSrcPrac = (const GAIA::U8*)pSrc;
			const GAIA::U8* pKeyPrac = (const GAIA::U8*)pKey;
			GAIA::U8* res = (GAIA::U8*)pRes;
			
			static const GAIA::NUM BLOCK_SIZE = 64;
			
			GAIA::U8 innerpad[BLOCK_SIZE];
			GAIA::U8 outerpad[BLOCK_SIZE];
			
			GAIA::ALGO::gmemset(innerpad, 0x36, sizeof(innerpad));
			GAIA::ALGO::gmemset(outerpad, 0x5C, sizeof(outerpad));
			
			for(GAIA::NUM x = 0; x < sKeyLen; ++x)
			{
				innerpad[x] = innerpad[x] ^ pKeyPrac[x];
				outerpad[x] = outerpad[x] ^ pKeyPrac[x];
			}
			
			GAIA::U8 sha1res[20];
			
			// Inner padding.
			{
				GAIA::DIGIT::SHA1 sha1;
				sha1.update(innerpad, sizeof(innerpad));
				sha1.update(pSrcPrac, sSrcLen);
				sha1.result(sha1res);
			}
			
			// Outer padding.
			{
				GAIA::DIGIT::SHA1 sha1;
				sha1.update(outerpad, sizeof(outerpad));
				sha1.update(sha1res, sizeof(sha1res));
				sha1.result(res);
			}
			
			return GAIA::True;
		}
	}
}

#endif
