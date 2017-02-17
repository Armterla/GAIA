#ifndef 	__GAIA_CTN_SINGLELIST_H__
#define 	__GAIA_CTN_SINGLELIST_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType> class BasicSingleList : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;	
		public:
			typedef BasicSingleList<_DataType, _SizeType> __MyType;
		public:
			class Node : public GAIA::Base
			{
			public:
				_DataType t;
				Node* pNext;
			};

			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicSingleList;
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
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (){m_pNode = m_pContainer->prevnode(*m_pNode); return *this;}
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
					_SizeType index = m_pContainer->nodeindex(*m_pNode);
					index -= c;
					*this = m_pContainer->index2it(index);
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
				friend class BasicSingleList;
			public:
				GINL const_it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pNode == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual const _DataType& operator * () const{return m_pNode->t;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (){m_pNode = m_pNode->pNext; return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (){m_pNode = m_pContainer->prevnode(*m_pNode); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator = (const GAIA::ITERATOR::ConstIterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator == (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator != (*GSCAST(const const_it*)(&src));}
				GINL GAIA::BL operator == (const const_it& src) const{return m_pNode == src.m_pNode;}
				GINL GAIA::BL operator != (const const_it& src) const{return !this->operator == (src);}
				GINL const_it& operator = (const const_it& src){GAST(&src != this); m_pContainer = src.m_pContainer; m_pNode = src.m_pNode; return *this;}
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
					_SizeType index = m_pContainer->nodeindex(*m_pNode);
					index -= c;
					*this = m_pContainer->index2constit(index);
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
				GINL GAIA::GVOID init(){m_pContainer = GNIL; m_pNode = GNIL;}
			private:
				const __MyType* m_pContainer;
				const Node* m_pNode;
			};
		public:
			GINL BasicSingleList(){this->init();}
			GINL BasicSingleList(const __MyType& src){this->init(); this->operator = (src);}
			GINL ~BasicSingleList(){this->destroy();}
			GINL GAIA::BL empty() const{return m_pFront == GNIL;}
			GINL _SizeType size() const
			{
				_SizeType ret = 0;
				for(Node* pTemp = m_pFront; pTemp != GNIL; )
				{
					pTemp = pTemp->pNext;
					++ret;
				}
				return ret;
			}
			GINL GAIA::GVOID clear(){this->destroy();}
			GINL GAIA::GVOID destroy()
			{
				for(; m_pFront != GNIL; )
				{
					Node* pTemp = m_pFront;
					m_pFront = m_pFront->pNext;
					gdel pTemp;
				}
			}
			GINL GAIA::GVOID push_back(const _DataType& t)
			{
				if(this->empty())
				{
					m_pFront = gnew Node;
					m_pFront->t = t;
					m_pFront->pNext = GNIL;
					return;
				}
		
				Node* pTemp = m_pFront;
				for(; pTemp->pNext != GNIL; pTemp = pTemp->pNext){}
				pTemp->pNext = gnew Node;
				pTemp->pNext->t = t;
				pTemp->pNext->pNext = GNIL;
			}
			GINL GAIA::BL pop_back()
			{
				if(this->empty())
					return GAIA::False;
				Node* pTemp = m_pFront;
				Node* pLast = GNIL;
				for(; pTemp->pNext != GNIL; pTemp = pTemp->pNext)
					pLast = pTemp;
				gdel pTemp;
				if(pLast != GNIL)
					pLast->pNext = GNIL;
				if(pTemp == m_pFront)
					m_pFront = GNIL;
				return GAIA::True;
			}
			GINL GAIA::GVOID push_front(const _DataType& t)
			{
				Node* pTemp = gnew Node;
				pTemp->t = t;
				pTemp->pNext = m_pFront;
				m_pFront = pTemp;
			}
			GINL GAIA::BL pop_front()
			{
				if(this->empty())
					return GAIA::False;
				Node* pTemp = m_pFront;
				m_pFront = m_pFront->pNext;
				gdel pTemp;
				return GAIA::True;
			}
			GINL GAIA::BL insert(const _DataType& t, const _SizeType& index)
			{
				Node* pLast = GNIL;
				Node* pTemp = m_pFront;
				for(_SizeType x = 0; x < index; ++x)
				{
					pLast = pTemp;
					if(pLast == GNIL)
						return GAIA::False;
					pTemp = pTemp->pNext;
				}
				Node* pNew = gnew Node;
				pNew->t = t;
				pNew->pNext = pTemp;
				if(pLast != GNIL)
					pLast->pNext = pNew;
				else
					m_pFront = pNew;
				return GAIA::True;
			}
			GINL GAIA::GVOID insert(const _DataType& t, it iter)
			{
				Node* pNew = gnew Node;
				pNew->t = t;
				if(iter.empty())
				{
					pNew->pNext = GNIL;
					Node* pTemp = m_pFront;
					if(pTemp != GNIL)
					{
						for(; pTemp->pNext != GNIL; pTemp = pTemp->pNext){}
						GAST(pTemp != GNIL);
						pTemp->pNext = pNew;
					}
					else
						m_pFront = pNew;
				}
				else
				{
					Node* pPrev = this->prevnode(*iter.m_pNode);
					pNew->pNext = iter.m_pNode;
					if(pPrev != GNIL)
						pPrev->pNext = pNew;
					else
						m_pFront = pNew;
				}
			}
			GINL GAIA::BL erase(const _SizeType& index)
			{
				if(this->empty())
					return GAIA::False;
				Node* pLast = GNIL;
				Node* pErase = m_pFront;
				for(_SizeType x = 0; x < index; ++x)
				{
					pLast = pErase;
					pErase = pErase->pNext;
					if(pErase == GNIL)
						return GAIA::False;
				}
				if(pLast != GNIL)
					pLast->pNext = pErase->pNext;
				else
					m_pFront = pErase->pNext;
				gdel pErase;
				return GAIA::True;
			}
			GINL GAIA::BL erase(it& iter)
			{
				GAST(!iter.empty());
				if(iter.empty())
					return GAIA::False;
				Node* pLast = m_pFront;
				for(; pLast != GNIL && pLast->pNext != iter.m_pNode; pLast = pLast->pNext){}
				Node* pErase = iter.m_pNode;
				if(pLast != GNIL)
					pLast->pNext = pErase->pNext;
				else
					m_pFront = pErase->pNext;
				++iter;
				gdel pErase;
				return GAIA::True;
			}
			GINL GAIA::GVOID swap(it& iter1, it& iter2){GAIA::ALGO::swap(*iter1, *iter2);}
			GINL const _DataType& front() const{return GCCAST(__MyType*)(this)->back();}
			GINL const _DataType& back() const{return GCCAST(__MyType*)(this)->front();}
			GINL _DataType& front(){GAST(m_pFront != GNIL); return m_pFront->t;}
			GINL _DataType& back()
			{
				GAST(m_pFront != GNIL);
				Node* pTemp = m_pFront;
				for(; pTemp->pNext != GNIL; pTemp = pTemp->pNext){}
				return pTemp->t;
			}
			GINL it frontit(){return this->node2it(m_pFront);}
			GINL it backit()
			{
				if(this->empty())
					return this->node2it(GNIL);
				Node* pTemp = m_pFront;
				for(; pTemp->pNext != GNIL; pTemp = pTemp->pNext){}
				return this->node2it(pTemp);
			}
			GINL const_it const_frontit() const{return this->node2constit(m_pFront);}
			GINL const_it const_backit() const
			{
				if(this->empty())
					return this->node2constit(GNIL);
				const Node* pTemp = m_pFront;
				for(; pTemp->pNext != GNIL; pTemp = pTemp->pNext){}
				return this->node2constit(pTemp);
			}
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
				this->destroy();
				const Node* pNode = src.m_pFront;
				for(; pNode != GNIL; pNode = pNode->pNext)
					this->push_back(pNode->t);
				return *this;
			}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				const Node* pSrc = m_pFront;
				const Node* pDst = src.m_pFront;
				for(;;)
				{
					if(pSrc == GNIL && pDst == GNIL)
						return 0;
					else if(pSrc == GNIL && pDst != GNIL)
						return -1;
					else if(pSrc != GNIL && pDst == GNIL)
						return +1;
					else
					{
						if(pSrc->t < pDst->t)
							return -1;
						else if(pSrc->t > pDst->t)
							return +1;
						pSrc = pSrc->pNext;
						pDst = pDst->pNext;
					}
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
			GINL GAIA::GVOID init(){m_pFront = GNIL;}
			GINL _SizeType nodeindex(const Node& n) const
			{
				_SizeType ret = 0;
				const Node* pTemp = m_pFront;
				for(; &n != pTemp; pTemp = pTemp->pNext, ++ret){}
				return ret;
			}
			GINL it node2it(Node* pNode)
			{
				it ret;
				ret.m_pContainer = this;
				ret.m_pNode = pNode;
				return ret;
			}
			GINL const_it node2constit(const Node* pNode) const
			{
				const_it ret;
				ret.m_pContainer = this;
				ret.m_pNode = pNode;
				return ret;
			}
			GINL it index2it(const _SizeType& index)
			{
				Node* pTemp = m_pFront;
				for(_SizeType x = 0; x < index; ++x)
					pTemp = pTemp->pNext;
				return this->node2it(pTemp);
			}
			GINL const_it index2constit(const _SizeType& index) const
			{
				const Node* pTemp = m_pFront;
				for(_SizeType x = 0; x < index; ++x)
					pTemp = pTemp->pNext;
				return this->node2constit(pTemp);
			}
			GINL const Node* prevnode(const Node& n) const{return GCCAST(__MyType*)(this)->prevnode(GCCAST(Node&)(n));}
			GINL Node* prevnode(Node& n)
			{
				Node* pTemp = m_pFront;
				for(; pTemp != GNIL && pTemp->pNext != &n; pTemp = pTemp->pNext){}
				return pTemp;
			}
		private:
			Node* m_pFront;
		};
		template<typename _DataType> class SingleList : public BasicSingleList<_DataType, GAIA::NUM>{public:};
	}
}

#endif
