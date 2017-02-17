#ifndef		__GAIA_MATH_QUA_H__
#define		__GAIA_MATH_QUA_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> class QUA : public GAIA::Base
		{
		public:
		public:
			typedef QUA<_DataType> __MyType;
		public:
			GINL QUA(){}
			template<typename _ParamDataType> QUA(const QUA<_ParamDataType>& src){this->operator = (src);}
			template<typename _ParamDataType> QUA(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> QUA(_ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> QUA(_ParamDataType v){this->operator = (v);}
			GINL GAIA::NUM size() const{return 4;}
			GINL _DataType length() const{}
			GINL _DataType lengthsq() const{}
			GINL GAIA::GVOID normalize(){}
			GINL GAIA::GVOID try_normalize()
			{
			}
			GINL GAIA::BL isnormalized() const{return GAIA::False;}
			GINL GAIA::GVOID identity();
			GINL GAIA::BL isidentity() const{return GAIA::False;}
			GINL GAIA::BL isalmostidentity() const{return GAIA::False;}
			GINL GAIA::BL isalmostidentity(const _DataType& epsilon) const{return GAIA::False;}
			GINL GAIA::BL iszero() const{return x == (_DataType)0 && y == (_DataType)0 && z == (_DataType)0 && w == (_DataType)0;}
			GINL GAIA::BL isfinited() const{return GAIA::MATH::gfinite(x) && GAIA::MATH::gfinite(y) && GAIA::MATH::gfinite(z) && GAIA::MATH::gfinite(w);}
			GINL GAIA::GVOID inverse(){}
			GINL const _DataType* fptr() const{return &x;}
			GINL _DataType* fptr(){return &x;}
			GINL const _DataType* bptr() const{return this->fptr() + this->size() - 1;}
			GINL _DataType* bptr(){return this->fptr() + this->size() - 1;}
			template<typename _ParamDataType> _DataType dot(const GAIA::MATH::QUA<_ParamDataType>& v) const{return (x * v.x + y * v.y + z * v.z) * w * v.w;}
			template<typename _ParamDataType> __MyType num(const GAIA::MATH::QUA<_ParamDataType>& v) const{__MyType ret; ret.x = x * v.x; ret.y = y * v.y; ret.z = z * v.z; ret.w = w * v.w; return ret;}
			template<typename _ParamDataType> _DataType exp(const GAIA::MATH::QUA<_ParamDataType>& v) const{}
			template<typename _ParamEndDataType, typename _ParamFactorDataType> __MyType& slerp(const GAIA::MATH::QUA<_ParamEndDataType>& end, const _ParamFactorDataType& factor){}
			template<typename _ParamDataType> GAIA::GVOID rotatex(_ParamDataType x){}
			template<typename _ParamDataType> GAIA::GVOID rotatey(_ParamDataType y){}
			template<typename _ParamDataType> GAIA::GVOID rotatez(_ParamDataType z){}
			template<typename _ParamDataType1, typename _ParamDataType2> GAIA::GVOID rotate(const GAIA::MATH::VEC3<_ParamDataType1>& nor, const _ParamDataType2& radian){}
			template<typename _ParamDataType> __MyType operator * (const GAIA::MATH::QUA<_ParamDataType>& src) const{}
			template<typename _ParamDataType> __MyType& operator *= (const GAIA::MATH::QUA<_ParamDataType>& src) const{}
			template<typename _ParamDataType> __MyType& operator = (const GAIA::MATH::QUA<_ParamDataType>& src){}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p){}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType* p){}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType v){}
			template<typename _ParamDataType> GAIA::BL operator == (const GAIA::MATH::QUA<_ParamDataType>& src) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator != (const GAIA::MATH::QUA<_ParamDataType>& src) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator >= (const GAIA::MATH::QUA<_ParamDataType>& src) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator <= (const GAIA::MATH::QUA<_ParamDataType>& src) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator > (const GAIA::MATH::QUA<_ParamDataType>& src) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator < (const GAIA::MATH::QUA<_ParamDataType>& src) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator == (const _ParamDataType& v) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator != (const _ParamDataType& v) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator >= (const _ParamDataType& v) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator <= (const _ParamDataType& v) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator > (const _ParamDataType& v) const{return GAIA::True;}
			template<typename _ParamDataType> GAIA::BL operator < (const _ParamDataType& v) const{return GAIA::True;}
			template<typename _ParamDataType> const _DataType& operator [] (const _ParamDataType& index) const{GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
			template<typename _ParamDataType> _DataType& operator [] (const _ParamDataType& index){GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
		public:
			_DataType x, y, z, w;
		};
	}
}

#endif
