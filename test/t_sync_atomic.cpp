#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_sync_atomic(GAIA::LOG::Log& logobj)
	{
		GAIA::SYNC::Atomic a, a1;
		GAIA::N64 n = a;
		if(n != 0)
			TERROR;
		if(a.Increase() != 1)
			TERROR;
		n = a;
		if(n != 1)
			TERROR;
		if(a.Increase() != 2)
			TERROR;
		n = a;
		if(n != 2)
			TERROR;
		if(a.Decrease() != 1)
			TERROR;
		n = a;
		if(n != 1)
			TERROR;
		if(a.Decrease() != 0)
			TERROR;
		n = a;
		if(n != 0)
			TERROR;
		a = 10;
		if((GAIA::N64)a != 10)
			TERROR;
		a1 = a;
		if((GAIA::N64)a1 != 10)
			TERROR;
		a = 12;
		if(a.Add(12) != 24)
			TERROR;
		if((GAIA::N64)a != 24)
			TERROR;
		if(a.Add(-14) != 10)
			TERROR;
		if((GAIA::N64)a != 10)
			TERROR;
		a = 13;
		a1 = 14;
		if(a.Add(a1) != 27)
			TERROR;
		if((GAIA::N64)a != 27)
			TERROR;
		a1 = -14;
		if(a.Add(a1) != 13)
			TERROR;
		if((GAIA::N64)a != 13)
			TERROR;
		if(a < (GAIA::SYNC::Atomic)13)
			TERROR;
		if(a > 13)
			TERROR;
		a.Set(0);
		if(a.Get() != 0)
			TERROR;
	}
}
