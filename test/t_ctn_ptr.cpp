#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_pter(GAIA::LOG::Log& logobj)
	{
		static const GAIA::N32 ELEMENT_COUNT = 100;
		GAIA::N32* pElements = gnew GAIA::N32[ELEMENT_COUNT];
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
			pElements[x] = x;
		typedef GAIA::CTN::Pter<GAIA::N32> __PterType;
		{
			__PterType pp;
			TAST(pp.empty());
		}
		__PterType p = pElements;
		if(p.empty())
			TERROR;
		if(*p != 0)
			TERROR;
		if(**GSCAST(const __PterType*)(&p) != 0)
			TERROR;
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
		{
			if(p[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
		{
			if((*GSCAST(const __PterType*)(&p))[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
			p[x] = 123456;
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
		{
			if(p[x] != 123456)
			{
				TERROR;
				break;
			}
		}
		__PterType p1 = p;
		TAST(p1 == p && p1 >= p && p1 <= p);
		if(p1 != p || p1 > p || p1 < p)
			TERROR;
		TAST(p == pElements && p >= pElements && p <= pElements);
		if(p != pElements || p > pElements || p < pElements)
			TERROR;
		TAST(p == GSCAST(const GAIA::N32*)(pElements) && p >= GSCAST(const GAIA::N32*)(pElements) && p <= GSCAST(const GAIA::N32*)(pElements));
		if(p != GSCAST(const GAIA::N32*)(pElements) || p > GSCAST(const GAIA::N32*)(pElements) || p < GSCAST(const GAIA::N32*)(pElements))
			TERROR;
		GAIA::N32* pElementTemp = p;
		if(p != pElementTemp)
			TERROR;
		const GAIA::N32* pConstElementTemp = p;
		if(p != pConstElementTemp)
			TERROR;
		p1 = p + 1;
		if(p1 - p != 1)
			TERROR;
		if(p - p1 != -1)
			TERROR;
		p1 += 10;
		p1 -= 5;
		if(p1 - p != 6)
			TERROR;
		if(p != ((p + 10) - 10))
			TERROR;
		p1 = p;
		p1++;
		++p1;
		p1--;
		--p1;
		if(p1 != p)
			TERROR;
		if(p1 + 10 - p != 10)
			TERROR;
		if(p1 - 10 - p != -10)
			TERROR;
		gdel[] pElements;
		pElements = GNIL;
	}
}
