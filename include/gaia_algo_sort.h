#ifndef		__GAIA_ALGO_SORT_H__
#define		__GAIA_ALGO_SORT_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_base.h"

#ifdef GAIA_USESTL
#	include <algorithm>
#endif

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType> GAIA::GVOID bsort(_DataType pBegin, _DataType pEnd) // Bubble sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			while(pBegin < pEnd)
			{
				_DataType pTemp = pBegin + 1;
				while(pTemp <= pEnd)
				{
					if(*pTemp < *pBegin)
						GAIA::ALGO::swap(*pBegin, *pTemp);
					++pTemp;
				}
				++pBegin;
			}
		}
		template<typename _DataType> GAIA::GVOID isort(_DataType pBegin, _DataType pEnd) // Insertion sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
		}
		template<typename _DataType> GAIA::GVOID ssort(_DataType pBegin, _DataType pEnd) // Selection sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
		}
		template<typename _DataType> GAIA::GVOID hsort(_DataType pBegin, _DataType pEnd) // Heap sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
		}
		template<typename _DataType> GAIA::GVOID esort(_DataType pBegin, _DataType pEnd) // External sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
		}
		template<typename _DataType> GAIA::GVOID qsort(_DataType pBegin, _DataType pEnd) // Quick sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(pBegin >= pEnd)
				return;
		#ifdef GAIA_USESTL
			std::sort(pBegin, pEnd + 1);
		#else
			// quick sort optimize.
			if(pEnd - pBegin < 16)
				return GAIA::ALGO::bsort(pBegin, pEnd);
			GAIA::ALGO::swap(*pEnd, *(pBegin + (pEnd - pBegin) / 2));

			/* Standard quick sort algorithm. */
			_DataType pCursor = pBegin;
			_DataType pSplit = pBegin;
			while(pCursor < pEnd)
			{
				if(*pCursor < *pEnd) // pEnd is the key.
				{
					GAIA::ALGO::swap(*pCursor, *pSplit);
					++pSplit;
				}
				++pCursor;
			}
			GAIA::ALGO::swap(*pSplit, *pEnd);
			GAIA::ALGO::qsort(pBegin, pSplit - 1);
			GAIA::ALGO::qsort(pSplit + 1, pEnd);
		#endif
		}
		template<typename _DataType> GAIA::GVOID sort(_DataType pBegin, _DataType pEnd) // Sort wrapper.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(pEnd - pBegin < 10)
				GAIA::ALGO::bsort(pBegin, pEnd);
			else
				GAIA::ALGO::qsort(pBegin, pEnd);
		}
		template<typename _DataType> GAIA::BL issorted(_DataType pBegin, _DataType pEnd)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
			while(pBegin != pEnd)
			{
				if(*pBegin > *(pBegin + 1))
					return GAIA::False;
				++pBegin;
			}
			return GAIA::True;
		}
		template<typename _DataType> GAIA::GVOID bsort(_DataType pBegin, _DataType pEnd, GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2)) // Bubble sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			while(pBegin < pEnd)
			{
				_DataType pTemp = pBegin + 1;
				while(pTemp <= pEnd)
				{
					if(cmpor(pTemp, pBegin) < 0)
						GAIA::ALGO::swap(*pBegin, *pTemp);
					++pTemp;
				}
				++pBegin;
			}
		}
		template<typename _DataType> GAIA::GVOID isort(_DataType pBegin, _DataType pEnd, GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2)) // Insertion sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
		}
		template<typename _DataType> GAIA::GVOID ssort(_DataType pBegin, _DataType pEnd, GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2)) // Selection sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
		}
		template<typename _DataType> GAIA::GVOID hsort(_DataType pBegin, _DataType pEnd, GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2)) // Heap sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
		}
		template<typename _DataType> GAIA::GVOID esort(_DataType pBegin, _DataType pEnd, GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2)) // External sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
		}
		template<typename _DataType> class ComparatorForSTL : public std::binary_function<_DataType, _DataType, GAIA::BL>
		{
		public:
			GINL ComparatorForSTL(GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2))
			{
				this->cmpor = cmpor;
			}
			GINL GAIA::BL operator()(const _DataType& a, const _DataType& b)  
			{
				return cmpor(&a, &b) < 0;
			}
		private:
			GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2);
		};
		template<typename _DataType> GAIA::GVOID qsort(_DataType pBegin, _DataType pEnd, GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2)) // Quick sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(pBegin >= pEnd)
				return;
		#ifdef GAIA_USESTL
			ComparatorForSTL<decltype(*pBegin)> stlcmpor(cmpor);
			std::sort(pBegin, pEnd + 1, stlcmpor);
		#else
			// quick sort optimize.
			if(pEnd - pBegin < 16)
				return GAIA::ALGO::bsort(pBegin, pEnd, cmpor);
			GAIA::ALGO::swap(*pEnd, *(pBegin + (pEnd - pBegin) / 2));

			/* Standard quick sort algorithm. */
			_DataType pCursor = pBegin;
			_DataType pSplit = pBegin;
			while(pCursor < pEnd)
			{
				if(cmpor(pCursor, *pEnd) < 0) // pEnd is the key.
				{
					GAIA::ALGO::swap(*pCursor, *pSplit);
					++pSplit;
				}
				++pCursor;
			}
			GAIA::ALGO::swap(*pSplit, *pEnd);
			GAIA::ALGO::qsort(pBegin, pSplit - 1, cmpor);
			GAIA::ALGO::qsort(pSplit + 1, pEnd, cmpor);
		#endif
		}
		template<typename _DataType> GAIA::GVOID sort(_DataType pBegin, _DataType pEnd, GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2)) // Sort wrapper.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(pEnd - pBegin < 10)
				GAIA::ALGO::bsort(pBegin, pEnd, cmpor);
			else
				GAIA::ALGO::qsort(pBegin, pEnd, cmpor);
		}
		template<typename _DataType> GAIA::BL issorted(_DataType pBegin, _DataType pEnd, GAIA::N32 (*cmpor)(const GAIA::GVOID* t1, const GAIA::GVOID* t2))
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
			while(pBegin != pEnd)
			{
				if(cmpor(pBegin, pBegin + 1) > 0)
					return GAIA::False;
				++pBegin;
			}
			return GAIA::True;
		}
		template<typename _DataType> GAIA::BL ksort(_DataType pBegin, _DataType pEnd) // Bucket sort.
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(!isintegertype(*pBegin))
				return GAIA::False;
			static const GAIA::NUM ELEMENT_COUNT = 0x8000;
			GAIA::N64 nMax = GAIA::NMMIN;
			GAIA::N64 nMin = GAIA::NMMAX;
			_DataType pTemp = pBegin;
			while(pTemp <= pEnd)
			{
				if(*pTemp > nMax)
					nMax = *pTemp;
				if(*pTemp < nMin)
					nMin = *pTemp;
				++pTemp;
			}
			if(nMax < nMin)
				return GAIA::False;
			if(nMax - nMin >= ELEMENT_COUNT)
				return GAIA::False;
			GAIA::NUM lists[ELEMENT_COUNT];
			for(GAIA::NUM x = 0; x < ELEMENT_COUNT; ++x)
				lists[x] = 0;
			pTemp = pBegin;
			while(pTemp <= pEnd)
			{
				++lists[*pTemp - nMin];
				++pTemp;
			}
			pTemp = pBegin;
			for(GAIA::NUM x = 0; x < ELEMENT_COUNT; ++x)
				for(GAIA::NUM y = 0; y < lists[x]; ++y)
					*pTemp++ = x + nMin;
			return GAIA::True;
		}
	}
}

#endif
