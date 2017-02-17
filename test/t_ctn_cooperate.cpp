#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_cooperate(GAIA::LOG::Log& logobj)
	{
		// Chars string cooperate.
		{
			typedef GAIA::CTN::BasicChars<GAIA::TCH, GAIA::N16, 64> __CharsType;
			typedef GAIA::CTN::BasicString<GAIA::TCH, GAIA::N32> __StringType;

			__CharsType chs = _T("Hello");
			__StringType str = _T("World");
			chs = str;
			if(chs != _T("World"))
				TERROR;
			chs.clear();
			chs = str;
			if(chs != _T("World"))
				TERROR;
			chs = _T("Hello");
			str = chs;
			if(str != _T("Hello"))
				TERROR;
			str.destroy();
			str = chs;
			if(str != _T("Hello"))
				TERROR;
			chs = _T("Hello");
			str = _T("World");
			chs += str;
			if(chs != _T("HelloWorld"))
				TERROR;
			chs = _T("World");
			str = _T("Hello");
			str += chs;
			if(str != _T("HelloWorld"))
				TERROR;
			chs = _T("Hello");
			str = _T("World");
			__CharsType chs_res = chs + str;
			if(chs_res != _T("HelloWorld"))
				TERROR;
			str = _T("Hello");
			chs = _T("World");
			__StringType str_res = str + chs;
			if(str_res != _T("HelloWorld"))
				TERROR;
		}
	}
}
