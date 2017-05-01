#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_pattern_stringindex(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 10000;
		static const GAIA::NUM FIND_COUNT = 100;
		
		// Create instance.
		GAIA::PATTERN::StringIndex stridx;
		TAST(stridx.empty());
		TAST(stridx.size() == 0);
		
		// Insert UUID string.
		GAIA::CTN::Vector<GAIA::X128> listUid;
		listUid.resize(SAMPLE_COUNT);
		for(GAIA::NUM x = 0; x < listUid.size(); ++x)
			listUid[x].uuid();
		for(GAIA::NUM x = 0; x < listUid.size(); ++x)
		{
			const GAIA::X128& uid = listUid[x];
			GAIA::U32 szUid[33];
			uid.tostring(szUid);
			stridx.insert(szUid, 32, &listUid[x]);
		}
		TAST(!stridx.empty());
		TAST(stridx.size() == listUid.size());
		
		// Find. 
		GAIA::CTN::AString strMatcher = "ABCD";
		GAIA::CTN::Vector<GAIA::U32> matcher;
		matcher.reserve(strMatcher.size());
		for(GAIA::NUM x = 0; x < strMatcher.size(); ++x)
			matcher.push_back(strMatcher[x]);
		GAIA::PATTERN::StringIndex::FindResult r;
		for(GAIA::NUM x = 0; x < FIND_COUNT; ++x)
			stridx.find(matcher.fptr(), matcher.size(), r);
		
		// Check result.
		for(GAIA::NUM y = 0; y < r.listResult.size(); ++y)
		{
			GAIA::NUM sIndex = r.listResult[y];
			GAIA::U32* pKey;
			GAIA::NUM sKeyLen;
			const GAIA::X128* pUid = (const GAIA::X128*)stridx.get(sIndex, &pKey, &sKeyLen);
			TAST(pUid != GNIL);
			TAST(sKeyLen == 32);
			GAIA::U32 szUid[33];
			pUid->tostring(szUid);
			TAST(GAIA::ALGO::gstrequal(szUid, pKey, 32));
		}
		
		// Destroy.
		stridx.clear();
		TAST(stridx.empty());
		TAST(stridx.size() == 0);
		stridx.destroy();
		TAST(stridx.empty());
		TAST(stridx.size() == 0);
	}
}
