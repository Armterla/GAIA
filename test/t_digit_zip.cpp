#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_zip(GAIA::LOG::Log& logobj)
	{
		// Simple function test.
		GAIA::CTN::Vector<GAIA::NUM> srcdata;
		for(GAIA::NUM x = 0; x < 10000; ++x)
			srcdata.push_back(x);
		GAIA::CTN::Buffer dstbuf;
		dstbuf.resize(srcdata.size() * sizeof(GAIA::NUM) * 2);
		GAIA::NUM dstsize = GAIA::DIGIT::zip(
			dstbuf.fptr(), dstbuf.write_size(), 
			srcdata.fptr(), (GAIA::NUM)(srcdata.size() * sizeof(GAIA::NUM)), 
			GAIA::DIGIT::ZIP_MODE_COMPRESS_BESTSPEED);
		if(dstsize <= 0)
			TERROR;
		dstbuf.resize(dstsize);
		srcdata.reset(0);
		GAIA::NUM srcsize = GAIA::DIGIT::unzip(srcdata.fptr(), (GAIA::NUM)(srcdata.size() * sizeof(GAIA::NUM)), dstbuf.fptr(), dstbuf.write_size());
		if(srcsize != (GAIA::NUM)(srcdata.size() * sizeof(GAIA::NUM)))
			TERROR;
		for(GAIA::NUM x = 0; x < srcdata.size(); ++x)
		{
			if(srcdata[x] != x)
			{
				TERROR;
				break;
			}
		}

		// Write only test.
		GAIA::DIGIT::Zip zip;
		if(zip.isbegin())
			TERROR;
		TAST(zip.begin(GAIA::DIGIT::ZIP_MODE_COMPRESS_DEFAULT));
		TAST(zip.empty());
		if(zip.mode() != GAIA::DIGIT::ZIP_MODE_COMPRESS_DEFAULT)
			TERROR;
		if(zip.read_size() != 0)
			TERROR;
		if(zip.write_size() != 0)
			TERROR;
		if(zip.remain() != 0)
			TERROR;
		if(zip.total() != 0)
			TERROR;
		TAST(zip.isbegin());
		for(int x = 0; x < srcdata.size(); ++x)
		{
			if(!zip.write(srcdata[x]))
			{
				TERROR;
				break;
			}
		}
		TAST(zip.flush());
		if(zip.write_size() != srcdata.size() * sizeof(GAIA::NUM))
			TERROR;
		if(zip.read_size() != 0)
			TERROR;
		TAST(zip.end());

		// Compress test.
		TAST(zip.begin(GAIA::DIGIT::ZIP_MODE_COMPRESS_DEFAULT));
		TAST(zip.empty());
		if(zip.mode() != GAIA::DIGIT::ZIP_MODE_COMPRESS_DEFAULT)
			TERROR;
		for(int x = 0; x < srcdata.size(); ++x)
		{
			if(!zip.write(srcdata[x]))
			{
				TERROR;
				break;
			}
		}
		TAST(zip.flush());
		GAIA::N32 nRemain = GSCAST(GAIA::N32)(zip.remain());
		if(nRemain <= 0)
			TERROR;
		GAIA::U8* pBuf = gnew GAIA::U8[nRemain];
		if(zip.read(pBuf, nRemain) != nRemain)
			TERROR;
		if(zip.write_size() != srcdata.size() * sizeof(GAIA::NUM))
			TERROR;
		if(zip.read_size() != zip.total())
			TERROR;
		if(zip.remain() != 0)
			TERROR;
		TAST(zip.end());

		// Uncompress test.
		TAST(zip.begin(GAIA::DIGIT::ZIP_MODE_UNCOMPRESS));
		TAST(zip.empty());
		if(zip.mode() != GAIA::DIGIT::ZIP_MODE_UNCOMPRESS)
			TERROR;
		if(zip.write(pBuf, nRemain) != nRemain)
			TERROR;
		TAST(zip.flush());
		if(zip.remain() != srcdata.size() * sizeof(GAIA::NUM))
			TERROR;
		for(GAIA::NUM x = 0; x < srcdata.size(); ++x)
		{
			GAIA::NUM t;
			if(!zip.read(t))
			{
				TERROR;
				break;
			}
			if(t != srcdata[x])
			{
				TERROR;
				break;
			}
		}
		if(zip.remain() != 0)
			TERROR;
		if(zip.read_size() != zip.total())
			TERROR;
		if(zip.read_size() != srcdata.size() * sizeof(GAIA::NUM))
			TERROR;
		TAST(zip.end());

		// No flush compress test.
		TAST(zip.begin(GAIA::DIGIT::ZIP_MODE_COMPRESS_DEFAULT));
		TAST(zip.empty());
		if(zip.mode() != GAIA::DIGIT::ZIP_MODE_COMPRESS_DEFAULT)
			TERROR;
		if(zip.write(pBuf, nRemain) != nRemain)
			TERROR;
		TAST(zip.end());

		// No flush uncompress test.
		TAST(zip.begin(GAIA::DIGIT::ZIP_MODE_UNCOMPRESS));
		TAST(zip.empty());
		if(zip.mode() != GAIA::DIGIT::ZIP_MODE_UNCOMPRESS)
			TERROR;
		if(zip.write(pBuf, nRemain) != nRemain)
			TERROR;
		TAST(zip.end());

		GAIA_DELETEARRAY_SAFE(pBuf);
	}
}
