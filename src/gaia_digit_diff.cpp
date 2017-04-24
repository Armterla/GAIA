#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_msys_base.h>
#include <gaia_msys_heapcrt.h>
#include <gaia_msys_heapesg.h>
#include <gaia_digit_diff.h>
#include <gaia_type_impl.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#if GAIA_MACHINE == GAIA_MACHINE32
#	define SIZEOF_SIZE_T 4
#else
#	define SIZEOF_SIZE_T 8
#endif
#include <zlib.h>
#include <xdelta3.h>
#include <xdelta3_impl.h>
#undef inline

namespace GAIA
{
	namespace DIGIT
	{
		GINL GAIA::GVOID* xd3_alloc_func(GAIA::GVOID* opaque, size_t items, usize_t size)
		{
			return gnew GAIA::U8[items * size];
		}
		GINL GAIA::GVOID xd3_free_func(GAIA::GVOID* opaque, GAIA::GVOID* address)
		{
			gdel[](GAIA::U8*)address;
		}
		GAIA::N64 diff(GAIA::GVOID* pDiff, const GAIA::N64& diffsize,
			const GAIA::GVOID* pSrc, const GAIA::N64& srcsize,
			const GAIA::GVOID* pDst, const GAIA::N64& dstsize, GAIA::DIGIT::DIFF_MODE diffmode)
		{
			GAIA::N32 nFlag = 0;
			switch(diffmode)
			{
			case GAIA::DIGIT::DIFF_MODE_DEFAULT:
				break;
			case GAIA::DIGIT::DIFF_MODE_BESTSPEED:
				nFlag = XD3_COMPLEVEL_1 & XD3_COMPLEVEL_MASK;
				break;
			case GAIA::DIGIT::DIFF_MODE_BESTSIZE:
				nFlag = XD3_COMPLEVEL_9 & XD3_COMPLEVEL_MASK;
				break;
			case GAIA::DIGIT::DIFF_MODE_NOCOMPRESS:
				nFlag = XD3_NOCOMPRESS;
				break;
			default:
				return GINVALID;
			}
			usize_t ret;
			if(xd3_encode_memory(
				(GAIA::U8*)pDst, (GAIA::U32)dstsize,
				(GAIA::U8*)pSrc, (GAIA::U32)srcsize,
				(GAIA::U8*)pDiff, &ret, (GAIA::U32)diffsize,
				nFlag) != 0)
				return GINVALID;
			return (GAIA::N64)ret;
		}
		GAIA::N64 undiff(GAIA::GVOID* pDst, const GAIA::N64& dstsize,
			const GAIA::GVOID* pSrc, const GAIA::N64& srcsize,
			const GAIA::GVOID* pDiff, const GAIA::N64& diffsize, GAIA::DIGIT::DIFF_MODE diffmode)
		{
			GAIA::N32 nFlag = 0;
			switch(diffmode)
			{
			case GAIA::DIGIT::DIFF_MODE_DEFAULT:
				break;
			case GAIA::DIGIT::DIFF_MODE_BESTSPEED:
				nFlag = XD3_COMPLEVEL_1 & XD3_COMPLEVEL_MASK;
				break;
			case GAIA::DIGIT::DIFF_MODE_BESTSIZE:
				nFlag = XD3_COMPLEVEL_9 & XD3_COMPLEVEL_MASK;
				break;
			case GAIA::DIGIT::DIFF_MODE_NOCOMPRESS:
				nFlag = XD3_NOCOMPRESS;
				break;
			default:
				return GINVALID;
			}
			usize_t ret;
			if(xd3_decode_memory(
				(GAIA::U8*)pDiff, (GAIA::N32)diffsize,
				(GAIA::U8*)pSrc, (GAIA::N32)srcsize,
				(GAIA::U8*)pDst, &ret, (GAIA::N32)dstsize,
				nFlag) != 0)
				return GINVALID;
			return (GAIA::N64)ret;
		}
		GAIA::BL Diff::empty() const
		{
			return this->write_size() == 0;
		}
		GAIA::N64 Diff::write_size() const
		{
			if(!this->isbegin())
				return 0;
			xd3_stream* d = (xd3_stream*)m_pStream;
			return d->total_in;
		}
		GAIA::N64 Diff::read_size() const
		{
			if(!this->isbegin())
				return 0;
			xd3_stream* d = (xd3_stream*)m_pStream;
			return d->total_out - m_BufferRW.size();
		}
		GAIA::N64 Diff::remain() const
		{
			if(!this->isbegin())
				return 0;
			return m_BufferRW.size();
		}
		GAIA::N64 Diff::total() const
		{
			if(!this->isbegin())
				return 0;
			xd3_stream* d = (xd3_stream*)m_pStream;
			return d->total_out;
		}
		GAIA::BL Diff::begin(GAIA::DIGIT::DIFF_MODE diffmode)
		{
			if(this->isbegin())
				return GAIA::False;
			if(diffmode == GAIA::DIGIT::DIFF_MODE_INVALID || diffmode >= GAIA::DIGIT::DIFF_MODE_MAXENUMCOUNT)
				return GAIA::False;

			GAIA::N32 nFlag = 0;
			switch(diffmode)
			{
			case GAIA::DIGIT::DIFF_MODE_DEFAULT:
				break;
			case GAIA::DIGIT::DIFF_MODE_BESTSPEED:
				nFlag = XD3_COMPLEVEL_1 & XD3_COMPLEVEL_MASK;
				break;
			case GAIA::DIGIT::DIFF_MODE_BESTSIZE:
				nFlag = XD3_COMPLEVEL_9 & XD3_COMPLEVEL_MASK;
				break;
			case GAIA::DIGIT::DIFF_MODE_NOCOMPRESS:
				nFlag = XD3_NOCOMPRESS;
				break;
			case GAIA::DIGIT::DIFF_MODE_UNDIFF:
				break;
			default:
				return GAIA::False;
			}

			//
			xd3_config cfg;
			zeromem(&cfg);
			xd3_init_config(&cfg, nFlag);
			cfg.winsize = GAIA::CTN::BufferRW::_blocksize;
			cfg.alloc = xd3_alloc_func;
			cfg.freef = xd3_free_func;
			cfg.opaque = GNIL;

			//
		#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
			xd3_stream* pStream = (xd3_stream*)g_gaia_globalmsys.memory_alloc(sizeof(xd3_stream));
		#else
			xd3_stream* pStream = (xd3_stream*)malloc(sizeof(xd3_stream));
		#endif
			zeromem(pStream);

			//
			if(xd3_config_stream(pStream, &cfg) != 0)
			{
			#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
				g_gaia_globalmsys.memory_release(pStream);
			#else
				free(pStream);
			#endif
				return GAIA::False;
			}

			//
		#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
			xd3_source* pSource = (xd3_source*)g_gaia_globalmsys.memory_alloc(sizeof(xd3_source));
		#else
			xd3_source* pSource = (xd3_source*)malloc(sizeof(xd3_source));
		#endif
			zeromem(pSource);
			pSource->blksize = GAIA::CTN::BufferRW::_blocksize;
			pSource->curblk = m_pBuffer;

			//
			GAIA::N64 nRequested = this->request_source((GAIA::GVOID*)pSource->curblk, pSource->blksize * pSource->getblkno, pSource->blksize);
			if(nRequested == 0 || nRequested == -1)
			{
				xd3_close_stream(pStream);
				xd3_free_stream(pStream);
			#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
				g_gaia_globalmsys.memory_release(pStream);
				g_gaia_globalmsys.memory_release(pSource);
			#else
				free(pStream);
				free(pSource);
			#endif
				return GAIA::False;
			}
			pSource->onblk = (GAIA::U32)nRequested;
			pSource->curblkno = 0;

			//
			if(xd3_set_source(pStream, pSource) != 0)
			{
				xd3_close_stream(pStream);
				xd3_free_stream(pStream);
			#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
				g_gaia_globalmsys.memory_release(pStream);
				g_gaia_globalmsys.memory_release(pSource);
			#else
				free(pStream);
				free(pSource);
			#endif
				return GAIA::False;
			}

			//
			m_pStream = pStream;
			m_pSource = pSource;
			m_diffmode = diffmode;
			return GAIA::True;
		}
		GAIA::BL Diff::end()
		{
			if(!this->isbegin())
				return GAIA::False;

			//
			xd3_source* pSource = (xd3_source*)m_pSource;
			xd3_stream* pStream = (xd3_stream*)m_pStream;
			if(xd3_close_stream(pStream) != 0)
				return GAIA::False;

			//
			xd3_free_stream(pStream);

			//
		#if defined(GAIA_HEAP) && defined(GAIA_HEAP_FORCUSTOM)
			g_gaia_globalmsys.memory_release(pStream);
			g_gaia_globalmsys.memory_release(pSource);
		#else
			free(pStream);
			free(pSource);
		#endif
			m_pStream = GNIL;
			m_pSource = GNIL;
			m_BufferRW.clear();
			m_diffmode = GAIA::DIGIT::DIFF_MODE_INVALID;

			return GAIA::True;
		}
		GAIA::BL Diff::isbegin() const
		{
			return m_pStream != GNIL;
		}
		GAIA::DIGIT::DIFF_MODE Diff::mode() const
		{
			return m_diffmode;
		}
		GAIA::U32 Diff::write(const GAIA::GVOID* p, GAIA::U32 size)
		{
			GPCHR_NULL_RET(p, GINVALID);
			GPCHR_BELOWEQUALZERO_RET(size, GINVALID);
			if(!this->isbegin())
				return GINVALID;

			//
			xd3_stream* pStream = (xd3_stream*)m_pStream;
			xd3_source* pSource = (xd3_source*)m_pSource;

			//
			xd3_avail_input(pStream, (GAIA::U8*)p, size);

			//
			for(;;)
			{
				GAIA::N32 nRes;
				if(m_diffmode == GAIA::DIGIT::DIFF_MODE_UNDIFF)
					nRes = xd3_decode_input(pStream);
				else
					nRes = xd3_encode_input(pStream);
				switch(nRes)
				{
				case XD3_INPUT:
					return size;
				case XD3_OUTPUT:
					{
						if(pStream->avail_out > 0)
							m_BufferRW.write(pStream->next_out, pStream->avail_out);
						xd3_consume_output(pStream);
					}
					break;
				case XD3_GETSRCBLK:
					{
						GAIA::N64 nRequested = this->request_source((GAIA::GVOID*)pSource->curblk, pSource->blksize * pSource->getblkno, pSource->blksize);
						if(nRequested == 0 || nRequested == -1)
							return GINVALID;
						pSource->onblk = (GAIA::U32)nRequested;
						pSource->curblkno = pSource->getblkno;
					}
					break;
				case XD3_GOTHEADER:
				case XD3_WINSTART:
				case XD3_WINFINISH:
					break;
				default:
					return GINVALID;
				}
			}
		}
		GAIA::U32 Diff::read(GAIA::GVOID* p, GAIA::U32 size)
		{
			GPCHR_NULL_RET(p, GINVALID);
			GPCHR_BELOWEQUALZERO_RET(size, GINVALID);
			if(!this->isbegin())
				return GINVALID;
			return m_BufferRW.read(p, size);
		}
		GAIA::BL Diff::flush()
		{
			if(!this->isbegin())
				return GAIA::False;

			//
			xd3_stream* pStream = (xd3_stream*)m_pStream;
			xd3_source* pSource = (xd3_source*)m_pSource;

			//
			xd3_set_flags(pStream, pStream->flags | XD3_FLUSH);

			//
			for(;;)
			{
				GAIA::N32 nRes;
				if(m_diffmode == GAIA::DIGIT::DIFF_MODE_UNDIFF)
					nRes = xd3_decode_input(pStream);
				else
					nRes = xd3_encode_input(pStream);
				switch(nRes)
				{
				case XD3_INPUT:
					{
						xd3_set_flags(pStream, pStream->flags & (~XD3_FLUSH));
						return GAIA::True;
					}
				case XD3_OUTPUT:
					{
						m_BufferRW.write(pStream->next_out, pStream->avail_out);
						xd3_consume_output(pStream);
					}
					break;
				case XD3_GETSRCBLK:
					{
						GAIA::N64 nRequested = this->request_source((GAIA::GVOID*)pSource->curblk, pSource->blksize * pSource->getblkno, pSource->blksize);
						if(nRequested == 0 || nRequested == -1)
						{
							xd3_set_flags(pStream, pStream->flags & (~XD3_FLUSH));
							return GAIA::False;
						}
						pSource->onblk = (GAIA::U32)nRequested;
						pSource->curblkno = pSource->getblkno;
					}
					break;
				case XD3_GOTHEADER:
				case XD3_WINSTART:
				case XD3_WINFINISH:
					break;
				default:
					xd3_set_flags(pStream, pStream->flags & (~XD3_FLUSH));
					return GAIA::False;
				}
			}
		}
	}
}
