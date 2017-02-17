#ifndef 	__GAIA_DIGIT_ZIP_H__
#define 	__GAIA_DIGIT_ZIP_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_bufferrw.h"

namespace GAIA
{
	namespace DIGIT
	{
		GAIA_ENUM_BEGIN(ZIP_MODE)
			ZIP_MODE_COMPRESS_DEFAULT,
			ZIP_MODE_COMPRESS_BESTSPEED,
			ZIP_MODE_COMPRESS_BESTSIZE,
			ZIP_MODE_COMPRESS_NOCOMPRESS,
			ZIP_MODE_UNCOMPRESS,
		GAIA_ENUM_END(ZIP_MODE)

		GINL GAIA::NUM max_zip_result_size(const GAIA::NUM& srcsize){return (GAIA::NUM)((GAIA::F32)srcsize * 1.1F + 32.0F);}
		GAIA::NUM zip(GAIA::GVOID* pDst, const GAIA::NUM& dstsize, const GAIA::GVOID* pSrc, const GAIA::NUM& srcsize, GAIA::DIGIT::ZIP_MODE zmode);
		GAIA::NUM unzip(GAIA::GVOID* pDst, const GAIA::NUM& dstsize, const GAIA::GVOID* pSrc, const GAIA::NUM& srcsize);
		
		class Zip : public GAIA::Base
		{
		public:
			Zip(){this->init();}
			~Zip(){if(this->isbegin()) this->end(); gdel m_pBuffer;}
			
			GAIA::BL empty() const;
			GAIA::N64 write_size() const;
			GAIA::N64 read_size() const;
			GAIA::N64 remain() const;
			GAIA::N64 total() const;
			
			GAIA::BL begin(GAIA::DIGIT::ZIP_MODE zmode);
			GAIA::BL end();
			GAIA::BL isbegin() const;
			GAIA::DIGIT::ZIP_MODE mode() const;
			
			GAIA::U32 write(const GAIA::GVOID* p, GAIA::U32 size);
			GAIA::U32 read(GAIA::GVOID* p, GAIA::U32 size);
			
			template<typename _ParamDataType> GAIA::BL write(const _ParamDataType& t){return this->write(&t, sizeof(t)) == sizeof(t);}
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType& t){return this->read(&t, sizeof(t)) == sizeof(t);}

			GAIA::BL flush();
			
		private:
			GAIA::GVOID init()
			{
				m_zmode = GAIA::DIGIT::ZIP_MODE_INVALID;
				m_pStream = GNIL;
				m_pBuffer = gnew GAIA::U8[GAIA::CTN::BufferRW::_blocksize];
			}
			
		private:
			GAIA::DIGIT::ZIP_MODE m_zmode;
			GAIA::GVOID* m_pStream;
			GAIA::CTN::BufferRW m_BufferRW;
			GAIA::U8* m_pBuffer;
		};
	}	
}

#endif
