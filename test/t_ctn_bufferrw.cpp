#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_bufferrw(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicBufferRW<GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM>, 13> __BufferRWType;
		__BufferRWType b, b1;
		TAST(b.empty());
		if(b.size() != 0)
			TERROR;
		if(b.capacity() != 0)
			TERROR;
		b.clear();
		b.destroy();
		TAST(b.empty());
		if(b.size() != 0)
			TERROR;
		if(b.capacity() != 0)
			TERROR;
		b.writex<GAIA::U8>("HelloWorld");
		if(b.size() != 11)
			TERROR;
		GAIA::CH szTemp[100];
		TAST(b.readx<GAIA::U8>(szTemp));
		if(GAIA::ALGO::gmemcmp(szTemp, "HelloWorld", 11) != 0)
			TERROR;
		TAST(b.empty());
		b.writex<GAIA::U8>("HelloKitty");
		b.writex<GAIA::U8>("HelloWorld");
		if(b.size() != 22)
			TERROR;
		TAST(b.readx<GAIA::U8>(szTemp));
		if(GAIA::ALGO::gmemcmp(szTemp, "HelloKitty", 11) != 0)
			TERROR;
		TAST(b.readx<GAIA::U8>(szTemp));
		if(GAIA::ALGO::gmemcmp(szTemp, "HelloWorld", 11) != 0)
			TERROR;
		for(GAIA::NUM x = 0; x < 100; ++x)
		{
			GAIA::ALGO::castv(x, szTemp, (GAIA::NUM)sizeof(szTemp));
			b << szTemp;
		}
		b1 = b;
		TAST(b1 == b);
		if(b1 != b)
			TERROR;
		TAST(b1 >= b);
		TAST(b1 <= b);
		if(b1 > b)
			TERROR;
		if(b1 < b)
			TERROR;
		b.destroy();
		b.write(123);
		b.write(456);
		if(b.readx<GAIA::NUM>() != 123)
			TERROR;
		if(b.readx<GAIA::NUM>() != 456)
			TERROR;
	}
}
