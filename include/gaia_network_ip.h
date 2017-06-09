#ifndef		__GAIA_NETWORK_IP_H__
#define		__GAIA_NETWORK_IP_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_compare.h"
#include "gaia_algo_string.h"
#include "gaia_algo_hash.h"

namespace GAIA
{
	namespace NETWORK
	{
		class IP : public GAIA::Base
		{
		public:
			GINL IP(){}
			GINL IP(const IP& src){this->operator = (src);}
			template<typename _ParamDataType> IP(const _ParamDataType* psz){this->operator = (psz);}
			GINL GAIA::GVOID reset(){GAIA::ALGO::reset(us, 0, 4);}
			GINL GAIA::BL check() const{return GAIA::ALGO::cmpk(us, 0, 4) != 0;}
			template<typename _ParamDataType> GAIA::BL fromstring(const _ParamDataType* psz)
			{
				const _ParamDataType* p = psz;
				GAIA::U32 uDotCnt = GAIA::ALGO::gstrcnt(psz, '.');
				if(uDotCnt == 3)
				{
					for(GAIA::N32 x = 0; x < 3; ++x)
					{
						p = GAIA::ALGO::str2int(p, us[3 - x]);
						++p;
					}
					GAIA::ALGO::str2int(p, us[0]);
					return GAIA::True;
				}
				return GAIA::False;
			}
			template<typename _ParamDataType> _ParamDataType* tostring(_ParamDataType* psz) const
			{
				_ParamDataType* p = psz;
				for(GAIA::N32 x = 0; x < 4; ++x)
				{
					p = GAIA::ALGO::int2str((GAIA::N32)us[3 - x], p);
					*(p - 1) = '.';
				}
				*(p - 1) = 0;
				return psz;
			}
			GINL GAIA::U64 hash() const{return GAIA::ALGO::hash(u);}
			GINL IP& operator = (const IP& src){GAST(&src != this); u = src.u; return *this;}
			template<typename _ParamDataType> IP& operator = (const _ParamDataType* psz){this->fromstring(psz); return *this;}
			GINL IP operator + (const IP& src) const{IP ret; ret.u = u + src.u; return ret;}
			GINL IP operator - (const IP& src) const{IP ret; ret.u = u - src.u; return ret;}
			template<typename _ParamDataType> IP operator + (const _ParamDataType& t) const{IP ret; ret.u = u + t; return ret;}
			template<typename _ParamDataType> IP operator - (const _ParamDataType& t) const{IP ret; ret.u = u - t; return ret;}
			GINL IP& operator += (const IP& src){u += src.u; return *this;}
			GINL IP& operator -= (const IP& src){u -= src.u; return *this;}
			template<typename _ParamDataType> IP& operator += (const _ParamDataType& t){u += t; return *this;}
			template<typename _ParamDataType> IP& operator -= (const _ParamDataType& t){u -= t; return *this;}
			GINL IP& operator ++ (){(*this) += 1; return *this;}
			GINL IP& operator -- (){(*this) -= 1; return *this;}
			GINL IP operator ++ (GAIA::N32){IP ret = *this; ++(*this); return ret;}
			GINL IP operator -- (GAIA::N32){IP ret = *this; --(*this); return ret;}
			GINL GAIA::N32 compare(const GAIA::NETWORK::IP& src) const
			{
				if(u < src.u)
					return -1;
				else if(u > src.u)
					return +1;
				return 0;
			}
			GCLASS_COMPARE_BYCOMPARE(IP)
		public:
			union
			{
				GAIA::U32 u;
				GAIA::U8 us[4];
				class{public:GAIA::U8 u3, u2, u1, u0;};
				GAIA::U32 uIPv4;
			};
		};
	}
}

#endif
