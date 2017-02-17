#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_stack(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicStack<GAIA::N32, GAIA::N32, GAIA::ALGO::ExtendGold<GAIA::N32> > __StackType;
		__StackType s;
		s.reserve(100);
		if(s.capacity() != 100)
			TERROR;
		if(s.size() != 0)
			TERROR;
		TAST(s.empty());
		__StackType::it newit;
		__StackType::const_it newcit;
		newit = s.frontit();
		TAST(newit.empty());
		newit = s.backit();
		TAST(newit.empty());
		newcit = s.const_frontit();
		TAST(newcit.empty());
		newcit = s.const_backit();
		TAST(newcit.empty());
		for(__StackType::_sizetype x = 0; x < s.capacity(); ++x)
			s.push_back(x);
		if(s.size() != s.capacity())
			TERROR;
		for(__StackType::_sizetype x = 0; x < s.size(); ++x)
		{
			if(s[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(s.capacity() != 100)
			TERROR;
		__StackType s1 = s;
		if(s1.capacity() != 100)
			TERROR;
		if(s1.size() != s1.capacity())
			TERROR;
		for(__StackType::_sizetype x = 0; x < s1.size(); ++x)
		{
			if(s1[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(__StackType::_sizetype size = s.size(); size > 0; --size)
		{
			if(s.back() != size - 1)
			{
				TERROR;
				break;
			}
			s.pop_back();
		}
		TAST(s.empty());
		s = s1;
		if(s.front() != s1.front())
			TERROR;
		s.resize(s.size() / 2);
		if(s.size() != s1.size() / 2)
			TERROR;
		for(__StackType::_sizetype x = 0; x < s.size(); ++x)
		{
			if(s.count(x) != 1)
			{
				TERROR;
				break;
			}
		}
		for(__StackType::_sizetype x = 0; s.capacity() != s.size(); ++x)
			s.push_back(x);
		if(s.size() != s.capacity())
			TERROR;
		s.resize(s1.size() / 2);
		for(__StackType::_sizetype x = 0; s.capacity() != s.size(); ++x)
			s.push_back(x);
		if(s.size() != s.capacity())
			TERROR;
		s1 = s;
		if(s > s1 || s < s1 || s != s1)
			TERROR;
		TAST(s >= s1 && s <= s1 && s == s1);
		__StackType::const_it cit = s.const_frontit();
		__StackType::it it = s1.frontit();
		for(; !it.empty(); ++it)
			*it = 10;
		it = s1.frontit();
		for(; !cit.empty(); ++cit, ++it)
			*it = *cit;
		if(s != s1)
			TERROR;
		__StackType s2;
		s2 = s1;
		s2 += s1;
		s += s;
		if(s != s2)
			TERROR;
		s = s1;
		cit = s.const_frontit();
		it = s.frontit();
		if((cit + 10) - cit != 10)
			TERROR;
		if((it + 10) - it != 10)
			TERROR;
		cit += 10;
		it += 10;
		if(cit - s.const_frontit() != 10)
			TERROR;
		if(it - s.frontit() != 10)
			TERROR;
		cit -= 20;
		it -= 20;
		TAST(cit.empty());
		TAST(it.empty());

		s.clear();
		for(GAIA::N32 x = 0; x < 100; ++x)
			s.push_back(x);
		s.resize_keep(200);
		if(s.size() != 200)
			TERROR;
		if(s.capacity() < 200)
			TERROR;
		for(GAIA::N32 x = 0; x < 100; ++x)
		{
			if(s[x] != x)
			{
				TERROR;
				break;
			}
		}
		s.resize_keep(50);
		if(s.size() != 50)
			TERROR;
		if(s.capacity() < 200)
			TERROR;
		for(GAIA::N32 x = 0; x < 50; ++x)
		{
			if(s[x] != x)
			{
				TERROR;
				break;
			}
		}
		s.destroy();
		s.resize_keep(100);
		if(s.size() != 100)
			TERROR;
		if(s.capacity() < 100)
			TERROR;
		s.resize_keep(0);
		if(s.size() != 0)
			TERROR;
		if(s.capacity() != 100)
			TERROR;

		s.destroy();
		for(GAIA::N32 x = 0; x < 100; ++x)
			s.push_back(x);
		s.reserve_keep(200);
		if(s.capacity() != 200)
			TERROR;
		if(s.size() != 100)
			TERROR;
		for(GAIA::N32 x = 0; x < 100; ++x)
		{
			if(s[x] != x)
			{
				TERROR;
				break;
			}
		}
		s.reserve_keep(50);
		if(s.capacity() != 50)
			TERROR;
		if(s.size() != 50)
			TERROR;
		for(GAIA::N32 x = 0; x < 50; ++x)
		{
			if(s[x] != x)
			{
				TERROR;
				break;
			}
		}
		s.destroy();
		s.reserve_keep(100);
		if(s.capacity() != 100)
			TERROR;
		if(s.size() != 0)
			TERROR;
		s.reserve_keep(0);
		if(s.capacity() != 0)
			TERROR;
		if(s.size() != 0)
			TERROR;
	}
}
