#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_hmac(GAIA::LOG::Log& logobj)
	{
		GAIA::CTN::AString strData;
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			GAIA::CH szTemp[2];
			szTemp[0] = '0' + (x % 10);
			szTemp[1] = '\0';
			strData += szTemp;
		}
		
		GAIA::CTN::AString strKey;
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			GAIA::CH szTemp[2];
			szTemp[0] = '0' + (x % 10);
			szTemp[1] = '\0';
			strKey += szTemp;
		}
		
		GAIA::U8 res[20];
		GAIA::DIGIT::hmacsha1(strKey.fptr(), strKey.size(), strKey.fptr(), strKey.size(), res);
		GAIA::ALGO::inverse(res, res + sizeofarray(res) - 1);
		
		GAIA::CH szTemp[41];
		GAIA::ALGO::hex2str(res, sizeofarray(res), szTemp);
		
		if(!GAIA::ALGO::gstriequal(szTemp, "cb070897dd78957c2a2976105b4b38ede43cb3e4"))
			TERROR;
	}
}
