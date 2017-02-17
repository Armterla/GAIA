#ifndef		__GAIA_MATH_RANDOM_LCG_H__
#define		__GAIA_MATH_RANDOM_LCG_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace MATH
	{
		class RandomLCG : public GAIA::Base
		{
		public:
			RandomLCG(){this->init();}
			GINL GAIA::U32 random()
			{
				/*
				*   LCG algorithm : 
				*       First step : r(n+1) = (A * r(n) + C) % M
				*       Second step : fetch bits from r(n+1)
				*
				*
				*   MSVC
				*       A = 214013
				*       C = 2531011
				*       Fetch Bits = [30-16]
				*
				*
				*   glibc(GCC)
				*       A = 1103515245
				*       C = 12345
				*       Fetch Bits = [30-0]
				*/
				//static const GAIA::U32 M = 0xFFFFFFFF;
				static const GAIA::U32 A = 214013;
				static const GAIA::U32 C = 2531011;
				m_currandvalue = A * m_currandvalue + C;
				return (m_currandvalue >> 16) & 0x7FFF;
			}
			GINL GAIA::GVOID seed(GAIA::U32 uSeed){m_currandvalue = uSeed;}
			GINL GAIA::U32 seed() const{return m_currandvalue;}
			GINL GAIA::U32 random_max() const{return 0x7FFF;}
			GINL GAIA::U8 random_u8(){return (GAIA::U8)(this->random() & 0xFF);}
			GINL GAIA::U16 random_u16(){return ((GAIA::U16)this->random_u8() << 8) | (GAIA::U16)this->random_u8();}
			GINL GAIA::U32 random_u32(){return ((GAIA::U32)this->random_u16() << 16) | (GAIA::U32)this->random_u16();}
			GINL GAIA::U64 random_u64(){return ((GAIA::U64)this->random_u32() << 32) | (GAIA::U64)this->random_u32();}
			GINL GAIA::F32 random_f32(){return (GAIA::F32)this->random_u32() / (GAIA::F32)GAIA::U32MAX;}
			GINL GAIA::F64 random_f64(){return (GAIA::F64)this->random_u64() / (GAIA::F64)GAIA::U64MAX;}
		private:
			GAIA::GVOID init(){m_currandvalue = 0;}
		private:
			GAIA::U32 m_currandvalue;
		};
	}
}

#endif
