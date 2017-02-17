#ifndef		__GAIA_ALGO_UNIQUE_H__
#define		__GAIA_ALGO_UNIQUE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType> GAIA::BL isunique_order(_DataType pBegin, _DataType pEnd)
		{
			GAST(pBegin <= pEnd);
			while(pBegin < pEnd)
			{
				if(*pBegin == *(pBegin + 1))
					return GAIA::False;
				++pBegin;
			}
			return GAIA::True;
		}
		template<typename _DataType> GAIA::BL isunique_noorder(_DataType pBegin, _DataType pEnd)
		{
			GAST(pBegin <= pEnd);
			while(pBegin < pEnd)
			{
				_DataType pTemp = pBegin + 1;
				while(pTemp <= pEnd)
				{
					if(*pTemp == *pBegin)
						return GAIA::False;
					++pTemp;
				}
				++pBegin;
			}
			return GAIA::True;
		}
		template<typename _DataType> _DataType unique_order(_DataType pBegin, _DataType pEnd)
		{
			GAST(pBegin <= pEnd);
			_DataType pLast = pBegin;
			++pBegin;
			while(pBegin <= pEnd)
			{
				if(*pBegin != *pLast)
				{
					++pLast;
					if(pLast != pBegin) // OPT for large object copy.
						*pLast = *pBegin;
				}
				++pBegin;
			}
			return pLast;
		}
		template<typename _DataType> _DataType unique_noorder(_DataType pBegin, _DataType pEnd)
		{
			GAST(pBegin <= pEnd);
			_DataType pFirst = pBegin;
			_DataType pLast = pBegin;
			++pBegin;
			while(pBegin <= pEnd)
			{
				GAIA::BL bExist = GAIA::False;
				_DataType pTemp = pFirst;
				while(pTemp <= pLast)
				{
					if(*pTemp == *pBegin)
					{
						bExist = GAIA::True;
						break;
					}
					++pTemp;
				}
				if(!bExist)
				{
					++pLast;
					if(pLast != pBegin) // OPT for large object copy.
						*pLast = *pBegin;
				}
				++pBegin;
			}
			return pLast;
		}
	}
}

#endif
