#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_stackbuffer(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::BasicStackBuffer<GAIA::NUM, 100> __BufferType;
		__BufferType b;
		TAST(b.empty());
		if(b.read_size() != 0)
			TERROR;
		if(b.write_size() != 0)
			TERROR;
		if(b.capacity() != __BufferType::_size)
			TERROR;
		if(b.fptr() == GNIL || b.bptr() == GNIL)
			TERROR;
		if(b.read_ptr() == GNIL || b.write_ptr() == GNIL)
			TERROR;
		if(GSCAST(const __BufferType*)(&b)->fptr() == GNIL || GSCAST(const __BufferType*)(&b)->bptr() == GNIL)
			TERROR;
		if(GSCAST(const __BufferType*)(&b)->read_ptr() == GNIL || GSCAST(const __BufferType*)(&b)->write_ptr() == GNIL)
			TERROR;
		b.resize(50);
		if(b.read_size() != 0 || b.write_size() != 50)
			TERROR;
		b.clear();
		if(b.read_size() != 0 || b.write_size() != 0)
			TERROR;
		if(b.capacity() != __BufferType::_size)
			TERROR;
		if(b.read_size() != 0 || b.write_size() != 0)
			TERROR;
		b.resize(40);
		if(b.read_size() != 0 || b.write_size() != 40)
			TERROR;
		b.write(_T("HelloWorld"), GAIA::ALGO::gstrlen(_T("HelloWorld")) * sizeof(GAIA::TCH) + sizeof(GAIA::TCH));
		if(b.write_size() != (GAIA::NUM)(40 + GAIA::ALGO::gstrlen(_T("HelloWorld")) * sizeof(GAIA::TCH) + sizeof(GAIA::TCH)))
			TERROR;
		if(b.read_size() != 0)
			TERROR;
		if(b.capacity() < b.write_size())
			TERROR;
		GAIA::U8 arr[40];
		TAST(b.read(arr, sizeof(arr)));
		GAIA::TCH sz[100];
		TAST(b.read(sz));
		if(GAIA::ALGO::gstrcmp(sz, _T("HelloWorld")) != 0)
			TERROR;
		if(b.read_size() != b.write_size())
			TERROR;
		TAST(b.seek_write(0, GAIA::SEEK_TYPE_BEGIN));
		TAST(b.seek_read(0, GAIA::SEEK_TYPE_BEGIN));
		GAIA::ALGO::gmemset(arr, 0xFF, sizeof(arr));
		b.write(arr, sizeof(arr));
		b.seek_read(b.write_size(), GAIA::SEEK_TYPE_BEGIN);
		TAST(b.read(sz));
		if(GAIA::ALGO::gstrcmp(sz, _T("HelloWorld")) != 0)
			TERROR;
		if(b.read_size() <= b.write_size())
			TERROR;
		b.seek_read(b.write_size(), GAIA::SEEK_TYPE_BEGIN);
		GAIA::ALGO::gmemset(sz, 0xFF, sizeof(sz));
		TAST(b.read(sz, GAIA::ALGO::gstrlen(_T("HelloWorld")) * sizeof(GAIA::TCH) + sizeof(GAIA::TCH)));
		if(GAIA::ALGO::gstrcmp(sz, _T("HelloWorld")) != 0)
			TERROR;
		b.clear();
		b.write(_T("HelloWorld"));
		__BufferType b1 = b;
		GAIA::ALGO::gmemset(sz, 0xFF, sizeof(sz));
		b1.read(sz);
		if(GAIA::ALGO::gstrcmp(sz, _T("HelloWorld")) != 0)
			TERROR;
		TAST(b == b1);
		TAST(b >= b1);
		TAST(b <= b1);
		if(b != b1)
			TERROR;
		if(b > b1)
			TERROR;
		if(b < b1)
			TERROR;
		TAST(b1 == b && b1 >= b && b1 <= b);
		if(b1 != b || b1 > b || b1 < b)
			TERROR;
		b.clear();
		b.write(123);
		b.write(456);
		if(b.readx<GAIA::NUM>() != 123)
			TERROR;
		if(b.readx<GAIA::NUM>() != 456)
			TERROR;
		b.clear();
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
		b.clear();
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
	}
}
