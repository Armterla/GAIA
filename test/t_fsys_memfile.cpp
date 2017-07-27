#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_fsys_memfile(GAIA::LOG::Log& logobj)
	{
	    GAIA::FSYS::MemFile fmem;
	    GAIA::FSYS::FileBase& f = fmem;
	    if(f.IsOpen())
	        TERROR;
	    if(f.GetOpenType() != GAIA::FSYS::FileBase::OPEN_TYPE_INVALID)
	        TERROR;
	    if(!GAIA::ALGO::gstremp(f.GetFileUrl()))
	        TERROR;
	    if(!f.Open("MemFile", GAIA::FSYS::FileBase::OPEN_TYPE_WRITE | GAIA::FSYS::FileBase::OPEN_TYPE_READ))
	        TERROR;
	    if(f.GetOpenType() != (GAIA::FSYS::FileBase::OPEN_TYPE_WRITE | GAIA::FSYS::FileBase::OPEN_TYPE_READ))
	        TERROR;
	    if(GAIA::ALGO::gstremp(f.GetFileUrl()))
	        TERROR;
	    if(!f.IsOpen())
	       TERROR;
	    if(f.Tell() != 0)
	        TERROR;
	    for(GAIA::NUM x = 0; x < 100; ++x)
	    {
	        if(!f.WriteObj(x))
	        {
	            TERROR;
	            break;
	        }
	    }
	    if(!f.Seek(0))
	        TERROR;
	    if(f.Tell() != 0)
	        TERROR;
	    if(f.Size() != 100 * sizeof(GAIA::NUM))
	        TERROR;
	    for(GAIA::NUM x = 0; x < 100; ++x)
	    {
	        GAIA::NUM v;
	        if(!f.ReadObj(v))
	        {
	            TERROR;
	            break;
	        }
	    }
	    if(!f.Seek(0))
	        TERROR;
	    for(GAIA::NUM x = 0; x < 100; ++x)
	    {
	        if(!f.WriteObj(x * 10))
	        {
	            TERROR;
	            break;
	        }
	    }
	    if(!f.Seek(0))
	        TERROR;
	    for(GAIA::NUM x = 0; x < 100; ++x)
	    {
	        GAIA::NUM v;
	        if(!f.ReadObj(v))
	        {
	            TERROR;
	            break;
	        }
	    }
	    if(f.Tell() != 100 * sizeof(GAIA::NUM))
	        TERROR;
	    if(f.Size() != 100 * sizeof(GAIA::NUM))
	        TERROR;
	    GAIA::FSYS::MemFile::_BufferType buf = fmem.GetBuffer();
	    if(buf.write_size() != 100 * sizeof(GAIA::NUM))
	        TERROR;
	    if(!f.Close())
	        TERROR;
	    if(f.GetOpenType() != GAIA::FSYS::FileBase::OPEN_TYPE_INVALID)
	        TERROR;
	    if(!GAIA::ALGO::gstremp(f.GetFileUrl()))
	        TERROR;
	}
}
