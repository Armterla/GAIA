#ifndef 	__GAIA_CTN_BOOK_H__
#define 	__GAIA_CTN_BOOK_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_stack.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_pool.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicBook : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		private:
			class UsedNode;
		public:
			typedef BasicBook<_DataType, _SizeType, _ExtendType> __MyType;
			typedef GAIA::CTN::BasicPool<UsedNode, _SizeType, _ExtendType> __PoolType; // UsedNode pool.
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicBook;
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
				friend class BasicBook;
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
			GINL BasicBook(){this->init();}
			GINL BasicBook(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return m_usedlist.empty();}
			GINL _SizeType size() const{return m_usedlist.size();}
			GINL _SizeType capacity() const{return m_fixedlist.capacity();}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				GAST(size >= 0);
				if(m_fixedlist.size() == size)
					return;
				m_fixedlist.reserve(size);
				m_freestack.reserve(size);
				m_usedlist.reserve(size);
			}
			GINL GAIA::GVOID clear()
			{
				m_pool.clear();
				m_fixedlist.clear();
				m_freestack.clear();
				m_usedlist.clear();
			}
			GINL GAIA::GVOID destroy()
			{
				m_pool.destroy();
				m_fixedlist.destroy();
				m_freestack.destroy();
				m_usedlist.destroy();
			}
			GINL _SizeType insert(const _DataType& t)
			{
				UsedNode* p = m_pool.alloc();
				FixedNode n;
				n.t = t;
				n.p = p;
				if(m_freestack.empty())
				{
					m_fixedlist.push_back(n);
					p->fi = m_fixedlist.size() - 1;
				}
				else
				{
					p->fi = m_freestack.back();
					m_fixedlist[p->fi] = n;
					m_freestack.pop_back();
				}
				m_usedlist.push_back(p);
				p->ui = m_usedlist.size() - 1;
				return p->ui;
			}
			GINL GAIA::BL erase(const _SizeType& usedindex)
			{
				GAST(usedindex >= 0 && usedindex < m_usedlist.size());
				if(usedindex < 0)
					return GAIA::False;
				if(usedindex >= m_usedlist.size())
					return GAIA::False;
				_SizeType fixedindex = this->fixedindex(usedindex);
				GAST(fixedindex >= 0 && fixedindex < m_fixedlist.size());
				if(fixedindex < 0)
					return GAIA::False;
				if(fixedindex >= m_fixedlist.size())
					return GAIA::False;
				FixedNode& n = m_fixedlist[fixedindex];
				GAST(n.p != GNIL);
				m_freestack.push_back(n.p->fi);
				if(usedindex + 1 != m_usedlist.size())
				{
					m_usedlist.back()->ui = usedindex;
					m_usedlist[usedindex] = m_usedlist.back();
				}
				m_usedlist.resize(m_usedlist.size() - 1);
				m_pool.release(n.p);
				n.p = GNIL;
				return GAIA::True;
			}
			GINL _SizeType usedindex(const _SizeType& fixedindex) const
			{
				if(fixedindex >= m_fixedlist.size())
					return GINVALID;
				UsedNode* p = m_fixedlist[fixedindex].p;
				if(p == GNIL)
					return GINVALID;
				GAST(p->ui < m_usedlist.size());
				return p->ui;
			}
			GINL _SizeType fixedindex(const _SizeType& usedindex) const
			{
				UsedNode* p = m_usedlist[usedindex];
				GAST(p != GNIL);
				if(p == GNIL)
					return GINVALID;
				GAST(p->fi < m_fixedlist.size());
				return p->fi;
			}
			GINL GAIA::BL relocation(const _SizeType& srcusedindex, const _SizeType& dstusedindex)
			{
				GPCHR_TRUE_RET(srcusedindex < 0 || srcusedindex >= this->size(), GAIA::False);
				GPCHR_TRUE_RET(dstusedindex < 0 || dstusedindex >= this->size(), GAIA::False);
				if(srcusedindex == dstusedindex)
					return GAIA::True;
				if(srcusedindex < dstusedindex)
				{
					UsedNode& srcusednode = *m_usedlist[srcusedindex];
					_SizeType fi = srcusednode.fi;
					for(_SizeType x = srcusedindex; x < dstusedindex; ++x) // Forward move. x + 1 is src index, x is dst index.
					{
						UsedNode& src = *m_usedlist[x + 1];
						UsedNode& dst = *m_usedlist[x];
						FixedNode& fsrc = m_fixedlist[src.fi];
						FixedNode& fdst = m_fixedlist[dst.fi];
						fdst.p = fsrc.p;
						dst.fi = src.fi;
					}
					UsedNode& dstusednode = *m_usedlist[dstusedindex];
					dstusednode.fi = fi;
					m_fixedlist[fi].p = &dstusednode;
				}
				else
				{
					UsedNode& srcusednode = *m_usedlist[srcusedindex];
					_SizeType fi = srcusednode.fi;
					for(_SizeType x = srcusedindex; x > dstusedindex; --x) // Backward move. x - 1 is src index, x is dst index.
					{
						UsedNode& src = *m_usedlist[x - 1];
						UsedNode& dst = *m_usedlist[x];
						FixedNode& fsrc = m_fixedlist[src.fi];
						FixedNode& fdst = m_fixedlist[dst.fi];
						fdst.p = fsrc.p;
						dst.fi = src.fi;
					}
					UsedNode& dstusednode = *m_usedlist[dstusedindex];
					dstusednode.fi = fi;
					m_fixedlist[fi].p = &dstusednode;
				}
				return GAIA::True;
			}
			GINL GAIA::BL swap(const _SizeType& usedindex1, const _SizeType& usedindex2)
			{
				GPCHR_TRUE_RET(usedindex1 < 0 || usedindex1 >= this->size(), GAIA::False);
				GPCHR_TRUE_RET(usedindex2 < 0 || usedindex2 >= this->size(), GAIA::False);
				if(usedindex1 == usedindex2)
					return GAIA::True;
				UsedNode& n1 = *m_usedlist[usedindex1];
				UsedNode& n2 = *m_usedlist[usedindex2];
				FixedNode& f1 = m_fixedlist[n1.fi];
				FixedNode& f2 = m_fixedlist[n2.fi];
				GAIA::ALGO::swap(f1.p, f2.p);
				GAIA::ALGO::swap(n1.fi, n2.fi);
				return GAIA::True;
			}
			GINL _SizeType find(const _DataType& t) const
			{
				for(_SizeType x = 0; x < this->size(); ++x)
				{
					if((*this)[x] == t)
						return x;
				}
				return GINVALID;
			}
			GINL const _DataType& operator[](const _SizeType& usedindex) const
			{
				_SizeType fixedindex = this->fixedindex(usedindex);
				GAST(fixedindex != GINVALID);
				return m_fixedlist[fixedindex].t;
			}
			GINL _DataType& operator[](const _SizeType& usedindex)
			{
				_SizeType fixedindex = this->fixedindex(usedindex);
				GAST(fixedindex != GINVALID);
				return m_fixedlist[fixedindex].t;
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				this->clear();
				m_fixedlist = src.m_fixedlist;
				m_freestack = src.m_freestack;
				m_usedlist = src.m_usedlist;
				for(_SizeType x = 0; x < m_usedlist.size(); ++x)
				{
					UsedNode* p = m_usedlist[x];
					GAST(p != GNIL);
					UsedNode* pNew = m_pool.alloc();
					*pNew = *p;
					m_usedlist[x] = pNew;
					m_fixedlist[pNew->fi].p = pNew;
				}
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
			GINL _DataType& front(){return *this->frontit();}
			GINL const _DataType& front() const{return *this->const_frontit();}
			GINL _DataType& back(){return *this->backit();}
			GINL const _DataType& back() const{return *this->const_backit();}
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
		private:
			class UsedNode : public GAIA::Base
			{
			public:
				_SizeType fi; // Fixed index.
				_SizeType ui; // Used index.
			};
			class FixedNode : public GAIA::Base
			{
			public:
				_DataType t;
				UsedNode* p;
			};
		private:
			typedef GAIA::CTN::BasicVector<FixedNode, _SizeType, _ExtendType> __FixedListType;
			typedef GAIA::CTN::BasicStack<_SizeType, _SizeType, _ExtendType> __FreeStackType; // Fixed list free stack.
			typedef GAIA::CTN::BasicVector<UsedNode*, _SizeType, _ExtendType> __UsedListType;
		private:
			GINL GAIA::GVOID init(){}
		private:
			__PoolType m_pool;
			__FixedListType m_fixedlist;
			__FreeStackType m_freestack;
			__UsedListType m_usedlist;
		};
		template<typename _DataType> class Book : public BasicBook<_DataType, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
