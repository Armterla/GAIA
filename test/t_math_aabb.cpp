#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_aabb(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::MATH::AABB<GAIA::REAL> __AABBType;
		typedef GAIA::MATH::AABB<GAIA::N16> __AABBTypeN16;
		__AABBType aabb;
		if(aabb.size() != 6)
			TERROR;
		aabb.identity();
		TAST(aabb.isidentity());
		__AABBType::_vectype center = aabb.center();
		if(center != (__AABBType::_datatype)0)
			TERROR;
		__AABBType::_vectype long_size = aabb.long_size();
		if(long_size.x >= (__AABBType::_datatype)0 ||
			long_size.y >= (__AABBType::_datatype)0 ||
			long_size.z >= (__AABBType::_datatype)0)
		{
			TERROR;
		}
		aabb.expand((__AABBType::_vectype)+10);
		aabb.expand((__AABBType::_vectype)-10);
		center = aabb.center();
		if(center != (__AABBType::_datatype)0)
			TERROR;
		long_size = aabb.long_size();
		if(long_size != (__AABBType::_datatype)20)
			TERROR;
		TAST(GAIA::MATH::gequal(aabb.long_radius(), GAIA::MATH::gsqrt(300.0)));
		if(aabb.short_radius() != (__AABBType::_datatype)10)
			TERROR;
		if(aabb.width() != (__AABBType::_datatype)20)
			TERROR;
		if(aabb.height() != (__AABBType::_datatype)20)
			TERROR;
		if(aabb.depth() != (__AABBType::_datatype)20)
			TERROR;
		aabb += (__AABBType::_vectype)10;
		if(aabb.pmin != (__AABBType::_vectype)0)
			TERROR;
		aabb -= (__AABBType::_vectype)10;
		aabb *= (__AABBType::_vectype)10;
		aabb /= (__AABBType::_vectype)10;
		center = aabb.center();
		if(center != (__AABBType::_datatype)0)
			TERROR;
		long_size = aabb.long_size();
		if(long_size != (__AABBType::_datatype)20)
			TERROR;
		TAST(GAIA::MATH::gequal(aabb.long_radius(), GAIA::MATH::gsqrt(300.0)));
		if(aabb.short_radius() != (__AABBType::_datatype)10)
			TERROR;
		if(aabb.width() != (__AABBType::_datatype)20)
			TERROR;
		if(aabb.height() != (__AABBType::_datatype)20)
			TERROR;
		if(aabb.depth() != (__AABBType::_datatype)20)
			TERROR;
		aabb = aabb + (__AABBType::_vectype)10;
		if(aabb.pmin != (__AABBType::_vectype)0)
			TERROR;
		aabb = aabb - (__AABBType::_vectype)10;
		aabb = aabb * (__AABBType::_vectype)10;
		aabb = aabb / (__AABBType::_vectype)10;
		center = aabb.center();
		if(center != (__AABBType::_datatype)0)
			TERROR;
		long_size = aabb.long_size();
		if(long_size != (__AABBType::_datatype)20)
			TERROR;
		TAST(GAIA::MATH::gequal(aabb.long_radius(), GAIA::MATH::gsqrt(300.0)));
		if(aabb.short_radius() != (__AABBType::_datatype)10)
			TERROR;
		if(aabb.width() != (__AABBType::_datatype)20)
			TERROR;
		if(aabb.height() != (__AABBType::_datatype)20)
			TERROR;
		if(aabb.depth() != (__AABBType::_datatype)20)
			TERROR;
		__AABBTypeN16 aabb16 = aabb;
		TAST(aabb16 == aabb);
		if(aabb16 != aabb)
			TERROR;
		TAST(aabb16 <= aabb);
		TAST(aabb16 >= aabb);
		if(aabb16 < aabb)
			TERROR;
		if(aabb16 > aabb)
			TERROR;
		aabb16 = 10;
		if(aabb16 == aabb)
			TERROR;
		TAST(aabb16 != aabb);
		if(aabb16 <= aabb)
			TERROR;
		TAST(aabb16 >= aabb);
		if(aabb16 < aabb)
			TERROR;
		TAST(aabb16 > aabb);
		aabb16 = (__AABBType::_datatype*)&aabb.pmin;
		TAST(aabb16 == aabb);
		if(aabb16 != aabb)
			TERROR;
		TAST(aabb16 <= aabb);
		TAST(aabb16 >= aabb);
		if(aabb16 < aabb)
			TERROR;
		if(aabb16 > aabb)
			TERROR;
		if(aabb[0] != (__AABBType::_datatype)-10)
			TERROR;
		if(aabb[1] != (__AABBType::_datatype)-10)
			TERROR;
		if(aabb[2] != (__AABBType::_datatype)-10)
			TERROR;
		if(aabb[3] != (__AABBType::_datatype)+10)
			TERROR;
		if(aabb[4] != (__AABBType::_datatype)+10)
			TERROR;
		if(aabb[5] != (__AABBType::_datatype)+10)
			TERROR;
		aabb = 0.0F;
		if(aabb != 0.0F)
			TERROR;
		TAST(aabb.iszero());
		TAST(aabb.isfinited());
		aabb.pmax /= 0.0F;
		if(aabb.isfinited())
			TERROR;
		aabb.identity();
		aabb.expand((__AABBType::_vectype) - 1.0F);
		aabb.expand((__AABBType::_vectype) + 1.0F);
		__AABBType::_vectype pos = 0.5F;
		aabb.project(pos);
		if(!GAIA::MATH::gequalzero(pos.x) ||
			!GAIA::MATH::gequalzero(pos.y) || 
			!GAIA::MATH::gequalzero(pos.z))
			TERROR;
		aabb.unproject(pos);
		if(!GAIA::MATH::gequal(pos.x, 0.5F) ||
			!GAIA::MATH::gequal(pos.y, 0.5F) || 
			!GAIA::MATH::gequal(pos.z, 0.5F))
			TERROR;
	}
}
