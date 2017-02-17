#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_ref(GAIA::LOG::Log& logobj)
	{
		static const GAIA::N32 ELEMENT_COUNT = 100;
		GAIA::N32* pElements = gnew GAIA::N32[ELEMENT_COUNT];
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
			pElements[x] = x;

		typedef GAIA::CTN::Ref<GAIA::N32> __RefType;
		{
			__RefType rr;
			TAST(rr.empty());
		}
		__RefType r = pElements;
		if(r.empty())
			TERROR;
		if(*r != 0)
			TERROR;
		if(**GSCAST(const __RefType*)(&r) != 0)
			TERROR;
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
		{
			if(r[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
		{
			if((*GSCAST(const __RefType*)(&r))[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
			r[x] = 123456;
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
		{
			if(r[x] != 123456)
			{
				TERROR;
				break;
			}
		}
		__RefType r1 = r;
		TAST(r1 == r && r1 >= r && r1 <= r);
		if(r1 != r || r1 > r || r1 < r)
			TERROR;
		TAST(r == pElements && r >= pElements && r <= pElements);
		if(r != pElements || r > pElements || r < pElements)
			TERROR;
		TAST(r == GSCAST(const GAIA::N32*)(pElements) && r >= GSCAST(const GAIA::N32*)(pElements) && r <= GSCAST(const GAIA::N32*)(pElements));
		if(r != GSCAST(const GAIA::N32*)(pElements) || r > GSCAST(const GAIA::N32*)(pElements) || r < GSCAST(const GAIA::N32*)(pElements))
			TERROR;
		GAIA::N32* pElementTemp = r;
		if(r != pElementTemp)
			TERROR;
		const GAIA::N32* pConstElementTemp = r;
		if(r != pConstElementTemp)
			TERROR;
		gdel[] pElements;
		pElements = GNIL;
	}
}
