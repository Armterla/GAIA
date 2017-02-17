#ifndef		__GAIA_MATH_NRGB_H__
#define		__GAIA_MATH_NRGB_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> class NRGB : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
		public:
			typedef NRGB<_DataType> __MyType;
		public:
			GINL NRGB(){}
			template<
				typename _ParamDataType1,
				typename _ParamDataType2,
				typename _ParamDataType3> NRGB(
					const _ParamDataType1& r,
					const _ParamDataType2& g,
					const _ParamDataType3& b)
			{
				this->r = GSCAST(_DataType)(r);
				this->g = GSCAST(_DataType)(g);
				this->b = GSCAST(_DataType)(b);
			}
			template<typename _ParamDataType> NRGB(const GAIA::MATH::NRGB<_ParamDataType>& src){this->operator = (src);}
			template<typename _ParamDataType> NRGB(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> NRGB(_ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> NRGB(_ParamDataType v){this->operator = (v);}

			GINL GAIA::NUM size() const{return 3;}

			GINL _DataType length() const{return GAIA::MATH::gsqrt(this->lengthsq());}
			GINL _DataType lengthsq() const{return r * r + g * g + b * b;}

			GINL GAIA::BL iszero() const{return r == (_DataType)0 && g == (_DataType)0 && b == (_DataType)0;}
			GINL GAIA::BL isfinited() const{return GAIA::MATH::gfinite(r) && GAIA::MATH::gfinite(g) && GAIA::MATH::gfinite(b);}

			GINL _DataType avg_rgb() const{return GAIA::MATH::gavg3(r, g, b);}
			GINL _DataType max_rgb() const{return GAIA::ALGO::gmax3(r, g, b);}
			GINL _DataType min_rgb() const{return GAIA::ALGO::gmin3(r, g, b);}

			GINL GAIA::GVOID torealmode(){(*this) /= 255;}
			GINL GAIA::GVOID tobytemode(){(*this) *= 255;}

			GINL GAIA::U32 tou32() const
			{
				GAIA::U32 ret;
				ret = 0;
				GRCAST(GAIA::U8*)(&ret)[2] = GSCAST(GAIA::U8)(r);
				GRCAST(GAIA::U8*)(&ret)[1] = GSCAST(GAIA::U8)(g);
				GRCAST(GAIA::U8*)(&ret)[0] = GSCAST(GAIA::U8)(b);
				return ret;
			}
			GINL GAIA::GVOID fromu32(GAIA::U32 u)
			{
				r = GSCAST(_DataType)(GRCAST(GAIA::U8*)(&u)[2]);
				g = GSCAST(_DataType)(GRCAST(GAIA::U8*)(&u)[1]);
				b = GSCAST(_DataType)(GRCAST(GAIA::U8*)(&u)[0]);
			}

			GINL const _DataType* fptr() const{return &r;}
			GINL _DataType* fptr(){return &r;}
			GINL const _DataType* bptr() const{return this->fptr() + this->size() - 1;}
			GINL _DataType* bptr(){return this->fptr() + this->size() - 1;}

			template<typename _ParamEndDataType, typename _ParamFactorDataType> GAIA::GVOID lerp(GAIA::MATH::NRGB<_ParamEndDataType>& end, const _ParamFactorDataType& factor)
			{
				r = GAIA::MATH::lerp(r, end.r, factor);
				g = GAIA::MATH::lerp(g, end.g, factor);
				b = GAIA::MATH::lerp(b, end.b, factor);
			}

			template<typename _ParamDataType> __MyType& operator = (const GAIA::MATH::NRGB<_ParamDataType>& src){r = src.r; g = src.g; b = src.b; return *this;}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p){r = p[0]; g = p[1]; b = p[2]; return *this;}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType* p){return this->operator = (GSCAST(const _ParamDataType*)(p));}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType v){r = g = b = v; return *this;}

			template<typename _ParamDataType> __MyType operator + (const GAIA::MATH::ARGB<_ParamDataType>& src) const{__MyType ret = *this; ret += src; return ret;}
			template<typename _ParamDataType> __MyType operator - (const GAIA::MATH::ARGB<_ParamDataType>& src) const{__MyType ret = *this; ret -= src; return ret;}
			template<typename _ParamDataType> __MyType operator * (const GAIA::MATH::ARGB<_ParamDataType>& src) const{__MyType ret = *this; ret *= src; return ret;}
			template<typename _ParamDataType> __MyType operator / (const GAIA::MATH::ARGB<_ParamDataType>& src) const{__MyType ret = *this; ret /= src; return ret;}
			template<typename _ParamDataType> __MyType& operator += (const GAIA::MATH::ARGB<_ParamDataType>& src){r += src.r; g += src.g; b += src.b; return *this;}
			template<typename _ParamDataType> __MyType& operator -= (const GAIA::MATH::ARGB<_ParamDataType>& src){r -= src.r; g -= src.g; b -= src.b; return *this;}
			template<typename _ParamDataType> __MyType& operator *= (const GAIA::MATH::ARGB<_ParamDataType>& src){r *= src.r; g *= src.g; b *= src.b; return *this;}
			template<typename _ParamDataType> __MyType& operator /= (const GAIA::MATH::ARGB<_ParamDataType>& src){r /= src.r; g /= src.g; b /= src.b; return *this;}

			template<typename _ParamDataType> __MyType operator + (const _ParamDataType& v) const{__MyType ret = *this; ret.r += v; ret.g += v; ret.b += v; return ret;}
			template<typename _ParamDataType> __MyType operator - (const _ParamDataType& v) const{__MyType ret = *this; ret.r -= v; ret.g -= v; ret.b -= v; return ret;}
			template<typename _ParamDataType> __MyType operator * (const _ParamDataType& v) const{__MyType ret = *this; ret.r *= v; ret.g *= v; ret.b *= v; return ret;}
			template<typename _ParamDataType> __MyType operator / (const _ParamDataType& v) const{__MyType ret = *this; ret.r /= v; ret.g /= v; ret.b /= v; return ret;}
			template<typename _ParamDataType> __MyType& operator += (_ParamDataType v){r += v; g += v; b += v; return *this;}
			template<typename _ParamDataType> __MyType& operator -= (_ParamDataType v){r -= v; g -= v; b -= v; return *this;}
			template<typename _ParamDataType> __MyType& operator *= (_ParamDataType v){r *= v; g *= v; b *= v; return *this;}
			template<typename _ParamDataType> __MyType& operator /= (_ParamDataType v){r /= v; g /= v; b /= v; return *this;}

			template<typename _ParamDataType> GAIA::BL operator == (const GAIA::MATH::NRGB<_ParamDataType>& src) const{return GAIA::ALGO::cmp3(this->fptr(), src.fptr()) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const GAIA::MATH::NRGB<_ParamDataType>& src) const{return !(this->operator == (src));}
			template<typename _ParamDataType> GAIA::BL operator >= (const GAIA::MATH::NRGB<_ParamDataType>& src) const{return GAIA::ALGO::cmp3(this->fptr(), src.fptr()) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const GAIA::MATH::NRGB<_ParamDataType>& src) const{return GAIA::ALGO::cmp3(this->fptr(), src.fptr()) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const GAIA::MATH::NRGB<_ParamDataType>& src) const{return !(this->operator <= (src));}
			template<typename _ParamDataType> GAIA::BL operator < (const GAIA::MATH::NRGB<_ParamDataType>& src) const{return !(this->operator >= (src));}

			template<typename _ParamDataType> GAIA::BL operator == (const _ParamDataType& v) const{return GAIA::ALGO::cmp3k(this->fptr(), v) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const _ParamDataType& v) const{return !(this->operator == (v));}
			template<typename _ParamDataType> GAIA::BL operator >= (const _ParamDataType& v) const{return GAIA::ALGO::cmp3k(this->fptr(), v) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const _ParamDataType& v) const{return GAIA::ALGO::cmp3k(this->fptr(), v) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const _ParamDataType& v) const{return !(this->operator <= (v));}
			template<typename _ParamDataType> GAIA::BL operator < (const _ParamDataType& v) const{return !(this->operator >= (v));}

		public:
			_DataType r, g, b;
		};
	}
}

#endif
