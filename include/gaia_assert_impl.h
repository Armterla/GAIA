#ifndef		__GAIA_ASSERT_IMPL_H__
#define		__GAIA_ASSERT_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_log.h"

extern GAIA::LOG::Log g_gaia_log;
extern GAIA::LOG::InvalidLog g_gaia_invalidlog;

namespace GAIA
{
	namespace DBG
	{
		GINL GAIA::GVOID debuglog(const GAIA::CH* pszHead, const GAIA::CH* pszFileName, GAIA::NUM sCodeLine)
		{
			g_gaia_log << g_gaia_log.Type(GAIA::LOG::Log::TYPE_ERROR)
					   << g_gaia_log.UserFilter(0x0001)
					   << pszHead
					   << pszFileName << "(" << sCodeLine << ")"
					   << g_gaia_log.End();
			g_gaia_log.Flush();
		}
	}
}

#endif
