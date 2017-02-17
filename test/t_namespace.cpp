#include "preheader.h"
#include "t_common.h"

#include "gaia_using_namespace.h"

namespace TEST
{
	extern GVOID t_namespace(LOG::Log& logobj)
	{
		{
			N32* p = gnew N32[1024];
			for(N32 x = 0; x < 1024; ++x)
				p[x] = xrandom();
			qsort(p, p + 1024 - 1);
			unique_order(p, p + 1024 - 1);
			gmemset(p, (N8)17, sizeof(N32) * 1024);
			gmemcpy(p, p + 1, sizeof(N32));
			gmemcmp(p, p + 1, sizeof(N32));
			gdel[] p;
			p = GNIL;
		}
		{
			TCH szTemp[32] = _T("HelloWorld");
			tolower(szTemp[0]);
			isalpha(szTemp[0]);
			isdigit(szTemp[0]);
			gstrlen(szTemp);
			gstrcmp(szTemp, szTemp);
			gstrstr(szTemp, szTemp);
		}
		{
			gusleep(0);
			gsleep(0);
		}
	}
}
