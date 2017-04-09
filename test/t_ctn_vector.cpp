#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_vector(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicVector<GAIA::N32, GAIA::N32, GAIA::ALGO::ExtendGold<GAIA::N32> > __VectorType;
		__VectorType vec;
		vec.reserve(100);
		if(vec.capacity() != 100)
			TERROR;
		if(vec.size() != 0)
			TERROR;
		TAST(vec.empty());
		__VectorType::it newit;
		__VectorType::const_it newcit;
		newit = vec.frontit();
		TAST(newit.empty());
		newit = vec.backit();
		TAST(newit.empty());
		newcit = vec.const_frontit();
		TAST(newcit.empty());
		newcit = vec.const_backit();
		TAST(newcit.empty());
		for(__VectorType::_sizetype x = 0; x < vec.capacity(); ++x)
			vec.push_back(x);
		if(vec.size() != vec.capacity())
			TERROR;
		for(__VectorType::_sizetype x = 0; x < vec.size(); ++x)
		{
			if(vec[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(vec.capacity() != 100)
			TERROR;
		vec.clear();
		for(__VectorType::_sizetype x = 0; x < vec.capacity(); ++x)
		{
			if(!vec.insert(x, x))
			{
				TERROR;
				break;
			}
		}
		if(vec.size() != vec.capacity())
			TERROR;
		for(__VectorType::_sizetype x = 0; x < vec.size(); ++x)
		{
			if(vec[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(vec.capacity() != 100)
			TERROR;
		__VectorType vec1;
		vec1.push_back(vec.fptr(), vec.size());
		if(vec1.capacity() != 100)
			TERROR;
		if(vec1.size() != vec1.capacity())
			TERROR;
		for(__VectorType::_sizetype x = 0; x < vec1.size(); ++x)
		{
			if(vec1[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(__VectorType::_sizetype size = vec.size(); size > 0; --size)
		{
			if(vec.back() != size - 1)
			{
				TERROR;
				break;
			}
			if(!vec.pop_back())
			{
				TERROR;
				break;
			}
		}

		__VectorType::_sizetype arrsize = vec1.size();
		__VectorType::_datatype* pTemp = gnew __VectorType::_datatype[arrsize];
		TAST(vec1.pop_back(pTemp, arrsize));
		vec1.push_back(pTemp, arrsize);
		vec.push_back(pTemp, arrsize);

		gdel[] pTemp;
		pTemp = GNIL;
		if(vec1.size() != arrsize ||
			vec.size() != arrsize)
		{
			TERROR;
		}
		for(__VectorType::_sizetype x = 0; x < vec.size(); ++x)
		{
			if(vec[x] != vec1[x])
				TERROR;
		}
		if(vec.front() != vec1.front())
			TERROR;
		vec.resize(vec.size() / 2);
		if(vec.size() != vec1.size() / 2)
			TERROR;
		for(__VectorType::_sizetype x = 0; x < vec.size(); ++x)
		{
			if(vec.count(x) != 1)
			{
				TERROR;
				break;
			}
		}
		vec.reset(0);
		if(vec.size() != vec1.size() / 2)
			TERROR;
		for(__VectorType::_sizetype x = 0; x < vec.size(); ++x)
		{
			if(vec[x] != 0)
			{
				TERROR;
				break;
			}
		}
		for(__VectorType::_sizetype x = 0; vec.capacity() != vec.size(); ++x)
			vec.push_back(x);
		if(vec.size() != vec.capacity())
			TERROR;
		vec.sort();
		TAST(GAIA::ALGO::issorted(vec.fptr(), vec.bptr()));
		if(vec.unique() != vec1.size() / 2)
			TERROR;
		vec.resize(vec1.size() / 2);
		for(__VectorType::_sizetype x = 0; vec.capacity() != vec.size(); ++x)
			vec.push_back(x);
		if(vec.size() != vec.capacity())
			TERROR;
		if(vec.unique_noorder() != vec1.size() / 2)
			TERROR;
		vec.sort();
		TAST(GAIA::ALGO::issorted(vec.fptr(), vec.bptr()));
		__VectorType::const_it cit = vec.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			if(vec.binary_search(*cit) == GINVALID)
			{
				TERROR;
				break;
			}
			__VectorType::it ittemp = vec.binary_searchit(*cit);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
			__VectorType::const_it cittemp = vec.const_binary_searchit(*cit);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
			if(vec.find(*cit, 0) == GINVALID)
			{
				TERROR;
				break;
			}
			if(vec.rfind(*cit, vec.size() - 1) == GINVALID)
			{
				TERROR;
				break;
			}
			ittemp = vec.findit(*cit);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
			cittemp = vec.const_findit(*cit);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
		}
		vec.inverse();
		__VectorType::it it = vec.frontit();
		for(; !it.empty(); ++it)
		{
			__VectorType::it itt = it;
			++itt;
			if(!itt.empty())
			{
				if(*it <= *itt)
				{
					TERROR;
					break;
				}
			}
		}
		vec1 = vec;
		TAST(vec.insert(997, 0));
		__VectorType::_sizetype insertindex1 = vec.size() / 2;
		TAST(vec.insert(998, insertindex1));
		__VectorType::_sizetype insertindex2 = vec.size();
		TAST(vec.insert(999, insertindex2));
		if(vec[0] != 997)
			TERROR;
		if(vec[insertindex1] != 998)
			TERROR;
		if(vec[insertindex2] != 999)
			TERROR;
		TAST(vec.erase(insertindex2));
		TAST(vec.erase(insertindex1));
		TAST(vec.erase(0));
		if(vec > vec1 || vec < vec1 || vec != vec1)
			TERROR;
		TAST(vec >= vec1 && vec <= vec1 && vec == vec1);
		vec.sort();
		it = vec.frontit();
		for(; !it.empty(); ++it)
		{
			__VectorType::it itfinded = vec.upper_equal(*it);
			if(itfinded.empty())
			{
				TERROR;
				break;
			}
			itfinded = vec.lower_equal(*it);
			if(itfinded.empty())
			{
				TERROR;
				break;
			}
		}
		const __VectorType& arrc = vec;
		cit = arrc.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			__VectorType::const_it citfinded = arrc.upper_equal(*cit);
			if(citfinded.empty())
			{
				TERROR;
				break;
			}
			citfinded = arrc.lower_equal(*cit);
			if(citfinded.empty())
			{
				TERROR;
				break;
			}
		}
		vec.inverse();
		cit = vec.const_frontit();
		it = vec1.frontit();
		for(; !cit.empty(); ++cit, ++it)
			*it = *cit;
		if(vec != vec1)
			TERROR;
		__VectorType vec2;
		vec2 = vec1;
		vec2 += vec1;
		vec += vec;
		if(vec != vec2)
			TERROR;
		vec = vec1;
		cit = vec.const_frontit();
		it = vec.frontit();
		if((cit + 10) - cit != 10)
			TERROR;
		if((it + 10) - it != 10)
			TERROR;
		cit += 10;
		it += 10;
		if(cit - vec.const_frontit() != 10)
			TERROR;
		if(it - vec.frontit() != 10)
			TERROR;
		cit -= 20;
		it -= 20;
		TAST(cit.empty());
		TAST(it.empty());
		vec.clear();
		if(vec == vec1)
			TERROR;
		vec.destroy();
		if(vec == vec1)
			TERROR;
		vec1.clear();
		if(vec != vec1)
			TERROR;
		vec1.destroy();
		if(vec != vec1)
			TERROR;

		vec.clear();
		for(GAIA::N32 x = 0; x < 100; ++x)
			vec.push_back(x);
		vec.resize_keep(200);
		if(vec.size() != 200)
			TERROR;
		if(vec.capacity() < 200)
			TERROR;
		for(GAIA::N32 x = 0; x < 100; ++x)
		{
			if(vec[x] != x)
			{
				TERROR;
				break;
			}
		}
		vec.resize_keep(50);
		if(vec.size() != 50)
			TERROR;
		if(vec.capacity() < 200)
			TERROR;
		for(GAIA::N32 x = 0; x < 50; ++x)
		{
			if(vec[x] != x)
			{
				TERROR;
				break;
			}
		}
		vec.destroy();
		vec.resize_keep(100);
		if(vec.size() != 100)
			TERROR;
		if(vec.capacity() < 100)
			TERROR;
		vec.resize_keep(0);
		if(vec.size() != 0)
			TERROR;
		if(vec.capacity() != 100)
			TERROR;

		vec.destroy();
		for(GAIA::N32 x = 0; x < 100; ++x)
			vec.push_back(x);
		vec.reserve_keep(200);
		if(vec.capacity() != 200)
			TERROR;
		if(vec.size() != 100)
			TERROR;
		for(GAIA::N32 x = 0; x < 100; ++x)
		{
			if(vec[x] != x)
			{
				TERROR;
				break;
			}
		}
		vec.reserve_keep(50);
		if(vec.capacity() != 50)
			TERROR;
		if(vec.size() != 50)
			TERROR;
		for(GAIA::N32 x = 0; x < 50; ++x)
		{
			if(vec[x] != x)
			{
				TERROR;
				break;
			}
		}
		vec.destroy();
		vec.reserve_keep(100);
		if(vec.capacity() != 100)
			TERROR;
		if(vec.size() != 0)
			TERROR;
		vec.reserve_keep(0);
		if(vec.capacity() != 0)
			TERROR;
		if(vec.size() != 0)
			TERROR;

		vec.destroy();
		for(GAIA::NUM x = 0; x < 3; ++x)
			vec.push_back(x);
		it = vec.frontit();
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
		TAST(vec.empty());

		for(GAIA::NUM x = 0; x < 3; ++x)
			vec.push_back(x);
		GAIA::ITERATOR::InverseIterator<__VectorType::_datatype, __VectorType::it> iit = vec.backit();
		for(GAIA::NUM x = 0; x < vec.size(); ++x)
		{
			if(*iit != vec.size() - x - 1)
			{
				TERROR;
				break;
			}
			++iit;
		}
		iit = vec.backit();
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
		TAST(vec.empty());

		vec.destroy();
		vec.push_back(0);
		vec.push_back(1);
		vec.push_back(2);
		vec.push_back(0);
		vec.push_back(1);
		vec.push_back(2);
		vec.push_back(0);
		if(vec.remove(0) != 0)
			TERROR;
		if(vec.size() != 6)
			TERROR;
		if(vec.removeall(0) != 2)
			TERROR;
		if(vec.size() != 4)
			TERROR;
		if(vec.removeall(2) != 2)
			TERROR;
		if(vec.remove(2) != GINVALID)
			TERROR;
		if(vec.remove(1, 2) != GINVALID)
			TERROR;
		if(vec.remove(0) != GINVALID)
			TERROR;
		if(vec.remove(1, 1) != 1)
			TERROR;
		if(vec.remove(1) != 0)
			TERROR;
		if(vec.remove(1) != GINVALID)
			TERROR;
		if(vec.size() != 0)
			TERROR;

		vec.destroy();
		for(__VectorType::_datatype x = 0; x < 10; ++x)
			vec.push_back(x);
		if(!vec.erase(0, 2)) // Result is : 2 3 4 5 6 7 8 9
			TERROR;
		if(vec.size() != 8)
			TERROR;
		if(!vec.erase(6, 2)) // Result is : 2 3 4 5 6 7
			TERROR;
		if(vec.size() != 6)
			TERROR;
		if(!vec.erase(1, 3)) // Result is : 2 6 7
			TERROR;
		if(vec.size() != 3)
			TERROR;
		if(vec[0] != 2 || vec[1] != 6 || vec[2] != 7)
			TERROR;
		if(!vec.erase(0, 3)) // Result is null.
			TERROR;
		if(!vec.empty())
			TERROR;

		vec.destroy();
		for(__VectorType::_datatype x = 0; x < 10; ++x)
			vec.push_back(x);
		TAST(vec.keep(5));
		TAST(vec.size() == 5);
		for(__VectorType::_datatype x = 0; x < 5; ++x)
		{
			if(vec[x] != x + 5)
			{
				TERROR;
				break;
			}
		}
	}
}
