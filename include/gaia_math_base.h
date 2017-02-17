#ifndef		__GAIA_MATH_BASE_H__
#define		__GAIA_MATH_BASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_type_traits2.h"
#include "gaia_math_constant.h"

#include <math.h>
#include <float.h>

namespace GAIA
{
	namespace MATH
	{
		template<typename _DataType> _DataType gabs(const _DataType& t)
		{
			if(t < 0)
				return -t;
			return t;
		}
		template<typename _DataType> _DataType gsign(const _DataType& t)
		{
			if(t < (_DataType)0)
				return (_DataType)-1;
			else if(t > (_DataType)0)
				return (_DataType)+1;
			else
				return (_DataType)0;
		}
		template<typename _DataType, typename _LowDataType, typename _HighDataType> _DataType gclamp(const _DataType& src, const _LowDataType& limitlow, const _HighDataType& limithigh)
		{
			GAST(limitlow <= limithigh);
			_DataType ret = src;
			if(ret < limitlow)
				ret = (_DataType)limitlow;
			if(ret > limithigh)
				ret = (_DataType)limithigh;
			return ret;
		}
		template<typename _DataType> _DataType gsaturate(const _DataType& src)
		{
			return gclamp(src, (_DataType)0, (_DataType)1);
		}
		template<typename _DataType> _DataType gfloor(const _DataType& src)
		{
			_DataType tinteger = GSCAST(_DataType)(GSCAST(GAIA::N64)(src));
			_DataType t = src - tinteger;
			if(t == (_DataType)0)
				return src;
			return tinteger;
		}
		template<typename _DataType> _DataType gceil(const _DataType& src)
		{
			_DataType tinteger = GSCAST(_DataType)((GAIA::N64)src);
			_DataType t = src - tinteger;
			if(t == (_DataType)0)
				return src;
			return tinteger + (_DataType)1;
		}
		template<typename _DataType> GAIA::BL gequalzero(const _DataType& t)
		{
			if(t >= R(-0.00001) && t <= R(+0.00001))
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType, typename _PrecisionDataType> GAIA::BL gequalzero(const _DataType& t, const _PrecisionDataType& epsilon)
		{
			if(t >= -epsilon && t <= +epsilon)
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType1, typename _DataType2> GAIA::BL gequal(const _DataType1& t1, const _DataType2& t2)
		{
			if(GAIA::MATH::gabs(t1 - t2) <= (typename GAIA::TRAITS::DataTypeConvertTraits<_DataType1, _DataType2>::ConvertedType)R(0.00001))
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType1, typename _DataType2, typename _PrecisionDataType> GAIA::BL gequal(const _DataType1& t1, const _DataType2& t2, const _PrecisionDataType& epsilon)
		{
			if(gabs(t1 - t2) < epsilon)
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType> _DataType gavg2(_DataType t1, _DataType t2)
		{
			return (t1 + t2) / (_DataType)2;
		}
		template<typename _DataType> _DataType gavg3(_DataType t1, _DataType t2, _DataType t3)
		{
			return (t1 + t2 + t3) / (_DataType)3;
		}
		template<typename _DataType> _DataType gavg4(_DataType t1, _DataType t2, _DataType t3, _DataType t4)
		{
			return (t1 + t2 + t3 + t4) / (_DataType)4;
		}
		template<typename _DataType, typename _ResultDataType> GAIA::GVOID gavgs(_DataType pBegin, _DataType pEnd, _ResultDataType& res)
		{
			GAST(!!pBegin);
			GAST(!!pEnd);
			res = (_ResultDataType)0;
			_DataType p = pBegin;
			while(p != pEnd)
			{
				res += *p;
				++p;
			}
			res += *p;
			res /= (pEnd - pBegin + 1);
		}
		template<typename _DataType> _DataType grealleft(const _DataType& t)
		{
			return (_DataType)(GAIA::N64)t;
		}
		template<typename _DataType> _DataType grealright(const _DataType& t)
		{
			return t - GAIA::MATH::grealleft(t);
		}
		template<typename _DataType> _DataType ground(const _DataType& t)
		{
			GAIA::N64 i = (GAIA::N64)t;
			_DataType v = (_DataType)(t - i);
			if(GAIA::MATH::gabs(v) >= 0.5)
			{
				if(i >= 0)
					return (_DataType)(i + 1);
				else
					return (_DataType)(i - 1);
			}
			else
				return (_DataType)i;
		}
		template<typename _DataType> _DataType gmod(const _DataType& t, const _DataType& unit)
		{
			GAIA::N64 count = (GAIA::N64)(t / unit);
			return t - (_DataType)count * unit;
		}
		template<typename _DataType> _DataType gtocycle(const _DataType& t, const _DataType& cycle)
		{
			_DataType ret = GAIA::MATH::gmod(t, cycle);
			if(ret < (_DataType)0)
				ret += cycle;
			return ret;
		}
		template<typename _DataType> _DataType gto360(const _DataType& degree)
		{
			return GAIA::MATH::gtocycle(degree, (_DataType)360);
		}
		template<typename _DataType> _DataType gto2pi(const _DataType& radian)
		{
			return GAIA::MATH::gtocycle(radian, (_DataType)(GAIA::MATH::PI * R(2.0)));
		}
		template<typename _DataType> _DataType gsin(const _DataType& v){return GSCAST(_DataType)(sin(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType gcos(const _DataType& v){return GSCAST(_DataType)(cos(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType gacos(const _DataType& v){return GSCAST(_DataType)(acos(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType gasin(const _DataType& v){return GSCAST(_DataType)(asin(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType gtan(const _DataType& v){return GSCAST(_DataType)(tan(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType gsinh(const _DataType& v){return GSCAST(_DataType)(sinh(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType gcosh(const _DataType& v){return GSCAST(_DataType)(cosh(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType gatan(const _DataType& v){return GSCAST(_DataType)(atan(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType gatan2(const _DataType& y, const _DataType& x){return GSCAST(_DataType)(atan2(GSCAST(GAIA::F64)(y), GSCAST(GAIA::F64)(x)));}
		template<typename _DataType> _DataType gpow(const _DataType& x, const _DataType& y){return GSCAST(_DataType)(pow(GSCAST(GAIA::F64)(x), GSCAST(GAIA::F64)(y)));}
		template<typename _DataType> _DataType gsqrt(const _DataType& v){return GSCAST(_DataType)(sqrt(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType glog(const _DataType& v){return GSCAST(_DataType)(log(GSCAST(GAIA::F64)(v)));}
		template<typename _DataType> _DataType glog10(const _DataType& v){return GSCAST(_DataType)(log10(GSCAST(GAIA::F64)(v)));}
		GINL GAIA::BL gfinite(const GAIA::F32& v)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			return _finite(v) != 0;
		#elif GAIA_OS == GAIA_OS_ANDROID
			return __captured::isfinite(v) != 0;
		#else
			return isfinite(v) != 0;
		#endif
		}
		GINL GAIA::BL gfinite(const GAIA::F64& v)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			return _finite(v) != 0;
		#elif GAIA_OS == GAIA_OS_ANDROID
			return __captured::isfinite(v) != 0;
		#else
			return isfinite(v) != 0;
		#endif
		}
		template<typename _DataType> GAIA::BL gfinite(const _DataType& v){return GAIA::True;}
		template<typename _DataType> GAIA::BL gisprimenum(_DataType t)
		{
			for(_DataType x = 2; x < t; ++x)
			{
				if(t % x == 0)
					return GAIA::False;
			}
			return GAIA::True;
		}
	}
}

#endif
