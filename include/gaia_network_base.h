#ifndef		__GAIA_NETWORK_BASE_H__
#define		__GAIA_NETWORK_BASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
#include "gaia_ctn_vector.h"

namespace GAIA
{
	namespace NETWORK
	{
		static const GAIA::NUM GAIA_NETWORK_MTU = 1200;

		/*!
			@brief Get host name.
		*/
		GINL GAIA::BL GetHostName(GAIA::CH* pszResult, const GAIA::N32& size);

		/*!
			@brief Get current host ip list.
		*/
		GINL GAIA::GVOID GetHostIPList(const GAIA::CH* pszHostName, GAIA::CTN::Vector<GAIA::NETWORK::IP>& listResult);

		/*!
			@brief Convert GAIA IP to socket sockaddr_in.
		*/
		GINL GAIA::GVOID ip2sip(const GAIA::NETWORK::IP& ip, GAIA::GVOID* sa);

		/*!
			@brief Convert socket sockaddr_in to GAIA IP.
		*/
		GINL GAIA::GVOID sip2ip(const GAIA::GVOID* sa, GAIA::NETWORK::IP& ip);

		/*!
			@brief Convert a readable port to socket port.
		*/
		GINL GAIA::U16 port2sport(GAIA::U16 uPort);

		/*!
			@brief Convert a socket port to a readable port.
		*/
		GINL GAIA::U16 sport2port(GAIA::U16 uPort);

		/*!
			@brief Convert GAIA Addr to socket sockaddr_in.
		*/
		GINL GAIA::GVOID addr2saddr(const GAIA::NETWORK::Addr& a, GAIA::GVOID* sa, GAIA::N32 nFamily);

		/*!
			@brief Convert sockaddr_in to GAIA Addr.
		*/
		GINL GAIA::GVOID saddr2addr(const GAIA::GVOID* sa, GAIA::NETWORK::Addr& a);
	}
}

#endif
