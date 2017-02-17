#ifndef		__GAIA_SYSTEM_H__
#define		__GAIA_SYSTEM_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#include <stdlib.h>

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#else
#endif

namespace GAIA
{
	namespace SYSTEM
	{
		GINL GAIA::NUM GetProcessorCoreCount()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			SYSTEM_INFO si;
			::GetSystemInfo(&si);
			return (GAIA::NUM)si.dwNumberOfProcessors;
		#else
			return 1;
		#endif
		}

		GINL GAIA::NUM GetMachineWordLength()
		{
			if(sizeof(GAIA::NM) == sizeof(GAIA::N64))
				return 64;

		#if GAIA_OS == GAIA_OS_WINDOWS
			SYSTEM_INFO si;
			::GetSystemInfo(&si);
			if(si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
					si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
				return 64;
			else
				return 32;
		#else
			return 32;
		#endif
		}
		GINL GAIA::BL ShellExec(const GAIA::CH* pszCmd)
		{
			if(GAIA::ALWAYSFALSE){}
			else if(GAIA_INTERNAL_NAMESPACE::gstrcmp(pszCmd, "clear") == 0)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				system("cls");
			#else
				system("clear");
			#endif
			}
			else
				return GAIA::False;
			return GAIA::True;
		}
	}
}

#endif
