#ifndef 	__GAIA_CTN_DOUBLELIST_H__
#define 	__GAIA_CTN_DOUBLELIST_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType> class BasicDoubleList : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;	
		public:
			typedef BasicDoubleList<_DataType, _SizeType> __MyType;
		public:
			class Node : public GAIA::Base
			{
			public:
				_DataType t;
				Node* pPrev;
				Node* pNext;
			};

			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicDoubleList;
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
				friend class BasicDoubleList;
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
				GINL GAIA::GVOID init(){m_pContainer = GNIL; m_pNode = GNIL;}
			private:
				const __MyType* m_pContainer;
				const Node* m_pNode;
			};
		public:
			GINL BasicDoubleList(){this->init();}
			GINL BasicDoubleList(const __MyType& src){this->init(); this->operator = (src);}
			GINL ~BasicDoubleList(){this->destroy();}
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
					m_pFront->pPrev = GNIL;
					m_pFront->pNext = GNIL;
					return;
				}
		
				Node* pTemp = m_pFront;
				for(; pTemp->pNext != GNIL; pTemp = pTemp->pNext){}
				pTemp->pNext = gnew Node;
				pTemp->pNext->t = t;
				pTemp->pNext->pPrev = pTemp;
				pTemp->pNext->pNext = GNIL;
			}
			GINL GAIA::BL pop_back()
			{
				if(this->empty())
					return GAIA::False;
				Node* pTemp = m_pFront;
				for(; pTemp->pNext != GNIL; pTemp = pTemp->pNext){}
				if(pTemp->pPrev != GNIL)
					pTemp->pPrev->pNext = GNIL;
				if(m_pFront == pTemp)
					m_pFront = GNIL;
				gdel pTemp;
				return GAIA::True;
			}
			GINL GAIA::GVOID push_front(const _DataType& t)
			{
				Node* pTemp = gnew Node;
				pTemp->t = t;
				pTemp->pPrev = GNIL;
				pTemp->pNext = m_pFront;
				if(m_pFront != GNIL)
					m_pFront->pPrev = pTemp;
				m_pFront = pTemp;
			}
			GINL GAIA::BL pop_front()
			{
				if(this->empty())
					return GAIA::False;
				Node* pTemp = m_pFront;
				m_pFront = m_pFront->pNext;
				gdel pTemp;
				if(m_pFront != GNIL)
					m_pFront->pPrev = GNIL;
				return GAIA::True;
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
				Node* pNew = gnew Node;
				pNew->t = t;
				pNew->pPrev = pLast;
				pNew->pNext = pTemp;
				if(pLast != GNIL)
					pLast->pNext = pNew;
				else
					m_pFront = pNew;
				if(pTemp != GNIL)
					pTemp->pPrev = pNew;
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
						pNew->pPrev = pTemp;
					}
					else
					{
						m_pFront = pNew;
						pNew->pPrev = GNIL;
					}
				}
				else
				{
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
				if(this->empty())
					return GAIA::False;
				Node* pErase = m_pFront;
				for(_SizeType x = 0; x < index; ++x)
				{
					pErase = pErase->pNext;
					if(pErase == GNIL)
						return GAIA::False;
				}
				if(pErase->pNext != GNIL)
					pErase->pNext->pPrev = pErase->pPrev;
				if(pErase->pPrev != GNIL)
					pErase->pPrev->pNext = pErase->pNext;
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
				Node* pErase = iter.m_pNode;
				if(pErase->pNext != GNIL)
					pErase->pNext->pPrev = pErase->pPrev;
				if(pErase->pPrev != GNIL)
					pErase->pPrev->pNext = pErase->pNext;
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
		private:
			Node* m_pFront;
		};
		template<typename _DataType> class DoubleList : public BasicDoubleList<_DataType, GAIA::NUM>{public:};
	}
}

#endif
