#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_argb(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::MATH::ARGB<GAIA::REAL> __ARGBType;
		typedef GAIA::MATH::ARGB<GAIA::N16> __ARGBN16Type;

		__ARGBType argb;
		if(argb.size() != 4)
			TERROR;
		argb.a = 1;
		argb.r = 2;
		argb.g = 3;
		argb.b = 4;
		__ARGBType argb1;
		argb1 = __ARGBType(1, 2, 3, 4);
		if(argb != argb1)
			TERROR;
		__ARGBN16Type argbn16;
		argbn16 = __ARGBN16Type(1, 2, 3, 4);
		argb = (__ARGBN16Type::_datatype*)&argbn16;
		if(argb != argb1)
			TERROR;
		TAST(GAIA::MATH::gequal(argb.lengthsq(), 2 * 2 + 3 * 3 + 4 * 4));
		TAST(GAIA::MATH::gequal(argb.length(), GAIA::MATH::gsqrt(GAIA::REAL(2 * 2 + 3 * 3 + 4 * 4))));
		if(argb.avg_argb() != (__ARGBType::_datatype)2.5)
			TERROR;
		if(argb.avg_rgb() != (__ARGBType::_datatype)3)
			TERROR;
		if(argb.max_argb() != (__ARGBType::_datatype)4)
			TERROR;
		if(argb.min_argb() != (__ARGBType::_datatype)1)
			TERROR;
		if(argb.max_rgb() != (__ARGBType::_datatype)4)
			TERROR;
		if(argb.min_rgb() != (__ARGBType::_datatype)2)
			TERROR;
		argbn16 = __ARGBN16Type(1, 2, 3, 4);
		argb = (__ARGBN16Type::_datatype*)&argbn16;
		TAST(argb == argbn16);
		if(argb != argbn16)
			TERROR;
		TAST(argb >= argbn16);
		TAST(argb <= argbn16);
		if(argb > argbn16)
			TERROR;
		if(argb > argbn16)
			TERROR;
		argb = 1;
		if(argb == argb1)
			TERROR;
		TAST(argb != argb1);
		if(argb >= argb1)
			TERROR;
		TAST(argb <= argb1);
		if(argb > argb1)
			TERROR;
		TAST(argb < argb1);
		argb = 0.0F;
		if(argb != 0.0F)
			TERROR;
		TAST(argb.iszero());
		TAST(argb.isfinited());
		argb /= 0.0F;
		if(argb.isfinited())
			TERROR;
		argb.fromu32(0xFF010203);
		if(argb.tou32() != 0xFF010203)
			TERROR;
	}
}
