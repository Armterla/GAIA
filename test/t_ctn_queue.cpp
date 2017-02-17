#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_queue(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicQueue<GAIA::N32, GAIA::N32, GAIA::ALGO::ExtendGold<GAIA::N32> > __QueueType;
		__QueueType q;
		q.reserve(100);
		if(q.capacity() != 100)
			TERROR;
		if(q.size() != 0)
			TERROR;
		TAST(q.empty());
		__QueueType::it newit;
		__QueueType::const_it newcit;
		newit = q.frontit();
		TAST(newit.empty());
		newit = q.backit();
		TAST(newit.empty());
		newcit = q.const_frontit();
		TAST(newcit.empty());
		newcit = q.const_backit();
		TAST(newcit.empty());
		for(__QueueType::_sizetype x = 0; x < q.capacity(); ++x)
			q.push_back(x);
		if(q.size() != q.capacity())
			TERROR;
		for(__QueueType::_sizetype x = 0; x < q.size(); ++x)
		{
			if(q[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(q.capacity() != 100)
			TERROR;
		__QueueType q1 = q;
		if(q1.capacity() != 100)
			TERROR;
		if(q1.size() != q1.capacity())
			TERROR;
		for(__QueueType::_sizetype x = 0; x < q1.size(); ++x)
		{
			if(q1[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(__QueueType::_sizetype x = 0; q.size() != 0; ++x)
		{
			if(q.front() != x)
			{
				TERROR;
				break;
			}
			q.pop_front();
		}
		TAST(q.empty());
		q = q1;
		if(q.front() != q1.front())
			TERROR;
		q.resize(q.size() / 2);
		if(q.size() != q1.size() / 2)
			TERROR;
		for(__QueueType::_sizetype x = 0; x < q.size(); ++x)
		{
			if(q.count(x) != 1)
			{
				TERROR;
				break;
			}
		}
		for(__QueueType::_sizetype x = 0; q.capacity() != q.size(); ++x)
			q.push_back(x);
		if(q.size() != q.capacity())
			TERROR;
		q.resize(q1.size() / 2);
		for(__QueueType::_sizetype x = 0; q.capacity() != q.size(); ++x)
			q.push_back(x);
		if(q.size() != q.capacity())
			TERROR;
		q1 = q;
		if(q > q1 || q < q1 || q != q1)
			TERROR;
		TAST(q >= q1 && q <= q1 && q == q1);
		__QueueType::const_it cit = q.const_frontit();
		__QueueType::it it = q1.frontit();
		for(; !it.empty(); ++it)
			*it = 10;
		it = q1.frontit();
		for(; !cit.empty(); ++cit, ++it)
			*it = *cit;
		if(q != q1)
			TERROR;
		__QueueType q2;
		q2 = q1;
		q2 += q1;
		q += q;
		if(q != q2)
			TERROR;
		q = q1;
		cit = q.const_frontit();
		it = q.frontit();
		if((cit + 10) - cit != 10)
			TERROR;
		if((it + 10) - it != 10)
			TERROR;
		cit += 10;
		it += 10;
		if(cit - q.const_frontit() != 10)
			TERROR;
		if(it - q.frontit() != 10)
			TERROR;
		cit -= 20;
		it -= 20;
		TAST(cit.empty());
		TAST(it.empty());

		q.destroy();
		q.resize_keep(10);
		if(q.size() != 10)
			TERROR;
		if(q.capacity() < 10)
			TERROR;
		q.clear();
		for(GAIA::N32 x = 0; x < 10; ++x)
			q.push_back(x);
		q.resize_keep(20);
		if(q.size() != 20)
			TERROR;
		if(q.capacity() < 20)
			TERROR;
		for(GAIA::N32 x = 0; x < 10; ++x)
		{
			if(q[x] != x)
			{
				TERROR;
				break;
			}
		}
		q.resize_keep(5);
		if(q.size() != 5)
			TERROR;
		if(q.capacity() < 20)
			TERROR;
		for(GAIA::N32 x = 0; x < 5; ++x)
		{
			if(q[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::N32 x = 0; x < 5; ++x)
		{
			if(q.front() != x)
			{
				TERROR;
				break;
			}
			q.pop_front();
		}

		q.destroy();
		q.reserve_keep(10);
		if(q.size() != 0)
			TERROR;
		if(q.capacity() != 10)
			TERROR;
		for(GAIA::N32 x = 0; x < 10; ++x)
			q.push_back(x);
		q.reserve_keep(20);
		if(q.size() != 10)
			TERROR;
		if(q.capacity() != 20)
			TERROR;
		for(GAIA::N32 x = 0; x < 10; ++x)
		{
			if(q[x] != x)
			{
				TERROR;
				break;
			}
		}
		q.reserve_keep(5);
		if(q.size() != 5)
			TERROR;
		if(q.capacity() != 5)
			TERROR;
		for(GAIA::N32 x = 0; x < 5; ++x)
		{
			if(q[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::N32 x = 0; x < 5; ++x)
		{
			if(q.front() != x)
			{
				TERROR;
				break;
			}
			q.pop_front();
		}
	}
}
