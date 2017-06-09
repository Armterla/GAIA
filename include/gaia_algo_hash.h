#ifndef		__GAIA_ALGO_HASH_H__
#define		__GAIA_ALGO_HASH_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType> GAIA::U64 hash(const _DataType& t)
		{
			return t.hash();
		}
		template<typename _DataType> GAIA::U64 hash(const _DataType* p)
		{
			return p->hash();
		}
		GINL GAIA::U64 hash(const GAIA::CH* p)
		{
			GAIA::U64 ret = 0;
			for(; *p != '\0'; ++p)
				ret += *p;
			return ret;
		}
		GINL GAIA::U64 hash(GAIA::CH* p)
		{
			return GAIA::ALGO::hash((const GAIA::CH*)p);
		}
		GINL GAIA::U64 hash(const GAIA::WCH* p)
		{
			GAIA::U64 ret = 0;
			for(; *p != '\0'; ++p)
				ret += *p;
			return ret;
		}
		GINL GAIA::U64 hash(GAIA::WCH* p)
		{
			return GAIA::ALGO::hash((const GAIA::WCH*)p);
		}
		GINL GAIA::U64 hash(GAIA::U8 u)
		{
			return u;
		}
		GINL GAIA::U64 hash(GAIA::U16 u)
		{
			return u;
		}
		GINL GAIA::U64 hash(GAIA::U32 u)
		{
			return u;
		}
		GINL GAIA::U64 hash(GAIA::U64 u)
		{
			return u;
		}
		GINL GAIA::U64 hash(GAIA::UM u)
		{
			return u;
		}
		GINL GAIA::U64 hash(GAIA::N8 n)
		{
			return n < 0 ? (~n) : n;
		}
		GINL GAIA::U64 hash(GAIA::N16 n)
		{
			return n < 0 ? (~n) : n;
		}
		GINL GAIA::U64 hash(GAIA::N32 n)
		{
			return n < 0 ? (~n) : n;
		}
		GINL GAIA::U64 hash(GAIA::N64 n)
		{
			return n < 0 ? (~n) : n;
		}
		GINL GAIA::U64 hash(GAIA::NM n)
		{
			return n < 0 ? (~n) : n;
		}
		GINL GAIA::U64 hash(GAIA::WCH c)
		{
			return c;
		}
		GINL GAIA::U64 hash(GAIA::F32 f)
		{
			return *(GAIA::U32*)&f;
		}
		GINL GAIA::U64 hash(GAIA::F64 d)
		{
			return *(GAIA::U64*)&d;
		}
		GINL GAIA::U64 hash(const GAIA::GVOID* p)
		{
			return GAIA::ALGO::hash((GAIA::U64)p);
		}
		GINL GAIA::U64 hash(GAIA::GVOID* p)
		{
			return GAIA::ALGO::hash((GAIA::U64)p);
		}
		GINL GAIA::U64 hash(const GAIA::GVOID* p, GAIA::NUM sSize)
		{
			GAIA::U64 uRet = 0;
			GAIA::NUM s64Size = sSize / 8;
			for(GAIA::NUM x = 0; x < s64Size; ++x)
			{
				uRet += *(GAIA::U64*)p;
				p = ((GAIA::U8*)p) + sizeof(GAIA::U64);
			}
			if(sSize % 8 >= 4)
			{
				uRet += *(GAIA::U32*)p;
				p = ((GAIA::U8*)p) + sizeof(GAIA::U32);
			}
			if(sSize % 4 >= 2)
			{
				uRet += *(GAIA::U16*)p;
				p = ((GAIA::U8*)p) + sizeof(GAIA::U16);
			}
			if(sSize % 2 != 0)
			{
				uRet += *(GAIA::U8*)p;
				p = ((GAIA::U8*)p) + sizeof(GAIA::U8);
			}
			return uRet;
		}
		GINL GAIA::U64 hash(GAIA::GVOID* p, GAIA::NUM sSize)
		{
			return GAIA::ALGO::hash((const GAIA::GVOID*)p, sSize);
		}
	}
}

#endif
