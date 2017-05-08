#ifndef		__GAIA_DIGIT_MD5_H__
#define		__GAIA_DIGIT_MD5_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace DIGIT
	{
		static const GAIA::U8 MD5_PADDING[64] =
		{
			0x80, 
			0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, 0, 0, 0
		};

		/*!
		 	@brief Stream based MD5 algorithm class.
		*/
		class MD5 : public GAIA::Base
		{
		public:
			/*!
			 	@brief MD5 context class.
			*/
			class Context : public GAIA::Base
			{
			public:
				/*!
				 	@brief Constructor.
				*/
				GINL Context(){}
				
				/*!
				 	@brief Copy constructor.
				*/
				GINL Context(const Context& src){this->operator = (src);}
				
				/*!
				 	@brief Reset current context to the default value.
				*/
				GINL GAIA::GVOID reset()
				{
					count[0] = count[1] = 0;
					state[0] = 0x67452301;
					state[1] = 0xefcdab89;
					state[2] = 0x98badcfe;
					state[3] = 0x10325476;
				}
				
				/*!
				 	@brief Check current context is changed or not.
				 
				 	@return If changed, return GAIA::True, or will return GAIA::False.
				*/
				GINL GAIA::BL check() const
				{
					if(count[0] != 0)
						return GAIA::True;
					if(count[1] != 0)
						return GAIA::True;
					if(state[0] != 0x67452301)
						return GAIA::True;
					if(state[1] != 0xefcdab89)
						return GAIA::True;
					if(state[2] != 0x98badcfe)
						return GAIA::True;
					if(state[3] != 0x10325476)
						return GAIA::True;
					return GAIA::False;
				}
				
				/*!
				 	@brief Deep copy context from another one.
				 
				 	@return Return current object.
				*/
				GINL Context& operator = (const Context& src)
				{
					GAIA::ALGO::gmemcpy(state, src.state, sizeof(state));
					GAIA::ALGO::gmemcpy(count, src.count, sizeof(count));
					GAIA::ALGO::gmemcpy(buffer, src.buffer, sizeof(buffer));
					return *this;
				}
			public:
				GAIA::U32 state[4];
				GAIA::U32 count[2];
				GAIA::U8 buffer[64];
			};

		public:
			/*!
			 	@brief Constructor.
			*/
			GINL MD5(){this->reset();}
			
			/*!
			 	@brief Reset MD5 context.
			 
			 	@remarks After this function call, the object will be equal to a new allocated MD5 object.
			*/
			GINL GAIA::GVOID reset(){m_ctx.reset();}
			
			/*!
			 	@brief Check MD5 context is changed or not.
			 
			 	@return If MD5 context changed(calculate any data MD5), return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL check() const{return m_ctx.check();}
			
			/*!
			 	@brief Set MD5 context.
			 
			 	@param ctx [in] Specify the new MD5 context.
			*/
			GINL GAIA::GVOID context(const Context& ctx){m_ctx = ctx;}
			
			/*!
			 	@brief Get MD5 context.
			 
			 	@return Return MD5 context of current object.
			*/
			GINL const Context& context() const{return m_ctx;}
			
			/*!
			 	@brief Calculate binary data's MD5.
			 
			 	@param p [in] Specify a binary data buffer.
			 
			 	@param size [in] Specify the size of parameter p in bytes.
			*/
			GINL GAIA::GVOID update(const GAIA::U8* p, GAIA::U32 size)
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return;
				GAST(size > 0);
				if(size <= 0)
					return;

				GAIA::U32 i;
				GAIA::U32 index = (GAIA::U32)((m_ctx.count[0] >> 3) & 0x3F);
				if((m_ctx.count[0] += (size << 3)) < (size << 3))
					m_ctx.count[1]++;
				m_ctx.count[1] += (size >> 29);
				GAIA::U32 uPartLen = 64 - index;
				if(size >= uPartLen)
				{
					GAIA::ALGO::gmemcpy(&m_ctx.buffer[index], p, uPartLen);
					this->transform(m_ctx.state, m_ctx.buffer);
					for(i = uPartLen; i + 63 < size; i += 64)
						this->transform(m_ctx.state, &p[i]);
					index = 0;
				}
				else
					i = 0;
				if(size - i > 0)
					GAIA::ALGO::gmemcpy(&m_ctx.buffer[index], &p[i], size - i);
			}
			
			/*!
			 	@brief Calculate data type's MD5.
			 
			 	@param t [in] Specify a data type to calculate.
			*/
			template<typename _DataType> GAIA::GVOID update(const _DataType& t){this->update((const GAIA::U8*)&t, sizeof(t));}
			
			/*!
			 	@brief Get MD5 result.
			 
			 	@param res [out] Used for saving the MD5 result.
			 		The length of this parameter must above equal 16 bytes.
			*/
			GINL GAIA::GVOID result(GAIA::U8 res[16])
			{
				// Backup.
				Context old_ctx = m_ctx;

				// Calculate result.
				GAIA::U8 bits[8];
				this->encode(bits, m_ctx.count, 8);
				GAIA::U32 index = (GAIA::U32)((m_ctx.count[0] >> 3) & 0x3f);
				GAIA::U32 padLen = (index < 56) ? (56 - index) : (120 - index);
				this->update(MD5_PADDING, padLen);
				this->update(bits, 8);
				this->encode(res, m_ctx.state, 16);

				// Restore.
				m_ctx = old_ctx;
			}

		private:
			#define MD5_S11 7
			#define MD5_S12 12
			#define MD5_S13 17
			#define MD5_S14 22
			#define MD5_S21 5
			#define MD5_S22 9
			#define MD5_S23 14
			#define MD5_S24 20
			#define MD5_S31 4
			#define MD5_S32 11
			#define MD5_S33 16
			#define MD5_S34 23
			#define MD5_S41 6
			#define MD5_S42 10
			#define MD5_S43 15
			#define MD5_S44 21

			#define MD5_F(x, y, z) (((x) & (y))| ((~x) & (z)))
			#define MD5_G(x, y, z) (((x) & (z))| ((y) & (~z)))
			#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
			#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))

			#define MD5_ROTATE_LEFT(x, n)(((x) << (n)) | ((x) >> (32 - (n))))

			#define MD5_FF(a, b, c, d, x, s, ac) \
			{\
				(a) += MD5_F((b), (c), (d)) + (x) + (GAIA::U32)(ac);\
				(a) = MD5_ROTATE_LEFT((a), (s));\
				(a) += (b);\
			}

			#define MD5_GG(a, b, c, d, x, s, ac) \
			{\
				(a) += MD5_G((b), (c), (d)) + (x) + (GAIA::U32)(ac);\
				(a) = MD5_ROTATE_LEFT((a), (s));\
				(a) += (b);\
			}

			#define MD5_HH(a, b, c, d, x, s, ac) \
			{\
				(a) += MD5_H((b), (c), (d)) + (x) + (GAIA::U32)(ac);\
				(a) = MD5_ROTATE_LEFT((a), (s));\
				(a) += (b);\
			}

			#define MD5_II(a, b, c, d, x, s, ac) \
			{\
				(a) += MD5_I((b), (c), (d)) + (x) + (GAIA::U32)(ac);\
				(a) = MD5_ROTATE_LEFT((a), (s));\
				(a) += (b);\
			}

		private:
			GINL GAIA::GVOID encode(GAIA::U8* pOutput, const GAIA::U32* pInput, GAIA::N32 len) const
			{
				for(GAIA::U32 i = 0, j = 0; j < (GAIA::U32)len; i++, j += 4)
				{
					pOutput[j] = (GAIA::U8)(pInput[i] & 0xFF);
					pOutput[j + 1] = (GAIA::U8)((pInput[i] >> 8) & 0xFF);
					pOutput[j + 2] = (GAIA::U8)((pInput[i] >> 16) & 0xFF);
					pOutput[j + 3] = (GAIA::U8)((pInput[i] >> 24) & 0xFF);
				}
			}

			GINL GAIA::GVOID decode(GAIA::U32* pOutput, const GAIA::U8* pInput, GAIA::N32 len) const
			{
				for(GAIA::U32 i = 0, j = 0; j < (GAIA::U32)len; i++, j += 4)
				{
					pOutput[i] = ((GAIA::U32)pInput[j])|
					(((GAIA::U32)pInput[j + 1]) << 8)|
					(((GAIA::U32)pInput[j + 2]) << 16)|
					(((GAIA::U32)pInput[j + 3]) << 24);
				}
			}

			GINL GAIA::GVOID transform(GAIA::U32 state[4], const GAIA::U8 block[64]) const
			{
				GAIA::U32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

				this->decode(x, block, 64);

				/* Round 1. */
				MD5_FF(a, b, c, d, x[0], MD5_S11, 0xd76aa478);
				MD5_FF(d, a, b, c, x[1], MD5_S12, 0xe8c7b756);
				MD5_FF(c, d, a, b, x[2], MD5_S13, 0x242070db);
				MD5_FF(b, c, d, a, x[3], MD5_S14, 0xc1bdceee);
				MD5_FF(a, b, c, d, x[4], MD5_S11, 0xf57c0faf);
				MD5_FF(d, a, b, c, x[5], MD5_S12, 0x4787c62a);
				MD5_FF(c, d, a, b, x[6], MD5_S13, 0xa8304613);
				MD5_FF(b, c, d, a, x[7], MD5_S14, 0xfd469501);
				MD5_FF(a, b, c, d, x[8], MD5_S11, 0x698098d8);
				MD5_FF(d, a, b, c, x[9], MD5_S12, 0x8b44f7af);
				MD5_FF(c, d, a, b, x[10], MD5_S13, 0xffff5bb1);
				MD5_FF(b, c, d, a, x[11], MD5_S14, 0x895cd7be);
				MD5_FF(a, b, c, d, x[12], MD5_S11, 0x6b901122);
				MD5_FF(d, a, b, c, x[13], MD5_S12, 0xfd987193);
				MD5_FF(c, d, a, b, x[14], MD5_S13, 0xa679438e);
				MD5_FF(b, c, d, a, x[15], MD5_S14, 0x49b40821);

				/* Round 2. */
				MD5_GG(a, b, c, d, x[1], MD5_S21, 0xf61e2562);
				MD5_GG(d, a, b, c, x[6], MD5_S22, 0xc040b340);
				MD5_GG(c, d, a, b, x[11], MD5_S23, 0x265e5a51);
				MD5_GG(b, c, d, a, x[0], MD5_S24, 0xe9b6c7aa);
				MD5_GG(a, b, c, d, x[5], MD5_S21, 0xd62f105d);
				MD5_GG(d, a, b, c, x[10], MD5_S22, 0x2441453);
				MD5_GG(c, d, a, b, x[15], MD5_S23, 0xd8a1e681);
				MD5_GG(b, c, d, a, x[4], MD5_S24, 0xe7d3fbc8);
				MD5_GG(a, b, c, d, x[9], MD5_S21, 0x21e1cde6);
				MD5_GG(d, a, b, c, x[14], MD5_S22, 0xc33707d6);
				MD5_GG(c, d, a, b, x[3], MD5_S23, 0xf4d50d87);
				MD5_GG(b, c, d, a, x[8], MD5_S24, 0x455a14ed);
				MD5_GG(a, b, c, d, x[13], MD5_S21, 0xa9e3e905);
				MD5_GG(d, a, b, c, x[2], MD5_S22, 0xfcefa3f8);
				MD5_GG(c, d, a, b, x[7], MD5_S23, 0x676f02d9);
				MD5_GG(b, c, d, a, x[12], MD5_S24, 0x8d2a4c8a);

				/* Round 3. */
				MD5_HH(a, b, c, d, x[5], MD5_S31, 0xfffa3942);
				MD5_HH(d, a, b, c, x[8], MD5_S32, 0x8771f681);
				MD5_HH(c, d, a, b, x[11], MD5_S33, 0x6d9d6122);
				MD5_HH(b, c, d, a, x[14], MD5_S34, 0xfde5380c);
				MD5_HH(a, b, c, d, x[1], MD5_S31, 0xa4beea44);
				MD5_HH(d, a, b, c, x[4], MD5_S32, 0x4bdecfa9);
				MD5_HH(c, d, a, b, x[7], MD5_S33, 0xf6bb4b60);
				MD5_HH(b, c, d, a, x[10], MD5_S34, 0xbebfbc70);
				MD5_HH(a, b, c, d, x[13], MD5_S31, 0x289b7ec6);
				MD5_HH(d, a, b, c, x[0], MD5_S32, 0xeaa127fa);
				MD5_HH(c, d, a, b, x[3], MD5_S33, 0xd4ef3085);
				MD5_HH(b, c, d, a, x[6], MD5_S34, 0x4881d05);
				MD5_HH(a, b, c, d, x[9], MD5_S31, 0xd9d4d039);
				MD5_HH(d, a, b, c, x[12], MD5_S32, 0xe6db99e5);
				MD5_HH(c, d, a, b, x[15], MD5_S33, 0x1fa27cf8);
				MD5_HH(b, c, d, a, x[2], MD5_S34, 0xc4ac5665);

				/* Round 4. */
				MD5_II(a, b, c, d, x[0], MD5_S41, 0xf4292244);
				MD5_II(d, a, b, c, x[7], MD5_S42, 0x432aff97);
				MD5_II(c, d, a, b, x[14], MD5_S43, 0xab9423a7);
				MD5_II(b, c, d, a, x[5], MD5_S44, 0xfc93a039);
				MD5_II(a, b, c, d, x[12], MD5_S41, 0x655b59c3);
				MD5_II(d, a, b, c, x[3], MD5_S42, 0x8f0ccc92);
				MD5_II(c, d, a, b, x[10], MD5_S43, 0xffeff47d);
				MD5_II(b, c, d, a, x[1], MD5_S44, 0x85845dd1);
				MD5_II(a, b, c, d, x[8], MD5_S41, 0x6fa87e4f);
				MD5_II(d, a, b, c, x[15], MD5_S42, 0xfe2ce6e0);
				MD5_II(c, d, a, b, x[6], MD5_S43, 0xa3014314);
				MD5_II(b, c, d, a, x[13], MD5_S44, 0x4e0811a1);
				MD5_II(a, b, c, d, x[4], MD5_S41, 0xf7537e82);
				MD5_II(d, a, b, c, x[11], MD5_S42, 0xbd3af235);
				MD5_II(c, d, a, b, x[2], MD5_S43, 0x2ad7d2bb);
				MD5_II(b, c, d, a, x[9], MD5_S44, 0xeb86d391);

				state[0] += a;
				state[1] += b;
				state[2] += c;
				state[3] += d;

				zeromems(x);
			}

		private:
			Context m_ctx;
		};
	}
}

#endif
