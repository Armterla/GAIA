#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_set(GAIA::LOG::Log& logobj)
	{
		static const GAIA::N32 ELEMENT_SIZE = 100;
		typedef GAIA::CTN::BasicSet<GAIA::N32, GAIA::U32, GAIA::U32, GAIA::ALGO::ExtendGold<GAIA::N32> > __SetType;
		__SetType s;
		TAST(s.empty());
		if(s.size() != 0)
			TERROR;
		if(s.capacity() != 0)
			TERROR;
		__SetType::it newit;
		__SetType::const_it newcit;
		newit = s.frontit();
		TAST(newit.empty());
		newit = s.backit();
		TAST(newit.empty());
		newcit = s.const_frontit();
		TAST(newcit.empty());
		newcit = s.const_backit();
		TAST(newcit.empty());
		s.clear();
		s.destroy();
		for(__SetType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			__SetType::it itinsert;
			if(!s.insert(x, &itinsert) || *itinsert != x)
			{
				TERROR;
				break;
			}
		}
		for(__SetType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			__SetType::it itinsert;
			if(s.insert(x, &itinsert) || *itinsert != x)
			{
				TERROR;
				break;
			}
		}
		for(__SetType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(s.find(x) == GNIL)
			{
				TERROR;
				break;
			}
			if(!s.find(x))
			{
				TERROR;
				break;
			}
			if(GSCAST(const __SetType*)(&s)->find(x) == GNIL)
			{
				TERROR;
				break;
			}
			__SetType::it ittemp = s.findit(x);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
			__SetType::const_it cittemp = s.const_findit(x);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
			__SetType::it it_l = s.upper_equal(x);
			if(it_l.empty())
			{
				TERROR;
				break;
			}
			__SetType::const_it cit_l = GSCAST(const __SetType*)(&s)->upper_equal(x);
			if(cit_l.empty())
			{
				TERROR;
				break;
			}
			__SetType::it it_u = s.lower_equal(x);
			if(it_l.empty())
			{
				TERROR;
				break;
			}
			__SetType::const_it cit_u = GSCAST(const __SetType*)(&s)->lower_equal(x);
			if(cit_l.empty())
			{
				TERROR;
				break;
			}
		}
		for(__SetType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!s.erase(x))
			{
				TERROR;
				break;
			}
		}
		s.destroy();
		TAST(s.empty());
		if(s.size() != 0)
			TERROR;
		if(s.capacity() != 0)
			TERROR;
		for(__SetType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!s.insert(x))
			{
				TERROR;
				break;
			}
		}
		__SetType::it itfront = s.frontit();
		__SetType::const_it citfront = s.const_frontit();
		__SetType::it itback = s.backit();
		__SetType::const_it citback = s.const_backit();
		for(__SetType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(itfront.empty() ||
				citfront.empty() ||
				itback.empty() ||
				citback.empty())
			{
				TERROR;
				break;
			}
			if(*itfront != x ||
				*citfront != x ||
				*itback != ELEMENT_SIZE - x - 1 ||
				*citback != ELEMENT_SIZE - x - 1)
			{
				TERROR;
				break;
			}
			++itfront;
			++citfront;
			--itback;
			--citback;
		}
		if(!itfront.empty() ||
			!citfront.empty() ||
			!itback.empty() ||
			!citback.empty())
		{
			TERROR;
		}
		s.clear();
		TAST(s.empty());
		if(s.size() != 0)
			TERROR;
		if(s.capacity() == 0)
			TERROR;
		itfront = s.frontit();
		citfront = s.const_frontit();
		itback = s.backit();
		citback = s.const_backit();
		if(!itfront.empty() ||
			!citfront.empty() ||
			!itback.empty() ||
			!citback.empty())
		{
			TERROR;
		}
		if(itfront != itback)
			TERROR;
		if(citfront != citback)
			TERROR;
		for(__SetType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!s.insert(x))
			{
				TERROR;
				break;
			}
		}
		__SetType s1 = s;
		TAST(s1 == s && s1 >= s && s1 <= s);
		if(s1 != s || s1 > s || s1 < s)
			TERROR;
		itfront = s1.frontit();
		citfront = s1.const_frontit();
		itback = s1.backit();
		citback = s1.const_backit();
		if(itfront >= itback || itfront > itback || itfront == itback ||
			citfront >= citback || citfront > citback || citfront == citback)
		{
			TERROR;
		}
		if(!(itfront <= itback && itfront < itback && itfront != itback &&
			citfront <= citback && citfront < citback && citfront != citback))
		{
			TERROR;
		}
		if((itfront + 10) - 10 != s1.frontit() ||
			(citfront + 10) - 10 != s1.const_frontit())
		{
			TERROR;
		}
		if(itfront + 10 - s1.frontit() != 10 ||
			citfront + 10 - s1.const_frontit() != 10)
		{
			TERROR;
		}
		itfront += ELEMENT_SIZE - 1;
		itback -= ELEMENT_SIZE - 1;
		citfront += ELEMENT_SIZE - 1;
		citback -= ELEMENT_SIZE - 1;
		if(itfront != s1.backit() ||
			itback != s1.frontit() ||
			citfront != s1.const_backit() ||
			citback != s1.const_frontit())
		{
			TERROR;
		}

		s.destroy();
		for(__SetType::_datatype x = 0; x < 3; ++x)
			s.insert(x);
		__SetType::it it = s.frontit();
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
		TAST(s.empty());

		for(__SetType::_datatype x = 0; x < 3; ++x)
			s.insert(x);
		GAIA::ITERATOR::InverseIterator<__SetType::_datatype, __SetType::it> iit = s.backit();
		for(__SetType::_sizetype x = 0; x < s.size(); ++x)
		{
			if(*iit != (__SetType::_datatype)(s.size() - x - 1))
			{
				TERROR;
				break;
			}
			++iit;
		}
		iit = s.backit();
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
		TAST(s.empty());

		s.destroy();
		for(__SetType::_datatype x = 0; x < 10; x += 2)
			s.insert(x);
		if(s.lower_equal(0) != s.upper_equal(0))
			TERROR;
		if(s.lower_equal(2) != s.upper_equal(2))
			TERROR;
		if(s.lower_equal(8) != s.upper_equal(8))
			TERROR;
		if(s.lower_equal(10) == s.upper_equal(10))
			TERROR;
		if(s.lower_equal(-1) == s.upper_equal(-1))
			TERROR;
		__SetType::it it_l = s.lower_equal(5);
		__SetType::it it_u = s.upper_equal(5);
		if(it_l.empty() || *it_l != 4)
			TERROR;
		if(it_u.empty() || *it_u != 6)
			TERROR;
		s.destroy();
	}
}
