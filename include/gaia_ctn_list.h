#ifndef		__GAIA_CTN_LIST_H__
#define		__GAIA_CTN_LIST_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_pool.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicList : public GAIA::Base
		{
		private:
			class Node : public GAIA::Base
			{
			public:
				_DataType t;
				Node* pNext;
				Node* pPrev;
			};
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicList<_DataType, _SizeType, _ExtendType> __MyType;
			typedef BasicPool<Node, _SizeType, _ExtendType> __PoolType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicList;
			public:
				GINL it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pNode == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual GAIA::BL erase()
				{
					if(this->empty())
						return GAIA::False;
					return m_pContainer->erase(*this);
				}
				GINL virtual _DataType& operator * (){return m_pNode->t;}
				GINL virtual const _DataType& operator * () const{return m_pNode->t;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (){m_pNode = m_pNode->pNext; return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (){m_pNode = m_pNode->pPrev; return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator = (const GAIA::ITERATOR::Iterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator == (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator != (*GSCAST(const it*)(&src));}
				GINL GAIA::BL operator == (const it& src) const{return m_pNode == src.m_pNode;}
				GINL GAIA::BL operator != (const it& src) const{return !this->operator == (src);}
				GINL it& operator = (const it& src){GAST(&src != this); m_pContainer = src.m_pContainer; m_pNode = src.m_pNode; return *this;}
				GINL it& operator += (_SizeType c)
				{
					GAST(!this->empty());
					if(this->empty())
						return *this;
					while(c > 0)
					{
						++(*this);
						if(this->empty())
							return *this;
						--c;
					}
					while(c < 0)
					{
						--(*this);
						if(this->empty())
							return *this;
						++c;
					}
					return *this;
				}
				GINL it& operator -= (_SizeType c)
				{
					GAST(!this->empty());
					if(this->empty())
						return *this;
					while(c > 0)
					{
						--(*this);
						if(this->empty())
							return *this;
						--c;
					}
					while(c < 0)
					{
						++(*this);
						if(this->empty())
							return *this;
						++c;
					}
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
					it iter = *this;
					_SizeType ret = 0;
					for(; !iter.empty(); --iter)
					{
						if(iter == src)
							return ret;
						++ret;
					}
					iter = *this;
					ret = 0;
					for(; !iter.empty(); ++iter)
					{
						if(iter == src)
							return ret;
						--ret;
					}
					return ret;
				}
			private:
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
			private:
				GINL GAIA::GVOID init(){m_pContainer = GNIL; m_pNode = GNIL;}
			private:
				__MyType* m_pContainer;
				Node* m_pNode;
			};
			class const_it : public GAIA::ITERATOR::ConstIterator<_DataType>
			{
			private:
				friend class BasicList;
			public:
				GINL const_it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pNode == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual const _DataType& operator * () const{return m_pNode->t;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (){m_pNode = m_pNode->pNext; return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (){m_pNode = m_pNode->pPrev; return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator = (const GAIA::ITERATOR::ConstIterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator == (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator != (*GSCAST(const const_it*)(&src));}
				GINL GAIA::BL operator == (const const_it& src) const{return m_pNode == src.m_pNode;}
				GINL GAIA::BL operator != (const const_it& src) const{return !this->operator == (src);}
				GINL const_it& operator = (const const_it& src){GAST(&src != this); m_pNode = src.m_pNode; return *this;}
				GINL const_it& operator += (_SizeType c)
				{
					GAST(!this->empty());
					if(this->empty())
						return *this;
					while(c > 0)
					{
						++(*this);
						if(this->empty())
							return *this;
						--c;
					}
					while(c < 0)
					{
						--(*this);
						if(this->empty())
							return *this;
						++c;
					}
					return *this;
				}
				GINL const_it& operator -= (_SizeType c)
				{
					GAST(!this->empty());
					if(this->empty())
						return *this;
					while(c > 0)
					{
						--(*this);
						if(this->empty())
							return *this;
						--c;
					}
					while(c < 0)
					{
						++(*this);
						if(this->empty())
							return *this;
						++c;
					}
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
					const_it iter = *this;
					_SizeType ret = 0;
					for(; !iter.empty(); --iter)
					{
						if(iter == src)
							return ret;
						++ret;
					}
					iter = *this;
					ret = 0;
					for(; !iter.empty(); ++iter)
					{
						if(iter == src)
							return ret;
						--ret;
					}
					return ret;
				}
			private:
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
			private:
				GINL GAIA::GVOID init(){m_pNode = GNIL;}
			private:
				const Node* m_pNode;
			};
		public:
			GINL BasicList(){this->init();}
			GINL BasicList(const __MyType& src){this->init(); this->operator = (src);}
			GINL const _SizeType& capacity() const{return m_pool.capacity();}
			GINL GAIA::BL empty() const{return m_pool.empty();}
			GINL _SizeType size() const{return m_pool.size();}
			GINL GAIA::GVOID clear(){m_pFront = m_pBack = GNIL; m_pool.clear();}
			GINL GAIA::GVOID destroy(){m_pFront = m_pBack = GNIL; m_pool.destroy();}
			GINL GAIA::GVOID push_front(const _DataType& t)
			{
				it iter;
				iter.m_pNode = m_pFront;
				this->insert(t, iter);
			}
			GINL GAIA::BL pop_front()
			{
				it iter;
				iter.m_pNode = m_pFront;
				return this->erase(iter);
			}
			GINL GAIA::GVOID push_back(const _DataType& t)
			{
				it iter;
				iter.m_pNode = GNIL;
				this->insert(t, iter);
			}
			GINL GAIA::BL pop_back()
			{
				it iter;
				iter.m_pNode = m_pBack;
				return this->erase(iter);
			}
			GINL GAIA::BL insert(const _DataType& t, const _SizeType& index)
			{
				Node* pLast = GNIL;
				Node* pTemp = m_pFront;
				for(GAIA::NUM x = 0; x < index; ++x)
				{
					pLast = pTemp;
					if(pLast == GNIL)
						return GAIA::False;
					pTemp = pTemp->pNext;
				}
				Node* pNew = m_pool.alloc();
				pNew->t = t;
				pNew->pPrev = pLast;
				pNew->pNext = pTemp;
				if(pLast != GNIL)
					pLast->pNext = pNew;
				else
					m_pFront = pNew;
				if(pTemp != GNIL)
					pTemp->pPrev = pNew;
				else
					m_pBack = pNew;
				return GAIA::True;
			}
			GINL GAIA::GVOID insert(const _DataType& t, it iter)
			{
				if(iter.empty())
				{
					if(m_pBack == GNIL)
					{
						m_pFront = m_pBack = m_pool.alloc();
						m_pBack->t = t;
						m_pBack->pPrev = GNIL;
						m_pBack->pNext = GNIL;
					}
					else
					{
						Node* pNew = m_pool.alloc();
						pNew->t = t;
						m_pBack->pNext = pNew;
						pNew->pPrev = m_pBack;
						pNew->pNext = GNIL;
						m_pBack = pNew;
					}
				}
				else
				{
					Node* pNew = m_pool.alloc();
					pNew->t = t;
					pNew->pNext = iter.m_pNode;
					pNew->pPrev = iter.m_pNode->pPrev;
					if(iter.m_pNode->pPrev != GNIL)
						iter.m_pNode->pPrev->pNext = pNew;
					else
						m_pFront = pNew;
					iter.m_pNode->pPrev = pNew;
				}
			}
			GINL GAIA::BL erase(const _SizeType& index)
			{
				GAST(index >= 0 && index < this->size());
				if(index < 0)
					return GAIA::False;
				else if(index >= this->size())
					return GAIA::False;
				Node* pErase = m_pFront;
				for(_SizeType x = 0; x < index; ++x)
					pErase = pErase->pNext;
				if(pErase->pNext != GNIL)
					pErase->pNext->pPrev = pErase->pPrev;
				else
					m_pBack = pErase->pPrev;
				if(pErase->pPrev != GNIL)
					pErase->pPrev->pNext = pErase->pNext;
				else
					m_pFront = pErase->pNext;
				m_pool.release(pErase);
				return GAIA::True;
			}
			GINL GAIA::BL erase(it& iter)
			{
				if(iter.empty())
					return GAIA::False;
				Node* pErase = iter.m_pNode;
				if(pErase->pNext != GNIL)
					pErase->pNext->pPrev = pErase->pPrev;
				else
					m_pBack = pErase->pPrev;
				if(pErase->pPrev != GNIL)
					pErase->pPrev->pNext = pErase->pNext;
				else
					m_pFront = pErase->pNext;
				++iter;
				m_pool.release(pErase);
				return GAIA::True;
			}
			GINL GAIA::GVOID swap(it& iter1, it& iter2){GAIA::ALGO::swap(*iter1, *iter2);}
			GINL _DataType& front(){return m_pFront->t;}
			GINL const _DataType& front() const{return m_pFront->t;}
			GINL _DataType& back(){return m_pBack->t;}
			GINL const _DataType& back() const{return m_pBack->t;}
			GINL it frontit(){it ret; ret.m_pContainer = this; ret.m_pNode = m_pFront; return ret;}
			GINL it backit(){it ret; ret.m_pContainer = this; ret.m_pNode = m_pBack; return ret;}
			GINL const_it const_frontit() const{const_it ret; ret.m_pNode = m_pFront; return ret;}
			GINL const_it const_backit() const{const_it ret; ret.m_pNode = m_pBack; return ret;}
			GINL GAIA::GVOID inverse()
			{
				if(this->size() > 1)
					GAIA::ALGO::inverse(this->frontit(), this->backit());
			}
			GINL __MyType& operator += (const __MyType& src)
			{
				const_it it = src.const_frontit();
				_SizeType size = src.size();
				for(; !it.empty(); ++it)
				{
					this->push_back(*it);
					--size;
					if(size == 0)
						break;
				}
				return *this;
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				this->destroy();
				const_it iter = src.const_frontit();
				for(; !iter.empty(); ++iter)
					this->push_back(*iter);
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
			GINL _DataType& operator [] (const _SizeType& t)
			{
				Node* pTemp = m_pFront;
				for(_SizeType x = 0; x < t; ++x)
				{
					GAST(pTemp != GNIL);
					pTemp = pTemp->pNext;
				}
				return pTemp->t;
			}
			GINL const _DataType& operator [] (const _SizeType& t) const{return GCCAST(__MyType*)(this)->operator[](t);}
		private:
			GINL GAIA::GVOID init(){m_pFront = m_pBack = GNIL;}
		private:
			Node* m_pFront;
			Node* m_pBack;
			__PoolType m_pool;
		};
		template<typename _DataType> class List : public BasicList<_DataType, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
