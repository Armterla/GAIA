#ifndef		__GAIA_CTN_ARRAY_H__
#define		__GAIA_CTN_ARRAY_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_search.h"
#include "gaia_algo_sort.h"
#include "gaia_algo_unique.h"
#include "gaia_algo_replace.h"
#include "gaia_iterator.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, _SizeType _Size> class BasicArray : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
		public:
			static const _SizeType _size = _Size;
		public:
			typedef BasicArray<_DataType, _SizeType, _Size> __MyType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicArray;
			public:
				GINL it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pContainer == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual GAIA::BL erase()
				{
					if(this->empty())
						return GAIA::False;
					if(!m_pContainer->erase(m_index))
						return GAIA::False;
					if(m_index >= m_pContainer->size())
						this->init();
					return GAIA::True;
				}
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
				friend class BasicArray;
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
			GINL BasicArray(){this->init();}
			GINL BasicArray(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL const _SizeType& size() const{return m_size;}
			GINL _SizeType capacity() const{return _Size;}
			GINL _SizeType typesize() const{return sizeof(_DataType);}
			GINL _SizeType datasize() const{return this->typesize() * this->size();}
			GINL GAIA::GVOID clear(){m_size = 0;}
			GINL GAIA::BL push_back(const _DataType& t){if(this->size() < this->capacity()){m_data[m_size++] = t; return GAIA::True;} else return GAIA::False;}
			GINL GAIA::BL push_back(const _DataType* p, const _SizeType& size)
			{
				GAST(!!p);
				GAST(size > 0);
				_SizeType newsize = this->size() + size;
				if(newsize > this->capacity())
					return GAIA::False;
				for(_SizeType x = 0; x < size; ++x)
					m_data[m_size++] = p[x];
				return GAIA::True;
			}
			GINL GAIA::BL pop_back(){GAST(!this->empty()); if(!this->empty()){m_size--; return GAIA::True;} return GAIA::False;}
			GINL GAIA::BL pop_back(_DataType* p, const _SizeType& size)
			{
				GAST(!!p);
				GAST(size > 0);
				if(size > this->size())
					return GAIA::False;
				for(_SizeType x = 0; x < size; ++x)
					p[x] = m_data[this->size() - size + x];
				m_size -= size;
				return GAIA::True;
			}
			GINL _DataType& front(){GAST(!this->empty()); return *this->fptr();}
			GINL const _DataType& front() const{GAST(!this->empty()); return *this->fptr();}
			GINL _DataType& back(){GAST(!this->empty()); return *this->bptr();}
			GINL const _DataType& back() const{GAST(!this->empty()); return *this->bptr();}
			GINL _DataType* fptr(){GAST(!this->empty()); return &(*this)[0];}
			GINL const _DataType* fptr() const{GAST(!this->empty()); return &(*this)[0];}
			GINL _DataType* bptr(){GAST(!this->empty()); return &(*this)[this->size() - 1];}
			GINL const _DataType* bptr() const{GAST(!this->empty()); return &(*this)[this->size() - 1];}
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
			GINL GAIA::GVOID reset(const _DataType& t){GAIA::ALGO::reset(this->fptr(), t, this->size());}
			GINL _SizeType count(const _DataType& t) const
			{
				if(this->empty())
					return 0;
				_SizeType ret = 0;
				GAIA::ALGO::count(this->fptr(), this->bptr(), t, ret);
				return ret;
			}
			GINL GAIA::GVOID sort(){if(this->empty()) return; GAIA::ALGO::sort(m_data, &m_data[this->size() - 1]);}
			GINL _SizeType unique()
			{
				if(this->empty())
					return 0;
				_DataType* p = GAIA::ALGO::unique_order(this->fptr(), this->bptr());
				_SizeType ret = m_size - ((_SizeType)(p - this->fptr()) + 1);
				m_size = (_SizeType)(p - this->fptr()) + 1;
				return ret;
			}
			GINL _SizeType unique_noorder()
			{
				if(this->empty())
					return 0;
				_DataType* p = GAIA::ALGO::unique_noorder(this->fptr(), this->bptr());
				_SizeType ret = m_size - ((_SizeType)(p - this->fptr()) + 1);
				m_size = (_SizeType)(p - this->fptr()) + 1;
				return ret;
			}
			GINL _SizeType binary_search(const _DataType& t) const
			{
				if(this->size() <= 0)
					return (_SizeType)GINVALID;
				const _DataType* pFinded = GAIA::ALGO::gbinary_search(m_data, &m_data[this->size() - 1], t);
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_data);
			}
			GINL it binary_searchit(const _DataType& t)
			{
				it iter;
				_SizeType findedidx = this->binary_search(t);
				if(findedidx == GINVALID)
					return iter;
				iter.m_pContainer = this;
				iter.m_index = findedidx;
				return iter;
			}
			GINL const_it const_binary_searchit(const _DataType& t) const
			{
				const_it iter;
				_SizeType findedidx = this->binary_search(t);
				if(findedidx == GINVALID)
					return iter;
				iter.m_pContainer = this;
				iter.m_index = findedidx;
				return iter;
			}
			GINL GAIA::GVOID swap(const _SizeType& index1, const _SizeType& index2){GAIA::ALGO::swap(this->operator[](index1), this->operator[](index2));}
			GINL GAIA::GVOID relocation(const _SizeType& srcindex, const _SizeType& dstindex)
			{
				GPCHR_TRUE(srcindex < 0 || srcindex >= this->size());
				GPCHR_TRUE(dstindex < 0 || dstindex >= this->size());
				if(srcindex == dstindex)
					return;
				if(srcindex < dstindex)
				{
					_DataType t = (*this)[srcindex];
					for(_SizeType x = srcindex; x < dstindex; ++x) // Forward move. x + 1 is src index, x is dst index.
						(*this)[x] = (*this)[x + 1];
					(*this)[dstindex] = t;
				}
				else
				{
					_DataType t = (*this)[srcindex];
					for(_SizeType x = srcindex; x > dstindex; --x) // Backward move. x - 1 is src index, x is dst index.
						(*this)[x] = (*this)[x - 1];
					(*this)[dstindex] = t;
				}
			}
			GINL _SizeType find(const _DataType& t, const _SizeType& index = 0) const
			{
				if(this->empty())
					return (_SizeType)GINVALID;
				if(index >= this->size())
					return (_SizeType)GINVALID;
				const _DataType* pFinded = GAIA::ALGO::find(this->fptr() + index, this->bptr(), t);
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_data);
			}
			GINL _SizeType rfind(const _DataType& t, _SizeType index = GINVALID) const
			{
				if(this->empty())
					return (_SizeType)GINVALID;
				if(index >= this->size())
					return (_SizeType)GINVALID;
				else if(index == GINVALID)
					index = this->size() - 1;
				const _DataType* pFinded = GAIA::ALGO::rfind(this->fptr(), this->fptr() + index, t);
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_data);
			}
			GINL _SizeType replace(const _DataType& tOld, const _DataType& tNew)
			{
				return GAIA::ALGO::replace(this->fptr(), this->size(), tOld, tNew);
			}
			GINL GAIA::GVOID inverse(){GAIA::ALGO::inverse(this->fptr(), this->bptr());}
			GINL GAIA::BL insert(const _DataType& t, const _SizeType& index)
			{
				GAST(index >= 0 && index <= this->size());
				if(index > this->size())
					return GAIA::False;
				if(this->size() == this->capacity())
					return GAIA::False;
				if(this->empty())
					this->push_back(t);
				else
				{
					this->resize(this->size() + 1);
					GAIA::ALGO::move_right(this->fptr() + this->size() - 1,
						this->fptr() + this->size() - 2,
						this->size() - index);
					this->operator[](index) = t;
				}
				return GAIA::True;
			}
			GINL GAIA::BL erase(const _SizeType& index)
			{
				GAST(index >= 0 && index < this->size());
				if(this->empty())
					return GAIA::False;
				if(index < 0)
					return GAIA::False;
				else if(index >= this->size())
					return GAIA::False;
				if(index + 1 < this->size())
					GAIA::ALGO::move_left(&this->operator[](index), &this->operator[](index + 1), this->size() - index - 1);
				m_size--;
				return GAIA::True;
			}
			GINL GAIA::BL erase(const _SizeType& index, const _SizeType& len)
			{
				GAST(index >= 0 && index < this->size());
				if(index < 0)
					return GAIA::False;
				else if(index >= this->size())
					return GAIA::False;
				GAST(len > 0);
				if(len <= 0)
					return GAIA::False;
				GAST(index + len <= this->size());
				if(index + len > this->size())
					return GAIA::False;
				if(index + len != this->size())
					GAIA::ALGO::move_left(this->fptr() + index, this->fptr() + index + len, this->size() - len - index);
				m_size -= len;
				return GAIA::True;
			}
			GINL _SizeType remove(const _DataType& t, const _SizeType& index = 0)
			{
				_SizeType f = this->find(t, index);
				if(f == GINVALID)
					return GINVALID;
				this->erase(f);
				return f;
			}
			GINL _SizeType removeall(const _DataType& t, const _SizeType& index = 0)
			{
				_SizeType ret = 0;
				_SizeType f = 0;
				while((f = this->find(t, f)) != GINVALID)
				{
					this->erase(f);
					++ret;
				}
				return ret;
			}
			GINL it findit(const _DataType& t, const _SizeType& index = 0)
			{
				it iter;
				_SizeType findedidx = this->find(t, index);
				if(findedidx == GINVALID)
					return iter;
				iter.m_pContainer = this;
				iter.m_index = findedidx;
				return iter;
			}
			GINL const_it const_findit(const _DataType& t, const _SizeType& index = 0) const
			{
				const_it iter;
				_SizeType findedidx = this->find(t, index);
				if(findedidx == GINVALID)
					return iter;
				iter.m_pContainer = this;
				iter.m_index = findedidx;
				return iter;
			}
			GINL it upper_equal(const _DataType& t)
			{
				it iter;
				if(!this->empty())
				{
					_DataType* p = GAIA::ALGO::upper_equal(this->fptr(), this->bptr(), t);
					if(p != GNIL)
					{
						iter.m_pContainer = this;
						iter.m_index = GSCAST(_SizeType)(p - this->fptr());
					}
				}
				return iter;
			}
			GINL const_it upper_equal(const _DataType& t) const
			{
				const_it iter;
				if(!this->empty())
				{
					const _DataType* p = GAIA::ALGO::upper_equal(this->fptr(), this->bptr(), t);
					if(p != GNIL)
					{
						iter.m_pContainer = this;
						iter.m_index = GSCAST(_SizeType)(p - this->fptr());
					}
				}
				return iter;
			}
			GINL it lower_equal(const _DataType& t)
			{
				it iter;
				if(!this->empty())
				{
					_DataType* p = GAIA::ALGO::lower_equal(this->fptr(), this->bptr(), t);
					if(p != GNIL)
					{
						iter.m_pContainer = this;
						iter.m_index = GSCAST(_SizeType)(p - this->fptr());
					}
				}
				return iter;
			}
			GINL const_it lower_equal(const _DataType& t) const
			{
				const_it iter;
				if(!this->empty())
				{
					const _DataType* p = GAIA::ALGO::lower_equal(this->fptr(), this->bptr(), t);
					if(p != GNIL)
					{
						iter.m_pContainer = this;
						iter.m_index = GSCAST(_SizeType)(p - this->fptr());
					}
				}
				return iter;
			}
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
			GINL __MyType& operator += (const __MyType& src)
			{
				if(!src.empty())
				{
					_SizeType size = src.size(); // For += self.
					this->push_back(src.fptr(), size);
				}
				return *this;
			}
			GINL const _DataType& operator [] (const _SizeType& index) const{return m_data[index];}
			GINL _DataType& operator [] (const _SizeType& index){return m_data[index];}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				this->resize(src.size());
				for(_sizetype x = 0; x < src.size(); ++x)
					this->operator[](x) = src[x];
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
		template<typename _DataType, GAIA::NUM _Size> class Array : public BasicArray<_DataType, GAIA::NUM, _Size>{public:};
	}
}

#endif
