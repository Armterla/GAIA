#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_bitset(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicBitset<GAIA::U32, GAIA::ALGO::ExtendGold<GAIA::NUM> > __BitsetType;
		__BitsetType b;
		TAST(b.empty());
		b.resize(255);
		if(b.size() != 255)
			TERROR;
		if(b.capacity() != 255)
			TERROR;
		b.clear();
		for(__BitsetType::_sizetype x = 0; x < b.size(); ++x)
		{
			if(b.exist(x))
			{
				TERROR;
				break;
			}
		}
		for(__BitsetType::_sizetype x = 0; x < b.size(); ++x)
		{
			b.set(x);
			if(!b.exist(x))
			{
				TERROR;
				break;
			}
			b.reset(x);
			if(b.exist(x))
			{
				TERROR;
				break;
			}
			b.inverse(x);
			if(!b.exist(x))
			{
				TERROR;
				break;
			}
		}
		if(b.size() != 255)
			TERROR;
		if(b.capacity() != 255)
			TERROR;
		b.clear();
		for(__BitsetType::_sizetype x = 0; x < b.size(); ++x)
		{
			if(b.exist(x))
			{
				TERROR;
				break;
			}
		}
		b = ~b;
		__BitsetType b1 = b;
		for(__BitsetType::_sizetype x = 0; x < b.size(); ++x)
		{
			if(b.exist(x) != b1.exist(x))
			{
				TERROR;
				break;
			}
		}
		if(b.bptr() - b.fptr() != 31)
			TERROR;
		b.clear();
		b1.clear();
		b1 = b;
		TAST(b1 == b && b1 >= b && b1 <= b);
		if(b1 != b || b1 > b || b1 < b)
			TERROR;
		b ^= b1;
		b ^= b1;
		if(b != b1)
			TERROR;
		b.clear();
		b1.clear();
		b ^= b;
		if(b != b1)
			TERROR;
		b.clear();
		b1.clear();
		b = ~b;
		b &= b1;
		if(b != b1)
			TERROR;
		b.clear();
		b1.clear();
		b = ~b;
		b &= b1;
		if(b != b1)
			TERROR;
		b.clear();
		b1.clear();
		b = ~b;
		b1 |= b;
		if(b1 != b)
			TERROR;
		b.clear();
		b1.clear();
		b = ~b;
		b |= b;
		if(b != b)
			TERROR;
		b.clear();
		b1.clear();
		TAST(b.zero());
		if(b.one())
			TERROR;
		b = ~b;
		if(b.zero())
			TERROR;
		TAST(b.one());
		b.reset(10);
		if(b.zero())
			TERROR;
		if(b.one())
			TERROR;
	}
}
