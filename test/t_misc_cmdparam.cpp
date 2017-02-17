#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_misc_cmdparam(GAIA::LOG::Log& logobj)
	{
		GAIA::MISC::CmdParam cp;
		if(cp.is_begin_decl())
			TERROR;
		cp.clear_decl();
		cp.clear_cmd();
		cp.begin_decl();
		{
			TAST(cp.is_begin_decl());
			/*
			*	TEST CASE:
			*		-D[1,] -d[1,] -r -m -o[1,] -omax[1,] -s[1,1]
			*		MUTEX GROUP:-D -d
			*		MUTEX GROUP:-o -s
			*		COOP GROUP:-o -omax
			*/
			if(cp.cmd_decl(_T("-D"), _T("Force delete files."), 1, GINVALID, GAIA::MISC::CmdParam::CMD_TYPE_INVALID) == GINVALID)
				TERROR;
			TAST(cp.cmd_param_decl(_T("-D"), 1, GINVALID, 0, GINVALID, _T("FileName"), GAIA::MISC::CmdParam::PARAM_TYPE_MUSTEXISTFILE));
			if(cp.cmd_decl(_T("-d"), _T("Delete files."), 1, GINVALID, GAIA::MISC::CmdParam::CMD_TYPE_INVALID) == GINVALID)
				TERROR;
			TAST(cp.cmd_param_decl(_T("-d"), 1, GINVALID, 0, GINVALID, _T("FileName"), GAIA::MISC::CmdParam::PARAM_TYPE_MUSTEXISTFILE));
			if(cp.cmd_decl(_T("-r"), _T("Recursive search files."), 0, 0, GAIA::MISC::CmdParam::CMD_TYPE_INVALID) == GINVALID)
				TERROR;
			if(cp.cmd_decl(_T("-m"), _T("Show message."), 0, 0, GAIA::MISC::CmdParam::CMD_TYPE_INVALID) == GINVALID)
				TERROR;
			if(cp.cmd_decl(_T("-o"), _T("Set output file."), 1, GINVALID, GAIA::MISC::CmdParam::CMD_TYPE_INVALID) == GINVALID)
				TERROR;
			TAST(cp.cmd_param_decl(_T("-o"), 1, GINVALID, 0, GINVALID, _T("FileName"), GAIA::MISC::CmdParam::PARAM_TYPE_MUSTEXISTFILE));
			if(cp.cmd_decl(_T("-omax"), _T("Set output file's max size."), 1, 1, GAIA::MISC::CmdParam::CMD_TYPE_INVALID) == GINVALID)
				TERROR;
			TAST(cp.cmd_param_decl(_T("-omax"), 1, 1, 0, 0, _T("Max output file size."), GAIA::MISC::CmdParam::PARAM_TYPE_INT));
			if(cp.cmd_decl(_T("-s"), _T("Set output stream"), 1, 1, GAIA::MISC::CmdParam::CMD_TYPE_INVALID) == GINVALID)
				TERROR;
			TAST(cp.cmd_param_decl(_T("-s"), 1, 1, 0, GINVALID, _T("StreamName"), GAIA::MISC::CmdParam::PARAM_TYPE_STRING));
			cp.cmd_mutex_decl(_T("-d"), _T("-D"));
			cp.cmd_mutex_decl(_T("-o"), _T("-s"));
			cp.cmd_coop_decl(_T("-o"), _T("-omax"));
			cp.cmd_coop_decl(_T("-o"), _T("-omax"));
		}
		cp.end_decl();

		if(cp.finddecl(_T("-D"), 1) == GINVALID)
			TERROR;
		if(cp.finddecl(_T("-d"), 1) == GINVALID)
			TERROR;
		if(cp.finddecl(_T("-r"), 0) == GINVALID)
			TERROR;
		GAIA::STREAM::StringStream strstream;
		TAST(cp.build(_T("-D text.txt -r -m -o output.txt -omax 123"), strstream));
		for(GAIA::NUM x = 0; x < cp.cmd_size(); ++x)
		{
			const GAIA::TCH* pszCmd = cp.cmd(x);
			GAST(!GAIA::ALGO::gstremp(pszCmd));
			strstream << pszCmd << " ";
			for(GAIA::NUM y = 0; y < cp.param_size(x); ++y)
			{
				const GAIA::TCH* pszParam = cp.param(x, y);
				GAST(!GAIA::ALGO::gstremp(pszParam));
				strstream << pszParam << " ";
			}
			strstream << "\n";
		}
		const GAIA::TCH* pszStringStreamResult = strstream.getstring();
		GAIA::STREAM::STDStream stdstream;
		stdstream << pszStringStreamResult;
		if(!GAIA::ALGO::gstrequal(pszStringStreamResult, "-D text.txt \n-r \n-m \n-o output.txt \n-omax 123 \n"))
			TERROR;
		cp.clear_cmd();
		cp.clear_decl();
	}
}
