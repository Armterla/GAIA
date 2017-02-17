#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_staticbufferpool(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::StaticBufferPool __StaticBufferPoolType;
		__StaticBufferPoolType bp;
		if(!bp.empty())
			TERROR;
		if(bp.size() != 0)
			TERROR;
		if(bp.capacity() != 0)
			TERROR;
		if(bp.buffer_size() != 0)
			TERROR;
		bp.clear();
		if(!bp.empty())
			TERROR;
		if(bp.size() != 0)
			TERROR;
		if(bp.capacity() != 0)
			TERROR;
		if(bp.buffer_size() != 0)
			TERROR;
		bp.destroy();
		if(!bp.empty())
			TERROR;
		if(bp.size() != 0)
			TERROR;
		if(bp.capacity() != 0)
			TERROR;
		if(bp.buffer_size() != 0)
			TERROR;
		bp.reserve(10);
		if(!bp.empty())
			TERROR;
		if(bp.size() != 0)
			TERROR;
		if(bp.capacity() != 10)
			TERROR;
		if(bp.buffer_size() != 0)
			TERROR;
		const GAIA::U8* pBuf = bp.alloc("HelloWorld", sizeof("HelloWorld"));
		if(pBuf == GNIL)
			TERROR;
		if(GAIA::ALGO::gstrcmp(pBuf, "HelloWorld") != 0)
			TERROR;
		if(bp.empty())
			TERROR;
		if(bp.size() != 1)
			TERROR;
		if(bp.capacity() != 10)
			TERROR;
		if(bp.buffer_size() != 1)
			TERROR;
		const GAIA::U8* pBuf1 = bp.alloc("HelloWorld", sizeof("HelloWorld"));
		if(pBuf1 == GNIL)
			TERROR;
		if(pBuf1 != pBuf)
			TERROR;
		if(bp.empty())
			TERROR;
		if(bp.size() != 1)
			TERROR;
		if(bp.capacity() != 10)
			TERROR;
		if(bp.buffer_size() != 1)
			TERROR;
		const GAIA::U8* pBufT = bp.request("HelloWorld", sizeof("HelloWorld"));
		if(pBufT != pBuf)
			TERROR;
		pBufT = bp.request("HelloKitty", sizeof("HelloKitty"));
		if(pBufT != GNIL)
			TERROR;
		const GAIA::U8* pBuf2 = bp.alloc("HelloKitty", sizeof("HelloKitty"));
		const GAIA::U8* pBuf3 = bp.alloc("HelloKitty", sizeof("HelloKitty"));
		if(pBuf2 == GNIL)
			TERROR;
		if(GAIA::ALGO::gstrcmp(pBuf2, "HelloKitty") != 0)
			TERROR;
		if(pBuf3 == GNIL)
			TERROR;
		if(pBuf3 != pBuf2)
			TERROR;
		if(bp.size() != 2)
			TERROR;
		if(bp.capacity() != 10)
			TERROR;
		if(bp.buffer_size() != 2)
			TERROR;
		for(GAIA::NUM x = 0; x < bp.buffer_size(); ++x)
		{
			GAIA::NUM sSize;
			const GAIA::U8* pBufTemp = bp.get(x, sSize);
			if(pBufTemp == GNIL)
			{
				TERROR;
				break;
			}
			if(GAIA::ALGO::gstrcmp(pBufTemp, "HelloWorld") != 0 &&
			   GAIA::ALGO::gstrcmp(pBufTemp, "HelloKitty") != 0)
			{
				TERROR;
				break;
			}
		}
		__StaticBufferPoolType bp1 = bp;
		if(bp1.size() != 2)
			TERROR;
		if(bp1.buffer_size() != 2)
			TERROR;
	}
}
