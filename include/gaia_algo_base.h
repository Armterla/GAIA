#ifndef		__GAIA_ALGO_BASE_H__
#define		__GAIA_ALGO_BASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType1, typename _DataType2>
		GAIA::GVOID move(_DataType1& dst, const _DataType2& src){dst = src;}
		template<typename _DataType, typename _SizeType>
		GAIA::GVOID move_left(_DataType p, const _SizeType& size)
		{
			for(_SizeType x = 0; x < size; ++x)
			{
				*p = *(p + 1);
				++p;
			}
		}
		template<typename _DataType, typename _SizeType>
		GAIA::GVOID move_right(_DataType p, const _SizeType& size)
		{
			for(_SizeType x = 0; x < size; ++x)
			{
				*p = *(p - 1);
				--p;
			}
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::GVOID move_left(_DataType1 dst, _DataType2 src, const _SizeType& size)
		{
			_SizeType sizetemp = size;
			while(sizetemp > 0)
			{
				*dst = *src;
				++dst;
				++src;
				--sizetemp;
			}
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::GVOID move_right(_DataType1 dst, _DataType2 src, const _SizeType& size)
		{
			_SizeType sizetemp = size;
			while(sizetemp > 0)
			{
				*dst = *src;
				--dst;
				--src;
				--sizetemp;
			}
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::GVOID swap(_DataType1& t1, _DataType2& t2)
		{
			typename GAIA::TRAITS::DataTypeConvertTraits<_DataType1, _DataType2>::ConvertedType t = t2;
			t2 = t1;
			t1 = t;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::GVOID swaps(_DataType1 p1, _DataType2 p2, const _SizeType& size)
		{
			for(_SizeType x = 0; x < size; ++x)
				GAIA::ALGO::swap(p1[x], p2[x]);
		}
		template<typename _DataType, typename _KeyType>
		_DataType swapleft(_DataType pBegin, _DataType pEnd, const _KeyType& key)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			_DataType pTemp = pBegin;
			while(pTemp <= pEnd)
			{
				if(*pTemp == key)
				{
					if(pBegin != pTemp)
						GAIA::ALGO::swap(*pBegin, *pTemp);
					++pBegin;
				}
				++pTemp;
			}
			return pBegin;
		}
		template<typename _DataType, typename _KeyType>
		_DataType swapleft(_DataType pBegin, _DataType pEnd, _KeyType pKeyBegin, _KeyType pKeyEnd)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(!!pKeyBegin);
			GAST(!!pKeyBegin);
			while(pKeyBegin <= pKeyEnd)
			{
				pBegin = GAIA::ALGO::swapleft(pBegin, pEnd, *pKeyBegin);
				++pKeyBegin;
			}
			return pBegin;
		}
		template<typename _DataType, typename _KeyType>
		_DataType swapleft_inversekey(_DataType pBegin, _DataType pEnd, const _KeyType& key)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			_DataType pTemp = pBegin;
			while(pTemp <= pEnd)
			{
				if(*pTemp != key)
				{
					if(pBegin != pTemp)
						GAIA::ALGO::swap(*pBegin, *pTemp);
					++pBegin;
				}
				++pTemp;
			}
			return pBegin;
		}
		template<typename _DataType, typename _KeyType>
		_DataType swapleft_inversekey(_DataType pBegin, _DataType pEnd, _KeyType pKeyBegin, _KeyType pKeyEnd)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(!!pKeyBegin);
			GAST(!!pKeyBegin);
			_DataType pTemp = pBegin;
			while(pTemp <= pEnd)
			{
				GAIA::BL bExist = GAIA::False;
				_KeyType pTempKey = pKeyBegin;
				while(pTempKey <= pKeyEnd)
				{
					if(*pTemp == *pTempKey)
					{
						bExist = GAIA::True;
						break;
					}
					++pTempKey;
				}
				if(!bExist)
				{
					if(pBegin != pTemp)
						GAIA::ALGO::swap(*pBegin, *pTemp);
					++pBegin;
				}
				++pTemp;
			}
			return pBegin;
		}
		template<typename _DataType1, typename _DataType2, typename _DataType3, typename _DataType4>
		_DataType1 insert(_DataType1 dst, _DataType2 size, _DataType3 src, _DataType4 index)
		{
			GAST(dst != GNIL);
			GAST(size >= 0);
			GAST(index >= 0 && index <= size);
			if(index != size)
				GAIA::ALGO::move_right(dst + size, dst + size - 1, size - index);
			dst[index] = src;
			return dst;
		}
		template<typename _DataType> GAIA::GVOID inverse(_DataType pBegin, _DataType pEnd)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			while(pBegin != pEnd)
			{
				GAIA::ALGO::swap(*pBegin, *pEnd);
				++pBegin;
				if(pBegin == pEnd)
					break;
				--pEnd;
			}
		}
	}
}

#endif
