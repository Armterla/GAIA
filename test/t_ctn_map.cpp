#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_map(GAIA::LOG::Log& logobj)
	{
		static const GAIA::N32 ELEMENT_SIZE = 100;
		typedef GAIA::CTN::BasicMap<GAIA::N32, GAIA::N32, GAIA::U32, GAIA::U32, GAIA::ALGO::ExtendGold<GAIA::N32> > __MapType;
		__MapType m;
		TAST(m.empty());
		if(m.size() != 0)
			TERROR;
		if(m.capacity() != 0)
			TERROR;
		__MapType::it newit;
		__MapType::const_it newcit;
		newit = m.frontit();
		TAST(newit.empty());
		newit = m.backit();
		TAST(newit.empty());
		newcit = m.const_frontit();
		TAST(newcit.empty());
		newcit = m.const_backit();
		TAST(newcit.empty());
		m.clear();
		m.destroy();
		for(__MapType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			__MapType::it itinsert;
			if(!m.insert(x, x, &itinsert) || *itinsert.key() != x || *itinsert != x)
			{
				TERROR;
				break;
			}
		}
		for(__MapType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			__MapType::it itinsert;
			if(m.insert(x, x, &itinsert) || *itinsert.key() != x || *itinsert != x)
			{
				TERROR;
				break;
			}
		}
		for(__MapType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(m.find(x) == GNIL)
			{
				TERROR;
				break;
			}
			if(GSCAST(const __MapType*)(&m)->find(x) == GNIL)
			{
				TERROR;
				break;
			}
			__MapType::it ittemp = m.findit(x);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
			__MapType::const_it cittemp = m.const_findit(x);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
			__MapType::it it_l = m.upper_equal(x);
			if(it_l.empty())
			{
				TERROR;
				break;
			}
			__MapType::const_it cit_l = GSCAST(const __MapType*)(&m)->upper_equal(x);
			if(cit_l.empty())
			{
				TERROR;
				break;
			}
			__MapType::it it_u = m.lower_equal(x);
			if(it_l.empty())
			{
				TERROR;
				break;
			}
			__MapType::const_it cit_u = GSCAST(const __MapType*)(&m)->lower_equal(x);
			if(cit_l.empty())
			{
				TERROR;
				break;
			}
		}
		for(__MapType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!m.erase(x))
			{
				TERROR;
				break;
			}
		}
		m.destroy();
		TAST(m.empty());
		if(m.size() != 0)
			TERROR;
		if(m.capacity() != 0)
			TERROR;
		for(__MapType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
			m.insert(x, x);
		__MapType::it itfront = m.frontit();
		__MapType::const_it citfront = m.const_frontit();
		__MapType::it itback = m.backit();
		__MapType::const_it citback = m.const_backit();
		for(__MapType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
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
				*itfront.key() != x || 
				*citfront.key() != x || 
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
		m.clear();
		TAST(m.empty());
		if(m.size() != 0)
			TERROR;
		if(m.capacity() == 0)
			TERROR;
		itfront = m.frontit();
		citfront = m.const_frontit();
		itback = m.backit();
		citback = m.const_backit();
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
		for(__MapType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
			m.insert(x, x);
		__MapType m1 = m;
		TAST(m1 == m && m1 >= m && m1 <= m);
		if(m1 != m || m1 > m || m1 < m)
			TERROR;
		itfront = m1.frontit();
		citfront = m1.const_frontit();
		itback = m1.backit();
		citback = m1.const_backit();
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
		if((itfront + 10) - 10 != m1.frontit() ||
			(citfront + 10) - 10 != m1.const_frontit())
		{
			TERROR;
		}
		if(itfront + 10 - m1.frontit() != 10 ||
			citfront + 10 - m1.const_frontit() != 10)
		{
			TERROR;
		}
		itfront += ELEMENT_SIZE - 1;
		itback -= ELEMENT_SIZE - 1;
		citfront += ELEMENT_SIZE - 1;
		citback -= ELEMENT_SIZE - 1;
		if(itfront != m1.backit() ||
			itback != m1.frontit() ||
			citfront != m1.const_backit() ||
			citback != m1.const_frontit())
		{
			TERROR;
		}

		m.destroy();
		for(__MapType::_datatype x = 0; x < 3; ++x)
			m.insert(x, x);
		__MapType::it it = m.frontit();
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
		TAST(m.empty());

		for(__MapType::_datatype x = 0; x < 3; ++x)
			m.insert((__MapType::_keytype)x, x);
		GAIA::ITERATOR::InverseIterator<__MapType::_datatype, __MapType::it> iit = m.backit();
		for(__MapType::_sizetype x = 0; x < m.size(); ++x)
		{
			if(*iit != (__MapType::_datatype)(m.size() - x - 1))
			{
				TERROR;
				break;
			}
			++iit;
		}
		iit = m.backit();
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
		TAST(m.empty());

		m.destroy();
		for(__MapType::_datatype x = 0; x < 10; x += 2)
			m.insert(x, x);
		if(m.lower_equal(0) != m.upper_equal(0))
			TERROR;
		if(m.lower_equal(2) != m.upper_equal(2))
			TERROR;
		if(m.lower_equal(8) != m.upper_equal(8))
			TERROR;
		if(m.lower_equal(10) == m.upper_equal(10))
			TERROR;
		if(m.lower_equal(-1) == m.upper_equal(-1))
			TERROR;
		__MapType::it it_l = m.lower_equal(5);
		__MapType::it it_u = m.upper_equal(5);
		if(it_l.empty() || *it_l != 4 || *it_l.key() != 4)
			TERROR;
		if(it_u.empty() || *it_u != 6 || *it_u.key() != 6)
			TERROR;
		m.destroy();
	}
}
