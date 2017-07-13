#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class Node : public GAIA::Base
	{
	public:
		Node(){}
		GINL GAIA::GVOID SetValue(GAIA::NUM s){m_s = s;}
		GINL GAIA::NUM GetValue() const{return m_s;} 
	private:
		GAIA::NUM m_s;
	};
	
	GINL GAIA::N32 NodeCmpor(const GAIA::GVOID* t, const GAIA::GVOID* k)
	{
		Node* pT = (Node*)t;
		GAIA::NUM* pK = (GAIA::NUM*)k;
		if(pT->GetValue() < *pK)
			return -1;
		else if(pT->GetValue() > *pK)
			return +1;
		return 0;
	}
	
	extern GAIA::GVOID t_algo_search(GAIA::LOG::Log& logobj)
	{
		GAIA::NUM list[10];
		for(GAIA::NUM x = 0; x < sizeofarray(list); ++x)
			list[x] = x * 2;
		for(GAIA::NUM x = 0; x < sizeofarray(list); ++x)
		{
			GAIA::NUM x2 = x * 2;
			GAIA::NUM* pFinded = GAIA::ALGO::find(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::glinear_search(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::gbinary_search(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::gbinary_search(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::upper_equal(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			pFinded = GAIA::ALGO::lower_equal(list, list + sizeofarray(list) - 1, x2);
			if(pFinded == GNIL || *pFinded != x2)
			{
				TERROR;
				break;
			}

			GAIA::NUM count_result = 0;
			GAIA::ALGO::count(list, list + sizeofarray(list) - 1, x2, count_result);
			if(count_result != 1)
			{
				TERROR;
				break;
			}
			count_result = 0;
			GAIA::ALGO::count_order(list, list + sizeofarray(list) - 1, x2, count_result);
			if(count_result != 1)
			{
				TERROR;
				break;
			}
			count_result = 0;
			GAIA::ALGO::counts(list, list + sizeofarray(list) - 1, list, list + sizeofarray(list) - 1, count_result);
			if(count_result != sizeofarray(list))
			{
				TERROR;
				break;
			}
			count_result = 0;
			GAIA::ALGO::counts_order(list, list + sizeofarray(list) - 1, list, list + sizeofarray(list) - 1, count_result);
			if(count_result != sizeofarray(list))
			{
				TERROR;
				break;
			}
		}
		
		static const GAIA::NUM SAMPLE_COUNT = 100;
		GAIA::CTN::Vector<Node> listNode;
		GAIA::CTN::Array<Node, SAMPLE_COUNT> arrNode;
		GAIA::CTN::ArrayVector<Node, SAMPLE_COUNT> arrvecNode;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			Node n;
			n.SetValue(x);
			listNode.push_back(n);
			arrNode.push_back(n);
			arrvecNode.push_back(n);
		}
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			const Node* pFinded = GAIA::ALGO::gbinary_search(listNode.fptr(), listNode.bptr(), x, NodeCmpor);
			if(pFinded == GNIL)
				TERROR;
			if(pFinded->GetValue() != x)
				TERROR;
			pFinded = GAIA::ALGO::glinear_search(listNode.fptr(), listNode.bptr(), x, NodeCmpor);
			if(pFinded == GNIL)
				TERROR;
			if(pFinded->GetValue() != x)
				TERROR;
			pFinded = GAIA::ALGO::lower_equal(listNode.fptr(), listNode.bptr(), x, NodeCmpor);
			if(pFinded == GNIL)
				TERROR;
			if(pFinded->GetValue() != x)
				TERROR;
			pFinded = GAIA::ALGO::upper_equal(listNode.fptr(), listNode.bptr(), x, NodeCmpor);
			if(pFinded == GNIL)
				TERROR;
			if(pFinded->GetValue() != x)
				TERROR;
			
			if(listNode.binary_search(x, NodeCmpor) != x)
				TERROR;
			if(arrNode.binary_search(x, NodeCmpor) != x)
				TERROR;
			if(arrvecNode.binary_search(x, NodeCmpor) != x)
				TERROR;
			
			if(listNode.binary_searchit(x, NodeCmpor) - listNode.frontit() != x)
				TERROR;
			if(arrNode.binary_searchit(x, NodeCmpor) - arrNode.frontit() != x)
				TERROR;
			if(arrvecNode.binary_searchit(x, NodeCmpor) - arrvecNode.frontit() != x)
				TERROR;
			
			if(listNode.const_binary_searchit(x, NodeCmpor) - listNode.const_frontit() != x)
				TERROR;
			if(arrNode.const_binary_searchit(x, NodeCmpor) - arrNode.const_frontit() != x)
				TERROR;
			if(arrvecNode.const_binary_searchit(x, NodeCmpor) - arrvecNode.const_frontit() != x)
				TERROR;
		}
	}
}
