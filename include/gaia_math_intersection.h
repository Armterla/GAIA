#ifndef		__GAIA_MATH_INTERSECTION_H__
#define		__GAIA_MATH_INTERSECTION_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"

namespace GAIA
{
	namespace MATH
	{
		/* Geometry self intersection start. */
		template<
			typename _Line1BeginDataType,
			typename _Line1EndDataType,
			typename _Line2BeginDataType,
			typename _Line2EndDataType>
		GAIA::INTERSECT_TYPE intersect_line2(
				const _Line1BeginDataType* pLine1Begin,
				const _Line1EndDataType* pLine1End,
				const _Line2BeginDataType* pLine2Begin,
				const _Line2EndDataType* pLine2End)
		{
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}

		template<
			typename _Line1BeginDataType,
			typename _Line1EndDataType,
			typename _Line2BeginDataType,
			typename _Line2EndDataType>
		GAIA::INTERSECT_TYPE intersect_line3(
			const _Line1BeginDataType* pLine1Begin,
			const _Line1EndDataType* pLine1End,
			const _Line2BeginDataType* pLine2Begin,
			const _Line2EndDataType* pLine2End)
		{
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}

		template<
			typename _AABRDataType1,
			typename _AABRDataType2>
		GAIA::INTERSECT_TYPE intersect_aabr(
			const _AABRDataType1* pAABR1,
			const _AABRDataType2* pAABR2)
		{
			if(pAABR1[2] < pAABR2[0] || pAABR1[0] > pAABR2[2])
				return GAIA::INTERSECT_TYPE_OUT;
			if(pAABR1[3] < pAABR2[1] || pAABR1[1] > pAABR2[3])
				return GAIA::INTERSECT_TYPE_OUT;
			if(pAABR1[0] <= pAABR2[0] && pAABR1[1] <= pAABR2[1] &&
				pAABR1[2] >= pAABR2[2] && pAABR1[3] >= pAABR2[3])
				return GAIA::INTERSECT_TYPE_IN;
			return GAIA::INTERSECT_TYPE_INTERSECTED;
		}

		template<
			typename _AABBDataType1,
			typename _AABBDataType2>
		GAIA::INTERSECT_TYPE intersect_aabb(
			const _AABBDataType1* pAABB1,
			const _AABBDataType2* pAABB2)
		{
			if(pAABB1[3] < pAABB2[0] || pAABB1[0] > pAABB2[3])
				return GAIA::INTERSECT_TYPE_OUT;
			if(pAABB1[4] < pAABB2[1] || pAABB1[1] > pAABB2[4])
				return GAIA::INTERSECT_TYPE_OUT;
			if(pAABB1[5] < pAABB2[2] || pAABB1[2] > pAABB2[5])
				return GAIA::INTERSECT_TYPE_OUT;
			if(pAABB1[0] <= pAABB2[0] && pAABB1[1] <= pAABB2[1] && pAABB1[2] <= pAABB2[2] &&
				pAABB1[3] >= pAABB2[3] && pAABB1[4] >= pAABB2[4] && pAABB1[5] >= pAABB2[5])
				return GAIA::INTERSECT_TYPE_IN;
			return GAIA::INTERSECT_TYPE_INTERSECTED;
		}

		/* Point geometry intersect with others start. */
		template<
			typename _PointDataType,
			typename _LineBeginDataType,
			typename _LineEndDataType>
		GAIA::INTERSECT_TYPE intersect_point2_line2(
			const _PointDataType* pPoint,
			const _LineBeginDataType* pLineBegin,
			const _LineEndDataType* pLineEnd)
		{
			_LineBeginDataType dist = (pLineEnd[0] - pLineBegin[0]) * (pLineBegin[1] - pPoint[1]) -
				(pLineBegin[0] - pPoint[0]) * (pLineEnd[1] - pLineBegin[1]);
			if(dist > (_LineBeginDataType)0)
				return GAIA::INTERSECT_TYPE_OUT;
			else if(dist == (_LineBeginDataType)0)
				return GAIA::INTERSECT_TYPE_INTERSECTED;
			else
				return GAIA::INTERSECT_TYPE_IN;
		}

		template<
			typename _PointDataType,
			typename _TriangleDataType>
		GAIA::INTERSECT_TYPE intersect_point2_triangle2(
			const _PointDataType* pPoint,
			const _TriangleDataType* p0,
			const _TriangleDataType* p1,
			const _TriangleDataType* p2)
		{
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}

		template<
			typename _PointDataType,
			typename _LineBeginDataType,
			typename _LineEndDataType>
		GAIA::INTERSECT_TYPE intersect_point3_line3(
			const _PointDataType* pPoint,
			const _LineBeginDataType* pLineBegin,
			const _LineEndDataType* pLineEnd)
		{
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}

		template<
			typename _PointDataType,
			typename _PlaneDataType>
		GAIA::INTERSECT_TYPE intersect_point3_plane(
			const _PointDataType* pPoint,
			const _PlaneDataType* pPlane)
		{
			_PlaneDataType dist;
			GAIA::MATH::dot34(pPoint, pPoint, dist);
			if(dist > (_PlaneDataType)0)
				return GAIA::INTERSECT_TYPE_NOTINTERSECT;
			return INTERSECT_TYPE_INTERSECTED;
		}

		template<
			typename _PointDataType,
			typename _AABRDataType>
		GAIA::INTERSECT_TYPE intersect_point2_aabr(
			const _PointDataType* pPoint,
			const _AABRDataType* pAABR)
		{
			if(pPoint[0] >= pAABR[0] && pPoint[0] <= pAABR[2] &&
				pPoint[1] >= pAABR[1] && pPoint[1] <= pAABR[3])
				return GAIA::INTERSECT_TYPE_IN;
			return GAIA::INTERSECT_TYPE_OUT;
		}

		template<
			typename _PointDataType,
			typename _AABBDataType>
		GAIA::INTERSECT_TYPE intersect_point3_aabb(
			const _PointDataType* pPoint,
			const _AABBDataType* pAABB)
		{
			if(pPoint[0] >= pAABB[0] && pPoint[0] <= pAABB[3] &&
				pPoint[1] >= pAABB[1] && pPoint[1] <= pAABB[4] &&
				pPoint[2] >= pAABB[2] && pPoint[2] <= pAABB[5])
				return GAIA::INTERSECT_TYPE_IN;
			return GAIA::INTERSECT_TYPE_OUT;
		}

		/* Line geometry intersect with others start. */

		/* Ray geometry intersect with others start. */
		template<
			typename _PosDataType,
			typename _DirDataType,
			typename _PlaneDataType,
			typename _WeightDataType>
		GAIA::INTERSECT_TYPE intersect_ray3_plane(
			const _PosDataType* pRayPos,
			const _DirDataType* pRayDir,
			const _PlaneDataType* plane,
			_WeightDataType& d)
		{
			_PlaneDataType plane_normal[3];
			plane_normal[0] = -plane[0];
			plane_normal[1] = -plane[1];
			plane_normal[2] = -plane[2];
			_PlaneDataType ndotu;
			GAIA::MATH::dot3(pRayDir, plane_normal, ndotu);
			if(GAIA::MATH::gequalzero(ndotu))
				return GAIA::INTERSECT_TYPE_NOTINTERSECT;

			_PlaneDataType sdotu;
			GAIA::MATH::dot3(pRayPos, plane_normal, sdotu);
			d = (plane[3] - sdotu) / ndotu;
			return GAIA::INTERSECT_TYPE_INTERSECTED;
		}

		template<
			typename _PosDataType,
			typename _DirDataType,
			typename _AABRDataType>
		GAIA::INTERSECT_TYPE intersect_ray2_aabr(
			const _PosDataType* pRayPos,
			const _DirDataType* pRayDir,
			const _AABRDataType* pAABR)
		{
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}

		template<
			typename _PosDataType,
			typename _DirDataType,
			typename _AABBDataType>
		GAIA::INTERSECT_TYPE intersect_ray3_aabb(
			const _PosDataType* pRayPos,
			const _DirDataType* pRayDir,
			const _AABBDataType* pAABB)
		{
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}

		template<
			typename _PosDataType,
			typename _DirDataType,
			typename _SphereCenterDataType,
			typename _SphereRadiusDataType,
			typename _WeightDataType>
		GAIA::INTERSECT_TYPE intersect_ray3_sphere(
			const _PosDataType* pRayPos,
			const _DirDataType* pRayDir,
			const _SphereCenterDataType* pSphereCenter,
			const _SphereRadiusDataType& sphere_radius,
			_WeightDataType& d1, _WeightDataType& d2)
		{
		#if GAIA_PROFILE == GAIA_PROFILE_DEBUG
			GAIA::REAL rLen;
			GAIA::MATH::length3(pRayDir, rLen);
			GAST(GAIA::MATH::gequal(rLen, (GAIA::REAL)1));
		#endif

			d1 = (_WeightDataType)0;
			d2 = (_WeightDataType)0;

			_SphereCenterDataType e[3];
			GAIA::MATH::sub3(pSphereCenter, pRayPos, e);
			_SphereCenterDataType e2;
			GAIA::MATH::dot3(e, e, e2);
			_SphereRadiusDataType r2 = sphere_radius * sphere_radius;
			if(e2 < r2)
				return GAIA::INTERSECT_TYPE_INTERSECTED;

			_DirDataType a;
			GAIA::MATH::dot3(e, pRayDir, a);
			_DirDataType a2 = a * a;
			_SphereRadiusDataType f2 = r2 - e2 + a2;
			if(f2 < (_SphereRadiusDataType)0)
				return GAIA::INTERSECT_TYPE_NOTINTERSECT;

			_SphereRadiusDataType f = GAIA::MATH::gsqrt(f2);
			d1 = a - f;
			d2 = a + f;

			return GAIA::INTERSECT_TYPE_INTERSECTED;
		}

		template<
			typename _PosDataType,
			typename _DirDataType,
			typename _TriangleDataType>
		GAIA::INTERSECT_TYPE intersect_ray3_triangle3(
			const _PosDataType* pRayPos,
			const _DirDataType* pRayDir,
			const _TriangleDataType* p0,
			const _TriangleDataType* p1,
			const _TriangleDataType* p2)
		{
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}

		/* AABR geometry intersect with others start. */
		template<
			typename _AABRDataType,
			typename _CircleCenterDataType,
			typename _CircleRadiusDataType,
			typename _ResultDataType>
		GAIA::INTERSECT_TYPE intersect_aabb_circle(
			const _AABRDataType* pAABB,
			const _CircleCenterDataType* pCircleCenter,
			const _CircleRadiusDataType& circle_radius,
			_ResultDataType* pDistSq) // pDistSq is the nearest distancesq from circle center to point which on aabr surface.
		{
			_CircleRadiusDataType distsq;
			GAIA::MATH::nearest_distancesq_vec2_aabr(pCircleCenter, pAABB, distsq);
			if(pDistSq != GNIL)
				*pDistSq = distsq;
			if(distsq > circle_radius * circle_radius)
				return GAIA::INTERSECT_TYPE_INTERSECTED;
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}

		/* AABB geometry intersect with others start. */
		template<
			typename _AABBDataType,
			typename _SphereCenterDataType,
			typename _SphereRadiusDataType,
			typename _ResultDataType>
		GAIA::INTERSECT_TYPE intersect_aabb_sphere(
			const _AABBDataType* pAABB,
			const _SphereCenterDataType* pSphereCenter,
			const _SphereRadiusDataType& sphere_radius,
			_ResultDataType* pDistSq) // pDistSq is the nearest distancesq from sphere center to point which on aabb surface.
		{
			_SphereRadiusDataType distsq;
			GAIA::MATH::nearest_distancesq_vec3_aabb(pSphereCenter, pAABB, distsq);
			if(pDistSq != GNIL)
				*pDistSq = distsq;
			if(distsq > sphere_radius * sphere_radius)
				return GAIA::INTERSECT_TYPE_INTERSECTED;
			return GAIA::INTERSECT_TYPE_NOTINTERSECT;
		}
	}
}

#endif
