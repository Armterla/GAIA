#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_stackqueue(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicStackQueue<GAIA::N32, GAIA::N32, 100> __StackQueueType;
		__StackQueueType sq;
		if(sq.capacity() != 100)
			TERROR;
		if(sq.size() != 0)
			TERROR;
		TAST(sq.empty());
		__StackQueueType::it newit;
		__StackQueueType::const_it newcit;
		newit = sq.frontit();
		TAST(newit.empty());
		newit = sq.backit();
		TAST(newit.empty());
		newcit = sq.const_frontit();
		TAST(newcit.empty());
		newcit = sq.const_backit();
		TAST(newcit.empty());
		for(__StackQueueType::_sizetype x = 0; x < sq.capacity(); ++x)
		{
			if(!sq.push_back(x))
			{
				TERROR;
				break;
			}
		}
		if(sq.size() != sq.capacity())
			TERROR;
		for(__StackQueueType::_sizetype x = 0; x < sq.size(); ++x)
		{
			if(sq[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(sq.capacity() != 100)
			TERROR;
		__StackQueueType sq1 = sq;
		if(sq1.capacity() != 100)
			TERROR;
		if(sq1.size() != sq1.capacity())
			TERROR;
		for(__StackQueueType::_sizetype x = 0; x < sq1.size(); ++x)
		{
			if(sq1[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(__StackQueueType::_sizetype x = 0; sq.size() != 0; ++x)
		{
			if(sq.front() != x)
			{
				TERROR;
				break;
			}
			sq.pop_front();
		}
		TAST(sq.empty());
		sq = sq1;
		if(sq.front() != sq1.front())
			TERROR;
		TAST(sq.resize(sq.size() / 2));
		if(sq.size() != sq1.size() / 2)
			TERROR;
		for(__StackQueueType::_sizetype x = 0; x < sq.size(); ++x)
		{
			if(sq.count(x) != 1)
			{
				TERROR;
				break;
			}
		}
		for(__StackQueueType::_sizetype x = 0; sq.capacity() != sq.size(); ++x)
			sq.push_back(x);
		if(sq.size() != sq.capacity())
			TERROR;
		sq.resize(sq1.size() / 2);
		for(__StackQueueType::_sizetype x = 0; sq.capacity() != sq.size(); ++x)
			sq.push_back(x);
		if(sq.size() != sq.capacity())
			TERROR;
		sq1 = sq;
		if(sq > sq1 || sq < sq1 || sq != sq1)
			TERROR;
		TAST(sq >= sq1 && sq <= sq1 && sq == sq1);
		__StackQueueType::const_it cit = sq.const_frontit();
		__StackQueueType::it it = sq1.frontit();
		for(; !it.empty(); ++it)
			*it = 10;
		it = sq1.frontit();
		for(; !cit.empty(); ++cit, ++it)
			*it = *cit;
		if(sq != sq1)
			TERROR;
		__StackQueueType sq2;
		sq2 = sq1;
		sq2 += sq1;
		sq += sq;
		if(sq != sq2)
			TERROR;
		sq = sq1;
		cit = sq.const_frontit();
		it = sq.frontit();
		if((cit + 10) - cit != 10)
			TERROR;
		if((it + 10) - it != 10)
			TERROR;
		cit += 10;
		it += 10;
		if(cit - sq.const_frontit() != 10)
			TERROR;
		if(it - sq.frontit() != 10)
			TERROR;
		cit -= 20;
		it -= 20;
		TAST(cit.empty());
		TAST(it.empty());
	}
}
