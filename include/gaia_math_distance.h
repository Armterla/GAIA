#ifndef		__GAIA_MATH_DISTANCE_H__
#define		__GAIA_MATH_DISTANCE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _PointDataType, typename _LineBeginDataType, typename _LineEndDataType, typename _ResultDataType> GAIA::GVOID dist2_pos_line(const _PointDataType* pPos, const _LineBeginDataType* pLineBegin, const _LineEndDataType* pLineEnd, _ResultDataType& res)
		{
		}
		template<typename _PointDataType, typename _LineBeginDataType, typename _LineEndDataType, typename _ResultDataType> GAIA::GVOID dist3_pos_line(const _PointDataType* pPos, const _LineBeginDataType* pLineBegin, const _LineEndDataType* pLineEnd, _ResultDataType& res)
		{
			_LineBeginDataType vecPosToBegin[3], vecPosToEnd[3], vecEndToBegin[3];
			GAIA::MATH::sub3(pLineBegin, pPos, vecPosToBegin);
			GAIA::MATH::sub3(pLineEnd, pPos, vecPosToEnd);
			GAIA::MATH::sub3(pLineBegin, pLineEnd, vecEndToBegin);

			_LineBeginDataType vecNorm[3];
			GAIA::MATH::cross3(vecPosToBegin, vecPosToEnd, vecNorm);

			_LineBeginDataType area;
			GAIA::MATH::length3(vecNorm, area);

			_LineBeginDataType len;
			GAIA::MATH::length3(vecEndToBegin, len);

			res = (_ResultDataType)(area / len);
		}
		template<typename _PointDataType, typename _PlaneDataType, typename _ResultDataType> GAIA::GVOID dist3_pos_plane(const _PointDataType* pPos, const _PlaneDataType* pPlane, _ResultDataType& res)
		{
			GAIA::MATH::dot34(pPos, pPlane, res);
		}
	}
}

#endif
