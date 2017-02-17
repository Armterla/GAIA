#ifndef		__GAIA_MATH_MTX44_H__
#define		__GAIA_MATH_MTX44_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_math_linearalgebra.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> class MTX44 : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
		public:
			typedef MTX44<_DataType> __MyType;
		public:
			GINL MTX44(){}
			GINL MTX44(const __MyType& src){this->operator = (src);}
			template<typename _ParamDataType> MTX44(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> MTX44(_ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> MTX44(const _ParamDataType& v){this->operator = (v);}
			GINL GAIA::NUM size() const{return sizeofarray2(m);}
			GINL GAIA::NUM sizex() const{return sizeofarray(m[0]);}
			GINL GAIA::NUM sizey() const{return this->size() / this->sizex();}
			GINL GAIA::GVOID identity()
			{
				m[0][0] = (_DataType)1; m[0][1] = (_DataType)0; m[0][2] = (_DataType)0; m[0][3] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = (_DataType)1; m[1][2] = (_DataType)0; m[1][3] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0; m[2][2] = (_DataType)1; m[2][3] = (_DataType)0;
				m[3][0] = (_DataType)0; m[3][1] = (_DataType)0; m[3][2] = (_DataType)0; m[3][3] = (_DataType)1;
			}
			GINL GAIA::BL isidentity() const
			{
				if(	m[0][0] == (_DataType)1 && m[0][1] == (_DataType)0 && m[0][2] == (_DataType)0 && m[0][3] == (_DataType)0 &&
					m[1][0] == (_DataType)0 && m[1][1] == (_DataType)1 && m[1][2] == (_DataType)0 && m[1][3] == (_DataType)0 &&
					m[2][0] == (_DataType)0 && m[2][1] == (_DataType)0 && m[2][2] == (_DataType)1 && m[2][3] == (_DataType)0 &&
					m[3][0] == (_DataType)0 && m[3][1] == (_DataType)0 && m[3][2] == (_DataType)0 && m[3][3] == (_DataType)1)
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isalmostidentity() const
			{
				if(	GAIA::MATH::gequal(m[0][0], (_DataType)1) && GAIA::MATH::gequal(m[0][1], (_DataType)0) && GAIA::MATH::gequal(m[0][2], (_DataType)0) && GAIA::MATH::gequal(m[0][3], (_DataType)0) &&
					GAIA::MATH::gequal(m[1][0], (_DataType)0) && GAIA::MATH::gequal(m[1][1], (_DataType)1) && GAIA::MATH::gequal(m[1][2], (_DataType)0) && GAIA::MATH::gequal(m[1][3], (_DataType)0) &&
					GAIA::MATH::gequal(m[2][0], (_DataType)0) && GAIA::MATH::gequal(m[2][1], (_DataType)0) && GAIA::MATH::gequal(m[2][2], (_DataType)1) && GAIA::MATH::gequal(m[2][3], (_DataType)0) &&
					GAIA::MATH::gequal(m[3][0], (_DataType)0) && GAIA::MATH::gequal(m[3][1], (_DataType)0) && GAIA::MATH::gequal(m[3][2], (_DataType)0) && GAIA::MATH::gequal(m[3][3], (_DataType)1))
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isalmostidentity(const _DataType& epsilon) const
			{
				if(	GAIA::MATH::gequal(m[0][0], (_DataType)1, epsilon) && GAIA::MATH::gequal(m[0][1], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[0][2], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[0][3], (_DataType)0, epsilon) &&
					GAIA::MATH::gequal(m[1][0], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[1][1], (_DataType)1, epsilon) && GAIA::MATH::gequal(m[1][2], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[1][3], (_DataType)0, epsilon) &&
					GAIA::MATH::gequal(m[2][0], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[2][1], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[2][2], (_DataType)1, epsilon) && GAIA::MATH::gequal(m[2][3], (_DataType)0, epsilon) &&
					GAIA::MATH::gequal(m[3][0], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[3][1], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[3][2], (_DataType)0, epsilon) && GAIA::MATH::gequal(m[3][3], (_DataType)1, epsilon))
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::BL isfinited() const
			{
				return
					GAIA::MATH::gfinite(m[0][0]) &&
					GAIA::MATH::gfinite(m[0][1]) &&
					GAIA::MATH::gfinite(m[0][2]) &&
					GAIA::MATH::gfinite(m[0][3]) &&
					GAIA::MATH::gfinite(m[1][0]) &&
					GAIA::MATH::gfinite(m[1][1]) &&
					GAIA::MATH::gfinite(m[1][2]) &&
					GAIA::MATH::gfinite(m[1][3]) &&
					GAIA::MATH::gfinite(m[2][0]) &&
					GAIA::MATH::gfinite(m[2][1]) &&
					GAIA::MATH::gfinite(m[2][2]) &&
					GAIA::MATH::gfinite(m[2][3]) &&
					GAIA::MATH::gfinite(m[3][0]) &&
					GAIA::MATH::gfinite(m[3][1]) &&
					GAIA::MATH::gfinite(m[3][2]) &&
					GAIA::MATH::gfinite(m[3][3]);
			}
			GINL GAIA::GVOID transpose()
			{
				GAIA::ALGO::swap(m[0][1], m[1][0]);
				GAIA::ALGO::swap(m[0][2], m[2][0]);
				GAIA::ALGO::swap(m[0][3], m[3][0]);
				GAIA::ALGO::swap(m[1][2], m[2][1]);
				GAIA::ALGO::swap(m[1][3], m[3][1]);
				GAIA::ALGO::swap(m[2][3], m[3][2]);
			}
			GINL GAIA::BL inverse()
			{
			#if 0 // Come from Mathematica.
				__MyType mtx = *this;
				m[0][0] = -mtx.m[2][3] * mtx.m[3][2] * mtx.m[1][1] + mtx.m[2][2] * mtx.m[3][3] * mtx.m[1][1] + mtx.m[2][3] * mtx.m[3][1] * mtx.m[1][2] - mtx.m[2][2] * mtx.m[3][1] * mtx.m[1][3] - mtx.m[3][3] * mtx.m[1][2] * mtx.m[2][1] + mtx.m[3][2] * mtx.m[1][3] * mtx.m[2][1];
				m[0][1] = +mtx.m[0][1] * mtx.m[2][3] * mtx.m[3][2] - mtx.m[0][1] * mtx.m[2][2] * mtx.m[3][3] - mtx.m[2][3] * mtx.m[3][1] * mtx.m[0][2] + mtx.m[2][2] * mtx.m[3][1] * mtx.m[0][3] + mtx.m[3][3] * mtx.m[0][2] * mtx.m[2][1] - mtx.m[3][2] * mtx.m[0][3] * mtx.m[2][1];
				m[0][2] = -mtx.m[3][3] * mtx.m[0][2] * mtx.m[1][1] + mtx.m[3][2] * mtx.m[0][3] * mtx.m[1][1] + mtx.m[0][1] * mtx.m[3][3] * mtx.m[1][2] - mtx.m[3][1] * mtx.m[0][3] * mtx.m[1][2] - mtx.m[0][1] * mtx.m[3][2] * mtx.m[1][3] + mtx.m[3][1] * mtx.m[0][2] * mtx.m[1][3];
				m[0][3] = +mtx.m[2][3] * mtx.m[0][2] * mtx.m[1][1] - mtx.m[2][2] * mtx.m[0][3] * mtx.m[1][1] - mtx.m[0][1] * mtx.m[2][3] * mtx.m[1][2] + mtx.m[0][1] * mtx.m[2][2] * mtx.m[1][3] + mtx.m[0][3] * mtx.m[1][2] * mtx.m[2][1] - mtx.m[0][2] * mtx.m[1][3] * mtx.m[2][1];
				m[1][0] = +mtx.m[2][3] * mtx.m[3][2] * mtx.m[1][0] - mtx.m[2][2] * mtx.m[3][3] * mtx.m[1][0] - mtx.m[2][3] * mtx.m[3][0] * mtx.m[1][2] + mtx.m[2][2] * mtx.m[3][0] * mtx.m[1][3] + mtx.m[3][3] * mtx.m[1][2] * mtx.m[2][0] - mtx.m[3][2] * mtx.m[1][3] * mtx.m[2][0];
				m[1][1] = -mtx.m[0][0] * mtx.m[2][3] * mtx.m[3][2] + mtx.m[0][0] * mtx.m[2][2] * mtx.m[3][3] + mtx.m[2][3] * mtx.m[3][0] * mtx.m[0][2] - mtx.m[2][2] * mtx.m[3][0] * mtx.m[0][3] - mtx.m[3][3] * mtx.m[0][2] * mtx.m[2][0] + mtx.m[3][2] * mtx.m[0][3] * mtx.m[2][0];
				m[1][2] = +mtx.m[3][3] * mtx.m[0][2] * mtx.m[1][0] - mtx.m[3][2] * mtx.m[0][3] * mtx.m[1][0] - mtx.m[0][0] * mtx.m[3][3] * mtx.m[1][2] + mtx.m[3][0] * mtx.m[0][3] * mtx.m[1][2] + mtx.m[0][0] * mtx.m[3][2] * mtx.m[1][3] - mtx.m[3][0] * mtx.m[0][2] * mtx.m[1][3];
				m[1][3] = -mtx.m[2][3] * mtx.m[0][2] * mtx.m[1][0] + mtx.m[2][2] * mtx.m[0][3] * mtx.m[1][0] + mtx.m[0][0] * mtx.m[2][3] * mtx.m[1][2] - mtx.m[0][0] * mtx.m[2][2] * mtx.m[1][3] - mtx.m[0][3] * mtx.m[1][2] * mtx.m[2][0] + mtx.m[0][2] * mtx.m[1][3] * mtx.m[2][0];
				m[2][0] = -mtx.m[2][3] * mtx.m[3][1] * mtx.m[1][0] + mtx.m[2][3] * mtx.m[3][0] * mtx.m[1][1] - mtx.m[3][3] * mtx.m[1][1] * mtx.m[2][0] + mtx.m[3][1] * mtx.m[1][3] * mtx.m[2][0] + mtx.m[3][3] * mtx.m[1][0] * mtx.m[2][1] - mtx.m[3][0] * mtx.m[1][3] * mtx.m[2][1];
				m[2][1] = -mtx.m[0][1] * mtx.m[2][3] * mtx.m[3][0] + mtx.m[0][0] * mtx.m[2][3] * mtx.m[3][1] + mtx.m[0][1] * mtx.m[3][3] * mtx.m[2][0] - mtx.m[3][1] * mtx.m[0][3] * mtx.m[2][0] - mtx.m[0][0] * mtx.m[3][3] * mtx.m[2][1] + mtx.m[3][0] * mtx.m[0][3] * mtx.m[2][1];
				m[2][2] = -mtx.m[0][1] * mtx.m[3][3] * mtx.m[1][0] + mtx.m[3][1] * mtx.m[0][3] * mtx.m[1][0] + mtx.m[0][0] * mtx.m[3][3] * mtx.m[1][1] - mtx.m[3][0] * mtx.m[0][3] * mtx.m[1][1] + mtx.m[0][1] * mtx.m[3][0] * mtx.m[1][3] - mtx.m[0][0] * mtx.m[3][1] * mtx.m[1][3];
				m[2][3] = +mtx.m[0][1] * mtx.m[2][3] * mtx.m[1][0] - mtx.m[0][0] * mtx.m[2][3] * mtx.m[1][1] + mtx.m[0][3] * mtx.m[1][1] * mtx.m[2][0] - mtx.m[0][1] * mtx.m[1][3] * mtx.m[2][0] - mtx.m[0][3] * mtx.m[1][0] * mtx.m[2][1] + mtx.m[0][0] * mtx.m[1][3] * mtx.m[2][1];
				m[3][0] = +mtx.m[2][2] * mtx.m[3][1] * mtx.m[1][0] - mtx.m[2][2] * mtx.m[3][0] * mtx.m[1][1] + mtx.m[3][2] * mtx.m[1][1] * mtx.m[2][0] - mtx.m[3][1] * mtx.m[1][2] * mtx.m[2][0] - mtx.m[3][2] * mtx.m[1][0] * mtx.m[2][1] + mtx.m[3][0] * mtx.m[1][2] * mtx.m[2][1];
				m[3][1] = +mtx.m[0][1] * mtx.m[2][2] * mtx.m[3][0] - mtx.m[0][0] * mtx.m[2][2] * mtx.m[3][1] - mtx.m[0][1] * mtx.m[3][2] * mtx.m[2][0] + mtx.m[3][1] * mtx.m[0][2] * mtx.m[2][0] + mtx.m[0][0] * mtx.m[3][2] * mtx.m[2][1] - mtx.m[3][0] * mtx.m[0][2] * mtx.m[2][1];
				m[3][2] = +mtx.m[0][1] * mtx.m[3][2] * mtx.m[1][0] - mtx.m[3][1] * mtx.m[0][2] * mtx.m[1][0] - mtx.m[0][0] * mtx.m[3][2] * mtx.m[1][1] + mtx.m[3][0] * mtx.m[0][2] * mtx.m[1][1] - mtx.m[0][1] * mtx.m[3][0] * mtx.m[1][2] + mtx.m[0][0] * mtx.m[3][1] * mtx.m[1][2];
				m[3][3] = -mtx.m[0][1] * mtx.m[2][2] * mtx.m[1][0] + mtx.m[0][0] * mtx.m[2][2] * mtx.m[1][1] - mtx.m[0][2] * mtx.m[1][1] * mtx.m[2][0] + mtx.m[0][1] * mtx.m[1][2] * mtx.m[2][0] + mtx.m[0][2] * mtx.m[1][0] * mtx.m[2][1] - mtx.m[0][0] * mtx.m[1][2] * mtx.m[2][1];
				_DataType d = mtx.m[0][0] * m[0][0] + mtx.m[0][1] * m[1][0] + mtx.m[0][2] * m[2][0] + mtx.m[0][3] * m[3][0];
				GAST(!GAIA::MATH::xequalzero(d));
				if(d == 0.0)
					return GAIA::False;
				d = 1.0 / d;
				for(GAIA::NUM i = 0; i < 4; ++i)
				{
					for(GAIA::NUM j = 0; j < 4; ++j)
						m[i][j] *= d;
				}
			#else // Come from DOOM3
				_DataType r0[2][2], r1[2][2], r2[2][2], r3[2][2];
				_DataType a, det, invdet;
				_DataType* m = GRCAST(_DataType*)(this);
				det = m[0] * m[5] - m[1] * m[4];
				if(GAIA::MATH::gabs( det ) < +0.00001)
				{
					GASTFALSE;
					return GAIA::False;
				}
				invdet = 1.0 / det;
				r0[0][0] = +m[5] * invdet;
				r0[0][1] = -m[1] * invdet;
				r0[1][0] = -m[4] * invdet;
				r0[1][1] = +m[0] * invdet;
				r1[0][0] = r0[0][0] * m[2] + r0[0][1] * m[6];
				r1[0][1] = r0[0][0] * m[3] + r0[0][1] * m[7];
				r1[1][0] = r0[1][0] * m[2] + r0[1][1] * m[6];
				r1[1][1] = r0[1][0] * m[3] + r0[1][1] * m[7];
				r2[0][0] = m[8] * r1[0][0] + m[9] * r1[1][0];
				r2[0][1] = m[8] * r1[0][1] + m[9] * r1[1][1];
				r2[1][0] = m[12] * r1[0][0] + m[13] * r1[1][0];
				r2[1][1] = m[12] * r1[0][1] + m[13] * r1[1][1];
				r3[0][0] = r2[0][0] - m[10];
				r3[0][1] = r2[0][1] - m[11];
				r3[1][0] = r2[1][0] - m[14];
				r3[1][1] = r2[1][1] - m[15];
				det = r3[0][0] * r3[1][1] - r3[0][1] * r3[1][0];
				if(GAIA::MATH::gabs(det) < +0.00001)
				{
					GASTFALSE;
					return GAIA::False;
				}
				invdet = 1.0 / det;
				a = r3[0][0];
				r3[0][0] =  r3[1][1] * invdet;
				r3[0][1] = -r3[0][1] * invdet;
				r3[1][0] = -r3[1][0] * invdet;
				r3[1][1] = a * invdet;
				r2[0][0] = m[8] * r0[0][0] + m[9] * r0[1][0];
				r2[0][1] = m[8] * r0[0][1] + m[9] * r0[1][1];
				r2[1][0] = m[12] * r0[0][0] + m[13] * r0[1][0];
				r2[1][1] = m[12] * r0[0][1] + m[13] * r0[1][1];
				m[2*4+0] = r3[0][0] * r2[0][0] + r3[0][1] * r2[1][0];
				m[2*4+1] = r3[0][0] * r2[0][1] + r3[0][1] * r2[1][1];
				m[3*4+0] = r3[1][0] * r2[0][0] + r3[1][1] * r2[1][0];
				m[3*4+1] = r3[1][0] * r2[0][1] + r3[1][1] * r2[1][1];
				m[0*4+0] = r0[0][0] - r1[0][0] * m[8] - r1[0][1] * m[12];
				m[0*4+1] = r0[0][1] - r1[0][0] * m[9] - r1[0][1] * m[13];
				m[1*4+0] = r0[1][0] - r1[1][0] * m[8] - r1[1][1] * m[12];
				m[1*4+1] = r0[1][1] - r1[1][0] * m[9] - r1[1][1] * m[13];
				m[0*4+2] = r1[0][0] * r3[0][0] + r1[0][1] * r3[1][0];
				m[0*4+3] = r1[0][0] * r3[0][1] + r1[0][1] * r3[1][1];
				m[1*4+2] = r1[1][0] * r3[0][0] + r1[1][1] * r3[1][0];
				m[1*4+3] = r1[1][0] * r3[0][1] + r1[1][1] * r3[1][1];
				m[2*4+2] = -r3[0][0];
				m[2*4+3] = -r3[0][1];
				m[3*4+2] = -r3[1][0];
				m[3*4+3] = -r3[1][1];
			#endif
				return GAIA::True;
			}
			GINL GAIA::GVOID look(const GAIA::MATH::VEC3<_DataType>& pos, const GAIA::MATH::VEC3<_DataType>& lookat, const GAIA::MATH::VEC3<_DataType>& up)
			{
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				GAIA::MATH::VEC3<_DataType> zaxis = lookat - pos;
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				GAIA::MATH::VEC3<_DataType> zaxis = pos - lookat;
			#endif
				GAST(!GAIA::MATH::gequalzero(zaxis.lengthsq()));
				zaxis.normalize();
				GAIA::MATH::VEC3<_DataType> xaxis = up.cross(zaxis);
				GAST(!GAIA::MATH::gequalzero(xaxis.lengthsq()));
				xaxis.normalize();
				GAIA::MATH::VEC3<_DataType> yaxis = zaxis.cross(xaxis);
				m[0][0] = xaxis.x;			m[0][1] = yaxis.x;			m[0][2] = zaxis.x;			m[0][3] = (_DataType)0;
				m[1][0] = xaxis.y;			m[1][1] = yaxis.y;			m[1][2] = zaxis.y;			m[1][3] = (_DataType)0;
				m[2][0] = xaxis.z;			m[2][1] = yaxis.z;			m[2][2] = zaxis.z;			m[2][3] = (_DataType)0;
				m[3][0] = -xaxis.dot(pos);	m[3][1] = -yaxis.dot(pos);	m[3][2] = -zaxis.dot(pos);	m[3][3] = (_DataType)1;
			}
			GINL GAIA::GVOID ortho(const _DataType& width, const _DataType& height, const _DataType& znear, const _DataType& zfar)
			{
				_DataType fac = znear - zfar;
				m[0][0] = (_DataType)2 / width;	m[0][1] = (_DataType)0;				m[0][2] = (_DataType)0;			m[0][3] = (_DataType)0;
				m[1][0] = (_DataType)0;			m[1][1] = (_DataType)2 / height;	m[1][2] = (_DataType)0;			m[1][3] = (_DataType)0;
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				m[2][0] = (_DataType)0;			m[2][1] = (_DataType)0;				m[2][2] = (_DataType)-1 / fac;	m[2][3] = (_DataType)0;
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				m[2][0] = (_DataType)0;			m[2][1] = (_DataType)0;				m[2][2] = (_DataType)1 / fac;	m[2][3] = (_DataType)0;
			#endif
				m[3][0] = (_DataType)0;			m[3][1] = (_DataType)0;				m[3][2] = znear / fac;			m[3][3] = (_DataType)1;
			}
			GINL _DataType ortho_width() const
			{
				return 2.0 / m[0][0];
			}
			GINL _DataType ortho_height() const
			{
				return 2.0 / m[1][1];
			}
			GINL _DataType ortho_znear() const
			{
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				return -1.0 / m[2][2] * m[3][2];
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				return +1.0 / m[2][2] * m[3][2];
			#endif
				
			}
			GINL _DataType ortho_zfar() const
			{
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				return this->ortho_znear() + 1.0 / m[2][2];
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				return this->ortho_znear() - 1.0 / m[2][2];
			#endif
			}
			GINL GAIA::GVOID perspective(const _DataType& width, const _DataType& height, const _DataType& znear, const _DataType& zfar)
			{
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				_DataType fac = zfar / (zfar - znear);
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				_DataType fac = zfar / (znear - zfar);
			#endif
				m[0][0] = (_DataType)2 * znear / width;	m[0][1] = (_DataType)0;					m[0][2] = (_DataType)0;				m[0][3] = (_DataType)0;
				m[1][0] = (_DataType)0;					m[1][1] = (_DataType)2 * znear / height;m[1][2] = (_DataType)0;				m[1][3] = (_DataType)0;
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				m[2][0] = (_DataType)0;					m[2][1] = (_DataType)0;					m[2][2] = fac;						m[2][3] = (_DataType)1;
				m[3][0] = (_DataType)0;					m[3][1] = (_DataType)0;					m[3][2] = -znear * fac;				m[3][3] = (_DataType)0;
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				m[2][0] = (_DataType)0;					m[2][1] = (_DataType)0;					m[2][2] = fac;						m[2][3] = (_DataType)-1;
				m[3][0] = (_DataType)0;					m[3][1] = (_DataType)0;					m[3][2] = znear * fac;				m[3][3] = (_DataType)0;
			#endif
			}
			GINL GAIA::GVOID perspectivefov(const _DataType& fovy, const _DataType& aspect, const _DataType& znear, const _DataType& zfar)
			{
				_DataType ys = (_DataType)1 / GAIA::MATH::gtan(fovy / (_DataType)2);
				_DataType xs = ys / aspect;
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				_DataType fac = zfar / (zfar - znear);
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				_DataType fac = zfar / (znear - zfar);
			#endif
				m[0][0] = xs;				m[0][1] = (_DataType)0;		m[0][2] = (_DataType)0;				m[0][3] = (_DataType)0;
				m[1][0] = (_DataType)0;		m[1][1] = ys;				m[1][2] = (_DataType)0;				m[1][3] = (_DataType)0;
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				m[2][0] = (_DataType)0;		m[2][1] = (_DataType)0;		m[2][2] = fac;						m[2][3] = (_DataType)1;
				m[3][0] = (_DataType)0;		m[3][1] = (_DataType)0;		m[3][2] = -znear * fac;				m[3][3] = (_DataType)0;
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				m[2][0] = (_DataType)0;		m[2][1] = (_DataType)0;		m[2][2] = fac;						m[2][3] = (_DataType)-1;
				m[3][0] = (_DataType)0;		m[3][1] = (_DataType)0;		m[3][2] = znear * fac;				m[3][3] = (_DataType)0;
			#endif
			}
			GINL _DataType perspective_width() const
			{
				return (_DataType)2 * this->perspective_znear() / m[0][0];
			}
			GINL _DataType perspective_height() const
			{
				return (_DataType)2 * this->perspective_znear() / m[1][1];
			}
			GINL _DataType perspective_fovy() const
			{
				return GAIA::MATH::gatan((_DataType)1 / m[1][1]) * (_DataType)2;
			}
			GINL _DataType perspective_aspect() const
			{
				return m[1][1] / m[0][0];
			}
			GINL _DataType perspective_znear() const
			{
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				return -m[3][2] / m[2][2];
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				return m[3][2] / m[2][2];
			#endif
			}
			GINL _DataType perspective_zfar() const
			{
			#if GAIA_COORDINATE == GAIA_COORDINATE_LEFTHAND
				return m[3][2] / (1 - m[2][2]);
			#elif GAIA_COORDINATE == GAIA_COORDINATE_RIGHTHAND
				return m[3][2] / (1 + m[2][2]);
			#endif
			}
			GINL GAIA::BL isortho() const{return GAIA::MATH::gequal(m[3][3], (_DataType)1);}
			GINL GAIA::GVOID project(GAIA::BL bOrtho, const _DataType& width, const _DataType& height, const _DataType& znear, const _DataType& zfar)
			{
				if(bOrtho)
					this->ortho(width, height, znear, zfar);
				else
					this->perspective(width, height, znear, zfar);
			}
			GINL _DataType project_width() const
			{
				if(this->isortho())
					return this->ortho_width();
				else
					return this->perspective_width();
			}
			GINL _DataType project_height() const
			{
				if(this->isortho())
					return this->ortho_height();
				else
					return this->perspective_height();
			}
			GINL _DataType project_znear() const
			{
				if(this->isortho())
					return this->ortho_znear();
				else
					return this->perspective_znear();
			}
			GINL _DataType project_zfar() const
			{
				if(this->isortho())
					return this->ortho_zfar();
				else
					return this->perspective_zfar();
			}
			template<typename _ParamDataType1, typename _ParamDataType2, typename _ParamDataType3> GAIA::GVOID translate(const _ParamDataType1& x, const _ParamDataType2& y, const _ParamDataType3& z)
			{
				m[0][0] = (_DataType)1; m[0][1] = (_DataType)0; m[0][2] = (_DataType)0; m[0][3] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = (_DataType)1; m[1][2] = (_DataType)0; m[1][3] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0; m[2][2] = (_DataType)1; m[2][3] = (_DataType)0;
				m[3][0] = (_DataType)x;	m[3][1] = (_DataType)y;	m[3][2] = (_DataType)z;	m[3][3] = (_DataType)1;
			}
			template<typename _ParamDataType> GAIA::GVOID rotatex(const _ParamDataType& x)
			{
				_DataType cosv = (_DataType)GAIA::MATH::gcos(x);
				_DataType sinv = (_DataType)GAIA::MATH::gsin(x);
				m[0][0] = (_DataType)1; m[0][1] = (_DataType)0; m[0][2] = (_DataType)0; m[0][3] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = cosv;			m[1][2] = sinv;			m[1][3] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = -sinv;			m[2][2] = cosv;			m[2][3] = (_DataType)0;
				m[3][0] = (_DataType)0; m[3][1] = (_DataType)0; m[3][2] = (_DataType)0; m[3][3] = (_DataType)1;
			}
			template<typename _ParamDataType> GAIA::GVOID rotatey(const _ParamDataType& y)
			{
				_DataType cosv = (_DataType)GAIA::MATH::gcos(y);
				_DataType sinv = (_DataType)GAIA::MATH::gsin(y);
				m[0][0] = cosv;			m[0][1] = (_DataType)0; m[0][2] = -sinv;		m[0][3] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = (_DataType)1; m[1][2] = (_DataType)0; m[1][3] = (_DataType)0;
				m[2][0] = sinv;			m[2][1] = (_DataType)0; m[2][2] = cosv;			m[2][3] = (_DataType)0;
				m[3][0] = (_DataType)0; m[3][1] = (_DataType)0; m[3][2] = (_DataType)0; m[3][3] = (_DataType)1;
			}
			template<typename _ParamDataType> GAIA::GVOID rotatez(const _ParamDataType& z)
			{
				_DataType cosv = (_DataType)GAIA::MATH::gcos(z);
				_DataType sinv = (_DataType)GAIA::MATH::gsin(z);
				m[0][0] = cosv;			m[0][1] = sinv;			m[0][2] = (_DataType)0; m[0][3] = (_DataType)0;
				m[1][0] = -sinv;			m[1][1] = cosv;			m[1][2] = (_DataType)0; m[1][3] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0; m[2][2] = (_DataType)1; m[2][3] = (_DataType)0;
				m[3][0] = (_DataType)0; m[3][1] = (_DataType)0; m[3][2] = (_DataType)0; m[3][3] = (_DataType)1;
			}
			template<typename _ParamDataType1, typename _ParamDataType2, typename _ParamDataType3> GAIA::GVOID rotate(const _ParamDataType1& x, const _ParamDataType2& y, const _ParamDataType3& z)
			{
				GAIA::MATH::MTX33<_DataType> mtx33;
				mtx33.rotate(x, y, z);
				this->from33(mtx33);
				m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = (_DataType)0;
				m[3][3] = (_DataType)1;
			}
			template<typename _ParamDataType1, typename _ParamDataType2> GAIA::GVOID rotate(const GAIA::MATH::VEC3<_ParamDataType1>& nor, const _ParamDataType2& radian)
			{
				_DataType cosv = (_DataType)GAIA::MATH::gcos(radian);
				_DataType sinv = (_DataType)GAIA::MATH::gsin(radian);
				_DataType cosvi = (_DataType)1 - cosv;
				m[0][0] = nor.x * nor.x * cosvi + cosv;			m[0][1] = nor.x * nor.y * cosvi + nor.z * sinv; m[0][2] = nor.x * nor.z * cosvi - nor.y * sinv; m[0][3] = (_DataType)0;
				m[1][0] = nor.x * nor.y * cosvi - nor.z * sinv; m[1][1] = nor.y * nor.y * cosvi + cosv;			m[1][2] = nor.y * nor.z * cosvi + nor.x * sinv; m[1][3] = (_DataType)0;
				m[2][0] = nor.x * nor.z * cosvi + nor.y * sinv; m[2][1] = nor.y * nor.z * cosvi - nor.x * sinv; m[2][2] = nor.z * nor.z * cosvi + cosv;			m[2][3] = (_DataType)0;
				m[3][0] = (_DataType)0;							m[3][1] = (_DataType)0;							m[3][2] = (_DataType)0;							m[3][3] = (_DataType)1;
			}
			template<typename _ParamDataType1, typename _ParamDataType2, typename _ParamDataType3> GAIA::GVOID scale(const _ParamDataType1& x, const _ParamDataType2& y, const _ParamDataType3& z)
			{
				m[0][0] = x;			m[0][1] = (_DataType)0; m[0][2] = (_DataType)0; m[0][3] = (_DataType)0;
				m[1][0] = (_DataType)0; m[1][1] = y;			m[1][2] = (_DataType)0; m[1][3] = (_DataType)0;
				m[2][0] = (_DataType)0; m[2][1] = (_DataType)0; m[2][2] = z;			m[2][3] = (_DataType)0;
				m[3][0] = (_DataType)0; m[3][1] = (_DataType)0; m[3][2] = (_DataType)0; m[3][3] = (_DataType)1;
			}
			template<typename _ParamDataType> GAIA::GVOID position(const GAIA::MATH::VEC3<_ParamDataType>& src){m[3][0] = src.x; m[3][1] = src.y; m[3][2] = src.z;}
			GAIA::MATH::VEC3<_DataType> position() const{GAIA::MATH::VEC3<_DataType> ret; ret.x = m[3][0]; ret.y = m[3][1]; ret.z = m[3][2]; return ret;}
			template<typename _ParamEndDataType, typename _ParamFactorDataType> __MyType& lerp(const GAIA::MATH::MTX44<_ParamEndDataType>& end, const _ParamFactorDataType& factor)
			{
			}
			template<typename _ParamDataType> __MyType operator * (const GAIA::MATH::MTX44<_ParamDataType>& src) const
			{
				__MyType ret;
				ret.m[0][0] = m[0][0] * src.m[0][0] + m[0][1] * src.m[1][0] + m[0][2] * src.m[2][0] + m[0][3] * src.m[3][0];
				ret.m[0][1] = m[0][0] * src.m[0][1] + m[0][1] * src.m[1][1] + m[0][2] * src.m[2][1] + m[0][3] * src.m[3][1];
				ret.m[0][2] = m[0][0] * src.m[0][2] + m[0][1] * src.m[1][2] + m[0][2] * src.m[2][2] + m[0][3] * src.m[3][2];
				ret.m[0][3] = m[0][0] * src.m[0][3] + m[0][1] * src.m[1][3] + m[0][2] * src.m[2][3] + m[0][3] * src.m[3][3];
				ret.m[1][0] = m[1][0] * src.m[0][0] + m[1][1] * src.m[1][0] + m[1][2] * src.m[2][0] + m[1][3] * src.m[3][0];
				ret.m[1][1] = m[1][0] * src.m[0][1] + m[1][1] * src.m[1][1] + m[1][2] * src.m[2][1] + m[1][3] * src.m[3][1];
				ret.m[1][2] = m[1][0] * src.m[0][2] + m[1][1] * src.m[1][2] + m[1][2] * src.m[2][2] + m[1][3] * src.m[3][2];
				ret.m[1][3] = m[1][0] * src.m[0][3] + m[1][1] * src.m[1][3] + m[1][2] * src.m[2][3] + m[1][3] * src.m[3][3];
				ret.m[2][0] = m[2][0] * src.m[0][0] + m[2][1] * src.m[1][0] + m[2][2] * src.m[2][0] + m[2][3] * src.m[3][0];
				ret.m[2][1] = m[2][0] * src.m[0][1] + m[2][1] * src.m[1][1] + m[2][2] * src.m[2][1] + m[2][3] * src.m[3][1];
				ret.m[2][2] = m[2][0] * src.m[0][2] + m[2][1] * src.m[1][2] + m[2][2] * src.m[2][2] + m[2][3] * src.m[3][2];
				ret.m[2][3] = m[2][0] * src.m[0][3] + m[2][1] * src.m[1][3] + m[2][2] * src.m[2][3] + m[2][3] * src.m[3][3];
				ret.m[3][0] = m[3][0] * src.m[0][0] + m[3][1] * src.m[1][0] + m[3][2] * src.m[2][0] + m[3][3] * src.m[3][0];
				ret.m[3][1] = m[3][0] * src.m[0][1] + m[3][1] * src.m[1][1] + m[3][2] * src.m[2][1] + m[3][3] * src.m[3][1];
				ret.m[3][2] = m[3][0] * src.m[0][2] + m[3][1] * src.m[1][2] + m[3][2] * src.m[2][2] + m[3][3] * src.m[3][2];
				ret.m[3][3] = m[3][0] * src.m[0][3] + m[3][1] * src.m[1][3] + m[3][2] * src.m[2][3] + m[3][3] * src.m[3][3];
				return ret;
			}
			template<typename _ParamDataType> __MyType& operator *= (const GAIA::MATH::MTX44<_ParamDataType>& src){__MyType temp = *this; *this = temp * src; return *this;}
			GINL const _DataType* fptr() const{return GRCAST(const _DataType*)(m);}
			GINL _DataType* fptr(){return GRCAST(_DataType*)(m);}
			GINL const _DataType* bptr() const{return this->fptr() + this->size() - 1;}
			GINL _DataType* bptr(){return this->fptr() + this->size() - 1;}
			GINL GAIA::GVOID to43(GAIA::MATH::MTX43<_DataType>& dst) const
			{
				dst.m[0][0] = m[0][0]; dst.m[0][1] = m[0][1]; dst.m[0][2] = m[0][2];
				dst.m[1][0] = m[1][0]; dst.m[1][1] = m[1][1]; dst.m[1][2] = m[1][2];
				dst.m[2][0] = m[2][0]; dst.m[2][1] = m[2][1]; dst.m[2][2] = m[2][2];
				dst.m[3][0] = m[3][0]; dst.m[3][1] = m[3][1]; dst.m[3][2] = m[3][2];
			}
			GINL GAIA::GVOID from43(const GAIA::MATH::MTX43<_DataType>& src)
			{
				m[0][0] = src.m[0][0]; m[0][1] = src.m[0][1]; m[0][2] = src.m[0][2];
				m[1][0] = src.m[1][0]; m[1][1] = src.m[1][1]; m[1][2] = src.m[1][2];
				m[2][0] = src.m[2][0]; m[2][1] = src.m[2][1]; m[2][2] = src.m[2][2];
				m[3][0] = src.m[3][0]; m[3][1] = src.m[3][1]; m[3][2] = src.m[3][2];
			}
			GINL GAIA::GVOID to33(GAIA::MATH::MTX33<_DataType>& dst) const
			{
				dst.m[0][0] = m[0][0]; dst.m[0][1] = m[0][1]; dst.m[0][2] = m[0][2];
				dst.m[1][0] = m[1][0]; dst.m[1][1] = m[1][1]; dst.m[1][2] = m[1][2];
				dst.m[2][0] = m[2][0]; dst.m[2][1] = m[2][1]; dst.m[2][2] = m[2][2];
			}
			GINL GAIA::GVOID from33(const GAIA::MATH::MTX33<_DataType>& src)
			{
				m[0][0] = src.m[0][0]; m[0][1] = src.m[0][1]; m[0][2] = src.m[0][2];
				m[1][0] = src.m[1][0]; m[1][1] = src.m[1][1]; m[1][2] = src.m[1][2];
				m[2][0] = src.m[2][0]; m[2][1] = src.m[2][1]; m[2][2] = src.m[2][2];
			}
			template<typename _ParamDataType> __MyType& operator = (const GAIA::MATH::MTX44<_ParamDataType>& src)
			{
				m[0][0] = (_DataType)src.m[0][0]; m[0][1] = (_DataType)src.m[0][1]; m[0][2] = (_DataType)src.m[0][2]; m[0][3] = (_DataType)src.m[0][3];
				m[1][0] = (_DataType)src.m[1][0]; m[1][1] = (_DataType)src.m[1][1]; m[1][2] = (_DataType)src.m[1][2]; m[1][3] = (_DataType)src.m[1][3];
				m[2][0] = (_DataType)src.m[2][0]; m[2][1] = (_DataType)src.m[2][1]; m[2][2] = (_DataType)src.m[2][2]; m[2][3] = (_DataType)src.m[2][3];
				m[3][0] = (_DataType)src.m[3][0]; m[3][1] = (_DataType)src.m[3][1]; m[3][2] = (_DataType)src.m[3][2]; m[3][3] = (_DataType)src.m[3][3];
				return *this;
			}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p){GAST(p != GNIL); GAIA::ALGO::copy(this->fptr(), p, this->size()); return *this;}
			template<typename _ParamDataType> __MyType& operator = (_ParamDataType* p){return this->operator = (GSCAST(const _ParamDataType*)(p));}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType& v){GAIA::ALGO::reset(this->fptr(), v, this->size()); return *this;}
			template<typename _ParamDataType> GAIA::BL operator == (const GAIA::MATH::MTX44<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const GAIA::MATH::MTX44<_ParamDataType>& src) const{return !(this->operator == (src));}
			template<typename _ParamDataType> GAIA::BL operator >= (const GAIA::MATH::MTX44<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const GAIA::MATH::MTX44<_ParamDataType>& src) const{return GAIA::ALGO::cmps(this->fptr(), src.fptr(), this->size()) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const GAIA::MATH::MTX44<_ParamDataType>& src) const{return !(this->operator <= (src));}
			template<typename _ParamDataType> GAIA::BL operator < (const GAIA::MATH::MTX44<_ParamDataType>& src) const{return !(this->operator >= (src));}
			template<typename _ParamDataType> GAIA::BL operator == (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) != 0;}
			template<typename _ParamDataType> GAIA::BL operator >= (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) > 0;}
			template<typename _ParamDataType> GAIA::BL operator < (const _ParamDataType* p) const{return GAIA::ALGO::cmps(this->fptr(), p, this->size()) < 0;}
			template<typename _ParamDataType> const _DataType& operator [] (const _ParamDataType& index) const{GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
			template<typename _ParamDataType> _DataType& operator [] (const _ParamDataType& index){GAST(index >= 0 && index < this->size()); return this->fptr()[index];}
			template<typename _ParamDataType1, typename _ParamDataType2> _DataType& operator () (const _ParamDataType1& i1, const _ParamDataType2& i2){GAST(i1 >= 0 && i1 < 4); GAST(i2 >= 0 && i2 < 4); return m[i1][i2];}
		public:
			_DataType m[4][4];
		};
	}
}

#endif
