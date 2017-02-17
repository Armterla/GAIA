#ifndef		__GAIA_CTN_STACK_H__
#define		__GAIA_CTN_STACK_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"
#include "gaia_algo_extend.h"
#include "gaia_algo_search.h"
#include "gaia_ctn.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicStack : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicStack<_DataType, _SizeType, _ExtendType> __MyType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicStack;
			public:
				GINL it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pContainer == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual _DataType& operator * (){GAST(!!m_pContainer); return (*m_pContainer)[m_index];}
				GINL virtual const _DataType& operator * () const{GAST(!!m_pContainer); return (*m_pContainer)[m_index];}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (){GAST(!!m_pContainer); ++m_index; if(m_index >= m_pContainer->size()) this->init(); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (){GAST(!!m_pContainer); --m_index; if(m_index >= m_pContainer->size() || m_index < 0) this->init(); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator = (const GAIA::ITERATOR::Iterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator == (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator != (*GSCAST(const it*)(&src));}
				GINL it& operator = (const it& src){GAST(&src != this); m_index = src.m_index; m_pContainer = src.m_pContainer; return *this;}
				GINL it& operator += (const _SizeType& c)
				{
					if(m_pContainer == GNIL)
						return *this;
					m_index += c;
					if(m_index >= m_pContainer->size() || m_index < 0)
						this->init();
					return *this;
				}
				GINL it& operator -= (const _SizeType& c)
				{
					if(m_pContainer == GNIL)
						return *this;
					m_index -= c;
					if(m_index >= m_pContainer->size() || m_index < 0)
						this->init();
					return *this;
				}
				GINL it operator + (const _SizeType& c) const
				{
					it ret = *this;
					ret += c;
					return ret;
				}
				GINL it operator - (const _SizeType& c) const
				{
					it ret = *this;
					ret -= c;
					return ret;
				}
				GINL _SizeType operator - (const it& src) const
				{
					if(this->empty() || src.empty())
						return 0;
					return this->m_index - src.m_index;
				}
				GCLASS_COMPARE(m_index, it)
			private:
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
			private:
				GINL GAIA::GVOID init(){m_pContainer = GNIL; m_index = 0;}
			private:
				_SizeType m_index;
				__MyType* m_pContainer;
			};
			class const_it : public GAIA::ITERATOR::ConstIterator<_DataType>
			{
			private:
				friend class BasicStack;
			public:
				GINL const_it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pContainer == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual const _DataType& operator * () const{GAST(!!m_pContainer); return (*m_pContainer)[m_index];}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (){GAST(!!m_pContainer); ++m_index; if(m_index >= m_pContainer->size()) this->init(); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (){GAST(!!m_pContainer); --m_index; if(m_index >= m_pContainer->size() || m_index < 0) this->init(); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator = (const GAIA::ITERATOR::ConstIterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator == (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator != (*GSCAST(const const_it*)(&src));}
				GINL const_it& operator = (const const_it& src){GAST(&src != this); m_index = src.m_index; m_pContainer = src.m_pContainer; return *this;}
				GINL const_it& operator += (const _SizeType& c)
				{
					if(m_pContainer == GNIL)
						return *this;
					m_index += c;
					if(m_index >= m_pContainer->size() || m_index < 0)
						this->init();
					return *this;
				}
				GINL const_it& operator -= (const _SizeType& c)
				{
					if(m_pContainer == GNIL)
						return *this;
					m_index -= c;
					if(m_index >= m_pContainer->size() || m_index < 0)
						this->init();
					return *this;
				}
				GINL const_it operator + (const _SizeType& c) const
				{
					const_it ret = *this;
					ret += c;
					return ret;
				}
				GINL const_it operator - (const _SizeType& c) const
				{
					const_it ret = *this;
					ret -= c;
					return ret;
				}
				GINL _SizeType operator - (const const_it& src) const
				{
					if(this->empty() || src.empty())
						return 0;
					return this->m_index - src.m_index;
				}
				GCLASS_COMPARE(m_index, const_it)
			private:
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
			private:
				GINL GAIA::GVOID init(){m_pContainer = GNIL; m_index = 0;}
			private:
				_SizeType m_index;
				const __MyType* m_pContainer;
			};
		public:
			GINL BasicStack(){this->init();}
			GINL BasicStack(const __MyType& src){this->init(); this->operator = (src);}
			GINL ~BasicStack(){if(m_pFront != GNIL) gdel[] m_pFront;}
			GAIA_CTN_PROXY
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL GAIA::BL nill() const{return m_pFront == GNIL;}
			GINL GAIA::GVOID clear(){m_size = 0;}
			GINL const _SizeType& size() const{return m_size;}
			GINL const _SizeType& capacity() const{return m_capacity;}
			GINL _SizeType typesize() const{return sizeof(_DataType);}
			GINL _SizeType datasize() const{return this->typesize() * this->size();}
			GINL GAIA::GVOID resize(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size <= this->capacity())
				{
					m_size = size;
					return;
				}
				this->reserve(size);
				m_size = size;
			}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				GAST(size >= 0);
				if(this->capacity() == size)
				{
					this->clear();
					return;
				}
				this->destroy();
				if(size > 0)
				{
					m_pFront = gnew _DataType[size];
					m_capacity = size;
				}
			}
			GINL GAIA::GVOID resize_keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size <= this->capacity())
					m_size = size;
				else
				{
					if(this->size() == 0)
						this->resize(size);
					else
					{
						_DataType* pNew = gnew _DataType[size];
						GAIA::ALGO::copy(pNew, this->fptr(), this->size());
						gdel[] m_pFront;
						m_pFront = pNew;
						m_size = size;
						m_capacity = size;
					}
				}
			}
			GINL GAIA::GVOID reserve_keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(this->capacity() == size)
					return;
				if(size > 0)
				{
					_DataType* pNew = gnew _DataType[size];
					_SizeType validsize = GAIA::ALGO::gmin(size, this->size());
					if(!this->empty())
						GAIA::ALGO::copy(pNew, this->fptr(), validsize);
					this->destroy();
					m_pFront = pNew;
					m_capacity = size;
					m_size = validsize;
				}
				else
					this->destroy();
			}
			GINL GAIA::GVOID destroy(){if(m_pFront != GNIL){gdel[] m_pFront; m_pFront = GNIL; m_capacity = m_size = 0;}}
			GINL _SizeType count(const _DataType& t) const
			{
				if(this->empty())
					return 0;
				_SizeType ret = 0;
				GAIA::ALGO::count(this->fptr(), this->bptr(), t, ret);
				return ret;
			}
			GINL GAIA::GVOID push_back(const _DataType& t)
			{
				if(this->size() == this->capacity())
				{
					_ExtendType increaser;
					_SizeType newsize = increaser.Increase(this->capacity());
					_DataType* pTemp = gnew _DataType[newsize];
					if(m_pFront != GNIL)
					{
						for(_SizeType c = 0; c < this->size(); ++c)
							pTemp[c] = m_pFront[c];
						gdel[] m_pFront;
					}
					m_pFront = pTemp;
					m_capacity = newsize;
				}
				m_pFront[m_size++] = t;
			}
			GINL GAIA::BL pop_back(){if(this->size() > 0){--m_size; return GAIA::True;} return GAIA::False;}
			GINL _DataType& front(){return this->operator[](0);}
			GINL const _DataType& front() const{return this->operator[](0);}
			GINL _DataType& back(){return this->operator[](this->size() - 1);}
			GINL const _DataType& back() const{return this->operator[](this->size() - 1);}
			GINL _DataType* fptr(){return m_pFront;}
			GINL const _DataType* fptr() const{return m_pFront;}
			GINL _DataType* bptr(){return &m_pFront[m_size - 1];}
			GINL const _DataType* bptr() const{return &m_pFront[m_size - 1];}
			GINL it frontit()
			{
				it ret;
				if(this->empty())
				{
					ret.m_index = 0;
					ret.m_pContainer = GNIL;
				}
				else
				{
					ret.m_index = 0;
					ret.m_pContainer = this;
				}
				return ret;
			}
			GINL it backit()
			{
				it ret;
				if(this->empty())
				{
					ret.m_index = 0;
					ret.m_pContainer = GNIL;
				}
				else
				{
					ret.m_index = this->size() - 1;
					ret.m_pContainer = this;
				}
				return ret;
			}
			GINL const_it const_frontit() const
			{
				const_it ret;
				if(this->empty())
				{
					ret.m_index = 0;
					ret.m_pContainer = GNIL;
				}
				else
				{
					ret.m_index = 0;
					ret.m_pContainer = this;
				}
				return ret;
			}
			GINL const_it const_backit() const
			{
				const_it ret;
				if(this->empty())
				{
					ret.m_index = 0;
					ret.m_pContainer = GNIL;
				}
				else
				{
					ret.m_index = this->size() - 1;
					ret.m_pContainer = this;
				}
				return ret;
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				if(this->capacity() >= src.size())
					this->clear();
				else
					this->reserve(src.size());
				if(src.size() > 0)
				{
					GAIA::ALGO::copy(m_pFront, src.m_pFront, src.size());
					m_size = src.size();
				}
				return *this;
			}
			GINL _DataType& operator[](const _SizeType& index)
			{
				GAST(index < m_size);
				return m_pFront[index];
			}
			GINL const _DataType& operator[](const _SizeType& index) const
			{
				GAST(index < m_size);
				return m_pFront[index];
			}
			GINL __MyType& operator += (const __MyType& src)
			{
				if(this == &src)
				{
					if(!this->empty())
					{
						__MyType temp = *this;
						*this += temp;
						return *this;
					}
				}
				const_it it = src.const_frontit();
				for(; !it.empty(); ++it)
					this->push_back(*it);
				return *this;
			}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				if(this->size() < src.size())
					return -1;
				else if(this->size() > src.size())
					return +1;
				const_it srcit = this->const_frontit();
				const_it dstit = src.const_frontit();
				for(;;)
				{
					if(srcit.empty() || dstit.empty())
						return 0;
					if(*srcit < *dstit)
						return -1;
					else if(*srcit > *dstit)
						return +1;
					++srcit;
					++dstit;
				}
			}
			GCLASS_COMPARE_BYCOMPARE(__MyType)
		private:
			GINL GAIA::GVOID init(){m_pFront = GNIL; m_capacity = m_size = 0;}
		private:
			_DataType* m_pFront;
			_SizeType m_capacity;
			_SizeType m_size;
		};
		template<typename _DataType> class Stack : public BasicStack<_DataType, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
