#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_locale(GAIA::LOG::Log& logobj)
	{
		GAIA::WCH wszEnglishSrc[1024] = L"Hello World!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"";
		GAIA::CH szEnglishDst[1024];

		/* ASCII coding. */
		GAIA::NUM sLen = GAIA::ALGO::gstrlen(wszEnglishSrc) + 1;
		if(GAIA::LOCALE::w2m(wszEnglishSrc, sLen, szEnglishDst, 1024, GAIA::CHARSET_TYPE_ASCII) != sLen)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szEnglishDst, "Hello World!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"") != 0)
			TERROR;
		if(GAIA::LOCALE::m2w(szEnglishDst, sLen, wszEnglishSrc, 1024, GAIA::CHARSET_TYPE_ASCII) != sLen)
			TERROR;

		/* System coding. */
		GAIA::ALGO::gstrcpy(wszEnglishSrc, L"Hello World!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"");
		sLen = GAIA::ALGO::gstrlen(wszEnglishSrc) + 1;
		if(GAIA::LOCALE::w2m(wszEnglishSrc, sLen, szEnglishDst, 1024, GAIA::CHARSET_TYPE_SYS) != sLen)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szEnglishDst, "Hello World!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"") != 0)
			TERROR;
		if(GAIA::LOCALE::m2w(szEnglishDst, sLen, wszEnglishSrc, 1024, GAIA::CHARSET_TYPE_SYS) != sLen)
			TERROR;

		/* UTF8 coding. */
		GAIA::ALGO::gstrcpy(wszEnglishSrc, L"Hello World!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"");
		sLen = GAIA::ALGO::gstrlen(wszEnglishSrc) + 1;
		if(GAIA::LOCALE::w2m(wszEnglishSrc, sLen, szEnglishDst, 1024, GAIA::CHARSET_TYPE_UTF8) != sLen)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szEnglishDst, "Hello World!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"") != 0)
			TERROR;
		if(GAIA::LOCALE::m2w(szEnglishDst, sLen, wszEnglishSrc, 1024, GAIA::CHARSET_TYPE_UTF8) != sLen)
			TERROR;

		/* ASCII coding. */
		GAIA::ALGO::gstrcpy(wszEnglishSrc, L"你好，世界！!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"");
		sLen = GAIA::ALGO::gstrlen(wszEnglishSrc) + 1;
		if(GAIA::LOCALE::w2m(wszEnglishSrc, sLen, szEnglishDst, 1024, GAIA::CHARSET_TYPE_ASCII) != sLen)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szEnglishDst, L"你好，世界！!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"") == 0)
			TERROR;
		if(GAIA::LOCALE::m2w(szEnglishDst, sLen, wszEnglishSrc, 1024, GAIA::CHARSET_TYPE_ASCII) != sLen)
			TERROR;

		/* System coding. */
		GAIA::ALGO::gstrcpy(wszEnglishSrc, L"你好，世界！!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"");
		sLen = GAIA::ALGO::gstrlen(wszEnglishSrc) + 1;
		if((sLen = GAIA::LOCALE::w2m(wszEnglishSrc, sLen, szEnglishDst, 1024, GAIA::CHARSET_TYPE_SYS)) == 0)
			TERROR;
		if(GAIA::LOCALE::m2w(szEnglishDst, sLen, wszEnglishSrc, 1024, GAIA::CHARSET_TYPE_SYS) == 0)
			TERROR;
		if(GAIA::ALGO::gstrcmp(wszEnglishSrc, L"你好，世界！!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"") != 0)
			TERROR;

		/* UTF8 coding. */
		GAIA::ALGO::gstrcpy(wszEnglishSrc, L"你好，世界！!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"");
		sLen = GAIA::ALGO::gstrlen(wszEnglishSrc) + 1;
		if((sLen = GAIA::LOCALE::w2m(wszEnglishSrc, sLen, szEnglishDst, 1024, GAIA::CHARSET_TYPE_UTF8)) == 0)
			TERROR;
		if(GAIA::LOCALE::m2w(szEnglishDst, sLen, wszEnglishSrc, 1024, GAIA::CHARSET_TYPE_UTF8) == 0)
			TERROR;
		if(GAIA::ALGO::gstrcmp(wszEnglishSrc, L"你好，世界！!@#$ \t\n%^&*()_+-=~`[]{},.<>/?;':\"") != 0)
			TERROR;
	}
}
