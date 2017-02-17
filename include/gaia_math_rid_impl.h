#ifndef 	__GAIA_MATH_RID_IMPL_H__
#define 	__GAIA_MATH_RID_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_random_lcg.h"
#include "gaia_math_rid.h"

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
	namespace MATH
	{
		GINL GAIA::GVOID RID128::uuid()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			::CoCreateGuid(GRCAST(GUID*)(u));
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
			u0 = s_lcg.random() * 0xFFFF + s_lcg.random();

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
			u1 = s_lcg_network.random() * 0xFFFF + s_lcg_network.random();

			/* Calculate u2(Local tick random value). */
			GAIA::U64 uTickTime = GAIA::TIME::tick_time();
			GAIA::MATH::RandomLCG lcg;
			lcg.seed(uTickTime % lcg.random_max());
			u2 = lcg.random() * 0xFFFF + lcg.random();

			/* Calculate u3(clock time. */
			u3 = uClockTime / 1000 / 1000;
		#else
			uuid_generate(*GRCAST(uuid_t*)(u));
		#endif
		}
		GINL GAIA::GVOID RID64::uuid()
		{
			GAIA::MATH::RID128 t;
			t.uuid();
			u64_0 = t.u64_0;
		}
		GINL GAIA::GVOID RID32::uuid()
		{
			GAIA::MATH::RID128 t;
			t.uuid();
			u0 = t.u0;
		}
	}
}

#endif
