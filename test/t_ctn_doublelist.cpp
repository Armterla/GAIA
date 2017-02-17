#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_doublelist(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::DoubleList<GAIA::NUM> __DoubleListType;

		__DoubleListType dl;
		TAST(dl.empty());
		if(dl.size() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
			dl.push_back(x);
		dl.clear();
		TAST(dl.empty());
		if(dl.size() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
			dl.push_back(x);
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			TAST(dl.pop_back());
		}
		TAST(dl.empty());
		if(dl.size() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
			dl.push_back(x);
		if(dl.front() != 0)
			TERROR;
		if(dl.back() != dl.size() - 1)
			TERROR;
		if(dl.backit() - dl.frontit() != dl.size() - 1)
			TERROR;
		if(dl.frontit() + (dl.size() - 1) != dl.backit())
			TERROR;
		TAST(dl.frontit() + (dl.size() - 1) == dl.backit());
		if(dl.const_backit() - dl.const_frontit() != dl.size() - 1)
			TERROR;
		if(dl.const_frontit() + (dl.size() - 1) != dl.const_backit())
			TERROR;
		TAST(dl.const_frontit() + (dl.size() - 1) == dl.const_backit());
		for(GAIA::NUM x = 0; x < dl.size(); ++x)
		{
			if(dl[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(dl.size() != 10)
			TERROR;
		if(dl.empty())
			TERROR;
		__DoubleListType::it it = dl.frontit();
		__DoubleListType::const_it cit = dl.const_frontit();
		it += dl.size() / 2;
		it -= dl.size() / 2;
		cit += dl.size() / 2;
		cit -= dl.size() / 2;
		if(it != dl.frontit())
			TERROR;
		if(cit != dl.const_frontit())
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
		it = dl.frontit();
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
		TAST(dl.empty());
		if(dl.backit() - dl.frontit() != 0)
			TERROR;
		if(dl.backit() - dl.backit() != 0)
			TERROR;
		if(dl.const_backit() - dl.const_frontit() != 0)
			TERROR;
		if(dl.const_backit() - dl.const_backit() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
			dl.push_front(x);
		if(dl.size() != 10)
			TERROR;
		if(dl.empty())
			TERROR;
		if(dl.front() != dl.size() - 1)
			TERROR;
		if(dl.back() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < dl.size(); ++x)
		{
			if(dl[x] != dl.size() - x - 1)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			TAST(dl.pop_front());
		}
		TAST(dl.empty());
		if(dl.size() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(!dl.insert(x, x))
			{
				TERROR;
				break;
			}
		}
		for(GAIA::NUM x = 0; x < dl.size(); ++x)
		{
			if(dl[x] != x)
			{
				TERROR;
				break;
			}
		}
		TAST(dl.insert(-1, 0));
		TAST(dl.insert(123, 1));
		if(dl[0] != -1)
			TERROR;
		if(dl[1] != 123)
			TERROR;
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(dl[x + 2] != x)
			{
				TERROR;
				break;
			}
		}
		TAST(dl.erase(1));
		TAST(dl.erase(0));
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(dl[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(; !dl.empty(); )
		{
			if(!dl.erase(0))
			{
				TERROR;
				break;
			}
		}
		TAST(dl.empty());
		dl.insert(0, dl.frontit());
		dl.insert(1, dl.frontit());
		dl.insert(2, dl.backit());
		dl.insert(3, dl.frontit() + 1);
		if(dl[0] != 1)
			TERROR;
		if(dl[1] != 3)
			TERROR;
		if(dl[2] != 2)
			TERROR;
		if(dl[3] != 0)
			TERROR;
		TAST(dl.erase(it = dl.frontit() + 2));
		TAST(dl.erase(it = dl.frontit() + 2));
		TAST(dl.erase(it = dl.frontit() + 1));
		TAST(dl.erase(it = dl.frontit()));
		TAST(dl.empty());
		for(GAIA::NUM x = 0; x < 10; ++x)
			dl.insert(x, 0);
		dl.inverse();
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(dl[x] != x)
			{
				TERROR;
				break;
			}
		}
	}
}