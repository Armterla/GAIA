#ifndef 	__GAIA_MATH_RANDOM_H__
#define 	__GAIA_MATH_RANDOM_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#include <stdlib.h>

namespace GAIA
{
	namespace MATH
	{
		GINL GAIA::N32 xrandom(){return rand();}
		GINL GAIA::N32 xrandom_max(){return RAND_MAX;}
		GINL GAIA::GVOID xrandom_seed(GAIA::N32 seed){srand(seed);}
		GINL GAIA::U8 xrandom_u8(){return (GAIA::U8)(xrandom() & 0xFF);}
		GINL GAIA::U16 xrandom_u16(){return ((GAIA::U16)xrandom_u8() << 8) | (GAIA::U16)xrandom_u8();}
		GINL GAIA::U32 xrandom_u32(){return ((GAIA::U32)xrandom_u16() << 16) | (GAIA::U32)xrandom_u16();}
		GINL GAIA::U64 xrandom_u64(){return ((GAIA::U64)xrandom_u32() << 32) | (GAIA::U64)xrandom_u32();}
		GINL GAIA::F32 xrandom_f32(){return (GAIA::F32)xrandom_u32() / (GAIA::F32)GAIA::U32MAX;}
		GINL GAIA::F64 xrandom_f64(){return (GAIA::F64)xrandom_u64() / (GAIA::F64)GAIA::U64MAX;}
	}
}

#endif
