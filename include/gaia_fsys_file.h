#ifndef		__GAIA_FSYS_FILE_H__
#define		__GAIA_FSYS_FILE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_locale.h"
#include "gaia_algo_string.h"
#include "gaia_ctn_charsstring.h"
#include "gaia_fsys_filebase.h"

#include <stdio.h>

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <io.h>
#else
#	include <unistd.h>
#endif

namespace GAIA
{
	namespace FSYS
	{
		class File : public FileBase
		{
		public:
			GINL File()
			{
			#ifdef GAIA_DEBUG_INSTANCECOUNT
				GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_FILE, +1);
			#endif
				m_fileopentype = OPEN_TYPE_INVALID;
				m_size = m_offset = 0;
				m_pFile = GNIL;
			}
			GINL virtual ~File()
			{
				if(this->IsOpen())
					this->Close();
			#ifdef GAIA_DEBUG_INSTANCECOUNT
				GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_FILE, -1);
			#endif
			}
			GINL virtual GAIA::BL Open(const GAIA::CH* fileurl, const GAIA::UM& opentype)
			{
				if(this->IsOpen())
					this->Close();
				GAST(!!fileurl);
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(opentype & OPEN_TYPE_CREATEALWAYS)
					m_pFile = (GAIA::GVOID*)fopen(fileurl, "wb+"); // Create for read and write.
				else if(opentype & OPEN_TYPE_WRITE)
					m_pFile = (GAIA::GVOID*)fopen(fileurl, "rb+"); // Open for read and write.
				else if(opentype == OPEN_TYPE_READ)
					m_pFile = (GAIA::GVOID*)fopen(fileurl, "rb"); // Open for read.
				else
					return GAIA::False;
			#else
				if(opentype & OPEN_TYPE_CREATEALWAYS)
					m_pFile = (GAIA::GVOID*)fopen(fileurl, "wb+"); // Create for read and write.
				else if(opentype & OPEN_TYPE_WRITE)
					m_pFile = (GAIA::GVOID*)fopen(fileurl, "rb+"); // Open for read and write.
				else if(opentype == OPEN_TYPE_READ)
					m_pFile = (GAIA::GVOID*)fopen(fileurl, "rb"); // Open for read.
				else
					return GAIA::False;
			#endif
				if(m_pFile == GNIL)
					return GAIA::False;
				m_offset = 0;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(_fseeki64((FILE*)m_pFile, 0, SEEK_END) != 0)
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				if(fseeko((FILE*)m_pFile, 0, SEEK_END) != 0)
			#else
				if(fseeko64((FILE*)m_pFile, 0, SEEK_END) != 0)
			#endif
				{
					this->Close();
					return GAIA::False;
				}
			#if GAIA_OS == GAIA_OS_WINDOWS
				m_size = _ftelli64((FILE*)m_pFile);
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				m_size = ftello((FILE*)m_pFile);
			#else
				m_size = ftello64((FILE*)m_pFile);
			#endif
				GAST(m_size != -1);
				if(m_size == -1)
				{
					this->Close();
					return GAIA::False;
				}
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(_fseeki64((FILE*)m_pFile, 0, SEEK_SET) != 0)
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				if(fseeko((FILE*)m_pFile, 0, SEEK_SET) != 0)
			#else
				if(fseeko64((FILE*)m_pFile, 0, SEEK_SET) != 0)
			#endif
				{
					this->Close();
					return GAIA::False;
				}
				m_strFileUrl = fileurl;
				m_fileopentype = opentype;
			#ifdef GAIA_DEBUG_INSTANCECOUNT
				GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_OPENNEDFILE, +1);
			#endif
				return GAIA::True;
			}
			GINL virtual GAIA::BL Close()
			{
				if(m_pFile != GNIL)
				{
					fclose((FILE*)m_pFile);
					m_pFile = GNIL;
					m_fileopentype = OPEN_TYPE_INVALID;
					m_strFileUrl.destroy();
					m_size = m_offset = 0;
				#ifdef GAIA_DEBUG_INSTANCECOUNT
					GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_OPENNEDFILE, -1);
				#endif
					return GAIA::True;
				}
				return GAIA::False;
			}
			GINL virtual GAIA::BL IsOpen() const{return m_pFile != GNIL;}
			GINL virtual const GAIA::CH* GetFileUrl() const{return m_strFileUrl.fptr();}
			GINL virtual GAIA::UM GetOpenType() const{return m_fileopentype;}
			GINL virtual GAIA::FSYS::FileBase::__FileSizeType Size() const{return m_size;}
			GINL virtual GAIA::BL Resize(const GAIA::FSYS::FileBase::__FileSizeType& size)
			{
				if(!(m_fileopentype & OPEN_TYPE_WRITE) &&
					!(m_fileopentype & OPEN_TYPE_CREATEALWAYS))
					return false;
				if(m_pFile != GNIL)
				{
					if(size <= m_size)
					{
					#if GAIA_OS == GAIA_OS_WINDOWS
						if(_chsize_s(fileno((FILE*)m_pFile), size) != 0)
							return GAIA::False;
					#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
						if(ftruncate(fileno((FILE*)m_pFile), size) != 0)
					#else
						if(ftruncate64(fileno((FILE*)m_pFile), size) != 0)
							return GAIA::False;
					#endif
						return GAIA::True;
					}
					else
					{
					#if GAIA_OS == GAIA_OS_WINDOWS
						GAIA::FSYS::FileBase::__FileSizeType cur = _ftelli64((FILE*)m_pFile);
					#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
						GAIA::FSYS::FileBase::__FileSizeType cur = ftello((FILE*)m_pFile);
					#else
						GAIA::FSYS::FileBase::__FileSizeType cur = ftello64((FILE*)m_pFile);
					#endif
						GAST(cur != -1);
						if(cur == -1)
							return GAIA::False;
					#if GAIA_OS == GAIA_OS_WINDOWS
						if(_fseeki64((FILE*)m_pFile, size - 1, SEEK_SET) != 0)
					#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
						if(fseeko((FILE*)m_pFile, size - 1, SEEK_SET) != 0)
					#else
						if(fseeko64((FILE*)m_pFile, size - 1, SEEK_SET) != 0)
					#endif
							return GAIA::False;
						GAIA::U8 uEnd = 0;
						if(!this->Write(&uEnd, sizeof(uEnd)))
							return GAIA::False;
						if(!this->Flush())
							return GAIA::False;
					#if GAIA_OS == GAIA_OS_WINDOWS
						if(_fseeki64((FILE*)m_pFile, cur, SEEK_SET) != 0)
					#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
						if(fseeko((FILE*)m_pFile, cur, SEEK_SET) != 0)
					#else
						if(fseeko64((FILE*)m_pFile, cur, SEEK_SET) != 0)
					#endif
							return GAIA::False;
						m_size = size;
						return GAIA::True;
					}
				}
				return GAIA::False;
			}
			GINL virtual GAIA::N32 Read(GAIA::GVOID* pDst, GAIA::N32 size)
			{
				GAST(!!pDst);
				GAST(size > 0);
				if(pDst == GNIL)
					return 0;
				if(size <= 0)
					return 0;
				if(m_pFile == GNIL)
					return 0;
				if(!(m_fileopentype & OPEN_TYPE_READ))
					return 0;
				GAIA::FSYS::FileBase::__FileSizeType nReaded;
				if((nReaded = fread(pDst, 1, size, (FILE*)m_pFile)) > 0)
				{
					m_offset += nReaded;
					return (GAIA::N32)nReaded;
				}
				return 0;
			}
			GINL virtual GAIA::N32 Write(const GAIA::GVOID* pSrc, GAIA::N32 size)
			{
				GAST(!!pSrc);
				GAST(size > 0);
				if(pSrc == GNIL)
					return 0;
				if(size <= 0)
					return 0;
				if(m_pFile == GNIL)
					return 0;
				if(!(m_fileopentype & OPEN_TYPE_WRITE))
					return 0;
				GAIA::FSYS::FileBase::__FileSizeType nWriten;
				if((nWriten = fwrite(pSrc, 1, size, (FILE*)m_pFile)) > 0)
				{
					m_offset += nWriten;
					if(m_offset > m_size)
						m_size = m_offset;
					return (GAIA::N32)nWriten;
				}
				return 0;
			}
			GINL virtual GAIA::BL Seek(const GAIA::FSYS::FileBase::__FileSizeType& offset, GAIA::SEEK_TYPE seektype = GAIA::SEEK_TYPE_BEGIN)
			{
				if(m_pFile == GNIL)
					return GAIA::False;
				GAIA::FSYS::FileBase::__FileSizeType toffset = m_offset;
				if(seektype == SEEK_TYPE_BEGIN)
					toffset = GAIA::MATH::gclamp(offset, 0, m_size);
				else if(seektype == SEEK_TYPE_END)
					toffset = GAIA::MATH::gclamp(m_size - offset, 0, m_size);
				else if(seektype == SEEK_TYPE_FORWARD)
					toffset = GAIA::MATH::gclamp(m_offset + offset, 0, m_size);
				else if(seektype == SEEK_TYPE_BACKWARD)
					toffset = GAIA::MATH::gclamp(m_offset - offset, 0, m_size);
				if(toffset == m_offset)
					return GAIA::True;
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(_fseeki64((FILE*)m_pFile, toffset, SEEK_SET) == 0)
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				if(fseeko((FILE*)m_pFile, toffset, SEEK_SET) == 0)
			#else
				if(fseeko64((FILE*)m_pFile, toffset, SEEK_SET) == 0)
			#endif
				{
					m_offset = toffset;
					return GAIA::True;
				}
				return GAIA::False;
			}
			GINL virtual GAIA::FSYS::FileBase::__FileSizeType Tell() const{return m_offset;}
			GINL virtual GAIA::BL Flush()
			{
				if(m_pFile == GNIL)
					return GAIA::False;
				if(fflush((FILE*)m_pFile) == 0)
					return GAIA::True;
				return GAIA::False;
			}
		private:
			GAIA::CTN::ACharsString m_strFileUrl;
			GAIA::UM m_fileopentype;
			GAIA::FSYS::FileBase::__FileSizeType m_size;
			GAIA::FSYS::FileBase::__FileSizeType m_offset;
			GAIA::GVOID* m_pFile;
		};
	}
}

#endif
