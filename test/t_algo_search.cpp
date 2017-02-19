#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_algo_search(GAIA::LOG::Log& logobj)
	{
		GAIA::NUM list[10];
		for(GAIA::NUM x = 0; x < sizeofarray(list); ++x)
			list[x] = x * 2;
		for(GAIA::NUM x = 0; x < sizeofarray(list); ++x)
		{
			GAIA::NUM x2 = x * 2;
			GAIA::NUM* pFinded = GAIA::ALGO::find(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::glinear_search(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::gbinary_search(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::gbinary_search(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::upper_equal(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::lower_equal(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			GAIA::NUM count_result = 0;
			GAIA::ALGO::count(list, list + sizeofarray(list) - 1, x2, count_result);
			if(count_result != 1)
			{
				TERROR;
				break;
			}
			count_result = 0;
			GAIA::ALGO::count_order(list, list + sizeofarray(list) - 1, x2, count_result);
			if(count_result != 1)
			{
				TERROR;
				break;
			}
			count_result = 0;
			GAIA::ALGO::counts(list, list + sizeofarray(list) - 1, list, list + sizeofarray(list) - 1, count_result);
			if(count_result != sizeofarray(list))
			{
				TERROR;
				break;
			}
			count_result = 0;
			GAIA::ALGO::counts_order(list, list + sizeofarray(list) - 1, list, list + sizeofarray(list) - 1, count_result);
			if(count_result != sizeofarray(list))
			{
				TERROR;
				break;
			}
		}
	}
}
