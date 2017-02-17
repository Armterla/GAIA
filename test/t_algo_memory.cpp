#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_algo_memory(GAIA::LOG::Log& logobj)
	{
		GAIA::U16 u16 = 1234;
		if(GAIA::ALGO::swapendian(GAIA::ALGO::swapendian(u16)) != 1234)
			TERROR;
		GAIA::U32 u32 = 12345678;
		if(GAIA::ALGO::swapendian(GAIA::ALGO::swapendian(u32)) != 12345678)
			TERROR;
		GAIA::U64 u64 = 1234567812345678;
		if(GAIA::ALGO::swapendian(GAIA::ALGO::swapendian(u64)) != 1234567812345678)
			TERROR;
	}
}
