#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_algo_set(GAIA::LOG::Log& logobj)
	{
		GAIA::CH szTemp[] = "HelloWorld";
		GAIA::N32 nLen = GAIA::ALGO::gstrlen(szTemp);
		GAIA::ALGO::set_random(szTemp, szTemp + nLen - 1, 10);
		GAIA::ALGO::set_random(szTemp, szTemp + nLen - 1, 100);

		GAIA::CH sz1[4];
		GAIA::CH sz2[4];
		GAIA::CH sz[7];
		sz[6] = '\0';

		GAIA::ALGO::gstrcpy(sz1, "123");
		GAIA::ALGO::gstrcpy(sz2, "ABC");
		if(GAIA::ALGO::set_merge(sz, sz + 7, sz1, sz1 + 2, sz2, sz2 + 2) != 6)
			TERROR;
		if(GAIA::ALGO::gstrcmp(sz, "123ABC") != 0)
			TERROR;
		if(GAIA::ALGO::set_merge_sorted(sz, sz + 7, sz1, sz1 + 2, sz2, sz2 + 2) != 6)
			TERROR;
		if(GAIA::ALGO::gstrcmp(sz, "123ABC") != 0)
			TERROR;

		GAIA::ALGO::gstrcpy(sz1, "1a2");
		GAIA::ALGO::gstrcpy(sz2, "b3c");
		if(GAIA::ALGO::set_merge_sorted(sz, sz + 6, sz1, sz1 + 2, sz2, sz2 + 2) != 6)
			TERROR;
	}
}
