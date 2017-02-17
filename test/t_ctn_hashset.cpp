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
		}
	}
}