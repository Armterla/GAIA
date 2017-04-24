#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_msys_base.h>
#include <gaia_msys_heapcrt.h>
#include <gaia_msys_heapesg.h>
#include <gaia_digit_zip.h>
#include <gaia_type_impl.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#include <zlib.h>

namespace GAIA
{
	namespace DIGIT
	{
		GINL GAIA::GVOID* zlib_alloc_func(GAIA::GVOID* opaque, uInt items, uInt size)
		{
			return gnew GAIA::U8[items * size];
		}
		GINL GAIA::GVOID zlib_free_func(GAIA::GVOID* opaque, GAIA::GVOID* address)
		{
			gdel[](GAIA::U8*)address;
		}
		GAIA::NUM zip(GAIA::GVOID* pDst, const GAIA::NUM& dstsize, const GAIA::GVOID* pSrc, const GAIA::NUM& srcsize, GAIA::DIGIT::ZIP_MODE zmode)
		{
			GPCHR_NULL_RET(pDst, GINVALID);
			GPCHR_BELOWEQUALZERO_RET(dstsize, GINVALID);
			GPCHR_NULL_RET(pSrc, GINVALID);
			GPCHR_BELOWEQUALZERO_RET(srcsize, GINVALID);
			GAIA::N32 zipmode;
			switch(zmode)
			{
			case GAIA::DIGIT::ZIP_MODE_COMPRESS_DEFAULT:
				zipmode = Z_DEFAULT_COMPRESSION;
				break;
			case GAIA::DIGIT::ZIP_MODE_COMPRESS_BESTSPEED:
				zipmode = Z_BEST_SPEED;
				break;
			case GAIA::DIGIT::ZIP_MODE_COMPRESS_BESTSIZE:
				zipmode = Z_BEST_COMPRESSION;
				break;
			case GAIA::DIGIT::ZIP_MODE_COMPRESS_NOCOMPRESS:
				zipmode = Z_NO_COMPRESSION;
				break;
			default:
				return GINVALID;
			}
			uLongf zdstsize = (uLongf)dstsize;
			GAIA::N32 nRes = compress2((Bytef*)pDst, &zdstsize, (const Bytef*)pSrc, srcsize, zipmode);
			if(nRes == Z_OK)
				return (GAIA::NUM)zdstsize;
			return GINVALID;
		}
		GAIA::NUM unzip(GAIA::GVOID* pDst, const GAIA::NUM& dstsize, const GAIA::GVOID* pSrc, const GAIA::NUM& srcsize)
		{
			GPCHR_NULL_RET(pDst, GINVALID);
			GPCHR_BELOWEQUALZERO_RET(dstsize, GINVALID);
			GPCHR_NULL_RET(pSrc, GINVALID);
			GPCHR_BELOWEQUALZERO_RET(srcsize, GINVALID);
			uLongf zdstsize = (uLongf)dstsize;
			GAIA::N32 nRes = uncompress((Bytef*)pDst, &zdstsize, (const Bytef*)pSrc, srcsize);
			if(nRes == Z_OK)
				return (GAIA::NUM)zdstsize;
			return GINVALID;
		}
		GAIA::BL Zip::empty() const
		{
			return this->write_size() == 0;
		}
		GAIA::N64 Zip::write_size() const
		{
			if(!this->isbegin())
				return 0;
			z_stream* z = (z_stream*)m_pStream;
			return z->total_in;
		}
		GAIA::N64 Zip::read_size() const
		{
			if(!this->isbegin())
				return 0;
			z_stream* z = (z_stream*)m_pStream;
			return z->total_out - m_BufferRW.size();
		}
		GAIA::N64 Zip::remain() const
		{
			if(!this->isbegin())
				return 0;
			return m_BufferRW.size();
		}
		GAIA::N64 Zip::total() const
		{
			if(!this->isbegin())
				return 0;
			z_stream* z = (z_stream*)m_pStream;
			return z->total_out;
		}
		GAIA::BL Zip::begin(GAIA::DIGIT::ZIP_MODE zmode)
		{
			if(this->isbegin())
				return GAIA::False;
			if(zmode == GAIA::DIGIT::ZIP_MODE_INVALID || zmode >= GAIA::DIGIT::ZIP_MODE_MAXENUMCOUNT)
				return GAIA::False;

		#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
			z_stream* pStream = (z_stream*)g_gaia_globalmsys.memory_alloc(sizeof(z_stream));
		#else
			z_stream* pStream = (z_stream*)malloc(sizeof(z_stream));
		#endif
			pStream->zalloc = zlib_alloc_func;
			pStream->zfree = zlib_free_func;
			pStream->opaque = GNIL;

			if(zmode == GAIA::DIGIT::ZIP_MODE_UNCOMPRESS)
			{
				if(inflateInit(pStream) != Z_OK)
				{
				#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
					g_gaia_globalmsys.memory_release(pStream);
				#else
					free(pStream);
				#endif
					return GAIA::False;
				}
			}
			else
			{
				GAIA::N32 zipmode;
				switch(zmode)
				{
				case GAIA::DIGIT::ZIP_MODE_COMPRESS_DEFAULT:
					zipmode = Z_DEFAULT_COMPRESSION;
					break;
				case GAIA::DIGIT::ZIP_MODE_COMPRESS_BESTSPEED:
					zipmode = Z_BEST_SPEED;
					break;
				case GAIA::DIGIT::ZIP_MODE_COMPRESS_BESTSIZE:
					zipmode = Z_BEST_COMPRESSION;
					break;
				case GAIA::DIGIT::ZIP_MODE_COMPRESS_NOCOMPRESS:
					zipmode = Z_NO_COMPRESSION;
					break;
				default:
					zipmode = Z_DEFAULT_COMPRESSION;
					break;
				}

				if(deflateInit(pStream, zipmode) != Z_OK)
				{
				#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
					g_gaia_globalmsys.memory_release(pStream);
				#else
					free(pStream);
				#endif
					return GAIA::False;
				}
			}

			m_pStream = pStream;
			m_zmode = zmode;
			return GAIA::True;
		}
		GAIA::BL Zip::end()
		{
			if(!this->isbegin())
				return GAIA::False;

			z_stream* z = (z_stream*)m_pStream;
			if(m_zmode == GAIA::DIGIT::ZIP_MODE_UNCOMPRESS)
			{
				if(inflateEnd((z_stream*)m_pStream) != Z_OK)
					return GAIA::False;
			}
			else
			{
				if(deflate(z, Z_FINISH) != Z_STREAM_END)
					return GAIA::False;
				if(deflateEnd((z_stream*)m_pStream) != Z_OK)
					return GAIA::False;
			}

		#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
			g_gaia_globalmsys.memory_release(m_pStream);
		#else
			free(m_pStream);
		#endif
			m_zmode = GAIA::DIGIT::ZIP_MODE_INVALID;
			m_pStream = GNIL;
			m_BufferRW.destroy();
			return GAIA::True;
		}
		GAIA::BL Zip::isbegin() const
		{
			return m_pStream != GNIL;
		}
		GAIA::DIGIT::ZIP_MODE Zip::mode() const
		{
			return m_zmode;
		}
		GAIA::U32 Zip::write(const GAIA::GVOID* p, GAIA::U32 size)
		{
			GPCHR_NULL_RET(p, GINVALID);
			GPCHR_BELOWEQUALZERO_RET(size, GINVALID);
			if(!this->isbegin())
				return GINVALID;

			z_stream* z = (z_stream*)m_pStream;
			z->next_in = (Bytef*)p;
			z->avail_in = size;

			for(; z->avail_in > 0; )
			{
				z->next_out = m_pBuffer;
				z->avail_out = GAIA::CTN::BufferRW::_blocksize;
				GAIA::N32 nResult;
				if(m_zmode == GAIA::DIGIT::ZIP_MODE_UNCOMPRESS)
					nResult = inflate(z, Z_NO_FLUSH);
				else
					nResult = deflate(z, Z_NO_FLUSH);
				if(nResult != Z_OK)
					break;
				if(GAIA::CTN::BufferRW::_blocksize - z->avail_out > 0)
					m_BufferRW.write(m_pBuffer, GAIA::CTN::BufferRW::_blocksize - z->avail_out);
			}
			return size - z->avail_in;
		}
		GAIA::U32 Zip::read(GAIA::GVOID* p, GAIA::U32 size)
		{
			GPCHR_NULL_RET(p, GINVALID);
			GPCHR_BELOWEQUALZERO_RET(size, GINVALID);
			if(!this->isbegin())
				return GINVALID;
			return m_BufferRW.read(p, size);
		}
		GAIA::BL Zip::flush()
		{
			if(!this->isbegin())
				return GAIA::False;

			z_stream* z = (z_stream*)m_pStream;
			z->next_in = GNIL;
			z->avail_in = 0;
			for(;;)
			{
				z->next_out = m_pBuffer;
				z->avail_out = GAIA::CTN::BufferRW::_blocksize;
				GAIA::N32 nResult;
				if(m_zmode == GAIA::DIGIT::ZIP_MODE_UNCOMPRESS)
					nResult = inflate(z, Z_SYNC_FLUSH);
				else
					nResult = deflate(z, Z_SYNC_FLUSH);
				if(nResult == Z_BUF_ERROR)
					return GAIA::True;
				if(nResult == Z_STREAM_END)
					return GAIA::True;
				else if(nResult != Z_OK)
					return GAIA::False;
				if(GAIA::CTN::BufferRW::_blocksize - z->avail_out > 0)
					m_BufferRW.write(m_pBuffer, GAIA::CTN::BufferRW::_blocksize - z->avail_out);
			}
		}
	}	
}
