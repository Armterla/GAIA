#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_array(GAIA::LOG::Log& logobj)
	{
		static const GAIA::N32 ELEMENT_COUNT = 100;
		typedef GAIA::CTN::BasicArray<GAIA::N32, GAIA::N32, ELEMENT_COUNT> __ArrayType;
		__ArrayType arr;
		if(arr.capacity() != ELEMENT_COUNT)
			TERROR;
		if(arr.size() != 0)
			TERROR;
		TAST(arr.empty());
		__ArrayType::it newit;
		__ArrayType::const_it newcit;
		newit = arr.frontit();
		TAST(newit.empty());
		newit = arr.backit();
		TAST(newit.empty());
		newcit = arr.const_frontit();
		TAST(newcit.empty());
		newcit = arr.const_backit();
		TAST(newcit.empty());
		for(__ArrayType::_sizetype x = 0; x < arr.capacity(); ++x)
		{
			if(!arr.push_back(x))
			{
				TERROR;
				break;
			}
		}
		if(arr.size() != arr.capacity())
			TERROR;
		for(__ArrayType::_sizetype x = 0; x < arr.size(); ++x)
		{
			if(arr[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(arr.capacity() != ELEMENT_COUNT)
			TERROR;
		arr.clear();
		for(__ArrayType::_sizetype x = 0; x < arr.capacity(); ++x)
		{
			if(!arr.insert(x, x))
			{
				TERROR;
				break;
			}
		}
		if(arr.size() != arr.capacity())
			TERROR;
		for(__ArrayType::_sizetype x = 0; x < arr.size(); ++x)
		{
			if(arr[x] != x)
			{
				TERROR;
				break;
			}
		}
		if(arr.capacity() != ELEMENT_COUNT)
			TERROR;
		__ArrayType arr1;
		arr1.push_back(arr.fptr(), arr.size());
		if(arr1.capacity() != ELEMENT_COUNT)
			TERROR;
		if(arr1.size() != arr1.capacity())
			TERROR;
		for(__ArrayType::_sizetype x = 0; x < arr1.size(); ++x)
		{
			if(arr1[x] != x)
			{
				TERROR;
				break;
			}
		}
		for(__ArrayType::_sizetype size = arr.size(); size > 0; --size)
		{
			if(arr.back() != size - 1)
			{
				TERROR;
				break;
			}
			if(!arr.pop_back())
			{
				TERROR;
				break;
			}
		}

		__ArrayType::_sizetype arrsize = arr1.size();
		__ArrayType::_datatype* pTemp = gnew __ArrayType::_datatype[arrsize];
		TAST(arr1.pop_back(pTemp, arrsize));
		TAST(arr1.push_back(pTemp, arrsize));
		TAST(arr.push_back(pTemp, arrsize));
		gdel[] pTemp;
		pTemp = GNIL;
		if(arr1.size() != arrsize ||
			arr.size() != arrsize)
		{
			TERROR;
		}
		for(__ArrayType::_sizetype x = 0; x < arr.size(); ++x)
		{
			if(arr[x] != arr1[x])
				TERROR;
		}
		if(arr.front() != arr1.front())
			TERROR;
		TAST(arr.resize(arr.size() / 2));
		if(arr.size() != arr1.size() / 2)
			TERROR;
		for(__ArrayType::_sizetype x = 0; x < arr.size(); ++x)
		{
			if(arr.count(x) != 1)
			{
				TERROR;
				break;
			}
		}
		arr.reset(0);
		if(arr.size() != arr1.size() / 2)
			TERROR;
		for(__ArrayType::_sizetype x = 0; x < arr.size(); ++x)
		{
			if(arr[x] != 0)
			{
				TERROR;
				break;
			}
		}
		for(__ArrayType::_sizetype x = 0; arr.capacity() != arr.size(); ++x)
			arr.push_back(x);
		if(arr.size() != arr.capacity())
			TERROR;
		arr.sort();
		TAST(GAIA::ALGO::issorted(arr.fptr(), arr.bptr()));
		if(arr.unique() != arr1.size() / 2)
			TERROR;
		arr.resize(arr1.size() / 2);
		for(__ArrayType::_sizetype x = 0; arr.capacity() != arr.size(); ++x)
			arr.push_back(x);
		if(arr.size() != arr.capacity())
			TERROR;
		if(arr.unique_noorder() != arr1.size() / 2)
			TERROR;
		arr.sort();
		TAST(GAIA::ALGO::issorted(arr.fptr(), arr.bptr()));
		__ArrayType::const_it cit = arr.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			if(arr.binary_search(*cit) == GINVALID)
			{
				TERROR;
				break;
			}
			__ArrayType::it ittemp = arr.binary_searchit(*cit);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
			__ArrayType::const_it cittemp = arr.const_binary_searchit(*cit);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
			if(arr.find(*cit, 0) == GINVALID)
			{
				TERROR;
				break;
			}
			if(!arr.exist(*cit))
			{
				TERROR;
				break;
			}
			if(arr.rfind(*cit, arr.size() - 1) == GINVALID)
			{
				TERROR;
				break;
			}
			ittemp = arr.findit(*cit);
			if(ittemp.empty())
			{
				TERROR;
				break;
			}
			cittemp = arr.const_findit(*cit);
			if(cittemp.empty())
			{
				TERROR;
				break;
			}
		}
		arr.inverse();
		__ArrayType::it it = arr.frontit();
		for(; !it.empty(); ++it)
		{
			__ArrayType::it itt = it;
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
		arr1 = arr;
		TAST(arr.insert(997, 0));
		__ArrayType::_sizetype insertindex1 = arr.size() / 2;
		TAST(arr.insert(998, insertindex1));
		__ArrayType::_sizetype insertindex2 = arr.size();
		TAST(arr.insert(999, insertindex2));
		if(arr[0] != 997)
			TERROR;
		if(arr[insertindex1] != 998)
			TERROR;
		if(arr[insertindex2] != 999)
			TERROR;
		TAST(arr.erase(insertindex2));
		TAST(arr.erase(insertindex1));
		TAST(arr.erase(0));
		if(arr > arr1 || arr < arr1 || arr != arr1)
			TERROR;
		TAST(arr >= arr1 && arr <= arr1 && arr == arr1);
		arr.sort();
		it = arr.frontit();
		for(; !it.empty(); ++it)
		{
			__ArrayType::it itfinded = arr.upper_equal(*it);
			if(itfinded.empty())
			{
				TERROR;
				break;
			}
			itfinded = arr.lower_equal(*it);
			if(itfinded.empty())
			{
				TERROR;
				break;
			}
		}
		const __ArrayType& arrc = arr;
		cit = arrc.const_frontit();
		for(; !cit.empty(); ++cit)
		{
			__ArrayType::const_it citfinded = arrc.upper_equal(*cit);
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
		arr.inverse();
		cit = arr.const_frontit();
		it = arr1.frontit();
		for(; !cit.empty(); ++cit, ++it)
			*it = *cit;
		if(arr != arr1)
			TERROR;
		__ArrayType arr2;
		arr2 = arr1;
		arr2 += arr1;
		arr += arr;
		if(arr != arr2)
			TERROR;
		arr = arr1;
		cit = arr.const_frontit();
		it = arr.frontit();
		if((cit + 10) - cit != 10)
			TERROR;
		if((it + 10) - it != 10)
			TERROR;
		cit += 10;
		it += 10;
		if(cit - arr.const_frontit() != 10)
			TERROR;
		if(it - arr.frontit() != 10)
			TERROR;
		cit -= 20;
		it -= 20;
		TAST(cit.empty());
		TAST(it.empty());

		arr.clear();
		for(GAIA::NUM x = 0; x < 3; ++x)
			arr.push_back(x);
		it = arr.frontit();
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
		TAST(arr.empty());

		for(GAIA::NUM x = 0; x < 3; ++x)
			arr.push_back(x);
		GAIA::ITERATOR::InverseIterator<__ArrayType::_datatype, __ArrayType::it> iit = arr.backit();
		for(GAIA::NUM x = 0; x < arr.size(); ++x)
		{
			if(*iit != arr.size() - x - 1)
			{
				TERROR;
				break;
			}
			++iit;
		}
		iit = arr.backit();
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
		TAST(arr.empty());

		arr.clear();
		arr.push_back(0);
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(0);
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(0);
		if(arr.remove(0) != 0)
			TERROR;
		if(arr.size() != 6)
			TERROR;
		if(arr.removeall(0) != 2)
			TERROR;
		if(arr.size() != 4)
			TERROR;
		if(arr.removeall(2) != 2)
			TERROR;
		if(arr.remove(2) != GINVALID)
			TERROR;
		if(arr.remove(1, 2) != GINVALID)
			TERROR;
		if(arr.remove(0) != GINVALID)
			TERROR;
		if(arr.remove(1, 1) != 1)
			TERROR;
		if(arr.remove(1) != 0)
			TERROR;
		if(arr.remove(1) != GINVALID)
			TERROR;
		if(arr.size() != 0)
			TERROR;

		arr.clear();
		for(__ArrayType::_datatype x = 0; x < 10; ++x)
			arr.push_back(x);
		if(!arr.erase(0, 2)) // Result is : 2 3 4 5 6 7 8 9
			TERROR;
		if(arr.size() != 8)
			TERROR;
		if(!arr.erase(6, 2)) // Result is : 2 3 4 5 6 7
			TERROR;
		if(arr.size() != 6)
			TERROR;
		if(!arr.erase(1, 3)) // Result is : 2 6 7
			TERROR;
		if(arr.size() != 3)
			TERROR;
		if(arr[0] != 2 || arr[1] != 6 || arr[2] != 7)
			TERROR;
		if(!arr.erase(0, 3)) // Result is null.
			TERROR;
		if(!arr.empty())
			TERROR;

		arr.resize(10);
		arr.reset(-1);
		for(__ArrayType::_sizetype x = 0; x < 10; ++x)
			arr[x] = x;
		arr.resize_keep(20);
		for(__ArrayType::_sizetype x = 0; x < 10; ++x)
		{
			if(arr[x] != x)
			{
				TERROR;
				break;
			}
		}

		arr.clear();
		for(__ArrayType::_datatype x = 0; x < 10; ++x)
			arr.push_back(x);
		TAST(arr.keep(5));
		TAST(arr.size() == 5);
		for(__ArrayType::_datatype x = 0; x < 5; ++x)
		{
			if(arr[x] != x + 5)
			{
				TERROR;
				break;
			}
		}

		arr.clear();
		for(__ArrayType::_datatype x = 0; x < 10; ++x)
			arr.push_back(x);
		arr1.resize(100);
		TAST(arr.extract_left(5, arr1.fptr(), 100) == 5);
		TAST(arr1[0] == 0 && arr1[1] == 1 && arr1[2] == 2 && arr1[3] == 3 && arr1[4] == 4);
		TAST(arr.extract_right(5, arr1.fptr(), 100) == 4);
		TAST(arr1[0] == 6 && arr1[1] == 7 && arr1[2] == 8 && arr1[3] == 9);
		TAST(arr.extract_mid(3, 7, arr1.fptr(), 100) == 5);
		TAST(arr1[0] == 3 && arr1[1] == 4 && arr1[2] == 5 && arr1[3] == 6 && arr1[4] == 7);
		TAST(arr.extract(3, 7, arr1.fptr(), 100) == 5);
		TAST(arr1[0] == 3 && arr1[1] == 4 && arr1[2] == 5 && arr1[3] == 6 && arr1[4] == 7);
		arr1.clear();
		TAST(arr.extract_left(5, arr1) == 5);
		TAST(arr1.size() == 5);
		TAST(arr1[0] == 0 && arr1[1] == 1 && arr1[2] == 2 && arr1[3] == 3 && arr1[4] == 4);
		TAST(arr.extract_right(5, arr1) == 4);
		TAST(arr1.size() == 9);
		TAST(arr1[5] == 6 && arr1[6] == 7 && arr1[7] == 8 && arr1[8] == 9);
		TAST(arr.extract_mid(3, 7, arr1) == 5);
		TAST(arr1.size() == 14);
		TAST(arr1[9] == 3 && arr1[10] == 4 && arr1[11] == 5 && arr1[12] == 6 && arr1[13] == 7);
		TAST(arr.extract(3, 7, arr1) == 5);
		TAST(arr1.size() == 19);
		TAST(arr1[14] == 3 && arr1[15] == 4 && arr1[16] == 5 && arr1[17] == 6 && arr1[18] == 7);
	}
}
