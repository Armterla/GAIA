#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_book(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 100;
		typedef GAIA::CTN::Book<GAIA::NUM> __BookType;

		__BookType bt, bt1;
		TAST(bt.empty());
		if(bt.size() != 0)
			TERROR;
		if(bt.capacity() != 0)
			TERROR;
		bt.clear();
		bt.destroy();
		TAST(bt.empty());
		if(bt.size() != 0)
			TERROR;
		if(bt.capacity() != 0)
			TERROR;
		TAST(bt == bt1);
		if(bt != bt1)
			TERROR;
		TAST(bt >= bt1);
		TAST(bt <= bt1);
		if(bt > bt1)
			TERROR;
		if(bt < bt1)
			TERROR;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::NUM sUsedIndex = bt.insert(x);
			if(sUsedIndex != x)
			{
				TERROR;
				break;
			}
		}
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::NUM sFixedIndex = bt.fixedindex(x);
			if(sFixedIndex != x)
			{
				TERROR;
				break;
			}
			GAIA::NUM sUsedIndex = bt.usedindex(sFixedIndex);
			if(sUsedIndex != x)
			{
				TERROR;
				break;
			}
			if(bt.find(x) != sUsedIndex)
			{
				TERROR;
				break;
			}
			if(!bt.exist(x))
			{
				TERROR;
				break;
			}
		}
		bt1 = bt;
		if(bt.capacity() < bt.size())
			TERROR;
		if(bt1.capacity() < bt1.size())
			TERROR;
		TAST(bt == bt1);
		if(bt != bt1)
			TERROR;
		TAST(bt >= bt1);
		TAST(bt <= bt1);
		if(bt > bt1)
			TERROR;
		if(bt < bt1)
			TERROR;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; x += 10)
		{
			GAIA::NUM usedindex = bt.usedindex(x);
			if(usedindex == GINVALID)
			{
				TERROR;
				break;
			}
			if(!bt.erase(usedindex))
			{
				TERROR;
				break;
			}
		}
		if(bt.size() != SAMPLE_COUNT - SAMPLE_COUNT / 10)
			TERROR;
		if(bt.capacity() < bt.size())
			TERROR;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			GAIA::NUM usedindex = bt.usedindex(x);
			if(usedindex == GINVALID)
				continue;
			if(usedindex >= SAMPLE_COUNT - SAMPLE_COUNT / 10)
			{
				TERROR;
				break;
			}
		}
		if(bt1.capacity() < bt1.size())
			TERROR;
		if(bt == bt1)
			TERROR;
		TAST(bt != bt1);
		if(bt >= bt1)
			TERROR;
		TAST(bt <= bt1);
		if(bt > bt1)
			TERROR;
		TAST(bt < bt1);

		bt1 = bt;
		if(bt.capacity() < bt.size())
			TERROR;
		if(bt1.capacity() < bt1.size())
			TERROR;
		TAST(bt == bt1);
		if(bt != bt1)
			TERROR;
		TAST(bt >= bt1);
		TAST(bt <= bt1);
		if(bt > bt1)
			TERROR;
		if(bt < bt1)
			TERROR;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT - SAMPLE_COUNT / 10; ++x)
		{
			if(!bt.erase(0))
			{
				TERROR;
				break;
			}
		}
		TAST(bt.empty());
		if(bt.size() != 0)
			TERROR;
		if(bt.capacity() <= 0)
			TERROR;
		bt.clear();
		bt.destroy();

		{
			GAIA::NUM u1 = bt.insert(0);
			GAIA::NUM f1 = bt.fixedindex(u1);
			GAIA::NUM u2 = bt.insert(1);
			GAIA::NUM f2 = bt.fixedindex(u2);
			TAST(bt.erase(bt.usedindex(f1)));
			u2 = bt.usedindex(f2);
			if(u2 == GINVALID)
				TERROR;
		}

		{
			GAIA::NUM vlist[10];

			static const GAIA::NUM SAMPLESRC = 3;
			static const GAIA::NUM SAMPLEDST = 8;

			// Book swap function test.
			bt.destroy();
			bt1.destroy();
			for(GAIA::NUM x = 0; x < sizeofarray(vlist); ++x)
			{
				GAIA::NUM sUsedIndex = bt.insert(x);
				GAIA::NUM sFixedIndex = bt.fixedindex(sUsedIndex);
				vlist[x] = x;
				if(sUsedIndex != x)
				{
					TERROR;
					break;
				}
				if(sFixedIndex != x)
				{
					TERROR;
					break;
				}
			}
			bt.swap(SAMPLESRC, SAMPLEDST);
			if(bt.fixedindex(SAMPLESRC) != SAMPLEDST)
				TERROR;
			if(bt.fixedindex(SAMPLEDST) != SAMPLESRC)
				TERROR;
			if(bt[SAMPLESRC] != SAMPLEDST)
				TERROR;
			if(bt[SAMPLEDST] != SAMPLESRC)
				TERROR;

			// Book reuse index from small to big test.
			bt.destroy();
			bt1.destroy();
			for(GAIA::NUM x = 0; x < sizeofarray(vlist); ++x)
			{
				GAIA::NUM sUsedIndex = bt.insert(x);
				GAIA::NUM sFixedIndex = bt.fixedindex(sUsedIndex);
				vlist[x] = x;
				if(sUsedIndex != x)
				{
					TERROR;
					break;
				}
				if(sFixedIndex != x)
				{
					TERROR;
					break;
				}
			}
			bt.relocation(SAMPLESRC, SAMPLEDST);
			for(GAIA::NUM x = SAMPLESRC; x < SAMPLEDST; ++x)
				GAIA::ALGO::swap(vlist[x], vlist[x + 1]);
			for(GAIA::NUM x = 0; x < sizeofarray(vlist); ++x)
			{
				if(bt[x] != vlist[x])
				{
					TERROR;
					break;
				}
			}

			// Book reuse index from small to big test.
			bt.destroy();
			bt1.destroy();
			for(GAIA::NUM x = 0; x < sizeofarray(vlist); ++x)
			{
				GAIA::NUM sUsedIndex = bt.insert(x);
				GAIA::NUM sFixedIndex = bt.fixedindex(sUsedIndex);
				vlist[x] = x;
				if(sUsedIndex != x)
				{
					TERROR;
					break;
				}
				if(sFixedIndex != x)
				{
					TERROR;
					break;
				}
			}
			bt.relocation(SAMPLEDST, SAMPLESRC);
			for(GAIA::NUM x = SAMPLEDST; x > SAMPLESRC; --x)
				GAIA::ALGO::swap(vlist[x], vlist[x - 1]);
			for(GAIA::NUM x = 0; x < sizeofarray(vlist); ++x)
			{
				if(bt[x] != vlist[x])
				{
					TERROR;
					break;
				}
			}
		}
	}
}
