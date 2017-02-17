#ifndef		__GAIA_MATH_SPLINE_H__
#define		__GAIA_MATH_SPLINE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType, typename _WeightDataType, typename _ResultDataType> GAIA::GVOID hermite2_pos(const _DataType* pBeginPos, const _DataType* pEndPos, const _DataType* pBeginDir, const _DataType* pEndDir, _WeightDataType weight, _ResultDataType* pResultPos)
		{
			_DataType w2 = weight * weight;
			_DataType w3 = w2 * weight;

			_DataType a = 2 * w3 - 3 * w2 + 1;
			_DataType b = w3 - 2 * w2 + weight;
			_DataType c = w3 - w2;
			_DataType d = -2 * w3 + 3 * w2;

			_DataType ca[2], cb[2], cc[2], cd[2];
			GAIA::MATH::mul2(pBeginPos, a, ca);
			GAIA::MATH::mul2(pBeginDir, b, cb);
			GAIA::MATH::mul2(pEndDir, c, cc);
			GAIA::MATH::mul2(pEndPos, d, cd);
			GAIA::MATH::add2(ca, cb, pResultPos);
			GAIA::MATH::add2(cc, pResultPos, pResultPos);
			GAIA::MATH::add2(cd, pResultPos, pResultPos);
		}
		template<typename _DataType, typename _WeightDataType, typename _ResultDataType> GAIA::GVOID hermite2_dir(const _DataType* pBeginPos, const _DataType* pEndPos, const _DataType* pBeginDir, const _DataType* pEndDir, _WeightDataType weight, _ResultDataType* pResultDir)
		{
			_DataType w2 = weight * weight;

			_DataType a = 6 * w2 - 6 * weight;
			_DataType b = 3 * w2 - 4 * weight + 1;
			_DataType c = 3 * w2 - 2 * weight;
			_DataType d = -6 * w2 + 6 * weight;

			_DataType ca[2], cb[2], cc[2], cd[2];
			GAIA::MATH::mul2(pBeginPos, a, ca);
			GAIA::MATH::mul2(pBeginDir, b, cb);
			GAIA::MATH::mul2(pEndDir, c, cc);
			GAIA::MATH::mul2(pEndPos, d, cd);
			GAIA::MATH::add2(ca, cb, pResultDir);
			GAIA::MATH::add2(cc, pResultDir, pResultDir);
			GAIA::MATH::add2(cd, pResultDir, pResultDir);
		}
		template<typename _DataType, typename _WeightDataType, typename _ResultDataType> GAIA::GVOID hermite3_pos(const _DataType* pBeginPos, const _DataType* pEndPos, const _DataType* pBeginDir, const _DataType* pEndDir, _WeightDataType weight, _ResultDataType* pResultPos)
		{
			_DataType w2 = weight * weight;
			_DataType w3 = w2 * weight;

			_DataType a = 2 * w3 - 3 * w2 + 1;
			_DataType b = w3 - 2 * w2 + weight;
			_DataType c = w3 - w2;
			_DataType d = -2 * w3 + 3 * w2;

			_DataType ca[3], cb[3], cc[3], cd[3];
			GAIA::MATH::mul3(pBeginPos, a, ca);
			GAIA::MATH::mul3(pBeginDir, b, cb);
			GAIA::MATH::mul3(pEndDir, c, cc);
			GAIA::MATH::mul3(pEndPos, d, cd);
			GAIA::MATH::add3(ca, cb, pResultPos);
			GAIA::MATH::add3(cc, pResultPos, pResultPos);
			GAIA::MATH::add3(cd, pResultPos, pResultPos);
		}
		template<typename _DataType, typename _WeightDataType, typename _ResultDataType> GAIA::GVOID hermite3_dir(const _DataType* pBeginPos, const _DataType* pEndPos, const _DataType* pBeginDir, const _DataType* pEndDir, _WeightDataType weight, _ResultDataType* pResultDir)
		{
			_DataType w2 = weight * weight;

			_DataType a = 6 * w2 - 6 * weight;
			_DataType b = 3 * w2 - 4 * weight + 1;
			_DataType c = 3 * w2 - 2 * weight;
			_DataType d = -6 * w2 + 6 * weight;

			_DataType ca[3], cb[3], cc[3], cd[3];
			GAIA::MATH::mul3(pBeginPos, a, ca);
			GAIA::MATH::mul3(pBeginDir, b, cb);
			GAIA::MATH::mul3(pEndDir, c, cc);
			GAIA::MATH::mul3(pEndPos, d, cd);
			GAIA::MATH::add3(ca, cb, pResultDir);
			GAIA::MATH::add3(cc, pResultDir, pResultDir);
			GAIA::MATH::add3(cd, pResultDir, pResultDir);
		}
	}
}

#endif
