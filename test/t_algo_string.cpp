#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_algo_string(GAIA::LOG::Log& logobj)
	{
		/* start and end with */
		{
			if(!GAIA::ALGO::gstrstartwith("Hello World", "Hello"))
				TERROR;
			if(!GAIA::ALGO::gstrendwith("Hello World", "World"))
				TERROR;

			if(!GAIA::ALGO::gstrstartwithi("Hello World", "Hello"))
				TERROR;
			if(!GAIA::ALGO::gstrendwithi("Hello World", "World"))
				TERROR;

			if(!GAIA::ALGO::gstrstartwithi("Hello World", "hello"))
				TERROR;
			if(!GAIA::ALGO::gstrendwithi("Hello World", "world"))
				TERROR;

			if(GAIA::ALGO::gstrstartwith("Hello World", "abc"))
				TERROR;
			if(GAIA::ALGO::gstrendwith("Hello World", "abc"))
				TERROR;

			if(GAIA::ALGO::gstrstartwithi("Hello World", "abc"))
				TERROR;
			if(GAIA::ALGO::gstrendwithi("Hello World", "abc"))
				TERROR;

			if(GAIA::ALGO::gstrstartwithi("Hello World", "abc"))
				TERROR;
			if(GAIA::ALGO::gstrendwithi("Hello World", "abc"))
				TERROR;
		}

		/* Hex convert. */
		{
			GAIA::U64 u = 0x1234567890ABCDEF;
			GAIA::TCH szTemp[32];
			GAIA::ALGO::hex2str(GRCAST(const GAIA::U8*)((GAIA::GVOID*)&u), sizeof(u), szTemp);
			if(GAIA::ALGO::gstrcmp(szTemp, _T("1234567890ABCDEF")) != 0)
				TERROR;
			u = 0;
			GAIA::ALGO::str2hex(szTemp, sizeof(u), GRCAST(GAIA::U8*)(&u));
			if(u != 0x1234567890ABCDEF)
				TERROR;
		}

		// Punctuation test.
		{
			static const GAIA::CH puns[] = "`-=[]\\;\',./~!@#$%^&*()_+{}|:\"<>?";
			for(GAIA::NUM x = 0; x < 256; ++x)
			{
				GAIA::BL bExist = GAIA::False;
				for(GAIA::NUM y = 0; y < sizeof(puns) - 1; ++y)
				{
					if(puns[y] == x)
					{
						bExist = GAIA::True;
						break;
					}
				}
				if(GAIA::ALGO::ispunc(x) != bExist)
				{
					TERROR;
					break;
				}
			}
		}
	}
}
