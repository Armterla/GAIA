#ifndef		__GAIA_DIGIT_RSA_H__
#define		__GAIA_DIGIT_RSA_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_random_lcg.h"

#include "rsa.h"
#include "bignum.h"

namespace GAIA
{
	namespace DIGIT
	{
		GINL GAIA::N32 rsa_rand(GAIA::GVOID* p)
		{
			GAIA::MATH::RandomLCG& lcg = *(GAIA::MATH::RandomLCG*)p;
			return lcg.random();
		}

		/*!
		 	@brief RSA crypt algorithm.
		*/
		class RSA
		{
		public:
			static const GAIA::NUM DEFAULT_BIT_COUNT = 1024;
			
		public:
			/*!
				@brief Constructor.
			*/
			RSA()
			{
				m_sBitCount = 0;
				rsa_init(&m_rsa, RSA_PKCS_V15, 0);
			}

			/*!
				@brief Destructor.
			*/
			~RSA()
			{
				rsa_free(&m_rsa);
			}

			/*!
				@brief Reset context to default.
			 
			 	@remarks After this function call, all RSA context is reset.
			 		The object status is samed to a new allocated RSA object.
			*/
			GINL GAIA::GVOID reset()
			{
				m_sBitCount = 0;
				rsa_free(&m_rsa);
				rsa_init(&m_rsa, RSA_PKCS_V15, 0);
			}

			/*!
				@brief Get RSA crypt bit count.
			 		Default value is DEFAULT_BIT_COUNT.
			 
			 	@Return Return the bit count of current RSA object.
			*/
			GINL GAIA::NUM bit_count()
			{
				return m_sBitCount;
			}

			/*!
				@brief Build a key.
			 
			 	@param sBitCount [in] 
			 
			 	@param sExp [in]
			 
			 	@param uRandomSeed [in]
			 
			 	@remarks Current function will cost a lot of CPU time.
			*/
			GINL GAIA::BL build_key(GAIA::NUM sBitCount = DEFAULT_BIT_COUNT, GAIA::NUM sExp = 65537, GAIA::U32 uRandomSeed = (GAIA::U32)(GAIA::TIME::tick_time() % 10000))
			{
				m_sBitCount = sBitCount;
				return rsa_gen_key(&m_rsa, rsa_rand, &m_lcg, sBitCount, sExp) == 0;
			}

			/*!
				@brief Set public key.
			 
			 	@param n [in] Specify RSA-N digit of public key.
			 
			 	@param e [in] Specify RSA-E digit of public key.
			 
			 	@param sBitCount [in] Specify RSA-BitCount.
			 
			 	@return If set public key successfully, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL set_public_key(const GAIA::CH* n, const GAIA::CH* e, GAIA::NUM sBitCount = DEFAULT_BIT_COUNT)
			{
				GAST(!GAIA::ALGO::gstremp(n));
				GAST(!GAIA::ALGO::gstremp(e));
				if(mpi_read_string(&m_rsa.N, 10, n) != 0)
					return GAIA::False;
				if(mpi_read_string(&m_rsa.E, 10, e) != 0)
					return GAIA::False;
				m_rsa.len = sBitCount / 8;
				m_sBitCount = sBitCount;
				return GAIA::True;
			}

			/*!
				@brief Get public key.
			 
			 	@param n [in] Used for saving RSA-N digit of public key.
			 
			 	@param e [in] Used for saving RSA-E digit of public key.
			 
			 	@param nMaxNSize [in] Specify the max size of parameter n in bytes.
			 
			 	@param nMaxESize [in] Specify the max size of parameter e in bytes.
			 
			 	@return If get public key successfully, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL get_public_key(GAIA::CH* n, GAIA::CH* e, GAIA::N32 nMaxNSize = 1024, GAIA::N32 nMaxESize = 1024)
			{
				GAST(n != GNIL);
				GAST(e != GNIL);
				GAST(nMaxNSize > 0);
				GAST(nMaxESize > 0);
				if(m_sBitCount == 0)
					return GAIA::False;
				if(mpi_write_string(&m_rsa.N, 10, n, &nMaxNSize) != 0)
					return GAIA::False;
				if(mpi_write_string(&m_rsa.E, 10, e, &nMaxESize) != 0)
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief Set private key.
			 
			 	@param n [in] Specify RSA-N digit of private key.
			 
			 	@param d [in] Specify RSA-D digit of private key.
			 
			 	@param sBitCount [in] Specify RSA-BitCount.
			 
			 	@return If set private key successfully, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL set_private_key(const GAIA::CH* n, const GAIA::CH* d, GAIA::NUM sBitCount = DEFAULT_BIT_COUNT)
			{
				GAST(!GAIA::ALGO::gstremp(n));
				GAST(!GAIA::ALGO::gstremp(d));
				if(mpi_read_string(&m_rsa.N, 10, n) != 0)
					return GAIA::False;
				if(mpi_read_string(&m_rsa.D, 10, d) != 0)
					return GAIA::False;
				m_rsa.len = sBitCount / 8;
				m_sBitCount = sBitCount;
				return GAIA::True;
			}

			/*!
				@brief Get private key.
			 
			 	@param n [out] Used for saving RSA-N digit of private key.
			 
			 	@param d [out] Used for saving RSA-D digit of private key.
			 
			 	@param nMaxNSize [in] Specify the max size of parameter n in bytes.
			 
			 	@param nMaxDSize [in] Specify the max size of parameter d in bytes.
			 
			 	@return If get private key successfully, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL get_private_key(GAIA::CH* n, GAIA::CH* d, GAIA::N32 nMaxNSize = 1024, GAIA::N32 nMaxDSize = 1024)
			{
				GAST(n != GNIL);
				GAST(d != GNIL);
				GAST(nMaxNSize > 0);
				GAST(nMaxDSize > 0);
				if(m_sBitCount == 0)
					return GAIA::False;
				if(mpi_write_string(&m_rsa.N, 10, n, &nMaxNSize) != 0)
					return GAIA::False;
				if(mpi_write_string(&m_rsa.D, 10, d, &nMaxDSize) != 0)
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief Check public key.
			 
			 	@return If public key is available, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL check_public_key()
			{
				return rsa_check_pubkey(&m_rsa) == 0;
			}

			/*!
				@brief Check private key.
			 
			 	@return If private key is available, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL check_private_key()
			{
				return rsa_check_privkey(&m_rsa) == 0;
			}

			/*!
				@brief Check public and private key.
			 
			 	@return If the public key and private key is all available, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL check_key()
			{
				if(!this->check_public_key())
					return GAIA::False;
				if(!this->check_private_key())
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief Crypt data.
			 
			 	@param pData [in] Specify the source data which will be encrypted.
			 
			 	@param sDataLen [in] Specify the length of parameter pData in bytes.
			 
			 	@param pResult [out] Used for saving the encrypted data.
			 
			 	@param nMaxResultSize [in] Specify the length of parameter pResult in bytes. Default is 128.
			 
			 	@param bPKCS1 [in] Specify enable or disable PKCS1 mode. Default is GAIA::True.
			 
			 	@return Return the length of the result in chars.
			*/
			GINL GAIA::NUM encrypt(const GAIA::GVOID* pData, GAIA::NUM sDataLen, GAIA::GVOID* pResult, GAIA::N32 nMaxResultSize = 128, GAIA::BL bPKCS1 = GAIA::True)
			{
				GAST(pData != GNIL);
				GAST(pResult != GNIL);
				GAST(pData != pResult);
				if(nMaxResultSize < m_sBitCount / 8)
					return GINVALID;
				if(pData == pResult)
					return GINVALID;
				if(bPKCS1)
				{
					if(rsa_pkcs1_encrypt(&m_rsa, rsa_rand, &m_lcg, RSA_PUBLIC, sDataLen, (const GAIA::U8*)pData, (GAIA::U8*)pResult) == 0)
						return m_sBitCount / 8;
					return GINVALID;
				}
				else
				{
					if(rsa_public(&m_rsa, (const GAIA::U8*)pData, (GAIA::U8*)pResult) == 0)
						return m_sBitCount / 8;
					return GINVALID;
				}
			}

			/*!
				@brief Decrypt data.
			 
			 	@param pData [in] Specify the source data which will be decrypted.
			 
			 	@param pResult [out] Used for saving the decrypted data.
			 
			 	@param nMaxResultSize [in] Specify the length of parameter pResult in bytes. Default is 128.
			 
			 	@param bPKCS1 [in] Specify enable or disable PKCS1 mode. Default is GAIA::True.
			 
			 	@return Return the length of the result in chars.
			*/
			GINL GAIA::NUM decrypt(const GAIA::GVOID* pData, GAIA::GVOID* pResult, GAIA::N32 nMaxResultSize = 128, GAIA::BL bPKCS1 = GAIA::True)
			{
				GAST(pData != GNIL);
				GAST(pResult != GNIL);
				GAST(pData != pResult);
				if(pData == pResult)
					return GINVALID;
				if(bPKCS1)
				{
					GAIA::N32 nResultSize;
					if(rsa_pkcs1_decrypt(&m_rsa, RSA_PRIVATE, &nResultSize, (const GAIA::U8*)pData, (GAIA::U8*)pResult, nMaxResultSize) == 0)
						return nResultSize;
					return GINVALID;
				}
				else
				{
					if(rsa_private(&m_rsa, (const GAIA::U8*)pData, (GAIA::U8*)pResult) == 0)
						return GAIA::ALGO::gstrlen((GAIA::CH*)pResult);
					return GINVALID;
				}
			}
		private:
			GAIA::NUM m_sBitCount;
			rsa_context m_rsa;
			GAIA::MATH::RandomLCG m_lcg;
		};
	}
}

#endif
