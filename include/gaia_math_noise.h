#ifndef		__GAIA_MATH_NOIST_H__
#define		__GAIA_MATH_NOIST_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> GAIA::GVOID noise_integer(GAIA::N32 n, _DataType& res)
		{
			n = (n >> 13) ^ n;
			GAIA::N32 k = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7FFFFFFF;
			return (_DataType)1 - ((_DataType)k / 1073741824);
		}

		template<typename _DataType> _DataType noise_coherent(_DataType t)
		{
			GAIA::N32 x = (GAIA::N32)(GAIA::MATH::gfloor(t));

			_DataType t0, t1;
			noise_integer(x, t0);
			noise_integer(x + 1, t1);

			_DataType weight = t - GAIA::MATH::gfloor(t);
			return GAIA::MATH::lerp(t0, t1, weight);
		}

		template<typename _DataType> _DataType noise_coherent_hermite(_DataType t)
		{
			GAIA::N32 x = (GAIA::N32)(GAIA::MATH::gfloor(t));

			_DataType t0, t1;
			noise_integer(x, t0);
			noise_integer(x + 1, t1);

			_DataType weight = t - GAIA::MATH::gfloor(t);
			return GAIA::MATH::lerp(t0, t1, GAIA::MATH::lerpfactor_hermite(weight));
		}
	}
}

#endif
