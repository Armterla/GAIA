#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID tperf_ctn_dmpgraph(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 1000;

		typedef GAIA::CTN::BasicDMPGraph<GAIA::NUM, GAIA::CH, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __DMPGraphType;
		__DMPGraphType dmp;

		GAIA::CTN::Vector<GAIA::CTN::BasicChars<GAIA::CH, GAIA::N8, 64> > listUUID;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CH szTemp[64 + 1];
			GAIA::MATH::RID128 rid;
			rid.uuid();
			rid.tostring(szTemp);
			rid.uuid();
			rid.tostring(szTemp + 32);
			listUUID.push_back(szTemp);
		}

		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
			dmp.insert(listUUID[x].fptr(), listUUID[x].size(), x);

		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
		{
			const GAIA::NUM* pFinded = dmp.find(listUUID[x].fptr(), listUUID[x].size());
			if(pFinded == GNIL || *pFinded != x)
				TERROR;
		}

		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
			dmp.erase(listUUID[x].fptr(), listUUID[x].size(), x);
	}
}
