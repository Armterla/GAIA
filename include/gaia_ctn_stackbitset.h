#ifndef		__GAIA_CTN_STACKBITSET_H__
#define		__GAIA_CTN_STACKBITSET_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace CTN
	{
		#define GAIA_STACKBITSET_SRC (m_bits[index / 8])
		#define GAIA_STACKBITSET_CUR (1 << (index % 8))
		template<typename _SizeType, _SizeType _Size> class BasicStackBitset : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
		public:
			static const _SizeType _size = _Size;
		public:
			typedef BasicStackBitset<_SizeType, _Size> __MyType;
		public:
			GINL BasicStackBitset(){this->clear();}
			GINL BasicStackBitset(const __MyType& src){this->operator = (src);}
			GINL GAIA::GVOID clear(){GAIA::ALGO::gmemset(this->fptr(), 0, _Size);}
			GINL GAIA::BL empty() const{return GAIA::False;}
			GINL GAIA::BL zero() const{return GAIA::ALGO::gmemchr(this->fptr(), 0, _Size) == 0;}
			GINL GAIA::BL one() const{return GAIA::ALGO::gmemchr(this->fptr(), 0xFF, _Size) == 0;}
			GINL _SizeType size() const{return this->capacity();}
			GINL _SizeType capacity() const{return _Size * 8;}
			GINL GAIA::BL exist(const _SizeType& index) const{return (GAIA_STACKBITSET_SRC & GAIA_STACKBITSET_CUR) != 0;}
			GINL GAIA::GVOID set(const _SizeType& index){GAIA_STACKBITSET_SRC |= GAIA_STACKBITSET_CUR;}
			GINL GAIA::GVOID reset(const _SizeType& index){GAIA_STACKBITSET_SRC &= ~GAIA_STACKBITSET_CUR;}
			GINL GAIA::GVOID inverse(const _SizeType& index){if(this->exist(index)) this->reset(index); else this->set(index);}
			GINL GAIA::U8* fptr(){return m_bits;}
			GINL GAIA::U8* bptr(){return &m_bits[_Size - 1];}
			GINL const GAIA::U8* fptr() const{return m_bits;}
			GINL const GAIA::U8* bptr() const{return &m_bits[_Size - 1];}
			GINL __MyType& operator = (const __MyType& src){GAST(&src != this); GAIA::ALGO::gmemcpy(this->fptr(), src.fptr(), _Size); return *this;}
			GINL __MyType& operator |= (const __MyType& src){for(_SizeType x = 0; x < _Size; ++x) this->fptr()[x] |= src.fptr()[x]; return *this;}
			GINL __MyType& operator &= (const __MyType& src){for(_SizeType x = 0; x < _Size; ++x) this->fptr()[x] &= src.fptr()[x]; return *this;}
			GINL __MyType& operator ^= (const __MyType& src){for(_SizeType x = 0; x < _Size; ++x) this->fptr()[x] ^= src.fptr()[x]; return *this;}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				return GAIA::ALGO::gmemcmp(this->fptr(), src.fptr(), _Size);
			}
			GCLASS_COMPARE_BYCOMPARE(__MyType)
			GINL __MyType operator ~ () const
			{
				__MyType ret = *this;
				for(_SizeType x = 0; x < _Size; ++x)
					ret.fptr()[x] ^= (GAIA::U8)GINVALID;
				return ret;
			}
			GINL GAIA::BL operator[](const _SizeType& index) const{return this->exist(index);}
		private:
			GAIA::U8 m_bits[_Size];
		};
		template<GAIA::NUM _Size> class StackBitset : public BasicStackBitset<GAIA::NUM, _Size>{public:};
	}
}

#endif
