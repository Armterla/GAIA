#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_md5(GAIA::LOG::Log& logobj)
	{
		GAIA::DIGIT::MD5 md5;
		GAIA::U8 md5res1[16];
		GAIA::U8 md5res2[16];
		md5.result(md5res1);

		md5.reset();
		for(GAIA::N32 x = 0; x < 2100; x += 3)
		{
			GAIA::U8 temp[3];
			for(GAIA::N32 y = 0; y < 3; ++y)
				temp[y] = (GAIA::U8)((x + y) % 256);
			md5.update(temp, sizeof(temp));
			GAIA::DIGIT::MD5::Context ctx = md5.context();
			md5.context(ctx);
		}
		md5.result(md5res1);

		md5.reset();
		for(GAIA::N32 x = 0; x < 2100; x += 7)
		{
			GAIA::U8 temp[7];
			for(GAIA::N32 y = 0; y < 7; ++y)
				temp[y] = (GAIA::U8)((x + y) % 256);
			md5.update(temp, sizeof(temp));
			GAIA::DIGIT::MD5::Context ctx = md5.context();
			md5.context(ctx);
		}
		md5.result(md5res2);

		if(GAIA::ALGO::gmemcmp(md5res1, md5res2, sizeof(md5res1)) != 0)
			TERROR;

		md5.reset();
		md5.update((const GAIA::U8*)"abcdefghijklmnopqrstuvwxyz", 26);
		md5.result(md5res1);
		GAIA::CH szMD5Res[33];
		GAIA::ALGO::hex2str(md5res1, 16, szMD5Res);
		if(GAIA::ALGO::gstrcmp(szMD5Res, "3BE167CA6C49FB7D00E49261D7D3FCC3") != 0)
			TERROR;
	}
}
