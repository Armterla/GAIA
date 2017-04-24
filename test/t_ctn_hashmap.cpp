#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class CustomKey : public GAIA::Base
	{
	public:
		GAIA::U64 hash() const
		{
			return m_v;
		}
		GAIA::BL operator == (const CustomKey& src) const
		{
			return m_v == src.m_v;
		}
		GAIA::BL operator != (const CustomKey& src) const
		{
			return m_v != src.m_v;
		}
	public:
		GAIA::N64 m_v;
	};
	extern GAIA::GVOID t_ctn_hashmap(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM ELEMENT_COUNT = 10;
		typedef GAIA::CTN::BasicHashMap<GAIA::N32, GAIA::N32, GAIA::N32, GAIA::N32, GAIA::ALGO::ExtendGold<GAIA::N32> > __HashMapType;
		__HashMapType hm, hm1;
		TAST(hm.empty());
		TAST(hm.capacity() == 0);
		TAST(hm.size() == 0);
		for(__HashMapType::_keytype x = 0; x < ELEMENT_COUNT; ++x)
		{
			if(!hm.insert(x, -x))
				TERROR;
		}
		TAST(!hm.empty());
		for(__HashMapType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			__HashMapType::_datatype* pFinded = hm.find(x);
			if(pFinded == GNIL)
			{
				TERROR;
				break;
			}
			if(*pFinded != -x)
			{
				TERROR;
				break;
			}
			__HashMapType::it it = hm.findit(x);
			if(it.empty())
			{
				TERROR;
				break;
			}
			if(*it != -x)
			{
				TERROR;
				break;
			}
			__HashMapType::const_it cit = hm.const_findit(x);
			if(cit.empty())
			{
				TERROR;
				break;
			}
			if(*cit != -x)
			{
				TERROR;
				break;
			}
		}
		hm1 = hm;
		TAST(hm1.size() == hm.size());
		for(__HashMapType::_keytype x = 0; x < ELEMENT_COUNT; ++x)
		{
			if(!hm.erase(x))
			{
				TERROR;
				break;
			}
		}
		TAST(hm.empty());
		TAST(hm.size() == 0);
		TAST(hm.capacity() != 0);
		TAST(!hm.erase(-1));
		hm = hm1;
		__HashMapType::it itfront = hm.frontit();
		TAST(!itfront.empty());
		__HashMapType::it itback = hm.backit();
		TAST(!itback.empty());
		__HashMapType::const_it citfront = hm.const_frontit();
		TAST(!citfront.empty());
		__HashMapType::const_it citback = hm.const_backit();
		TAST(!citback.empty());
		for(__HashMapType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
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
		itfront = hm.frontit();
		itback = hm.backit();
		citfront = hm.const_frontit();
		citback = hm.const_backit();
		hm.clear();
		hm.destroy();
		hm1.destroy();
		hm1.clear();
		TAST(hm.empty());
		TAST(hm1.empty());

		GAIA::CTN::HashMap<CustomKey, CustomKey> hm_userdecl;
		for(__HashMapType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			CustomKey ck;
			ck.m_v = x;
			hm_userdecl.insert(ck, ck);
		}
		for(__HashMapType::_datatype x = 0; x < ELEMENT_COUNT; ++x)
		{
			CustomKey cd;
			cd.m_v = x;
			CustomKey* pFinded = hm_userdecl.find(cd);
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
			GAIA::CTN::HashMap<CustomKey, CustomKey>::it it = hm_userdecl.findit(cd);
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
			GAIA::CTN::HashMap<CustomKey, CustomKey>::const_it cit = hm_userdecl.const_findit(cd);
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
		GAIA::CTN::HashMap<const GAIA::CH*, GAIA::N32> hmbystr;
		if(!hmbystr.insert(K1, 0))
			TERROR;
		if(!hmbystr.insert(K2, 1))
			TERROR;
		if(!hmbystr.insert(K3, 2))
			TERROR;
		GAIA::N32* pFinded = hmbystr.find(K1);
		if(pFinded == GNIL)
			TERROR;
		else if(*pFinded != 0)
			TERROR;
		pFinded = hmbystr.find(K2);
		if(pFinded == GNIL)
			TERROR;
		else if(*pFinded != 1)
			TERROR;
		pFinded = hmbystr.find(K3);
		if(pFinded == GNIL)
			TERROR;
		else if(*pFinded != 2)
			TERROR;
		GAIA::CTN::HashMap<const GAIA::CH*, GAIA::N32>::it it = hmbystr.findit(K1);
		if(it.empty())
			TERROR;
		else if(*it != 0)
			TERROR;
		GAIA::CTN::HashMap<const GAIA::CH*, GAIA::N32>::const_it cit = hmbystr.const_findit(K1);
		if(cit.empty())
			TERROR;
		else if(*cit != 0)
			TERROR;
		it = hmbystr.findit(K2);
		if(it.empty())
			TERROR;
		else if(*it != 1)
			TERROR;
		cit = hmbystr.const_findit(K2);
		if(cit.empty())
			TERROR;
		else if(*cit != 1)
			TERROR;
		it = hmbystr.findit(K3);
		if(it.empty())
			TERROR;
		else if(*it != 2)
			TERROR;
		cit = hmbystr.const_findit(K3);
		if(cit.empty())
			TERROR;
		else if(*cit != 2)
			TERROR;
	}
}
