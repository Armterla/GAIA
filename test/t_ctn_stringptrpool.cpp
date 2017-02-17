#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_stringptrpool(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::StringPtrPool<GAIA::TCH> __StringPtrPoolType;

		__StringPtrPoolType sp, sp1;
		const GAIA::TCH* psz;
		const GAIA::TCH* psz1;
		const GAIA::TCH* psz2;
		const GAIA::TCH* pszt;
		if(!sp.empty())
			TERROR;
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		sp.clear();
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		sp.destroy();
		if(sp.size() != 0)
			TERROR;
		if(sp.capacity() != 0)
			TERROR;
		sp.reserve(100);
		if(sp.capacity() != 100)
			TERROR;
		psz = sp.alloc(_T("HelloWorld"));
		if(GAIA::ALGO::gstrcmp(psz, "HelloWorld") != 0)
			TERROR;
		if(sp.size() != 1)
			TERROR;
		if(sp.empty())
			TERROR;
		if(!sp.release(psz))
			TERROR;
		if(sp.size() != 1)
			TERROR;
		if(sp.empty())
			TERROR;
		psz = sp.alloc(_T("HelloWorld"));
		psz1 = sp.alloc(_T("HelloWorld"));
		if(psz != psz1)
			TERROR;
		if(sp.size() != 1)
			TERROR;
		if(sp.empty())
			TERROR;
		if(GAIA::ALGO::gstrcmp(psz, "HelloWorld") != 0)
			TERROR;
		if(GAIA::ALGO::gstrcmp(psz, psz1) != 0)
			TERROR;
		pszt = sp.request(_T("HelloWorld"));
		if(pszt != psz)
			TERROR;
		if(!sp.release(pszt))
			TERROR;
		pszt = sp.request(_T("HelloKitty"));
		if(pszt != GNIL)
			TERROR;
		psz2 = sp.alloc(_T("HelloKitty"));
		if(GAIA::ALGO::gstrcmp(psz2, "HelloKitty") != 0)
			TERROR;
		if(sp.size() != 2)
			TERROR;
		if(sp.string_size() != 2)
			TERROR;
		for(GAIA::NUM x = 0; x < sp.string_size(); ++x)
		{
			const GAIA::TCH* pszTemp = sp.get(x);
			if(pszTemp == GNIL)
			{
				TERROR;
				break;
			}
			if(GAIA::ALGO::gstrcmp(pszTemp, "HelloWorld") != 0 &&
					GAIA::ALGO::gstrcmp(pszTemp, "HelloKitty") != 0)
				TERROR;
		}
		if(sp.find(_T("HelloWorld")) == GNIL)
			TERROR;
		else
		{
			const GAIA::TCH* pszTemp = sp.find(_T("HelloWorld"));
			if(GAIA::ALGO::gstrcmp(pszTemp, "HelloWorld") != 0)
				TERROR;
		}
		if(sp.find(_T("HelloKitty")) == GNIL)
			TERROR;
		else
		{
			const GAIA::TCH* pszTemp = sp.find(_T("HelloKitty"));
			if(GAIA::ALGO::gstrcmp(pszTemp, "HelloKitty") != 0)
				TERROR;
		}
		if(!sp.release(psz))
			TERROR;
		if(!sp.release(psz1))
			TERROR;
		if(!sp.release(psz2))
			TERROR;
		if(sp.size() != 2)
			TERROR;
		if(sp.empty())
			TERROR;
		sp.clear();
		if(sp.size() != 0)
			TERROR;
		if(!sp.empty())
			TERROR;
		sp.destroy();
		psz = sp.alloc(_T("HelloWorld"));
		psz1 = sp.alloc(_T("HelloWorld"));
		psz2 = sp.alloc(_T("HelloKitty"));
		sp1 = sp;
		sp.release(psz);
		if(GAIA::ALGO::gstrcmp(psz, "HelloWorld") != 0)
			TERROR;
		sp.release(psz1);
		if(GAIA::ALGO::gstrcmp(psz, "HelloWorld") != 0)
			TERROR;
		sp.release(psz2);
		if(GAIA::ALGO::gstrcmp(psz2, "HelloKitty") != 0)
			TERROR;
		sp.recycle_bycount(1);
		sp.recycle_bysize(1024);
		sp.recycle();
		sp.destroy();
		if(sp1.size() != 2)
			TERROR;
		if(sp1.empty())
			TERROR;
		for(GAIA::NUM x = 0; x < sp1.string_size(); ++x)
		{
			const GAIA::TCH* pszTemp = sp1.get(x);
			if(pszTemp == GNIL)
			{
				TERROR;
				break;
			}
			if(GAIA::ALGO::gstrcmp(pszTemp, "HelloWorld") != 0 &&
					GAIA::ALGO::gstrcmp(pszTemp, "HelloKitty") != 0)
				TERROR;
		}
		if(sp1.find(_T("HelloWorld")) == 0)
			TERROR;
		else
		{
			const GAIA::TCH* pszTemp = sp1.find(_T("HelloWorld"));
			if(GAIA::ALGO::gstrcmp(pszTemp, "HelloWorld") != 0)
				TERROR;
		}
		if(sp1.find(_T("HelloKitty")) == 0)
			TERROR;
		else
		{
			const GAIA::TCH* pszTemp = sp1.find(_T("HelloKitty"));
			if(GAIA::ALGO::gstrcmp(pszTemp, "HelloKitty") != 0)
				TERROR;
		}
	}
}
