#ifndef		__GAIA_ALGO_SET_H__
#define		__GAIA_ALGO_SET_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType, typename _SizeType>
		GAIA::GVOID set_random(_DataType pBegin, _DataType pEnd, const _SizeType& times)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			GAST(pBegin < pEnd);
			if(pBegin >= pEnd)
				return;
			GAIA::U64 uLen = pEnd - pBegin + 1;
			if(uLen < GAIA::U8MAX)
			{
				for(_SizeType x = 0; x < times; ++x)
				{
					GAIA::U8 u1 = GAIA::MATH::grand_u8() % uLen;
					GAIA::U8 u2 = GAIA::MATH::grand_u8() % uLen;
					GAIA::ALGO::swap(pBegin[u1], pBegin[u2]);
				}
			}
			else if(uLen < GAIA::U16MAX)
			{
				for(_SizeType x = 0; x < times; ++x)
				{
					GAIA::U16 u1 = GAIA::MATH::grand_u16() % uLen;
					GAIA::U16 u2 = GAIA::MATH::grand_u16() % uLen;
					GAIA::ALGO::swap(pBegin[u1], pBegin[u2]);
				}
			}
			else if(uLen < GAIA::U32MAX)
			{
				for(_SizeType x = 0; x < times; ++x)
				{
					GAIA::U32 u1 = GAIA::MATH::grand_u32() % uLen;
					GAIA::U32 u2 = GAIA::MATH::grand_u32() % uLen;
					GAIA::ALGO::swap(pBegin[u1], pBegin[u2]);
				}
			}
			else
			{
				for(_SizeType x = 0; x < times; ++x)
				{
					GAIA::U64 u1 = GAIA::MATH::grand_u64() % uLen;
					GAIA::U64 u2 = GAIA::MATH::grand_u64() % uLen;
					GAIA::ALGO::swap(pBegin[u1], pBegin[u2]);
				}
			}
		}
		template<typename _DataType> GAIA::NUM set_and_count(
				_DataType pDstBegin, _DataType pDstEnd,
				_DataType pSrcBegin, _DataType pSrcEnd) // If return value is GINVALID, the call failed!
		{
			GAST(pDstBegin != GNIL);
			GAST(pDstEnd != GNIL);
			GAST(pSrcBegin != GNIL);
			GAST(pSrcEnd != GNIL);
			if(!GAIA::ALGO::isunique_noorder(pSrcBegin, pSrcEnd))
				return GINVALID;
			GAIA::NUM ret = 0;
			while(pSrcBegin <= pSrcEnd)
			{
				_DataType pTemp = pDstBegin;
				while(pTemp <= pDstEnd)
				{
					if(*pTemp == *pSrcBegin)
						++ret;
					++pTemp;
				}
				++pSrcBegin;
			}
			return ret;
		}
		template<typename _DataType, typename _SizeType> GAIA::NUM set_or_count(
				_DataType pDstBegin, _DataType pDstEnd,
				_DataType pSrcBegin, _DataType pSrcEnd) // If return value is GINVALID, the call failed!
		{
			GAST(pDstBegin != GNIL);
			GAST(pDstEnd != GNIL);
			GAST(pSrcBegin != GNIL);
			GAST(pSrcEnd != GNIL);
			if(!GAIA::ALGO::isunique_noorder(pSrcBegin, pSrcEnd))
				return GINVALID;
			GAIA::NUM ret = pDstEnd - pDstBegin + 1;
			while(pSrcBegin <= pSrcEnd)
			{
				GAIA::BL bExist = GAIA::False;
				_DataType pTemp = pDstBegin;
				while(pTemp <= pDstEnd)
				{
					if(*pTemp == *pSrcBegin)
					{
						bExist = GAIA::True;
						break;
					}
					++pTemp;
				}
				if(!bExist)
					++ret;
				++pSrcBegin;
			}
			return ret;
		}
		template<typename _DataType> GAIA::NUM set_sub_count(
				_DataType pDstBegin, _DataType pDstEnd,
				_DataType pSrcBegin, _DataType pSrcEnd) // If return value is GINVALID, the call failed!
		{
			GAST(pDstBegin != GNIL);
			GAST(pDstEnd != GNIL);
			GAST(pSrcBegin != GNIL);
			GAST(pSrcEnd != GNIL);
			if(!GAIA::ALGO::isunique_noorder(pSrcBegin, pSrcEnd))
				return GINVALID;
			GAIA::NUM ret = pDstEnd - pDstBegin + 1;
			while(pSrcBegin <= pSrcEnd)
			{
				_DataType pTemp = pDstBegin;
				while(pTemp <= pDstEnd)
				{
					if(*pTemp == *pSrcBegin)
					{
						GAST(ret > 0);
						--ret;
					}
					++pTemp;
				}
				++pSrcBegin;
			}
			return ret;
		}
		template<typename _DataType> GAIA::NUM set_and(
				_DataType pDstBegin, _DataType pDstEnd,
				_DataType pSrcBegin, _DataType pSrcEnd) // If return value is GINVALID, the call failed!
		{
			GAST(pDstBegin != GNIL);
			GAST(pDstEnd != GNIL);
			GAST(pSrcBegin != GNIL);
			GAST(pSrcEnd != GNIL);
			if(!GAIA::ALGO::isunique_noorder(pSrcBegin, pSrcEnd))
				return GINVALID;
			_DataType pNew = GAIA::ALGO::swapleft(pDstBegin, pDstEnd, pSrcBegin, pSrcEnd);
			return GSCAST(GAIA::NUM)(pNew - pDstBegin);
		}
		template<typename _DataType, typename _SizeType> GAIA::NUM set_or(
				_DataType pDstBegin, _DataType pDstEnd,
				_SizeType dst_max_size, _DataType pSrcBegin, _DataType pSrcEnd) // If return value is GINVALID, the call failed! If the dst_max_size <= 0, it means the buffer is must enough.
		{
			GAST(pDstBegin != GNIL);
			GAST(pDstEnd != GNIL);
			GAST(pSrcBegin != GNIL);
			GAST(pSrcEnd != GNIL);
			if(!GAIA::ALGO::isunique_noorder(pSrcBegin, pSrcEnd))
				return GINVALID;
			_DataType pWrite = pDstEnd;
			while(pSrcBegin <= pSrcEnd)
			{
				GAIA::BL bExist = GAIA::False;
				_DataType pTemp = pDstBegin;
				while(pTemp <= pDstEnd)
				{
					if(*pTemp == *pSrcBegin)
					{
						bExist = GAIA::True;
						break;
					}
					++pTemp;
				}
				if(!bExist)
					*(++pWrite) = *pSrcBegin;
				++pSrcBegin;
			}
			return GSCAST(GAIA::NUM)(pWrite - pDstBegin);
		}
		template<typename _DataType> GAIA::NUM set_sub(
				_DataType pDstBegin, _DataType pDstEnd,
				_DataType pSrcBegin, _DataType pSrcEnd) // If return value is GINVALID, the call failed!
		{
			GAST(pDstBegin != GNIL);
			GAST(pDstEnd != GNIL);
			GAST(pSrcBegin != GNIL);
			GAST(pSrcEnd != GNIL);
			if(!GAIA::ALGO::isunique_noorder(pSrcBegin, pSrcEnd))
				return GINVALID;
			_DataType pNew = GAIA::ALGO::swapleft_inversekey(pDstBegin, pDstEnd, pSrcBegin, pSrcEnd);
			return GSCAST(GAIA::NUM)(pNew - pDstBegin);
		}
		template<typename _DataType> GAIA::NUM set_merge(
				_DataType pDstBegin, _DataType pDstEnd,
				_DataType pSrc1Begin, _DataType pSrc1End,
				_DataType pSrc2Begin, _DataType pSrc2End)
		{
			GAST(pDstBegin != GNIL);
			GAST(pDstEnd != GNIL);
			GAST(pSrc1Begin != GNIL);
			GAST(pSrc1End != GNIL);
			GAST(pSrc2Begin != GNIL);
			GAST(pSrc2End != GNIL);
			_DataType pCursor = pDstBegin;
			while(pSrc1Begin <= pSrc1End)
			{
				if(pCursor > pDstEnd)
					return (GAIA::NUM)(pCursor - pDstBegin);
				*pCursor++ = *pSrc1Begin++;
			}
			while(pSrc2Begin <= pSrc2End)
			{
				if(pCursor > pDstEnd)
					return (GAIA::NUM)(pCursor - pDstBegin);
				*pCursor++ = *pSrc2Begin++;
			}
			return (GAIA::NUM)(pCursor - pDstBegin);
		}
		template<typename _DataType> GAIA::NUM set_merge_sorted(
				_DataType pDstBegin, _DataType pDstEnd,
				_DataType pSrc1Begin, _DataType pSrc1End,
				_DataType pSrc2Begin, _DataType pSrc2End)
		{
			GAST(pDstBegin != GNIL);
			GAST(pDstEnd != GNIL);
			GAST(pSrc1Begin != GNIL);
			GAST(pSrc1End != GNIL);
			GAST(pSrc2Begin != GNIL);
			GAST(pSrc2End != GNIL);
			_DataType pCursor = pDstBegin;
			while(pCursor <= pDstEnd)
			{
				if(pSrc1Begin <= pSrc1End && pSrc2Begin <= pSrc2End)
				{
					if(*pSrc1Begin < *pSrc2Begin)
						*pCursor++ = *pSrc1Begin++;
					else
						*pCursor++ = *pSrc2Begin++;
				}
				else if(pSrc1Begin <= pSrc1End)
					*pCursor++ = *pSrc1Begin++;
				else if(pSrc2Begin <= pSrc2End)
					*pCursor++ = *pSrc2Begin++;
				else
					break;
			}
			return (GAIA::NUM)(pCursor - pDstBegin);
		}
	}
}

#endif
