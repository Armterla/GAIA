#ifndef		__GAIA_CTN_BITSET_H__
#define		__GAIA_CTN_BITSET_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"

namespace GAIA
{
	namespace CTN
	{
		#define GAIA_BITSET_SRC (m_pFront[index / 8])
		#define GAIA_BITSET_CUR (1 << (index % 8))
		template<typename _SizeType, typename _ExtendType> class BasicBitset : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicBitset<_SizeType, _ExtendType> __MyType;
		public:
			GINL BasicBitset(){this->init();}
			GINL BasicBitset(const __MyType& src){this->init(); this->operator = (src);}
			GINL ~BasicBitset(){this->destroy();}
			GINL GAIA::GVOID clear(){if(!this->empty()) GAIA::ALGO::gmemset(this->fptr(), 0, this->buffer_size(this->size()));}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL GAIA::BL nill() const{return m_pFront == GNIL;}
			GINL GAIA::BL zero() const{if(this->empty()) return GAIA::False; return GAIA::ALGO::gmemchr(this->fptr(), 0, this->buffer_size(this->size())) == 0;}
			GINL GAIA::BL one() const{if(this->empty()) return GAIA::False; return GAIA::ALGO::gmemchr(this->fptr(), 0xFF, this->buffer_size(this->size())) == 0;}
			GINL const _SizeType& size() const{return m_size;}
			GINL const _SizeType& capacity() const{return m_capacity;}
			GINL GAIA::GVOID destroy(){if(m_pFront != GNIL){gdel[] m_pFront; m_pFront = GNIL;} m_size = m_capacity = 0;}
			GINL GAIA::U8* fptr(){if(this->empty()) return GNIL; return m_pFront;}
			GINL GAIA::U8* bptr(){if(this->empty()) return GNIL; return &m_pFront[this->buffer_size(this->size()) - 1];}
			GINL const GAIA::U8* fptr() const{if(this->empty()) return GNIL; return m_pFront;}
			GINL const GAIA::U8* bptr() const{if(this->empty()) return GNIL; return &m_pFront[this->buffer_size(this->size()) - 1];}
			GINL GAIA::BL exist(const _SizeType& index) const{GAST(index >= 0 && index < this->size()); if(index >= this->size()) return GAIA::False; return (GAIA_BITSET_SRC & GAIA_BITSET_CUR) != 0;}
			GINL GAIA::GVOID set(const _SizeType& index){GAST(index >= 0 && index < this->size()); if(index >= this->size()) return; GAIA_BITSET_SRC |= GAIA_BITSET_CUR;}
			GINL GAIA::GVOID reset(const _SizeType& index){GAST(index >= 0 && index < this->size()); if(index >= this->size()) return; GAIA_BITSET_SRC &= ~GAIA_BITSET_CUR;}
			GINL GAIA::GVOID inverse(const _SizeType& index){GAST(index >= 0 && index < this->size()); if(index >= this->size()) return; if(this->exist(index)) this->reset(index); else this->set(index);}
			GINL GAIA::GVOID push_back(GAIA::BL bSet)
			{
				if(this->size() == this->capacity())
				{
					_ExtendType increaser;
					_SizeType newsize = increaser.Increase(this->capacity()) + 8;
					this->exten(newsize - this->capacity());
				}
				++m_size;
				this->set(m_size - 1);
			}
			GINL GAIA::GVOID resize(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size < this->size())
					m_size = size;
				else
				{
					if(size > this->capacity())
					{
						this->destroy();
						this->reserve(size);
						m_size = size;
						this->clear();
						m_size = size;
					}
					else
					{
						for(_SizeType x = m_size; x < size; ++x)
							this->set(x);
						m_size = size;
					}
				}
			}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				GAST(size >= 0);
				this->destroy();
				if(size > 0)
				{
					m_pFront = gnew GAIA::U8[this->buffer_size(size)];
					GAIA::ALGO::reset(m_pFront, 0, this->buffer_size(size));
					m_capacity = size;
					m_size = 0;
				}
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				this->destroy();
				this->resize(src.size());
				GAIA::ALGO::copy(this->fptr(), src.fptr(), this->buffer_size(src.size()));
				return *this;
			}
			GINL __MyType& operator |= (const __MyType& src)
			{
				if(this->capacity() < src.size())
					this->exten(src.size() - this->capacity());
				this->resize(src.size());
				_SizeType buffer_size = src.buffer_size(src.size());
				for(_SizeType x = 0; x < buffer_size; ++x)
					this->fptr()[x] |= src.fptr()[x];
				return *this;
			}
			GINL __MyType& operator &= (const __MyType& src)
			{
				_SizeType minsize = GAIA::ALGO::gmin(this->buffer_size(this->size()), src.buffer_size(src.size()));
				for(_SizeType x = 0; x < minsize; ++x)
					this->fptr()[x] &= src.fptr()[x];
				return *this;
			}
			GINL __MyType& operator ^= (const __MyType& src)
			{
				_SizeType minsize = GAIA::ALGO::gmin(this->buffer_size(this->size()), src.buffer_size(src.size()));
				for(_SizeType x = 0; x < minsize; ++x)
					this->fptr()[x] ^= src.fptr()[x];
				return *this;
			}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				if(this->size() < src.size())
					return -1;
				else if(this->size() > src.size())
					return +1;
				if(this->size() == 0)
					return 0;
				return GAIA::ALGO::gmemcmp(this->fptr(), src.fptr(), this->buffer_size(this->size()));
			}
			GCLASS_COMPARE_BYCOMPARE(__MyType)
			GINL __MyType operator ~ () const
			{
				__MyType ret = *this;
				_SizeType buffer_size = ret.buffer_size(ret.size());
				for(_SizeType x = 0; x < buffer_size; ++x)
					ret.fptr()[x] ^= (GAIA::U8)GINVALID;
				return ret;
			}
			GINL GAIA::BL operator[](const _SizeType& index) const{return this->exist(index);}
		private:
			GINL GAIA::GVOID init(){m_pFront = GNIL; m_size = m_capacity = 0;}
			GINL GAIA::GVOID exten(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size == 0)
					return;
				GAIA::U8* pNew = gnew GAIA::U8[this->buffer_size(this->capacity() + size)];
				if(this->size() > 0)
					GAIA::ALGO::gmemcpy(pNew, this->fptr(), this->buffer_size(this->size()));
				if(m_pFront != GNIL)
					gdel[] m_pFront;
				m_pFront = pNew;
				m_capacity += size;
			}
			GINL _SizeType buffer_size(const _SizeType& size) const{return (size / 8) + ((size % 8 != 0) ? 1 : 0);}
		private:
			GAIA::U8* m_pFront;
			_SizeType m_size;
			_SizeType m_capacity;
		};
		class Bitset : public BasicBitset<GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
