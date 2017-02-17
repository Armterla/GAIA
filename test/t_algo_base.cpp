#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_algo_base(GAIA::LOG::Log& logobj)
	{
		GAIA::NUM list[10];
		for(GAIA::NUM x = 0; x < sizeofarray(list); ++x)
			list[x] = x;
		GAIA::ALGO::inverse(list, list + sizeofarray(list) - 1);
		for(GAIA::NUM x = 0; x < sizeofarray(list); ++x)
		{
			if(list[x] != sizeofarray(list) - x - 1)
			{
				TERROR;
				break;
			}
		}
	}
}
