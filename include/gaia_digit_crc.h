#ifndef 	__GAIA_DIGIT_CRC_H__
#define 	__GAIA_DIGIT_CRC_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_vector.h"

namespace GAIA
{
	namespace DIGIT
	{
		extern const GAIA::U32 CRC_MAP32[256];
		
		/*!
		 	@brief Stream based CRC32 algorithm class.
		*/
		class CRC32
		{
		public:
			/*!
			 	@brief Constructor.
			*/
			GINL CRC32(){m_crc = 0;}
			
			/*!
			 	@brief Check current CRC context is changed or not.
				 
				@return If changed, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL check() const{return m_crc != 0;}
			
			/*!
			 	@brief Reset current CRC context to default.
			*/
			GINL GAIA::GVOID reset(){m_crc = 0;}
			
			/*!
			 	@brief Get CRC result.
			 
			 	@return Return CRC32 result.
			*/
			GINL GAIA::U32 result() const{return m_crc;}
			
			/*!
			 	@brief Write more binary data for calculating CRC result.
			 
			 	@param pBuf [in] Specify the buffer which will be writen.
			 
			 	@param size [in] Specify the size of parameter pBuf in bytes.
			 
			 	@return If success, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL update(const GAIA::GVOID* pBuf, GAIA::U32 size)
			{
				GAST(!!pBuf);
				GAST(size > 0);
				if(pBuf == GNIL)
					return GAIA::False;
				if(size <= 0)
					return GAIA::False;
				const GAIA::U8* p = GSCAST(const GAIA::U8*)(pBuf);
				m_crc = m_crc ^ 0xFFFFFFFF;
				while (size >= 8)
				{
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					size -= 8;
				}
				while(size > 0)
				{
					m_crc = CRC_MAP32[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					--size;
				}
				m_crc ^= 0xFFFFFFFF;
				return GAIA::True;
			}
			
			/*!
			 	@brief Write a data type for calculate CRC result.
			 
			 	@param t [in] Specify a data type.
			 
			 	@return If success, return GAIA::True, or will return GAIA::False.
			*/
			template<typename _ParamDataType> GAIA::BL update(const _ParamDataType& t)
			{
				return this->update(&t, sizeof(t));
			}
			
		private:
			GAIA::U32 m_crc;
		};
	}
}

#endif
