#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_trietree(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicTrieTree<GAIA::N32, GAIA::U32, GAIA::ALGO::ExtendGold<GAIA::U32> > __TrieType;
		static const GAIA::N32 COUNTX = 10;
		static const GAIA::N32 COUNTZ = 10;
		__TrieType::_datatype SAMPLES[COUNTZ][COUNTX];
		for(__TrieType::_datatype z = 0; z < COUNTZ; ++z)
			for(__TrieType::_datatype x = 0; x < COUNTX; ++x)
				SAMPLES[z][x] = z * COUNTX + x;
		__TrieType t;
		__TrieType::it it;
		__TrieType::const_it cit;
		TAST(t.empty());
		if(t.size())
			TERROR;
		if(t.count(t.root()) != 0)
			TERROR;
		if(t.catagory_count(t.root()) != 0)
			TERROR;
		if(t.full_count(t.root()) != 0)
			TERROR;
		__TrieType::it newit;
		__TrieType::const_it newcit;
		newit = t.frontit();
		TAST(newit.empty());
		newit = t.backit();
		TAST(newit.empty());
		newcit = t.const_frontit();
		TAST(newcit.empty());
		newcit = t.const_backit();
		TAST(newcit.empty());
		t.clear();
		t.destroy();
		for(__TrieType::_datatype z = 0; z < COUNTZ; ++z)
			t.insert(SAMPLES[z], COUNTX);
		if(t.empty())
			TERROR;
		if(t.size() != COUNTX * COUNTZ)
			TERROR;
		it = t.frontit();
		__TrieType::_sizetype uCount = 0;
		for(; !it.empty(); ++it)
			++uCount;
		if(uCount != 100)
			TERROR;
		cit = t.const_frontit();
		uCount = 0;
		for(; !cit.empty(); ++cit)
			++uCount;
		if(uCount != 100)
			TERROR;
		it = t.backit();
		uCount = 0;
		for(; !it.empty(); --it)
			++uCount;
		if(uCount != 100)
			TERROR;
		cit = t.const_backit();
		uCount = 0;
		for(; !cit.empty(); --cit)
			++uCount;
		if(uCount != 100)
			TERROR;
		t.destroy();
		TAST(t.empty());
		t.clear();
		TAST(t.empty());
		for(__TrieType::_datatype z = 0; z < COUNTZ; ++z)
			t.insert(SAMPLES[z], COUNTX);
		for(__TrieType::_datatype z = 0; z < COUNTZ; ++z)
		{
			if(!t.exist(SAMPLES[z], COUNTX))
			{
				TERROR;
				break;
			}

			if(!t.erase(SAMPLES[z], COUNTX))
			{
				TERROR;
				break;
			}
		}
		TAST(t.empty());
		if(t.size() != 0)
			TERROR;
		t.clear();
		for(__TrieType::_datatype z = 0; z < COUNTZ; ++z)
			t.insert(SAMPLES[z], COUNTX);
		__TrieType::_sizetype uLeafCount = 0;
		it = t.frontit();
		for(; !it.empty(); ++it)
		{
			if(t.leaf(it))
				++uLeafCount;
		}
		if(uLeafCount != COUNTZ)
			TERROR;
		uLeafCount = 0;
		cit = t.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			if(t.leaf(cit))
				++uLeafCount;
		}
		if(uLeafCount != COUNTZ)
			TERROR;
		it = t.frontit();
		for(; !it.empty(); ++it)
		{
			if(t.root(it))
			{
				TERROR;
				break;
			}
		}
		it = t.backit();
		for(; !it.empty(); --it)
		{
			if(t.root(it))
			{
				TERROR;
				break;
			}
		}
		it = t.frontit();
		for(; !it.empty(); ++it)
		{
			if(t.leaf(it))
			{
				__TrieType::it itt = it;
				__TrieType::_sizetype uParentTimes = 0;
				while(!itt.empty() && itt != t.root_it())
				{
					++uParentTimes;
					itt = t.parent_it(itt);
				}
				if(uParentTimes != COUNTX)
				{
					TERROR;
					break;
				}
			}
		}
		cit = t.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			if(t.leaf(cit))
			{
				__TrieType::const_it citt = cit;
				__TrieType::_sizetype uParentTimes = 0;
				while(!citt.empty() && citt != GSCAST(const __TrieType*)(&t)->root_it())
				{
					++uParentTimes;
					citt = t.parent_it(citt);
				}
				if(uParentTimes != COUNTX)
				{
					TERROR;
					break;
				}
			}
		}
		__TrieType t1 = t;
		TAST(t1 == t && t1 >= t && t1 <= t);
		if(t1 != t || t1 > t || t1 < t)
			TERROR;
		it = t1.frontit();
		cit = t1.const_frontit();
		for(; !it.empty(); ++it, ++cit)
		{
			__TrieType::_datatype arr[COUNTX];
			__TrieType::_sizetype index = 0;
			__TrieType::it ittemp = it;
			while(!ittemp.empty() && it != t1.root_it())
			{
				arr[index++] = *ittemp;
				ittemp = t1.parent_it(ittemp);
			}
			if(index == 0)
			{
				TERROR;
				break;
			}
			GAIA::ALGO::inverse(arr, arr + index - 1);
			__TrieType::it itfind_l = t1.upper_equal(arr, index);
			__TrieType::const_it citfind_l = GSCAST(const __TrieType*)(&t1)->upper_equal(arr, index);
			__TrieType::it itfind_u = t1.lower_equal(arr, index);
			__TrieType::const_it citfind_u = GSCAST(const __TrieType*)(&t1)->lower_equal(arr, index);
			if(itfind_l != itfind_u || itfind_l != it)
			{
				TERROR;
				break;
			}
			if(citfind_l != citfind_u || citfind_l != cit)
			{
				TERROR;
				break;
			}
			index = 0;
			for(__TrieType::_datatype x = *it + 1; x < COUNTX; ++x)
				arr[index++] = x;
			if(index > 0)
			{
				__TrieType::Node* pFinded = t1.find(t1.tonode(it), arr, index);
				if(pFinded == GNIL)
				{
					TERROR;
					break;
				}
				__TrieType::it itfind = t1.toit(*pFinded);
				if(itfind.empty())
				{
					TERROR;
					break;
				}
				if(!t1.leaf(itfind))
				{
					TERROR;
					break;
				}
			}
		}
		{
			__TrieType::it it = t1.frontit();
			__TrieType::const_it cit = t1.const_frontit();
			__TrieType::it it_b = t1.backit();
			__TrieType::const_it cit_b = t1.const_backit();
			TAST(it_b != it);
			if(it_b == it)
				TERROR;
			TAST(cit_b != cit);
			if(cit_b == cit)
				TERROR;
			if((it + 10) - t1.frontit() != 10)
				TERROR;
			if((cit + 10) - t1.const_frontit() != 10)
				TERROR;
			if(t1.backit() - (it_b - 10) != 10)
				TERROR;
			if(t1.const_backit() - (cit_b - 10) != 10)
				TERROR;
		}
		it = t1.frontit();
		cit = t1.const_frontit();
		for(__TrieType::_sizetype x = 0; x < t1.size(); ++x)
		{
			if(it.empty())
			{
				TERROR;
				break;
			}
			if(cit.empty())
			{
				TERROR;
				break;
			}
			__TrieType::Node* pNode = t1.tonode(it);
			if(pNode == GNIL)
			{
				TERROR;
				break;
			}
			__TrieType::it ittemp = t1.toit(*pNode);
			if(ittemp != it)
			{
				TERROR;
				break;
			}
			const __TrieType::Node* pConstNode = GSCAST(const __TrieType*)(&t1)->tonode(cit);
			if(pConstNode == GNIL)
			{
				TERROR;
				break;
			}
			__TrieType::const_it cittemp = GSCAST(const __TrieType*)(&t1)->toit(*pConstNode);
			if(cittemp != cit)
			{
				TERROR;
				break;
			}
			if(t1.leaf(it))
			{
				if(t1.count(*pNode) != 1)
				{
					TERROR;
					break;
				}
			}
			else
			{
				if(t1.count(*pNode) != 0)
				{
					TERROR;
					break;
				}
			}
			if(t1.catagory_count(*pNode) != 1)
			{
				TERROR;
				break;
			}
			if(t1.full_count(*pNode) != 1)
			{
				TERROR;
				break;
			}
			if(!t1.leaf(cit))
			{
				if(t1.childsize(*pNode) != 1)
				{
					TERROR;
					break;
				}
				__TrieType::__NodeTreeType::it c_itfront = t1.child_front_it(*pNode);
				if(c_itfront.empty())
				{
					TERROR;
					break;
				}
				__TrieType::__NodeTreeType::it c_itback = t1.child_back_it(*pNode);
				if(c_itback.empty())
				{
					TERROR;
					break;
				}
				if(c_itfront != c_itback)
				{
					TERROR;
					break;
				}
				__TrieType::Node* pChildNodeFront = t1.tonode(c_itfront);
				__TrieType::Node* pChildNodeBack = t1.tonode(c_itback);
				if(pChildNodeFront == GNIL || pChildNodeFront != pChildNodeBack)
				{
					TERROR;
					break;
				}
				__TrieType::__NodeTreeType::const_it c_citfront = t1.child_const_front_it(*pNode);
				if(c_citfront.empty())
				{
					TERROR;
					break;
				}
				__TrieType::__NodeTreeType::const_it c_citback = t1.child_const_back_it(*pNode);
				if(c_citback.empty())
				{
					TERROR;
					break;
				}
				if(c_citfront != c_citback)
				{
					TERROR;
					break;
				}
				const __TrieType::Node* pConstChildNodeFront = t1.tonode(c_citfront);
				const __TrieType::Node* pConstChildNodeBack = t1.tonode(c_citback);
				if(pConstChildNodeFront == GNIL || pConstChildNodeFront != pConstChildNodeBack)
				{
					TERROR;
					break;
				}
			}
			++it;
			++cit;
		}
	}
}
