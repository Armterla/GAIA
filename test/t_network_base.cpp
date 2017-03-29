#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_network_base(GAIA::LOG::Log& logobj)
	{
		GAIA::NETWORK::Addr addr;
		addr.fromstring("127.0.0.1:1234");
		char szTemp[1024];
		addr.tostring(szTemp);
		if(GAIA::ALGO::gstrcmp(szTemp, "127.0.0.1:1234") != 0)
			TERROR;

		GAIA::CH szHostName[GAIA::MAXPL];
		if(!GAIA::NETWORK::GetHostName(szHostName, sizeof(szHostName)))
			TERROR;

		GAIA::CTN::Vector<GAIA::NETWORK::IP> listIP;
		if(!GAIA::NETWORK::GetHostIPList(szHostName, listIP))
			TERROR;
		if(listIP.empty())
			TERROR;
		for(GAIA::NUM x = 0; x < listIP.size(); ++x)
		{
			const GAIA::NETWORK::IP& ip = listIP[x];
			if(!ip.check())
			{
				TERROR;
				break;
			}
		}

		GAIA::NETWORK::IP ip;
		if(!GAIA::NETWORK::GetHostIP(szHostName, ip))
			TERROR;
		if(!ip.check())
			TERROR;

		if(!GAIA::NETWORK::GetHostIPList("www.baidu.com", listIP))
			TERROR;
		if(listIP.empty())
			TERROR;
		for(GAIA::NUM x = 0; x < listIP.size(); ++x)
		{
			const GAIA::NETWORK::IP& ip = listIP[x];
			if(!ip.check())
			{
				TERROR;
				break;
			}
		}

		if(!GAIA::NETWORK::GetHostIP("www.baidu.com", ip))
			TERROR;
		if(!ip.check())
			TERROR;
	}
}
