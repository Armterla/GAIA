#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_stringpool(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::StringPool<GAIA::TCH> __StringPoolType;

		__StringPoolType sp, sp1;
		GAIA::NUM s1, s2, s11, s22, st;
		TAST(sp.empty());
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		sp.clear();
		TAST(sp.empty());
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		sp.destroy();
		TAST(sp.empty());
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		sp.reserve(100);
		if(sp.capacity() != 100)
			TERROR;
		s1 = sp.alloc(_T("HelloWorld"));
		if(GAIA::ALGO::gstrcmp(sp.get(s1), _T("HelloWorld")) != 0)
			TERROR;
		st = sp.request(_T("HelloWorld"));
		if(st == GINVALID)
			TERROR;
		if(!sp.release(st))
			TERROR;
		st = sp.request(_T("HelloKitty"));
		if(st != GINVALID)
			TERROR;
		s2 = sp.alloc(_T("HelloKitty"));
		if(GAIA::ALGO::gstrcmp(sp.get(s2), _T("HelloKitty")) != 0)
			TERROR;
		if(sp.empty())
			TERROR;
		if(sp.size() != 2)
			TERROR;
		if(sp.capacity() < sp.size())
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
		s11 = sp1.alloc(_T("ABC"));
		if(GAIA::ALGO::gstrcmp(sp1.get(s11), _T("ABC")) != 0)
			TERROR;
		s22 = sp1.alloc(_T("DEF"));
		if(GAIA::ALGO::gstrcmp(sp1.get(s22), _T("DEF")) != 0)
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
		if(sp.size() != 2)
			TERROR;
		if(sp1.size() != 2)
			TERROR;
		if(sp.capacity() < sp.size())
			TERROR;
		if(sp1.capacity() < sp1.size())
			TERROR;
		TAST(sp.release(s1));
		TAST(sp.release(s2));
		TAST(sp1.release(s11));
		TAST(sp1.release(s22));
		if(sp.practice_size() != 0)
			TERROR;
		if(sp1.practice_size() != 0)
			TERROR;
	}
}
