#ifndef		__GAIA_MATH_TRANSFORM_H__
#define		__GAIA_MATH_TRANSFORM_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _EulerAngleDataType, typename _QuaDataType> GAIA::GVOID compose_qua(const _EulerAngleDataType& x, const _EulerAngleDataType& y, const _EulerAngleDataType& z, _QuaDataType* pResult)
		{
		}
		template<typename _QuaDataType, typename _EulerAngleDataType> GAIA::GVOID decompose_qua(const _QuaDataType* pQua, _EulerAngleDataType& x, _EulerAngleDataType& y, _EulerAngleDataType& z)
		{
			_QuaDataType sp = (_QuaDataType)-2 * (pQua[1] * pQua[2] - pQua[3] * pQua[0]);

			if(GAIA::MATH::gabs(sp) > (_QuaDataType)0.9999)
			{
				x = (_EulerAngleDataType)(GAIA::MATH::PI * (_QuaDataType)0.5 * sp);
				y = (_EulerAngleDataType)(GAIA::MATH::gatan2(-pQua[0] * pQua[2] + pQua[3] * pQua[1], (_QuaDataType)0.5 - pQua[1] * pQua[1] - pQua[2] * pQua[2]));
				z = (_EulerAngleDataType)0;
			}
			else
			{
				x = (_EulerAngleDataType)(GAIA::MATH::gasin(GAIA::MATH::gclamp(sp, (_QuaDataType)-1, (_QuaDataType)+1)));
				y = (_EulerAngleDataType)(GAIA::MATH::gatan2(pQua[0] * pQua[2] + pQua[3] * pQua[1], (_QuaDataType)0.5 - pQua[0] * pQua[0] - pQua[1] * pQua[1]));
				z = (_EulerAngleDataType)(GAIA::MATH::gatan2(pQua[0] * pQua[1] + pQua[3] * pQua[2], (_QuaDataType)0.5 - pQua[0] * pQua[0] - pQua[2] * pQua[2]));
			}
		}
		template<typename _EulerAngleDataType, typename _QuaDataType> GAIA::GVOID compose_qua(const _EulerAngleDataType* pEulerAngle, _QuaDataType* pResult)
		{
			GAIA::MATH::compose_qua(pEulerAngle[0], pEulerAngle[1], pEulerAngle[2], pResult);
		}
		template<typename _QuaDataType, typename _EulerAngleDataType> GAIA::GVOID decompose_qua(const _QuaDataType* pQua, _EulerAngleDataType* pResult)
		{
			GAIA::MATH::decompose_qua(pQua, pResult[0], pResult[1], pResult[2]);
		}
		template<typename _DataType, typename _MtxDataType> GAIA::GVOID compose_mtx44(
			const _DataType& tx, const _DataType& ty, const _DataType& tz,
			const _DataType& rx, const _DataType& ry, const _DataType& rz,
			const _DataType& sx, const _DataType& sy, const _DataType& sz,
			_MtxDataType* pResult)
		{
		}
		template<typename _MtxDataType, typename _DataType> GAIA::GVOID decompose_mtx44(
			const _MtxDataType* pMtx,
			_DataType& tx, _DataType& ty, _DataType& tz,
			_DataType& rx, _DataType& ry, _DataType& rz,
			_DataType& sx, _DataType& sy, _DataType& sz)
		{
		}
		template<typename _DataType, typename _MtxDataType> GAIA::GVOID compose_mtx44(
			const _DataType* pLocation,
			_MtxDataType* pResult)
		{
			GAIA::MATH::compose_mtx44(
				pLocation[0], pLocation[1], pLocation[2],
				pLocation[3], pLocation[4], pLocation[5],
				pLocation[6], pLocation[7], pLocation[8],
				pResult);
		}
		template<typename _MtxDataType, typename _DataType> GAIA::GVOID decompose_mtx44(
			const _MtxDataType* pMtx,
			_DataType* pLocation)
		{
			GAIA::MATH::decompose_mtx44(
				pMtx,
				pLocation[0], pLocation[1], pLocation[2],
				pLocation[3], pLocation[4], pLocation[5],
				pLocation[6], pLocation[7], pLocation[8]);
		}
	}
}

#endif
