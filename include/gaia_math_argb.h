#ifndef		__GAIA_MATH_ARGB_H__
#define		__GAIA_MATH_ARGB_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> class ARGB : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
		public:
			typedef ARGB<_DataType> __MyType;
		public:
			GINL ARGB(){}
			template<
				typename _ParamDataType1,
				typename _ParamDataType2,
				typename _ParamDataType3,
				typename _ParamDataType4> ARGB(
					const _ParamDataType1& a,
					const _ParamDataType2& r,
					const _ParamDataType3& g,
					const _ParamDataType4& b)
			{
				this->a = GSCAST(_DataType)(a);
				this->r = GSCAST(_DataType)(r);
				this->g = GSCAST(_DataType)(g);
				this->b = GSCAST(_DataType)(b);
			}
			template<typename _ParamDataType> ARGB(const GAIA::MATH::ARGB<_ParamDataType>& src){this->operator = (src);}
			template<typename _ParamDataType> ARGB(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> ARGB(_ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> ARGB(_ParamDataType v){this->operator = (v);}

			GINL GAIA::NUM size() const{return 4;}

			GINL _DataType length() const{return GAIA::MATH::gsqrt(this->lengthsq());}
			GINL _DataType lengthsq() const{return r * r + g * g + b * b;}

			GINL GAIA::BL iszero() const{return a == (_DataType)0 && r == (_DataType)0 && g == (_DataType)0 && b == (_DataType)0;}
			GINL GAIA::BL isfinited() const{return GAIA::MATH::gfinite(a) && GAIA::MATH::gfinite(r) && GAIA::MATH::gfinite(g) && GAIA::MATH::gfinite(b);}

			GINL _DataType avg_rgb() const{return GAIA::MATH::gavg3(r, g, b);}
			GINL _DataType avg_argb() const{return GAIA::MATH::gavg4(a, r, g, b);}
			GINL _DataType max_rgb() const{return GAIA::ALGO::gmax3(r, g, b);}
			GINL _DataType max_argb() const{return GAIA::ALGO::gmax(GAIA::ALGO::gmax(a, r), GAIA::ALGO::gmax(g, b));}
			GINL _DataType min_rgb() const{return GAIA::ALGO::gmin3(r, g, b);}
			GINL _DataType min_argb() const{return GAIA::ALGO::gmin(GAIA::ALGO::gmin(a, r), GAIA::ALGO::gmin(g, b));}

			GINL GAIA::GVOID torealmode(){(*this) /= 255;}
			GINL GAIA::GVOID tobytemode(){(*this) *= 255;}

			GINL GAIA::U32 tou32() const
			{
				GAIA::U32 ret;
				GRCAST(GAIA::U8*)(&ret)[3] = GSCAST(GAIA::U8)(a);
				GRCAST(GAIA::U8*)(&ret)[2] = GSCAST(GAIA::U8)(r);
				GRCAST(GAIA::U8*)(&ret)[1] = GSCAST(GAIA::U8)(g);
				GRCAST(GAIA::U8*)(&ret)[0] = GSCAST(GAIA::U8)(b);
				return ret;
			}
			GINL GAIA::GVOID fromu32(GAIA::U32 u)
			{
				a = GSCAST(_DataType)(GRCAST(GAIA::U8*)(&u)[3]);
				r = GSCAST(_DataType)(GRCAST(GAIA::U8*)(&u)[2]);
				g = GSCAST(_DataType)(GRCAST(GAIA::U8*)(&u)[1]);
				b = GSCAST(_DataType)(GRCAST(GAIA::U8*)(&u)[0]);
			}

			GINL const _DataType* fptr() const{return &r;}
			GINL _DataType* fptr(){return &r;}
			GINL const _DataType* bptr() const{return this->fptr() + this->size() - 1;}
			GINL _DataType* bptr(){return this->fptr() + this->size() - 1;}

			template<typename _ParamEndDataType, typename _ParamFactorDataType> GAIA::GVOID lerp(GAIA::MATH::ARGB<_ParamEndDataType>& end, const _ParamFactorDataType& factor)
			{
				a = GAIA::MATH::lerp(a, end.a, factor);
				r = GAIA::MATH::lerp(r, end.r, factor);
				g = GAIA::MATH::lerp(g, end.g, factor);
				b = GAIA::MATH::lerp(b, end.b, factor);
			}

			template<typename _ParamDataType> __MyType& operator = (const GAIA::MATH::ARGB<_ParamDataType>& src){a = src.a; r = src.r; g = src.g; b = src.b; return *this;}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p){r = p[0]; g = p[1]; b = p[2]; a = p[3]; return *this;}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType* p){return this->operator = (GSCAST(const _ParamDataType*)(p));}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType v){a = r = g = b = v; return *this;}

			template<typename _ParamDataType> __MyType operator + (const GAIA::MATH::ARGB<_ParamDataType>& src) const{__MyType ret = *this; ret += src; return ret;}
			template<typename _ParamDataType> __MyType operator - (const GAIA::MATH::ARGB<_ParamDataType>& src) const{__MyType ret = *this; ret -= src; return ret;}
			template<typename _ParamDataType> __MyType operator * (const GAIA::MATH::ARGB<_ParamDataType>& src) const{__MyType ret = *this; ret *= src; return ret;}
			template<typename _ParamDataType> __MyType operator / (const GAIA::MATH::ARGB<_ParamDataType>& src) const{__MyType ret = *this; ret /= src; return ret;}
			template<typename _ParamDataType> __MyType& operator += (const GAIA::MATH::ARGB<_ParamDataType>& src){a += src.a; r += src.r; g += src.g; b += src.b; return *this;}
			template<typename _ParamDataType> __MyType& operator -= (const GAIA::MATH::ARGB<_ParamDataType>& src){a -= src.a; r -= src.r; g -= src.g; b -= src.b; return *this;}
			template<typename _ParamDataType> __MyType& operator *= (const GAIA::MATH::ARGB<_ParamDataType>& src){a *= src.a; r *= src.r; g *= src.g; b *= src.b; return *this;}
			template<typename _ParamDataType> __MyType& operator /= (const GAIA::MATH::ARGB<_ParamDataType>& src){a /= src.a; r /= src.r; g /= src.g; b /= src.b; return *this;}

			template<typename _ParamDataType> __MyType operator + (const _ParamDataType& v) const{__MyType ret = *this; ret.a += v; ret.r += v; ret.g += v; ret.b += v; return ret;}
			template<typename _ParamDataType> __MyType operator - (const _ParamDataType& v) const{__MyType ret = *this; ret.a -= v; ret.r -= v; ret.g -= v; ret.b -= v; return ret;}
			template<typename _ParamDataType> __MyType operator * (const _ParamDataType& v) const{__MyType ret = *this; ret.a *= v; ret.r *= v; ret.g *= v; ret.b *= v; return ret;}
			template<typename _ParamDataType> __MyType operator / (const _ParamDataType& v) const{__MyType ret = *this; ret.a /= v; ret.r /= v; ret.g /= v; ret.b /= v; return ret;}
			template<typename _ParamDataType> __MyType& operator += (_ParamDataType v){a += v; r += v; g += v; b += v; return *this;}
			template<typename _ParamDataType> __MyType& operator -= (_ParamDataType v){a -= v; r -= v; g -= v; b -= v; return *this;}
			template<typename _ParamDataType> __MyType& operator *= (_ParamDataType v){a *= v; r *= v; g *= v; b *= v; return *this;}
			template<typename _ParamDataType> __MyType& operator /= (_ParamDataType v){a /= v; r /= v; g /= v; b /= v; return *this;}

			template<typename _ParamDataType> GAIA::BL operator == (const GAIA::MATH::ARGB<_ParamDataType>& src) const{return GAIA::ALGO::cmp4(this->fptr(), src.fptr()) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const GAIA::MATH::ARGB<_ParamDataType>& src) const{return !(this->operator == (src));}
			template<typename _ParamDataType> GAIA::BL operator >= (const GAIA::MATH::ARGB<_ParamDataType>& src) const{return GAIA::ALGO::cmp4(this->fptr(), src.fptr()) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const GAIA::MATH::ARGB<_ParamDataType>& src) const{return GAIA::ALGO::cmp4(this->fptr(), src.fptr()) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const GAIA::MATH::ARGB<_ParamDataType>& src) const{return !(this->operator <= (src));}
			template<typename _ParamDataType> GAIA::BL operator < (const GAIA::MATH::ARGB<_ParamDataType>& src) const{return !(this->operator >= (src));}

			template<typename _ParamDataType> GAIA::BL operator == (const _ParamDataType& v) const{return GAIA::ALGO::cmp4k(this->fptr(), v) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const _ParamDataType& v) const{return !(this->operator == (v));}
			template<typename _ParamDataType> GAIA::BL operator >= (const _ParamDataType& v) const{return GAIA::ALGO::cmp4k(this->fptr(), v) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const _ParamDataType& v) const{return GAIA::ALGO::cmp4k(this->fptr(), v) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const _ParamDataType& v) const{return !(this->operator <= (v));}
			template<typename _ParamDataType> GAIA::BL operator < (const _ParamDataType& v) const{return !(this->operator >= (v));}

		public:
			_DataType r, g, b, a;
		};
	}
}

#endif
