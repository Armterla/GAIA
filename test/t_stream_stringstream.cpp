#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_stream_stringstream(GAIA::LOG::Log& logobj)
	{
		GAIA::STREAM::StringStream stm;
		stm << (GAIA::U8)1 << " ";
		stm << (GAIA::U16)2 << " ";
		stm << (GAIA::U32)3 << " ";
		stm << (GAIA::U64)4 << " ";
		stm << (GAIA::N8)-5 << " ";
		stm << (GAIA::N16)+6 << " ";
		stm << (GAIA::N32)-7 << " ";
		stm << (GAIA::N64)+8 << " ";
		stm << (GAIA::UM)9 << " ";
		stm << (GAIA::NM)-10 << " ";
		stm << (GAIA::BL)GAIA::True << " ";
		stm << (GAIA::F32)11.123456 << " ";
		stm << (GAIA::F64)-12.654321 << " ";
		stm << (GAIA::CH*)"ansi string" << " ";
		stm << (GAIA::WCH*)L"unicode string" << " ";

		const GAIA::STREAM::StringStream::__CharType* p = stm.getstring();
		if(p == GNIL)
			TERROR;

		GAIA::U8 u8;
		GAIA::U16 u16;
		GAIA::U32 u32;
		GAIA::U64 u64;
		GAIA::N8 n8;
		GAIA::N16 n16;
		GAIA::N32 n32;
		GAIA::N64 n64;
		GAIA::UM um;
		GAIA::NM nm;
		GAIA::BL b;
		GAIA::F32 f32;
		GAIA::F64 f64;
		GAIA::CH szTemp[128];
		stm >> u8;
		stm >> u16;
		stm >> u32;
		stm >> u64;
		stm >> n8;
		stm >> n16;
		stm >> n32;
		stm >> n64;
		stm >> um;
		stm >> nm;
		stm >> b;
		stm >> f32;
		stm >> f64;
		stm >> szTemp;
		if(u8 != 1)
			TERROR;
		if(u16 != 2)
			TERROR;
		if(u32 != 3)
			TERROR;
		if(u64 != 4)
			TERROR;
		if(n8 != -5)
			TERROR;
		if(n16 != +6)
			TERROR;
		if(n32 != -7)
			TERROR;
		if(n64 != +8)
			TERROR;
		if(um != 9)
			TERROR;
		if(nm != -10)
			TERROR;
		TAST(b);
		TAST(GAIA::MATH::gequal(f32, 11.123456));
		TAST(GAIA::MATH::gequal(f64, -12.654321));
		if(GAIA::ALGO::gstrcmp(szTemp, " ansi string unicode string ") != 0)
			TERROR;
	}
}
