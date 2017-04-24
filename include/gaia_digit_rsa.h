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

		class RSA
		{
		public:
			/*!
				@brief Constructor.
			*/
			RSA()
			{
				m_sBitCount = 1024;
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
			*/
			GINL GAIA::GVOID reset()
			{
				rsa_free(&m_rsa);
				rsa_init(&m_rsa, RSA_PKCS_V15, 0);
			}

			/*!
				@brief Get RSA crypt bit count.
			*/
			GINL GAIA::NUM bit_count()
			{
				return m_sBitCount;
			}

			/*!
				@brief Build a key.
			*/
			GINL GAIA::BL build_key(GAIA::NUM sBitCount = 1024, GAIA::NUM sExp = 65537, GAIA::U32 uRandomSeed = (GAIA::U32)(GAIA::TIME::tick_time() % 10000))
			{
				m_sBitCount = sBitCount;
				return rsa_gen_key(&m_rsa, rsa_rand, &m_lcg, sBitCount, sExp) == 0;
			}

			/*!
				@brief Set public key.
			*/
			GINL GAIA::BL set_public_key(const GAIA::CH* m, const GAIA::CH* e, GAIA::NUM sBitCount = 1024)
			{
				GAST(!GAIA::ALGO::gstremp(m));
				GAST(!GAIA::ALGO::gstremp(e));
				if(mpi_read_string(&m_rsa.N, 10, m) != 0)
					return GAIA::False;
				if(mpi_read_string(&m_rsa.E, 10, e) != 0)
					return GAIA::False;
				m_rsa.len = sBitCount / 8;
				m_sBitCount = sBitCount;
				return GAIA::True;
			}

			/*!
				@brief Get public key.
			*/
			GINL GAIA::BL get_public_key(GAIA::CH* m, GAIA::CH* e, GAIA::N32 nMaxMSize = 1024, GAIA::N32 nMaxESize = 1024)
			{
				GAST(m != GNIL);
				GAST(e != GNIL);
				GAST(nMaxMSize > 0);
				GAST(nMaxESize > 0);
				if(mpi_write_string(&m_rsa.N, 10, m, &nMaxMSize) != 0)
					return GAIA::False;
				if(mpi_write_string(&m_rsa.E, 10, e, &nMaxESize) != 0)
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief Set private key.
			*/
			GINL GAIA::BL set_private_key(const GAIA::CH* m, const GAIA::CH* e, GAIA::NUM sBitCount = 1024)
			{
				GAST(!GAIA::ALGO::gstremp(m));
				GAST(!GAIA::ALGO::gstremp(e));
				if(mpi_read_string(&m_rsa.N, 10, m) != 0)
					return GAIA::False;
				if(mpi_read_string(&m_rsa.D, 10, e) != 0)
					return GAIA::False;
				m_rsa.len = sBitCount / 8;
				m_sBitCount = sBitCount;
				return GAIA::True;
			}

			/*!
				@brief Get private key.
			*/
			GINL GAIA::BL get_private_key(GAIA::CH* m, GAIA::CH* e, GAIA::N32 nMaxMSize = 1024, GAIA::N32 nMaxESize = 1024)
			{
				GAST(m != GNIL);
				GAST(e != GNIL);
				GAST(nMaxMSize > 0);
				GAST(nMaxESize > 0);
				if(mpi_write_string(&m_rsa.N, 10, m, &nMaxMSize) != 0)
					return GAIA::False;
				if(mpi_write_string(&m_rsa.D, 10, e, &nMaxESize) != 0)
					return GAIA::False;
				return GAIA::True;
			}

			/*!
				@brief Check public key.
			*/
			GINL GAIA::BL check_public_key()
			{
				return rsa_check_pubkey(&m_rsa) == 0;
			}

			/*!
				@brief Check private key.
			*/
			GINL GAIA::BL check_private_key()
			{
				return rsa_check_privkey(&m_rsa) == 0;
			}

			/*!
				@brief Check public and private key.
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
