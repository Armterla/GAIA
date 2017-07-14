#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_base64(GAIA::LOG::Log& logobj)
	{
		GAIA::U8 newsrc[1024];
		GAIA::U8 src[1024];
		GAIA::U8 dst[1024];
		GAIA::N32 nResEncode, nResDecode;

		GAIA::ALGO::gstrcpy(src, "a");
		nResEncode = GAIA::DIGIT::base64encode(dst, (GAIA::N32)sizeof(dst), src, 1);
		nResDecode = GAIA::DIGIT::base64decode(newsrc, (GAIA::N32)sizeof(newsrc), dst, nResEncode);
		if(nResDecode != 1 || GAIA::ALGO::gmemcmp(src, newsrc, nResDecode) != 0)
			TERROR;

		GAIA::ALGO::gstrcpy(src, "abcdefghijklmnopqrstuvwxyz");
		nResEncode = GAIA::DIGIT::base64encode(dst, (GAIA::N32)sizeof(dst), src, 26);
		dst[nResEncode] = '\0';
		if(GAIA::ALGO::gstrcmp(dst, "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=") != 0)
			TERROR;
		nResDecode = GAIA::DIGIT::base64decode(newsrc, (GAIA::N32)sizeof(newsrc), dst, nResEncode);
		if(nResDecode != 26 || GAIA::ALGO::gmemcmp(src, newsrc, nResDecode) != 0)
			TERROR;

		GAIA::ALGO::gstrcpy(src, "a1b2c3d4!@#$%^&*(");
		nResEncode = GAIA::DIGIT::base64encode(dst, (GAIA::N32)sizeof(dst), src, 17);
		dst[nResEncode] = '\0';
		if(GAIA::ALGO::gstrcmp(dst, "YTFiMmMzZDQhQCMkJV4mKig=") != 0)
			TERROR;
		nResDecode = GAIA::DIGIT::base64decode(newsrc, (GAIA::N32)sizeof(newsrc), dst, nResEncode);
		if(nResDecode != 17 || GAIA::ALGO::gmemcmp(src, newsrc, nResDecode) != 0)
			TERROR;

		for(GAIA::N32 x = 0; x < 10000; ++x)
		{
			GAIA::NUM sSrcLen = GAIA::MATH::grand();
			sSrcLen %= 256;
			sSrcLen++;
			for(GAIA::N32 y = 0; y < sSrcLen; ++y)
				src[y] = (GAIA::U8)(GAIA::MATH::grand() % 26) + 'a';
			src[sSrcLen] = '\0';
			nResEncode = GAIA::DIGIT::base64encode(dst, (GAIA::N32)sizeof(dst), src, sSrcLen);
			dst[nResEncode] = '\0';
			nResDecode = GAIA::DIGIT::base64decode(newsrc, (GAIA::N32)sizeof(newsrc), dst, nResEncode);
			if(nResDecode != sSrcLen || GAIA::ALGO::gmemcmp(src, newsrc, nResDecode) != 0)
			{
				TERROR;
				break;
			}
		}
	}
}
