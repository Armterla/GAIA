#ifndef 	__GAIA_DIGIT_ZIP_H__
#define 	__GAIA_DIGIT_ZIP_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_bufferrw.h"

namespace GAIA
{
	namespace DIGIT
	{
		/*!
		 	@brief Zip mode.
		*/
		GAIA_ENUM_BEGIN(ZIP_MODE)
			ZIP_MODE_COMPRESS_DEFAULT,		/*!< The default compress mode. */
			ZIP_MODE_COMPRESS_BESTSPEED, 	/*!< Compress for best speed, and the size will larger than best size. */
			ZIP_MODE_COMPRESS_BESTSIZE, 	/*!< Compress for best size, and the speed will slower than best speed. */
			ZIP_MODE_COMPRESS_NOCOMPRESS, 	/*!< No compress but make a package only, the speed is fastest. */
			ZIP_MODE_UNCOMPRESS, 			/*!< Uncompress the package. */
		GAIA_ENUM_END(ZIP_MODE)

		/*!
		 	@brief Get max result length when compress in bytes.
		 
		 	@param srcsize [in] Specify the source data size which will be compress.
		 
		 	@return Return the max result length when compress in bytes.
		*/
		GINL GAIA::NUM max_zip_result_size(const GAIA::NUM& srcsize){return (GAIA::NUM)((GAIA::F32)srcsize * 1.1F + 32.0F);}
		
		/*!
		 	@brief compress a binary buffer.
		 
		 	@param pDst [out] Used for saving the compressed result buffer. 
		 
		 	@param dstsize [in] Specify the buffer size of parameter pDst in bytes.
		 
		 	@param pSrc [in] Specify the source buffer.
		 
		 	@param srcsize [in] Specify the buffer size of parameter pSrc.
		 
		 	@param zmode [in] Specify the zip mode.
		 
		 	@return If success, return the practice destination size in bytes, if failed, return GINVALID.
		 
		 	@see ZIP_MODE.
		*/
		GAIA::NUM zip(GAIA::GVOID* pDst, const GAIA::NUM& dstsize, const GAIA::GVOID* pSrc, const GAIA::NUM& srcsize, GAIA::DIGIT::ZIP_MODE zmode);
		
		/*!
		 	@brief uncompress a binary buffer.
		 
		 	@param pDst [out] Used for saving the uncompressed result buffer.
		 
		 	@param dstsize [in] Specify the buffer size of parameter pDst in bytes.
		 
		 	@param pSrc [in] Specify the source buffer.
		 
		 	@param srcsize [in] Specify the buffer size of parameter pSrc
		 
		 	@return If success, return the practice destination size in bytes, if failed, return GINVALID.
		*/
		GAIA::NUM unzip(GAIA::GVOID* pDst, const GAIA::NUM& dstsize, const GAIA::GVOID* pSrc, const GAIA::NUM& srcsize);
		
		/*!
		 	@brief Stream based compress and uncompress class by zip algorithm.
		*/
		class Zip : public GAIA::Base
		{
		public:
			
			/*!
			 	@brief Constructor.
			*/
			Zip(){this->init();}
			
			/*!
			 	@brief Destructor.
			*/
			~Zip(){if(this->isbegin()) this->end(); gdel m_pBuffer;}
			
			/*!
			 	@brief Check the write buffer is empty or not.
			 
			 	@return If the write buffer is empty, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL empty() const;
			
			/*!
			 	@brief Get write buffer size in bytes.
			 
			 	@return Return the write buffer size in bytes.
			*/
			GAIA::N64 write_size() const;
			
			/*!
			 	@brief Get read buffer size in bytes.
			 
			 	@return Return the read buffer size in bytes.
			*/
			GAIA::N64 read_size() const;
			
			/*!
			 	@brief Get readable size in bytes. 
			 
			 	@return Return readable size in bytes.
			*/
			GAIA::N64 remain() const;
			
			/*!
			 	@brief Get total size in bytes, it include readed or havn't readed.
			 
			 	@return Return the total size in bytes.
			*/
			GAIA::N64 total() const;
			
			/*!
			 	@brief Begin to compress or uncompress data.
			 
			 	@param zmode [in] Specify the zip mode. If want to unzip, it must be ZIP_MODE_UNCOMPRESS.
			 
			 	@return If success, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL begin(GAIA::DIGIT::ZIP_MODE zmode);
			
			/*!
			 	@brief End to compress or uncompress data.
			 
			 	@return If success, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL end();
			
			/*!
			 	@brief Check is began or not began compress(or uncompress) data.
			 
			 	@return If began compress or uncompress, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL isbegin() const;
			
			/*!
			 	@brief Get current zip's work mode.
			 
			 	@return Return current zip mode.
			 		If not began, current function will return ZIP_MODE_INVALID.
			*/
			GAIA::DIGIT::ZIP_MODE mode() const;
			
			/*!
			 	@brief Write data to write buffer.
			 
			 	@param p [in] Specify the buffer which will be write to the zip stream.
			 
			 	@param size [in] Specify the buffer size of parameter p in bytes.
			 
			 	@return Return the writen size.
			*/
			GAIA::U32 write(const GAIA::GVOID* p, GAIA::U32 size);
			
			/*!
			 	@brief Read data from read buffer.
			 
			 	@param p [out] Specify the readed buffer.
			 
			 	@param size [in] Specify the buffer size of parameter p in bytes.
			 
			 	@return Return the readed size.
			*/
			GAIA::U32 read(GAIA::GVOID* p, GAIA::U32 size);
			
			/*!
			 	@brief Write data type to write buffer.
			 
			 	@param t [in] Specify the data type which will be writen.
			 
			 	@return If write data type successfully, return GAIA::True, or will return GAIA::False.
			*/
			template<typename _ParamDataType> GAIA::BL write(const _ParamDataType& t){return this->write(&t, sizeof(t)) == sizeof(t);}
			
			/*!
			 	@brief Read data type from read buffer.
			 
			 	@param t [out] Used for saving the data type which will be readed.
			 
			 	@return If read data type successfully, return GAIA::True, or will return GAIA::False.
			*/
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType& t){return this->read(&t, sizeof(t)) == sizeof(t);}

			/*!
			 	@brief Flush the write buffer.
			 
			 	@return If success, return GAIA::True, or will return GAIA::False.
			*/
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
