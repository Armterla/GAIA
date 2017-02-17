#ifndef 	__GAIA_THREAD_BASE_IMPL_H__
#define 	__GAIA_THREAD_BASE_IMPL_H__

#include "gaia_thread_base.h"

namespace GAIA
{
	namespace THREAD
	{
		GINL GAIA::UM threadid()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			return GSCAST(GAIA::UM)(::GetCurrentThreadId());
		#else
			return (GAIA::UM)(pthread_self());
		#endif
		}
	}
}

#endif
