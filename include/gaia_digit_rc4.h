#ifndef		__GAIA_DIGIT_RC4_H__
#define		__GAIA_DIGIT_RC4_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace DIGIT
	{
		/*!
		 	@brief RC4 crypt algorithm.
		 		Current class support stream based data crypt or decrypt.
		 
		 	_KeySize Specify the middle key size in bytes.
				Usually, middle key size will be 128(1024 bit) or 256(2048 bit) for safely crypt data.
		*/
		template<GAIA::U32 _KeySize> class RC4
		{
		public:
			static const GAIA::U32 _keysize = _KeySize;
			
		public:
			/*!
			 	@brief Set middle key. 
			 
			 	@param p [in] Specify the middle key.
			 
			 	@param uKeySize [in] Specify the middle key size in bytes.
			*/
			GINL GAIA::BL set_key(const GAIA::GVOID* p, GAIA::U32 uKeySize)
			{
				GAST(uKeySize == sizeof(m_key));
				if(uKeySize != sizeof(m_key))
					return GAIA::False;
				GAIA::ALGO::gmemcpy(m_key, p, sizeof(m_key));
				return GAIA::True;
			}
			
			/*!
			 	@brief Get middle key.
			 
			 	@param p [out] Used for saving the middle key.
			 
			 	@param uKeySize [in] Specify the buffer size of parameter p.
			*/
			GINL GAIA::BL get_key(GAIA::GVOID* p, GAIA::U32 uKeySize) const
			{
				GAST(uKeySize == sizeof(m_key));
				if(uKeySize != sizeof(m_key))
					return GAIA::False;
				GAIA::ALGO::gmemcpy(p, m_key, sizeof(m_key));
				return GAIA::True;
			}
			
			/*!
			 	@brief Build(Generate) middle key by origin key.
			 
			 	@param pKey [in] Specify the origin key.
			 		It must not NULL.
			 
			 	@param uKeySize [in] Specify the size of origin key in bytes.
			 		It must above zero.
			*/
			GINL GAIA::GVOID build_key(const GAIA::GVOID* pKey, GAIA::U32 uKeySize)
			{
				GAST(pKey != GNIL);
				GAST(uKeySize >= 0);

				for(GAIA::U32 x = 0; x < sizeof(m_key); ++x)
					m_key[x] = (GAIA::U8)(x % 0x0100);

				GAIA::U32 i1 = 0;
				GAIA::U32 i2 = 0;
				for(GAIA::U32 x = 0; x < _KeySize; ++x)
				{
					i2 = (((GAIA::U8*)pKey)[i1] + m_key[x] + i2) % _KeySize;
					GAIA::ALGO::swap(m_key[x], m_key[i2]);
					i1 = (i1 + 1) % uKeySize;
				}
			}
			
			/*!
			 	@brief Crypt or decrypt binary buffer.
			 
			 	@param pSrc [in] Specify the source buffer.
			 		It must not NULL.
			 
			 	@param pDst [out] Specify the destination buffer.
			 		It must not NULL.
			 
			 	@param uSize [in] Specify the source and destination buffer size in bytes.
			 		It must above zero.
			 
			 	@remarks The source buffer size must equal destination buffer size.
			*/
			GINL GAIA::GVOID update(const GAIA::GVOID* pSrc, GAIA::GVOID* pDst, GAIA::U32 uSize)
			{
				GAST(pSrc != GNIL);
				GAST(pDst != GNIL);
				GAST(uSize > 0);

				GAIA::U32 i1 = 0;
				GAIA::U32 i2 = 0;
				GAIA::U32 i;

				for(GAIA::U32 x = 0; x < uSize; x++)
				{
					i1 = (i1 + 1) % _KeySize;
					i2 = (m_key[i1] + i2) % _KeySize;
					GAIA::ALGO::swap(m_key[i1], m_key[i2]);
					i = (m_key[i1] + m_key[i2]) % _KeySize;
					((GAIA::U8*)pDst)[x] = ((GAIA::U8*)pSrc)[x] ^ m_key[i];
				}
			}
			
		private:
			GAIA::U8 m_key[_KeySize];
		};
	}
}

#endif
