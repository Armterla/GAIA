#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_fsys_dir(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::FSYS::File __FileType;
		typedef GAIA::FSYS::Dir __DirType;

		__FileType tfile;
		__DirType dir;

		GAIA::CH TEST_DIR[GAIA::MAXPL];
		GAIA::CH TEST_DIR_CHILD[GAIA::MAXPL];
		GAIA::CH TEST_DIR_CHILD_CHILD[GAIA::MAXPL];
		if(GAIA::ALGO::gstremp(g_gaia_appdocdir))
		{
			GAIA::ALGO::gstrcpy(TEST_DIR, "../testres/TestDir/");
			GAIA::ALGO::gstrcpy(TEST_DIR_CHILD, "../testres/TestDir/ChildDir/");
			GAIA::ALGO::gstrcpy(TEST_DIR_CHILD_CHILD, "../testres/TestDir/ChildDir/ChildChildDir/");
		}
		else
		{
			GAIA::ALGO::gstrcpy(TEST_DIR, g_gaia_appdocdir);
			GAIA::ALGO::gstrcat(TEST_DIR, "TestDir/");
			
			GAIA::ALGO::gstrcpy(TEST_DIR_CHILD, g_gaia_appdocdir);
			GAIA::ALGO::gstrcat(TEST_DIR_CHILD, "TestDir/ChildDir/");
			
			GAIA::ALGO::gstrcpy(TEST_DIR_CHILD_CHILD, g_gaia_appdocdir);
			GAIA::ALGO::gstrcat(TEST_DIR_CHILD_CHILD, "TestDir/ChildDir/ChildChildDir/");
		}

		TAST(dir.Create(TEST_DIR, GAIA::False));
		TAST(dir.Exist(TEST_DIR));
		TAST(dir.Remove(TEST_DIR, GAIA::False));
		if(dir.Exist(TEST_DIR))
			TERROR;
		TAST(dir.Create(TEST_DIR_CHILD_CHILD, GAIA::True));
		TAST(dir.Exist(TEST_DIR_CHILD_CHILD));
		TAST(dir.Exist(TEST_DIR_CHILD));
		TAST(dir.Exist(TEST_DIR));
		TAST(dir.Remove(TEST_DIR_CHILD_CHILD, GAIA::False));
		if(dir.Exist(TEST_DIR_CHILD_CHILD))
			TERROR;
		TAST(dir.Remove(TEST_DIR_CHILD, GAIA::False));
		if(dir.Exist(TEST_DIR_CHILD))
			TERROR;
		TAST(dir.Remove(TEST_DIR, GAIA::False));
		if(dir.Exist(TEST_DIR))
			TERROR;
		TAST(dir.Create(TEST_DIR_CHILD_CHILD, GAIA::True));
		TAST(dir.Remove(TEST_DIR, GAIA::True));
		if(dir.Exist(TEST_DIR))
			TERROR;
	}
}
