#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_stringref(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::TStringRef __StringRefType;
		GAIA::TCH szTemp[] = _T("Hello World! My name is GAIA!");
		__StringRefType str(szTemp, sizeofarray(szTemp) - 1);
		if(str != "Hello World! My name is GAIA!")
			TERROR;
		if(str.empty())
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(szTemp))
			TERROR;
		for(__StringRefType::it it = str.frontit(); !it.empty(); ++it)
		{
			if(*it != szTemp[it - str.frontit()])
			{
				TERROR;
				break;
			}
		}
	}
}