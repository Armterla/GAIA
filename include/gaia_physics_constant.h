#ifndef		__GAIA_PHYSICS_CONSTANTS_H__
#define		__GAIA_PHYSICS_CONSTANTS_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace PHYS
	{
		static const GAIA::F64 EARTH_MIN_RADIUS = 6356755.0;
		static const GAIA::F64 EARTH_MAX_RADIUS = 6378136.49;
		static const GAIA::F64 EARTH_AVG_RADIUS = 6371001.0;
		static const GAIA::F64 EARTH_AVG_DENSITY = 5515.0;
		static const GAIA::F64 EARTH_MASS = 5.974E+24;
		static const GAIA::F64 EARTH_LEAVE_SPEED = 7900.0;

		static const GAIA::F64 SUN_MIN_RADIUS = 696000000.0;
		static const GAIA::F64 SUN_MAX_RADIUS = 696000000.0;
		static const GAIA::F64 SUN_AVG_RADIUS = 696000000.0;
		static const GAIA::F64 SUN_AVG_DENSITY = 1409.0;
		static const GAIA::F64 SUN_MASS = 1.989E+30;
		static const GAIA::F64 SUN_LEAVE_SPEED = 11190.0;

		static const GAIA::F64 MOON_MIN_RADIUS = 1736800.0;
		static const GAIA::F64 MOON_MAX_RADIUS = 1738000.0;
		static const GAIA::F64 MOON_AVG_RADIUS = 1737400.0;
		static const GAIA::F64 MOON_AVG_DENSITY = 334.0;
		static const GAIA::F64 MOON_MASS = 7.349E+22;
		static const GAIA::F64 MOON_LEAVE_SPEED = 2380.0;
	}
}

#endif
