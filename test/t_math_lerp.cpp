#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_lerp(GAIA::LOG::Log& logobj)
	{
		GAIA::F32 f32 = GAIA::MATH::lerp(0.0F, 1, 1.0);
		if(f32 != 1.0F)
			TERROR;
		GAIA::F64 f64 = GAIA::MATH::lerpfactor_cos(1.0);
		TAST(GAIA::MATH::gequal(f64, 1.0));
	}
}
