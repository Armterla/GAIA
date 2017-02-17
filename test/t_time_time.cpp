#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_time_time(GAIA::LOG::Log& logobj)
	{
		static const GAIA::TCH* STRTIMELIST[] =
		{
			_T("20141001"),
			_T("201410010830"),
			_T("20141001083000"),
			_T("20141001083059999"),
			_T("20141001083059999123"),
		};
		static const GAIA::TCH* STRTIMELISTAUX[] =
		{
			_T("2014-10-01"),
			_T("2014-10-01_08:30"),
			_T("2014-10-01_08:30:00"),
			_T("2014-10-01_08:30:59_999"),
			_T("2014-10-01_08:30:59_999_123"),
		};

		GAIA::TCH szTemp[32], szTempAux[32];
		for(GAIA::NUM x = 0; x < sizeofarray(STRTIMELIST); ++x)
		{
			szTemp[0] = '\0';
			szTempAux[0] = '\0';
			GAIA::ALGO::gstrcpy(szTemp, STRTIMELIST[x]);
			if(!GAIA::TIME::timemkaux(szTemp, szTempAux))
			{
				TERROR;
				break;
			}
			if(GAIA::ALGO::gstrcmp(szTempAux, STRTIMELISTAUX[x]) != 0)
			{
				TERROR;
				break;
			}
			szTemp[0] = '\0';
			szTempAux[0] = '\0';
			GAIA::ALGO::gstrcpy(szTempAux, STRTIMELISTAUX[x]);
			if(!GAIA::TIME::timermaux(szTempAux, szTemp))
			{
				TERROR;
				break;
			}
			if(GAIA::ALGO::gstrcmp(szTemp, STRTIMELIST[x]) != 0)
			{
				TERROR;
				break;
			}
		}

		GAIA::TIME::Time t, t1;
		t.reset();
		if(t.check())
			TERROR;
		t1 = t;
		if(t1.check())
			TERROR;
		for(GAIA::NUM x = 0; x < sizeofarray(STRTIMELIST); ++x)
		{
			t = STRTIMELIST[x];

			t.to(szTemp);
			if(GAIA::ALGO::gstrcmp(szTemp, STRTIMELIST[x], GAIA::ALGO::gstrlen(STRTIMELIST[x])) != 0)
			{
				TERROR;
				break;
			}
			if(!t.isvalid())
			{
				TERROR;
				break;
			}

			GAIA::U64 uCompressTime;
			t.to(uCompressTime);
			t.from(uCompressTime);
			if(GAIA::ALGO::gstrcmp(szTemp, STRTIMELIST[x], GAIA::ALGO::gstrlen(STRTIMELIST[x])) != 0)
			{
				TERROR;
				break;
			}
			if(!t.isvalid())
			{
				TERROR;
				break;
			}

			GAIA::N64 usec = t.usecond();
			GAIA::TIME::Time tt;
			tt.usecond(usec);
			if(tt != t)
			{
				TERROR;
				break;
			}
			if(!(t == STRTIMELIST[x]))
			{
				TERROR;
				break;
			}
			if(t != STRTIMELIST[x])
			{
				TERROR;
				break;
			}
			if(!(t >= STRTIMELIST[x]))
			{
				TERROR;
				break;
			}
			if(!(t <= STRTIMELIST[x]))
			{
				TERROR;
				break;
			}
			if(t > STRTIMELIST[x])
			{
				TERROR;
				break;
			}
			if(t < STRTIMELIST[x])
			{
				TERROR;
				break;
			}
			GAIA::N64 nTime = t;
			if(!(t == nTime))
			{
				TERROR;
				break;
			}
			if(t != nTime)
			{
				TERROR;
				break;
			}
			if(!(t >= nTime))
			{
				TERROR;
				break;
			}
			if(!(t <= nTime))
			{
				TERROR;
				break;
			}
			if(t > nTime)
			{
				TERROR;
				break;
			}
			if(t < nTime)
			{
				TERROR;
				break;
			}
			t = nTime;
			if(!t.isvalid())
			{
				TERROR;
				break;
			}
			if(!(t == STRTIMELIST[x]))
			{
				TERROR;
				break;
			}
			if(t != STRTIMELIST[x])
			{
				TERROR;
				break;
			}
			if(!(t >= STRTIMELIST[x]))
			{
				TERROR;
				break;
			}
			if(!(t <= STRTIMELIST[x]))
			{
				TERROR;
				break;
			}
			if(t > STRTIMELIST[x])
			{
				TERROR;
				break;
			}
			if(t < STRTIMELIST[x])
			{
				TERROR;
				break;
			}
		}

		t = _T("20000930083059999123");
		GAIA::N64 usec = t.usecond();
		t.usecond(usec);
		if(t != _T("20000930083059999123"))
			TERROR;

		t = _T("20001001083059999123");
		usec = t.usecond();
		t.usecond(usec);
		if(t != _T("20001001083059999123"))
			TERROR;

		t = _T("20140930083059999123");
		usec = t.usecond();
		t.usecond(usec);
		if(t != _T("20140930083059999123"))
			TERROR;

		t = _T("20141001083059999123");
		usec = t.usecond();
		t.usecond(usec);
		if(t != _T("20141001083059999123"))
			TERROR;

		t = _T("20000930083059999123");
		t.dayinc();
		TAST(t.isvalid() && t == _T("20001001083059999123"));

		t = _T("20001001083059999123");
		t.daydec();
		TAST(t.isvalid() && t == _T("20000930083059999123"));

		t = _T("20000930083059999123");
		t += GSCAST(GAIA::N64)(24) * 60 * 60 * 1000 * 1000;
		TAST(t.isvalid() && t == _T("20001001083059999123"));

		t = _T("20001001083059999123");
		t -= GSCAST(GAIA::N64)(24) * 60 * 60 * 1000 * 1000;
		TAST(t.isvalid() && t == _T("20000930083059999123"));

		t = _T("20000930083059999123");
		t = t + GSCAST(GAIA::N64)(24) * 60 * 60 * 1000 * 1000;
		TAST(t.isvalid() && t == _T("20001001083059999123"));

		t = _T("20001001083059999123");
		t = t - GSCAST(GAIA::N64)(24) * 60 * 60 * 1000 * 1000;
		TAST(t.isvalid() && t == _T("20000930083059999123"));
	}
}
