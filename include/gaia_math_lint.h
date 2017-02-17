#ifndef		__GAIA_MATH_LINT_H__
#define		__GAIA_MATH_LINT_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_random_lcg.h"
#include "gaia_ctn_charsstring.h"
#include "bignum.h"

namespace GAIA
{
	namespace MATH
	{
		static GINL GAIA::N32 lint_random(GAIA::GVOID* p)
		{
			GAST(p != GNIL);
			GAIA::MATH::RandomLCG* pLCG = (GAIA::MATH::RandomLCG*)p;
			return (GAIA::N32)pLCG->random_u32();
		}

		/*!
			@brief Big integer calculate(The precision is infinite).
		*/
		class LInt : public GAIA::Base
		{
		public:
			GINL LInt(){mpi_init(&m_v, GNIL);}
			GINL LInt(const LInt& src){mpi_init(&m_v, GNIL); this->operator = (src);}
			GINL ~LInt(){mpi_free(&m_v, GNIL);}

			// Get parameters.
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL GAIA::NUM size() const
			{
				if(m_v.p == GNIL)
					return 0;
				return mpi_size(&m_v);
			}
			GINL GAIA::NUM least_bits_count() const{return mpi_lsb(&m_v);}
			GINL GAIA::NUM most_bits_count() const{return mpi_msb(&m_v);}

			// Calculate with self type.
			GINL LInt& operator = (const LInt& src)
			{
				if(mpi_copy(&m_v, &src.m_v) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator += (const LInt& src)
			{
				if(mpi_add_mpi(&m_v, &m_v, &src.m_v) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator -= (const LInt& src)
			{
				if(mpi_sub_mpi(&m_v, &m_v, &src.m_v) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator *= (const LInt& src)
			{
				if(mpi_mul_mpi(&m_v, &m_v, &src.m_v) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator /= (const LInt& src)
			{
				LInt vMod;
				if(mpi_div_mpi(&m_v, &vMod.m_v, &m_v, &src.m_v) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator %= (const LInt& src)
			{
				if(mpi_mod_mpi(&m_v, &m_v, &src.m_v) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt operator + (const LInt& src) const
			{
				LInt v = *this;
				v += src;
				return v;
			}
			GINL LInt operator - (const LInt& src) const
			{
				LInt v = *this;
				v -= src;
				return v;
			}
			GINL LInt operator * (const LInt& src) const
			{
				LInt v = *this;
				v *= src;
				return v;
			}
			GINL LInt operator / (const LInt& src) const
			{
				LInt v = *this;
				v /= src;
				return v;
			}
			GINL LInt operator % (const LInt& src) const
			{
				LInt v = *this;
				v %= src;
				return v;
			}

			// Calculate with other type.
			GINL LInt& operator = (GAIA::NUM src)
			{
				if(mpi_lset(&m_v, src) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator += (GAIA::NUM src)
			{
				if(mpi_add_int(&m_v, &m_v, src) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator -= (GAIA::NUM src)
			{
				if(mpi_sub_int(&m_v, &m_v, src) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator *= (GAIA::NUM src)
			{
				if(mpi_mul_int(&m_v, &m_v, src) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator /= (GAIA::NUM src)
			{
				LInt vMod;
				if(mpi_div_int(&m_v, &vMod.m_v, &m_v, src) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& operator %= (GAIA::NUM src)
			{
				t_int tres;
				if(mpi_mod_int(&tres, &m_v, src) != 0)
					GTHROW(Illegal);
				this->fromint((GAIA::NUM)tres);
				return *this;
			}
			GINL LInt operator + (GAIA::NUM src) const
			{
				LInt v = *this;
				v += src;
				return v;
			}
			GINL LInt operator - (GAIA::NUM src) const
			{
				LInt v = *this;
				v -= src;
				return v;
			}
			GINL LInt operator * (GAIA::NUM src) const
			{
				LInt v = *this;
				v %= src;
				return v;
			}
			GINL LInt operator / (GAIA::NUM src) const
			{
				LInt v = *this;
				v /= src;
				return v;
			}
			GINL LInt operator % (GAIA::NUM src) const
			{
				LInt v = *this;
				v %= src;
				return v;
			}

			//
			GINL LInt& operator ++ ()
			{
				(*this) += 1;
				return *this;
			}
			GINL LInt& operator -- ()
			{
				(*this) -= 1;
				return *this;
			}
			GINL LInt operator ++ (GAIA::N32 n)
			{
				LInt v = *this;
				(*this) += 1;
				return v;
			}
			GINL LInt operator -- (GAIA::N32 n)
			{
				LInt v = *this;
				(*this) -= 1;
				return v;
			}

			// Shift.
			GINL LInt& shiftleft(GAIA::NUM sShift)
			{
				GAST(sShift >= 0);
				if(mpi_shift_l(&m_v, sShift) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt& shiftright(GAIA::NUM sShift)
			{
				GAST(sShift >= 0);
				if(mpi_shift_r(&m_v, sShift) != 0)
					GTHROW(Illegal);
				return *this;
			}
			GINL LInt operator << (GAIA::NUM sShift) const
			{
				LInt v = *this;
				v.shiftleft(sShift);
				return v;
			}
			GINL LInt operator >> (GAIA::NUM sShift) const
			{
				LInt v = *this;
				v.shiftright(sShift);
				return v;
			}

			// Compare with self type.
			GINL GAIA::NUM compare(const LInt& src) const{return mpi_cmp_mpi(&m_v, &src.m_v);}
			GINL GAIA::BL operator == (const LInt& src) const{return this->compare(src) == 0;}
			GINL GAIA::BL operator != (const LInt& src) const{return this->compare(src) != 0;}
			GINL GAIA::BL operator >= (const LInt& src) const{return this->compare(src) >= 0;}
			GINL GAIA::BL operator <= (const LInt& src) const{return this->compare(src) <= 0;}
			GINL GAIA::BL operator > (const LInt& src) const{return this->compare(src) > 0;}
			GINL GAIA::BL operator < (const LInt& src) const{return this->compare(src) < 0;}

			// Compare with other type.
			GINL GAIA::NUM compare(GAIA::NUM src) const{return mpi_cmp_int(&m_v, src);}
			GINL GAIA::BL operator == (GAIA::NUM src) const{return this->compare(src) == 0;}
			GINL GAIA::BL operator != (GAIA::NUM src) const{return this->compare(src) != 0;}
			GINL GAIA::BL operator >= (GAIA::NUM src) const{return this->compare(src) >= 0;}
			GINL GAIA::BL operator <= (GAIA::NUM src) const{return this->compare(src) <= 0;}
			GINL GAIA::BL operator > (GAIA::NUM src) const{return this->compare(src) > 0;}
			GINL GAIA::BL operator < (GAIA::NUM src) const{return this->compare(src) < 0;}

			// Convert to other type.
			GINL GAIA::BL fromstring(const GAIA::CH* psz, GAIA::NUM sLen = GINVALID, GAIA::NUM sRadix = 10)
			{
				GPCHR_TRUE_RET(GAIA::ALGO::gstremp(psz), GAIA::False);
				GPCHR_ZERO_RET(sLen, GAIA::False);
				GPCHR_BELOWEQUALZERO_RET(sRadix, GNIL);
				if(sLen == GINVALID)
				{
					if(mpi_read_string(&m_v, sRadix, psz) != 0)
						GTHROW(Illegal);
				}
				else
				{
					GAIA::CTN::ACharsString str;
					str.assign(psz, sLen);
					if(mpi_read_string(&m_v, sRadix, str.fptr()) != 0)
						GTHROW(Illegal);
				}
				return GAIA::True;
			}
			GINL GAIA::CH* tostring(GAIA::CH* psz, GAIA::NUM sLen, GAIA::NUM sRadix = 10, GAIA::NUM* pResultSize = GNIL) const
			{
				GPCHR_NULL_RET(psz, GNIL);
				if(sLen != GINVALID)
					GPCHR_BELOWEQUALZERO_RET(sLen, GNIL);
				GPCHR_BELOWEQUALZERO_RET(sRadix, GNIL);
				if(sLen == GINVALID)
					sLen = GAIA::ALGO::gstrlen(psz);
				GAIA::NUM sLenTemp = sLen;
				if(mpi_write_string(&m_v, sRadix, psz, &sLenTemp) != 0)
					GTHROW(Illegal);
				if(pResultSize != GNIL)
					*pResultSize = sLenTemp;
				return psz;
			}
			GINL GAIA::BL frombuffer(const GAIA::GVOID* p, GAIA::NUM sBufferSize)
			{
				GPCHR_NULL_RET(p, GAIA::False);
				GPCHR_BELOWEQUALZERO_RET(sBufferSize, GAIA::False);
				if(mpi_read_binary(&m_v, (const GAIA::U8*)p, sBufferSize) != 0)
					GTHROW(Illegal);
				return GAIA::True;
			}
			GINL GAIA::GVOID* tobuffer(GAIA::GVOID* p, GAIA::NUM sBufferSize) const
			{
				GPCHR_NULL_RET(p, GNIL);
				GPCHR_BELOWEQUALZERO_RET(sBufferSize, GNIL);
				if(mpi_write_binary(&m_v, (GAIA::U8*)p, sBufferSize) != 0)
					GTHROW(Illegal);
				return p;
			}
			GINL GAIA::BL fromint(GAIA::NUM src)
			{
				if(mpi_lset(&m_v, src) != 0)
					GTHROW(Illegal);
				return GAIA::True;
			}
			GINL GAIA::NUM toint(GAIA::BL* pResult) const
			{
				GAIA::CH sz[GAIA::MAXPL];
				GAIA::NUM sResultSize;
				this->tostring(sz, sizeof(sz), 10, &sResultSize);
				if(sResultSize > 11)
					GTHROW(Illegal);
				GAIA::N64 n = GAIA::ALGO::acasts(sz);
				if(n > GAIA::N32MAX || n < GAIA::N32MIN)
					GTHROW(Illegal);
				return (GAIA::NUM)n;
			}
			template<typename _ParamDataType> LInt& operator = (const _ParamDataType& src)
			{
				this->fromint((GAIA::NUM)src);
				return *this;
			}
			template<typename _ParamDataType> LInt& operator = (const _ParamDataType* src)
			{
				this->fromstring(src, GINVALID, 10);
				return *this;
			}

			// Advance arithmetics.
			GINL GAIA::BL isprime() const
			{
				GAIA::MATH::RandomLCG lcg;
				if(mpi_is_prime((mpi*)&m_v, lint_random, &lcg) == 0)
					return GAIA::True;
				return GAIA::False;
			}
			GINL GAIA::GVOID randomprime(GAIA::NUM sBits)
			{
				GAIA::MATH::RandomLCG lcg;
				if(mpi_gen_prime(&m_v, sBits, 0, lint_random, &lcg) != 0)
					GTHROW(Illegal);
			}
			GINL LInt gcd(const LInt& src) const
			{
				LInt ret;
				if(mpi_gcd(&ret.m_v, &m_v, &src.m_v) != 0)
					GTHROW(Illegal);
				return ret;
			}

		private:
			mpi m_v;
		};
	}
}

#endif
