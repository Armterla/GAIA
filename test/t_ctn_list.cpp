#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_list(GAIA::LOG::Log& logobj)
	{
		static const GAIA::N32 ELEMENT_SIZE = 100;
		typedef GAIA::CTN::BasicList<GAIA::N32, GAIA::N32, GAIA::ALGO::ExtendGold<GAIA::N32> > __ListType;
		__ListType l;
		TAST(l.empty());
		if(l.size() != 0)
			TERROR;
		l.destroy();
		TAST(l.empty());
		if(l.size() != 0)
			TERROR;
		l.clear();
		TAST(l.empty());
		if(l.size() != 0)
			TERROR;
		__ListType::it newit;
		__ListType::const_it newcit;
		newit = l.frontit();
		TAST(newit.empty());
		newit = l.backit();
		TAST(newit.empty());
		newcit = l.const_frontit();
		TAST(newcit.empty());
		newcit = l.const_backit();
		TAST(newcit.empty());
		__ListType::it it = l.frontit();
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
			l.insert(x, it);
		if(l.empty())
			TERROR;
		if(l.size() != ELEMENT_SIZE)
			TERROR;
		GAIA::N32 nIndex = 0;
		__ListType::const_it cit = l.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			if(*cit != nIndex)
			{
				TERROR;
				break;
			}
			++nIndex;
		}
		it = l.frontit();
		TAST(l.erase(it));
		it = l.backit();
		TAST(l.erase(it));
		while(!l.empty())
		{
			it = l.frontit();
			if(!l.erase(it))
			{
				TERROR;
				break;
			}
		}
		TAST(l.empty());
		if(l.size() != 0)
			TERROR;
		l.destroy();
		it = l.backit();
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
			l.insert(x, it);
		it = l.frontit();
		while(!it.empty())
			l.erase(it);
		TAST(l.empty());
		if(l.size() != 0)
			TERROR;
		l.destroy();
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
			l.push_back(x);
		if(l.empty())
			TERROR;
		if(l.size() != ELEMENT_SIZE)
			TERROR;
		cit = l.const_frontit();
		nIndex = 0;
		for(; !cit.empty(); ++cit)
		{
			if(*cit != nIndex)
			{
				TERROR;
				break;
			}
			++nIndex;
		}
		while(!l.empty())
		{
			--nIndex;
			if(l.back() != nIndex)
			{
				TERROR;
				break;
			}
			l.pop_back();
		}
		l.destroy();
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
			l.push_front(x);
		if(l.empty())
			TERROR;
		if(l.size() != ELEMENT_SIZE)
			TERROR;
		cit = l.const_backit();
		nIndex = 0;
		for(; !cit.empty(); --cit)
		{
			if(*cit != nIndex)
			{
				TERROR;
				break;
			}
			++nIndex;
		}
		while(!l.empty())
		{
			--nIndex;
			if(l.front() != nIndex)
			{
				TERROR;
				break;
			}
			l.pop_front();
		}
		l.clear();
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
			l.push_front(x);
		__ListType l1;
		l1 = l;
		if(l > l1 || l < l1 || l != l1)
			TERROR;
		TAST(l >= l1 && l <= l1 && l == l1);
		l += l;
		if(l.size() != ELEMENT_SIZE * 2)
			TERROR;
		l.clear();
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
			l.push_back(x);
		l.inverse();
		cit = l.const_frontit();
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(*cit != ELEMENT_SIZE - x - 1)
			{
				TERROR;
				break;
			}
			++cit;
		}

		l.destroy();
		for(GAIA::NUM x = 0; x < 3; ++x)
			l.push_back(x);
		it = l.frontit();
		TAST(it.erase());
		if(it.empty())
			TERROR;
		if(*it != 1)
			TERROR;
		TAST(it.erase());
		if(it.empty())
			TERROR;
		if(*it != 2)
			TERROR;
		TAST(it.erase());
		TAST(it.empty());
		TAST(l.empty());

		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!l.insert(x, x))
			{
				TERROR;
				break;
			}
		}
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(l[x] != x)
			{
				TERROR;
				break;
			}
		}
		TAST(l.erase(0));
		TAST(l.erase(l.size() - 1));
		for(GAIA::N32 x = 0; x < ELEMENT_SIZE - 2; ++x)
		{
			if(!l.erase(0))
			{
				TERROR;
				break;
			}
		}
		TAST(l.empty());
		if(l.size() != 0)
			TERROR;

		for(GAIA::NUM x = 0; x < 3; ++x)
			l.push_back(x);
		GAIA::ITERATOR::InverseIterator<__ListType::_datatype, __ListType::it> iit = l.backit();
		for(GAIA::NUM x = 0; x < l.size(); ++x)
		{
			if(*iit != l.size() - x - 1)
			{
				TERROR;
				break;
			}
			++iit;
		}
		iit = l.backit();
		TAST(iit.erase());
		if(iit.empty())
			TERROR;
		if(*iit != 1)
			TERROR;
		TAST(iit.erase());
		if(iit.empty())
			TERROR;
		if(*iit != 0)
			TERROR;
		TAST(iit.erase());
		TAST(iit.empty());
		TAST(l.empty());
	}
}
