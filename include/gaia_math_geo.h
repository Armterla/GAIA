#ifndef		__GAIA_MATH_GEO_H__
#define		__GAIA_MATH_GEO_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"

namespace GAIA
{
	namespace MATH
	{
		template<
			typename _PointBeginDataType,
			typename _PointEndDataType,
			typename _LineDataType>
		GAIA::BL construct_line2_point2(
			const _PointBeginDataType* pPointBegin,
			const _PointEndDataType* pPointEnd,
			_LineDataType* pResultLine)
		{
			return GAIA::True;
		}

		template<
			typename _Point1DataType,
			typename _Point2DataType,
			typename _Point3DataType,
			typename _PlaneDataType>
			GAIA::BL construct_plane_point3(
			const _Point1DataType* pPoint1,
			const _Point2DataType* pPoint2,
			const _Point3DataType* pPoint3,
			_PlaneDataType* pResultPlane)
		{
			return GAIA::True;
		}
	}
}

#endif
