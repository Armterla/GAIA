#ifndef 	__GAIA_DIGIT_CRC_H__
#define 	__GAIA_DIGIT_CRC_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_vector.h"

namespace GAIA
{
	namespace DIGIT
	{
		/*!
		 	@brief Stream based CRC8 algorithm class.
		*/
		class CRC8
		{
		public:
			/*!
			 	@brief Constructor.
			*/
			GINL CRC8(){m_crc = 0; this->prepare();}
			
			/*!
			 	@brief Clear current CRC context.
			*/
			GINL GAIA::GVOID clear(){m_crc = 0;}
			
			/*!
			 	@brief Get CRC result.
			 
			 	@return Return CRC8 result.
			*/
			GINL GAIA::U8 result() const{return m_crc;}
			
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
				return GAIA::True;
			}
		private:
			GINL GAIA::GVOID prepare(){}
		private:
			GAIA::U8 m_crc;
			GAIA::CTN::Vector<GAIA::U8> m_crctable;
		};
		
		/*!
		 	@brief Stream based CRC16 algorithm class.
		*/
		class CRC16
		{
		public:
			/*!
			 	@brief Constructor.
			*/
			GINL CRC16(){m_crc = 0; this->prepare();}
			
			/*!
			 	@brief Clear current CRC context.
			*/
			GINL GAIA::GVOID clear(){m_crc = 0;}
			
			/*!
			 	@brief Get CRC result.
			 
			 	@return Return CRC16 result.
			*/
			GINL GAIA::U16 result() const{return m_crc;}
			
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
				return GAIA::True;
			}
		private:
			GINL GAIA::GVOID prepare(){}
		private:
			GAIA::U16 m_crc;
			GAIA::CTN::Vector<GAIA::U16> m_crctable;
		};
		
		/*!
		 	@brief Stream based CRC32 algorithm class.
		*/
		class CRC32
		{
		public:
			/*!
			 	@brief Constructor.
			*/
			GINL CRC32(){m_crc = 0; this->prepare();}
			
			/*!
			 	@brief Clear current CRC context.
			*/
			GINL GAIA::GVOID clear(){m_crc = 0;}
			
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
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					size -= 8;
				}
				while(size > 0)
				{
					m_crc = m_crctable[((GAIA::N32)m_crc ^ (*p++)) & 0xFF] ^ (m_crc >> 8);
					--size;
				}
				m_crc ^= 0xFFFFFFFF;
				return GAIA::True;
			}
		private:
			GINL GAIA::GVOID prepare()
			{
				static const GAIA::U8 p[] = {0, 1, 2, 4, 5, 7, 8, 10, 11, 12, 16, 22, 23, 26};
				m_crctable.resize(256);
				GAIA::U32 poly = 0;
				for(GAIA::N32 x = 0; x < sizeofarray(p); x++)
					poly |= 1 << (31 - p[x]);
				for(GAIA::N32 x = 0; x < 256; x++)
				{
					GAIA::U32 c = GSCAST(GAIA::U32)(x);
					for(GAIA::N32 y = 0; y < 8; y++)
						c = c & 1 ? poly ^ (c >> 1) : c >> 1;
					m_crctable[x] = c;
				}
			}
		private:
			GAIA::U32 m_crc;
			GAIA::CTN::Vector<GAIA::U32> m_crctable;
		};
	}
}

#endif
