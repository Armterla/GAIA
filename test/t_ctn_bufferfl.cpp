#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_bufferfl(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BufferFL __BufferType;
		__BufferType b;
		TAST(b.empty());
		if(b.read_size() != 0)
			TERROR;
		if(b.write_size() != 0)
			TERROR;
		if(b.capacity() != 0)
			TERROR;
		if(b.fptr() != GNIL || b.bptr() != GNIL)
			TERROR;
		if(b.read_ptr() != GNIL || b.write_ptr() != GNIL)
			TERROR;
		if(GSCAST(const __BufferType*)(&b)->fptr() != GNIL || GSCAST(const __BufferType*)(&b)->bptr() != GNIL)
			TERROR;
		if(GSCAST(const __BufferType*)(&b)->read_ptr() != GNIL || GSCAST(const __BufferType*)(&b)->write_ptr() != GNIL)
			TERROR;
		GAIA::U8 buf[100];
		b.proxy(buf, buf + sizeof(buf));
		TAST(b.empty());
		TAST(b.write_size() == 0 && b.read_size() == 0);
		TAST(b.capacity() == sizeof(buf));
		TAST(b.writex<GAIA::U8>("Hello World"));
		GAIA::CH szTemp[12];
		TAST(b.readx<GAIA::U8>(szTemp) == sizeof(szTemp) - 1);
		TAST(b.read_size() == b.write_size());
		TAST(b.read_size() == sizeof(szTemp));
		b.clear();
		TAST(b.read_size() == 0);
		TAST(b.write_size() == 0);
		TAST(b.capacity() == sizeof(buf));
		b.destroy();
		TAST(b.read_size() == 0);
		TAST(b.write_size() == 0);
		TAST(b.capacity() == 0);
		b.destroy();
		b.proxy(buf, buf + sizeof(buf));
		b.write(123);
		b.write(456);
		if(b.readx<GAIA::NUM>() != 123)
			TERROR;
		if(b.readx<GAIA::NUM>() != 456)
			TERROR;
		b.destroy();
		b.proxy(buf, buf + sizeof(buf));
		b.write((const GAIA::U8*)"12345678", 8);
		b.seek_read(4);
		b.keep_remain();
		if(b.remain() != 4)
			TERROR;
		if(b.read_size() != 0)
			TERROR;
		if(b.write_size() != 4)
			TERROR;
		if(b.readx<GAIA::U8>() != '5')
			TERROR;
		if(b.readx<GAIA::U8>() != '6')
			TERROR;
		if(b.readx<GAIA::U8>() != '7')
			TERROR;
		if(b.readx<GAIA::U8>() != '8')
			TERROR;
		b.destroy();
		b.proxy(buf, buf + sizeof(buf));
		b.write("HelloWorld", GAIA::ALGO::gstrlen("HelloWorld"));
		if(b.find(_T("Hello"), GAIA::ALGO::gstrlen(_T("Hello")), 0) != 0)
			TERROR;
		if(b.find(_T("HelloWorld"), GAIA::ALGO::gstrlen(_T("HelloWorld")), 0) != 0)
			TERROR;
		if(b.find(_T("Wo"), GAIA::ALGO::gstrlen(_T("Wo")), 0) != 5)
			TERROR;
		if(!b.startwith("Hello", GAIA::ALGO::gstrlen("Hello")))
			TERROR;
		if(b.startwith("World", GAIA::ALGO::gstrlen("World")))
			TERROR;
		if(!b.endwith("World", GAIA::ALGO::gstrlen("World")))
			TERROR;
		if(b.endwith("Hello", GAIA::ALGO::gstrlen("World")))
			TERROR;
		b.destroy();
		b.proxy(buf, buf + sizeof(buf));
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			if(!b.write(&x, sizeof(x)))
			{
				TERROR;
				break;
			}
		}
		for(GAIA::NUM x = 0; x < 10; ++x)
		{
			GAIA::NUM r;
			if(!b.peek(&r, sizeof(r)))
			{
				TERROR;
				break;
			}
			if(!b.drop(sizeof(r)))
			{
				TERROR;
				break;
			}
		}
		if(b.remain() != 0)
			TERROR;
	}
}
