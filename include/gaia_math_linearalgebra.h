#ifndef		__GAIA_MATH_LINEARALGEBRA_H__
#define		__GAIA_MATH_LINEARALGEBRA_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType1, typename _DataType2> GAIA::BL equal2(const _DataType1* v1, const _DataType2* v2)
		{
			if(GAIA::MATH::gequal(v1[0], v2[0]) &&
				GAIA::MATH::gequal(v1[1], v2[1]))
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType1, typename _DataType2> GAIA::BL equal3(const _DataType1* v1, const _DataType2* v2)
		{
			if(GAIA::MATH::gequal(v1[0], v2[0]) &&
				GAIA::MATH::gequal(v1[1], v2[1]) &&
				GAIA::MATH::gequal(v1[2], v2[2]))
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType1, typename _DataType2> GAIA::BL equal4(const _DataType1* v1, const _DataType2* v2)
		{
			if(GAIA::MATH::gequal(v1[0], v2[0]) &&
				GAIA::MATH::gequal(v1[1], v2[1]) &&
				GAIA::MATH::gequal(v1[2], v2[2]) &&
				GAIA::MATH::gequal(v1[3], v2[3]))
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType1, typename _DataType2, typename _PrecisionDataType> GAIA::BL equal2(const _DataType1* v1, const _DataType2* v2, const _PrecisionDataType& epsilon)
		{
			if(GAIA::MATH::gequal(v1[0], v2[0], epsilon) &&
				GAIA::MATH::gequal(v1[1], v2[1], epsilon))
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType1, typename _DataType2, typename _PrecisionDataType> GAIA::BL equal3(const _DataType1* v1, const _DataType2* v2, const _PrecisionDataType& epsilon)
		{
			if(GAIA::MATH::gequal(v1[0], v2[0], epsilon) &&
				GAIA::MATH::gequal(v1[1], v2[1], epsilon) &&
				GAIA::MATH::gequal(v1[2], v2[2], epsilon))
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType1, typename _DataType2, typename _PrecisionDataType> GAIA::BL equal4(const _DataType1* v1, const _DataType2* v2, const _PrecisionDataType& epsilon)
		{
			if(GAIA::MATH::gequal(v1[0], v2[0], epsilon) &&
				GAIA::MATH::gequal(v1[1], v2[1], epsilon) &&
				GAIA::MATH::gequal(v1[2], v2[2], epsilon) &&
				GAIA::MATH::gequal(v1[3], v2[3], epsilon))
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID add2(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] + v2[0];
			pResult[1] = v1[1] + v2[1];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID add3(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] + v2[0];
			pResult[1] = v1[1] + v2[1];
			pResult[2] = v1[2] + v2[2];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID add4(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] + v2[0];
			pResult[1] = v1[1] + v2[1];
			pResult[2] = v1[2] + v2[2];
			pResult[3] = v1[3] + v2[3];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID sub2(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] - v2[0];
			pResult[1] = v1[1] - v2[1];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID sub3(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] - v2[0];
			pResult[1] = v1[1] - v2[1];
			pResult[2] = v1[2] - v2[2];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID sub4(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] - v2[0];
			pResult[1] = v1[1] - v2[1];
			pResult[2] = v1[2] - v2[2];
			pResult[3] = v1[3] - v2[3];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mul2(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] * v2[0];
			pResult[1] = v1[1] * v2[1];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mul3(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] * v2[0];
			pResult[1] = v1[1] * v2[1];
			pResult[2] = v1[2] * v2[2];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mul4(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] * v2[0];
			pResult[1] = v1[1] * v2[1];
			pResult[2] = v1[2] * v2[2];
			pResult[3] = v1[3] * v2[3];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID div2(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] / v2[0];
			pResult[1] = v1[1] / v2[1];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID div3(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] / v2[0];
			pResult[1] = v1[1] / v2[1];
			pResult[2] = v1[2] / v2[2];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID div4(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] / v2[0];
			pResult[1] = v1[1] / v2[1];
			pResult[2] = v1[2] / v2[2];
			pResult[3] = v1[3] / v2[3];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mod2(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = GAIA::MATH::gmod(v1[0], v2[0]);
			pResult[1] = GAIA::MATH::gmod(v1[1], v2[1]);
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mod3(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = GAIA::MATH::gmod(v1[0], v2[0]);
			pResult[1] = GAIA::MATH::gmod(v1[1], v2[1]);
			pResult[2] = GAIA::MATH::gmod(v1[2], v2[2]);
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mod4(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			pResult[0] = GAIA::MATH::gmod(v1[0], v2[0]);
			pResult[1] = GAIA::MATH::gmod(v1[1], v2[1]);
			pResult[2] = GAIA::MATH::gmod(v1[2], v2[2]);
			pResult[3] = GAIA::MATH::gmod(v1[3], v2[3]);
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID add2(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] + v2;
			pResult[1] = v1[1] + v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID add3(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] + v2;
			pResult[1] = v1[1] + v2;
			pResult[2] = v1[2] + v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID add4(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] + v2;
			pResult[1] = v1[1] + v2;
			pResult[2] = v1[2] + v2;
			pResult[3] = v1[3] + v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID sub2(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] - v2;
			pResult[1] = v1[1] - v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID sub3(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] - v2;
			pResult[1] = v1[1] - v2;
			pResult[2] = v1[2] - v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID sub4(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] - v2;
			pResult[1] = v1[1] - v2;
			pResult[2] = v1[2] - v2;
			pResult[3] = v1[3] - v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mul2(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] * v2;
			pResult[1] = v1[1] * v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mul3(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] * v2;
			pResult[1] = v1[1] * v2;
			pResult[2] = v1[2] * v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mul4(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] * v2;
			pResult[1] = v1[1] * v2;
			pResult[2] = v1[2] * v2;
			pResult[3] = v1[3] * v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID div2(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] / v2;
			pResult[1] = v1[1] / v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID div3(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] / v2;
			pResult[1] = v1[1] / v2;
			pResult[2] = v1[2] / v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID div4(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = v1[0] / v2;
			pResult[1] = v1[1] / v2;
			pResult[2] = v1[2] / v2;
			pResult[3] = v1[3] / v2;
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mod2(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = GAIA::MATH::gmod(v1[0], v2);
			pResult[1] = GAIA::MATH::gmod(v1[1], v2);
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mod3(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = GAIA::MATH::gmod(v1[0], v2);
			pResult[1] = GAIA::MATH::gmod(v1[1], v2);
			pResult[2] = GAIA::MATH::gmod(v1[2], v2);
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID mod4(const _DataType1* v1, const _DataType2& v2, _ResultDataType* pResult)
		{
			pResult[0] = GAIA::MATH::gmod(v1[0], v2);
			pResult[1] = GAIA::MATH::gmod(v1[1], v2);
			pResult[2] = GAIA::MATH::gmod(v1[2], v2);
			pResult[3] = GAIA::MATH::gmod(v1[3], v2);
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID dot2(const _DataType1* v1, const _DataType2* v2, _ResultDataType& res)
		{
			res = v1[0] * v2[0] + v1[1] * v2[1];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID dot3(const _DataType1* v1, const _DataType2* v2, _ResultDataType& res)
		{
			res = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID dot34(const _DataType1* v1, const _DataType2* v2, _ResultDataType& res)
		{
			res = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v2[3];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID dot4(const _DataType1* v1, const _DataType2* v2, _ResultDataType& res)
		{
			res = (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]) * v1[3] * v2[3];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID cross3(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			_ResultDataType temp[3];
			temp[0] = v1[1] * v2[2] - v1[2] * v2[1];
			temp[1] = v1[2] * v2[0] - v1[0] * v2[2];
			temp[2] = v1[0] * v2[1] - v1[1] * v2[0];
			pResult[0] = temp[0];
			pResult[1] = temp[1];
			pResult[2] = temp[2];
		}
		template<typename _DataType1, typename _DataType2, typename _ResultDataType> GAIA::GVOID cross4(const _DataType1* v1, const _DataType2* v2, _ResultDataType* pResult)
		{
			_DataType1 v11[3];
			v11[0] = v1[0] / v1[3];
			v11[1] = v1[1] / v1[3];
			v11[2] = v1[2] / v1[3];

			_DataType2 v22[3];
			v22[0] = v2[0] / v2[3];
			v22[1] = v2[1] / v2[3];
			v22[2] = v2[2] / v2[3];

			GAIA::MATH::cross3(v11, v22, pResult);
			pResult[3] = (_ResultDataType)1;
		}
		template<typename _DataType, typename _ResultDataType> GAIA::GVOID lengthsq2(const _DataType* v, _ResultDataType& res)
		{
			res = v[0] * v[0] + v[1] * v[1];
		}
		template<typename _DataType, typename _ResultDataType> GAIA::GVOID lengthsq3(const _DataType* v, _ResultDataType& res)
		{
			res = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		}
		template<typename _DataType, typename _ResultDataType> GAIA::GVOID lengthsq4(const _DataType* v, _ResultDataType& res)
		{
			res = (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) * v[3] * v[3];
		}
		template<typename _DataType, typename _ResultDataType> GAIA::GVOID length2(const _DataType* v, _ResultDataType& res)
		{
			GAIA::MATH::lengthsq2(v, res);
			res = GAIA::MATH::gsqrt(res);
		}
		template<typename _DataType, typename _ResultDataType> GAIA::GVOID length3(const _DataType* v, _ResultDataType& res)
		{
			GAIA::MATH::lengthsq3(v, res);
			res = GAIA::MATH::gsqrt(res);
		}
		template<typename _DataType, typename _ResultDataType> GAIA::GVOID length4(const _DataType* v, _ResultDataType& res)
		{
			GAIA::MATH::lengthsq4(v, res);
			res = GAIA::MATH::gsqrt(res);
		}
	}
}

#endif
