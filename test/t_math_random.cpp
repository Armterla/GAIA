#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_random(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 32;

		GAIA::CTN::Vector<GAIA::NUM> listRandom;

		GAIA::MATH::grand_seed(0);
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
			listRandom.push_back(GAIA::MATH::grand());
		GAIA::MATH::grand_seed(0);
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			if(GAIA::MATH::grand() != listRandom[x])
			{
				TERROR;
				break;
			}
		}
		listRandom.sort();
		GAIA::NUM sDropCount = listRandom.unique();
		if(sDropCount + 1 >= SAMPLE_COUNT)
			TERROR;
	}
}
