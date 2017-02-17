#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	typedef GAIA::CTN::BasicDMPGraph<GAIA::CTN::AString, GAIA::CH, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __DMPGraphType;

	class TestDMPGraphFindCallBack : public __DMPGraphType::FindCallBack
	{
	public:
		virtual GAIA::BL find(__DMPGraphType& g,
			const GAIA::CH* pKeys, const GAIA::NUM& keysize, const GAIA::CTN::AString& t)
		{
			listData.push_back(t);
			return GAIA::True;
		}
	public:
		GINL GAIA::GVOID clear()
		{
			listData.clear();
		}
		GINL GAIA::GVOID sort()
		{
			listData.sort();
		}
	public:
		GAIA::CTN::Vector<GAIA::CTN::AString> listData;
	};

	extern GAIA::GVOID t_ctn_dmpgraph(GAIA::LOG::Log& logobj)
	{
		typedef __DMPGraphType __DMPGraphType;

		__DMPGraphType dmp;

		TAST(dmp.empty());
		if(dmp.size() != 0)
			TERROR;
		dmp.destroy();
		TAST(dmp.empty());
		if(dmp.size() != 0)
			TERROR;

		dmp.insert("HelloWorld", 10, "HelloWorld");
		if(dmp.empty())
			TERROR;
		if(dmp.size() != 1)
			TERROR;

		const GAIA::CTN::AString* pFinded = dmp.find("HelloWorld", 10);
		if(pFinded == GNIL)
			TERROR;
		pFinded = dmp.find("HelloWor", 8);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("lloWorld", 8);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("lloWor", 6);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("l", 1);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("ll", 2);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("llo", 3);
		if(pFinded != GNIL)
			TERROR;

		TestDMPGraphFindCallBack cb;
		GAIA::NUM sFinded;

		cb.clear();
		sFinded = dmp.find("HelloWorld", 10, GAIA::True, GAIA::True, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("HelloWorld", 10, GAIA::True, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("HelloWorld", 10, GAIA::False, GAIA::True, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("HelloWorld", 10, GAIA::False, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("HelloWor", 8, GAIA::False, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("lloWorld", 8, GAIA::False, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("lloWor", 6, GAIA::False, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("l", 1, GAIA::False, GAIA::False, cb);
		if(sFinded != 3 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("ll", 2, GAIA::False, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("llo", 3, GAIA::False, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		dmp.insert("ll", 2, "ll");
		if(dmp.size() != 2)
			TERROR;
		if(dmp.empty())
			TERROR;

		pFinded = dmp.find("ll", 2);
		if(pFinded == GNIL)
			TERROR;
		pFinded = dmp.find("HelloWor", 8);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("lloWorld", 8);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("lloWor", 6);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("l", 1);
		if(pFinded != GNIL)
			TERROR;
		pFinded = dmp.find("llo", 3);
		if(pFinded != GNIL)
			TERROR;

		cb.clear();
		sFinded = dmp.find("HelloWorld", 10, GAIA::True, GAIA::True, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("HelloWorld", 10, GAIA::True, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("HelloWorld", 10, GAIA::False, GAIA::True, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("HelloWorld", 10, GAIA::False, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		if(cb.listData[0] != "HelloWorld")
			TERROR;

		cb.clear();
		sFinded = dmp.find("ll", 2, GAIA::False, GAIA::False, cb);
		if(sFinded != 2 || cb.listData.size() != sFinded)
			TERROR;
		cb.sort();
		if(cb.listData[0] != "HelloWorld")
			TERROR;
		if(cb.listData[1] != "ll")
			TERROR;

		cb.clear();
		sFinded = dmp.find("ll", 2, GAIA::True, GAIA::False, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		cb.sort();
		if(cb.listData[0] != "ll")
			TERROR;

		cb.clear();
		sFinded = dmp.find("ll", 2, GAIA::False, GAIA::True, cb);
		if(sFinded != 1 || cb.listData.size() != sFinded)
			TERROR;
		cb.sort();
		if(cb.listData[0] != "ll")
			TERROR;

		dmp.destroy();
		TAST(dmp.empty());
		if(dmp.size() != 0)
			TERROR;

		// Erase 0 test.
		dmp.insert("1234", 4, "1234");
		dmp.insert("abcd", 4, "abcd");
		dmp.insert("1a", 2, "1a");
		dmp.insert("1ab2", 4, "1ab2");
		dmp.insert("111", 3, "111");
		dmp.insert("121", 3, "121");
		TAST(dmp.erase("1234", 4, "1234"));
		TAST(dmp.erase("abcd", 4, "abcd"));
		TAST(dmp.erase("1a", 2, "1a"));
		TAST(dmp.erase("1ab2", 4, "1ab2"));
		TAST(dmp.erase("111", 3, "111"));
		TAST(dmp.erase("121", 3, "121"));

		// Erase 1 test.
		GAIA::CTN::Vector<GAIA::CTN::AString> listUUID;
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			GAIA::MATH::RID128 rid;
			rid.uuid();
			GAIA::CH szTemp[33];
			rid.tostring(szTemp);
			listUUID.push_back(szTemp);
			dmp.insert(szTemp, 16, szTemp);
		}
		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
		{
			const GAIA::CTN::AString* pFinded = dmp.find(listUUID[x].fptr(), 16);
			if(pFinded == GNIL)
			{
				TERROR;
				break;
			}
		}
		if(dmp.size() != listUUID.size())
			TERROR;
		if(dmp.empty())
			TERROR;
		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
		{
			if(!dmp.erase(listUUID[x].fptr(), 16, listUUID[x]))
			{
				TERROR;
				break;
			}
		}
		if(dmp.size() != 0)
			TERROR;
		TAST(dmp.empty());

		// Erase 2 test.
		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
			dmp.insert(listUUID[x].fptr(), 16, listUUID[x].fptr());
		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
		{
			const GAIA::CTN::AString* pFinded = dmp.find(listUUID[x].fptr(), 16);
			if(pFinded == GNIL)
			{
				TERROR;
				break;
			}
		}
		if(dmp.size() != listUUID.size())
			TERROR;
		if(dmp.empty())
			TERROR;
		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
		{
			if(dmp.erase(listUUID[x].fptr(), 16) != 1)
			{
				TERROR;
				break;
			}
		}
		if(dmp.size() != 0)
			TERROR;
		TAST(dmp.empty());

		// Erase 3 test.
		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
			dmp.insert(listUUID[x].fptr(), 16, listUUID[x].fptr());
		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
		{
			const GAIA::CTN::AString* pFinded = dmp.find(listUUID[x].fptr(), 16);
			if(pFinded == GNIL)
			{
				TERROR;
				break;
			}
		}
		if(dmp.size() != listUUID.size())
			TERROR;
		if(dmp.empty())
			TERROR;
		for(GAIA::NUM x = 0; x < listUUID.size(); ++x)
		{
			if(dmp.erase(listUUID[x].fptr(), 16, GAIA::False, GAIA::False) != 1)
			{
				TERROR;
				break;
			}
		}
		if(dmp.size() != 0)
			TERROR;
		TAST(dmp.empty());
	}
}
