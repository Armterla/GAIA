#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID tperf_msys_heapesg(GAIA::LOG::Log& logobj)
	{
		// HeapESG's performance compare internal.
		{
			logobj << "\t\tBegin compare HeapESG and CRT memory management..." << logobj.End();

			static const GAIA::NUM RECURSIVE_TIMES = 10000;
			static const GAIA::NUM SAMPLE_COUNT = 100;

			GAIA::CTN::Vector<GAIA::GVOID*> listAllocated;
			listAllocated.reserve(SAMPLE_COUNT);

			GAIA::U64 uHeapESGStartTime = GAIA::TIME::tick_time();
			{
				for(GAIA::NUM y = 0; y < RECURSIVE_TIMES; ++y)
				{
					listAllocated.clear();
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						GAIA::GVOID* p = g_gaia_globalmsys.memory_alloc((x + 1) * 17);
						listAllocated.push_back(p);
					}
					for(GAIA::NUM x = 0; x < listAllocated.size(); ++x)
					{
						GAIA::GVOID* p = listAllocated[x];
						TAST(g_gaia_globalmsys.memory_size(p) == (x + 1) * 17);
						g_gaia_globalmsys.memory_release(p);
					}
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						GAIA::GVOID* p = g_gaia_globalmsys.memory_alloc((x + 1) * 17);
						TAST(g_gaia_globalmsys.memory_size(p) == (x + 1) * 17);
						g_gaia_globalmsys.memory_release(p);
					}
				}
			}
			GAIA::U64 uHeapESGEndTime = GAIA::TIME::tick_time();

			GAIA::U64 uCRTStartTime = GAIA::TIME::tick_time();
			{
				for(GAIA::NUM y = 0; y < RECURSIVE_TIMES; ++y)
				{
					listAllocated.clear();
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						GAIA::GVOID* p = malloc((x + 1) * 17);
						listAllocated.push_back(p);
					}
					for(GAIA::NUM x = 0; x < listAllocated.size(); ++x)
					{
						GAIA::GVOID* p = listAllocated[x];
						free(p);
					}
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						GAIA::GVOID* p = malloc((x + 1) * 17);
						free(p);
					}
				}
			}
			GAIA::U64 uCRTEndTime = GAIA::TIME::tick_time();

			logobj << "\t\tHeapESG Time = " << uHeapESGEndTime - uHeapESGStartTime << "(us)" << logobj.End();
			logobj << "\t\tCRT Time = " << uCRTEndTime - uCRTStartTime << "(us)"  << logobj.End();
			logobj << "\n" << logobj.End();
		}

	}
}
