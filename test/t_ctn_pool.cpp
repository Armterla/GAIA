#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_pool(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 100;
		typedef GAIA::CTN::Pool<GAIA::CTN::TString> __PoolType;
		__PoolType pl;
		GAIA::CTN::Vector<GAIA::CTN::TString*> listTemp;

		TAST(pl.empty());
		if(pl.size() != 0)
			TERROR;
		if(pl.capacity() != 0)
			TERROR;
		pl.clear();
		TAST(pl.empty());
		if(pl.size() != 0)
			TERROR;
		if(pl.capacity() != 0)
			TERROR;
		pl.destroy();
		TAST(pl.empty());
		if(pl.size() != 0)
			TERROR;
		if(pl.capacity() != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CTN::TString* p = pl.alloc();
			GAST(p != GNIL);
			listTemp.push_back(p);
		}
		if(pl.empty())
			TERROR;
		if(pl.size() != SAMPLE_COUNT)
			TERROR;
		if(pl.capacity() < SAMPLE_COUNT)
			TERROR;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CTN::TString* p = listTemp[x];
			GAST(p != GNIL);
			pl.release(p);
		}
		TAST(pl.empty());
		if(pl.size() != 0)
			TERROR;
		if(pl.capacity() < SAMPLE_COUNT)
			TERROR;
		pl.clear();
		TAST(pl.empty());
		if(pl.size() != 0)
			TERROR;
		if(pl.capacity() < SAMPLE_COUNT)
			TERROR;
		pl.destroy();
		TAST(pl.empty());
		if(pl.size() != 0)
			TERROR;
		if(pl.capacity() != 0)
			TERROR;
		listTemp.clear();
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CTN::TString* p = pl.alloc();
			GAST(p != GNIL);
			listTemp.push_back(p);
		}
		pl.clear();
		TAST(pl.empty());
		if(pl.size() != 0)
			TERROR;
		if(pl.capacity() < SAMPLE_COUNT)
			TERROR;
		pl.destroy();
		TAST(pl.empty());
		if(pl.size() != 0)
			TERROR;
		if(pl.capacity() != 0)
			TERROR;
	}
}
