#ifndef 	__GAIA_MATH_RANDOM_H__
#define 	__GAIA_MATH_RANDOM_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#include <stdlib.h>

namespace GAIA
{
	namespace MATH
	{
		GINL GAIA::N32 grand(){return rand();}
		GINL GAIA::N32 grand_max(){return RAND_MAX;}
		GINL GAIA::GVOID grand_seed(GAIA::N32 seed){srand(seed);}
		GINL GAIA::U8 grand_u8(){return (GAIA::U8)(grand() & 0xFF);}
		GINL GAIA::U16 grand_u16(){return ((GAIA::U16)grand_u8() << 8) | (GAIA::U16)grand_u8();}
		GINL GAIA::U32 grand_u32(){return ((GAIA::U32)grand_u16() << 16) | (GAIA::U32)grand_u16();}
		GINL GAIA::U64 grand_u64(){return ((GAIA::U64)grand_u32() << 32) | (GAIA::U64)grand_u32();}
		GINL GAIA::F32 grand_f32(){return (GAIA::F32)grand_u32() / (GAIA::F32)GAIA::U32MAX;}
		GINL GAIA::F64 grand_f64(){return (GAIA::F64)grand_u64() / (GAIA::F64)GAIA::U64MAX;}
	}
}

#endif
