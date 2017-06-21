#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_orderless(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 17;

		typedef GAIA::CTN::TString __StrType;
		typedef GAIA::CTN::Orderless<__StrType> __OrderlessType;

		__OrderlessType ol;
		if(ol.size() != 0)
			TERROR;
		if(ol.capacity() != 0)
			TERROR;
		TAST(ol.empty());
		ol.clear();
		if(ol.size() != 0)
			TERROR;
		if(ol.capacity() != 0)
			TERROR;
		TAST(ol.empty());
		ol.destroy();
		if(ol.size() != 0)
			TERROR;
		if(ol.capacity() != 0)
			TERROR;
		TAST(ol.empty());
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CTN::TString str = x;
			str = _T("HelloWorld") + str;
			if(ol.insert(str) != x)
			{
				TERROR;
				break;
			}
		}
		if(ol.size() != SAMPLE_COUNT)
			TERROR;
		if(ol.capacity() < SAMPLE_COUNT)
			TERROR;
		if(ol.empty())
			TERROR;
		ol.clear();
		ol.destroy();
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CTN::TString str = x;
			str = _T("HelloWorld") + str;
			if(ol.insert(str) != x)
			{
				TERROR;
				break;
			}
		}
		if(ol.find(_T("HelloWorld")) != GINVALID)
			TERROR;
		if(ol.find(_T("HelloWorld0")) == GINVALID)
			TERROR;
		if(ol.find(_T("HelloWorld1")) == GINVALID)
			TERROR;
		if(!ol.exist(_T("HelloWorld0")))
			TERROR;
		GAIA::CTN::TString strTemp = SAMPLE_COUNT - 1;
		strTemp = _T("HelloWorld") + strTemp;
		if(ol.find(strTemp) == GINVALID)
			TERROR;
		for(GAIA::NUM x = 0; x < ol.size(); ++x)
		{
			if(!ol.exist(x))
			{
				TERROR;
				break;
			}
		}
		TAST(ol.erase_index(0));
		if(ol.find(_T("HelloWorld0")) != GINVALID)
			TERROR;
		if(ol.exist(0))
			TERROR;
		TAST(ol.erase_index(1));
		if(ol.find(_T("HelloWorld1")) != GINVALID)
			TERROR;
		if(ol.exist(1))
			TERROR;
		TAST(ol.erase_index(10));
		if(ol.find(_T("HelloWorld10")) != GINVALID)
			TERROR;
		if(ol.exist(10))
			TERROR;
		TAST(ol.erase_index(SAMPLE_COUNT - 1));
		if(ol.find(strTemp) != GINVALID)
			TERROR;
		if(ol.exist(SAMPLE_COUNT - 1))
			TERROR;
		GAIA::NUM uExistCount = 0;
		for(GAIA::NUM x = 0; x < ol.size(); ++x)
		{
			if(ol.exist(x))
				++uExistCount;
		}
		if(uExistCount != SAMPLE_COUNT - 4)
			TERROR;
		ol.destroy();
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CTN::TString str = x;
			str = _T("HelloWorld") + str;
			if(ol.insert(str) != x)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CTN::TString str = x;
			str = _T("HelloWorld") + str;
			if(ol.insert(str) != x + SAMPLE_COUNT)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::CTN::TString str = x;
			str = _T("HelloWorld") + str;
			if(!ol.erase(str))
			{
				TERROR;
				break;
			}
		}
		if(ol.size() != SAMPLE_COUNT * 2)
			TERROR;
		if(ol.practice_size() != 0)
			TERROR;
		if(ol.capacity() == 0)
			TERROR;
		TAST(ol.empty());
	}
}
