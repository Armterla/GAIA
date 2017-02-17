#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_math_lint(GAIA::LOG::Log& logobj)
	{
		//
		GAIA::MATH::LInt v, vt, v1, v2, v3, v4, v5;
		v1 = 1;
		v2 = 2;
		v3 = 3;
		v4 = 4;
		v5 = 5;

		//
		if(!v.empty())
			TERROR;
		if(v.size() != 0)
			TERROR;

		//
		v = 100;
		if(v.empty())
			TERROR;
		if(v.size() == 0)
			TERROR;
		v += 1;
		v -= 2;
		v *= 3;
		v /= 4;
		v %= 5;
		if(v != 4)
			TERROR;
		GAIA::CH szTemp[GAIA::MAXPL];
		v.tostring(szTemp, sizeof(szTemp));
		if(GAIA::ALGO::gstrcmp(szTemp, "4") != 0)
			TERROR;

		//
		v = 100;
		v += v1;
		v -= v2;
		v *= v3;
		v /= v4;
		v %= v5;
		if(v != 4)
			TERROR;
		v.tostring(szTemp, sizeof(szTemp));
		if(GAIA::ALGO::gstrcmp(szTemp, "4") != 0)
			TERROR;

		//
		v.fromstring("100");
		v += v1;
		v -= v2;
		v *= v3;
		v /= v4;
		v %= v5;
		if(v != 4)
			TERROR;
		v.tostring(szTemp, sizeof(szTemp));
		if(GAIA::ALGO::gstrcmp(szTemp, "4") != 0)
			TERROR;

		//
		GAIA::U8 buf[GAIA::MAXPL];
		v = 100;
		v.tobuffer(buf, sizeof(buf));
		v.frombuffer(buf, GAIA::MAXPL);
		if(v != 100)
			TERROR;

		//
		v.randomprime(10);
		if(v.empty())
			TERROR;
		v.tostring(szTemp, sizeof(szTemp));
		szTemp[0] = '\0';

		//
		v = 100;
		vt = ++v;
		if(v != 101)
			TERROR;
		if(vt != 101)
			TERROR;

		v = 100;
		vt = --v;
		if(v != 99)
			TERROR;
		if(vt != 99)
			TERROR;

		v = 100;
		vt = v++;
		if(v != 101)
			TERROR;
		if(vt != 100)
			TERROR;

		v = 100;
		vt = v--;
		if(v != 99)
			TERROR;
		if(vt != 100)
			TERROR;

		//
		v = 1;
		for(GAIA::NUM x = 0; x < 100; ++x)
			v *= v3;
		for(GAIA::NUM x = 0; x < 100; ++x)
			v /= v3;
		if(v != 1)
			TERROR;
	}
}
