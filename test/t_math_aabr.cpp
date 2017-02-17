#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_aabr(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::MATH::AABR<GAIA::REAL> __AABRType;
		typedef GAIA::MATH::AABR<GAIA::N16> __AABRTypeN16;
		__AABRType aabr;
		if(aabr.size() != 4)
			TERROR;
		aabr.identity();
		TAST(aabr.isidentity());
		__AABRType::_vectype center = aabr.center();
		if(center != (__AABRType::_datatype)0)
			TERROR;
		__AABRType::_vectype longsize = aabr.long_size();
		if(longsize.x >= (__AABRType::_datatype)0 ||
			longsize.y >= (__AABRType::_datatype)0)
			TERROR;
		aabr.expand((__AABRType::_vectype)+10);
		aabr.expand((__AABRType::_vectype)-10);
		center = aabr.center();
		if(center != (__AABRType::_datatype)0)
			TERROR;
		longsize = aabr.long_size();
		if(longsize != (__AABRType::_datatype)20)
			TERROR;
		TAST(GAIA::MATH::gequal(aabr.long_radius(), GAIA::MATH::gsqrt(200.0)));
		if(aabr.short_radius() != (__AABRType::_datatype)10)
			TERROR;
		if(aabr.width() != (__AABRType::_datatype)20)
			TERROR;
		if(aabr.height() != (__AABRType::_datatype)20)
			TERROR;
		aabr += (__AABRType::_vectype)10;
		if(aabr.pmin != (__AABRType::_vectype)0)
			TERROR;
		aabr -= (__AABRType::_vectype)10;
		aabr *= (__AABRType::_vectype)10;
		aabr /= (__AABRType::_vectype)10;
		center = aabr.center();
		if(center != (__AABRType::_datatype)0)
			TERROR;
		longsize = aabr.long_size();
		if(longsize != (__AABRType::_datatype)20)
			TERROR;
		TAST(GAIA::MATH::gequal(aabr.long_radius(), GAIA::MATH::gsqrt(200.0)));
		if(aabr.short_radius() != (__AABRType::_datatype)10)
			TERROR;
		if(aabr.width() != (__AABRType::_datatype)20)
			TERROR;
		if(aabr.height() != (__AABRType::_datatype)20)
			TERROR;
		aabr = aabr + (__AABRType::_vectype)10;
		if(aabr.pmin != (__AABRType::_vectype)0)
			TERROR;
		aabr = aabr - (__AABRType::_vectype)10;
		aabr = aabr * (__AABRType::_vectype)10;
		aabr = aabr / (__AABRType::_vectype)10;
		center = aabr.center();
		if(center != (__AABRType::_datatype)0)
			TERROR;
		longsize = aabr.long_size();
		if(longsize != (__AABRType::_datatype)20)
			TERROR;
		TAST(GAIA::MATH::gequal(aabr.long_radius(), GAIA::MATH::gsqrt(200.0)));
		if(aabr.short_radius() != (__AABRType::_datatype)10)
			TERROR;
		if(aabr.width() != (__AABRType::_datatype)20)
			TERROR;
		if(aabr.height() != (__AABRType::_datatype)20)
			TERROR;
		__AABRTypeN16 aabr16 = aabr;
		TAST(aabr16 == aabr);
		if(aabr16 != aabr)
			TERROR;
		TAST(aabr16 <= aabr);
		TAST(aabr16 >= aabr);
		if(aabr16 < aabr)
			TERROR;
		if(aabr16 > aabr)
			TERROR;
		aabr16 = 10;
		if(aabr16 == aabr)
			TERROR;
		TAST(aabr16 != aabr);
		if(aabr16 <= aabr)
			TERROR;
		TAST(aabr16 >= aabr);
		if(aabr16 < aabr)
			TERROR;
		TAST(aabr16 > aabr);
		aabr16 = (__AABRType::_datatype*)&aabr.pmin;
		TAST(aabr16 == aabr);
		if(aabr16 != aabr)
			TERROR;
		TAST(aabr16 <= aabr);
		TAST(aabr16 >= aabr);
		if(aabr16 < aabr)
			TERROR;
		if(aabr16 > aabr)
			TERROR;
		if(aabr[0] != (__AABRType::_datatype)-10)
			TERROR;
		if(aabr[1] != (__AABRType::_datatype)-10)
			TERROR;
		if(aabr[2] != (__AABRType::_datatype)+10)
			TERROR;
		if(aabr[3] != (__AABRType::_datatype)+10)
			TERROR;
		aabr = 0.0F;
		if(aabr != 0.0F)
			TERROR;
		TAST(aabr.iszero());
		TAST(aabr.isfinited());
		aabr.pmax /= 0.0F;
		if(aabr.isfinited())
			TERROR;
		aabr.identity();
		aabr.expand((__AABRType::_vectype)-1.0F);
		aabr.expand((__AABRType::_vectype)+1.0F);
		__AABRType::_vectype pos = 0.5F;
		aabr.project(pos);
		if(!GAIA::MATH::gequalzero(pos.x) || 
			!GAIA::MATH::gequalzero(pos.y))
			TERROR;
		aabr.unproject(pos);
		if(!GAIA::MATH::gequal(pos.x, 0.5F) || 
			!GAIA::MATH::gequal(pos.y, 0.5F))
			TERROR;
	}
}
