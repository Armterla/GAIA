#ifndef		__GAIA_ALGO_SEARCH_H__
#define		__GAIA_ALGO_SEARCH_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType1, typename _DataType2>
		GAIA::BL equal(const _DataType1& t1, const _DataType2& t2)
		{
			return t1 == t2;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::BL equal(_DataType1 p1, _DataType2 p2, const _SizeType& size)
		{
			GAST(!!p1);
			GAST(!!p2);
			for(_SizeType x = 0; x < size; ++x)
			{
				if(p1[x] != p2[x])
					return GAIA::False;
			}
			return GAIA::True;
		}
		template<typename _DataType, typename _KeyType>
		_DataType find(_DataType pBegin, _DataType pEnd, const _KeyType& key)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin <= pEnd)
			{
				if(*pBegin == key)
					return pBegin;
				++pBegin;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType* findp(const _DataType** ppBegin, const _DataType** ppEnd, const _KeyType& key)
		{
			GAST(!!ppBegin);
			GAST(!!ppEnd);
			GAST(ppBegin <= ppEnd);
			while(ppBegin <= ppEnd)
			{
				if(**ppBegin == key)
					return const_cast<_DataType*>(*ppBegin);
				++ppBegin;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		_DataType finds(_DataType pBegin, _DataType pEnd, _KeyType p, const _SizeType& size)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(!!p);
			GAST(size != 0);
			while(pEnd - pBegin + 1 >= size)
			{
				if(GAIA::ALGO::equal(pBegin, p, size))
					return pBegin;
				++pBegin;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType rfind(_DataType pBegin, _DataType pEnd, const _KeyType& key)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin <= pEnd)
			{
				if(*pEnd == key)
					return pEnd;
				--pEnd;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType* rfindp(const _DataType** ppBegin, const _DataType** ppEnd, const _KeyType& key)
		{
			GAST(!!ppBegin);
			GAST(!!ppEnd);
			GAST(ppBegin <= ppEnd);
			while(ppBegin <= ppEnd)
			{
				if(**ppEnd == key)
					return const_cast<_DataType*>(*ppEnd);
				--ppEnd;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		_DataType rfinds(_DataType pBegin, _DataType pEnd, _KeyType p, const _SizeType& size)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(!!p);
			GAST(size != 0);
			while(pEnd - pBegin + 1 >= size)
			{
				if(GAIA::ALGO::equal(pEnd - size + 1, p, size))
					return pEnd - size + 1;
				--pEnd;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType glinear_search(_DataType pBegin, _DataType pEnd, const _KeyType& key)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin <= pEnd)
			{
				if(*pBegin >= key)
				{
					if(*pBegin == key)
						return pBegin;
					else
						break;
				}
				++pBegin;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType glinear_search(_DataType pBegin, _DataType pEnd, const _KeyType& key, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k))
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin <= pEnd)
			{
				GAIA::N32 nCmp = cmpor(pBegin, &key);
				if(nCmp == 0)
					return pBegin;
				else if(nCmp > 0)
					break;
				++pBegin;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType gbinary_search(_DataType pBegin, _DataType pEnd, const _KeyType& key)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			if(pBegin == pEnd)
			{
				if(*pBegin == key)
					return pBegin;
				return GNIL;
			}
			else
			{
				if(pEnd - pBegin <= 16)
					return GAIA::ALGO::glinear_search(pBegin, pEnd, key);
			}
			_DataType pMid = pBegin + (pEnd - pBegin) / 2;
			if(key <= *pMid)
				return GAIA::ALGO::gbinary_search(pBegin, pMid, key);
			else if(key > *pMid)
				return GAIA::ALGO::gbinary_search(pMid + 1, pEnd, key);
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType gbinary_search(_DataType pBegin, _DataType pEnd, const _KeyType& key, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k))
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			if(pBegin == pEnd)
			{
				GAIA::N32 nCmp = cmpor(pBegin, &key);
				if(nCmp == 0)
					return pBegin;
				return GNIL;
			}
			else
			{
				if(pEnd - pBegin <= 16)
					return GAIA::ALGO::glinear_search(pBegin, pEnd, key, cmpor);
			}
			_DataType pMid = pBegin + (pEnd - pBegin) / 2;
			GAIA::N32 nCmp = cmpor(pMid, &key);
			if(nCmp >= 0)
				return GAIA::ALGO::gbinary_search(pBegin, pMid, key, cmpor);
			else if(nCmp < 0)
				return GAIA::ALGO::gbinary_search(pMid + 1, pEnd, key, cmpor);
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType upper_equal(_DataType pBegin, _DataType pEnd, const _KeyType& key)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(pBegin == pEnd)
			{
				if(*pBegin >= key)
					return pBegin;
				return GNIL;
			}
			_DataType pMid = pBegin + (pEnd - pBegin) / 2;
			if(key <= *pMid)
			{
				_DataType pRet = GAIA::ALGO::upper_equal(pBegin, pMid, key);
				if(pRet != GNIL)
					return pRet;
			}
			else if(key > *pMid)
			{
				_DataType pRet = GAIA::ALGO::upper_equal(pMid + 1, pEnd, key);
				if(pRet != GNIL)
					return pRet;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType upper_equal(_DataType pBegin, _DataType pEnd, const _KeyType& key, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k))
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(pBegin == pEnd)
			{
				GAIA::N32 nCmp = cmpor(pBegin, &key);
				if(nCmp == 0)
					return pBegin;
				return GNIL;
			}
			_DataType pMid = pBegin + (pEnd - pBegin) / 2;
			GAIA::N32 nCmp = cmpor(pMid, &key);
			if(nCmp >= 0)
			{
				_DataType pRet = GAIA::ALGO::upper_equal(pBegin, pMid, key, cmpor);
				if(pRet != GNIL)
					return pRet;
			}
			else if(nCmp < 0)
			{
				_DataType pRet = GAIA::ALGO::upper_equal(pMid + 1, pEnd, key, cmpor);
				if(pRet != GNIL)
					return pRet;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType lower_equal(_DataType pBegin, _DataType pEnd, const _KeyType& key)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(pBegin == pEnd)
			{
				if(*pEnd <= key)
					return pEnd;
				return GNIL;
			}
			_DataType pMid = pBegin + (pEnd - pBegin) / 2;
			if((pEnd - pBegin) % 2 != 0)
				++pMid;
			if(key >= *pMid)
			{
				_DataType pRet = GAIA::ALGO::lower_equal(pMid, pEnd, key);
				if(pRet != GNIL)
					return pRet;
			}
			else if(key < *pMid)
			{
				_DataType pRet = GAIA::ALGO::lower_equal(pBegin, pMid - 1, key);
				if(pRet != GNIL)
					return pRet;
			}
			return GNIL;
		}
		template<typename _DataType, typename _KeyType>
		_DataType lower_equal(_DataType pBegin, _DataType pEnd, const _KeyType& key, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k))
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			if(pBegin == pEnd)
			{
				GAIA::N32 nCmp = cmpor(pEnd, &key);
				if(nCmp <= 0)
					return pEnd;
				return GNIL;
			}
			_DataType pMid = pBegin + (pEnd - pBegin) / 2;
			if((pEnd - pBegin) % 2 != 0)
				++pMid;
			GAIA::N32 nCmp = cmpor(pMid, &key);
			if(nCmp <= 0)
			{
				_DataType pRet = GAIA::ALGO::lower_equal(pMid, pEnd, key, cmpor);
				if(pRet != GNIL)
					return pRet;
			}
			else if(nCmp > 0)
			{
				_DataType pRet = GAIA::ALGO::lower_equal(pBegin, pMid - 1, key, cmpor);
				if(pRet != GNIL)
					return pRet;
			}
			return GNIL;
		}
		template<typename _DataType, typename _SizeType>
		GAIA::GVOID index(_DataType pBegin, _DataType pCurrent, _SizeType& result)
		{
			result = pCurrent - pBegin;
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID count(_DataType pBegin, _DataType pEnd, const _KeyType& key, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin != pEnd)
			{
				if(*pBegin == key)
					++result;
				++pBegin;
			}
			if(*pBegin == key)
				++result;
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID count_order(_DataType pBegin, _DataType pEnd, const _KeyType& key, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			pBegin = GAIA::ALGO::lower_equal(pBegin, pEnd, key);
			if(pBegin == GNIL || *pBegin != key)
				return;
			pEnd = GAIA::ALGO::upper_equal(pBegin, pEnd, key);
			if(pEnd == GNIL || *pEnd != key)
				return;
			result += (_SizeType)(pEnd - pBegin + 1);
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID counts(_DataType pBegin, _DataType pEnd, _KeyType pKeyBegin, _KeyType pKeyEnd, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(!!pKeyBegin);
			GAST(!!pKeyEnd);
			GAST(pKeyBegin <= pKeyEnd);
			while(pKeyBegin != pKeyEnd)
			{
				GAIA::ALGO::count(pBegin, pEnd, *pKeyBegin, result);
				++pKeyBegin;
			}
			GAIA::ALGO::count(pBegin, pEnd, *pKeyBegin, result);
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID counts_order(_DataType pBegin, _DataType pEnd, _KeyType pKeyBegin, _KeyType pKeyEnd, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(!!pKeyBegin);
			GAST(!!pKeyEnd);
			GAST(pKeyBegin <= pKeyEnd);
			while(pKeyBegin <= pKeyEnd)
			{
				GAIA::ALGO::count_order(pBegin, pEnd, *pKeyBegin, result);
				++pKeyBegin;
			}
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID prevcount(_DataType pBegin, _DataType pEnd, const _KeyType& key, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin != pEnd)
			{
				if(*pEnd != key)
					break;
				--pEnd;
				++result;
			}
			if(*pEnd == key)
				++result;
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID nextcount(_DataType pBegin, _DataType pEnd, const _KeyType& key, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin != pEnd)
			{
				if(*pBegin != key)
					break;
				++pBegin;
				++result;
			}
			if(*pBegin == key)
				++result;
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID lower_equal_count(_DataType pBegin, _DataType pEnd, const _KeyType& key, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin <= pEnd)
			{
				if(*pBegin <= key)
					++result;
				++pBegin;
			}
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID upper_equal_count(_DataType pBegin, _DataType pEnd, const _KeyType& key, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin <= pEnd)
			{
				if(*pBegin >= key)
					++result;
				++pBegin;
			}
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID lower_count(_DataType pBegin, _DataType pEnd, const _KeyType& key, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin <= pEnd)
			{
				if(*pBegin < key)
					++result;
				++pBegin;
			}
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID upper_count(_DataType pBegin, _DataType pEnd, const _KeyType& key, _SizeType& result)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin <= pEnd);
			while(pBegin <= pEnd)
			{
				if(*pBegin > key)
					++result;
				++pBegin;
			}
		}
		template<typename _DataType1, typename _SizeType1, typename _DataType2, typename _SizeType2>
		GAIA::BL startwith(_DataType1 p, _SizeType1 sizep, _DataType2 finder, _SizeType2 sizefinder)
		{
			GAST(p != GNIL);
			GAST(sizep > 0);
			GAST(finder != GNIL);
			GAST(sizefinder > 0);
			if(sizep < sizefinder)
				return GAIA::False;
			for(_SizeType2 x = 0; x < sizefinder; ++x)
			{
				if(p[x] != finder[x])
					return GAIA::False;
			}
			return GAIA::True;
		}
		template<typename _DataType1, typename _SizeType1, typename _DataType2, typename _SizeType2>
		GAIA::BL endwith(_DataType1 p, _SizeType1 sizep, _DataType2 finder, _SizeType2 sizefinder)
		{
			GAST(p != GNIL);
			GAST(sizep > 0);
			GAST(finder != GNIL);
			GAST(sizefinder > 0);
			if(sizep < sizefinder)
				return GAIA::False;
			_SizeType1 headoffset = sizep - sizefinder;
			for(_SizeType2 x = 0; x < sizefinder; ++x)
			{
				if(p[headoffset + x] != finder[x])
					return GAIA::False;
			}
			return GAIA::True;
		}
	}
}

#endif
