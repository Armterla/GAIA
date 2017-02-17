#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_msg(GAIA::LOG::Log& logobj)
	{
		GAIA::CTN::Msg msg;
		if(msg.size() != 0)
			TERROR;
		if(!msg.empty())
			TERROR;
		msg.clear();
		msg.destroy();
		if(msg.size() != 0)
			TERROR;
		if(!msg.empty())
			TERROR;
		GAIA::CH ch = 1;
		GAIA::U8 uch = 2;
		GAIA::N16 s = 3;
		GAIA::U16 us = 4;
		GAIA::N32 n = 5;
		GAIA::U32 un = 6;
		GAIA::N64 l = 7;
		GAIA::U64 ul = 8;
		GAIA::NM ml = 9;
		GAIA::UM uml = 10;
		GAIA::BL b = GAIA::True;
		GAIA::F32 f = 11.0F;
		GAIA::F64 d = 12.0F;
		msg.push(ch);
		msg.push(uch);
		msg.push(s);
		msg.push(us);
		msg.push(n);
		msg.push(un);
		msg.push((const GAIA::GVOID*)GNIL, 0);
		msg.push(l);
		msg.push(ul);
		msg.push((const char*)GNIL);
		msg.push(ml);
		msg.push(uml);
		msg.push(b);
		msg.push(f);
		msg.push(d);
		msg.push("Hello World!");
		msg.push(L"My Name Is GAIA::CTN::Msg!");
		msg.push("");
		msg.push((const GAIA::GVOID*)"Buffer Content!", sizeof("Buffer Content!"));
		if(msg.empty())
			TERROR;
		GAIA::CTN::Msg msg1 = msg;
		if(msg1.compare(msg) != 0)
			TERROR;
		ch = msg.pop();
		uch = msg.pop();
		s = msg.pop();
		us = msg.pop();
		n = msg.pop();
		un = msg.pop();
		GAIA::N32 nBufferLength;
		const GAIA::GVOID* pNullBinaryBuf = msg.pop(nBufferLength);
		if(pNullBinaryBuf != GNIL)
			TERROR;
		if(nBufferLength != 0)
			TERROR;
		l = msg.pop();
		ul = msg.pop();
		const GAIA::CH* pszNull = msg.pop();
		if(pszNull != GNIL)
			TERROR;
		ml = msg.pop();
		uml = msg.pop();
		b = msg.pop();
		f = msg.pop();
		d = msg.pop();
		if(GAIA::ALGO::gstrcmp((const GAIA::CH*)msg.pop(), "Hello World!") != 0)
			TERROR;
		if(GAIA::ALGO::gstrcmp((const GAIA::WCH*)msg.pop(), L"My Name Is GAIA::CTN::Msg!") != 0)
			TERROR;
		if(GAIA::ALGO::gstrcmp((const GAIA::CH*)msg.pop(), "") != 0)
			TERROR;
		const GAIA::GVOID* p = msg.pop(nBufferLength);
		if(nBufferLength != sizeof("Buffer Content!"))
			TERROR;
		if(GAIA::ALGO::gstrcmp((const GAIA::CH*)p, "Buffer Content!") != 0)
			TERROR;
		if(msg1.compare(msg) != 0)
			TERROR;
		if(msg.tell() != msg.size())
			TERROR;
		GAIA::N32 nBufferSize = msg.tostring((GAIA::WCH*)GNIL, GINVALID);
		GAIA::TCH* pszTemp = gnew GAIA::TCH[nBufferSize * 2];
		GAIA::N32 nPracSize = msg.tostring(pszTemp, nBufferSize * 2);
		if(nPracSize != nBufferSize)
			TERROR;
		if(!msg1.fromstring(pszTemp, nPracSize))
			TERROR;
		GAIA::TCH* pszTemp1 = gnew GAIA::TCH[nBufferSize * 2];
		GAIA::N32 nPracSize1 = msg1.tostring(pszTemp1, nBufferSize * 2);
		if(nPracSize != nPracSize1)
			TERROR;
		if(GAIA::ALGO::gstrcmp(pszTemp, pszTemp1) != 0)
			TERROR;
		gdel[] pszTemp;
		gdel[] pszTemp1;
		if(msg1 != msg)
			TERROR;
		msg.seek(0);
		msg.jump(18);
		p = msg.pop(nBufferLength);
		if(nBufferLength != sizeof("Buffer Content!"))
			TERROR;
		if(GAIA::ALGO::gstrcmp((const GAIA::CH*)p, "Buffer Content!") != 0)
			TERROR;
	}
}
