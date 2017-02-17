#ifndef		__GAIA_MATH_VEC3_H__
#define		__GAIA_MATH_VEC3_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> class VEC3 : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
		public:
			typedef VEC3<_DataType> __MyType;
		public:
			GINL VEC3(){}
			template<typename _ParamDataType> VEC3(const GAIA::MATH::VEC3<_ParamDataType>& v){this->operator = (v);}
			template<typename _ParamDataType1, typename _ParamDataType2, typename _ParamDataType3> VEC3(const _ParamDataType1& x, const _ParamDataType2& y, const _ParamDataType3& z){this->x = GSCAST(_DataType)(x); this->y = GSCAST(_DataType)(y); this->z = GSCAST(_DataType)(z);}
			template<typename _ParamDataType> VEC3(_ParamDataType v){this->operator = (v);}
			template<typename _ParamDataType> VEC3(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> VEC3(_ParamDataType* p){this->operator = (p);}
			GINL GAIA::NUM size() const{return 3;}
			GINL _DataType length() const{_DataType ret; GAIA::MATH::length3(this->fptr(), ret); return ret;}
			GINL _DataType lengthsq() const{_DataType ret; GAIA::MATH::lengthsq3(this->fptr(), ret); return ret;}
			GINL _DataType minimize() const{return GAIA::ALGO::gmin3(x, y, z);}
			GINL _DataType maximize() const{return GAIA::ALGO::gmax3(x, y, z);}
			template<typename _ParamDataType> __MyType& minimize(const _ParamDataType& src){if(x > src) x = src; if(y > src) y = src; if(z > src) z = src; return *this;}
			template<typename _ParamDataType> __MyType& maximize(const _ParamDataType& src){if(x < src) x = src; if(y < src) y = src; if(z < src) z = src; return *this;}
			template<typename _ParamDataType> __MyType& minimize(const GAIA::MATH::VEC3<_ParamDataType>& src){if(x > src.x) x = src.x; if(y > src.y) y = src.y; if(z > src.z) z = src.z; return *this;}
			template<typename _ParamDataType> __MyType& maximize(const GAIA::MATH::VEC3<_ParamDataType>& src){if(x < src.x) x = src.x; if(y < src.y) y = src.y; if(z < src.z) z = src.z; return *this;}
			GINL GAIA::GVOID normalize(){_DataType t = this->length(); (*this) /= t;}
			GINL GAIA::GVOID try_normalize()
			{
				if(x == (_DataType)0 && y == (_DataType)0 && z == (_DataType)0)
					return;
				_DataType t = this->length();
				(*this) /= t;
			}
			GINL GAIA::BL isnormalized() const{return this->lengthsq() == (_DataType)1;}
			GINL GAIA::BL iszero() const{return x == (_DataType)0 && y == (_DataType)0 && z == (_DataType)0;}
			GINL GAIA::BL isfinited() const{return GAIA::MATH::gfinite(x) && GAIA::MATH::gfinite(y) && GAIA::MATH::gfinite(z);}
			GINL GAIA::GVOID inverse(){x = (_DataType)1 / x; y = (_DataType)1 / y; z = (_DataType)1 / z;}
			GINL GAIA::GVOID neg(){x = -x; y = -y; z = -z;};
			template<typename _ParamDataType> _DataType dot(const GAIA::MATH::VEC3<_ParamDataType>& v) const
			{
				_DataType ret;
				GAIA::MATH::dot3(this->fptr(), v.fptr(), ret);
				return ret;
			}
			template<typename _ParamDataType> __MyType num(const GAIA::MATH::VEC3<_ParamDataType>& v) const{__MyType ret; ret.x = x * v.x; ret.y = y * v.y; ret.z = z * v.z; return ret;}
			template<typename _ParamDataType> __MyType cross(const GAIA::MATH::VEC3<_ParamDataType>& v) const
			{
				__MyType ret;
				GAIA::MATH::cross3(this->fptr(), v.fptr(), ret.fptr());
				return ret;
			}
			template<typename _ParamDataType1, typename _ParamDataType2> GAIA::GVOID lerp(const _ParamDataType1& src, const _ParamDataType2& s){x = GAIA::MATH::lerp(x, src, s); y = GAIA::MATH::lerp(y, src, s); z = GAIA::MATH::lerp(z, src, s);}
			template<typename _ParamDataType1, typename _ParamDataType2> GAIA::GVOID lerp(const GAIA::MATH::VEC3<_ParamDataType1>& src, const _ParamDataType2& s){x = GAIA::MATH::lerp(x, src.x, s); y = GAIA::MATH::lerp(y, src.y, s); z = GAIA::MATH::lerp(z, src.z, s);}
			template<typename _ParamDataType1, typename _ParamDataType2> GAIA::GVOID lerp(const GAIA::MATH::VEC3<_ParamDataType1>& src, const GAIA::MATH::VEC3<_ParamDataType2>& s){x = GAIA::MATH::lerp(x, src.x, s.x); y = GAIA::MATH::lerp(y, src.y, s.y); z = GAIA::MATH::lerp(z, src.z, s.z);}
			GINL GAIA::MATH::VEC2<_DataType> xy() const{GAIA::MATH::VEC2<_DataType> ret; ret.x = x; ret.y = y; return ret;}
			GINL GAIA::MATH::VEC2<_DataType> xz() const{GAIA::MATH::VEC2<_DataType> ret; ret.x = x; ret.y = z; return ret;}
			GINL GAIA::MATH::VEC2<_DataType> yz() const{GAIA::MATH::VEC2<_DataType> ret; ret.x = y; ret.y = z; return ret;}
			GINL GAIA::MATH::VEC2<_DataType> yx() const{GAIA::MATH::VEC2<_DataType> ret; ret.x = y; ret.y = x; return ret;}
			GINL GAIA::MATH::VEC2<_DataType> zx() const{GAIA::MATH::VEC2<_DataType> ret; ret.x = z; ret.y = x; return ret;}
			GINL GAIA::MATH::VEC2<_DataType> zy() const{GAIA::MATH::VEC2<_DataType> ret; ret.x = z; ret.y = y; return ret;}
			GINL GAIA::MATH::VEC3<_DataType> xyz() const{GAIA::MATH::VEC3<_DataType> ret; ret.x = x; ret.y = y; ret.z = z; return ret;}
			GINL GAIA::MATH::VEC3<_DataType> xzy() const{GAIA::MATH::VEC3<_DataType> ret; ret.x = x; ret.y = z; ret.z = y; return ret;}
			GINL GAIA::MATH::VEC3<_DataType> yxz() const{GAIA::MATH::VEC3<_DataType> ret; ret.x = y; ret.y = x; ret.z = z; return ret;}
			GINL GAIA::MATH::VEC3<_DataType> yzx() const{GAIA::MATH::VEC3<_DataType> ret; ret.x = y; ret.y = z; ret.z = x; return ret;}
			GINL GAIA::MATH::VEC3<_DataType> zxy() const{GAIA::MATH::VEC3<_DataType> ret; ret.x = z; ret.y = x; ret.z = y; return ret;}
			GINL GAIA::MATH::VEC3<_DataType> zyx() const{GAIA::MATH::VEC3<_DataType> ret; ret.x = z; ret.y = y; ret.z = x; return ret;}
			template<typename _ParamDataType> GAIA::GVOID xy(const GAIA::MATH::VEC2<_ParamDataType>& src){x = GSCAST(_DataType)(src.x); y = GSCAST(_DataType)(src.y);}
			template<typename _ParamDataType> GAIA::GVOID xz(const GAIA::MATH::VEC2<_ParamDataType>& src){x = GSCAST(_DataType)(src.x); z = GSCAST(_DataType)(src.y);}
			template<typename _ParamDataType> GAIA::GVOID yz(const GAIA::MATH::VEC2<_ParamDataType>& src){y = GSCAST(_DataType)(src.x); z = GSCAST(_DataType)(src.y);}
			template<typename _ParamDataType> GAIA::GVOID yx(const GAIA::MATH::VEC2<_ParamDataType>& src){y = GSCAST(_DataType)(src.x); x = GSCAST(_DataType)(src.y);}
			template<typename _ParamDataType> GAIA::GVOID zx(const GAIA::MATH::VEC2<_ParamDataType>& src){z = GSCAST(_DataType)(src.x); x = GSCAST(_DataType)(src.y);}
			template<typename _ParamDataType> GAIA::GVOID zy(const GAIA::MATH::VEC2<_ParamDataType>& src){z = GSCAST(_DataType)(src.x); y = GSCAST(_DataType)(src.y);}
			template<typename _ParamDataType> GAIA::GVOID xyz(const GAIA::MATH::VEC3<_ParamDataType>& src){x = GSCAST(_DataType)(src.x); y = GSCAST(_DataType)(src.y); z = GSCAST(_DataType)(src.z);}
			template<typename _ParamDataType> GAIA::GVOID xzy(const GAIA::MATH::VEC3<_ParamDataType>& src){x = GSCAST(_DataType)(src.x); z = GSCAST(_DataType)(src.y); y = GSCAST(_DataType)(src.z);}
			template<typename _ParamDataType> GAIA::GVOID yxz(const GAIA::MATH::VEC3<_ParamDataType>& src){y = GSCAST(_DataType)(src.x); x = GSCAST(_DataType)(src.y); z = GSCAST(_DataType)(src.z);}
			template<typename _ParamDataType> GAIA::GVOID yzx(const GAIA::MATH::VEC3<_ParamDataType>& src){y = GSCAST(_DataType)(src.x); z = GSCAST(_DataType)(src.y); x = GSCAST(_DataType)(src.z);}
			template<typename _ParamDataType> GAIA::GVOID zxy(const GAIA::MATH::VEC3<_ParamDataType>& src){z = GSCAST(_DataType)(src.x); x = GSCAST(_DataType)(src.y); y = GSCAST(_DataType)(src.z);}
			template<typename _ParamDataType> GAIA::GVOID zyx(const GAIA::MATH::VEC3<_ParamDataType>& src){z = GSCAST(_DataType)(src.x); y = GSCAST(_DataType)(src.y); x = GSCAST(_DataType)(src.z);}
			GINL const _DataType* fptr() const{return &x;}
			GINL _DataType* fptr(){return &x;}
			GINL const _DataType* bptr() const{return this->fptr() + this->size() - 1;}
			GINL _DataType* bptr(){return this->fptr() + this->size() - 1;}
			template<typename _ParamDataType> __MyType operator + (const GAIA::MATH::VEC3<_ParamDataType>& v) const{__MyType ret; ret.x = x + v.x; ret.y = y + v.y; ret.z = z + v.z; return ret;}
			template<typename _ParamDataType> __MyType operator - (const GAIA::MATH::VEC3<_ParamDataType>& v) const{__MyType ret; ret.x = x - v.x; ret.y = y - v.y; ret.z = z - v.z; return ret;}
			template<typename _ParamDataType> __MyType operator * (const GAIA::MATH::VEC3<_ParamDataType>& v) const{__MyType ret; ret.x = x * v.x; ret.y = y * v.y; ret.z = z * v.z; return ret;}
			template<typename _ParamDataType> __MyType operator / (const GAIA::MATH::VEC3<_ParamDataType>& v) const{__MyType ret; ret.x = x / v.x; ret.y = y / v.y; ret.z = z / v.z; return ret;}
			template<typename _ParamDataType> __MyType operator + (const _ParamDataType& v) const{__MyType ret; ret.x = x + v; ret.y = y + v; ret.z = z + v; return ret;}
			template<typename _ParamDataType> __MyType operator - (const _ParamDataType& v) const{__MyType ret; ret.x = x - v; ret.y = y - v; ret.z = z - v; return ret;}
			template<typename _ParamDataType> __MyType operator * (const _ParamDataType& v) const{__MyType ret; ret.x = x * v; ret.y = y * v; ret.z = z * v; return ret;}
			template<typename _ParamDataType> __MyType operator / (const _ParamDataType& v) const{__MyType ret; ret.x = x / v; ret.y = y / v; ret.z = z / v; return ret;}
			template<typename _ParamDataType> __MyType& operator += (const GAIA::MATH::VEC3<_ParamDataType>& v){x += v.x; y += v.y; z += v.z; return *this;}
			template<typename _ParamDataType> __MyType& operator -= (const GAIA::MATH::VEC3<_ParamDataType>& v){x -= v.x; y -= v.y; z -= v.z; return *this;}
			template<typename _ParamDataType> __MyType& operator *= (const GAIA::MATH::VEC3<_ParamDataType>& v){x *= v.x; y *= v.y; z *= v.z; return *this;}
			template<typename _ParamDataType> __MyType& operator /= (const GAIA::MATH::VEC3<_ParamDataType>& v){x /= v.x; y /= v.y; z /= v.z; return *this;}
			template<typename _ParamDataType> __MyType& operator += (_ParamDataType v){x += v; y += v; z += v; return *this;}
			template<typename _ParamDataType> __MyType& operator -= (_ParamDataType v){x -= v; y -= v; z -= v; return *this;}
			template<typename _ParamDataType> __MyType& operator *= (_ParamDataType v){x *= v; y *= v; z *= v; return *this;}
			template<typename _ParamDataType> __MyType& operator /= (_ParamDataType v){x /= v; y /= v; z /= v; return *this;}
			template<typename _ParamDataType> __MyType& operator = (const GAIA::MATH::VEC3<_ParamDataType>& v){x = GSCAST(_DataType)(v.x); y = GSCAST(_DataType)(v.y); z = GSCAST(_DataType)(v.z); return *this;}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType v){x = y = z = GSCAST(_DataType)(v); return *this;}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p){x = GSCAST(_DataType)(p[0]); y = GSCAST(_DataType)(p[1]); z = GSCAST(_DataType)(p[2]); return *this;}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType* p){return this->operator = (GSCAST(const _ParamDataType*)(p));}
			template<typename _ParamDataType> GAIA::BL operator == (const GAIA::MATH::VEC3<_ParamDataType>& v) const{return GAIA::ALGO::cmp3(this->fptr(), v.fptr()) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const GAIA::MATH::VEC3<_ParamDataType>& v) const{return !this->operator == (v);}
			template<typename _ParamDataType> GAIA::BL operator >= (const GAIA::MATH::VEC3<_ParamDataType>& v) const{return !this->operator < (v);}
			template<typename _ParamDataType> GAIA::BL operator <= (const GAIA::MATH::VEC3<_ParamDataType>& v) const{return !this->operator > (v);}
			template<typename _ParamDataType> GAIA::BL operator > (const GAIA::MATH::VEC3<_ParamDataType>& v) const{return GAIA::ALGO::cmp3(this->fptr(), v.fptr()) > 0;}
			template<typename _ParamDataType> GAIA::BL operator < (const GAIA::MATH::VEC3<_ParamDataType>& v) const{return GAIA::ALGO::cmp3(this->fptr(), v.fptr()) < 0;}
			template<typename _ParamDataType> GAIA::BL operator == (const _ParamDataType& v) const{return GAIA::ALGO::cmp3k(this->fptr(), v) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const _ParamDataType& v) const{return !this->operator == (v);}
			template<typename _ParamDataType> GAIA::BL operator >= (const _ParamDataType& v) const{return !this->operator < (v);}
			template<typename _ParamDataType> GAIA::BL operator <= (const _ParamDataType& v) const{return !this->operator > (v);}
			template<typename _ParamDataType> GAIA::BL operator > (const _ParamDataType& v) const{return GAIA::ALGO::cmp3k(this->fptr(), v) > 0;}
			template<typename _ParamDataType> GAIA::BL operator < (const _ParamDataType& v) const{return GAIA::ALGO::cmp3k(this->fptr(), v) < 0;}
			template<typename _ParamDataType> const _DataType& operator [] (const _ParamDataType& index) const{GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
			template<typename _ParamDataType> _DataType& operator [] (const _ParamDataType& index){GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
		public:
			_DataType x, y, z;
		};
#define GAIA_TEMP_MACRO(objtype, type) \
		template<typename _ParamDataType> objtype<_ParamDataType> operator + (type t, const objtype<_ParamDataType>& v){objtype<_ParamDataType> ret = t; ret += v; return ret;}\
		template<typename _ParamDataType> objtype<_ParamDataType> operator - (type t, const objtype<_ParamDataType>& v){objtype<_ParamDataType> ret = t; ret -= v; return ret;}\
		template<typename _ParamDataType> objtype<_ParamDataType> operator * (type t, const objtype<_ParamDataType>& v){objtype<_ParamDataType> ret = t; ret *= v; return ret;}\
		template<typename _ParamDataType> objtype<_ParamDataType> operator / (type t, const objtype<_ParamDataType>& v){objtype<_ParamDataType> ret = t; ret /= v; return ret;}
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::UM)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::NM)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::U8)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::N8)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::U16)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::N16)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::U32)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::N32)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::F32)
		GAIA_TEMP_MACRO(GAIA::MATH::VEC3, GAIA::F64)
#undef GAIA_TEMP_MACRO
	}
}

#endif
