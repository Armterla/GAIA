#ifndef		__GAIA_MATH_LIMIT_H__
#define		__GAIA_MATH_LIMIT_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _PointDataType, typename _AABRDataType, typename _ResultDataType> GAIA::GVOID nearest_point_vec2_aabr(const _PointDataType* pPos, const _AABRDataType* pAABR, _ResultDataType* res)
		{
			res[0] = pPos[0];
			res[1] = pPos[1];

			if(res[0] < pAABR[0])
				res[0] = pAABR[0];
			else if(res[0] > pAABR[2])
				res[0] = pAABR[2];

			if(res[1] < pAABR[1])
				res[1] = pAABR[1];
			else if(res[1] > pAABR[3])
				res[1] = pAABR[3];
		}
		template<typename _PointDataType, typename _AABBDataType, typename _ResultDataType> GAIA::GVOID nearest_point_vec3_aabb(const _PointDataType* pPos, const _AABBDataType* pAABB, _ResultDataType* res)
		{
			res[0] = pPos[0];
			res[1] = pPos[1];
			res[2] = pPos[2];

			if(res[0] < pAABB[0])
				res[0] = pAABB[0];
			else if(res[0] > pAABB[3])
				res[0] = pAABB[3];

			if(res[1] < pAABB[1])
				res[1] = pAABB[1];
			else if(res[1] > pAABB[4])
				res[1] = pAABB[4];

			if(res[2] < pAABB[2])
				res[2] = pAABB[2];
			else if(res[2] > pAABB[5])
				res[2] = pAABB[5];
		}
		template<typename _PointDataType, typename _AABRDataType, typename _ResultDataType> GAIA::GVOID nearest_distancesq_vec2_aabr(const _PointDataType* pPos, const _AABRDataType* pAABR, _ResultDataType& res)
		{
			_PointDataType pt[2];
			GAIA::MATH::nearest_point_vec2_aabr(pPos, pAABR, pt);
			GAIA::MATH::sub2(pt, pPos, pt);
			GAIA::MATH::lengthsq2(pt, res);
		}
		template<typename _PointDataType, typename _AABBDataType, typename _ResultDataType> GAIA::GVOID nearest_distancesq_vec3_aabb(const _PointDataType* pPos, const _AABBDataType* pAABB, _ResultDataType& res)
		{
			_PointDataType pt[3];
			GAIA::MATH::nearest_point_vec3_aabb(pPos, pAABB, pt);
			GAIA::MATH::sub3(pt, pPos, pt);
			GAIA::MATH::lengthsq3(pt, res);
		}
		template<typename _AABRDataType1, typename _AABRDataType2, typename _ResultDataType> GAIA::GVOID nearest_distancesq_aabr(const _AABRDataType1* pSrcAABR, const _AABRDataType2* pDstAABR, _ResultDataType& res)
		{
		}
		template<typename _AABBDataType1, typename _AABBDataType2, typename _ResultDataType> GAIA::GVOID nearest_distancesq_aabb(const _AABBDataType1* pSrcAABR, const _AABBDataType2* pDstAABR, _ResultDataType& res)
		{
		}
		template<typename _PointDataType, typename _AABRDataType, typename _ResultDataType> GAIA::GVOID nearest_distance_vec2_aabr(const _PointDataType* pPos, const _AABRDataType* pAABR, _ResultDataType& res)
		{
			GAIA::MATH::nearest_distancesq_vec2_aabr(pPos, pAABR, res);
			res = GAIA::MATH::gsqrt(res);
		}
		template<typename _PointDataType, typename _AABBDataType, typename _ResultDataType> GAIA::GVOID nearest_distance_vec3_aabb(const _PointDataType* pPos, const _AABBDataType* pAABB, _ResultDataType& res)
		{
			GAIA::MATH::nearest_distancesq_vec3_aabb(pPos, pAABB, res);
			res = GAIA::MATH::gsqrt(res);
		}
		template<typename _AABRDataType1, typename _AABRDataType2, typename _ResultDataType> GAIA::GVOID nearest_distance_aabr(const _AABRDataType1* pSrcAABR, const _AABRDataType2* pDstAABR, _ResultDataType& res)
		{
		}
		template<typename _AABBDataType1, typename _AABBDataType2, typename _ResultDataType> GAIA::GVOID nearest_distance_aabb(const _AABBDataType1* pSrcAABR, const _AABBDataType2* pDstAABR, _ResultDataType& res)
		{
		}
	}
}

#endif
