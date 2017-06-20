#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_arrayvector(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicArrayVector<GAIA::N32, GAIA::N32, GAIA::ALGO::ExtendGold<GAIA::N32>, 3> __ArrayVectorType;
		__ArrayVectorType av;
		av.reserve(100);
		if(av.capacity() != 100)
			TERROR;
		if(av.size() != 0)
			TERROR;
		TAST(av.empty());
		__ArrayVectorType::it newit;
		__ArrayVectorType::const_it newcit;
		newit = av.frontit();
		TAST(newit.empty());
		newit = av.backit();
		TAST(newit.empty());
		newcit = av.const_frontit();
		TAST(newcit.empty());
		newcit = av.const_backit();
		TAST(newcit.empty());
		for(__ArrayVectorType::_sizetype x = 0; x < av.capacity(); ++x)
			av.push_back(x);
		if(av.size() != av.capacity())
			TERROR;
		for(__ArrayVectorType::_sizetype x = 0; x < av.size(); ++x)
		{
			if(av[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(av.capacity() != 100)
			TERROR;
		av.clear();
		for(__ArrayVectorType::_sizetype x = 0; x < av.capacity(); ++x)
		{
			if(!av.insert(x, x))
			{
				TERROR;
				break;
			}
		}
		if(av.size() != av.capacity())
			TERROR;
		for(__ArrayVectorType::_sizetype x = 0; x < av.size(); ++x)
		{
			if(av[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(av.capacity() != 100)
			TERROR;
		__ArrayVectorType av1;
		av1.push_back(av.fptr(), av.size());
		if(av1.capacity() != 100)
			TERROR;
		if(av1.size() != av1.capacity())
			TERROR;
		for(__ArrayVectorType::_sizetype x = 0; x < av1.size(); ++x)
		{
			if(av1[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(__ArrayVectorType::_sizetype size = av.size(); size > 0; --size)
		{
			if(av.back() != size - 1)
			{
				TERROR;
				break;
			}
			if(!av.pop_back())
			{
				TERROR;
				break;
			}
		}

		__ArrayVectorType::_sizetype arrsize = av1.size();
		__ArrayVectorType::_datatype* pTemp = gnew __ArrayVectorType::_datatype[arrsize];
		TAST(av1.pop_back(pTemp, arrsize));
		av1.push_back(pTemp, arrsize);
		av.push_back(pTemp, arrsize);

		gdel[] pTemp;
		pTemp = GNIL;
		if(av1.size() != arrsize ||
			av.size() != arrsize)
		{
			TERROR;
		}
		for(__ArrayVectorType::_sizetype x = 0; x < av.size(); ++x)
		{
			if(av[x] != av1[x])
				TERROR;
		}
		if(av.front() != av1.front())
			TERROR;
		av.resize(av.size() / 2);
		if(av.size() != av1.size() / 2)
			TERROR;
		for(__ArrayVectorType::_sizetype x = 0; x < av.size(); ++x)
		{
			if(av.count(x) != 1)
			{
				TERROR;
				break;
			}
		}
		av.reset(0);
		if(av.size() != av1.size() / 2)
			TERROR;
		for(__ArrayVectorType::_sizetype x = 0; x < av.size(); ++x)
		{
			if(av[x] != 0)
			{
				TERROR;
				break;
			}
		}
		for(__ArrayVectorType::_sizetype x = 0; av.capacity() != av.size(); ++x)
			av.push_back(x);
		if(av.size() != av.capacity())
			TERROR;
		av.sort();
		TAST(GAIA::ALGO::issorted(av.fptr(), av.bptr()));
		if(av.unique() != av1.size() / 2)
			TERROR;
		av.resize(av1.size() / 2);
		for(__ArrayVectorType::_sizetype x = 0; av.capacity() != av.size(); ++x)
			av.push_back(x);
		if(av.size() != av.capacity())
			TERROR;
		if(av.unique_noorder() != av1.size() / 2)
			TERROR;
		av.sort();
		TAST(GAIA::ALGO::issorted(av.fptr(), av.bptr()));
		__ArrayVectorType::const_it cit = av.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			if(av.binary_search(*cit) == GINVALID)
			{
				TERROR;
				break;
			}
			__ArrayVectorType::it ittemp = av.binary_searchit(*cit);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
			__ArrayVectorType::const_it cittemp = av.const_binary_searchit(*cit);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
			if(av.find(*cit, 0) == GINVALID)
			{
				TERROR;
				break;
			}
			if(!av.exist(*cit))
			{
				TERROR;
				break;
			}
			if(av.rfind(*cit, av.size() - 1) == GINVALID)
			{
				TERROR;
				break;
			}
			ittemp = av.findit(*cit);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
			cittemp = av.const_findit(*cit);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
		}
		av.inverse();
		__ArrayVectorType::it it = av.frontit();
		for(; !it.empty(); ++it)
		{
			__ArrayVectorType::it itt = it;
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
		av1 = av;
		TAST(av.insert(997, 0));
		__ArrayVectorType::_sizetype insertindex1 = av.size() / 2;
		TAST(av.insert(998, insertindex1));
		__ArrayVectorType::_sizetype insertindex2 = av.size();
		TAST(av.insert(999, insertindex2));
		if(av[0] != 997)
			TERROR;
		if(av[insertindex1] != 998)
			TERROR;
		if(av[insertindex2] != 999)
			TERROR;
		TAST(av.erase(insertindex2));
		TAST(av.erase(insertindex1));
		TAST(av.erase(0));
		if(av > av1 || av < av1 || av != av1)
			TERROR;
		TAST(av >= av1 && av <= av1 && av == av1);
		it = av.frontit();
		for(; !it.empty(); ++it)
		{
			__ArrayVectorType::it itfinded = av.upper_equal(*it);
			if(itfinded.empty())
			{
				TERROR;
				break;
			}
			itfinded = av.lower_equal(*it);
			if(itfinded.empty())
			{
				TERROR;
				break;
			}
		}
		const __ArrayVectorType& arrc = av;
		cit = arrc.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			__ArrayVectorType::const_it citfinded = arrc.upper_equal(*cit);
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
		av.inverse();
		cit = av.const_frontit();
		it = av1.frontit();
		for(; !cit.empty(); ++cit, ++it)
			*it = *cit;
		if(av != av1)
			TERROR;
		__ArrayVectorType av2;
		av2 = av1;
		av2 += av1;
		av += av1;
		if(av != av2)
			TERROR;
		av = av1;
		cit = av.const_frontit();
		it = av.frontit();
		if((cit + 10) - cit != 10)
			TERROR;
		if((it + 10) - it != 10)
			TERROR;
		cit += 10;
		it += 10;
		if(cit - av.const_frontit() != 10)
			TERROR;
		if(it - av.frontit() != 10)
			TERROR;
		cit -= 20;
		it -= 20;
		TAST(cit.empty());
		TAST(it.empty());
		av.clear();
		if(av == av1)
			TERROR;
		av.destroy();
		if(av == av1)
			TERROR;
		av1.clear();
		if(av != av1)
			TERROR;
		av1.destroy();
		if(av != av1)
			TERROR;

		av.destroy();
		for(GAIA::NUM x = 0; x < 3; ++x)
			av.push_back(x);
		it = av.frontit();
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
		TAST(av.empty());

		for(GAIA::NUM x = 0; x < 3; ++x)
			av.push_back(x);
		GAIA::ITERATOR::InverseIterator<__ArrayVectorType::_datatype, __ArrayVectorType::it> iit = av.backit();
		for(GAIA::NUM x = 0; x < av.size(); ++x)
		{
			if(*iit != av.size() - x - 1)
			{
				TERROR;
				break;
			}
			++iit;
		}
		iit = av.backit();
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
		TAST(av.empty());

		av.destroy();
		av.push_back(0);
		av.push_back(1);
		av.push_back(2);
		av.push_back(0);
		av.push_back(1);
		av.push_back(2);
		av.push_back(0);
		if(av.remove(0) != 0)
			TERROR;
		if(av.size() != 6)
			TERROR;
		if(av.removeall(0) != 2)
			TERROR;
		if(av.size() != 4)
			TERROR;
		if(av.removeall(2) != 2)
			TERROR;
		if(av.remove(2) != GINVALID)
			TERROR;
		if(av.remove(1, 2) != GINVALID)
			TERROR;
		if(av.remove(0) != GINVALID)
			TERROR;
		if(av.remove(1, 1) != 1)
			TERROR;
		if(av.remove(1) != 0)
			TERROR;
		if(av.remove(1) != GINVALID)
			TERROR;
		if(av.size() != 0)
			TERROR;

		av.destroy();
		for(__ArrayVectorType::_datatype x = 0; x < 10; ++x)
			av.push_back(x);
		if(!av.erase(0, 2)) // Result is : 2 3 4 5 6 7 8 9
			TERROR;
		if(av.size() != 8)
			TERROR;
		if(!av.erase(6, 2)) // Result is : 2 3 4 5 6 7
			TERROR;
		if(av.size() != 6)
			TERROR;
		if(!av.erase(1, 3)) // Result is : 2 6 7
			TERROR;
		if(av.size() != 3)
			TERROR;
		if(av[0] != 2 || av[1] != 6 || av[2] != 7)
			TERROR;
		if(!av.erase(0, 3)) // Result is null.
			TERROR;
		if(!av.empty())
			TERROR;

		av.destroy();
		for(__ArrayVectorType::_datatype x = 0; x < 10; ++x)
			av.push_back(x);
		TAST(av.keep(5));
		TAST(av.size() == 5);
		for(__ArrayVectorType::_datatype x = 0; x < 5; ++x)
		{
			if(av[x] != x + 5)
			{
				TERROR;
				break;
			}
		}

		av.clear();
		for(__ArrayVectorType::_datatype x = 0; x < 10; ++x)
			av.push_back(x);
		av1.resize(100);
		TAST(av.extract_left(5, av1.fptr(), 100) == 5);
		TAST(av1[0] == 0 && av1[1] == 1 && av1[2] == 2 && av1[3] == 3 && av1[4] == 4);
		TAST(av.extract_right(5, av1.fptr(), 100) == 4);
		TAST(av1[0] == 6 && av1[1] == 7 && av1[2] == 8 && av1[3] == 9);
		TAST(av.extract_mid(3, 7, av1.fptr(), 100) == 5);
		TAST(av1[0] == 3 && av1[1] == 4 && av1[2] == 5 && av1[3] == 6 && av1[4] == 7);
		TAST(av.extract(3, 7, av1.fptr(), 100) == 5);
		TAST(av1[0] == 3 && av1[1] == 4 && av1[2] == 5 && av1[3] == 6 && av1[4] == 7);
		av1.clear();
		TAST(av.extract_left(5, av1) == 5);
		TAST(av1.size() == 5);
		TAST(av1[0] == 0 && av1[1] == 1 && av1[2] == 2 && av1[3] == 3 && av1[4] == 4);
		TAST(av.extract_right(5, av1) == 4);
		TAST(av1.size() == 9);
		TAST(av1[5] == 6 && av1[6] == 7 && av1[7] == 8 && av1[8] == 9);
		TAST(av.extract_mid(3, 7, av1) == 5);
		TAST(av1.size() == 14);
		TAST(av1[9] == 3 && av1[10] == 4 && av1[11] == 5 && av1[12] == 6 && av1[13] == 7);
		TAST(av.extract(3, 7, av1) == 5);
		TAST(av1.size() == 19);
		TAST(av1[14] == 3 && av1[15] == 4 && av1[16] == 5 && av1[17] == 6 && av1[18] == 7);
	}
}
