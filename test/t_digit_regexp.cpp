#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_regexp(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::DIGIT::RegExp __RegExpType;
		__RegExpType re;
		__RegExpType::CompileOption co;
		co.reset();
		TAST(re.Compile(_T("Hello"), co, sizeof(GAIA::TCH)));
		__RegExpType::ExecuteOption eo;
		eo.reset();
		TAST(re.Execute(_T("Hello World"), -1, 0, eo));

		re.SetResultMaxLen(10);
		const GAIA::TCH* pszSource = _T("#include <stdlib.h>\r\nint main(){\r\n\treturn 0;\r\n}");

		const GAIA::TCH* pszFinder = _T("#include");
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		const __RegExpType::ResultInfo& ri = re.GetResultInfo();
		if(ri.offsets.size() != 1)
			TERROR;

		pszFinder = _T("#include");
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		if(ri.offsets.size() != 1)
			TERROR;

		pszFinder = _T("stdlib.h");
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		if(ri.offsets.size() != 1)
			TERROR;

		pszFinder = _T("<stdlib.h>");
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		if(ri.offsets.size() != 1)
			TERROR;

		pszFinder = _T("int main");
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		if(ri.offsets.size() != 1)
			TERROR;

		pszFinder = _T("int main\\(\\)");
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		if(ri.offsets.size() != 1)
			TERROR;

		pszFinder = _T("^");
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		if(ri.offsets.size() != 1)
			TERROR;

		pszFinder = _T("\t");
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		if(ri.offsets.size() != 1)
			TERROR;

		pszFinder = _T("^int\\smain.*$");
		co.bMultiLine = true;
		TAST(re.Compile(pszFinder, co, sizeof(GAIA::TCH)));
		TAST(re.Execute(pszSource, -1, 0, eo));
		if(ri.offsets.size() != 1)
			TERROR;
	}
}
