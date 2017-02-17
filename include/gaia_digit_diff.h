#ifndef 	__GAIA_DIGIT_DIFF_H__
#define 	__GAIA_DIGIT_DIFF_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_bufferrw.h"

namespace GAIA
{
	namespace DIGIT
	{
		GAIA_ENUM_BEGIN(DIFF_MODE)
			DIFF_MODE_DEFAULT, 		/*!< Generate diff patch by default setting */
			DIFF_MODE_BESTSPEED, 	/*!< Generate diff patch with best speed */
			DIFF_MODE_BESTSIZE, 	/*!< Generate diff patch with smallest size */
			DIFF_MODE_NOCOMPRESS,
			DIFF_MODE_UNDIFF, 		/*!< Merge diff patch to source, and generate the destination binary data. */
		GAIA_ENUM_END(DIFF_MODE)

		/*!
			@brief Simple generate diff patch function.
		*/
		GAIA::N64 diff(GAIA::GVOID* pDiff, const GAIA::N64& diffsize,
			const GAIA::GVOID* pSrc, const GAIA::N64& srcsize,
			const GAIA::GVOID* pDst, const GAIA::N64& dstsize, GAIA::DIGIT::DIFF_MODE diffmode);

		/*!
			@brief Simple merge diff patch function.
		*/
		GAIA::N64 undiff(GAIA::GVOID* pDst, const GAIA::N64& dstsize,
			const GAIA::GVOID* pSrc, const GAIA::N64& srcsize,
			const GAIA::GVOID* pDiff, const GAIA::N64& diffsize, GAIA::DIGIT::DIFF_MODE diffmode);
		
		/*!
			Stream based binary difference class.
			
			Concept
				-# Source
					- it means source binary data.
				-# Destination
					- In generate diff patch work mode, the Destination means the data of binary-diff-patch.
					- In merge diff patch work mode, the Destination means the data of merge-result.
				-# Input
					- In generate diff patch work mode, the Input means the data of merge-result.
					- In merge diff patch work mode, the Input means the data of binary-diff-patch.
		*/
		class Diff : public GAIA::Base
		{
		public:
			Diff(){this->init();}
			~Diff(){if(this->isbegin()) this->end(); gdel m_pBuffer;}
			
			/*!
				@brief Check exist wrote data.

				@return If there is no data be wrote to read buffer after call begin function, return GAIA::True, or return GAIA::False.
				If not began(See begin function), return 0.
			*/
			GAIA::BL empty() const;
			
			/*!
				@brief Get wrote data size.

				@return Return how many data had been wrote after call begin function.
			*/
			GAIA::N64 write_size() const;
			
			/*!
				@brief Get readed data size.

				@return Return how many data had been read after call begin function.
			*/
			GAIA::N64 read_size() const;
			
			/*!
				@brief Get read able data size.

				@return Return how many data remained in read buffer, size in bytes. 
			*/
			GAIA::N64 remain() const;
			
			/*!
				@brief Get total read data size.

				@return Return how many data in read buffer in total. include readed data size!
			*/
			GAIA::N64 total() const;
			
			/*!
				@brief Begin to work.
				
				@param diffmode [in]
					Specify the work mode.
					
				@remarks
					Before use current class, call begin function first!
			*/
			GAIA::BL begin(GAIA::DIGIT::DIFF_MODE diffmode);
			
			/*!
				@brief End to work.
				
				@return If had began by call begin function, return GAIA::True, or will return GAIA::False.
				
				@remarks
					If the object of current class will be delete, the end function will be called automatic when 
					the object had been began by begin function.
			*/
			GAIA::BL end();
			
			/*!
				@brief Check the diff work is begin or not.

				If begin function had be called, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL isbegin() const;
			
			/*!
				@brief Get diff mode.

				@return Return current work mode which is specified by function begin by user.
			*/
			GAIA::DIGIT::DIFF_MODE mode() const;
			
			/*!
				@brief Write date to diff.
			*/
			GAIA::U32 write(const GAIA::GVOID* p, GAIA::U32 size);

			/*!
				@brief Read diff result.
			*/
			GAIA::U32 read(GAIA::GVOID* p, GAIA::U32 size);
			
			/*!
				@brief Write simple type data to diff.
			*/
			template<typename _ParamDataType> GAIA::BL write(const _ParamDataType& t){return this->write(&t, sizeof(t)) == sizeof(t);}

			/*!
				@brief Read simple type data result.
			*/
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType& t){return this->read(&t, sizeof(t)) == sizeof(t);}

			/*!
				@brief Flush the write buffer.

				Flush the write buffer will make the read buffer be fill, and then, call read to get result data.

				@remarks
					- What time return GAIA::True ? 
						- When all the data had been handled, and need more input data(user can call write function to commit more input data)
					- What time return GAIA::False ? 
						1. When work procedule call request_source function and failed, return GAIA::False.
						2. If not began by begin function.
			*/
			GAIA::BL flush();
			
		protected:
			/*!
				@brief Request(CallBack) the source data from class user.

				Because current class is work as a stream, and the source data maybe too large to be loaded in ram,
				when match the binary data between input and source, this function will be called, the callee will
				return the binary data to caller.
				
				@param p [out]
					Callee will fill the result data to it, it is be allocated by the caller, it must not NULL, it is a out parameter.
					
				@param nOffset [in]
					Specify the start position from the source data's head.
					
				@param nSize [in]
					Specify how many binary data is requested.
					
				@return
					If callee can fill any of byte data by param nOffset, return the practice data size which had be filled.
					If callee cannot return any of data by param nOffset and param nSize, return 0.
					WARRING! The callee cannot return value which below 0.
						
			*/
			virtual GAIA::N64 request_source(GAIA::GVOID* p, const GAIA::N64& nOffset, const GAIA::N64& nSize) = 0;

		private:
			GAIA::GVOID init()
			{
				m_diffmode = GAIA::DIGIT::DIFF_MODE_INVALID;
				m_pStream = GNIL;
				m_pSource = GNIL;
				m_pBuffer = gnew GAIA::U8[GAIA::CTN::BufferRW::_blocksize];
			}	
			
		private:
			GAIA::DIGIT::DIFF_MODE m_diffmode;
			GAIA::GVOID* m_pStream;
			GAIA::GVOID* m_pSource;
			GAIA::CTN::BufferRW m_BufferRW;
			GAIA::U8* m_pBuffer;
		}; 
	}	
}

#endif
