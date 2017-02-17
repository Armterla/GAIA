#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_nrgb(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::MATH::NRGB<GAIA::REAL> __NRGBType;
		typedef GAIA::MATH::NRGB<GAIA::N16> __NRGBN16Type;

		__NRGBType nrgb;
		if(nrgb.size() != 3)
			TERROR;
		nrgb.r = 2;
		nrgb.g = 3;
		nrgb.b = 4;
		__NRGBType nrgb1;
		nrgb1 = __NRGBType(2, 3, 4);
		if(nrgb != nrgb1)
			TERROR;
		__NRGBN16Type nrgbn16;
		nrgbn16 = __NRGBN16Type(2, 3, 4);
		nrgb = (__NRGBN16Type::_datatype*)&nrgbn16;
		if(nrgb != nrgb1)
			TERROR;
		TAST(GAIA::MATH::gequal(nrgb.lengthsq(), 2 * 2 + 3 * 3 + 4 * 4));
		TAST(GAIA::MATH::gequal(nrgb.length(), GAIA::MATH::gsqrt(GAIA::REAL(2 * 2 + 3 * 3 + 4 * 4))));
		if(nrgb.avg_rgb() != (__NRGBType::_datatype)3)
			TERROR;
		if(nrgb.max_rgb() != (__NRGBType::_datatype)4)
			TERROR;
		if(nrgb.min_rgb() != (__NRGBType::_datatype)2)
			TERROR;
		nrgbn16 = __NRGBN16Type(2, 3, 4);
		nrgb = (__NRGBN16Type::_datatype*)&nrgbn16;
		TAST(nrgb == nrgbn16);
		if(nrgb != nrgbn16)
			TERROR;
		TAST(nrgb >= nrgbn16);
		TAST(nrgb <= nrgbn16);
		if(nrgb > nrgbn16)
			TERROR;
		if(nrgb > nrgbn16)
			TERROR;
		nrgb = 1;
		if(nrgb == nrgb1)
			TERROR;
		TAST(nrgb != nrgb1);
		if(nrgb >= nrgb1)
			TERROR;
		TAST(nrgb <= nrgb1);
		if(nrgb > nrgb1)
			TERROR;
		TAST(nrgb < nrgb1);
		nrgb = 0.0F;
		if(nrgb != 0.0F)
			TERROR;
		TAST(nrgb.iszero());
		TAST(nrgb.isfinited());
		nrgb /= 0.0F;
		if(nrgb.isfinited())
			TERROR;

		nrgb.fromu32(0x00010203);
		if(nrgb.tou32() != 0x00010203)
			TERROR;
	}
}
