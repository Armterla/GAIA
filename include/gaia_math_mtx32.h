#ifndef		__GAIA_MATH_MTX32_H__
#define		__GAIA_MATH_MTX32_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> class MTX32 : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
		public:
			typedef MTX32<_DataType> __MyType;
		public:
			GINL MTX32(){}
			GINL MTX32(const __MyType& src){this->operator = (src);}
			template<typename _ParamDataType> MTX32(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> MTX32(_ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> MTX32(const _ParamDataType& v){this->operator = (v);}
			GINL GAIA::NUM size() const{return sizeofarray2(m);}
			GINL GAIA::NUM sizex() const{return sizeofarray(m[0]);}
			GINL GAIA::NUM sizey() const{return this->size() / this->sizex();}
			GINL GAIA::GVOID identity()
			{
				m[0][0] = (_DataType)1; m[0][1] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = (_DataType)1;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0;
			}
			GINL GAIA::BL isidentity() const
			{
				if(	m[0][0] == (_DataType)1 && m[0][1] == (_DataType)0 &&
					m[1][0] == (_DataType)0 && m[1][1] == (_DataType)1 &&
					m[2][0] == (_DataType)0 && m[2][1] == (_DataType)0)
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isalmostidentity() const
			{
				if(	GAIA::MATH::gequal(m[0][0], (_DataType)1) && GAIA::MATH::gequal(m[0][1], (_DataType)0) &&
					GAIA::MATH::gequal(m[1][0], (_DataType)0) && GAIA::MATH::gequal(m[1][1], (_DataType)1) &&
					GAIA::MATH::gequal(m[2][0], (_DataType)0) && GAIA::MATH::gequal(m[2][1], (_DataType)0))
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isalmostidentity(const _DataType& epsilon) const
			{
				if(	GAIA::MATH::gequal(m[0][0], (_DataType)1, epsilon) && GAIA::MATH::gequal(m[0][1], (_DataType)0, epsilon) &&
					GAIA::MATH::gequal(m[1][0], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[1][1], (_DataType)1, epsilon) &&
					GAIA::MATH::gequal(m[2][0], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[2][1], (_DataType)0, epsilon))
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isfinited() const
			{
				return
					GAIA::MATH::gfinite(m[0][0]) &&
					GAIA::MATH::gfinite(m[0][1]) &&
					GAIA::MATH::gfinite(m[1][0]) &&
					GAIA::MATH::gfinite(m[1][1]) &&
					GAIA::MATH::gfinite(m[2][0]) &&
					GAIA::MATH::gfinite(m[2][1]);
			}
			GINL const _DataType* fptr() const{return GRCAST(const _DataType*)(m);}
			GINL _DataType* fptr(){return GRCAST(_DataType*)(m);}
			GINL const _DataType* bptr() const{return this->fptr() + this->size() - 1;}
			GINL _DataType* bptr(){return this->fptr() + this->size() - 1;}
			template<typename _ParamDataType> __MyType& operator = (const GAIA::MATH::MTX32<_ParamDataType>& src)
			{
				m[0][0] = (_DataType)src.m[0][0]; m[0][1] = (_DataType)src.m[0][1];
				m[1][0] = (_DataType)src.m[1][0]; m[1][1] = (_DataType)src.m[1][1];
				m[2][0] = (_DataType)src.m[2][0]; m[2][1] = (_DataType)src.m[2][1];
				return *this;
			}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p){GAST(p != GNIL); GAIA::ALGO::copy(this->fptr(), p, this->size()); return *this;}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType* p){return this->operator = (GSCAST(const _ParamDataType*)(p));}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType& v){GAIA::ALGO::reset(this->fptr(), v, this->size()); return *this;}
			template<typename _ParamDataType> GAIA::BL operator == (const GAIA::MATH::MTX32<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const GAIA::MATH::MTX32<_ParamDataType>& src) const{return !(this->operator == (src));}
			template<typename _ParamDataType> GAIA::BL operator >= (const GAIA::MATH::MTX32<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const GAIA::MATH::MTX32<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const GAIA::MATH::MTX32<_ParamDataType>& src) const{return !(this->operator <= (src));}
			template<typename _ParamDataType> GAIA::BL operator < (const GAIA::MATH::MTX32<_ParamDataType>& src) const{return !(this->operator >= (src));}
			template<typename _ParamDataType> GAIA::BL operator == (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) != 0;}
			template<typename _ParamDataType> GAIA::BL operator >= (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) > 0;}
			template<typename _ParamDataType> GAIA::BL operator < (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) < 0;}
			template<typename _ParamDataType> const _DataType& operator [] (const _ParamDataType& index) const{GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
			template<typename _ParamDataType> _DataType& operator [] (const _ParamDataType& index){GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
			template<typename _ParamDataType1, typename _ParamDataType2> _DataType& operator () (const _ParamDataType1& i1, const _ParamDataType2& i2){GAST(i1 >= 0 && i1 < 3); GAST(i2 >= 0 && i2 < 3); return m[i1][i2];}
		public:
			_DataType m[3][2];
		};
	}
}

#endif
