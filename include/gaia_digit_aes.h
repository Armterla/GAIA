#ifndef		__GAIA_DIGIT_AES_H__
#define		__GAIA_DIGIT_AES_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64))
#	define AES_SWAP(x) (_lrotl(x, 8) & 0x00FF00FF | _lrotr(x, 8) & 0xFF00FF00)
#	define AES_GETU32(p) AES_SWAP(*((GAIA::U32*)(p)))
#	define AES_PUTU32(ct, st) {*((GAIA::U32 *)(ct)) = AES_SWAP((st));}
#else
#	define AES_GETU32(pt) (((GAIA::U32)(pt)[0] << 24) ^ ((GAIA::U32)(pt)[1] << 16) ^ ((GAIA::U32)(pt)[2] << 8) ^ ((GAIA::U32)(pt)[3]))
#	define AES_PUTU32(ct, st) {(ct)[0] = (GAIA::U8)((st) >> 24); (ct)[1] = (GAIA::U8)((st) >> 16); (ct)[2] = (GAIA::U8)((st) >> 8); (ct)[3] = (GAIA::U8)(st);}
#endif

#define AES_FULL_UNROLL

namespace GAIA
{
	namespace DIGIT
	{
		extern const GAIA::U32 AES_TE0[256];
		extern const GAIA::U32 AES_TE1[256];
		extern const GAIA::U32 AES_TE2[256];
		extern const GAIA::U32 AES_TE3[256];
		extern const GAIA::U32 AES_TD0[256];
		extern const GAIA::U32 AES_TD1[256];
		extern const GAIA::U32 AES_TD2[256];
		extern const GAIA::U32 AES_TD3[256];
		extern const GAIA::U8 AES_TD4[256];
		extern const GAIA::U32 AES_RCON[10];

		class AES
		{
		public:
			GAIA::BL set_key(const GAIA::GVOID* p, GAIA::U32 uSize)
			{
				GAST(uSize == sizeof(m_key));
				if(uSize != sizeof(m_key))
					return GAIA::False;
				GAIA::ALGO::gmemcpy(m_key, p, uSize);
				return GAIA::True;
			}
			GAIA::BL get_key(GAIA::GVOID* p, GAIA::U32 uSize)
			{
				GAST(uSize == sizeof(m_key));
				if(uSize != sizeof(m_key))
					return GAIA::False;
				GAIA::ALGO::gmemcpy(p, m_key, uSize);
				return GAIA::True;
			}
			GAIA::BL build_key(const GAIA::GVOID* p, GAIA::U32 uSize, GAIA::BL bEncoding)
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GAIA::False;
				GAST(uSize == 16 || uSize == 24 || uSize == 32);
				if(uSize != 16 && uSize != 24 && uSize != 32)
					return GAIA::False;
				if(bEncoding)
				{
					m_bEncoding = bEncoding;
					GAIA::N32 i = 0;
					GAIA::U32 temp;
					GAIA::U32* rk = m_key;
					if(uSize == 16)
						m_rounds = 10;
					else if(uSize==24)
						m_rounds = 12;
					else
						m_rounds = 14;
					rk[0] = AES_GETU32(((GAIA::U8*)p));
					rk[1] = AES_GETU32(((GAIA::U8*)p) + 4);
					rk[2] = AES_GETU32(((GAIA::U8*)p) + 8);
					rk[3] = AES_GETU32(((GAIA::U8*)p) + 12);
					if(uSize == 16)
					{
						while(GAIA::ALWAYSTRUE)
						{
							temp = rk[3];
							rk[4] = rk[0] ^
							(AES_TE2[(temp >> 16) & 0xFF] & 0xFF000000) ^
							(AES_TE3[(temp >> 8) & 0xFF] & 0x00FF0000) ^
							(AES_TE0[(temp) & 0xFF] & 0x0000FF00) ^
							(AES_TE1[(temp >> 24)] & 0x000000FF) ^
							AES_RCON[i];
							rk[5] = rk[1] ^ rk[4];
							rk[6] = rk[2] ^ rk[5];
							rk[7] = rk[3] ^ rk[6];
							if(++i == 10)
								return GAIA::True;
							rk += 4;
						}
					}
					rk[4] = AES_GETU32(((GAIA::U8*)p) + 16);
					rk[5] = AES_GETU32(((GAIA::U8*)p) + 20);
					if(uSize == 24)
					{
						while(GAIA::ALWAYSTRUE)
						{
							temp = rk[ 5];
							rk[ 6] = rk[ 0] ^
							(AES_TE2[(temp >> 16) & 0xFF] & 0xFF000000) ^
							(AES_TE3[(temp >> 8) & 0xFF] & 0x00FF0000) ^
							(AES_TE0[(temp) & 0xFF] & 0x0000FF00) ^
							(AES_TE1[(temp >> 24)] & 0x000000FF) ^
							AES_RCON[i];
							rk[ 7] = rk[ 1] ^ rk[ 6];
							rk[ 8] = rk[ 2] ^ rk[ 7];
							rk[ 9] = rk[ 3] ^ rk[ 8];
							if(++i == 8)
								return GAIA::True;
							rk[10] = rk[ 4] ^ rk[ 9];
							rk[11] = rk[ 5] ^ rk[10];
							rk += 6;
						}
					}
					rk[6] = AES_GETU32(((GAIA::U8*)p) + 24);
					rk[7] = AES_GETU32(((GAIA::U8*)p) + 28);
					if(uSize == 32)
					{
						while(1)
						{
							temp = rk[ 7];
							rk[ 8] = rk[ 0] ^
							(AES_TE2[(temp >> 16) & 0xFF] & 0xFF000000) ^
							(AES_TE3[(temp >> 8) & 0xFF] & 0x00FF0000) ^
							(AES_TE0[(temp) & 0xFF] & 0x0000FF00) ^
							(AES_TE1[(temp >> 24)] & 0x000000FF) ^
							AES_RCON[i];
							rk[ 9] = rk[ 1] ^ rk[ 8];
							rk[10] = rk[ 2] ^ rk[ 9];
							rk[11] = rk[ 3] ^ rk[10];
							if(++i == 7)
								return GAIA::True;
							temp = rk[11];
							rk[12] = rk[ 4] ^
							(AES_TE2[(temp >> 24)] & 0xFF000000) ^
							(AES_TE3[(temp >> 16) & 0xFF] & 0x00FF0000) ^
							(AES_TE0[(temp >> 8) & 0xFF] & 0x0000FF00) ^
							(AES_TE1[(temp) & 0xFF] & 0x000000FF);
							rk[13] = rk[ 5] ^ rk[12];
							rk[14] = rk[ 6] ^ rk[13];
							rk[15] = rk[ 7] ^ rk[14];
							rk += 8;
						}
					}
				}
				else
				{
					GAIA::U32 *rk;
					GAIA::N32 i, j;
					GAIA::U32 temp;
					
					if(!this->build_key(p, uSize, GAIA::True))
						return GAIA::False;
					m_bEncoding = bEncoding;
					rk = m_key;

					for (i = 0, j = 4 * m_rounds; i < j; i += 4, j -= 4)
					{
						temp = rk[i]; rk[i] = rk[j]; rk[j] = temp;
						temp = rk[i + 1]; rk[i + 1] = rk[j + 1]; rk[j + 1] = temp;
						temp = rk[i + 2]; rk[i + 2] = rk[j + 2]; rk[j + 2] = temp;
						temp = rk[i + 3]; rk[i + 3] = rk[j + 3]; rk[j + 3] = temp;
					}

					for (i = 1; i < m_rounds; i++)
					{
						rk += 4;
						rk[0] = AES_TD0[AES_TE1[(rk[0] >> 24)] & 0xFF] ^
								AES_TD1[AES_TE1[(rk[0] >> 16) & 0xFF] & 0xFF] ^
								AES_TD2[AES_TE1[(rk[0] >> 8) & 0xFF] & 0xFF] ^
								AES_TD3[AES_TE1[(rk[0]) & 0xFF] & 0xFF];
						rk[1] = AES_TD0[AES_TE1[(rk[1] >> 24)] & 0xFF] ^
								AES_TD1[AES_TE1[(rk[1] >> 16) & 0xFF] & 0xFF] ^
								AES_TD2[AES_TE1[(rk[1] >> 8) & 0xFF] & 0xFF] ^
								AES_TD3[AES_TE1[(rk[1]) & 0xFF] & 0xFF];
						rk[2] = AES_TD0[AES_TE1[(rk[2] >> 24)] & 0xFF] ^
								AES_TD1[AES_TE1[(rk[2] >> 16) & 0xFF] & 0xFF] ^
								AES_TD2[AES_TE1[(rk[2] >> 8) & 0xFF] & 0xFF] ^
								AES_TD3[AES_TE1[(rk[2]) & 0xFF] & 0xFF];
						rk[3] = AES_TD0[AES_TE1[(rk[3] >> 24)] & 0xFF] ^
								AES_TD1[AES_TE1[(rk[3] >> 16) & 0xFF] & 0xFF] ^
								AES_TD2[AES_TE1[(rk[3] >> 8) & 0xFF] & 0xFF] ^
								AES_TD3[AES_TE1[(rk[3]) & 0xFF] & 0xFF];
					}
				}
				return GAIA::True;
			}
			GAIA::GVOID update(const GAIA::GVOID* pSrc, const GAIA::GVOID* pDst, GAIA::U32 uSize)
			{
				if(m_bEncoding)
				{
					const GAIA::U32 *rk;
					GAIA::U32 s0, s1, s2, s3, t0, t1, t2, t3;
				#ifndef AES_FULL_UNROLL
					GAIA::N32 r;
				#endif
					rk = m_key;
					
					s0 = AES_GETU32(((GAIA::U8*)pSrc)) ^ rk[0];
					s1 = AES_GETU32(((GAIA::U8*)pSrc) + 4) ^ rk[1];
					s2 = AES_GETU32(((GAIA::U8*)pSrc) + 8) ^ rk[2];
					s3 = AES_GETU32(((GAIA::U8*)pSrc) + 12) ^ rk[3];
				#ifdef AES_FULL_UNROLL
					t0 = AES_TE0[s0 >> 24] ^ AES_TE1[(s1 >> 16) & 0xFF] ^ AES_TE2[(s2 >> 8) & 0xFF] ^ AES_TE3[s3 & 0xFF] ^ rk[4];
					t1 = AES_TE0[s1 >> 24] ^ AES_TE1[(s2 >> 16) & 0xFF] ^ AES_TE2[(s3 >> 8) & 0xFF] ^ AES_TE3[s0 & 0xFF] ^ rk[5];
					t2 = AES_TE0[s2 >> 24] ^ AES_TE1[(s3 >> 16) & 0xFF] ^ AES_TE2[(s0 >> 8) & 0xFF] ^ AES_TE3[s1 & 0xFF] ^ rk[6];
					t3 = AES_TE0[s3 >> 24] ^ AES_TE1[(s0 >> 16) & 0xFF] ^ AES_TE2[(s1 >> 8) & 0xFF] ^ AES_TE3[s2 & 0xFF] ^ rk[7];
					s0 = AES_TE0[t0 >> 24] ^ AES_TE1[(t1 >> 16) & 0xFF] ^ AES_TE2[(t2 >> 8) & 0xFF] ^ AES_TE3[t3 & 0xFF] ^ rk[8];
					s1 = AES_TE0[t1 >> 24] ^ AES_TE1[(t2 >> 16) & 0xFF] ^ AES_TE2[(t3 >> 8) & 0xFF] ^ AES_TE3[t0 & 0xFF] ^ rk[9];
					s2 = AES_TE0[t2 >> 24] ^ AES_TE1[(t3 >> 16) & 0xFF] ^ AES_TE2[(t0 >> 8) & 0xFF] ^ AES_TE3[t1 & 0xFF] ^ rk[10];
					s3 = AES_TE0[t3 >> 24] ^ AES_TE1[(t0 >> 16) & 0xFF] ^ AES_TE2[(t1 >> 8) & 0xFF] ^ AES_TE3[t2 & 0xFF] ^ rk[11];
					t0 = AES_TE0[s0 >> 24] ^ AES_TE1[(s1 >> 16) & 0xFF] ^ AES_TE2[(s2 >> 8) & 0xFF] ^ AES_TE3[s3 & 0xFF] ^ rk[12];
					t1 = AES_TE0[s1 >> 24] ^ AES_TE1[(s2 >> 16) & 0xFF] ^ AES_TE2[(s3 >> 8) & 0xFF] ^ AES_TE3[s0 & 0xFF] ^ rk[13];
					t2 = AES_TE0[s2 >> 24] ^ AES_TE1[(s3 >> 16) & 0xFF] ^ AES_TE2[(s0 >> 8) & 0xFF] ^ AES_TE3[s1 & 0xFF] ^ rk[14];
					t3 = AES_TE0[s3 >> 24] ^ AES_TE1[(s0 >> 16) & 0xFF] ^ AES_TE2[(s1 >> 8) & 0xFF] ^ AES_TE3[s2 & 0xFF] ^ rk[15];
					s0 = AES_TE0[t0 >> 24] ^ AES_TE1[(t1 >> 16) & 0xFF] ^ AES_TE2[(t2 >> 8) & 0xFF] ^ AES_TE3[t3 & 0xFF] ^ rk[16];
					s1 = AES_TE0[t1 >> 24] ^ AES_TE1[(t2 >> 16) & 0xFF] ^ AES_TE2[(t3 >> 8) & 0xFF] ^ AES_TE3[t0 & 0xFF] ^ rk[17];
					s2 = AES_TE0[t2 >> 24] ^ AES_TE1[(t3 >> 16) & 0xFF] ^ AES_TE2[(t0 >> 8) & 0xFF] ^ AES_TE3[t1 & 0xFF] ^ rk[18];
					s3 = AES_TE0[t3 >> 24] ^ AES_TE1[(t0 >> 16) & 0xFF] ^ AES_TE2[(t1 >> 8) & 0xFF] ^ AES_TE3[t2 & 0xFF] ^ rk[19];
					t0 = AES_TE0[s0 >> 24] ^ AES_TE1[(s1 >> 16) & 0xFF] ^ AES_TE2[(s2 >> 8) & 0xFF] ^ AES_TE3[s3 & 0xFF] ^ rk[20];
					t1 = AES_TE0[s1 >> 24] ^ AES_TE1[(s2 >> 16) & 0xFF] ^ AES_TE2[(s3 >> 8) & 0xFF] ^ AES_TE3[s0 & 0xFF] ^ rk[21];
					t2 = AES_TE0[s2 >> 24] ^ AES_TE1[(s3 >> 16) & 0xFF] ^ AES_TE2[(s0 >> 8) & 0xFF] ^ AES_TE3[s1 & 0xFF] ^ rk[22];
					t3 = AES_TE0[s3 >> 24] ^ AES_TE1[(s0 >> 16) & 0xFF] ^ AES_TE2[(s1 >> 8) & 0xFF] ^ AES_TE3[s2 & 0xFF] ^ rk[23];
					s0 = AES_TE0[t0 >> 24] ^ AES_TE1[(t1 >> 16) & 0xFF] ^ AES_TE2[(t2 >> 8) & 0xFF] ^ AES_TE3[t3 & 0xFF] ^ rk[24];
					s1 = AES_TE0[t1 >> 24] ^ AES_TE1[(t2 >> 16) & 0xFF] ^ AES_TE2[(t3 >> 8) & 0xFF] ^ AES_TE3[t0 & 0xFF] ^ rk[25];
					s2 = AES_TE0[t2 >> 24] ^ AES_TE1[(t3 >> 16) & 0xFF] ^ AES_TE2[(t0 >> 8) & 0xFF] ^ AES_TE3[t1 & 0xFF] ^ rk[26];
					s3 = AES_TE0[t3 >> 24] ^ AES_TE1[(t0 >> 16) & 0xFF] ^ AES_TE2[(t1 >> 8) & 0xFF] ^ AES_TE3[t2 & 0xFF] ^ rk[27];
					t0 = AES_TE0[s0 >> 24] ^ AES_TE1[(s1 >> 16) & 0xFF] ^ AES_TE2[(s2 >> 8) & 0xFF] ^ AES_TE3[s3 & 0xFF] ^ rk[28];
					t1 = AES_TE0[s1 >> 24] ^ AES_TE1[(s2 >> 16) & 0xFF] ^ AES_TE2[(s3 >> 8) & 0xFF] ^ AES_TE3[s0 & 0xFF] ^ rk[29];
					t2 = AES_TE0[s2 >> 24] ^ AES_TE1[(s3 >> 16) & 0xFF] ^ AES_TE2[(s0 >> 8) & 0xFF] ^ AES_TE3[s1 & 0xFF] ^ rk[30];
					t3 = AES_TE0[s3 >> 24] ^ AES_TE1[(s0 >> 16) & 0xFF] ^ AES_TE2[(s1 >> 8) & 0xFF] ^ AES_TE3[s2 & 0xFF] ^ rk[31];
					s0 = AES_TE0[t0 >> 24] ^ AES_TE1[(t1 >> 16) & 0xFF] ^ AES_TE2[(t2 >> 8) & 0xFF] ^ AES_TE3[t3 & 0xFF] ^ rk[32];
					s1 = AES_TE0[t1 >> 24] ^ AES_TE1[(t2 >> 16) & 0xFF] ^ AES_TE2[(t3 >> 8) & 0xFF] ^ AES_TE3[t0 & 0xFF] ^ rk[33];
					s2 = AES_TE0[t2 >> 24] ^ AES_TE1[(t3 >> 16) & 0xFF] ^ AES_TE2[(t0 >> 8) & 0xFF] ^ AES_TE3[t1 & 0xFF] ^ rk[34];
					s3 = AES_TE0[t3 >> 24] ^ AES_TE1[(t0 >> 16) & 0xFF] ^ AES_TE2[(t1 >> 8) & 0xFF] ^ AES_TE3[t2 & 0xFF] ^ rk[35];
					t0 = AES_TE0[s0 >> 24] ^ AES_TE1[(s1 >> 16) & 0xFF] ^ AES_TE2[(s2 >> 8) & 0xFF] ^ AES_TE3[s3 & 0xFF] ^ rk[36];
					t1 = AES_TE0[s1 >> 24] ^ AES_TE1[(s2 >> 16) & 0xFF] ^ AES_TE2[(s3 >> 8) & 0xFF] ^ AES_TE3[s0 & 0xFF] ^ rk[37];
					t2 = AES_TE0[s2 >> 24] ^ AES_TE1[(s3 >> 16) & 0xFF] ^ AES_TE2[(s0 >> 8) & 0xFF] ^ AES_TE3[s1 & 0xFF] ^ rk[38];
					t3 = AES_TE0[s3 >> 24] ^ AES_TE1[(s0 >> 16) & 0xFF] ^ AES_TE2[(s1 >> 8) & 0xFF] ^ AES_TE3[s2 & 0xFF] ^ rk[39];
					if(m_rounds > 10)
					{
						s0 = AES_TE0[t0 >> 24] ^ AES_TE1[(t1 >> 16) & 0xFF] ^ AES_TE2[(t2 >> 8) & 0xFF] ^ AES_TE3[t3 & 0xFF] ^ rk[40];
						s1 = AES_TE0[t1 >> 24] ^ AES_TE1[(t2 >> 16) & 0xFF] ^ AES_TE2[(t3 >> 8) & 0xFF] ^ AES_TE3[t0 & 0xFF] ^ rk[41];
						s2 = AES_TE0[t2 >> 24] ^ AES_TE1[(t3 >> 16) & 0xFF] ^ AES_TE2[(t0 >> 8) & 0xFF] ^ AES_TE3[t1 & 0xFF] ^ rk[42];
						s3 = AES_TE0[t3 >> 24] ^ AES_TE1[(t0 >> 16) & 0xFF] ^ AES_TE2[(t1 >> 8) & 0xFF] ^ AES_TE3[t2 & 0xFF] ^ rk[43];
						t0 = AES_TE0[s0 >> 24] ^ AES_TE1[(s1 >> 16) & 0xFF] ^ AES_TE2[(s2 >> 8) & 0xFF] ^ AES_TE3[s3 & 0xFF] ^ rk[44];
						t1 = AES_TE0[s1 >> 24] ^ AES_TE1[(s2 >> 16) & 0xFF] ^ AES_TE2[(s3 >> 8) & 0xFF] ^ AES_TE3[s0 & 0xFF] ^ rk[45];
						t2 = AES_TE0[s2 >> 24] ^ AES_TE1[(s3 >> 16) & 0xFF] ^ AES_TE2[(s0 >> 8) & 0xFF] ^ AES_TE3[s1 & 0xFF] ^ rk[46];
						t3 = AES_TE0[s3 >> 24] ^ AES_TE1[(s0 >> 16) & 0xFF] ^ AES_TE2[(s1 >> 8) & 0xFF] ^ AES_TE3[s2 & 0xFF] ^ rk[47];
						if(m_rounds > 12)
						{
							s0 = AES_TE0[t0 >> 24] ^ AES_TE1[(t1 >> 16) & 0xFF] ^ AES_TE2[(t2 >> 8) & 0xFF] ^ AES_TE3[t3 & 0xFF] ^ rk[48];
							s1 = AES_TE0[t1 >> 24] ^ AES_TE1[(t2 >> 16) & 0xFF] ^ AES_TE2[(t3 >> 8) & 0xFF] ^ AES_TE3[t0 & 0xFF] ^ rk[49];
							s2 = AES_TE0[t2 >> 24] ^ AES_TE1[(t3 >> 16) & 0xFF] ^ AES_TE2[(t0 >> 8) & 0xFF] ^ AES_TE3[t1 & 0xFF] ^ rk[50];
							s3 = AES_TE0[t3 >> 24] ^ AES_TE1[(t0 >> 16) & 0xFF] ^ AES_TE2[(t1 >> 8) & 0xFF] ^ AES_TE3[t2 & 0xFF] ^ rk[51];
							t0 = AES_TE0[s0 >> 24] ^ AES_TE1[(s1 >> 16) & 0xFF] ^ AES_TE2[(s2 >> 8) & 0xFF] ^ AES_TE3[s3 & 0xFF] ^ rk[52];
							t1 = AES_TE0[s1 >> 24] ^ AES_TE1[(s2 >> 16) & 0xFF] ^ AES_TE2[(s3 >> 8) & 0xFF] ^ AES_TE3[s0 & 0xFF] ^ rk[53];
							t2 = AES_TE0[s2 >> 24] ^ AES_TE1[(s3 >> 16) & 0xFF] ^ AES_TE2[(s0 >> 8) & 0xFF] ^ AES_TE3[s1 & 0xFF] ^ rk[54];
							t3 = AES_TE0[s3 >> 24] ^ AES_TE1[(s0 >> 16) & 0xFF] ^ AES_TE2[(s1 >> 8) & 0xFF] ^ AES_TE3[s2 & 0xFF] ^ rk[55];
						}
					}
					rk += m_rounds << 2;
				#else
					r = m_rounds >> 1;
					for (;;)
					{
						t0 = AES_TE0[(s0 >> 24)] ^ AES_TE1[(s1 >> 16) & 0xFF] ^ AES_TE2[(s2 >> 8) & 0xFF] ^ AES_TE3[(s3) & 0xFF] ^ rk[4];
						t1 = AES_TE0[(s1 >> 24)] ^ AES_TE1[(s2 >> 16) & 0xFF] ^ AES_TE2[(s3 >> 8) & 0xFF] ^ AES_TE3[(s0) & 0xFF] ^ rk[5];
						t2 = AES_TE0[(s2 >> 24)] ^ AES_TE1[(s3 >> 16) & 0xFF] ^ AES_TE2[(s0 >> 8) & 0xFF] ^ AES_TE3[(s1) & 0xFF] ^ rk[6];
						t3 = AES_TE0[(s3 >> 24)] ^ AES_TE1[(s0 >> 16) & 0xFF] ^ AES_TE2[(s1 >> 8) & 0xFF] ^ AES_TE3[(s2) & 0xFF] ^ rk[7];
						
						rk += 8;
						if(--r == 0)
							break;
						
						s0 = AES_TE0[(t0 >> 24)] ^ AES_TE1[(t1 >> 16) & 0xFF] ^ AES_TE2[(t2 >> 8) & 0xFF] ^ AES_TE3[(t3) & 0xFF] ^ rk[0];
						s1 = AES_TE0[(t1 >> 24)] ^ AES_TE1[(t2 >> 16) & 0xFF] ^ AES_TE2[(t3 >> 8) & 0xFF] ^ AES_TE3[(t0) & 0xFF] ^ rk[1];
						s2 = AES_TE0[(t2 >> 24)] ^ AES_TE1[(t3 >> 16) & 0xFF] ^ AES_TE2[(t0 >> 8) & 0xFF] ^ AES_TE3[(t1) & 0xFF] ^ rk[2];
						s3 = AES_TE0[(t3 >> 24)] ^ AES_TE1[(t0 >> 16) & 0xFF] ^ AES_TE2[(t1 >> 8) & 0xFF] ^ AES_TE3[(t2) & 0xFF] ^ rk[3];
					}
				#endif
					s0 = (AES_TE2[(t0 >> 24)] & 0xFF000000) ^
							(AES_TE3[(t1 >> 16) & 0xFF] & 0x00FF0000) ^
							(AES_TE0[(t2 >> 8) & 0xFF] & 0x0000FF00) ^
							(AES_TE1[(t3) & 0xFF] & 0x000000FF) ^ rk[0];
					AES_PUTU32(((GAIA::U8*)pDst), s0);

					s1 = (AES_TE2[(t1 >> 24)] & 0xFF000000) ^
							(AES_TE3[(t2 >> 16) & 0xFF] & 0x00FF0000) ^
							(AES_TE0[(t3 >> 8) & 0xFF] & 0x0000FF00) ^
							(AES_TE1[(t0) & 0xFF] & 0x000000FF) ^ rk[1];
					AES_PUTU32(((GAIA::U8*)pDst) + 4, s1);

					s2 = (AES_TE2[(t2 >> 24)] & 0xFF000000) ^
							(AES_TE3[(t3 >> 16) & 0xFF] & 0x00FF0000) ^
							(AES_TE0[(t0 >> 8) & 0xFF] & 0x0000FF00) ^
							(AES_TE1[(t1) & 0xFF] & 0x000000FF) ^ rk[2];
					AES_PUTU32(((GAIA::U8*)pDst) + 8, s2);

					s3 = (AES_TE2[(t3 >> 24)] & 0xFF000000) ^
							(AES_TE3[(t0 >> 16) & 0xFF] & 0x00FF0000) ^
							(AES_TE0[(t1 >> 8) & 0xFF] & 0x0000FF00) ^
							(AES_TE1[(t2) & 0xFF] & 0x000000FF) ^ rk[3];
					AES_PUTU32(((GAIA::U8*)pDst) + 12, s3);
				}
				else
				{
					const GAIA::U32 *rk;
					GAIA::U32 s0, s1, s2, s3, t0, t1, t2, t3;
				#ifndef AES_FULL_UNROLL
					GAIA::N32 r;
				#endif
					rk = m_key;
					
					s0 = AES_GETU32(((GAIA::U8*)pSrc)) ^ rk[0];
					s1 = AES_GETU32(((GAIA::U8*)pSrc) + 4) ^ rk[1];
					s2 = AES_GETU32(((GAIA::U8*)pSrc) + 8) ^ rk[2];
					s3 = AES_GETU32(((GAIA::U8*)pSrc) + 12) ^ rk[3];
				#ifdef AES_FULL_UNROLL
					t0 = AES_TD0[s0 >> 24] ^ AES_TD1[(s3 >> 16) & 0xFF] ^ AES_TD2[(s2 >> 8) & 0xFF] ^ AES_TD3[s1 & 0xFF] ^ rk[4];
					t1 = AES_TD0[s1 >> 24] ^ AES_TD1[(s0 >> 16) & 0xFF] ^ AES_TD2[(s3 >> 8) & 0xFF] ^ AES_TD3[s2 & 0xFF] ^ rk[5];
					t2 = AES_TD0[s2 >> 24] ^ AES_TD1[(s1 >> 16) & 0xFF] ^ AES_TD2[(s0 >> 8) & 0xFF] ^ AES_TD3[s3 & 0xFF] ^ rk[6];
					t3 = AES_TD0[s3 >> 24] ^ AES_TD1[(s2 >> 16) & 0xFF] ^ AES_TD2[(s1 >> 8) & 0xFF] ^ AES_TD3[s0 & 0xFF] ^ rk[7];
					s0 = AES_TD0[t0 >> 24] ^ AES_TD1[(t3 >> 16) & 0xFF] ^ AES_TD2[(t2 >> 8) & 0xFF] ^ AES_TD3[t1 & 0xFF] ^ rk[8];
					s1 = AES_TD0[t1 >> 24] ^ AES_TD1[(t0 >> 16) & 0xFF] ^ AES_TD2[(t3 >> 8) & 0xFF] ^ AES_TD3[t2 & 0xFF] ^ rk[9];
					s2 = AES_TD0[t2 >> 24] ^ AES_TD1[(t1 >> 16) & 0xFF] ^ AES_TD2[(t0 >> 8) & 0xFF] ^ AES_TD3[t3 & 0xFF] ^ rk[10];
					s3 = AES_TD0[t3 >> 24] ^ AES_TD1[(t2 >> 16) & 0xFF] ^ AES_TD2[(t1 >> 8) & 0xFF] ^ AES_TD3[t0 & 0xFF] ^ rk[11];
					t0 = AES_TD0[s0 >> 24] ^ AES_TD1[(s3 >> 16) & 0xFF] ^ AES_TD2[(s2 >> 8) & 0xFF] ^ AES_TD3[s1 & 0xFF] ^ rk[12];
					t1 = AES_TD0[s1 >> 24] ^ AES_TD1[(s0 >> 16) & 0xFF] ^ AES_TD2[(s3 >> 8) & 0xFF] ^ AES_TD3[s2 & 0xFF] ^ rk[13];
					t2 = AES_TD0[s2 >> 24] ^ AES_TD1[(s1 >> 16) & 0xFF] ^ AES_TD2[(s0 >> 8) & 0xFF] ^ AES_TD3[s3 & 0xFF] ^ rk[14];
					t3 = AES_TD0[s3 >> 24] ^ AES_TD1[(s2 >> 16) & 0xFF] ^ AES_TD2[(s1 >> 8) & 0xFF] ^ AES_TD3[s0 & 0xFF] ^ rk[15];
					s0 = AES_TD0[t0 >> 24] ^ AES_TD1[(t3 >> 16) & 0xFF] ^ AES_TD2[(t2 >> 8) & 0xFF] ^ AES_TD3[t1 & 0xFF] ^ rk[16];
					s1 = AES_TD0[t1 >> 24] ^ AES_TD1[(t0 >> 16) & 0xFF] ^ AES_TD2[(t3 >> 8) & 0xFF] ^ AES_TD3[t2 & 0xFF] ^ rk[17];
					s2 = AES_TD0[t2 >> 24] ^ AES_TD1[(t1 >> 16) & 0xFF] ^ AES_TD2[(t0 >> 8) & 0xFF] ^ AES_TD3[t3 & 0xFF] ^ rk[18];
					s3 = AES_TD0[t3 >> 24] ^ AES_TD1[(t2 >> 16) & 0xFF] ^ AES_TD2[(t1 >> 8) & 0xFF] ^ AES_TD3[t0 & 0xFF] ^ rk[19];
					t0 = AES_TD0[s0 >> 24] ^ AES_TD1[(s3 >> 16) & 0xFF] ^ AES_TD2[(s2 >> 8) & 0xFF] ^ AES_TD3[s1 & 0xFF] ^ rk[20];
					t1 = AES_TD0[s1 >> 24] ^ AES_TD1[(s0 >> 16) & 0xFF] ^ AES_TD2[(s3 >> 8) & 0xFF] ^ AES_TD3[s2 & 0xFF] ^ rk[21];
					t2 = AES_TD0[s2 >> 24] ^ AES_TD1[(s1 >> 16) & 0xFF] ^ AES_TD2[(s0 >> 8) & 0xFF] ^ AES_TD3[s3 & 0xFF] ^ rk[22];
					t3 = AES_TD0[s3 >> 24] ^ AES_TD1[(s2 >> 16) & 0xFF] ^ AES_TD2[(s1 >> 8) & 0xFF] ^ AES_TD3[s0 & 0xFF] ^ rk[23];
					s0 = AES_TD0[t0 >> 24] ^ AES_TD1[(t3 >> 16) & 0xFF] ^ AES_TD2[(t2 >> 8) & 0xFF] ^ AES_TD3[t1 & 0xFF] ^ rk[24];
					s1 = AES_TD0[t1 >> 24] ^ AES_TD1[(t0 >> 16) & 0xFF] ^ AES_TD2[(t3 >> 8) & 0xFF] ^ AES_TD3[t2 & 0xFF] ^ rk[25];
					s2 = AES_TD0[t2 >> 24] ^ AES_TD1[(t1 >> 16) & 0xFF] ^ AES_TD2[(t0 >> 8) & 0xFF] ^ AES_TD3[t3 & 0xFF] ^ rk[26];
					s3 = AES_TD0[t3 >> 24] ^ AES_TD1[(t2 >> 16) & 0xFF] ^ AES_TD2[(t1 >> 8) & 0xFF] ^ AES_TD3[t0 & 0xFF] ^ rk[27];
					t0 = AES_TD0[s0 >> 24] ^ AES_TD1[(s3 >> 16) & 0xFF] ^ AES_TD2[(s2 >> 8) & 0xFF] ^ AES_TD3[s1 & 0xFF] ^ rk[28];
					t1 = AES_TD0[s1 >> 24] ^ AES_TD1[(s0 >> 16) & 0xFF] ^ AES_TD2[(s3 >> 8) & 0xFF] ^ AES_TD3[s2 & 0xFF] ^ rk[29];
					t2 = AES_TD0[s2 >> 24] ^ AES_TD1[(s1 >> 16) & 0xFF] ^ AES_TD2[(s0 >> 8) & 0xFF] ^ AES_TD3[s3 & 0xFF] ^ rk[30];
					t3 = AES_TD0[s3 >> 24] ^ AES_TD1[(s2 >> 16) & 0xFF] ^ AES_TD2[(s1 >> 8) & 0xFF] ^ AES_TD3[s0 & 0xFF] ^ rk[31];
					s0 = AES_TD0[t0 >> 24] ^ AES_TD1[(t3 >> 16) & 0xFF] ^ AES_TD2[(t2 >> 8) & 0xFF] ^ AES_TD3[t1 & 0xFF] ^ rk[32];
					s1 = AES_TD0[t1 >> 24] ^ AES_TD1[(t0 >> 16) & 0xFF] ^ AES_TD2[(t3 >> 8) & 0xFF] ^ AES_TD3[t2 & 0xFF] ^ rk[33];
					s2 = AES_TD0[t2 >> 24] ^ AES_TD1[(t1 >> 16) & 0xFF] ^ AES_TD2[(t0 >> 8) & 0xFF] ^ AES_TD3[t3 & 0xFF] ^ rk[34];
					s3 = AES_TD0[t3 >> 24] ^ AES_TD1[(t2 >> 16) & 0xFF] ^ AES_TD2[(t1 >> 8) & 0xFF] ^ AES_TD3[t0 & 0xFF] ^ rk[35];
					t0 = AES_TD0[s0 >> 24] ^ AES_TD1[(s3 >> 16) & 0xFF] ^ AES_TD2[(s2 >> 8) & 0xFF] ^ AES_TD3[s1 & 0xFF] ^ rk[36];
					t1 = AES_TD0[s1 >> 24] ^ AES_TD1[(s0 >> 16) & 0xFF] ^ AES_TD2[(s3 >> 8) & 0xFF] ^ AES_TD3[s2 & 0xFF] ^ rk[37];
					t2 = AES_TD0[s2 >> 24] ^ AES_TD1[(s1 >> 16) & 0xFF] ^ AES_TD2[(s0 >> 8) & 0xFF] ^ AES_TD3[s3 & 0xFF] ^ rk[38];
					t3 = AES_TD0[s3 >> 24] ^ AES_TD1[(s2 >> 16) & 0xFF] ^ AES_TD2[(s1 >> 8) & 0xFF] ^ AES_TD3[s0 & 0xFF] ^ rk[39];
					if(m_rounds > 10)
					{
						s0 = AES_TD0[t0 >> 24] ^ AES_TD1[(t3 >> 16) & 0xFF] ^ AES_TD2[(t2 >> 8) & 0xFF] ^ AES_TD3[t1 & 0xFF] ^ rk[40];
						s1 = AES_TD0[t1 >> 24] ^ AES_TD1[(t0 >> 16) & 0xFF] ^ AES_TD2[(t3 >> 8) & 0xFF] ^ AES_TD3[t2 & 0xFF] ^ rk[41];
						s2 = AES_TD0[t2 >> 24] ^ AES_TD1[(t1 >> 16) & 0xFF] ^ AES_TD2[(t0 >> 8) & 0xFF] ^ AES_TD3[t3 & 0xFF] ^ rk[42];
						s3 = AES_TD0[t3 >> 24] ^ AES_TD1[(t2 >> 16) & 0xFF] ^ AES_TD2[(t1 >> 8) & 0xFF] ^ AES_TD3[t0 & 0xFF] ^ rk[43];
						t0 = AES_TD0[s0 >> 24] ^ AES_TD1[(s3 >> 16) & 0xFF] ^ AES_TD2[(s2 >> 8) & 0xFF] ^ AES_TD3[s1 & 0xFF] ^ rk[44];
						t1 = AES_TD0[s1 >> 24] ^ AES_TD1[(s0 >> 16) & 0xFF] ^ AES_TD2[(s3 >> 8) & 0xFF] ^ AES_TD3[s2 & 0xFF] ^ rk[45];
						t2 = AES_TD0[s2 >> 24] ^ AES_TD1[(s1 >> 16) & 0xFF] ^ AES_TD2[(s0 >> 8) & 0xFF] ^ AES_TD3[s3 & 0xFF] ^ rk[46];
						t3 = AES_TD0[s3 >> 24] ^ AES_TD1[(s2 >> 16) & 0xFF] ^ AES_TD2[(s1 >> 8) & 0xFF] ^ AES_TD3[s0 & 0xFF] ^ rk[47];
						if(m_rounds > 12)
						{
							s0 = AES_TD0[t0 >> 24] ^ AES_TD1[(t3 >> 16) & 0xFF] ^ AES_TD2[(t2 >> 8) & 0xFF] ^ AES_TD3[t1 & 0xFF] ^ rk[48];
							s1 = AES_TD0[t1 >> 24] ^ AES_TD1[(t0 >> 16) & 0xFF] ^ AES_TD2[(t3 >> 8) & 0xFF] ^ AES_TD3[t2 & 0xFF] ^ rk[49];
							s2 = AES_TD0[t2 >> 24] ^ AES_TD1[(t1 >> 16) & 0xFF] ^ AES_TD2[(t0 >> 8) & 0xFF] ^ AES_TD3[t3 & 0xFF] ^ rk[50];
							s3 = AES_TD0[t3 >> 24] ^ AES_TD1[(t2 >> 16) & 0xFF] ^ AES_TD2[(t1 >> 8) & 0xFF] ^ AES_TD3[t0 & 0xFF] ^ rk[51];
							t0 = AES_TD0[s0 >> 24] ^ AES_TD1[(s3 >> 16) & 0xFF] ^ AES_TD2[(s2 >> 8) & 0xFF] ^ AES_TD3[s1 & 0xFF] ^ rk[52];
							t1 = AES_TD0[s1 >> 24] ^ AES_TD1[(s0 >> 16) & 0xFF] ^ AES_TD2[(s3 >> 8) & 0xFF] ^ AES_TD3[s2 & 0xFF] ^ rk[53];
							t2 = AES_TD0[s2 >> 24] ^ AES_TD1[(s1 >> 16) & 0xFF] ^ AES_TD2[(s0 >> 8) & 0xFF] ^ AES_TD3[s3 & 0xFF] ^ rk[54];
							t3 = AES_TD0[s3 >> 24] ^ AES_TD1[(s2 >> 16) & 0xFF] ^ AES_TD2[(s1 >> 8) & 0xFF] ^ AES_TD3[s0 & 0xFF] ^ rk[55];
						}
					}
					rk += m_rounds << 2;
				#else
					r = m_rounds >> 1;
					for (;;)
					{
						t0 = AES_TD0[(s0 >> 24)] ^ AES_TD1[(s3 >> 16) & 0xFF] ^ AES_TD2[(s2 >> 8) & 0xFF] ^ AES_TD3[(s1) & 0xFF] ^ rk[4];
						t1 = AES_TD0[(s1 >> 24)] ^ AES_TD1[(s0 >> 16) & 0xFF] ^ AES_TD2[(s3 >> 8) & 0xFF] ^ AES_TD3[(s2) & 0xFF] ^ rk[5];
						t2 = AES_TD0[(s2 >> 24)] ^ AES_TD1[(s1 >> 16) & 0xFF] ^ AES_TD2[(s0 >> 8) & 0xFF] ^ AES_TD3[(s3) & 0xFF] ^ rk[6];
						t3 = AES_TD0[(s3 >> 24)] ^ AES_TD1[(s2 >> 16) & 0xFF] ^ AES_TD2[(s1 >> 8) & 0xFF] ^ AES_TD3[(s0) & 0xFF] ^ rk[7];
						
						rk += 8;
						if(--r == 0)
							break;
						
						s0 = AES_TD0[(t0 >> 24)] ^ AES_TD1[(t3 >> 16) & 0xFF] ^ AES_TD2[(t2 >> 8) & 0xFF] ^ AES_TD3[(t1) & 0xFF] ^ rk[0];
						s1 = AES_TD0[(t1 >> 24)] ^ AES_TD1[(t0 >> 16) & 0xFF] ^ AES_TD2[(t3 >> 8) & 0xFF] ^ AES_TD3[(t2) & 0xFF] ^ rk[1];
						s2 = AES_TD0[(t2 >> 24)] ^ AES_TD1[(t1 >> 16) & 0xFF] ^ AES_TD2[(t0 >> 8) & 0xFF] ^ AES_TD3[(t3) & 0xFF] ^ rk[2];
						s3 = AES_TD0[(t3 >> 24)] ^ AES_TD1[(t2 >> 16) & 0xFF] ^ AES_TD2[(t1 >> 8) & 0xFF] ^ AES_TD3[(t0) & 0xFF] ^ rk[3];
					}
				#endif
					s0 = ((GAIA::U32)AES_TD4[(t0 >> 24)] << 24) ^
						((GAIA::U32)AES_TD4[(t3 >> 16) & 0xFF] << 16) ^
						((GAIA::U32)AES_TD4[(t2 >> 8) & 0xFF] << 8) ^
						((GAIA::U32)AES_TD4[(t1) & 0xFF]) ^ rk[0];
					AES_PUTU32(((GAIA::U8*)pDst), s0);

					s1 = ((GAIA::U32)AES_TD4[(t1 >> 24)] << 24) ^
						((GAIA::U32)AES_TD4[(t0 >> 16) & 0xFF] << 16) ^
						((GAIA::U32)AES_TD4[(t3 >> 8) & 0xFF] << 8) ^
						((GAIA::U32)AES_TD4[(t2) & 0xFF]) ^ rk[1];
					AES_PUTU32(((GAIA::U8*)pDst) + 4, s1);

					s2 = ((GAIA::U32)AES_TD4[(t2 >> 24)] << 24) ^
						((GAIA::U32)AES_TD4[(t1 >> 16) & 0xFF] << 16) ^
						((GAIA::U32)AES_TD4[(t0 >> 8) & 0xFF] << 8) ^
						((GAIA::U32)AES_TD4[(t3) & 0xFF])^
						rk[2];
					AES_PUTU32(((GAIA::U8*)pDst) + 8, s2);

					s3 = ((GAIA::U32)AES_TD4[(t3 >> 24)] << 24) ^
						((GAIA::U32)AES_TD4[(t2 >> 16) & 0xFF] << 16) ^
						((GAIA::U32)AES_TD4[(t1 >> 8) & 0xFF] << 8) ^
						((GAIA::U32)AES_TD4[(t0) & 0xFF]) ^ rk[3];
					AES_PUTU32(((GAIA::U8*)pDst) + 12, s3);
				}
			}
		private:
			GAIA::U32 m_key[60];
			GAIA::N32 m_rounds;
			GAIA::BL m_bEncoding;
		};
	}
}

#endif
