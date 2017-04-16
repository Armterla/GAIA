#ifndef		__GAIA_TYPE_IMPL_H__
#define		__GAIA_TYPE_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_global_decl.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#else
#	if GAIA_OS != GAIA_OS_ANDROID
#		include <uuid/uuid.h>
#	endif
#endif

namespace GAIA
{
	GAIA::GVOID X128::uuid()
	{
	#if GAIA_OS == GAIA_OS_WINDOWS
		::CoCreateGuid(GRCAST(GUID*)(u8));
	#elif GAIA_OS == GAIA_OS_ANDROID
		/* Calculate u0(Global clock random value). */
		GAIA::U64 uClockTime = GAIA::TIME::local_time();
		static GAIA::BL s_bLCGInit = GAIA::False;
		static GAIA::MATH::RandomLCG s_lcg;
		if(!s_bLCGInit)
		{
			s_lcg.seed((uClockTime / 1000 / 1000) % s_lcg.random_max());
			s_bLCGInit = GAIA::True;
		}
		u32_0 = s_lcg.random() * 0xFFFF + s_lcg.random();

		/* Calculate u1(Global network IP address random value. */
		static GAIA::U32 s_uIPSum = 0;
		static GAIA::MATH::RandomLCG s_lcg_network;
		if(s_uIPSum == 0)
		{
			GAIA::CTN::Vector<GAIA::NETWORK::IP> listIP;
			GAIA::CH szHostName[260];
			GAIA::NETWORK::GetHostName(szHostName, 260);
			GAIA::NETWORK::GetHostIPList(szHostName, listIP);
			for(GAIA::NUM x = 0; x < listIP.size(); ++x)
				s_uIPSum += listIP[x].u;
			s_lcg_network.seed(s_uIPSum);
		}
		u32_1 = s_lcg_network.random() * 0xFFFF + s_lcg_network.random();

		/* Calculate u2(Local tick random value). */
		GAIA::U64 uTickTime = GAIA::TIME::tick_time();
		GAIA::MATH::RandomLCG lcg;
		lcg.seed(uTickTime % lcg.random_max());
		u32_2 = lcg.random() * 0xFFFF + lcg.random();

		/* Calculate u3(clock time). */
		u32_3 = uClockTime / 1000 / 1000;
	#else
		uuid_generate(*GRCAST(uuid_t*)(u8));
	#endif
	}
	template<typename _ParamDataType> GAIA::BL X128::check(const _ParamDataType* psz) const
	{
		if(psz == GNIL)
			return GAIA::False;
		if(GAIA_INTERNAL_NAMESPACE::gstrlen(psz) != 32)
			return GAIA::False;
		for(GAIA::NUM x = 0; x < 32; ++x)
		{
			if((psz[x] >= '0' && psz[x] <= '9') || 
				(psz[x] >= 'a' && psz[x] <= 'f') || 
				(psz[x] >= 'A' && psz[x] <= 'F')){}
			else
				return GAIA::False;
		}
		return GAIA::True;
	}
	template<typename _ParamDataType> GAIA::GVOID X128::fromstring(const _ParamDataType* psz)
	{
		GAIA_INTERNAL_NAMESPACE::str2hex(psz, sizeofarray(u8), u8);
	}
	template<typename _ParamDataType> _ParamDataType* X128::tostring(_ParamDataType* psz) const
	{
		GAIA_INTERNAL_NAMESPACE::hex2str(u8, sizeofarray(u8), psz);
		return psz;
	}

	GINL TYPEID nametotype(const GAIA::CH* psz)
	{
		for(GAIA::N32 x = 0; x < sizeofarray(TYPEID_ANAME); ++x)
		{
			if(GAIA_INTERNAL_NAMESPACE::gstrcmp(TYPEID_ANAME[x], psz) == 0)
				return GSCAST(TYPEID)(x);
		}
		return TYPEID_INVALID;
	}
	GINL TYPEID nametotype(const GAIA::WCH* psz)
	{
		for(GAIA::N32 x = 0; x < sizeofarray(TYPEID_WNAME); ++x)
		{
			if(GAIA_INTERNAL_NAMESPACE::gstrcmp(TYPEID_WNAME[x], psz) == 0)
				return GSCAST(TYPEID)(x);
		}
		return TYPEID_INVALID;
	}
	
	GINL GAIA::RELATION_TYPE GetRelationTypeByString(const GAIA::CH* psz, GAIA::NUM sLen)
	{
		GAST(psz != GNIL && *psz != '\0');
		GAST(sLen == GINVALID || sLen > 0);
		if(sLen == GINVALID)
		{
			for(GAIA::NUM x = 1; sizeofarray(GAIA::RELATION_TYPE_STRING); ++x)
			{
				if(GAIA::ALGO::gstrcmp(psz, GAIA::RELATION_TYPE_STRING[x]) == 0)
					return (RELATION_TYPE)x;
			}
		}
		else
		{
			for(GAIA::NUM x = 0; x < sizeofarray(GAIA::RELATION_TYPE_STRING); ++x)
			{
				if(GAIA::ALGO::gstrcmp(psz, GAIA::RELATION_TYPE_STRING[x], sLen) == 0)
					return (RELATION_TYPE)x;
			}
		}
		return RELATION_TYPE_INVALID;
	}
	
	GINL const CH* GetRelationTypeByEnum(GAIA::RELATION_TYPE rt)
	{
		if(rt >= sizeofarray(RELATION_TYPE_STRING))
			return GNIL;
		return RELATION_TYPE_STRING[(NUM)rt];
	}
}

#endif
