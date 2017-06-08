#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class CustomData : public GAIA::Base
	{
	public:
		GAIA::U64 hash() const
		{
			return m_v;
		}
		GAIA::BL operator == (const CustomData& src) const
		{
			return m_v == src.m_v;
		}
		GAIA::BL operator != (const CustomData& src) const
		{
			return m_v != src.m_v;
		}
	public:
		GAIA::N64 m_v;
	};
	extern GAIA::GVOID t_ctn_hashset(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM ELEMENT_COUNT = 10;
		typedef GAIA::CTN::BasicHashSet<GAIA::N32, GAIA::N32, GAIA::N32, GAIA::ALGO::ExtendGold<GAIA::N32> > __HashSetType;
		__HashSetType hs, hs1;
		TAST(hs.empty());
		TAST(hs.capacity() == 0);
		TAST(hs.size() == 0);
		for(__HashSetType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			if(!hs.insert(x))
			{
				TERROR;
				break;
			}
		}
		TAST(!hs.empty());
		for(__HashSetType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			__HashSetType::_datatype* pFinded = hs.find(x);
			if(pFinded == GNIL)
			{
				TERROR;
				break;
			}
			if(*pFinded != x)
			{
				TERROR;
				break;
			}
			__HashSetType::it it = hs.findit(x);
			if(it.empty())
			{
				TERROR;
				break;
			}
			if(*it != x)
			{
				TERROR;
				break;
			}
			__HashSetType::const_it cit = hs.const_findit(x);
			if(cit.empty())
			{
				TERROR;
				break;
			}
			if(*cit != x)
			{
				TERROR;
				break;
			}
		}
		hs1 = hs;
		TAST(hs1.size() == hs.size());
		for(__HashSetType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			if(!hs.erase(x))
			{
				TERROR;
				break;
			}
		}
		TAST(hs.empty());
		TAST(hs.size() == 0);
		TAST(hs.capacity() != 0);
		TAST(!hs.erase(-1));
		hs = hs1;
		__HashSetType::it itfront = hs.frontit();
		TAST(!itfront.empty());
		__HashSetType::it itback = hs.backit();
		TAST(!itback.empty());
		__HashSetType::const_it citfront = hs.const_frontit();
		TAST(!citfront.empty());
		__HashSetType::const_it citback = hs.const_backit();
		TAST(!citback.empty());
		for(__HashSetType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			TAST(!itfront.empty());
			TAST(!itback.empty());
			TAST(!citfront.empty());
			TAST(!citback.empty());
			++itfront;
			--itback;
			++citfront;
			--citback;
		}
		TAST(itfront.empty());
		TAST(itback.empty());
		TAST(citfront.empty());
		TAST(citback.empty());
		itfront = hs.frontit();
		itback = hs.backit();
		citfront = hs.const_frontit();
		citback = hs.const_backit();
		hs.clear();
		hs.destroy();
		hs1.destroy();
		hs1.clear();
		TAST(hs.empty());
		TAST(hs1.empty());

		GAIA::CTN::HashSet<CustomData> hs_userdecl;
		for(__HashSetType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			CustomData cd;
			cd.m_v = x;
			hs_userdecl.insert(cd);
		}
		for(__HashSetType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			CustomData cd;
			cd.m_v = x;
			CustomData* pFinded = hs_userdecl.find(cd);
			if(pFinded == GNIL)
			{
				TERROR;
				break;
			}
			if(pFinded->m_v != x)
			{
				TERROR;
				break;
			}
			GAIA::CTN::HashSet<CustomData>::it it = hs_userdecl.findit(cd);
			if(it.empty())
			{
				TERROR;
				break;
			}
			if((*it).m_v != x)
			{
				TERROR;
				break;
			}
			GAIA::CTN::HashSet<CustomData>::const_it cit = hs_userdecl.const_findit(cd);
			if(cit.empty())
			{
				TERROR;
				break;
			}
			if((*cit).m_v != x)
			{
				TERROR;
				break;
			}
		}

		static const GAIA::CH* K1 = "HelloWorld";
		static const GAIA::CH* K2 = "HelloKitty";
		static const GAIA::CH* K3 = "HelloGAIA";
		GAIA::CTN::HashSet<const GAIA::CH*> hsbystr;
		if(!hsbystr.insert(K1))
			TERROR;
		if(!hsbystr.insert(K2))
			TERROR;
		if(!hsbystr.insert(K3))
			TERROR;
		const GAIA::CH** ppFinded = hsbystr.find(K1);
		if(ppFinded == GNIL)
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*ppFinded, K1))
			TERROR;
		ppFinded = hsbystr.find(K2);
		if(ppFinded == GNIL)
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*ppFinded, K2))
			TERROR;
		ppFinded = hsbystr.find(K3);
		if(ppFinded == GNIL)
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*ppFinded, K3))
			TERROR;
		GAIA::CTN::HashSet<const GAIA::CH*>::it it = hsbystr.findit(K1);
		if(it.empty())
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*it, K1))
			TERROR;
		GAIA::CTN::HashSet<const GAIA::CH*>::const_it cit = hsbystr.const_findit(K1);
		if(cit.empty())
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*cit, K1))
			TERROR;
		it = hsbystr.findit(K2);
		if(it.empty())
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*it, K2))
			TERROR;
		cit = hsbystr.const_findit(K2);
		if(cit.empty())
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*cit, K2))
			TERROR;
		it = hsbystr.findit(K3);
		if(it.empty())
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*it, K3))
			TERROR;
		cit = hsbystr.const_findit(K3);
		if(cit.empty())
			TERROR;
		else if(!GAIA::ALGO::gstrequal(*cit, K3))
			TERROR;
		
		// By String.
		{
			GAIA::CTN::HashSet<GAIA::CTN::AString> set_str;
			TAST(set_str.insert("Hello World"));
			TAST(set_str.insert("Hello Kitty"));
			GAIA::CTN::AString* pFinded = set_str.find("Hello World");
			TAST(pFinded != GNIL);
			pFinded = set_str.find("Hello Kitty");
			TAST(pFinded != GNIL);
			pFinded = set_str.find("Hello Arm");
			TAST(pFinded == GNIL);
		}
		
		// By Chars.
		{
			GAIA::CTN::HashSet<GAIA::CTN::AChars> set_chs;
			TAST(set_chs.insert("Hello World"));
			TAST(set_chs.insert("Hello Kitty"));
			GAIA::CTN::AChars* pFinded = set_chs.find("Hello World");
			TAST(pFinded != GNIL && *pFinded == "Hello World");
			pFinded = set_chs.find("Hello Kitty");
			TAST(pFinded != GNIL && *pFinded == "Hello Kitty");
			pFinded = set_chs.find("Hello Arm");
			TAST(pFinded == GNIL);
		}
		
		// By CharsString.
		{
			GAIA::CTN::HashSet<GAIA::CTN::ACharsString> set_chsstr;
			TAST(set_chsstr.insert("Hello World"));
			TAST(set_chsstr.insert("Hello Kitty"));
			GAIA::CTN::ACharsString* pFinded = set_chsstr.find("Hello World");
			TAST(pFinded != GNIL && *pFinded == "Hello World");
			pFinded = set_chsstr.find("Hello Kitty");
			TAST(pFinded != GNIL && *pFinded == "Hello Kitty");
			pFinded = set_chsstr.find("Hello Arm");
			TAST(pFinded == GNIL);
		}
	}
}
