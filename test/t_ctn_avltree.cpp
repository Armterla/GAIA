#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_avltree(GAIA::LOG::Log& logobj)
	{
		static const GAIA::N32 ELEMENT_SIZE = 100;
		typedef GAIA::CTN::BasicAVLTree<GAIA::N32, GAIA::U32, GAIA::U32, GAIA::ALGO::ExtendGold<GAIA::U32> > __AVLType;
		__AVLType avl;
		TAST(avl.empty());
		if(avl.size() != 0)
			TERROR;
		if(avl.capacity() != 0)
			TERROR;
		avl.clear();
		avl.destroy();
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			__AVLType::it itinsert;
			if(!avl.insert(x, &itinsert) || *itinsert != x)
			{
				TERROR;
				break;
			}
		}
		if(avl.empty())
			TERROR;
		if(avl.size() != ELEMENT_SIZE)
			TERROR;
		if(avl.capacity() < ELEMENT_SIZE)
			TERROR;
		TAST(GAIA::ALGO::issorted(avl.frontit(), avl.backit()));
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(avl.insert(x))
			{
				TERROR;
				break;
			}
		}
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(avl.find(x) == GNIL)
			{
				TERROR;
				break;
			}
			if(!avl.exist(x))
			{
				TERROR;
				break;
			}
			__AVLType::it ittemp = avl.findit(x);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
		}
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if((*(const __AVLType*)&avl).find(x) == GNIL)
			{
				TERROR;
				break;
			}
			__AVLType::const_it cittemp = avl.const_findit(x);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
		}
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!avl.erase(x))
			{
				TERROR;
				break;
			}
		}
		TAST(avl.empty());
		if(avl.size() != 0)
			TERROR;
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!avl.insert(x))
			{
				TERROR;
				break;
			}
		}
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(x % 10 == 1)
			{
				if(!avl.erase(x))
				{
					TERROR;
					break;
				}
			}
		}
		if(avl.empty())
			TERROR;
		if(avl.size() != ELEMENT_SIZE - ELEMENT_SIZE / 10)
			TERROR;
		__AVLType::_datatype* pMin = avl.minimize();
		if(pMin == GNIL)
			TERROR;
		if(*pMin != 0)
			TERROR;
		__AVLType::_datatype* pMax = avl.maximize();
		if(pMax == GNIL)
			TERROR;
		if(*pMax != ELEMENT_SIZE - 1)
			TERROR;
		avl.destroy();
		pMin = avl.minimize();
		if(pMin != GNIL)
			TERROR;
		if(pMin != GNIL)
			TERROR;
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!avl.insert(x))
			{
				TERROR;
				break;
			}
		}
		__AVLType::it frontit = avl.frontit();
		__AVLType::const_it cfrontit = avl.const_frontit();
		__AVLType::it backit = avl.backit();
		__AVLType::const_it cbackit = avl.const_backit();
		GAIA::N32 tdata = 0;
		for(; !frontit.empty(); ++frontit)
		{
			if(*frontit != tdata)
			{
				TERROR;
				break;
			}
			++tdata;
		}
		if(tdata != ELEMENT_SIZE)
			TERROR;
		tdata = 0;
		for(; !cfrontit.empty(); ++cfrontit)
		{
			if(*cfrontit != tdata)
			{
				TERROR;
				break;
			}
			++tdata;
		}
		if(tdata != ELEMENT_SIZE)
			TERROR;
		tdata = ELEMENT_SIZE - 1;
		for(; !backit.empty(); --backit)
		{
			if(tdata != *backit)
			{
				TERROR;
				break;
			}
			--tdata;
		}
		if(tdata != -1)
			TERROR;
		tdata = ELEMENT_SIZE - 1;
		for(; !cbackit.empty(); --cbackit)
		{
			if(tdata != *cbackit)
			{
				TERROR;
				break;
			}
			--tdata;
		}
		if(tdata != -1)
			TERROR;
		avl.clear();
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			if(!avl.insert(x))
			{
				TERROR;
				break;
			}
		}
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			__AVLType::it it_l = avl.upper_equal(x);
			__AVLType::const_it cit_l = (*(const __AVLType*)&avl).upper_equal(x);
			__AVLType::it it_u = avl.lower_equal(x);
			__AVLType::const_it cit_u = (*(const __AVLType*)&avl).lower_equal(x);
			if(*it_l != *cit_l ||
				*it_l != *it_u ||
				*it_l != *cit_u)
			{
				TERROR;
				break;
			}
		}
		__AVLType avl1 = avl;
		if(avl1.size() != avl.size())
			TERROR;
		if(avl1.empty() != avl.empty())
			TERROR;
		{
			__AVLType::it it = avl.frontit();
			__AVLType::const_it cit = avl.const_frontit();
			__AVLType::it it_b = avl.backit();
			__AVLType::const_it cit_b = avl.const_backit();
			TAST(it_b >= it && it_b > it && it_b != it);
			if(it_b <= it || it_b < it || it_b == it)
				TERROR;
			TAST(cit_b >= cit && cit_b > cit && cit_b != cit);
			if(cit_b <= cit || cit_b < cit || cit_b == cit)
				TERROR;
			if((it + 10) - avl.frontit() != 10)
				TERROR;
			if((cit + 10) - avl.const_frontit() != 10)
				TERROR;
			if(avl.backit() - (it_b - 10) != 10)
				TERROR;
			if(avl.const_backit() - (cit_b - 10) != 10)
				TERROR;
		}
		if(avl > avl1 || avl < avl1 || avl != avl1)
			TERROR;
		TAST(avl >= avl1 && avl <= avl1 && avl == avl1);

		avl.destroy();
		for(__AVLType::_datatype x = 0; x < 3; ++x)
			avl.insert(x);
		__AVLType::it it = avl.frontit();
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
		TAST(avl.empty());

		for(__AVLType::_datatype x = 0; x < 3; ++x)
			avl.insert(x);
		GAIA::ITERATOR::InverseIterator<__AVLType::_datatype, __AVLType::it> iit = avl.backit();
		for(__AVLType::_sizetype x = 0; x < avl.size(); ++x)
		{
			if(*iit != (__AVLType::_datatype)(avl.size() - x - 1))
			{
				TERROR;
				break;
			}
			++iit;
		}
		iit = avl.backit();
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
		TAST(avl.empty());

		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
			avl.insert(x);
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
		{
			frontit = avl.frontit();
			cfrontit = avl.const_frontit();
			if(*frontit != x)
			{
				TERROR;
				break;
			}
			if(*cfrontit != x)
			{
				TERROR;
				break;
			}
			if(!frontit.erase())
			{
				TERROR;
				break;
			}
		}

		TAST(avl.empty());
		for(__AVLType::_datatype x = 0; x < ELEMENT_SIZE; ++x)
			avl.insert(x);
		__AVLType::_datatype lastindex = -1;
		for(it = avl.frontit(); !it.empty(); )
		{
			if(*it != lastindex + 1)
			{
				TERROR;
				break;
			}
			lastindex = *it;
			it.erase();
		}

		avl.destroy();
		for(__AVLType::_datatype x = 0; x < 10; x += 2)
			avl.insert(x);
		if(avl.lower_equal(0) != avl.upper_equal(0))
			TERROR;
		if(avl.lower_equal(2) != avl.upper_equal(2))
			TERROR;
		if(avl.lower_equal(8) != avl.upper_equal(8))
			TERROR;
		if(avl.lower_equal(10) == avl.upper_equal(10))
			TERROR;
		if(avl.lower_equal(-1) == avl.upper_equal(-1))
			TERROR;
		__AVLType::it it_l = avl.lower_equal(5);
		__AVLType::it it_u = avl.upper_equal(5);
		if(it_l.empty() || *it_l != 4)
			TERROR;
		if(it_u.empty() || *it_u != 6)
			TERROR;
		avl.destroy();
	}
}
