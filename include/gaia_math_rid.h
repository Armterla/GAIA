#ifndef 	__GAIA_MATH_RID_H__
#define 	__GAIA_MATH_RID_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_algo_hash.h"

namespace GAIA
{
	namespace MATH
	{
		/* 128 bit random id. */
		class RID128 : public GAIA::Base
		{
		public:
			GINL GAIA::NUM size() const{return sizeofarray(u);}
			GINL GAIA::BL empty() const{return u64_0 == 0 && u64_1 == 0;}
			GINL GAIA::GVOID clear(){u64_0 = u64_1 = 0;}
			const GAIA::U8* fptr() const{return u;}
			GAIA::U8* fptr(){return u;}
			GINL GAIA::GVOID uuid();
			template<typename _ParamDataType> GAIA::GVOID fromstring(const _ParamDataType* psz){GAIA::ALGO::str2hex(psz, sizeofarray(u), u);}
			template<typename _ParamDataType> _ParamDataType* tostring(_ParamDataType* psz) const{GAIA::ALGO::hex2str(u, sizeofarray(u), psz); return psz;}
			GINL GAIA::U64 hash() const{return GAIA::ALGO::hash(u64_0 + u64_1);}
			GINL GAIA::MATH::RID128& operator = (const GAIA::MATH::RID128& src){u64_0 = src.u64_0; u64_1 = src.u64_1; return *this;}
			GINL GAIA::N32 compare(const GAIA::MATH::RID128& src) const
			{
				if(u64_0 < src.u64_0)
					return -1;
				else if(u64_0 > src.u64_0)
					return +1;
				if(u64_1 < src.u64_1)
					return -1;
				else if(u64_1 > src.u64_1)
					return +1;
				return 0;
			}
			GCLASS_COMPARE_BYCOMPARE(GAIA::MATH::RID128)
		public:
			union
			{
				GAIA::U8 u[16];
				class
				{
				public:
					GAIA::U32 u0;
					GAIA::U32 u1;
					GAIA::U32 u2;
					GAIA::U32 u3;
				};
				class
				{
				public:
					GAIA::U64 u64_0;
					GAIA::U64 u64_1;
				};
			};
		};

		/* 64 bit random id. */
		class RID64 : public GAIA::Base
		{
		public:
			GINL GAIA::NUM size() const{return sizeofarray(u);}
			GINL GAIA::BL empty() const{return u64_0 == 0;}
			GINL GAIA::GVOID clear(){u64_0 = 0;}
			const GAIA::U8* fptr() const{return u;}
			GAIA::U8* fptr(){return u;}
			GINL GAIA::GVOID uuid();
			template<typename _ParamDataType> GAIA::GVOID fromstring(const _ParamDataType* psz){GAIA::ALGO::str2hex(psz, sizeofarray(u), u);}
			template<typename _ParamDataType> _ParamDataType* tostring(_ParamDataType* psz) const{GAIA::ALGO::hex2str(u, sizeofarray(u), psz); return psz;}
			GINL GAIA::U64 hash() const{return GAIA::ALGO::hash(u64_0);}
			GINL GAIA::MATH::RID64& operator = (const GAIA::MATH::RID64& src){u64_0 = src.u64_0; return *this;}
			GINL GAIA::N32 compare(const GAIA::MATH::RID64& src) const
			{
				if(u64_0 < src.u64_0)
					return -1;
				else if(u64_0 > src.u64_0)
					return +1;
				return 0;
			}
			GCLASS_COMPARE_BYCOMPARE(GAIA::MATH::RID64)
		public:
			union
			{
				GAIA::U8 u[8];
				class
				{
				public:
					GAIA::U32 u0;
					GAIA::U32 u1;
				};
				class
				{
				public:
					GAIA::U64 u64_0;
				};
			};
		};

		/* 32 bit random id. */
		class RID32 : public GAIA::Base
		{
		public:
			GINL GAIA::NUM size() const{return sizeofarray(u);}
			GINL GAIA::BL empty() const{return u0 == 0;}
			GINL GAIA::GVOID clear(){u0 = 0;}
			const GAIA::U8* fptr() const{return u;}
			GAIA::U8* fptr(){return u;}
			GINL GAIA::GVOID uuid();
			template<typename _ParamDataType> GAIA::GVOID fromstring(const _ParamDataType* psz){GAIA::ALGO::str2hex(psz, sizeofarray(u), u);}
			template<typename _ParamDataType> _ParamDataType* tostring(_ParamDataType* psz) const{GAIA::ALGO::hex2str(u, sizeofarray(u), psz); return psz;}
			GINL GAIA::U64 hash() const{return GAIA::ALGO::hash(u0);}
			GINL GAIA::MATH::RID32& operator = (const GAIA::MATH::RID32& src){u0 = src.u0; return *this;}
			GINL GAIA::N32 compare(const GAIA::MATH::RID32& src) const
			{
				if(u0 < src.u0)
					return -1;
				else if(u0 > src.u0)
					return +1;
				return 0;
			}
			GCLASS_COMPARE_BYCOMPARE(GAIA::MATH::RID32)
		public:
			union
			{
				GAIA::U8 u[4];
				class
				{
				public:
					GAIA::U32 u0;
				};
			};
		};
	}
}

#endif
