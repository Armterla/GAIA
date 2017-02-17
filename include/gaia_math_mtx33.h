#ifndef		__GAIA_MATH_MTX33_H__
#define		__GAIA_MATH_MTX33_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> class MTX33 : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
		public:
			typedef MTX33<_DataType> __MyType;
		public:
			GINL MTX33(){}
			GINL MTX33(const __MyType& src){this->operator = (src);}
			template<typename _ParamDataType> MTX33(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> MTX33(_ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> MTX33(const _ParamDataType& v){this->operator = (v);}
			GINL GAIA::NUM size() const{return sizeofarray2(m);}
			GINL GAIA::NUM sizex() const{return sizeofarray(m[0]);}
			GINL GAIA::NUM sizey() const{return this->size() / this->sizex();}
			GINL GAIA::GVOID identity()
			{
				m[0][0] = (_DataType)1; m[0][1] = (_DataType)0; m[0][2] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = (_DataType)1; m[1][2] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0; m[2][2] = (_DataType)1;
			}
			GINL GAIA::BL isidentity() const
			{
				if(	m[0][0] == (_DataType)1 && m[0][1] == (_DataType)0 && m[0][2] == (_DataType)0 &&
					m[1][0] == (_DataType)0 && m[1][1] == (_DataType)1 && m[1][2] == (_DataType)0 &&
					m[2][0] == (_DataType)0 && m[2][1] == (_DataType)0 && m[2][2] == (_DataType)1)
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isalmostidentity() const
			{
				if(	GAIA::MATH::gequal(m[0][0], (_DataType)1) && GAIA::MATH::gequal(m[0][1], (_DataType)0) && GAIA::MATH::gequal(m[0][2], (_DataType)0) &&
					GAIA::MATH::gequal(m[1][0], (_DataType)0) && GAIA::MATH::gequal(m[1][1], (_DataType)1) && GAIA::MATH::gequal(m[1][2], (_DataType)0) &&
					GAIA::MATH::gequal(m[2][0], (_DataType)0) && GAIA::MATH::gequal(m[2][1], (_DataType)0) && GAIA::MATH::gequal(m[2][2], (_DataType)1))
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isalmostidentity(const _DataType& epsilon) const
			{
				if(	GAIA::MATH::gequal(m[0][0], (_DataType)1, epsilon) && GAIA::MATH::gequal(m[0][1], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[0][2], (_DataType)0, epsilon) &&
					GAIA::MATH::gequal(m[1][0], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[1][1], (_DataType)1, epsilon) && GAIA::MATH::gequal(m[1][2], (_DataType)0, epsilon) &&
					GAIA::MATH::gequal(m[2][0], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[2][1], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[2][2], (_DataType)1, epsilon))
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isfinited() const
			{
				return
					GAIA::MATH::gfinite(m[0][0]) &&
					GAIA::MATH::gfinite(m[0][1]) &&
					GAIA::MATH::gfinite(m[0][2]) &&
					GAIA::MATH::gfinite(m[1][0]) &&
					GAIA::MATH::gfinite(m[1][1]) &&
					GAIA::MATH::gfinite(m[1][2]) &&
					GAIA::MATH::gfinite(m[2][0]) &&
					GAIA::MATH::gfinite(m[2][1]) &&
					GAIA::MATH::gfinite(m[2][2]);
			}
			GINL GAIA::GVOID transpose()
			{
				GAIA::ALGO::swap(m[0][1], m[1][0]);
				GAIA::ALGO::swap(m[0][2], m[2][0]);
				GAIA::ALGO::swap(m[1][2], m[2][1]);
			}
			GINL GAIA::BL inverse()
			{
				__MyType mtx = *this;
				m[0][0] = +mtx.m[2][2] * mtx.m[1][1] - mtx.m[1][2] * mtx.m[2][1];
				m[0][1] = +mtx.m[0][1] * mtx.m[2][2] + mtx.m[0][2] * mtx.m[2][1];
				m[0][2] = -mtx.m[0][2] * mtx.m[1][1] + mtx.m[0][1] * mtx.m[1][2];
				m[1][0] = +mtx.m[2][2] * mtx.m[1][0] + mtx.m[1][2] * mtx.m[2][0];
				m[1][1] = +mtx.m[0][0] * mtx.m[2][2] - mtx.m[0][2] * mtx.m[2][0];
				m[1][2] = +mtx.m[0][2] * mtx.m[1][0] - mtx.m[0][0] * mtx.m[1][2];
				m[2][0] = -mtx.m[1][1] * mtx.m[2][0] + mtx.m[1][0] * mtx.m[2][1];
				m[2][1] = +mtx.m[0][1] * mtx.m[2][0] - mtx.m[0][0] * mtx.m[2][1];
				m[2][2] = -mtx.m[0][1] * mtx.m[1][0] + mtx.m[0][0] * mtx.m[1][1];
				_DataType d = mtx.m[0][0] * m[0][0] + mtx.m[0][1] * m[1][0] + mtx.m[0][2] * m[2][0];
				GAST(!GAIA::MATH::gequalzero(d));
				if(d == 0.0)
					return GAIA::False;
				d = 1.0 / d;
				for(GAIA::NUM i = 0; i < 3; ++i)
				{
					for(GAIA::NUM j = 0; j < 3; ++j)
						m[i][j] *= d;
				}
				return GAIA::True;
			}
			template<typename _ParamDataType1, typename _ParamDataType2> GAIA::GVOID translate(const _ParamDataType1& x, const _ParamDataType2& y)
			{
				m[0][0] = (_DataType)1; m[0][1] = (_DataType)0; m[0][2] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = (_DataType)1; m[1][2] = (_DataType)0;
				m[2][0] = (_DataType)x;	m[2][1] = (_DataType)y;	m[2][2] = (_DataType)1;
			}
			template<typename _ParamDataType1, typename _ParamDataType2, typename _ParamDataType3> GAIA::GVOID translate(const _ParamDataType1& x, const _ParamDataType2& y, const _ParamDataType3& z)
			{
				m[0][0] = (_DataType)1; m[0][1] = (_DataType)0; m[0][2] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = (_DataType)1; m[1][2] = (_DataType)0;
				m[2][0] = (_DataType)x;	m[2][1] = (_DataType)y;	m[2][2] = (_DataType)z;
			}
			template<typename _ParamDataType> GAIA::GVOID rotatex(const _ParamDataType& x)
			{
				_DataType cosv = (_DataType)GAIA::MATH::gcos(x);
				_DataType sinv = (_DataType)GAIA::MATH::gsin(x);
				m[0][0] = (_DataType)1; m[0][1] = (_DataType)0; m[0][2] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = cosv;			m[1][2] = sinv;
				m[2][0] = (_DataType)0; m[2][1] = -sinv;			m[2][2] = cosv;
			}
			template<typename _ParamDataType> GAIA::GVOID rotatey(const _ParamDataType& y)
			{
				_DataType cosv = (_DataType)GAIA::MATH::gcos(y);
				_DataType sinv = (_DataType)GAIA::MATH::gsin(y);
				m[0][0] = cosv;			m[0][1] = (_DataType)0; m[0][2] = -sinv;
				m[1][0] = (_DataType)0; m[1][1] = (_DataType)1; m[1][2] = (_DataType)0;
				m[2][0] = sinv;			m[2][1] = (_DataType)0; m[2][2] = cosv;
			}
			template<typename _ParamDataType> GAIA::GVOID rotatez(const _ParamDataType& z)
			{
				_DataType cosv = (_DataType)GAIA::MATH::gcos(z);
				_DataType sinv = (_DataType)GAIA::MATH::gsin(z);
				m[0][0] = cosv;			m[0][1] = sinv;			m[0][2] = (_DataType)0;
				m[1][0] = -sinv;			m[1][1] = cosv;			m[1][2] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0; m[2][2] = (_DataType)1;
			}
			template<typename _ParamDataType1, typename _ParamDataType2, typename _ParamDataType3> GAIA::GVOID rotate(const _ParamDataType1& x, const _ParamDataType2& y, const _ParamDataType3& z)
			{
				/* This function rotate from Z to X and to Y axis, this is same to Microsoft direct3d D3DXMatrixRotatePitchYawRoll. */
				__MyType mtxRotateX, mtxRotateY, mtxRotateZ;
				mtxRotateX.rotatex(x);
				mtxRotateY.rotatey(y);
				mtxRotateZ.rotatez(z);
				*this = mtxRotateZ * mtxRotateX * mtxRotateY;
			}
			template<typename _ParamDataType1, typename _ParamDataType2> GAIA::GVOID rotate(const GAIA::MATH::VEC2<_ParamDataType1>& nor, const _ParamDataType2& radian)
			{
			}
			template<typename _ParamDataType1, typename _ParamDataType2> GAIA::GVOID scale(const _ParamDataType1& x, const _ParamDataType2& y)
			{
				m[0][0] = x;			m[0][1] = (_DataType)0; m[0][2] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = y;			m[1][2] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0; m[2][2] = (_DataType)1;
			}
			template<typename _ParamDataType1, typename _ParamDataType2, typename _ParamDataType3> GAIA::GVOID scale(const _ParamDataType1& x, const _ParamDataType2& y, const _ParamDataType3& z)
			{
				m[0][0] = x;			m[0][1] = (_DataType)0; m[0][2] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = y;			m[1][2] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0; m[2][2] = (_DataType)z;
			}
			template<typename _ParamDataType> GAIA::GVOID position(const GAIA::MATH::VEC2<_ParamDataType>& src){m[2][0] = src.x; m[2][1] = src.y;}
			GAIA::MATH::VEC2<_DataType> position() const{GAIA::MATH::VEC2<_DataType> ret; ret.x = m[2][0]; ret.y = m[2][1]; return ret;}
			template<typename _ParamEndDataType, typename _ParamFactorDataType> __MyType& lerp(const GAIA::MATH::MTX33<_ParamEndDataType>& end, const _ParamFactorDataType& factor)
			{
			}
			template<typename _ParamDataType> __MyType operator * (const GAIA::MATH::MTX33<_ParamDataType>& src) const
			{
				__MyType ret;
				ret.m[0][0] = m[0][0] * src.m[0][0] + m[0][1] * src.m[1][0] + m[0][2] * src.m[2][0];
				ret.m[0][1] = m[0][0] * src.m[0][1] + m[0][1] * src.m[1][1] + m[0][2] * src.m[2][1];
				ret.m[0][2] = m[0][0] * src.m[0][2] + m[0][1] * src.m[1][2] + m[0][2] * src.m[2][2];
				ret.m[1][0] = m[1][0] * src.m[0][0] + m[1][1] * src.m[1][0] + m[1][2] * src.m[2][0];
				ret.m[1][1] = m[1][0] * src.m[0][1] + m[1][1] * src.m[1][1] + m[1][2] * src.m[2][1];
				ret.m[1][2] = m[1][0] * src.m[0][2] + m[1][1] * src.m[1][2] + m[1][2] * src.m[2][2];
				ret.m[2][0] = m[2][0] * src.m[0][0] + m[2][1] * src.m[1][0] + m[2][2] * src.m[2][0];
				ret.m[2][1] = m[2][0] * src.m[0][1] + m[2][1] * src.m[1][1] + m[2][2] * src.m[2][1];
				ret.m[2][2] = m[2][0] * src.m[0][2] + m[2][1] * src.m[1][2] + m[2][2] * src.m[2][2];
				return ret;
			}
			template<typename _ParamDataType> __MyType& operator *= (const GAIA::MATH::MTX33<_ParamDataType>& src){__MyType temp = *this; *this = temp * src; return *this;}
			GINL const _DataType* fptr() const{return GRCAST(const _DataType*)(m);}
			GINL _DataType* fptr(){return GRCAST(_DataType*)(m);}
			GINL const _DataType* bptr() const{return this->fptr() + this->size() - 1;}
			GINL _DataType* bptr(){return this->fptr() + this->size() - 1;}
			GINL GAIA::GVOID to32(GAIA::MATH::MTX32<_DataType>& dst) const
			{
				dst.m[0][0] = m[0][0]; dst.m[0][1] = m[0][1];
				dst.m[1][0] = m[1][0]; dst.m[1][1] = m[1][1];
				dst.m[2][0] = m[2][0]; dst.m[2][1] = m[2][1];
			}
			GINL GAIA::GVOID from32(const GAIA::MATH::MTX32<_DataType>& src)
			{
				m[0][0] = src.m[0][0]; m[0][1] = src.m[0][1];
				m[1][0] = src.m[1][0]; m[1][1] = src.m[1][1];
				m[2][0] = src.m[2][0]; m[2][1] = src.m[2][1];
			}
			GINL GAIA::GVOID to22(GAIA::MATH::MTX22<_DataType>& dst) const
			{
				dst.m[0][0] = m[0][0]; dst.m[0][1] = m[0][1];
				dst.m[1][0] = m[1][0]; dst.m[1][1] = m[1][1];
			}
			GINL GAIA::GVOID from22(const GAIA::MATH::MTX22<_DataType>& src)
			{
				m[0][0] = src.m[0][0]; m[0][1] = src.m[0][1];
				m[1][0] = src.m[1][0]; m[1][1] = src.m[1][1];
			}
			template<typename _ParamDataType> __MyType& operator = (const GAIA::MATH::MTX33<_ParamDataType>& src)
			{
				m[0][0] = (_DataType)src.m[0][0]; m[0][1] = (_DataType)src.m[0][1]; m[0][2] = (_DataType)src.m[0][2];
				m[1][0] = (_DataType)src.m[1][0]; m[1][1] = (_DataType)src.m[1][1]; m[1][2] = (_DataType)src.m[1][2];
				m[2][0] = (_DataType)src.m[2][0]; m[2][1] = (_DataType)src.m[2][1]; m[2][2] = (_DataType)src.m[2][2];
				return *this;
			}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p){GAST(p != GNIL); GAIA::ALGO::copy(this->fptr(), p, this->size()); return *this;}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType* p){return this->operator = (GSCAST(const _ParamDataType*)(p));}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType& v){GAIA::ALGO::reset(this->fptr(), v, this->size()); return *this;}
			template<typename _ParamDataType> GAIA::BL operator == (const GAIA::MATH::MTX33<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const GAIA::MATH::MTX33<_ParamDataType>& src) const{return !(this->operator == (src));}
			template<typename _ParamDataType> GAIA::BL operator >= (const GAIA::MATH::MTX33<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const GAIA::MATH::MTX33<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const GAIA::MATH::MTX33<_ParamDataType>& src) const{return !(this->operator <= (src));}
			template<typename _ParamDataType> GAIA::BL operator < (const GAIA::MATH::MTX33<_ParamDataType>& src) const{return !(this->operator >= (src));}
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
			_DataType m[3][3];
		};
	}
}

#endif
