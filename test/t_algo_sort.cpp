#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class BaseBase : public virtual GAIA::Base
	{
	public:
		virtual GAIA::N32 func(){return m_nbase;}
	public:
		GAIA::N32 m_nbase;
	};
	class Base1 : public virtual BaseBase
	{
	public:
		virtual GAIA::N32 func() const{return m_n1;}
	public:
		GAIA::N32 m_n1;
	};
	class Base2 : public virtual BaseBase
	{
	public:
		virtual GAIA::N32 func() const{return m_n2;}
	public:
		GAIA::N32 m_n2;
	};
	class Node : public virtual Base1, public virtual Base2
	{
	public:
		virtual GAIA::N32 func() const{return m_v;}
	public:
		GAIA::NUM m_v;
	};
	GINL GAIA::N32 CmporForNode(const GAIA::GVOID* p1, const GAIA::GVOID* p2)
	{
		const Node& t1 = *(Node*)p1;
		const Node& t2 = *(Node*)p2;
		if(t1.m_v < t2.m_v)
			return -1;
		else if(t1.m_v > t2.m_v)
			return +1;
		return 0;
	}
	extern GAIA::GVOID t_algo_sort(GAIA::LOG::Log& logobj)
	{
		// Without comparator test.
		{
			// Generate test data.
			GAIA::MATH::RandomLCG lcg;
			GAIA::CTN::Vector<GAIA::N64> listData;
			listData.resize(1000);
			for(GAIA::NUM x = 0; x < listData.size(); ++x)
				listData[x] = lcg.random_u16() % 0x8000 - 2000;
			GAIA::CTN::Vector<GAIA::N64> listDataSorted = listData;
			GAIA::ALGO::sort(listDataSorted.fptr(), listDataSorted.bptr());

			// Bucket sort.
			GAIA::CTN::Vector<GAIA::N64> listDataOrigin = listData;
			if(!GAIA::ALGO::ksort(listData.fptr(), listData.bptr()))
				TERROR;
			if(!GAIA::ALGO::issorted(listData.fptr(), listData.bptr()))
				TERROR;
			for(GAIA::NUM x = 0; x < listData.size(); ++x)
			{
				if(listData[x] != listDataSorted[x])
					TERROR;
			}

			// Quick sort.
			listData = listDataOrigin;
			GAIA::ALGO::qsort(listData.fptr(), listData.bptr());
			if(!GAIA::ALGO::issorted(listData.fptr(), listData.bptr()))
				TERROR;
			for(GAIA::NUM x = 0; x < listData.size(); ++x)
			{
				if(listData[x] != listDataSorted[x])
					TERROR;
			}
		}
		
		// With comparator test.
		{
			GAIA::CTN::Vector<Node> listNode;
			listNode.resize(1000);
			for(GAIA::NUM x = 0; x < listNode.size(); ++x)
				listNode[x].m_v = x;
			GAIA::MATH::RandomLCG lcg;
			for(GAIA::NUM x = 0; x < listNode.size(); ++x)
			{
				GAIA::NUM a = lcg.random() % listNode.size();
				GAIA::NUM b = lcg.random() % listNode.size();
				if(a != b)
					listNode.swap(a, b);
			}
			GAIA::ALGO::sort(listNode.fptr(), listNode.bptr(), CmporForNode);
			for(GAIA::NUM x = 0; x < listNode.size(); ++x)
			{
				if(listNode[x].m_v != x)
				{
					TERROR;
					break;
				}
			}
			if(!GAIA::ALGO::issorted(listNode.fptr(), listNode.bptr(), CmporForNode))
				TERROR;
		}
	}
}
