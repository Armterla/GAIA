#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_sha1(GAIA::LOG::Log& logobj)
	{
		GAIA::DIGIT::SHA1 sha1;
		GAIA::U8 sha1res1[20];
		GAIA::U8 sha1res2[20];
		sha1.result(sha1res1);

		sha1.reset();
		for(GAIA::N32 x = 0; x < 2100; x += 3)
		{
			GAIA::U8 temp[3];
			for(GAIA::N32 y = 0; y < 3; ++y)
				temp[y] = (GAIA::U8)((x + y) % 256);
			sha1.update(temp, sizeof(temp));
			GAIA::DIGIT::SHA1::Context ctx = sha1.context();
			sha1.context(ctx);
		}
		sha1.result(sha1res1);

		sha1.reset();
		for(GAIA::N32 x = 0; x < 2100; x += 7)
		{
			GAIA::U8 temp[7];
			for(GAIA::N32 y = 0; y < 7; ++y)
				temp[y] = (GAIA::U8)((x + y) % 256);
			sha1.update(temp, sizeof(temp));
			GAIA::DIGIT::SHA1::Context ctx = sha1.context();
			sha1.context(ctx);
		}
		sha1.result(sha1res2);

		if(GAIA::ALGO::gmemcmp(sha1res1, sha1res2, sizeof(sha1res1)) != 0)
			TERROR;

		sha1.reset();
		sha1.update((const GAIA::U8*)"abcdefghijklmnopqrstuvwxyz", 26);
		sha1.result(sha1res1);
		GAIA::ALGO::inverse(sha1res1, sha1res1 + sizeof(sha1res1) - 1);
		GAIA::CH szSHA1Res[41];
		GAIA::ALGO::hex2str(sha1res1, 20, szSHA1Res);
		if(GAIA::ALGO::gstrcmp(szSHA1Res, "32D10C7B8CF96570CA04CE37F2A19D84240D3A89") != 0)
			TERROR;
	}
}
