#ifndef		__GAIA_ALGO_COMPARE_H__
#define		__GAIA_ALGO_COMPARE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_type_traits2.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType1, typename _DataType2>
		const typename GAIA::TRAITS::DataTypeConvertTraits<_DataType1, _DataType2>::ConvertedType
		gmax(const _DataType1& v1, const _DataType2& v2)
		{
			return v1 > v2 ? v1 : v2;
		}
		template<typename _DataType1, typename _DataType2>
		const typename GAIA::TRAITS::DataTypeConvertTraits<_DataType1, _DataType2>::ConvertedType
		gmin(const _DataType1& v1, const _DataType2& v2)
		{
			return v1 > v2 ? v2 : v1;
		}
		template<typename _DataType> _DataType gmax3(_DataType v1, _DataType v2, _DataType v3)
		{
			return GAIA::ALGO::gmax(GAIA::ALGO::gmax(v1, v2), v3);
		}
		template<typename _DataType> _DataType gmin3(_DataType v1, _DataType v2, _DataType v3)
		{
			return GAIA::ALGO::gmin(GAIA::ALGO::gmin(v1, v2), v3);
		}
		template<typename _DataType> _DataType gmid3(_DataType v1, _DataType v2, _DataType v3)
		{
			if(v1 > v2)
			{
				if(v2 > v3)
					return v2;
				else
				{
					if(v3 > v1)
						return v1;
					else
						return v3;
				}
			}
			else
			{
				if(v1 > v3)
					return v1;
				else
				{
					if(v3 > v2)
						return v2;
					else
						return v3;
				}
			}
		}
		template<typename _DataType> GAIA::GVOID minmax2(
				_DataType v1, _DataType v2, _DataType& resmin, _DataType& resmax)
		{
			if(v1 < v2)
			{
				resmin = v1;
				resmax = v2;
			}
			else
			{
				resmin = v2;
				resmax = v1;
			}
		}
		template<typename _DataType> GAIA::GVOID minmidmax3(
				_DataType v1, _DataType v2, _DataType v3, _DataType& resmin, _DataType& resmid, _DataType& resmax)
		{
			if(v1 > v2)
			{
				if(v2 > v3)
				{
					resmin = v3;
					resmid = v2;
					resmax = v1;
				}
				else
				{
					if(v3 > v1)
					{
						resmin = v2;
						resmid = v1;
						resmax = v3;
					}
					else
					{
						resmin = v2;
						resmid = v3;
						resmax = v1;
					}
				}
			}
			else
			{
				if(v1 > v3)
				{
					resmin = v3;
					resmid = v1;
					resmax = v2;
				}
				else
				{
					if(v3 > v2)
					{
						resmin = v1;
						resmid = v2;
						resmax = v3;
					}
					else
					{
						resmin = v1;
						resmid = v3;
						resmax = v2;
					}
				}
			}
		}
		template<typename _DataType, typename _ResultDataType>
		GAIA::GVOID gmaxs(_DataType pBegin, _DataType pEnd, _ResultDataType& res)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			while(pBegin != pEnd)
			{
				if(*pBegin > res)
					res = *pBegin;
				++pBegin;
			}
			if(*pBegin > res)
				res = *pBegin;
		}
		template<typename _DataType, typename _ResultDataType>
		GAIA::GVOID gmins(_DataType pBegin, _DataType pEnd, _ResultDataType& res)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			while(pBegin != pEnd)
			{
				if(*pBegin < res)
					res = *pBegin;
				++pBegin;
			}
			if(*pBegin < res)
				res = *pBegin;
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 cmp(const _DataType1& v1, const _DataType2& v2)
		{
			if(v1 < v2)
				return -1;
			else if(v1 > v2)
				return +1;
			else
				return 0;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::N32 cmps(_DataType1 p1, _DataType2 p2, const _SizeType& size)
		{
			_SizeType sizet = size;
			while(sizet > 0)
			{
				if(*p1 < *p2)
					return -1;
				else if(*p1 > *p2)
					return +1;
				++p1;
				++p2;
				--sizet;
			}
			return 0;
		}
		template<typename _DataType1, typename _SizeType1, typename _DataType2, typename _SizeType2>
		GAIA::N32 cmps(_DataType1 p1, const _SizeType1& size1, _DataType2 p2, const _SizeType2& size2)
		{
			if(size1 < size2)
				return -1;
			else if(size1 > size2)
				return +1;
			else
				return GAIA::ALGO::cmps(p1, p2, size1);
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::N32 cmpk(_DataType p, const _KeyType& key, const _SizeType& size)
		{
			_SizeType sizet = size;
			while(sizet > 0)
			{
				if(*p < key)
					return -1;
				else if(*p > key)
					return +1;
				++p;
				--sizet;
			}
			return 0;
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 cmp2(_DataType1 p1, _DataType2 p2)
		{
			if(p1[0] < p2[0])
				return -1;
			else if(p1[0] > p2[0])
				return +1;
			else
			{
				if(p1[1] < p2[1])
					return -1;
				else if(p1[1] > p2[1])
					return +1;
				else
					return 0;
			}
		}
		template<typename _DataType, typename _KeyType>
		GAIA::N32 cmp2k(_DataType p, const _KeyType& key)
		{
			if(p[0] < key)
				return -1;
			else if(p[0] > key)
				return +1;
			else
			{
				if(p[1] < key)
					return -1;
				else if(p[1] > key)
					return +1;
				else
					return 0;
			}
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 cmp3(_DataType1 p1, _DataType2 p2)
		{
			if(p1[0] < p2[0])
				return -1;
			else if(p1[0] > p2[0])
				return +1;
			else
			{
				if(p1[1] < p2[1])
					return -1;
				else if(p1[1] > p2[1])
					return +1;
				else
				{
					if(p1[2] < p2[2])
						return -1;
					else if(p1[2] > p2[2])
						return +1;
					else
						return 0;
				}
			}
		}
		template<typename _DataType, typename _KeyType>
		GAIA::N32 cmp3k(_DataType p, const _KeyType& key)
		{
			if(p[0] < key)
				return -1;
			else if(p[0] > key)
				return +1;
			else
			{
				if(p[1] < key)
					return -1;
				else if(p[1] > key)
					return +1;
				else
				{
					if(p[2] < key)
						return -1;
					else if(p[2] > key)
						return +1;
					else
						return 0;
				}
			}
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 cmp4(_DataType1 p1, _DataType2 p2)
		{
			if(p1[0] < p2[0])
				return -1;
			else if(p1[0] > p2[0])
				return +1;
			else
			{
				if(p1[1] < p2[1])
					return -1;
				else if(p1[1] > p2[1])
					return +1;
				else
				{
					if(p1[2] < p2[2])
						return -1;
					else if(p1[2] > p2[2])
						return +1;
					else
					{
						if(p1[3] < p2[3])
							return -1;
						else if(p1[3] > p2[3])
							return +1;
						else
							return 0;
					}
				}
			}
		}
		template<typename _DataType, typename _KeyType>
		GAIA::N32 cmp4k(_DataType p, const _KeyType& key)
		{
			if(p[0] < key)
				return -1;
			else if(p[0] > key)
				return +1;
			else
			{
				if(p[1] < key)
					return -1;
				else if(p[1] > key)
					return +1;
				else
				{
					if(p[2] < key)
						return -1;
					else if(p[2] > key)
						return +1;
					else
					{
						if(p[3] < key)
							return -1;
						else if(p[3] > key)
							return +1;
						else
							return 0;
					}
				}
			}
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 cmpp(_DataType1 p1, _DataType2 p2)
		{
			if(p1 == GNIL && p2 == GNIL)
				return 0;
			else if(p1 != GNIL && p2 == GNIL)
				return -1;
			else if(p1 == GNIL && p2 != GNIL)
				return +1;
			else
			{
				if(*p1 < *p2)
					return -1;
				else if(*p1 > *p2)
					return +1;
				else
					return 0;
			}
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::GVOID copy(_DataType1 dst, _DataType2 src, const _SizeType& size)
		{
			_SizeType sizet = size;
			while(sizet > 0)
			{
				*dst++ = *src++;
				--sizet;
			}
		}
		template<typename _DataType, typename _SizeType>
		GAIA::GVOID gnil(_DataType dst, const _SizeType& size)
		{
			_SizeType sizet = size;
			while(sizet > 0)
			{
				*dst++ = GNIL;
				--sizet;
			}
		}
		template<typename _DataType, typename _KeyType, typename _SizeType>
		GAIA::GVOID reset(_DataType dst, const _KeyType& t, const _SizeType& size)
		{
			_SizeType sizet = size;
			while(sizet > 0)
			{
				*dst++ = t;
				--sizet;
			}
		}
	}
}

#endif
