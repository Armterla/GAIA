#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_log_log(GAIA::LOG::Log& logobj)
	{
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			GAIA::LOG::Log::TYPE logtype = GAIA::LOG::Log::TYPE_LOG;
			switch(x % 5)
			{
			case 0:
				logtype = GAIA::LOG::Log::TYPE_LOG;
				break;
			case 1:
				logtype = GAIA::LOG::Log::TYPE_WARNING;
				break;
			case 2:
				logtype = GAIA::LOG::Log::TYPE_ERROR;
				break;
			case 3:
				logtype = GAIA::LOG::Log::TYPE_USER;
				break;
			case 4:
				logtype = GAIA::LOG::Log::TYPE_DEVELOP;
				break;
			default:
				logtype = GAIA::LOG::Log::TYPE_LOG;
				break;
			}

			g_gaia_log <<
				g_gaia_log.Type(logtype) <<
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

		GLOG << "Log macro" << GEND;
		GWAR << "War macro" << GEND;
		GERR << "Err macro" << GEND;
		GUSR << "Usr macro" << GEND;
		GDEV << "Dev macro" << GEND;
	}
}
