#ifndef		__GAIA_FSYS_FILEBASE_H__
#define		__GAIA_FSYS_FILEBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_ctn_buffer.h"

namespace GAIA
{
	namespace FSYS
	{
		/*!
			@brief File description class.
		*/
		class FileDesc : public GAIA::Base
		{
		public:
			/*!
				@brief Reset all member variables to default value.
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
				@brief Check current FileInfo's member variables is all valid or not.
			 
				@return If current FileInfo's member variables is all value, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL check() const
			{
				if(pszName == GNIL)
					return GAIA::False;
				return GAIA::True;
			}

		public:
			/*!
				@brief Specify the file's name.
			*/
			const GAIA::CH* pszName;

			/*!
				@brief Specify the file's create time.
			*/
			GAIA::U64 uCreateTime;

			/*!
				@brief Specify the file's last write time(modify time).
			*/
			GAIA::U64 uLastWriteTime;

			/*!
				@brief Specify the file's last read time(access time).
			*/
			GAIA::U64 uLastReadTime;

			/*!
				@brief Specify the file is a directory or not.
			*/
			GAIA::BL bDirectory : 1;
		};

		/*!
			@brief File's base class.
		*/
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
				@brief Open the file.
			 
				@param fileurl [in] Specify the file's url.
			 
				@param opentype [in] Specify the file's open type.
			 
				@return If open file successfully, return GAIA::True, or will return GAIA::False.
			*/
			virtual GAIA::BL Open(const GAIA::CH* fileurl, const GAIA::UM& opentype) = 0;

			/*!
				@brief Close the file.
			 
				@return If close file successfully, return GAIA::True, or will return GAIA::False.
			*/
			virtual GAIA::BL Close() = 0;

			/*!
				@brief Check current file is open or not.
			 
				@return If the file is openned, return GAIA::True, or will return GAIA::False.
			*/
			virtual GAIA::BL IsOpen() const = 0;

			/*!
				@brief Get file url.
			 
				@return Return file url.
			*/
			virtual const GAIA::CH* GetFileUrl() const = 0;

			/*!
				@brief Get file's open type.
			 
				@see GAIA::FSYS::FileBase::OPEN_TYPE.
			*/
			virtual GAIA::UM GetOpenType() const = 0;

			/*!
				@brief Get file's size in bytes.
			*/
			virtual GAIA::FSYS::FileBase::__FileSizeType Size() const = 0;

			/*!
				@brief Resize the file.
			 
			 	@param size [in] Specify the new file size.
			 
			 	@return If resize file successfully, return GAIA::True, or return GAIA::False.
			*/
			virtual GAIA::BL Resize(const GAIA::FSYS::FileBase::__FileSizeType& size) = 0;

			/*!
				@brief Read data from file.
			 
			 	@param pDst [in] Used for save the readed result.
			 
			 	@param size [in] Specify the buffer size of parameter pDst in bytes.
			 
			 	@return Return the practice read size.
			*/
			virtual GAIA::N32 Read(GAIA::GVOID* pDst, GAIA::N32 size) = 0;

			/*!
				@brief Write data to file.
			 
			 	@param pSrc [in] Specify the buffer which will be write to file.
			 
			 	@param size [in] Specify the buffer size of parameter pSrc in bytes.
			 
			 	@return Return the practice write size.
			*/
			virtual GAIA::N32 Write(const GAIA::GVOID* pSrc, GAIA::N32 size) = 0;

			/*!
				@brief Seek file pointer.
			 
			 	@param offset [in] Specify the size of seek offset in bytes.
			 
			 	@param seektype [in] Specify the seek mode.
			 
			 	@return If seek successfully, return GAIA::True, or will return GAIA::False.
			*/
			virtual GAIA::BL Seek(const GAIA::FSYS::FileBase::__FileSizeType& offset, GAIA::SEEK_TYPE seektype = GAIA::SEEK_TYPE_BEGIN) = 0;

			/*!
				@brief Retrieve the file pointer.
			 
			 	@return Return current location of file pointer.
			*/
			virtual GAIA::FSYS::FileBase::__FileSizeType Tell() const = 0;

			/*!
				@brief Flush the file's write buffer.
			 
			 	@return If flush success, return GAIA::True, or will return GAIA::False.
			*/
			virtual GAIA::BL Flush() = 0;

			/*!
				@brief Read a object from file.
			 
			 	@return If read object successfully, return GAIA::True, or will return GAIA::False.
			 
				@remarks The object must be a base c data type, or a struct with all c data type member variables,\n
					c data type is char, short, int, long long, float, double, bool.\n
					The integer type support "unsigned" indicator.\n
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
				@brief Write a object to file.
			 
			 	@return If write object successfully, return GAIA::True, or will return GAIA::False.
			 
				@remarks The object must be a base c data type, or a struct with all c data type member variables,\n
					c data type is char, short, int, long long, float, double, bool.\n
					The integer type support "unsigned" indicator.\n
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
				@brief Read all remain data to a buffer object.
			 
				@param buf [out] Used for save the result data.
			 
				@return If success return GAIA::True, or will return GAIA::False.
			 
				@remarks If the file remain size is above 1Gbytes, this function will return GAIA::False.
			 		After this function call and return true, the parameter buf will be filled by the remain data, and the buf's writer pointer will move to the end of the data.
		 			After this function call and return true, the file pointer will move to the end of file.
			 		After this function call and return false, the file pointer will not be moved.
		 	*/
			GAIA::BL ReadRemainAll(GAIA::CTN::Buffer& buf)
			{
				GAST(!!this->IsOpen());
				if(!this->IsOpen())
					return GAIA::False;
				GAIA::FSYS::FileBase::__FileSizeType oldseek = this->Tell();
				GAIA::FSYS::FileBase::__FileSizeType r = this->Size() - oldseek;
				if(r > 1024 * 1024 * 1024)
					return GAIA::False;
				buf.resize((GAIA::NUM)r);
				GAIA::FSYS::FileBase::__FileSizeType t = 0;
				while(r > 0)
				{
					GAIA::FSYS::FileBase::__FileSizeType step = r;
					if(step > 1024 * 1024 * 16)
						step = 1024 * 1024 * 16;
					GAIA::FSYS::FileBase::__FileSizeType readed = this->Read(buf.fptr() + t, (GAIA::NUM)step);
					if(readed <= 0)
					{
						this->Seek(oldseek);
						return GAIA::False;
					}
					r -= readed;
					t += readed;
				}
				return GAIA::True;
			}

			/*!
				@brief Write a string to file.
			 
			 	@return If write string successfully, return GAIA::True, or will return GAIA::False.
			 
				@remarks This method write a integer(int) type for string length first, and then write the string without '\0'.
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
				@brief Read a string from file.
			 
			 	@return If read a string successfully, return GAIA::True, or will return GAIA::False.
			 
				@remarks This method read a integer(int) type from file for string length first, and then read the string without '\0'.\n
					But '\0' will be filled to the last location of parameter pszString.
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
				@brief Write a text string to the file.
			 
			 	@return If write text successfully, return GAIA::True, or will return GAIA::False.
			 
				@remarks This method write the string without '\0'.
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
				@brief Stream operator for read object.
			 
			 	@return Return current object.
			 
				@remarks The object must be a base c data type, or a struct with all c data type member variables,\n
					c data type is char, short, int, long long, float, double, bool.\n
					The integer type support "unsigned" indicator.\n
			*/
			template<typename _ObjType> FileBase& operator >> (_ObjType& t){this->ReadObj(t); return *this;}

			/*!
				@brief Stream operator for write object.
			 
			 	@return Return current object.
			 
				@remarks The object must be a base c data type, or a struct with all c data type member variables,\n
					c data type is char, short, int, long long, float, double, bool.\n
					The integer type support "unsigned" indicator.\n
			*/
			template<typename _ObjType> FileBase& operator << (const _ObjType& t){this->WriteObj(t); return *this;}
			
	#ifdef GAIA_DEBUG_MACHINELENGTH
			
		private: // Protect for 32-64bit error.
			
			/*! @brief Access denied overload function. */
			GINL FileBase& operator = (const FileBase& src){GAST(&src != this); return *this;}

			/*! @brief Access denied overload function. */
			GINL GAIA::BL Read(GAIA::NM& obj){return GAIA::False;}

			/*! @brief Access denied overload function. */
			GINL GAIA::BL Write(const GAIA::NM& obj){return GAIA::False;}

			/*! @brief Access denied overload function. */
			GINL GAIA::BL Read(GAIA::UM& obj){return GAIA::False;}

			/*! @brief Access denied overload function. */
			GINL GAIA::BL Write(const GAIA::UM& obj){return GAIA::False;}

			/*! @brief Access denied overload function. */
			GINL GAIA::BL Read(GAIA::WCH& obj){return GAIA::False;}

			/*! @brief Access denied overload function. */
			GINL GAIA::BL Write(const GAIA::WCH& obj){return GAIA::False;}

			/*! @brief Access denied overload function. */
			GINL FileBase& operator >> (GAIA::NM& t){return *this;}

			/*! @brief Access denied overload function. */
			GINL FileBase& operator << (const GAIA::NM& t){return *this;}

			/*! @brief Access denied overload function. */
			GINL FileBase& operator >> (GAIA::UM& t){return *this;}

			/*! @brief Access denied overload function. */
			GINL FileBase& operator << (const GAIA::UM& t){return *this;}

			/*! @brief Access denied overload function. */
			GINL FileBase& operator >> (GAIA::WCH& t){return *this;}

			/*! @brief Access denied overload function. */
			GINL FileBase& operator << (const GAIA::WCH& t){return *this;}

			/*! @brief Access denied overload function. */
			GINL GAIA::N32 Read(GAIA::NM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*! @brief Access denied overload function. */
			GINL GAIA::N32 Write(const GAIA::NM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*! @brief Access denied overload function. */
			GINL GAIA::N32 Write(GAIA::NM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*! @brief Access denied overload function. */
			GINL GAIA::N32 Read(GAIA::UM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*! @brief Access denied overload function. */
			GINL GAIA::N32 Write(const GAIA::UM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}

			/*! @brief Access denied overload function. */
			GINL GAIA::N32 Write(GAIA::UM* pDst, const GAIA::FSYS::FileBase::__FileSizeType& size){return GINVALID;}
	#endif
		};
	}
}

#endif
