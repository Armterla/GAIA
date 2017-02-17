#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_tree(GAIA::LOG::Log& logobj)
	{
		static const GAIA::N32 ELEMENT_COUNT = 5;
		typedef GAIA::CTN::BasicTree<GAIA::N32, GAIA::U32, GAIA::ALGO::ExtendGold<GAIA::U32> > __TreeType;
		__TreeType tr;
		TAST(tr.empty());
		if(tr.size() != 0)
			TERROR;
		if(tr.capacity() != 0)
			TERROR;
		if(tr.root() != GNIL)
			TERROR;
		if(GSCAST(__TreeType*)(&tr)->root() != GNIL)
			TERROR;
		__TreeType::it newit;
		__TreeType::const_it newcit;
		newit = tr.frontit();
		TAST(newit.empty());
		newit = tr.backit();
		TAST(newit.empty());
		newcit = tr.const_frontit();
		TAST(newcit.empty());
		newcit = tr.const_backit();
		TAST(newcit.empty());
		if(tr.insert(123, GNIL) == GNIL)
			TERROR;
		if(tr.insert(321, GNIL) != GNIL)
			TERROR;
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
		{
			__TreeType::Node* pNode = tr.insert(x, tr.root());
			if(pNode == GNIL)
			{
				TERROR;
				break;
			}
			GAIA::BL bFailed = GAIA::False;
			for(GAIA::N32 y = 0; y < ELEMENT_COUNT; ++y)
			{
				__TreeType::Node* pChild = tr.insert(x + y, pNode);
				if(pChild == GNIL)
				{
					TERROR;
					bFailed = GAIA::True;
					break;
				}
				GAIA::BL bFailedFailed = GAIA::False;
				for(GAIA::N32 z = 0; z < ELEMENT_COUNT; ++z)
				{
					__TreeType::Node* pChildChild = tr.insert(x + y + z, pChild);
					if(pChildChild == GNIL)
					{
						TERROR;
						bFailedFailed = GAIA::True;
						break;
					}
				}
				if(bFailedFailed)
				{
					bFailed = GAIA::True;
					break;
				}
			}
			if(bFailed)
				break;
		}
		__TreeType::_sizetype tTotalNodeCount = 1 + ELEMENT_COUNT + ELEMENT_COUNT * ELEMENT_COUNT + ELEMENT_COUNT * ELEMENT_COUNT * ELEMENT_COUNT;
		if(tr.size() != tTotalNodeCount)
			TERROR;
		__TreeType::_sizetype tsize = 0;
		__TreeType::it it = tr.frontit();
		for(; !it.empty(); ++it)
			++tsize;
		if(tsize != tTotalNodeCount)
			TERROR;
		tsize = 0;
		it = tr.backit();
		for(; !it.empty(); --it)
			++tsize;
		if(tsize != tTotalNodeCount)
			TERROR;
		tsize = 0;
		__TreeType::const_it cit = tr.const_frontit();
		for(; !cit.empty(); ++cit)
			++tsize;
		if(tsize != tTotalNodeCount)
			TERROR;
		tsize = 0;
		cit = tr.const_backit();
		for(; !cit.empty(); --cit)
			++tsize;
		if(tsize != tTotalNodeCount)
			TERROR;
		if(tr.size() > tr.capacity())
			TERROR;
		__TreeType tr1 = tr;
		if(tr1.size() != tr.size() ||
			tr1.capacity() != tr.capacity() ||
			tr1.empty() != tr.empty())
			TERROR;
		it = tr.frontit();
		while(!it.empty())
		{
			__TreeType::it itnext = it;
			++itnext;
			__TreeType::Node* pNode = tr.tonode(it);
			GAST(pNode != GNIL);
			tr.erase(*pNode);
			if(tr.empty())
				break;
			it = itnext;
		}
		TAST(tr.empty());
		if(tr.size() != 0)
			TERROR;
		if(tr.capacity() == 0)
			TERROR;
		tr = tr1;
		it = tr.backit();
		while(!it.empty())
		{
			__TreeType::it itprev = it;
			--itprev;
			__TreeType::Node* pNode = tr.tonode(it);
			GAST(pNode != GNIL);
			tr.erase(*pNode);
			if(tr.empty())
				break;
			it = itprev;
		}
		TAST(tr.empty());
		if(tr.size() != 0)
			TERROR;
		if(tr.capacity() == 0)
			TERROR;
		tr = tr1;
		tr.clear();
		tr.destroy();
		TAST(tr.empty());
		if(tr.size() != 0)
			TERROR;
		if(tr.capacity() != 0)
			TERROR;
		tr = tr1;
		tr1.clear();
		tr1.destroy();
		tr.clear();
		tr.destroy();
		__TreeType::_datatype tdata = 0;
		if(tr.insert(tdata++, GNIL) == GNIL)
			TERROR;
		for(GAIA::N32 x = 0; x < ELEMENT_COUNT; ++x)
		{
			__TreeType::Node* pNode = tr.insert(tdata++, tr.root());
			if(pNode == GNIL)
			{
				TERROR;
				break;
			}
			GAIA::BL bFailed = GAIA::False;
			for(GAIA::N32 y = 0; y < ELEMENT_COUNT; ++y)
			{
				__TreeType::Node* pChild = tr.insert(tdata++, pNode);
				if(pChild == GNIL)
				{
					TERROR;
					bFailed = GAIA::True;
					break;
				}
				GAIA::BL bFailedFailed = GAIA::False;
				for(GAIA::N32 z = 0; z < ELEMENT_COUNT; ++z)
				{
					__TreeType::Node* pChildChild = tr.insert(tdata++, pChild);
					if(pChildChild == GNIL)
					{
						TERROR;
						bFailedFailed = GAIA::True;
						break;
					}
				}
				if(bFailedFailed)
				{
					bFailed = GAIA::True;
					break;
				}
			}
			if(bFailed)
				break;
		}
		if(tr.size() != tTotalNodeCount)
			TERROR;
		GAIA::CTN::Vector<__TreeType::_datatype> listTemp;
		listTemp.reserve(tTotalNodeCount);
		it = tr.frontit();
		for(; !it.empty(); ++it)
			listTemp.push_back(*it);
		TAST(GAIA::ALGO::issorted(listTemp.fptr(), listTemp.bptr()));
		GAIA::CTN::Vector<__TreeType::_datatype>::it itback_v = listTemp.backit();
		__TreeType::it itback = tr.backit();
		for(; !itback.empty(); --itback, --itback_v)
		{
			if(*itback != *itback_v)
			{
				TERROR;
				break;
			}
		}
	}
}
