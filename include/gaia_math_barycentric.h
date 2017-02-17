#ifndef		__GAIA_MATH_BARYCENTRIC_H__
#define		__GAIA_MATH_BARYCENTRIC_H__

namespace GAIA
{
	namespace MATH
	{
		template<
			typename _PointDataType,
			typename _WeightDataType>
		GAIA::GVOID barycentric_weight(
			const _PointDataType* p,
			const _PointDataType* pA,
			const _PointDataType* pB,
			const _PointDataType* pC,
			_WeightDataType* pResult)
		{
			pResult[2] = (p[0] - pC[0]) / (pB[0] - pC[0]) - (p[1] - pC[1]) / (pB[1] - pC[1]);
			pResult[2] /= (pA[0] - pC[0]) / (pB[0] - pC[0]) - (pA[1] - pC[1]) / (pB[1] - pC[1]);
			pResult[0] = (p[0] - pC[0]) / (pB[0] - pC[0]) - (pA[0] - pC[0]) / (pB[0] - pC[0]) * pResult[2];
			pResult[1] = (_WeightDataType)1 - pResult[2] - pResult[0];
		}
	}
}

#endif
