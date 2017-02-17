#ifndef		__GAIA_MATH_AABR_H__
#define		__GAIA_MATH_AABR_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> class AABR : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
		public:
			typedef AABR<_DataType> __MyType;
		public:
			typedef GAIA::MATH::VEC2<_DataType> _vectype;
		public:
			GINL AABR(){}
			template<
				typename _ParamDataType1,
				typename _ParamDataType2,
				typename _ParamDataType3,
				typename _ParamDataType4> AABR(
				const _ParamDataType1& pmin_x,
				const _ParamDataType2& pmin_y,
				const _ParamDataType3& pmax_x,
				const _ParamDataType4& pmax_y)
			{
				pmin.x = GSCAST(_DataType)(pmin_x);
				pmin.y = GSCAST(_DataType)(pmin_y);
				pmax.x = GSCAST(_DataType)(pmax_x);
				pmax.y = GSCAST(_DataType)(pmax_y);
			}
			template<typename _ParamDataType1, typename _ParamDataType2> AABR(const GAIA::MATH::VEC2<_ParamDataType1>& pmin, const GAIA::MATH::VEC2<_ParamDataType2>& pmax)
			{
				this->pmin = pmin;
				this->pmax = pmax;
			}
			template<typename _ParamDataType> AABR(const GAIA::MATH::AABR<_ParamDataType>& src){this->operator = (src);}
			template<typename _ParamDataType> AABR(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> AABR(_ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> AABR(_ParamDataType v){this->operator = (v);}
			GINL GAIA::NUM size() const{return 4;}
			GINL GAIA::GVOID identity(){pmin = (_DataType)+99999999; pmax = (_DataType)-99999999;}
			GINL GAIA::BL isidentity() const{return pmin.x > pmax.x || pmin.y > pmax.y;}
			GINL _vectype center() const{return (pmin + pmax) / (_DataType)2;}
			GINL _vectype long_size() const{return pmax - pmin;}
			GINL _DataType long_radius() const{return (pmax - pmin).length() / (_DataType)2;}
			GINL _DataType short_radius() const{return (pmax - pmin).minimize() / (_DataType)2;}
			GINL _DataType width() const{return pmax.x - pmin.x;}
			GINL _DataType height() const{return pmax.y - pmin.y;}
			GINL GAIA::BL iszero() const{return pmin.iszero() && pmax.iszero();}
			GINL GAIA::BL isfinited() const{return pmin.isfinited() && pmax.isfinited();}
			GINL _vectype point(const GAIA::NUM& sIndex) const
			{
				switch(sIndex)
				{
				case 0:
					return this->ss();
				case 1:
					return this->bs();
				case 2:
					return this->bb();
				case 3:
					return this->sb();
				default:
					GASTFALSE;
					return (_vectype)0;
				}
			}
			GINL _vectype ss() const{return _vectype(pmin.x, pmin.y);}
			GINL _vectype bs() const{return _vectype(pmax.x, pmin.y);}
			GINL _vectype bb() const{return _vectype(pmax.x, pmax.y);}
			GINL _vectype sb() const{return _vectype(pmin.x, pmax.y);}
			GINL const _DataType* fptr() const{return GRCAST(const _DataType*)(&pmin);}
			GINL _DataType* fptr(){return GRCAST(_DataType*)(&pmin);}
			GINL const _DataType* bptr() const{return this->fptr() + this->size() - 1;}
			GINL _DataType* bptr(){return this->fptr() + this->size() - 1;}
			template<typename _ParamDataType> GAIA::GVOID expand(const GAIA::MATH::AABR<_ParamDataType>& t){pmin.minimize(t.pmin); pmax.maximize(t.pmax);}
			template<typename _ParamDataType> GAIA::GVOID expand(const GAIA::MATH::VEC2<_ParamDataType>& t){pmin.minimize(t); pmax.maximize(t);}
			template<typename _ParamDataType> GAIA::BL clamp(GAIA::MATH::VEC2<_ParamDataType>& t) const
			{
				GAIA::BL bRet = GAIA::False;
				if(t.x < pmin.x){t.x = pmin.x; bRet = GAIA::True;}
				if(t.y < pmin.y){t.y = pmin.y; bRet = GAIA::True;}
				if(t.x > pmax.x){t.x = pmax.x; bRet = GAIA::True;}
				if(t.y > pmax.y){t.y = pmax.y; bRet = GAIA::True;}
				return bRet;
			}
			template<typename _ParamDataType> GAIA::BL clamp(GAIA::MATH::AABR<_ParamDataType>& t) const
			{
				GAIA::BL bRet = GAIA::False;
				if(t.pmin.x < pmin.x){t.pmin.x = pmin.x; bRet = GAIA::True;}
				if(t.pmin.y < pmin.y){t.pmin.y = pmin.y; bRet = GAIA::True;}
				if(t.pmax.x < pmin.x){t.pmax.x = pmin.x; bRet = GAIA::True;}
				if(t.pmax.y < pmin.y){t.pmax.y = pmin.y; bRet = GAIA::True;}
				if(t.pmin.x > pmax.x){t.pmin.x = pmax.x; bRet = GAIA::True;}
				if(t.pmin.y > pmax.y){t.pmin.y = pmax.y; bRet = GAIA::True;}
				if(t.pmax.x > pmax.x){t.pmax.x = pmax.x; bRet = GAIA::True;}
				if(t.pmax.y > pmax.y){t.pmax.y = pmax.y; bRet = GAIA::True;}
				return bRet;
			}
			template<typename _ParamDataType> GAIA::GVOID project(GAIA::MATH::VEC2<_ParamDataType>& t) const{t = pmin + t * this->long_size();}
			template<typename _ParamDataType> GAIA::GVOID unproject(GAIA::MATH::VEC2<_ParamDataType>& t) const{t = (t - pmin) / this->long_size();}
			template<typename _ParamDataType> __MyType operator + (const GAIA::MATH::VEC2<_ParamDataType>& v){__MyType ret; ret.pmin = pmin + v; ret.pmax = pmax + v; return ret;}
			template<typename _ParamDataType> __MyType operator - (const GAIA::MATH::VEC2<_ParamDataType>& v){__MyType ret; ret.pmin = pmin - v; ret.pmax = pmax - v; return ret;}
			template<typename _ParamDataType> __MyType operator * (const GAIA::MATH::VEC2<_ParamDataType>& v){__MyType ret; ret.pmin = pmin * v; ret.pmax = pmax * v; return ret;}
			template<typename _ParamDataType> __MyType operator / (const GAIA::MATH::VEC2<_ParamDataType>& v){__MyType ret; ret.pmin = pmin / v; ret.pmax = pmax / v; return ret;}
			template<typename _ParamDataType> __MyType& operator += (const GAIA::MATH::VEC2<_ParamDataType>& v){pmin += v; pmax += v; return *this;}
			template<typename _ParamDataType> __MyType& operator -= (const GAIA::MATH::VEC2<_ParamDataType>& v){pmin -= v; pmax -= v; return *this;}
			template<typename _ParamDataType> __MyType& operator *= (const GAIA::MATH::VEC2<_ParamDataType>& v){pmin *= v; pmax *= v; return *this;}
			template<typename _ParamDataType> __MyType& operator /= (const GAIA::MATH::VEC2<_ParamDataType>& v){pmin /= v; pmax /= v; return *this;}
			template<typename _ParamDataType> _vectype nearest_point(const GAIA::MATH::VEC2<_ParamDataType>& v) const
			{
				_vectype ret;
				if(v.x < pmin.x)
					ret.x = pmin.x;
				else if(v.x > pmax.x)
					ret.x = pmax.x;
				else
					ret.x = v.x;
				if(v.y < pmin.y)
					ret.y = pmin.y;
				else if(v.y > pmax.y)
					ret.y = pmax.y;
				else
					ret.y = v.y;
				return ret;
			}
			template<typename _ParamDataType> _DataType distance(const GAIA::MATH::AABR<_ParamDataType>& src) const{return GAIA::MATH::gsqrt(this->distancesq(src));}
			template<typename _ParamDataType> _DataType distancesq(const GAIA::MATH::AABR<_ParamDataType>& src) const
			{
				_DataType d[4];
				d[0] = GAIA::MATH::gabs(pmin.x - src.pmin.x);
				d[1] = GAIA::MATH::gabs(pmin.x - src.pmax.x);
				d[3] = GAIA::MATH::gabs(pmax.x - src.pmin.x);
				d[4] = GAIA::MATH::gabs(pmax.x - src.pmax.x);
				_DataType nearest_x = GAIA::ALGO::gmin(
					GAIA::ALGO::gmin(d[0], d[1]), GAIA::ALGO::gmin(d[2], d[3]));
				d[0] = GAIA::MATH::gabs(pmin.y - src.pmin.y);
				d[1] = GAIA::MATH::gabs(pmin.y - src.pmax.y);
				d[3] = GAIA::MATH::gabs(pmax.y - src.pmin.y);
				d[4] = GAIA::MATH::gabs(pmax.y - src.pmax.y);
				_DataType nearest_y = GAIA::ALGO::gmin(
					GAIA::ALGO::gmin(d[0], d[1]), GAIA::ALGO::gmin(d[2], d[3]));
				return nearest_x * nearest_x + nearest_y * nearest_y;
			}
			template<typename _ParamDataType> _DataType distance(const GAIA::MATH::VEC2<_ParamDataType>& v) const{return GAIA::MATH::gsqrt(this->distancesq(v));}
			template<typename _ParamDataType> _DataType distancesq(const GAIA::MATH::VEC2<_ParamDataType>& v) const
			{
				_vectype nearestpoint = this->nearest_point(v);
				return (v - nearestpoint).lengthsq();
			}
			template<typename _ParamDataType> GAIA::INTERSECT_TYPE intersect_point(const GAIA::MATH::VEC2<_ParamDataType>& v) const
			{
				return GAIA::MATH::intersect_point2_aabr(v.fptr(), this->fptr());
			}
			template<typename _ParamDataType> GAIA::INTERSECT_TYPE intersect_aabr(const GAIA::MATH::AABR<_ParamDataType>& aabb) const
			{
				return GAIA::MATH::intersect_aabr(this->fptr(), aabb.fptr());
			}
			template<typename _ParamCircleCenterDataType, typename _ParamCircleRadiusDataType, typename _ParamResultDataType> GAIA::INTERSECT_TYPE intersect_circle(const GAIA::MATH::VEC3<_ParamCircleCenterDataType>& circle_center, const _ParamCircleRadiusDataType& circle_radius, _ParamResultDataType* pDistSq) const
			{
				return GAIA::MATH::intersect_aabb_circle(this->fptr(), circle_center.fptr(), circle_radius, pDistSq);
			}
			template<typename _ParamDataType> GAIA::INTERSECT_TYPE intersect_line(const GAIA::MATH::PLANE<_ParamDataType>* pPlanes, const GAIA::NUM& sPlaneCount) const
			{
				GAST(pPlanes != GNIL);
			}
			template<typename _ParamDataType> __MyType& operator = (const GAIA::MATH::AABR<_ParamDataType>& src){pmin = src.pmin; pmax = src.pmax; return *this;}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p){pmin = p; pmax = p + 2; return *this;}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType* p){return this->operator = (GSCAST(const _ParamDataType*)(p));}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType v){pmin = v; pmax = v; return *this;}
			template<typename _ParamDataType> GAIA::BL operator == (const GAIA::MATH::AABR<_ParamDataType>& t) const{return pmin == t.pmin && pmax == t.pmax;}
			template<typename _ParamDataType> GAIA::BL operator != (const GAIA::MATH::AABR<_ParamDataType>& t) const{return !(this->operator == (t));}
			template<typename _ParamDataType> GAIA::BL operator >= (const GAIA::MATH::AABR<_ParamDataType>& t) const
			{
				if(pmin < t.pmin)
					return GAIA::False;
				else if(pmin > t.pmin)
					return GAIA::True;
				if(pmax < t.pmax)
					return GAIA::False;
				else if(pmax >= t.pmax)
					return GAIA::True;
				return GAIA::False;
			}
			template<typename _ParamDataType> GAIA::BL operator <= (const GAIA::MATH::AABR<_ParamDataType>& t) const
			{
				if(pmin > t.pmin)
					return GAIA::False;
				else if(pmin < t.pmin)
					return GAIA::True;
				if(pmax > t.pmax)
					return GAIA::False;
				else if(pmax <= t.pmax)
					return GAIA::True;
				return GAIA::False;
			}
			template<typename _ParamDataType> GAIA::BL operator > (const GAIA::MATH::AABR<_ParamDataType>& t) const{return !(this->operator <= (t));}
			template<typename _ParamDataType> GAIA::BL operator < (const GAIA::MATH::AABR<_ParamDataType>& t) const{return !(this->operator >= (t));}
			template<typename _ParamDataType> GAIA::BL operator == (const _ParamDataType& v) const{return pmin == v && pmax == v;}
			template<typename _ParamDataType> GAIA::BL operator != (const _ParamDataType& v) const{return !this->operator == (v);}
			template<typename _ParamDataType> GAIA::BL operator >= (const _ParamDataType& v) const
			{
				if(pmin < v)
					return GAIA::False;
				else if(pmin > v)
					return GAIA::True;
				if(pmax < v)
					return GAIA::False;
				else if(pmax >= v)
					return GAIA::True;
				return GAIA::False;
			}
			template<typename _ParamDataType> GAIA::BL operator <= (const _ParamDataType& v) const
			{
				if(pmin > v)
					return GAIA::False;
				else if(pmin < v)
					return GAIA::True;
				if(pmax > v)
					return GAIA::False;
				else if(pmax <= v)
					return GAIA::True;
				return GAIA::False;
			}
			template<typename _ParamDataType> GAIA::BL operator > (const _ParamDataType& v) const{return !this->operator <= (v);}
			template<typename _ParamDataType> GAIA::BL operator < (const _ParamDataType& v) const{return !this->operator >= (v);}
			template<typename _ParamDataType> const _DataType& operator [] (const _ParamDataType& index) const{GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
			template<typename _ParamDataType> _DataType& operator [] (const _ParamDataType& index){GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
		private:
			GINL GAIA::GVOID selfupdate()
			{
				if(pmin.x > pmax.x)
					GAIA::ALGO::swap(pmin.x, pmax.x);
				if(pmin.y > pmax.y)
					GAIA::ALGO::swap(pmin.y, pmax.y);
			}
		public:
			_vectype pmin;
			_vectype pmax;
		};
	}
}

#endif
