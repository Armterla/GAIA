#ifndef		__GAIA_FSYS_MEMFILE_H__
#define		__GAIA_FSYS_MEMFILE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"
#include "gaia_ctn_charsstring.h"
#include "gaia_ctn_buffer.h"
#include "gaia_fsys_filebase.h"

namespace GAIA
{
	namespace FSYS
	{
		class MemFile : public GAIA::FSYS::FileBase
		{
		public:
			typedef GAIA::CTN::BasicBuffer<GAIA::FSYS::FileBase::__FileSizeType, GAIA::ALGO::ExtendGold<GAIA::FSYS::FileBase::__FileSizeType> > _BufferType;
		public:
			GINL MemFile(){this->init();}
			GINL ~MemFile(){if(this->IsOpen()) this->Close();}
		public:
			virtual GAIA::BL Open(const GAIA::CH* fileurl, const GAIA::UM& opentype)
			{
				if(this->IsOpen())
					this->Close();
				m_strFileUrl = fileurl;
				m_fileopentype = opentype;
				return GAIA::True;
			}
			virtual GAIA::BL Close()
			{
				if(!this->IsOpen())
					return GAIA::False;
				m_strFileUrl.clear();
				m_fileopentype = GAIA::FSYS::FileBase::OPEN_TYPE_INVALID;
				m_buf.destroy();
				m_filesize = 0;
				return GAIA::True;
			}
			virtual GAIA::BL IsOpen() const{return m_fileopentype != GAIA::FSYS::FileBase::OPEN_TYPE_INVALID;}
			GINL virtual const GAIA::CH* GetFileUrl() const{return m_strFileUrl.fptr();}
			GINL virtual GAIA::UM GetOpenType() const{return m_fileopentype;}
			virtual GAIA::FSYS::FileBase::__FileSizeType Size() const{return m_filesize;}
			virtual GAIA::BL Resize(const GAIA::FSYS::FileBase::__FileSizeType& size)
			{
				GPCHR_BELOWZERO_RET(size, GAIA::False);
				if(!(m_fileopentype & GAIA::FSYS::FileBase::OPEN_TYPE_WRITE))
					return GAIA::False;
				m_buf.resize_keep(size);
				m_filesize = size;
				return GAIA::True;
			}
			virtual GAIA::N32 Read(GAIA::GVOID* pDst, GAIA::N32 size)
			{
				GPCHR_NULL_RET(pDst, GINVALID);
				GPCHR_BELOWEQUALZERO_RET(size, GINVALID);
				GAIA::FSYS::FileBase::__FileSizeType remainsize = m_filesize - m_buf.write_size();
				if(remainsize == 0)
					return 0;
				if(size > remainsize)
					size = (GAIA::N32)remainsize;
				GAIA::ALGO::gmemcpy(pDst, m_buf.write_ptr(), size);
				if(!m_buf.seek_write(m_buf.write_size() + size))
					return GINVALID;
				return size;
			}
			virtual GAIA::N32 Write(const GAIA::GVOID* pSrc, GAIA::N32 size)
			{
				GPCHR_NULL_RET(pSrc, GINVALID);
				GPCHR_BELOWEQUALZERO_RET(size, GINVALID);
				if(!(m_fileopentype & GAIA::FSYS::FileBase::OPEN_TYPE_WRITE))
					return GINVALID;
				m_buf.write(pSrc, size);
				if(m_buf.write_size() > m_filesize)
					m_filesize = m_buf.write_size();
				return size;
			}
			virtual GAIA::BL Seek(const GAIA::FSYS::FileBase::__FileSizeType& offset, GAIA::SEEK_TYPE seektype = GAIA::SEEK_TYPE_BEGIN)
			{
				if(!(m_fileopentype & GAIA::FSYS::FileBase::OPEN_TYPE_WRITE) &&
				   !(m_fileopentype & GAIA::FSYS::FileBase::OPEN_TYPE_READ))
					return GAIA::False;
				GAIA::FSYS::FileBase::__FileSizeType oldwritesize = m_buf.write_size();
				if(!m_buf.seek_write(offset, seektype))
					return GAIA::False;
				if(m_buf.write_size() > m_filesize)
				{
					m_buf.seek_write(oldwritesize);
					return GAIA::False;
				}
				return GAIA::True;
			}
			virtual GAIA::FSYS::FileBase::__FileSizeType Tell() const{return m_buf.write_size();}
			virtual GAIA::BL Flush()
			{
				if(!(m_fileopentype & GAIA::FSYS::FileBase::OPEN_TYPE_WRITE))
					return GAIA::False;
				return GAIA::True;
			}
		public:
			GINL const _BufferType& GetBuffer() const{return m_buf;}
		private:
			GINL GAIA::GVOID init()
			{
				m_fileopentype = GAIA::FSYS::FileBase::OPEN_TYPE_INVALID;
				m_filesize = 0;
			}
		private:
			GAIA::CTN::ACharsString m_strFileUrl;
			GAIA::UM m_fileopentype;
			_BufferType m_buf;
			GAIA::FSYS::FileBase::__FileSizeType m_filesize;
		};
	}
}

#endif
