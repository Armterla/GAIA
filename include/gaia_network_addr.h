#ifndef		__GAIA_NETWORK_ADDR_H__
#define		__GAIA_NETWORK_ADDR_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_network_ip.h"

namespace GAIA
{
	namespace NETWORK
	{
		class Addr : public GAIA::Base
		{
		public:
			GINL Addr(){}
			GINL Addr(const Addr& src){this->operator = (src);}
			GINL Addr(const GAIA::NETWORK::IP& ip, GAIA::U16 uPort){this->ip = ip; this->uPort = uPort;}
			GINL GAIA::GVOID reset(){ip.reset(); uPort = 0;}
			GINL GAIA::BL check() const{return ip.check() && uPort != 0;}
			template<typename _ParamDataType> GAIA::BL fromstring(const _ParamDataType* psz)
			{
				if(!ip.fromstring(psz))
					return GAIA::False;
				const _ParamDataType* p = GAIA::ALGO::gstrch(psz, ':');
				if(p == GNIL)
					return GAIA::False;
				++p;
				GAIA::ALGO::str2int(p, uPort);
				return GAIA::True;
			}
			template<typename _ParamDataType> _ParamDataType* tostring(_ParamDataType* psz) const
			{
				ip.tostring(psz);
				_ParamDataType* p = GAIA::ALGO::gstrend(psz);
				*p = ':';
				++p;
				p = GAIA::ALGO::int2str((GAIA::N32)uPort, p);
				*(p - 1) = 0;
				return psz;
			}
			GINL Addr& operator = (const Addr& src){GAST(&src != this); ip = src.ip; uPort = src.uPort; return *this;}
			GINL GAIA::N32 compare(const GAIA::NETWORK::Addr& src) const
			{
				if(ip < src.ip)
					return -1;
				else if(ip > src.ip)
					return +1;
				if(uPort < src.uPort)
					return -1;
				else if(uPort > src.uPort)
					return +1;
				return 0;
			}
			GCLASS_COMPARE_BYCOMPARE(GAIA::NETWORK::Addr)
		public:
			GAIA::NETWORK::IP ip;
			GAIA::U16 uPort;
		};
	}
}

#endif
