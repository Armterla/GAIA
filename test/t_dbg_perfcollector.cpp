#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class PerfCallBack : public GAIA::DBG::PerfCollector::CallBack
	{
	public:
		GINL PerfCallBack(GAIA::LOG::Log& logobj){this->m_pLogObj = &logobj;}
		virtual GAIA::BL OnCollect(const GAIA::CH* pszItemName, GAIA::UM uThreadID, const GAIA::U64& uTotalTime, const GAIA::U64& uMinTime, const GAIA::U64& uMaxTime, const GAIA::U64& uTotalCount)
		{
			GAIA::LOG::Log& logobj = *m_pLogObj;
			TAST(!GAIA::ALGO::gstremp(pszItemName));
			TAST(uTotalCount > 0);
			if(!GAIA::ALGO::gstrequal(pszItemName, "TestPerf") &&
			   !GAIA::ALGO::gstrequal(pszItemName, "TestPerfMulti") &&
			   !GAIA::ALGO::gstrequal(pszItemName, "TestPerfAuto"))
				TERROR;
			TAST(uMinTime <= uMaxTime);
			return GAIA::True;
		}

	private:
		GAIA::LOG::Log* m_pLogObj;
	};

	extern GAIA::GVOID t_dbg_perfcollector(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 17;

		PerfCallBack cb(logobj);
		GAIA::DBG::PerfCollector perf;
		perf.Reset();

		// Single.
		perf.Begin("TestPerf");
		GAIA::SYNC::gsleep(1);
		TAST(perf.IsBegin("TestPerf"));
		perf.End("TestPerf");
		TAST(!perf.IsBegin("TestPerf"));
		{
			GAIA::U64 uTotalTime, uMinTime, uMaxTime, uTotalCount;
			if(!perf.Get("TestPerf", &uTotalTime, &uMinTime, &uMaxTime, &uTotalCount))
				TERROR;
			GAIA::CTN::Vector<const GAIA::CH*> listResult;
			perf.Collect(listResult);
			perf.Collect(cb);
			TAST(listResult.size() == 1);
			TAST(listResult.front() != GNIL);
			TAST(GAIA::ALGO::gstrequal(listResult.front(), "TestPerf"));
			perf.DumpToLog();
		}
		perf.Reset();

		// Multi.
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::N64 nInstanceID = x;
			perf.Begin("TestPerfMulti", nInstanceID);
			GAIA::SYNC::gsleep(1);
			if(!perf.IsBegin("TestPerfMulti", nInstanceID))
			{
				TERROR;
				break;
			}
			perf.End("TestPerfMulti", nInstanceID);
			if(perf.IsBegin("TestPerfMulti", nInstanceID))
			{
				TERROR;
				break;
			}
		}
		{
			GAIA::U64 uTotalTime, uMinTime, uMaxTime, uTotalCount;
			if(!perf.Get("TestPerfMulti", &uTotalTime, &uMinTime, &uMaxTime, &uTotalCount))
				TERROR;
			GAIA::CTN::Vector<const GAIA::CH*> listResult;
			perf.Collect(listResult);
			perf.Collect(cb);
			TAST(listResult.size() == 1);
			TAST(listResult.front() != GNIL);
			TAST(GAIA::ALGO::gstrequal(listResult.front(), "TestPerfMulti"));
			perf.DumpToLog();
		}
		perf.Reset();

		// Auto.
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::DBG::PerfCollectorAuto pca(perf, "TestPerfAuto", GINVALID);
			GAIA::SYNC::gsleep(1);
		}
		{
			GAIA::U64 uTotalTime, uMinTime, uMaxTime, uTotalCount;
			if(!perf.Get("TestPerfAuto", &uTotalTime, &uMinTime, &uMaxTime, &uTotalCount))
				TERROR;
			GAIA::CTN::Vector<const GAIA::CH*> listResult;
			perf.Collect(listResult);
			perf.Collect(cb);
			TAST(listResult.size() == 1);
			TAST(listResult.front() != GNIL);
			TAST(GAIA::ALGO::gstrequal(listResult.front(), "TestPerfAuto"));
			perf.DumpToLog();
		}
		perf.Reset();
	}
}
