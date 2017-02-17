#ifndef		__GAIA_MATH_PROJECTION_H__
#define		__GAIA_MATH_PROJECTION_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"

namespace GAIA
{
	namespace MATH
	{
		template<
			typename _PointDataType,
			typename _PlaneDataType,
			typename _ResultDataType>
		GAIA::GVOID project_point3_plane(
			const _PointDataType* pPos,
			const _PlaneDataType* pPlane,
			_ResultDataType* pResult)
		{
			_PointDataType dist;
			GAIA::MATH::dot34(pPos, pPlane, dist);
			GAIA::MATH::mul3(pPlane, dist, pResult);
			GAIA::MATH::sub3(pPos, pResult, pResult);
		}
	}
}

#endif
