#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_staticstringpool(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::StaticStringPool<GAIA::TCH> __StaticStringPoolType;
		__StaticStringPoolType sp, sp1;

		__StaticStringPoolType::_sizetype s1;
		__StaticStringPoolType::_sizetype s2;
		__StaticStringPoolType::_sizetype st;

		TAST(sp.empty());
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		if(sp.string_size() != 0)
			TERROR;
		sp.clear();
		TAST(sp.empty());
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		if(sp.string_size() != 0)
			TERROR;
		sp.destroy();
		TAST(sp.empty());
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		if(sp.string_size() != 0)
			TERROR;
		sp1.clear();
		sp = sp1;
		TAST(sp == sp1);
		if(sp != sp1)
			TERROR;
		TAST(sp >= sp1);
		TAST(sp <= sp1);
		if(sp > sp1)
			TERROR;
		if(sp < sp1)
			TERROR;
		s1 = sp.alloc(_T("HelloWorld"));
		if(GAIA::ALGO::gstrcmp(sp.get(s1), _T("HelloWorld")) != 0)
			TERROR;
		st = sp.request(_T("HelloWorld"));
		if(st == GINVALID)
			TERROR;
		st = sp.request(_T("HelloKitty"));
		if(st != GINVALID)
			TERROR;
		s2 = sp.alloc(_T("HelloKitty"));
		if(GAIA::ALGO::gstrcmp(sp.get(s2), _T("HelloKitty")) != 0)
			TERROR;
		if(sp == sp1)
			TERROR;
		TAST(sp != sp1);
		TAST(sp >= sp1);
		if(sp <= sp1)
			TERROR;
		TAST(sp > sp1);
		if(sp < sp1)
			TERROR;
		s1 = sp1.alloc(_T("ABC"));
		if(GAIA::ALGO::gstrcmp(sp1.get(s1), _T("ABC")) != 0)
			TERROR;
		s2 = sp1.alloc(_T("EFG"));
		if(GAIA::ALGO::gstrcmp(sp1.get(s2), _T("EFG")) != 0)
			TERROR;
		if(sp == sp1)
			TERROR;
		TAST(sp != sp1);
		TAST(sp >= sp1);
		if(sp <= sp1)
			TERROR;
		TAST(sp > sp1);
		if(sp < sp1)
			TERROR;
		sp1 = sp;
		TAST(sp == sp1);
		if(sp != sp1)
			TERROR;
		TAST(sp >= sp1);
		TAST(sp <= sp1);
		if(sp > sp1)
			TERROR;
		if(sp < sp1)
			TERROR;
		if(sp.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")) + 1 + GAIA::ALGO::gstrlen(_T("HelloKitty")) + 1)
			TERROR;
		if(sp.capacity() < sp.size())
			TERROR;
		if(sp.string_size() != 2)
			TERROR;
		if(sp.get(0) == GNIL)
			TERROR;
		if(GAIA::ALGO::gstrcmp(sp.get(0), _T("HelloWorld")) != 0)
			TERROR;
		if(sp.get(1) == GNIL)
			TERROR;
		if(GAIA::ALGO::gstrcmp(sp.get(1), _T("HelloKitty")) != 0)
			TERROR;
		sp.clear();
		s1 = sp.alloc(_T("HelloWorld"));
		if(GAIA::ALGO::gstrcmp(sp.get(s1), _T("HelloWorld")) != 0)
			TERROR;
		s2 = sp.alloc(_T("HelloKitty"));
		if(GAIA::ALGO::gstrcmp(sp.get(s2), _T("HelloKitty")) != 0)
			TERROR;
		if(s1 == s2)
			TERROR;
		s2 = sp.alloc(_T("HelloWorld"));
		if(s1 != s2)
			TERROR;
	}
}
