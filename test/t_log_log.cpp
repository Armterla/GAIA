#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_log_log(GAIA::LOG::Log& logobj)
	{
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			g_gaia_log <<
				g_gaia_log.Type(GAIA::LOG::Log::TYPE_WARNING) <<
				g_gaia_log.UserFilter(0x12345678) <<
				"Test log " <<
				(GAIA::U8)1 << " " <<
				(GAIA::U16)2 << " " <<
				(GAIA::U32)3 << " " <<
				(GAIA::U64)4 << " " <<
				(GAIA::N8)-5 << " " <<
				(GAIA::N16)+6 << " " <<
				(GAIA::N32)-7 << " " <<
				(GAIA::N64)+8 << " " <<
				(GAIA::UM)9 << " " <<
				(GAIA::NM)-10 << " " <<
				(GAIA::BL)GAIA::True << " " <<
				(GAIA::F32)11.12345678 << " " <<
				(GAIA::F64)12.12345678 << " " <<
				(GAIA::CH*)"ansi string" << " " <<
				(GAIA::WCH*)L"unicode string" <<
				g_gaia_log.End();
		}
	}
}
