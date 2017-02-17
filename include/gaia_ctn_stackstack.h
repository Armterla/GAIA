#ifndef		__GAIA_CTN_STACKSTACK_H__
#define		__GAIA_CTN_STACKSTACK_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, _SizeType _Size> class BasicStackStack : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
		public:
			static const _SizeType _size = _Size;
		public:
			typedef BasicStackStack<_DataType, _SizeType, _Size> __MyType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicStackStack;
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
				friend class BasicStackStack;
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
				GINL const_it& operator = (const const_it& src){GAST(&src != this); m_index = src.m_index; m_pContainer = src.m_pContainer; return *this;}
				GCLASS_COMPARE(m_index, const_it)
			private:
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
				GINL GAIA::GVOID init(){m_pContainer = GNIL; m_index = 0;}
			private:
				_SizeType m_index;
				const __MyType* m_pContainer;
			};
		public:
			GINL BasicStackStack(){this->init();}
			GINL BasicStackStack(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL const _SizeType& size() const{return m_size;}
			GINL _SizeType capacity() const{return _Size;}
			GINL _SizeType typesize() const{return sizeof(_DataType);}
			GINL _SizeType datasize() const{return this->typesize() * this->size();}
			GINL GAIA::BL resize(const _SizeType& size)
			{
				GAST(size >= 0 && size <= this->capacity());
				if(size >= 0 && size <= this->capacity())
				{
					m_size = size;
					return GAIA::True;
				}
				return GAIA::False;
			}
			GINL GAIA::GVOID clear(){m_size = 0;}
			GINL _SizeType count(const _DataType& t) const{_SizeType ret = 0; GAIA::ALGO::count(this->fptr(), this->bptr(), t, ret); return ret;}
			GINL GAIA::BL push_back(const _DataType& t)
			{
				if(this->size() < this->capacity())
				{
					m_data[m_size] = t;
					++m_size;
					return GAIA::True;
				}
				else
					return GAIA::False;
			}
			GINL GAIA::BL pop_back()
			{
				if(m_size > 0)
				{
					--m_size;
					return GAIA::True;
				}
				else
					return GAIA::False;
			}
			GINL _DataType& front(){return this->operator[](0);}
			GINL const _DataType& front() const{return this->operator[](0);}
			GINL _DataType& back(){return this->operator[](this->size() - 1);}
			GINL const _DataType& back() const{return this->operator[](this->size() - 1);}
			GINL _DataType* fptr(){return m_data;}
			GINL const _DataType* fptr() const{return m_data;}
			GINL _DataType* bptr(){return &m_data[m_size - 1];}
			GINL const _DataType* bptr() const{return &m_data[m_size - 1];}
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
				this->clear();
				if(!src.empty())
				{
					GAIA::ALGO::copy(m_data, src.m_data, src.size());
					m_size = src.size();
				}
				return *this;
			}
			GINL _DataType& operator[](const _SizeType& index){return m_data[index];}
			GINL const _DataType& operator[](const _SizeType& index) const{return m_data[index];}
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
			GINL GAIA::GVOID init(){m_size = 0;}
		private:
			_DataType m_data[_Size];
			_SizeType m_size;
		};
		template<typename _DataType, GAIA::NUM _Size> class StackStack : public BasicStackStack<_DataType, GAIA::NUM, _Size>{public:};
	}
}

#endif
