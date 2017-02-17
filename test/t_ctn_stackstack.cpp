#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_stackstack(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicStackStack<GAIA::N32, GAIA::N32, 100> __StackStackType;
		__StackStackType ss;
		if(ss.capacity() != 100)
			TERROR;
		if(ss.size() != 0)
			TERROR;
		TAST(ss.empty());
		__StackStackType::it newit;
		__StackStackType::const_it newcit;
		newit = ss.frontit();
		TAST(newit.empty());
		newit = ss.backit();
		TAST(newit.empty());
		newcit = ss.const_frontit();
		TAST(newcit.empty());
		newcit = ss.const_backit();
		TAST(newcit.empty());
		for(__StackStackType::_sizetype x = 0; x < ss.capacity(); ++x)
		{
			if(!ss.push_back(x))
			{
				TERROR;
				break;
			}
		}
		if(ss.size() != ss.capacity())
			TERROR;
		for(__StackStackType::_sizetype x = 0; x < ss.size(); ++x)
		{
			if(ss[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(ss.capacity() != 100)
			TERROR;
		__StackStackType ss1 = ss;
		if(ss1.capacity() != 100)
			TERROR;
		if(ss1.size() != ss1.capacity())
			TERROR;
		for(__StackStackType::_sizetype x = 0; x < ss1.size(); ++x)
		{
			if(ss1[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(__StackStackType::_sizetype size = ss.size(); size > 0; --size)
		{
			if(ss.back() != size - 1)
			{
				TERROR;
				break;
			}
			ss.pop_back();
		}
		TAST(ss.empty());
		ss = ss1;
		if(ss.front() != ss1.front())
			TERROR;
		TAST(ss.resize(ss.size() / 2));
		if(ss.size() != ss1.size() / 2)
			TERROR;
		for(__StackStackType::_sizetype x = 0; x < ss.size(); ++x)
		{
			if(ss.count(x) != 1)
			{
				TERROR;
				break;
			}
		}
		for(__StackStackType::_sizetype x = 0; ss.capacity() != ss.size(); ++x)
			ss.push_back(x);
		if(ss.size() != ss.capacity())
			TERROR;
		ss.resize(ss1.size() / 2);
		for(__StackStackType::_sizetype x = 0; ss.capacity() != ss.size(); ++x)
			ss.push_back(x);
		if(ss.size() != ss.capacity())
			TERROR;
		ss1 = ss;
		if(ss > ss1 || ss < ss1 || ss != ss1)
			TERROR;
		TAST(ss >= ss1 && ss <= ss1 && ss == ss1);
		__StackStackType::const_it cit = ss.const_frontit();
		__StackStackType::it it = ss1.frontit();
		for(; !it.empty(); ++it)
			*it = 10;
		it = ss1.frontit();
		for(; !cit.empty(); ++cit, ++it)
			*it = *cit;
		if(ss != ss1)
			TERROR;
		__StackStackType ss2;
		ss2 = ss1;
		ss2 += ss1;
		ss += ss;
		if(ss != ss2)
			TERROR;
		ss = ss1;
		cit = ss.const_frontit();
		it = ss.frontit();
		if((cit + 10) - cit != 10)
			TERROR;
		if((it + 10) - it != 10)
			TERROR;
		cit += 10;
		it += 10;
		if(cit - ss.const_frontit() != 10)
			TERROR;
		if(it - ss.frontit() != 10)
			TERROR;
		cit -= 20;
		it -= 20;
		TAST(cit.empty());
		TAST(it.empty());
	}
}
