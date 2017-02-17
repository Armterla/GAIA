#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_secset(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::Secset<GAIA::NUM> __SecsetType;
		__SecsetType st;
		TAST(st.empty());
		if(st.size() != 0)
			TERROR;
		st.clear();
		st.destroy();
		TAST(st.empty());
		if(st.size() != 0)
			TERROR;
		TAST(st.insert(5, 8, GAIA::True));
		TAST(st.insert(10, 12, GAIA::True));
		TAST(st.insert(22, 30, GAIA::True));
		if(st.empty())
			TERROR;
		if(st.size() != 3)
			TERROR;
		__SecsetType::__SetType::const_it it = st.const_frontit();
		if(it.empty())
			TERROR;
		{
			const __SecsetType::Node& n = *it;
			if(n.b != 5 || n.e != 8)
				TERROR;
			++it;
			if(it.empty())
				TERROR;
		}
		{
			const __SecsetType::Node& n = *it;
			if(n.b != 10 || n.e != 12)
				TERROR;
			++it;
			if(it.empty())
				TERROR;
		}
		{
			const __SecsetType::Node& n = *it;
			if(n.b != 22 || n.e != 30)
				TERROR;
			++it;
			TAST(it.empty());
		}
		TAST(st.insert(9, 9, GAIA::True));
		TAST(st.insert(13, 21, GAIA::True));
		if(st.empty())
			TERROR;
		if(st.size() != 1)
			TERROR;
		it = st.const_frontit();
		if(it.empty())
			TERROR;
		{
			const __SecsetType::Node& n = *it;
			if(n.b != 5 || n.e != 30)
				TERROR;
			++it;
			TAST(it.empty());
		}
		TAST(!st.erase(9, 9, GAIA::False));
		TAST(!st.erase(13, 21, GAIA::False));
		TAST(st.erase(9, 9, GAIA::True));
		TAST(st.erase(13, 21, GAIA::True));
		it = st.const_frontit();
		if(it.empty())
			TERROR;
		{
			const __SecsetType::Node& n = *it;
			if(n.b != 5 || n.e != 8)
				TERROR;
			++it;
			if(it.empty())
				TERROR;
		}
		{
			const __SecsetType::Node& n = *it;
			if(n.b != 10 || n.e != 12)
				TERROR;
			++it;
			if(it.empty())
				TERROR;
		}
		{
			const __SecsetType::Node& n = *it;
			if(n.b != 22 || n.e != 30)
				TERROR;
			++it;
			TAST(it.empty());
		}
		__SecsetType st1 = st;
		TAST(!st1.erase_next(8, GAIA::False));
		TAST(!st1.erase_prev(10, GAIA::False));
		TAST(st1.erase_next(8, GAIA::True));
		TAST(st1.size() == 1);
		{
			const __SecsetType::Node& n = *st1.const_frontit();
			if(n.b != 5 || n.e != 7)
				TERROR;
		}
		st1 = st;
		TAST(st1.erase_prev(22, GAIA::True));
		TAST(st1.size() == 1);
		{
			const __SecsetType::Node& n = *st1.const_frontit();
			if(n.b != 23 || n.e != 30)
				TERROR;
		}
		st1 = st;
		TAST(st1 == st);
		TAST(!(st1 != st));
		TAST(st1 >= st);
		TAST(st1 <= st);
		TAST(!(st1 > st));
		TAST(!(st1 < st));
		st.clear();
		TAST(st.const_frontit().empty());
		st.destroy();
	}
}