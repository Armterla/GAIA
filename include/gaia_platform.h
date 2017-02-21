#ifndef		__GAIA_PLATFORM_H__
#define		__GAIA_PLATFORM_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	pragma comment(lib, "ws2_32.lib")
#	include <objbase.h>
#	pragma comment(lib, "ole32.lib")
#endif

namespace GAIA
{
	class Platform
	{
	public:
		GINL Platform()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			::WSAData wsadata;
			if(::WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
				GTHROWM(NotSupport, "Windows Network");
			if(CoInitialize(GNIL) != S_OK)
				GTHROWM(NotSupport, "Windows COM");
		#else
		#endif
		}
		GINL ~Platform()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			::WSACleanup();
			CoUninitialize();
		#else
		#endif
		}
	};
}

#endif
