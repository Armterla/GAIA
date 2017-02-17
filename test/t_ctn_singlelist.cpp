#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_singlelist(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::SingleList<GAIA::NUM> __SingleListType;

		__SingleListType sl;
		TAST(sl.empty());
		if(sl.size() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
			sl.push_back(x);
		sl.clear();
		TAST(sl.empty());
		if(sl.size() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
			sl.push_back(x);
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			TAST(sl.pop_back());
		}
		TAST(sl.empty());
		if(sl.size() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
			sl.push_back(x);
		if(sl.front() != 0)
			TERROR;
		if(sl.back() != sl.size() - 1)
			TERROR;
		if(sl.backit() - sl.frontit() != sl.size() - 1)
			TERROR;
		if(sl.frontit() + (sl.size() - 1) != sl.backit())
			TERROR;
		TAST(sl.frontit() + (sl.size() - 1) == sl.backit());
		if(sl.const_backit() - sl.const_frontit() != sl.size() - 1)
			TERROR;
		if(sl.const_frontit() + (sl.size() - 1) != sl.const_backit())
			TERROR;
		TAST(sl.const_frontit() + (sl.size() - 1) == sl.const_backit());
		for(GAIA::NUM x = 0; x < sl.size(); ++x)
		{
			if(sl[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(sl.size() != 10)
			TERROR;
		if(sl.empty())
			TERROR;
		__SingleListType::it it = sl.frontit();
		__SingleListType::const_it cit = sl.const_frontit();
		it += sl.size() / 2;
		it -= sl.size() / 2;
		cit += sl.size() / 2;
		cit -= sl.size() / 2;
		if(it != sl.frontit())
			TERROR;
		if(cit != sl.const_frontit())
			TERROR;
		for(GAIA::NUM x = 0; !it.empty(); ++it, ++x)
		{
			if(*it != x)
			{
				TERROR;
				break;
			}
		}
		TAST(it.empty());
		for(GAIA::NUM x = 0; !cit.empty(); ++cit, ++x)
		{
			if(*cit != x)
			{
				TERROR;
				break;
			}
		}
		TAST(cit.empty());
		it = sl.frontit();
		for(GAIA::NUM x = 0; !it.empty(); ++x)
		{
			if(*it != x)
			{
				TERROR;
				break;
			}
			if(!it.erase())
			{
				TERROR;
				break;
			}
		}
		TAST(it.empty());
		TAST(sl.empty());
		if(sl.backit() - sl.frontit() != 0)
			TERROR;
		if(sl.backit() - sl.backit() != 0)
			TERROR;
		if(sl.const_backit() - sl.const_frontit() != 0)
			TERROR;
		if(sl.const_backit() - sl.const_backit() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
			sl.push_front(x);
		if(sl.size() != 10)
			TERROR;
		if(sl.empty())
			TERROR;
		if(sl.front() != sl.size() - 1)
			TERROR;
		if(sl.back() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < sl.size(); ++x)
		{
			if(sl[x] != sl.size() - x - 1)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			TAST(sl.pop_front());
		}
		TAST(sl.empty());
		if(sl.size() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(!sl.insert(x, x))
			{
				TERROR;
				break;
			}
		}
		for(GAIA::NUM x = 0; x < sl.size(); ++x)
		{
			if(sl[x] != x)
			{
				TERROR;
				break;
			}
		}
		TAST(sl.insert(-1, 0));
		TAST(sl.insert(123, 1));
		if(sl[0] != -1)
			TERROR;
		if(sl[1] != 123)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(sl[x + 2] != x)
			{
				TERROR;
				break;
			}
		}
		TAST(sl.erase(1));
		TAST(sl.erase(0));
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(sl[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(; !sl.empty(); )
		{
			if(!sl.erase(0))
			{
				TERROR;
				break;
			}
		}
		TAST(sl.empty());
		sl.insert(0, sl.frontit());
		sl.insert(1, sl.frontit());
		sl.insert(2, sl.backit());
		sl.insert(3, sl.frontit() + 1);
		if(sl[0] != 1)
			TERROR;
		if(sl[1] != 3)
			TERROR;
		if(sl[2] != 2)
			TERROR;
		if(sl[3] != 0)
			TERROR;
		TAST(sl.erase(it = sl.frontit() + 2));
		TAST(sl.erase(it = sl.frontit() + 2));
		TAST(sl.erase(it = sl.frontit() + 1));
		TAST(sl.erase(it = sl.frontit()));
		TAST(sl.empty());
		for(GAIA::NUM x = 0; x < 10; ++x)
			sl.insert(x, 0);
		sl.inverse();
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(sl[x] != x)
			{
				TERROR;
				break;
			}
		}
	}
}