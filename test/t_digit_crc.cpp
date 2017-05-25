#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_crc(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 10000;
		
		// Fill source buffer.
		GAIA::CTN::Vector<GAIA::NUM> datas;
		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
			datas.push_back(x);
		
		// CRC32 test.
		GAIA::DIGIT::CRC32 crc32;
		if(crc32.check())
			TERROR;
		if(!crc32.update(datas.fptr(), datas.datasize()))
			TERROR;
		if(!crc32.check())
			TERROR;
		GAIA::U32 uCRCRes32 = crc32.result();
		crc32.reset();
		if(crc32.check())
			TERROR;
		for(GAIA::NUM x = 0; x < datas.size(); ++x)
		{
			if(!crc32.update(datas[x]))
				TERROR;
		}
		if(!crc32.check())
			TERROR;
		GAIA::U32 uCRCRes32ForDataType = crc32.result();
		TAST(uCRCRes32 == uCRCRes32ForDataType);
	}
}
