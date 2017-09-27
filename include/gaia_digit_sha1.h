#ifndef		__GAIA_DIGIT_SHA1_H__
#define		__GAIA_DIGIT_SHA1_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace DIGIT
	{
		/*!
		 	@brief Stream based SHA1 algorithme class.
		*/
		class SHA1 : public GAIA::Base
		{
		public:
			
			/*!
			 	@brief SHA1 context class.
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
					res[0] = 0x67452301;
					res[1] = 0xEFCDAB89;
					res[2] = 0x98BADCFE;
					res[3] = 0x10325476;
					res[4] = 0xC3D2E1F0;
					l = 0;
					h = 0;
					zeromems(msg);
					msgsize = 0;
					zeromems(msgbuf);
					msgbufsize = 0;
				}

				/*!
				 	@brief Check the context is changed or not.
				 
				 	@return If changed, return GAIA::True, or will return GAIA::False.
				*/
				GINL GAIA::BL check() const
				{
					if(res[0] != 0x67452301)
						return GAIA::True;
					if(res[1] != 0xEFCDAB89)
						return GAIA::True;
					if(res[2] != 0x98BADCFE)
						return GAIA::True;
					if(res[3] != 0x10325476)
						return GAIA::True;
					if(res[4] != 0xC3D2E1F0)
						return GAIA::True;
					if(l != 0)
						return GAIA::True;
					if(h != 0)
						return GAIA::True;
					for(GAIA::NUM x = 0; x < sizeof(msg); ++x)
					{
						if(msg[x] != 0)
							return GAIA::True;
					}
					if(msgsize != 0)
						return GAIA::True;
					for(GAIA::NUM x = 0; x < sizeof(msgbuf); ++x)
					{
						if(msgbuf[x] != 0)
							return GAIA::True;
					}
					if(msgbufsize != 0)
						return GAIA::True;
					return GAIA::False;
				}
				
				/*!
				 	@brief Deep copy context from another one.
				 
				 	@return Return current object.
				*/
				GINL Context& operator = (const Context& src)
				{
					GAIA::ALGO::gmemcpy(res, src.res, sizeof(res));
					l = src.l;
					h = src.h;
					GAIA::ALGO::gmemcpy(msg, src.msg, sizeof(msg));
					msgsize = src.msgsize;
					GAIA::ALGO::gmemcpy(msgbuf, src.msgbuf, sizeof(msgbuf));
					msgbufsize = src.msgbufsize;
					return *this;
				}
			public:
				GAIA::U32 res[5];
				GAIA::U32 l;
				GAIA::U32 h;
				GAIA::U8 msg[64];
				GAIA::U32 msgsize;
				GAIA::U8 msgbuf[64];
				GAIA::U32 msgbufsize;
			};
		public:
			/*!
			 	@brief Constructor.
			*/
			GINL SHA1(){this->reset();}
			
			/*!
			 	@brief Reset SHA1 context.
			 
			 	@remarks After this function call, the object will be equal to a new allocated MD5 object.
			*/
			GINL GAIA::GVOID reset(){m_ctx.reset();}
			
			/*!
			 	@brief Check SHA1 context is changed or not.
			 
			 	@return If SHA1 context changed(calculate any data SHA1), return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL check() const{return m_ctx.check();}
			
			/*!
			 	@brief Set SHA1 context.
			 
			 	@param ctx [in] Specify the new SHA1 context.
			*/
			GINL GAIA::GVOID context(const Context& ctx){m_ctx = ctx;}
			
			/*!
			 	@brief Get SHA1 context.
			 
			 	@return Return SHA1 context of current object.
			*/
			GINL const Context& context() const{return m_ctx;}
			
			/*!
			 	@brief Calculate binary data's SHA1.
			 
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
				if(m_ctx.msgbufsize + size < sizeof(m_ctx.msg))
				{
					GAIA::ALGO::gmemcpy(m_ctx.msgbuf + m_ctx.msgbufsize, p, size);
					m_ctx.msgbufsize += size;
					return;
				}
				if(m_ctx.msgbufsize > 0)
				{
					GAIA::U32 remainsize = sizeof(m_ctx.msg) - m_ctx.msgbufsize;
					GAIA::ALGO::gmemcpy(m_ctx.msg, m_ctx.msgbuf, m_ctx.msgbufsize);
					GAIA::ALGO::gmemcpy(m_ctx.msg + m_ctx.msgbufsize, p, remainsize);
					this->update_msg_size(sizeof(m_ctx.msg));
					this->process_msg_block();
					this->update_msg_size(0);
					m_ctx.msgbufsize = 0;
					p += remainsize;
					GAST(size >= remainsize);
					size -= remainsize;
				}
				while(size >= sizeof(m_ctx.msg))
				{
					GAIA::ALGO::gmemcpy(m_ctx.msg, p, sizeof(m_ctx.msg));
					this->update_msg_size(sizeof(m_ctx.msg));
					this->process_msg_block();
					this->update_msg_size(0);
					p += sizeof(m_ctx.msg);
					size -= sizeof(m_ctx.msg);
				}
				if(size > 0)
				{
					GAST(size < sizeof(m_ctx.msg));
					GAST(m_ctx.msgbufsize == 0);
					m_ctx.msgbufsize = size;
					GAIA::ALGO::gmemcpy(m_ctx.msgbuf, p, size);
				}
			}
			
			/*!
			 	@brief Calculate data type's SHA1.
			 
			 	@param t [in] Specify a data type to calculate.
			*/
			template<typename _DataType> GAIA::GVOID update(const _DataType& t){this->update((const GAIA::U8*)&t, sizeof(t));}
			
			/*!
			 	@brief Get SHA1 result.
			 
			 	@param res [out] Used for saving the SHA1 result.
			 		The length of this parameter must above equal 20 bytes.
			*/
			GINL GAIA::GVOID result(GAIA::U8 res[20])
			{				
				// Backup.
				Context old_ctx = m_ctx;

				// Calculate msgbuf data.
				if(m_ctx.msgbufsize > 0)
					GAIA::ALGO::gmemcpy(m_ctx.msg, m_ctx.msgbuf, m_ctx.msgbufsize);
				if(sizeof(m_ctx.msg) - m_ctx.msgbufsize > 0)
					GAIA::ALGO::gmemset(m_ctx.msg + m_ctx.msgbufsize, 0, sizeof(m_ctx.msg) - m_ctx.msgbufsize);
				this->update_msg_size(m_ctx.msgbufsize);
				this->pad_msg();

				// Copy result.
				GAIA::ALGO::gmemcpy(res, m_ctx.res, sizeof(m_ctx.res));
				for(GAIA::NUM x = 0; x < 20; x += 4)
				{
					GAIA::ALGO::swap(res[x + 0], res[x + 3]);
					GAIA::ALGO::swap(res[x + 1], res[x + 2]);
				}

				// Restore.
				m_ctx = old_ctx;
			}
		private:
			GINL GAIA::GVOID update_msg_size(GAIA::U32 size)
			{
				m_ctx.msgsize = size;
				if((m_ctx.l += (size << 3)) < (size << 3))
					++m_ctx.h;
				m_ctx.h += (size >> 29);
			}
			GINL GAIA::GVOID process_msg_block()
			{
				static const GAIA::U32 K[] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};
				GAIA::U32 t;
				GAIA::U32 W[80];
				GAIA::U32 A, B, C, D, E;
				for(t = 0; t < 16; ++t)
				{
					W[t] = ((GAIA::U32)m_ctx.msg[t * 4]) << 24;
					W[t] |= ((GAIA::U32)m_ctx.msg[t * 4 + 1]) << 16;
					W[t] |= ((GAIA::U32)m_ctx.msg[t * 4 + 2]) << 8;
					W[t] |= ((GAIA::U32)m_ctx.msg[t * 4 + 3]);
				}
				for(t = 16; t < 80; ++t)
					W[t] = this->circular_shift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
				A = m_ctx.res[0];
				B = m_ctx.res[1];
				C = m_ctx.res[2];
				D = m_ctx.res[3];
				E = m_ctx.res[4];
				for(t = 0; t < 20; ++t)
				{
					GAIA::U32 temp = this->circular_shift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
					temp &= 0xFFFFFFFF;
					E = D;
					D = C;
					C = this->circular_shift(30, B);
					B = A;
					A = temp;
				}
				for(t = 20; t < 40; ++t)
				{
					GAIA::U32 temp = this->circular_shift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
					temp &= 0xFFFFFFFF;
					E = D;
					D = C;
					C = this->circular_shift(30, B);
					B = A;
					A = temp;
				}
				for(t = 40; t < 60; ++t)
				{
					GAIA::U32 temp = this->circular_shift(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
					temp &= 0xFFFFFFFF;
					E = D;
					D = C;
					C = this->circular_shift(30, B);
					B = A;
					A = temp;
				}
				for(t = 60; t < 80; ++t)
				{
					GAIA::U32 temp = this->circular_shift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
					temp &= 0xFFFFFFFF;
					E = D;
					D = C;
					C = this->circular_shift(30, B);
					B = A;
					A = temp;
				}
				m_ctx.res[0] = (m_ctx.res[0] + A) & 0xFFFFFFFF;
				m_ctx.res[1] = (m_ctx.res[1] + B) & 0xFFFFFFFF;
				m_ctx.res[2] = (m_ctx.res[2] + C) & 0xFFFFFFFF;
				m_ctx.res[3] = (m_ctx.res[3] + D) & 0xFFFFFFFF;
				m_ctx.res[4] = (m_ctx.res[4] + E) & 0xFFFFFFFF;
				
				m_ctx.msgsize = 0;
			}
			GINL GAIA::GVOID pad_msg()
			{
				if(m_ctx.msgsize > 55)
				{
					m_ctx.msg[m_ctx.msgsize++] = 0x80;
					while(m_ctx.msgsize < sizeof(m_ctx.msg))
						m_ctx.msg[m_ctx.msgsize++] = 0;
					this->process_msg_block();
					while(m_ctx.msgsize < 56)
						m_ctx.msg[m_ctx.msgsize++] = 0;
				}
				else
				{
					m_ctx.msg[m_ctx.msgsize++] = 0x80;
					while(m_ctx.msgsize < 56)
						m_ctx.msg[m_ctx.msgsize++] = 0;
				}
				m_ctx.msg[56] = (GAIA::U8)((m_ctx.h >> 24) & 0xFF);
				m_ctx.msg[57] = (GAIA::U8)((m_ctx.h >> 16) & 0xFF);
				m_ctx.msg[58] = (GAIA::U8)((m_ctx.h >> 8) & 0xFF);
				m_ctx.msg[59] = (GAIA::U8)((m_ctx.h) & 0xFF);
				m_ctx.msg[60] = (GAIA::U8)((m_ctx.l >> 24) & 0xFF);
				m_ctx.msg[61] = (GAIA::U8)((m_ctx.l >> 16) & 0xFF);
				m_ctx.msg[62] = (GAIA::U8)((m_ctx.l >> 8) & 0xFF);
				m_ctx.msg[63] = (GAIA::U8)((m_ctx.l) & 0xFF);
				this->process_msg_block();
			}
			GINL GAIA::U32 circular_shift(GAIA::U32 bits, GAIA::U32 word)
			{
				return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32 - bits));
			}
		private:
			Context m_ctx;
		};
	}
}

#endif
