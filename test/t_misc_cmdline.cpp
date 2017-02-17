#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_misc_cmdline(GAIA::LOG::Log& logobj)
	{
		GAIA::MISC::CmdLine cmdline;
		cmdline.decl("cmd1", "cmd1desc");
		cmdline.decl_param("p1", "p1desc");
		cmdline.decl_param("p2", "p2desc");
		cmdline.decl("cmd2", "cmd2desc");
		cmdline.decl_param("p3", "p3desc");
		cmdline.decl_param("p4", "p4desc");
		cmdline.decl("cmd3", "cmd3desc");
		cmdline.decl_param("p5", "p5desc");
		cmdline.decl_param("p6", "p6desc");
		cmdline.decl("cmd4", "cmd4desc");
		cmdline.decl_param("p7", "p7desc");
		cmdline.decl_param("p8", "p8desc");
		cmdline.decl("cmd5", "cmd5desc");
		cmdline.decl_param("p9", "p9desc");
		cmdline.decl_param("p10", "p10desc");

		GAIA::STREAM::StringStream strstream;
		strstream << "cmd1 p1 p2;cmd2 p3 p4; cmd3  p5    p6;cmd4 p7 p8 ;cmd5 p9 p10";
		GAIA::CTN::Queue<GAIA::MISC::CmdLine::__StringType> que;
		que.push_back("cmd1");
		que.push_back("p1");
		que.push_back("p2");
		que.push_back("cmd2");
		que.push_back("p3");
		que.push_back("p4");
		que.push_back("cmd3");
		que.push_back("p5");
		que.push_back("p6");
		que.push_back("cmd4");
		que.push_back("p7");
		que.push_back("p8");
		while(cmdline.exec(strstream))
		{
			for(;;)
			{
				GAIA::MISC::CmdLine::__StringType str = cmdline.pop();
				if(str.empty())
					break;
				if(str != que.front())
				{
					TERROR;
					break;
				}
				que.pop_front();
			}
			if(que.empty())
				break;
		}
	}
}
