#ifndef		__GAIA_MATH_CONVERT_H__
#define		__GAIA_MATH_CONVERT_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_constant.h"

namespace GAIA
{
	namespace MATH
	{
		/*!
			@brief Convert radian to degree.
		*/
		template<typename _DataType> _DataType degree(const _DataType& radian){return radian / GAIA::MATH::PI * (_DataType)180;}

		/*!
			@brief Convert degree to radian.
		*/
		template<typename _DataType> _DataType radian(const _DataType& degree){return degree / (_DataType)180 * GAIA::MATH::PI;}
	}
}

#endif
