#ifndef		__GAIA_SYNC_BASE_H__
#define		__GAIA_SYNC_BASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#else
#	include <unistd.h>
#endif

namespace GAIA
{
	namespace SYNC
	{
		/*!
			@brief Sleep current thread by milliseconds.

			@param uMilliSeconds [in] Specify the sleep time in milliseconds.

			@return Return the time practice sleeped in milliseconds.
		*/
		GINL GAIA::U32 gsleep(const GAIA::U32& uMilliSeconds)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			::Sleep((GAIA::U32)uMilliSeconds);
			return uMilliSeconds;
		#else
			::usleep(GSCAST(useconds_t)(uMilliSeconds * 1000));
			return uMilliSeconds;
		#endif
		}

		/*!
			@brief Sleep current thread by microseconds.

			@param uMilliSeconds [in] Specify the sleep time in microseconds.

			@return Return the time practice sleeped in microseconds.
		*/
		GINL GAIA::U64 gusleep(const GAIA::U64& uMicroSeconds)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			::Sleep((GAIA::U32)uMicroSeconds / 1000);
			return uMicroSeconds;
		#else
			::usleep((useconds_t)uMicroSeconds);
			return uMicroSeconds;
		#endif
		}
	}
}

#endif
