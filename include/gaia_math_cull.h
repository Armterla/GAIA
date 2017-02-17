#ifndef		__GAIA_MATH_CULL_H__
#define		__GAIA_MATH_CULL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"

namespace GAIA
{
	namespace MATH
	{
		template<
			typename _PlaneDataType,
			typename _PlaneCountDataType,
			typename _TriangleDataType,
			typename _ResultDataType,
			typename _ResultCountDataType>
		GAIA::GVOID cull_plane_triangle(
			const _PlaneDataType* planes,
			const _PlaneCountDataType& plane_count,
			const _TriangleDataType* p0,
			const _TriangleDataType* p1,
			const _TriangleDataType* p2,
			_ResultDataType* pResult,
			_ResultCountDataType& result_count)
		{
			static const GAIA::NUM MAX_CULL_PLANE_COUNT = 32;

			GAST(planes != GNIL);
			GAST(plane_count != 0);
			GAST(pResult != GNIL);
			GAST(plane_count <= MAX_CULL_PLANE_COUNT);
			if(plane_count > MAX_CULL_PLANE_COUNT)
			{
				result_count = 0;
				return;
			}

			_ResultDataType pBuffer[MAX_CULL_PLANE_COUNT + 3][3];

			//
			pResult[0] = p0;
			pResult[1] = p1;
			pResult[2] = p2;
			result_count = 3;

			for(_PlaneCountDataType p = 0; p < plane_count; ++p)
			{
				GAIA::BL bIntersected = GAIA::False;
				_ResultCountDataType intersect_index[2] = {-2, -2};
				GAIA::BL drop_head[2] = {GAIA::False, GAIA::False};
				_ResultDataType intersect_pos[2][3];

				// Helper.
				GAIA::NUM index = 0;
				pResult[result_count] = pResult[0]; // For arithmetic easy.

				//
				for(_ResultCountDataType l = 0; l < result_count; ++l)
				{
					// Intersect triangle side with plane.
					_PlaneDataType plane_normal[3];
					plane_normal[0] = -planes[p * 4 + 0];
					plane_normal[1] = -planes[p * 4 + 1];
					plane_normal[2] = -planes[p * 4 + 2];

					_ResultDataType s[3];
					s[0] = pResult[l * 3 + 0];
					s[1] = pResult[l * 3 + 1];
					s[2] = pResult[l * 3 + 2];

					_ResultDataType e[3];
					e[0] = pResult[(l + 1) * 3 + 0];
					e[1] = pResult[(l + 1) * 3 + 1];
					e[2] = pResult[(l + 1) * 3 + 2];

					_ResultDataType d[3];
					GAIA::MATH::sub3(e, s, d);

					_PlaneDataType ndotu;
					GAIA::MATH::dot3(d, plane_normal, ndotu);
					if(GAIA::MATH::gequalzero(ndotu))
					{
						_PlaneDataType dotres;
						GAIA::MATH::dot34(s, &planes[p * 4], dotres);
						if(dotres <= (_PlaneDataType)0)
							bIntersected = GAIA::True;
						continue;
					}

					_PlaneDataType sdotu;
					GAIA::MATH::dot3(s, plane_normal, sdotu);

					_PlaneDataType t = (planes[p * 4 + 3] - sdotu) / ndotu;
					_ResultDataType posIntersect[3];
					GAIA::MATH::mul3(d, t, posIntersect);
					GAIA::MATH::add3(s, posIntersect, posIntersect);

					if((t >= (_PlaneDataType)1 && ndotu < (_PlaneDataType)0) || (t <= (_PlaneDataType)0 && ndotu > (_PlaneDataType)0))
						bIntersected = GAIA::True;

					// If there is a intersect point exist. Push it to intersect_index list.
					if(t > (_PlaneDataType)0 && t < (_PlaneDataType)1)
					{
						intersect_index[index] = l;
						intersect_pos[index][0] = posIntersect[0];
						intersect_pos[index][1] = posIntersect[1];
						intersect_pos[index][2] = posIntersect[2];
						if(ndotu > (_PlaneDataType)0)
							drop_head[index] = GAIA::True;
						index++;
					}
				}

				// If there is no intersect point. Reset output and return.
				if((intersect_index[0] == -2 || intersect_index[1] == -2) && !bIntersected)
				{
					result_count = 0;
					return;
				}

				// Update result list(Triangle Fan Mode).
				if(intersect_index[0] != -2 && intersect_index[1] != -2)
				{
					for(_ResultCountDataType v = 0; v < result_count; ++v)
					{
						pBuffer[v][0] = pResult[v * 3 + 0];
						pBuffer[v][1] = pResult[v * 3 + 1];
						pBuffer[v][2] = pResult[v * 3 + 2];
					}

					GAIA::NUM sSrcIndex = 0;
					GAIA::NUM sDstIndex = 0;
					for(_ResultCountDataType l = 0; l < result_count; ++l)
					{
						if((intersect_index[0] == l - 1 || (l == 0 && intersect_index[0] == result_count - 1)) && !drop_head[0])
						{
							pResult[sDstIndex * 3 + 0] = intersect_pos[0][0];
							pResult[sDstIndex * 3 + 1] = intersect_pos[0][1];
							pResult[sDstIndex * 3 + 2] = intersect_pos[0][2];
							++sDstIndex;
						}
						if((intersect_index[1] == l - 1 || (l == 0 && intersect_index[1] == result_count - 1)) && !drop_head[1])
						{
							pResult[sDstIndex * 3 + 0] = intersect_pos[1][0];
							pResult[sDstIndex * 3 + 1] = intersect_pos[1][1];
							pResult[sDstIndex * 3 + 2] = intersect_pos[1][2];
							++sDstIndex;
						}
						if(intersect_index[0] == l && drop_head[0])
						{
							pResult[sDstIndex * 3 + 0] = intersect_pos[0][0];
							pResult[sDstIndex * 3 + 1] = intersect_pos[0][1];
							pResult[sDstIndex * 3 + 2] = intersect_pos[0][2];
							++sDstIndex;
						}
						if(intersect_index[1] == l && drop_head[1])
						{
							pResult[sDstIndex * 3 + 0] = intersect_pos[1][0];
							pResult[sDstIndex * 3 + 1] = intersect_pos[1][1];
							pResult[sDstIndex * 3 + 2] = intersect_pos[1][2];
							++sDstIndex;
						}
						if((drop_head[0] && l > intersect_index[0] && l <= intersect_index[1]) ||
							(!drop_head[0] && (l <= intersect_index[0] || l > intersect_index[1])))
						{
							pResult[sDstIndex * 3 + 0] = pBuffer[sSrcIndex][0];
							pResult[sDstIndex * 3 + 1] = pBuffer[sSrcIndex][1];
							pResult[sDstIndex * 3 + 2] = pBuffer[sSrcIndex][2];
							++sDstIndex;
						}
						sSrcIndex++;
					}

					result_count = sDstIndex;
				}
			}
		}
	}
}

#endif
