#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_algo_sort(GAIA::LOG::Log& logobj)
	{
		// Generate test data.
		GAIA::MATH::RandomLCG lcg;
		GAIA::CTN::Vector<GAIA::N64> listData;
		listData.resize(1000);
		for(GAIA::NUM x = 0; x < listData.size(); ++x)
			listData[x] = lcg.random_u16() % 0x8000 - 2000;
		GAIA::CTN::Vector<GAIA::N64> listDataSorted = listData;
		GAIA::ALGO::sort(listDataSorted.fptr(), listDataSorted.bptr());

		// Bucket sort.
		GAIA::CTN::Vector<GAIA::N64> listDataOrigin = listData;
		if(!GAIA::ALGO::ksort(listData.fptr(), listData.bptr()))
			TERROR;
		if(!GAIA::ALGO::issorted(listData.fptr(), listData.bptr()))
			TERROR;
		for(GAIA::NUM x = 0; x < listData.size(); ++x)
		{
			if(listData[x] != listDataSorted[x])
				TERROR;
		}

		// Quick sort.
		listData = listDataOrigin;
		GAIA::ALGO::qsort(listData.fptr(), listData.bptr());
		if(!GAIA::ALGO::issorted(listData.fptr(), listData.bptr()))
			TERROR;
		for(GAIA::NUM x = 0; x < listData.size(); ++x)
		{
			if(listData[x] != listDataSorted[x])
				TERROR;
		}
	}
}
