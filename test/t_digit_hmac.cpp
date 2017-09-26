#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_hmac(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM DATA_SIZE = 10000;
		static const GAIA::NUM KEY_SIZE = 50;
		GAIA::CTN::AString strData;
		for(GAIA::NUM x = 0; x < DATA_SIZE; ++x)
		{
			GAIA::CH szTemp[2];
			szTemp[0] = '0' + (x % 10);
			szTemp[1] = '\0';
			strData += szTemp;
		}
		
		GAIA::CTN::AString strKey;
		for(GAIA::NUM x = 0; x < KEY_SIZE; ++x)
		{
			GAIA::CH szTemp[2];
			szTemp[0] = '0' + (x % 10);
			szTemp[1] = '\0';
			strKey += szTemp;
		}
		
		GAIA::U8 res[20];
		GAIA::DIGIT::hmacsha1(strData.fptr(), strData.size(), strKey.fptr(), strKey.size(), res);
		GAIA::ALGO::inverse(res, res + sizeofarray(res) - 1);
		
		GAIA::CH szTemp[41];
		GAIA::ALGO::hex2str(res, sizeofarray(res), szTemp);
		
		if(!GAIA::ALGO::gstriequal(szTemp, "91d0deaff2c413a3aea50a872f3d0d538ab5b6fe"))
			TERROR;
	}
}
