#ifndef		__GAIA_NETWORK_BASE_IMPL_H__
#define		__GAIA_NETWORK_BASE_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
#include "gaia_ctn_vector.h"
#include "gaia_network_base.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#else
#	include <sys/errno.h>
#	if GAIA_OS == GAIA_OS_ANDROID
#		include <fcntl.h>
#	else
#		include <sys/fcntl.h>
#	endif
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <netdb.h>
#endif

namespace GAIA
{
	namespace NETWORK
	{
		GINL GAIA::BL GetHostName(GAIA::CH* pszResult, const GAIA::N32& size){return gethostname(pszResult, size) != GINVALID;}
		GINL GAIA::BL GetHostIPList(const GAIA::CH* pszHostName, GAIA::CTN::Vector<GAIA::NETWORK::IP>& listResult)
		{
			GAIA::BL bRet = GAIA::False;
			hostent* pHostEnt = gethostbyname(pszHostName);
			if(pHostEnt != GNIL)
			{
				GAIA::N32 nIndex = 0;
				while(pHostEnt->h_addr_list[nIndex] != GNIL)
				{
					if(pHostEnt->h_addrtype == AF_INET)
					{
						in_addr* addr = GRCAST(in_addr*)(pHostEnt->h_addr_list[nIndex]);
						GAIA::NETWORK::IP ip;
						ip.reset();
						ip.u0 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)((*(GAIA::U32*)addr) & 0x000000FF) >> 0);
						ip.u1 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)((*(GAIA::U32*)addr) & 0x0000FF00) >> 8);
						ip.u2 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)((*(GAIA::U32*)addr) & 0x00FF0000) >> 16);
						ip.u3 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)((*(GAIA::U32*)addr) & 0xFF000000) >> 24);
						listResult.push_back(ip);
						++nIndex;
						bRet = GAIA::True;
					}
					else if(pHostEnt->h_addrtype == AF_INET6)
					{
					}
				}
			}
			return bRet;
		}
		GINL GAIA::BL GetHostIP(const GAIA::CH* pszHostName, GAIA::NETWORK::IP& ip)
		{
			hostent* pHostEnt = gethostbyname(pszHostName);
			if(pHostEnt == GNIL)
				return GAIA::False;
			in_addr* addr = GRCAST(in_addr*)(pHostEnt->h_addr_list[0]);
			if(addr == GNIL)
				return GAIA::False;
			if(pHostEnt->h_addrtype == AF_INET)
			{
				ip.reset();
				ip.u0 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)((*(GAIA::U32*)addr) & 0x000000FF) >> 0);
				ip.u1 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)((*(GAIA::U32*)addr) & 0x0000FF00) >> 8);
				ip.u2 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)((*(GAIA::U32*)addr) & 0x00FF0000) >> 16);
				ip.u3 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)((*(GAIA::U32*)addr) & 0xFF000000) >> 24);
			}
			else
			{
				return GAIA::False;
			}
			return GAIA::True;
		}
		GINL GAIA::GVOID ip2sip(const GAIA::NETWORK::IP& ip, GAIA::GVOID* sa)
		{
			sockaddr_in& saddr = *(sockaddr_in*)sa;
			saddr.sin_addr.s_addr = (ip.u0 << 0) | (ip.u1 << 8) | (ip.u2 << 16) | (ip.u3 << 24);
		}
		GINL GAIA::GVOID sip2ip(const GAIA::GVOID* sa, GAIA::NETWORK::IP& ip)
		{
			sockaddr_in& saddr = *(sockaddr_in*)sa;
			ip.u0 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)(saddr.sin_addr.s_addr & 0x000000FF) >> 0);
			ip.u1 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)(saddr.sin_addr.s_addr & 0x0000FF00) >> 8);
			ip.u2 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)(saddr.sin_addr.s_addr & 0x00FF0000) >> 16);
			ip.u3 = GSCAST(GAIA::U8)(GSCAST(GAIA::U32)(saddr.sin_addr.s_addr & 0xFF000000) >> 24);
		}
		GINL GAIA::U16 port2sport(GAIA::U16 uPort)
		{
			return htons(uPort);
		}
		GINL GAIA::U16 sport2port(GAIA::U16 uPort)
		{
			return ntohs(uPort);
		}
		GINL GAIA::GVOID addr2saddr(const GAIA::NETWORK::Addr& a, GAIA::GVOID* sa, GAIA::N32 nFamily)
		{
			sockaddr_in& saddr = *(sockaddr_in*)sa;
			saddr.sin_family = nFamily;
			GAIA::NETWORK::ip2sip(a.ip, sa);
			saddr.sin_port = GAIA::NETWORK::port2sport(a.uPort);
		}
		GINL GAIA::GVOID saddr2addr(const GAIA::GVOID* sa, GAIA::NETWORK::Addr& a)
		{
			sockaddr_in& saddr = *(sockaddr_in*)sa;
			GAIA::NETWORK::sip2ip(sa, a.ip);
			a.uPort = GAIA::NETWORK::sport2port(saddr.sin_port);
		}
	}
}

#endif
