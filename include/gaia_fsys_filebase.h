#ifndef		__GAIA_FSYS_FILEBASE_H__
#define		__GAIA_FSYS_FILEBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"

namespace GAIA
{
	namespace FSYS
	{
		class FileInfo : public GAIA::Base
		{
		public:
			/*!
				@brief
			*/
			GINL GAIA::GVOID reset()
			{
				pszName = GNIL;
				bDirectory = GAIA::False;
				uCreateTime = 0;
				uLastWriteTime = 0;
				uLastReadTime = 0;
			}

			/*!
				@brief
			*/
			GINL GAIA::BL check() const
			{
				if(pszName == GNIL)
					return GAIA::False;
				return GAIA::True;
			}

		public:
			/*!
				@brief
			*/
			const GAIA::TCH* pszName;

			/*!
				@brief
			*/
			GAIA::U64 uCreateTime;

			/*!
				@brief
			*/
			GAIA::U64 uLastWriteTime;

			/*!
				@brief
			*/
			GAIA::U64 uLastReadTime;

			/*!
				@brief
			*/
			GAIA::BL bDirectory : 1;

		};

		class FileBase : public GAIA::Entity
		{
		public:
			typedef GAIA::N64 __FileSizeType;

		public:
			GAIA_ENUM_BEGIN(OPEN_TYPE)
				OPEN_TYPE_READ	= 1 << 0,
				OPEN_TYPE_WRITE = 1 << 1,
				OPEN_TYPE_CREATEALWAYS	= 1 << 2,
			GAIA_ENUM_END(OPEN_TYPE)

		public:
			/*!
				@brief
			*/
			virtual GAIA::BL Open(const GAIA::TCH* filekey, const GAIA::UM& opentype) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Close() = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL IsOpen() const = 0;

			/*!
				@brief
			*/
			virtual const GAIA::TCH* GetFileKey() const = 0;

			/*!
				@brief
			*/
			virtual GAIA::UM GetOpenType() const = 0;

			/*!
				@brief
			*/
			virtual GAIA::FSYS::FileBase::__FileSizeType Size() const = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Resize(const GAIA::FSYS::FileBase::__FileSizeType& size) = 0;

			/*!
				@brief
			*/
			virtual GAIA::N32 Read(GAIA::GVOID* pDst, GAIA::N32 size) = 0;

			/*!
				@brief
			*/
			virtual GAIA::N32 Write(const GAIA::GVOID* pSrc, GAIA::N32 size) = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Seek(const GAIA::FSYS::FileBase::__FileSizeType& offset, GAIA::SEEK_TYPE seektype = GAIA::SEEK_TYPE_BEGIN) = 0;

			/*!
				@brief
			*/
			virtual GAIA::FSYS::FileBase::__FileSizeType Tell() const = 0;

			/*!
				@brief
			*/
			virtual GAIA::BL Flush() = 0;

			/*!
				@brief
			*/
			template<typename _ObjType> GAIA::BL ReadObj(_ObjType& obj)
			{
				GAST(!!this->IsOpen());
				if(!this->IsOpen())
					return GAIA::False;
				if(sizeof(obj) > this->Size() - this->Tell())
					GTHROW_RET(ExceedContent, GAIA::False);
				if(this->Read(&obj, sizeof(obj)) != sizeof(obj))
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief
			*/
			template<typename _ObjType> GAIA::BL WriteObj(const _ObjType& obj)
			{
				GAST(!!this->IsOpen());
				if(!this->IsOpen())
					return GAIA::False;
				if(this->Write(&obj, sizeof(obj)) != sizeof(obj))
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief
			*/
			template<typename _ParamType> GAIA::BL WriteString(const _ParamType* pszString)
			{
				GAIA::N32 nLen;
				if(pszString != GNIL)
					nLen = GAIA::ALGO::gstrlen(pszString);
				else
					nLen = 0;
				this->WriteObj(nLen);
				if(nLen > 0)
				{
					if(this->Write(pszString, nLen * sizeof(_ParamType)) != nLen * sizeof(_ParamType))
						return GAIA::False;
				}
				return GAIA::True;
			}

			/*!
				@brief
			*/
			template<typename _ParamType> GAIA::BL ReadString(_ParamType* pszString, GAIA::N32 nLen, GAIA::N32& nResultLen)
			{
				GAST(pszString != GNIL);
				if(pszString == GNIL)
					return GAIA::False;
				if(!this->ReadObj(nResultLen))
					return GAIA::False;
				if(nResultLen >= nLen)
				{
					this->Seek(sizeof(nResultLen), GAIA::SEEK_TYPE_FORWARD);
					return GAIA::False;
				}
				if(nResultLen != 0)
				{
					if(this->Read(pszString, nResultLen * sizeof(_ParamType)) != nResultLen * sizeof(_ParamType))
						return GAIA::False;
				}
				pszString[nResultLen] = '\0';
				return GAIA::True;
			}

			/*!
				@brief
			*/
			template<typename _ParamType> GAIA::BL WriteText(const _ParamType* pszText)
			{
				if(GAIA::ALGO::gstremp(pszText))
					return GAIA::False;
				GAIA::N32 nLen = GAIA::ALGO::gstrlen(pszText);
				if(this->Write(pszText, nLen * sizeof(_ParamType)) != nLen * sizeof(_ParamType))
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief
			*/
			template<typename _ObjType> FileBase& operator >> (_ObjType& t){this->ReadObj(t); return *this;}

			/*!
				@brief
			*/
			template<typename _ObjType> FileBase& operator << (const _ObjType& t){this->WriteObj(t); return *this;}
	#ifdef GAIA_DEBUG_MACHINELENGTH
		private: // Protect for 32-64bit error.
			/*!
				@brief Access denied overload function.
			*/
			GINL FileBase& operator = (const FileBase& src)
			{
				GAST(&src != this);
				return *this;
			}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::BL Read(GAIA::NM& obj){return GAIA::False;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::BL Write(const GAIA::NM& obj){return GAIA::False;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::BL Read(GAIA::UM& obj){return GAIA::False;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::BL Write(const GAIA::UM& obj){return GAIA::False;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::BL Read(GAIA::WCH& obj){return GAIA::False;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::BL Write(const GAIA::WCH& obj){return GAIA::False;}

			/*!
				@brief Access denied overload function.
			*/
			GINL FileBase& operator >> (GAIA::NM& t){return *this;}

			/*!
				@brief Access denied overload function.
			*/
			GINL FileBase& operator << (const GAIA::NM& t){return *this;}

			/*!
				@brief Access denied overload function.
			*/
			GINL FileBase& operator >> (GAIA::UM& t){return *this;}

			/*!
				@brief Access denied overload function.
			*/
			GINL FileBase& operator << (const GAIA::UM& t){return *this;}

			/*!
				@brief Access denied overload function.
			*/
			GINL FileBase& operator >> (GAIA::WCH& t){return *this;}

			/*!
				@brief Access denied overload function.
			*/
			GINL FileBase& operator << (const GAIA::WCH& t){return *this;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::N32 Read(GAIA::NM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::N32 Write(const GAIA::NM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::N32 Write(GAIA::NM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::N32 Read(GAIA::UM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::N32 Write(const GAIA::UM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*!
				@brief Access denied overload function.
			*/
			GINL GAIA::N32 Write(GAIA::UM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}
	#endif
		};
	}
}

#endif
