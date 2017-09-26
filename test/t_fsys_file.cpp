#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_fsys_file(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::FSYS::File __FileType;
		typedef GAIA::FSYS::Dir __DirType;

		GAIA::CH TEST_FILE_NAME[GAIA::MAXPL];
		if(GAIA::ALGO::gstremp(g_gaia_appdocdir))
			GAIA::ALGO::gstrcpy(TEST_FILE_NAME, "../testres/filetest.txt");
		else
		{
			GAIA::ALGO::gstrcpy(TEST_FILE_NAME, g_gaia_appdocdir);
			GAIA::ALGO::gstrcat(TEST_FILE_NAME, "filetest.txt");
		}

		__DirType dir;
		__FileType tfile;
		if(!GAIA::ALGO::gstremp(tfile.GetFileUrl()))
			TERROR;
		if(tfile.GetOpenType() != GAIA::FSYS::FileBase::OPEN_TYPE_INVALID)
			TERROR;
		TAST(tfile.Open(TEST_FILE_NAME, __FileType::OPEN_TYPE_WRITE | __FileType::OPEN_TYPE_CREATEALWAYS));
		if(GAIA::ALGO::gstrcmp(tfile.GetFileUrl(), TEST_FILE_NAME) != 0)
			TERROR;
		if(tfile.GetOpenType() != (GAIA::FSYS::FileBase::OPEN_TYPE_WRITE | GAIA::FSYS::FileBase::OPEN_TYPE_CREATEALWAYS))
			TERROR;
		TAST(tfile.IsOpen());
		if(tfile.Size() != 0)
			TERROR;
		TAST(dir.ExistFile(TEST_FILE_NAME));
		TAST(tfile.Close());
		TAST(dir.ExistFile(TEST_FILE_NAME));
		TAST(tfile.Open(TEST_FILE_NAME, __FileType::OPEN_TYPE_READ));
		TAST(tfile.IsOpen());
		if(tfile.Size() != 0)
			TERROR;
		TAST(dir.ExistFile(TEST_FILE_NAME));
		TAST(tfile.Close());
		TAST(dir.ExistFile(TEST_FILE_NAME));
		TAST(tfile.Open(TEST_FILE_NAME, __FileType::OPEN_TYPE_WRITE | __FileType::OPEN_TYPE_CREATEALWAYS));
		TAST(tfile.IsOpen());
		if(tfile.Write("Hello World", GAIA::ALGO::gstrlen("Hello World")) != GAIA::ALGO::gstrlen("Hello World"))
			TERROR;
		if(tfile.Size() != GAIA::ALGO::gstrlen("Hello World"))
			TERROR;
		GAIA::U32 uData = 123456789;
		TAST(tfile.WriteObj(uData));
		if(tfile.Size() != (GAIA::FSYS::File::__FileSizeType)(GAIA::ALGO::gstrlen("Hello World") + sizeof(uData)))
			TERROR;
		TAST(tfile.WriteText("Hello Kitty"));
		if(tfile.Size() != (GAIA::FSYS::File::__FileSizeType)(GAIA::ALGO::gstrlen("Hello World") + sizeof(uData) + GAIA::ALGO::gstrlen("Hello Kitty")))
			TERROR;
		TAST(tfile.Close());
		TAST(tfile.Open(TEST_FILE_NAME, __FileType::OPEN_TYPE_READ));
		TAST(tfile.IsOpen());
		if(tfile.Size() != (GAIA::FSYS::File::__FileSizeType)(GAIA::ALGO::gstrlen("Hello World") + sizeof(uData) + GAIA::ALGO::gstrlen("Hello Kitty")))
			TERROR;
		GAIA::CH szText[128];
		if(tfile.Read(szText, GAIA::ALGO::gstrlen("Hello World")) != GAIA::ALGO::gstrlen("Hello World"))
			TERROR;
		if(GAIA::ALGO::gstrcmp(szText, "Hello World", GAIA::ALGO::gstrlen("Hello World")) != 0)
			TERROR;
		uData = 0;
		TAST(tfile.ReadObj(uData));
		if(uData != 123456789)
			TERROR;
		if(tfile.Read(szText, GAIA::ALGO::gstrlen("Hello Kitty")) != GAIA::ALGO::gstrlen("Hello Kitty"))
			TERROR;
		if(GAIA::ALGO::gstrcmp(szText, "Hello Kitty", GAIA::ALGO::gstrlen("Hello Kitty")) != 0)
			TERROR;
		TAST(tfile.Close());
		if(!GAIA::ALGO::gstremp(tfile.GetFileUrl()))
			TERROR;
		if(tfile.GetOpenType() != GAIA::FSYS::FileBase::OPEN_TYPE_INVALID)
			TERROR;
		TAST(tfile.Open(TEST_FILE_NAME, __FileType::OPEN_TYPE_READ));
		TAST(tfile.IsOpen());
		GAIA::CTN::Buffer buf;
		TAST(tfile.ReadRemainAll(buf));
		if(buf.write_size() != GAIA::ALGO::gstrlen("Hello World") + sizeof(GAIA::U32) + GAIA::ALGO::gstrlen("Hello Kitty"))
			TERROR;
		if(GAIA::ALGO::gstrcmp(buf.fptr(), "Hello World", GAIA::ALGO::gstrlen("Hello World")) != 0)
			TERROR;
		if(*(GAIA::U32*)(buf.fptr() + GAIA::ALGO::gstrlen("Hello World")) != uData)
			TERROR;
		if(GAIA::ALGO::gstrcmp(buf.fptr() + GAIA::ALGO::gstrlen("Hello World") + sizeof(GAIA::U32), "Hello Kitty", GAIA::ALGO::gstrlen("Hello Kitty")) != 0)
			TERROR;
		TAST(tfile.Close());
		TAST(dir.RemoveFile(TEST_FILE_NAME));
	}
}
