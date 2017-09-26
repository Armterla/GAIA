#ifndef		__GAIA_CTN_ARRAYVECTOR_H__
#define		__GAIA_CTN_ARRAYVECTOR_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"
#include "gaia_algo_extend.h"
#include "gaia_algo_hash.h"
#include "gaia_ctn.h"
#include "gaia_ctn_array.h"
#include "gaia_ctn_vector.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType, _SizeType _Size> class BasicArrayVector : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			static const _SizeType _size = _Size;
		public:
			typedef BasicArrayVector<_DataType, _SizeType, _ExtendType, _Size> __MyType;
			typedef GAIA::CTN::BasicArray<_DataType, _SizeType, _Size> __ArrayType;
			typedef GAIA::CTN::BasicVector<_DataType, _SizeType, _ExtendType> __VectorType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicArrayVector;
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
				friend class BasicArrayVector;
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
			GINL BasicArrayVector(){}
			GINL BasicArrayVector(const __MyType& src){this->operator = (src);}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL const _SizeType& size() const
			{
				if(m_vec.capacity() != 0)
					return m_vec.size();
				else
					return m_arr.size();
			}
			GINL _SizeType capacity() const{return GAIA::ALGO::gmax(m_vec.capacity(), m_arr.capacity());}
			GINL _SizeType capacity_array() const{return m_arr.capacity();}
			GINL _SizeType typesize() const{return sizeof(_DataType);}
			GINL _SizeType datasize() const{return this->typesize() * this->size();}
			GINL GAIA::GVOID resize(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size == this->size())
					return;
				else if(size > this->size())
				{
					if(size > m_arr.capacity())
					{
						m_arr.clear();
						m_vec.resize(size);
					}
					else
					{
						m_vec.destroy();
						m_arr.resize(size);
					}
				}
				else
				{
					if(m_vec.capacity() != 0)
						m_vec.resize(size);
					else
						m_arr.resize(size);
				}
			}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size > m_arr.capacity())
				{
					m_arr.clear();
					m_vec.reserve(size);
				}
				else
				{
					m_vec.destroy();
					m_arr.clear();
				}
			}
			GINL GAIA::GVOID resize_keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size == this->size())
					return;
				if(size > m_arr.capacity())
				{
					if(m_vec.capacity() == 0 && !m_arr.empty())
					{
						m_vec.reserve(size);
						m_vec.push_back(m_arr.fptr(), m_arr.size());
						m_vec.resize(size);
						m_arr.clear();
					}
					else
					{
						GAST(m_arr.empty());
						m_vec.resize_keep(size);
					}
				}
				else
				{
					if(m_vec.capacity() != 0 && !m_vec.empty())
					{
						GAST(m_arr.empty());
						m_arr.push_back(m_vec.fptr(), GAIA::ALGO::gmin(m_vec.size(), size));
					}
					m_vec.destroy();
					m_arr.resize(size);
				}
			}
			GINL GAIA::GVOID reserve_keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size == this->capacity())
					return;
				if(size > m_arr.capacity())
				{
					if(m_vec.capacity() == 0 && !m_arr.empty())
					{
						m_vec.reserve(size);
						m_vec.push_back(m_arr.fptr(), m_arr.size());
						m_vec.resize(m_arr.size());
						m_arr.clear();
					}
					else
					{
						GAST(m_arr.empty());
						m_vec.reserve_keep(size);
					}
				}
				else
				{
					if(m_vec.capacity() != 0 && !m_vec.empty())
					{
						GAST(m_arr.empty());
						m_arr.push_back(m_vec.fptr(), GAIA::ALGO::gmin(size, m_vec.size()));
					}
					m_vec.destroy();
				}
			}
			GINL GAIA::BL keep(const _SizeType& size)
			{
				if(size > this->size())
					return GAIA::False;
				if(size == this->size())
					return GAIA::True;
				_SizeType srcbegin = this->size() - size;
				for(_SizeType x = 0; x < size; ++x)
					(*this)[x] = (*this)[srcbegin + x];
				this->resize(size);
				return GAIA::True;
			}
			GINL GAIA::GVOID clear(){m_arr.clear(); m_vec.clear();}
			GINL GAIA::GVOID destroy(){m_arr.clear(); m_vec.destroy();}
			GINL GAIA::GVOID push_back(const _DataType& t)
			{
				if(m_vec.capacity() != 0)
				{
					if(m_vec.size() == m_vec.capacity())
					{
						if(m_vec.size() >= m_arr.capacity())
							m_vec.push_back(t);
						else
						{
							m_arr.clear();
							for(_SizeType x = 0; x < m_vec.size(); ++x)
								m_arr.push_back(m_vec[x]);
							m_vec.destroy();
							m_arr.push_back(t);
						}
					}
					else
						m_vec.push_back(t);
				}
				else
				{
					if(m_arr.size() == m_arr.capacity())
					{
						m_vec.reserve(m_arr.size() + 1);
						for(_SizeType x = 0; x < m_arr.size(); ++x)
							m_vec.push_back(m_arr[x]);
						m_arr.clear();
						m_vec.push_back(t);
					}
					else
						m_arr.push_back(t);
				}
			}
			GINL GAIA::GVOID push_back(const _DataType* p, const _SizeType& size)
			{
				GAST(p != GNIL);
				GAST(size > 0);
				if(m_vec.capacity() != 0)
				{
					if(m_vec.capacity() - m_vec.size() < size)
					{
						if(m_vec.size() + size > m_arr.capacity())
							m_vec.push_back(p, size);
						else
						{
							m_arr.clear();
							for(_SizeType x = 0; x < m_vec.size(); ++x)
								m_arr.push_back(m_vec[x]);
							m_vec.destroy();
							m_arr.push_back(p, size);
						}
					}
					else
						m_vec.push_back(p, size);
				}
				else
				{
					if(m_arr.size() + size > m_arr.capacity())
					{
						m_vec.reserve(m_arr.size() + size);
						for(_SizeType x = 0; x < m_arr.size(); ++x)
							m_vec.push_back(m_arr[x]);
						m_arr.clear();
						m_vec.push_back(p, size);
					}
					else
						m_arr.push_back(p, size);
				}
			}
			GINL GAIA::BL pop_back()
			{
				if(m_vec.size() != 0)
					m_vec.pop_back();
				else
				{
					if(m_arr.empty())
						return GAIA::False;
					m_arr.pop_back();
				}
				return GAIA::True;
			}
			GINL GAIA::BL pop_back(_DataType* p, const _SizeType& size)
			{
				if(m_vec.capacity() != 0)
					return m_vec.pop_back(p, size);
				else
					return m_arr.pop_back(p, size);
			}
			GINL GAIA::GVOID inverse()
			{
				if(m_vec.size() > 1)
					m_vec.inverse();
				else if(m_arr.size() > 1)
					m_arr.inverse();
			}
			GINL GAIA::BL insert(const _DataType& t, const _SizeType& index)
			{
				if(m_vec.capacity() != 0)
				{
					if(m_vec.size() == m_vec.capacity())
					{
						if(m_vec.size() >= m_arr.capacity())
							return m_vec.insert(t, index);
						else
						{
							m_arr.clear();
							for(_SizeType x = 0; x < m_vec.size(); ++x)
								m_arr.push_back(m_vec[x]);
							m_vec.destroy();
							return m_arr.insert(t, index);
						}
					}
					else
						return m_vec.insert(t, index);
				}
				else
				{
					if(m_arr.size() == m_arr.capacity())
					{
						m_vec.reserve(m_arr.size() + 1);
						for(_SizeType x = 0; x < m_arr.size(); ++x)
							m_vec.push_back(m_arr[x]);
						m_arr.clear();
						return m_vec.insert(t, index);
					}
					else
						m_arr.push_back(t);
				}
				return GAIA::True;
			}
			GINL GAIA::BL erase(const _SizeType& index)
			{
				if(m_vec.capacity() != 0)
					return m_vec.erase(index);
				else
					return m_arr.erase(index);
			}
			GINL GAIA::BL erase(const _SizeType& index, const _SizeType& len)
			{
				if(m_vec.capacity() != 0)
					return m_vec.erase(index, len);
				else
					return m_arr.erase(index, len);
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
			template<typename _ParamDataType> _SizeType extract(_SizeType index_start, _SizeType index_end, _ParamDataType* pResult, const _SizeType& nResultSize) const
			{
				if(m_vec.capacity() != 0)
					return m_vec.extract(index_start, index_end, pResult, nResultSize);
				else
					return m_arr.extract(index_start, index_end, pResult, nResultSize);
			}
			template<typename _ParamDataType> _SizeType extract_left(_SizeType index, _ParamDataType* pResult, const _SizeType& nResultSize) const
			{
				return this->extract(0, index - 1, pResult, nResultSize);
			}
			template<typename _ParamDataType> _SizeType extract_right(_SizeType index, _ParamDataType* pResult, const _SizeType& nResultSize) const
			{
				return this->extract(index + 1, GINVALID, pResult, nResultSize);
			}
			template<typename _ParamDataType> _SizeType extract_mid(_SizeType index_start, _SizeType index_end, _ParamDataType* pResult, const _SizeType& nResultSize) const
			{
				return this->extract(index_start, index_end, pResult, nResultSize);
			}
			GINL _SizeType extract(_SizeType index_start, _SizeType index_end, __MyType& res) const
			{
				if(index_start == GINVALID)
					index_start = 0;
				if(index_end == GINVALID)
					index_end = this->size() - 1;
				_SizeType oldsize = res.size();
				res.resize_keep(oldsize + index_end - index_start + 1);
				if(m_vec.capacity() != 0)
					return m_vec.extract(index_start, index_end, res.fptr() + oldsize, res.size() - oldsize);
				else
					return m_arr.extract(index_start, index_end, res.fptr() + oldsize, res.size() - oldsize);
			}
			GINL _SizeType extract_left(_SizeType index, __MyType& res) const
			{
				return this->extract(0, index - 1, res);
			}
			GINL _SizeType extract_right(_SizeType index, __MyType& res) const
			{
				return this->extract(index + 1, GINVALID, res);
			}
			GINL _SizeType extract_mid(_SizeType index_start, _SizeType index_end, __MyType& res) const
			{
				return this->extract(index_start, index_end, res);
			}
			GINL _SizeType find(const _DataType& t, const _SizeType& index = 0) const
			{
				if(m_vec.capacity() != 0)
					return m_vec.find(t, index);
				else
					return m_arr.find(t, index);
			}
			GINL _SizeType rfind(const _DataType& t, _SizeType index = GINVALID) const
			{
				if(m_vec.capacity() != 0)
					return m_vec.rfind(t, index);
				else
					return m_arr.rfind(t, index);
			}
			GINL GAIA::BL exist(const _DataType& t) const
			{
				return this->find(t) != GINVALID;
			}
			GINL _SizeType replace(const _DataType& tOld, const _DataType& tNew)
			{
				if(m_vec.capacity() != 0)
					m_vec.replace(tOld, tNew);
				else
					m_arr.replace(tOld, tNew);
			}
			GINL GAIA::GVOID reset(const _DataType& t)
			{
				if(m_vec.capacity() != 0)
					m_vec.reset(t);
				else
					m_arr.reset(t);
			}
			GINL _SizeType count(const _DataType& t) const
			{
				if(m_vec.capacity() != 0)
					return m_vec.count(t);
				else
					return m_arr.count(t);
			}
			GINL GAIA::GVOID sort()
			{
				if(m_vec.capacity() != 0)
					return m_vec.sort();
				else
					return m_arr.sort();
			}
			GINL _SizeType unique()
			{
				if(m_vec.capacity() != 0)
					return m_vec.unique();
				else
					return m_vec.unique_noorder();
			}
			GINL _SizeType unique_noorder()
			{
				if(m_vec.capacity() != 0)
					return m_vec.unique_noorder();
				else
					return m_arr.unique_noorder();
			}
			GINL _SizeType binary_search(const _DataType& t) const
			{
				if(m_vec.capacity() != 0)
					return m_vec.binary_search(t);
				else
					return m_arr.binary_search(t);
			}
			template<typename _ParamKeyType> _SizeType binary_search(const _ParamKeyType& k, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k)) const
			{
				if(m_vec.capacity() != 0)
					return m_vec.binary_search(k, cmpor);
				else
					return m_arr.binary_search(k, cmpor);
			}
			GINL it binary_searchit(const _DataType& t)
			{
				it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::it ittemp = m_vec.binary_searchit(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.frontit();
				}
				else
				{
					typename __ArrayType::it ittemp = m_arr.binary_searchit(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.frontit();
				}
				return ret;
			}
			template<typename _ParamKeyType> it binary_searchit(const _ParamKeyType& k, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k))
			{
				it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::it ittemp = m_vec.binary_searchit(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.frontit();
				}
				else
				{
					typename __ArrayType::it ittemp = m_arr.binary_searchit(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.frontit();
				}
				return ret;
			}
			GINL const_it const_binary_searchit(const _DataType& t) const
			{
				const_it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::const_it ittemp = m_vec.const_binary_searchit(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.const_frontit();
				}
				else
				{
					typename __ArrayType::const_it ittemp = m_arr.const_binary_searchit(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.const_frontit();
				}
				return ret;
			}
			template<typename _ParamKeyType> const_it const_binary_searchit(const _ParamKeyType& k, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k)) const
			{
				const_it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::const_it ittemp = m_vec.const_binary_searchit(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.const_frontit();
				}
				else
				{
					typename __ArrayType::const_it ittemp = m_arr.const_binary_searchit(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.const_frontit();
				}
				return ret;
			}
			GINL GAIA::GVOID swap(const _SizeType& index1, const _SizeType& index2)
			{
				if(m_vec.capacity() != 0)
					m_vec.swap(index1, index2);
				else
					m_arr.swap(index1, index2);
			}
			GINL GAIA::GVOID relocation(const _SizeType& srcindex, const _SizeType& dstindex)
			{
				if(m_vec.capacity() != 0)
					m_vec.relocation(srcindex, dstindex);
				else
					m_arr.relocation(srcindex, dstindex);
			}
			GINL it findit(const _DataType& t)
			{
				it iter;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::it ittemp = m_vec.findit(t);
					iter.m_pContainer = this;
					iter.m_index = ittemp - m_vec.frontit();
				}
				else
				{
					typename __ArrayType::it ittemp = m_arr.findit(t);
					iter.m_pContainer = this;
					iter.m_index = ittemp - m_arr.frontit();
				}
				return iter;
			}
			GINL const_it const_findit(const _DataType& t) const
			{
				const_it iter;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::const_it ittemp = m_vec.const_findit(t);
					iter.m_pContainer = this;
					iter.m_index = ittemp - m_vec.const_frontit();
				}
				else
				{
					typename __ArrayType::const_it ittemp = m_arr.const_findit(t);
					iter.m_pContainer = this;
					iter.m_index = ittemp - m_arr.const_frontit();
				}
				return iter;
			}
			GINL it upper_equal(const _DataType& t)
			{
				it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::it ittemp = m_vec.upper_equal(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.frontit();
				}
				else
				{
					typename __ArrayType::it ittemp = m_arr.upper_equal(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.frontit();
				}
				return ret;
			}
			GINL const_it upper_equal(const _DataType& t) const
			{
				const_it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::const_it ittemp = m_vec.upper_equal(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.const_frontit();
				}
				else
				{
					typename __ArrayType::const_it ittemp = m_arr.upper_equal(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.const_frontit();
				}
				return ret;
			}
			GINL it lower_equal(const _DataType& t)
			{
				it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::it ittemp = m_vec.lower_equal(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.frontit();
				}
				else
				{
					typename __ArrayType::it ittemp = m_arr.lower_equal(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.frontit();
				}
				return ret;
			}
			GINL const_it lower_equal(const _DataType& t) const
			{
				const_it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::const_it ittemp = m_vec.lower_equal(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.const_frontit();
				}
				else
				{
					typename __ArrayType::const_it ittemp = m_arr.lower_equal(t);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.const_frontit();
				}
				return ret;
			}
			template<typename _ParamKeyType> it upper_equal(const _ParamKeyType& k, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k))
			{
				it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::it ittemp = m_vec.upper_equal(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.frontit();
				}
				else
				{
					typename __ArrayType::it ittemp = m_arr.upper_equal(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.frontit();
				}
				return ret;
			}
			template<typename _ParamKeyType> const_it upper_equal(const _ParamKeyType& k, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k)) const
			{
				const_it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::const_it ittemp = m_vec.upper_equal(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.const_frontit();
				}
				else
				{
					typename __ArrayType::const_it ittemp = m_arr.upper_equal(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.const_frontit();
				}
				return ret;
			}
			template<typename _ParamKeyType> it lower_equal(const _ParamKeyType& k, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k))
			{
				it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::it ittemp = m_vec.lower_equal(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.frontit();
				}
				else
				{
					typename __ArrayType::it ittemp = m_arr.lower_equal(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.frontit();
				}
				return ret;
			}
			template<typename _ParamKeyType> const_it lower_equal(const _ParamKeyType& k, GAIA::N32 (*cmpor)(const GAIA::GVOID* t, const GAIA::GVOID* k)) const
			{
				const_it ret;
				if(m_vec.capacity() != 0)
				{
					typename __VectorType::const_it ittemp = m_vec.lower_equal(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_vec.const_frontit();
				}
				else
				{
					typename __ArrayType::const_it ittemp = m_arr.lower_equal(k, cmpor);
					ret.m_pContainer = this;
					ret.m_index = ittemp - m_arr.const_frontit();
				}
				return ret;
			}
			GINL __MyType& operator += (const __MyType& src)
			{
				this->reserve_keep(this->size() + src.size());
				for(_SizeType x = 0; x < src.size(); ++x)
					this->push_back(src[x]);
				return *this;
			}
			GINL const _DataType& operator[](const _SizeType& index) const
			{
				if(m_vec.capacity() != 0)
					return m_vec[index];
				else
					return m_arr[index];
			}
			GINL _DataType& operator[](const _SizeType& index)
			{
				if(m_vec.capacity() != 0)
					return m_vec[index];
				else
					return m_arr[index];
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				m_arr = src.m_arr;
				m_vec = src.m_vec;
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
			GINL _DataType& front(){GAST(!this->empty()); return *this->fptr();}
			GINL const _DataType& front() const{GAST(!this->empty()); return *this->fptr();}
			GINL _DataType& back(){GAST(!this->empty()); return *this->bptr();}
			GINL const _DataType& back() const{GAST(!this->empty()); return *this->bptr();}
			GINL _DataType* fptr(){GAST(!this->empty()); return &(*this)[0];}
			GINL const _DataType* fptr() const{GAST(!this->empty()); return &(*this)[0];}
			GINL _DataType* bptr(){GAST(!this->empty()); return &(*this)[this->size() - 1];}
			GINL const _DataType* bptr() const{GAST(!this->empty()); return &(*this)[this->size() - 1];}
			GINL it frontit()
			{
				it ret;
				if(!this->empty())
				{
					ret.m_pContainer = this;
					ret.m_index = 0;
				}
				return ret;
			}
			GINL it backit()
			{
				it ret;
				if(!this->empty())
				{
					ret.m_pContainer = this;
					ret.m_index = this->size() - 1;
				}
				return ret;
			}
			GINL const_it const_frontit() const
			{
				const_it ret;
				if(!this->empty())
				{
					ret.m_pContainer = this;
					ret.m_index = 0;
				}
				return ret;
			}
			GINL const_it const_backit() const
			{
				const_it ret;
				if(!this->empty())
				{
					ret.m_pContainer = this;
					ret.m_index = this->size() - 1;
				}
				return ret;
			}
			GINL GAIA::U64 hash() const
			{
				if(this->empty())
					return 0;
				return GAIA::ALGO::hash((const GAIA::GVOID*)this->fptr(), this->datasize());
			}
		private:
			__ArrayType m_arr;
			__VectorType m_vec;
		};
		template<typename _DataType, GAIA::NUM _Size> class ArrayVector : public BasicArrayVector<_DataType, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM>, _Size>{public:};
	}
}

#endif
