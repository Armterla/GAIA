#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class PerfCallBack : public GAIA::DBG::PerfCollector::CallBack
	{
	public:
		virtual GAIA::BL OnCollect(const GAIA::CH* pszItemName, GAIA::UM uThreadID, const GAIA::U64& uTotalTime, const GAIA::U64& uMinTime, const GAIA::U64& uMaxTime, const GAIA::U64& uTotalCount)
		{
			return GAIA::True;
		}
	};

	extern GAIA::GVOID t_dbg_perfcollector(GAIA::LOG::Log& logobj)
	{
		PerfCallBack cb;
		GAIA::DBG::PerfCollector perf;
		perf.Reset();
		perf.Begin("TestPerf");
		TAST(perf.IsBegin("TestPerf"));
		perf.End("TestPerf");
		TAST(!perf.IsBegin("TestPerf"));
		GAIA::U64 uTotalTime, uMinTime, uMaxTime, uTotalCount;
		perf.Get("TestPerf", &uTotalTime, &uMinTime, &uMaxTime, &uTotalCount);
		GAIA::CTN::Vector<const GAIA::CH*> listResult;
		perf.Collect(listResult);
		perf.Collect(cb);
		TAST(listResult.size() == 1);
		TAST(listResult.front() != GNIL);
		TAST(GAIA::ALGO::gstrequal(listResult.front(), "TestPerf"));
		perf.Reset();
	}
}
