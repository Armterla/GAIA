#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_basic(GAIA::LOG::Log& logobj)
	{
		if(GAIA::MATH::gabs(-1.23456F) != 1.23456F)
			TERROR;
		if(GAIA::MATH::gabs(1.23456F) != 1.23456F)
			TERROR;
		if(GAIA::MATH::gabs(-1) != 1)
			TERROR;
		if(GAIA::MATH::gsign(-1.23456F) != -1.0F)
			TERROR;
		if(GAIA::MATH::gsign(1.23456F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gsign(-1) != -1)
			TERROR;
		if(GAIA::MATH::gsign(1) != 1)
			TERROR;
		if(GAIA::MATH::gclamp(0.5F, 0.0F, 1.0F) != 0.5F)
			TERROR;
		if(GAIA::MATH::gclamp(-0.5F, 0.0F, 1.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gclamp(1.5F, 0.0F, 1.0F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gsaturate(0.5F) != 0.5F)
			TERROR;
		if(GAIA::MATH::gsaturate(-0.5F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gsaturate(1.5F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gfloor(0.5F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gfloor(1.1F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gfloor(0) != 0)
			TERROR;
		if(GAIA::MATH::gfloor(1) != 1)
			TERROR;
		if(GAIA::MATH::gfloor(-1) != -1)
			TERROR;
		if(GAIA::MATH::gceil(0.5F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gceil(1.1F) != 2.0F)
			TERROR;
		if(GAIA::MATH::gceil(0) != 0)
			TERROR;
		if(GAIA::MATH::gceil(1) != 1)
			TERROR;
		if(GAIA::MATH::gceil(-1) != -1)
			TERROR;
		TAST(GAIA::MATH::gequal(-1, -1));
		TAST(GAIA::MATH::gequal(-1.234567F, -1.234567));
		TAST(GAIA::MATH::gequal(-1.234567F, -1.234567F));
		TAST(GAIA::MATH::gequal(-1.23456789F, -1.23456788F));
		TAST(GAIA::MATH::gequal(-1, -1, 0.00001F));
		TAST(GAIA::MATH::gequal(-1.234567F, -1.234567, 0.00001F));
		TAST(GAIA::MATH::gequal(-1.234567F, -1.234567F, 0.00001F));
		TAST(GAIA::MATH::gequal(-1.23456789F, -1.23456788F, 0.00001F));
		if(GAIA::MATH::grealleft(1234.567) != 1234.0)
			TERROR;
		TAST(GAIA::MATH::gequal(GAIA::MATH::grealright(1234.567), 0.567));
		if(GAIA::MATH::grealleft(-1234.567) != -1234.0)
			TERROR;
		TAST(GAIA::MATH::gequal(GAIA::MATH::grealright(-1234.567), -0.567));
		if(GAIA::MATH::ground(3) != 3)
			TERROR;
		if(GAIA::MATH::ground(-3) != -3)
			TERROR;
		if(GAIA::MATH::ground(-1.51) != -2.0)
			TERROR;
		if(GAIA::MATH::ground(-1.49) != -1.0)
			TERROR;
		if(GAIA::MATH::ground(1.51) != 2.0)
			TERROR;
		if(GAIA::MATH::ground(1.49) != 1.0)
			TERROR;
		if(GAIA::MATH::gmod(13, 5) != 3)
			TERROR;
		if(GAIA::MATH::gmod(-13, 5) != -3)
			TERROR;
		TAST(GAIA::MATH::gequal(GAIA::MATH::gmod(12.345, 2.0), 0.345));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gmod(-12.345, 2.0), -0.345));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gmod(-12.345, -2.0), -0.345));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gmod(12.345, 0.02), 0.005));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gmod(-12.345, 0.02), -0.005));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gmod(-12.345, -0.02), -0.005));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gto360(360.0F * 4.0F + 12.345F), 12.345F, 0.001F));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gto360(360.0F * 4.0F - 12.345F), 360.0F - 12.345F, 0.001F));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gto360(-360.0F * 4.0F - 12.345F), 360.0F - 12.345F, 0.001F));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gto360(-360.0F * 4.0F + 12.345F), 12.345F, 0.001F));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gto2pi(GAIA::MATH::PI * 2.0F * 4.0F + 0.123), 0.123, 0.001F));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gto2pi(GAIA::MATH::PI * 2.0F * 4.0F - 0.123), GAIA::MATH::PI * 2.0F - 0.123, 0.001F));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gto2pi(-GAIA::MATH::PI * 2.0F * 4.0F - 0.123), GAIA::MATH::PI * 2.0F - 0.123, 0.001F));
		TAST(GAIA::MATH::gequal(GAIA::MATH::gto2pi(-GAIA::MATH::PI * 2.0F * 4.0F + 0.123), 0.123, 0.001F));
		if(GAIA::MATH::gcos(0.0F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gsin(0.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gacos(0.0F) != GAIA::MATH::PI * 0.5F)
			TERROR;
		if(GAIA::MATH::gasin(0.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gtan(0.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gsinh(0.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gcosh(0.0F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gatan(0.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gatan2(0.0F, 1.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gcosh(0.0F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gpow(0.0F, 1.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::gpow(0.0F, 0.0F) != 1.0F)
			TERROR;
		if(GAIA::MATH::gsqrt(1.0F) != 1.0F)
			TERROR;
		if(GAIA::MATH::glog(1.0F) != 0.0F)
			TERROR;
		if(GAIA::MATH::glog10(1.0F) != 0.0F)
			TERROR;
		TAST(GAIA::MATH::gfinite(1.0));
		TAST(GAIA::MATH::gfinite(1));
	}
}
