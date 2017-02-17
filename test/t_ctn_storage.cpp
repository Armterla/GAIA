#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_storage(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicStorage<GAIA::NM, GAIA::ALGO::ExtendGold<GAIA::NM>, 32> __StorageType;
		__StorageType st;
		if(st.capacity() != 0)
			TERROR;
		if(st.size() != 0)
			TERROR;
		TAST(st.empty());
		st.clear();
		st.destroy();
		if(st.capacity() != 0)
			TERROR;
		if(st.size() != 0)
			TERROR;
		TAST(st.empty());
		st.increase_reserve(10000, 1024 * 1024 * __StorageType::_pagesize);
		TAST(st.empty());
		if(st.capacity() == 0)
			TERROR;
		if(st.size() != 0)
			TERROR;
		__StorageType::Node n;
		TAST(st.insert(40, n));
		if(st.capacity() != 1024 * 1024 * __StorageType::_pagesize)
			TERROR;
		if(st.size() != 64)
			TERROR;
		TAST(st.erase(n.head()));
		if(st.capacity() != 1024 * 1024 * __StorageType::_pagesize)
			TERROR;
		if(st.size() != 0)
			TERROR;
		st.destroy();
		TAST(st.empty());
		if(st.capacity() != 0)
			TERROR;
		if(st.size() != 0)
			TERROR;
		st.increase_reserve(10000, 1024 * 1024 * __StorageType::_pagesize);
		typedef GAIA::CTN::Vector<GAIA::NM> __VectorType;
		__VectorType vt;
		static const GAIA::N32 SAMPLE_COUNT = 100;
		for(GAIA::N32 x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::N32 nSize = GAIA::MATH::xrandom() % 256 + 1;
			if(!st.insert(nSize, n))
			{
				TERROR;
				break;
			}
			vt.push_back(n.head());
		}
		__StorageType st1 = st;
		for(__VectorType::_sizetype x = 0; x < vt.size(); x += 2)
		{
			if(st.find(vt[x]) == GNIL)
			{
				TERROR;
				break;
			}
		}
		for(__VectorType::_sizetype x = 0; x < vt.size(); x += 2)
		{
			if(!st.erase(vt[x]))
			{
				TERROR;
				break;
			}
		}
		for(__VectorType::_sizetype x = 1; x < vt.size(); x += 2)
		{
			if(!st.erase(vt[x]))
			{
				TERROR;
				break;
			}
		}
		TAST(st.empty());
		for(__VectorType::_sizetype x = 0; x < vt.size(); x += 2)
		{
			if(st1.find(vt[x]) == GNIL)
			{
				TERROR;
				break;
			}
		}
		for(__VectorType::_sizetype x = 0; x < vt.size(); x += 2)
		{
			if(!st1.erase(vt[x]))
			{
				TERROR;
				break;
			}
		}
		for(__VectorType::_sizetype x = 1; x < vt.size(); x += 2)
		{
			if(!st1.erase(vt[x]))
			{
				TERROR;
				break;
			}
		}
		TAST(st1.empty());
	}
}
