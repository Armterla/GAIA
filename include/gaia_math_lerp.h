#ifndef 	__GAIA_MATH_LERP_H__
#define 	__GAIA_MATH_LERP_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _StartDataType, typename _EndDataType, typename _WeightDataType> _StartDataType lerp(const _StartDataType& start, const _EndDataType& end, const _WeightDataType& s){return start + s * (end - start);}
		template<typename _StartDataType, typename _EndDataType, typename _WeightDataType, typename _ResultDataType> GAIA::GVOID lerp_sphere(const _StartDataType* start, const _EndDataType* end, const _WeightDataType& s, _ResultDataType* pResult){}
		template<typename _DataType> _DataType lerpfactor_cos(_DataType s){return ((_DataType)1 - GAIA::MATH::gcos(s * GAIA::MATH::PI)) / (_DataType)2;}
		template<typename _DataType> _DataType lerpfactor_hermite(_DataType s){return (_DataType)-2 * s * s * s + (_DataType)3 * s * s;}
	}
}

#endif
