#ifndef 	__GAIA_CTN_AVLTREE_H__
#define 	__GAIA_CTN_AVLTREE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_pool.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _HeightType, typename _ExtendType> class BasicAVLTree : public GAIA::Base
		{
		private:
			friend class it;
			friend class const_it;
		private:
			class Node : public GAIA::Base
			{
			public:
				GCLASS_COMPARE(t, Node)
				_DataType t;
				_HeightType h;
				Node* pPrev;
				Node* pNext;
				Node* pParent;
			};
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _HeightType _heighttype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicAVLTree<_DataType, _SizeType, _HeightType, _ExtendType> __MyType;
			typedef BasicPool<Node, _SizeType, _ExtendType> __PoolType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicAVLTree;
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
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ ()
				{
					if(m_pNode == GNIL)
						return *this;
					m_pNode = this->select_next(m_pNode);
					return *this;
				}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- ()
				{
					if(m_pNode == GNIL)
						return *this;
					m_pNode = this->select_prev(m_pNode);
					return *this;
				}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator = (const GAIA::ITERATOR::Iterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator == (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator != (*GSCAST(const it*)(&src));}
				GINL it& operator = (const it& src){GAST(&src != this); m_pNode = src.m_pNode; m_pContainer = src.m_pContainer; return *this;}
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
					it iter = *this;
					_SizeType ret = 0;
					for(; iter > src; --iter)
						++ret;
					for(; iter < src; ++iter)
						--ret;
					return ret;
				}
				GINL GAIA::BL operator == (const it& src) const{return GAIA::ALGO::cmpp(m_pNode, src.m_pNode) == 0;}
				GINL GAIA::BL operator != (const it& src) const{return !(*this == src);}
				GINL GAIA::BL operator >= (const it& src) const{return GAIA::ALGO::cmpp(m_pNode, src.m_pNode) >= 0;}
				GINL GAIA::BL operator <= (const it& src) const{return GAIA::ALGO::cmpp(m_pNode, src.m_pNode) <= 0;}
				GINL GAIA::BL operator > (const it& src) const{return !(*this <= src);}
				GINL GAIA::BL operator < (const it& src) const{return !(*this >= src);}
			private:
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
				GINL Node* select_next(Node* pNode)
				{
					if(pNode->pNext != GNIL)
						pNode = m_pContainer->front_node(pNode->pNext);
					else
					{
						for(;;)
						{
							if(pNode->pParent == GNIL)
							{
								pNode = GNIL;
								break;
							}
							else
							{
								if(pNode == pNode->pParent->pPrev)
								{
									pNode = pNode->pParent;
									break;
								}
								else
									pNode = pNode->pParent;
							}
						}
					}
					return pNode;
				}
				GINL Node* select_prev(Node* pNode)
				{
					if(pNode->pPrev != GNIL)
						pNode = m_pContainer->back_node(pNode->pPrev);
					else
					{
						for(;;)
						{
							if(pNode->pParent == GNIL)
							{
								pNode = GNIL;
								break;
							}
							else
							{
								if(pNode == pNode->pParent->pNext)
								{
									pNode = pNode->pParent;
									break;
								}
								else
									pNode = pNode->pParent;
							}
						}
					}
					return pNode;
				}
			private:
				GINL GAIA::GVOID init(){m_pNode = GNIL; m_pContainer = GNIL;}
			private:
				Node* m_pNode;
				__MyType* m_pContainer;
			};
			class const_it : public GAIA::ITERATOR::ConstIterator<_DataType>
			{
			private:
				friend class BasicAVLTree;
			public:
				GINL const_it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pNode == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual const _DataType& operator * () const{return m_pNode->t;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ ()
				{
					if(m_pNode == GNIL)
						return *this;
					m_pNode = this->select_next(m_pNode);
					return *this;
				}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- ()
				{
					if(m_pNode == GNIL)
						return *this;
					m_pNode = this->select_prev(m_pNode);
					return *this;
				}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator = (const GAIA::ITERATOR::ConstIterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator == (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator != (*GSCAST(const const_it*)(&src));}
				GINL const_it& operator = (const const_it& src){GAST(&src != this); m_pNode = src.m_pNode; m_pContainer = src.m_pContainer; return *this;}
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
					const_it iter = *this;
					_SizeType ret = 0;
					for(; iter > src; --iter)
						++ret;
					for(; iter < src; ++iter)
						--ret;
					return ret;
				}
				GINL GAIA::BL operator == (const const_it& src) const{return GAIA::ALGO::cmpp(m_pNode, src.m_pNode) == 0;}
				GINL GAIA::BL operator != (const const_it& src) const{return !(*this == src);}
				GINL GAIA::BL operator >= (const const_it& src) const{return GAIA::ALGO::cmpp(m_pNode, src.m_pNode) >= 0;}
				GINL GAIA::BL operator <= (const const_it& src) const{return GAIA::ALGO::cmpp(m_pNode, src.m_pNode) <= 0;}
				GINL GAIA::BL operator > (const const_it& src) const{return !(*this <= src);}
				GINL GAIA::BL operator < (const const_it& src) const{return !(*this >= src);}
			private:
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
				GINL const Node* select_next(const Node* pNode)
				{
					if(pNode->pNext != GNIL)
						pNode = m_pContainer->front_node(pNode->pNext);
					else
					{
						for(;;)
						{
							if(pNode->pParent == GNIL)
							{
								pNode = GNIL;
								break;
							}
							else
							{
								if(pNode == pNode->pParent->pPrev)
								{
									pNode = pNode->pParent;
									break;
								}
								else
									pNode = pNode->pParent;
							}
						}
					}
					return pNode;
				}
				GINL const Node* select_prev(const Node* pNode)
				{
					if(pNode->pPrev != GNIL)
						pNode = m_pContainer->back_node(pNode->pPrev);
					else
					{
						for(;;)
						{
							if(pNode->pParent == GNIL)
							{
								pNode = GNIL;
								break;
							}
							else
							{
								if(pNode == pNode->pParent->pNext)
								{
									pNode = pNode->pParent;
									break;
								}
								else
									pNode = pNode->pParent;
							}
						}
					}
					return pNode;
				}
			private:
				GINL GAIA::GVOID init(){m_pNode = GNIL; m_pContainer = GNIL;}
			private:
				const Node* m_pNode;
				const __MyType* m_pContainer;
			};
		public:
			GINL BasicAVLTree(){this->init();}
			GINL BasicAVLTree(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return m_pool.empty();}
			GINL _SizeType size() const{return m_pool.size();}
			GINL const _SizeType& capacity() const{return m_pool.capacity();}
			GINL GAIA::GVOID clear(){m_pRoot = GNIL; m_pool.clear();}
			GINL GAIA::GVOID destroy(){m_pRoot = GNIL; m_pool.destroy();}
			GINL GAIA::BL insert(const _DataType& t, typename __MyType::it* pIter = GNIL)
			{
				if(m_pRoot == GNIL)
				{
					m_pRoot = m_pool.alloc();
					m_pRoot->t = t;
					m_pRoot->h = 1;
					m_pRoot->pPrev = GNIL;
					m_pRoot->pNext = GNIL;
					m_pRoot->pParent = GNIL;
					if(pIter != GNIL)
					{
						pIter->m_pNode = m_pRoot;
						pIter->m_pContainer = this;
					}
					return GAIA::True;
				}
				else
				{
					GAIA::BL bResult = GAIA::False;
					Node* pResultNode;
					m_pRoot = this->insert_node(m_pRoot, &pResultNode, t, bResult);
					m_pRoot->pParent = GNIL;
					if(pIter != GNIL)
					{
						pIter->m_pNode = pResultNode;
						pIter->m_pContainer = this;
					}
					return bResult;
				}
			}
			GINL GAIA::BL erase(const _DataType& t)
			{
				if(m_pRoot == GNIL)
					return GAIA::False;
				GAIA::BL bResult = GAIA::False;
				m_pRoot = this->erase_node(m_pRoot, t, bResult);
				if(m_pRoot != GNIL)
					m_pRoot->pParent = GNIL;
				return bResult;
			}
			GINL GAIA::BL erase(it& iter)
			{
				if(iter.empty())
					return GAIA::False;
				_DataType t = *iter;
				if(!this->erase(t))
					return GAIA::False;
				iter = this->upper_equal(t);
				return GAIA::True;
			}
			GINL _DataType* find(const _DataType& t)
			{
				if(m_pRoot == GNIL)
					return GNIL;
				return this->find_node(m_pRoot, t);
			}
			GINL const _DataType* find(const _DataType& t) const
			{
				if(m_pRoot == GNIL)
					return GNIL;
				return this->find_node(m_pRoot, t);
			}
			GINL it upper_equal(const _DataType& t)
			{
				it iter;
				if(m_pRoot == GNIL)
				{
					iter.m_pNode = GNIL;
					iter.m_pContainer = GNIL;
					return iter;
				}
				iter.m_pNode = this->lower_bound_node(m_pRoot, t);
				if(iter.m_pNode != GNIL)
					iter.m_pContainer = this;
				return iter;
			}
			GINL it lower_equal(const _DataType& t)
			{
				it iter;
				if(m_pRoot == GNIL)
				{
					iter.m_pNode = GNIL;
					iter.m_pContainer = GNIL;
					return iter;
				}
				iter.m_pNode = this->upper_bound_node(m_pRoot, t);
				if(iter.m_pNode != GNIL)
					iter.m_pContainer = this;
				return iter;
			}
			GINL const_it upper_equal(const _DataType& t) const
			{
				const_it iter;
				if(m_pRoot == GNIL)
				{
					iter.m_pNode = GNIL;
					iter.m_pContainer = GNIL;
					return iter;
				}
				iter.m_pNode = this->lower_bound_node(m_pRoot, t);
				if(iter.m_pNode != GNIL)
					iter.m_pContainer = this;
				return iter;
			}
			GINL const_it lower_equal(const _DataType& t) const
			{
				const_it iter;
				if(m_pRoot == GNIL)
				{
					iter.m_pNode = GNIL;
					iter.m_pContainer = GNIL;
					return iter;
				}
				iter.m_pNode = this->upper_bound_node(m_pRoot, t);
				if(iter.m_pNode != GNIL)
					iter.m_pContainer = this;
				return iter;
			}
			GINL const _DataType* minimize() const{if(m_pRoot == GNIL) return GNIL; return &this->findmin(m_pRoot);}
			GINL _DataType* minimize(){if(m_pRoot == GNIL) return GNIL; return &this->findmin(m_pRoot);}
			GINL const _DataType* maximize() const{if(m_pRoot == GNIL) return GNIL; return &this->findmax(m_pRoot);}
			GINL _DataType* maximize(){if(m_pRoot == GNIL) return GNIL; return &this->findmax(m_pRoot);}
			GINL _DataType& front(){return *this->frontit();}
			GINL const _DataType& front() const{return *this->const_frontit();}
			GINL _DataType& back(){return *this->backit();}
			GINL const _DataType& back() const{return *this->const_backit();}
			GINL it frontit()
			{
				it iter;
				if(m_pRoot == GNIL)
				{
					iter.m_pNode = GNIL;
					iter.m_pContainer = GNIL;
				}
				else
				{
					iter.m_pNode = this->front_node(m_pRoot);
					iter.m_pContainer = this;
				}
				return iter;
			}
			GINL it backit()
			{
				it iter;
				if(m_pRoot == GNIL)
				{
					iter.m_pNode = GNIL;
					iter.m_pContainer = GNIL;
				}
				else
				{
					iter.m_pNode = this->back_node(m_pRoot);
					iter.m_pContainer = this;
				}
				return iter;
			}
			GINL const_it const_frontit() const
			{
				const_it iter;
				if(m_pRoot == GNIL)
				{
					iter.m_pNode = GNIL;
					iter.m_pContainer = GNIL;
				}
				else
				{
					iter.m_pNode = this->front_node(m_pRoot);
					iter.m_pContainer = this;
				}
				return iter;
			}
			GINL const_it const_backit() const
			{
				const_it iter;
				if(m_pRoot == GNIL)
				{
					iter.m_pNode = GNIL;
					iter.m_pContainer = GNIL;
				}
				else
				{
					iter.m_pNode = this->back_node(m_pRoot);
					iter.m_pContainer = this;
				}
				return iter;
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				this->clear();
				if(!src.empty())
				{
					const_it it = src.const_frontit();
					for(; !it.empty(); ++it)
						this->insert(*it);
				}
				return *this;
			}
			GINL __MyType& operator += (const __MyType& src)
			{
				if(this == &src)
					GTHROW_RET(InvalidParam, *this);
				for(__MyType::const_it it = src.const_frontit(); !it.empty(); ++it)
					this->insert(*it);
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
			GINL GAIA::GVOID init(){m_pRoot = GNIL;}
			GINL GAIA::GVOID rotate_prev(Node*& pNode)
			{
				Node* pTemp = pNode->pNext;
				pNode->pNext = pTemp->pPrev;
				if(pNode->pNext != GNIL)
					pNode->pNext->pParent = pNode;
				pTemp->pPrev = pNode;
				if(pTemp->pPrev != GNIL)
					pTemp->pPrev->pParent = pTemp;
				pNode->h = this->height(pNode);
				pTemp->h = this->height(pTemp);
				pNode = pTemp;
			}
			GINL GAIA::GVOID rotate_next(Node*& pNode)
			{
				Node* pTemp = pNode->pPrev;
				pNode->pPrev = pTemp->pNext;
				if(pNode->pPrev != GNIL)
					pNode->pPrev->pParent = pNode;
				pTemp->pNext = pNode;
				if(pTemp->pNext != GNIL)
					pTemp->pNext->pParent = pTemp;
				pNode->h = this->height(pNode);
				pTemp->h = this->height(pTemp);
				pNode = pTemp;
			}
			GINL _HeightType height(Node* pNode)
			{
				_HeightType prevh = pNode->pPrev == GNIL ? 0 : pNode->pPrev->h;
				_HeightType nexth = pNode->pNext == GNIL ? 0 : pNode->pNext->h;
				return GAIA::ALGO::gmax(prevh, nexth) + 1;
			}
			GINL GAIA::GVOID balance(Node*& pNode)
			{
				_HeightType prevh = pNode->pPrev == GNIL ? 0 : pNode->pPrev->h;
				_HeightType nexth = pNode->pNext == GNIL ? 0 : pNode->pNext->h;
				if(prevh > nexth + 1)
				{
					_HeightType prevnexth = pNode->pPrev->pNext == GNIL ? 0 : pNode->pPrev->pNext->h;
					_HeightType prevprevh = pNode->pPrev->pPrev == GNIL ? 0 : pNode->pPrev->pPrev->h;
					if(prevnexth > prevprevh)
						this->rotate_prev(pNode->pPrev);
					this->rotate_next(pNode);
				}
				else if(nexth > prevh + 1)
				{
					_HeightType nextprevh = pNode->pNext->pPrev == GNIL ? 0 : pNode->pNext->pPrev->h;
					_HeightType nextnexth = pNode->pNext->pNext == GNIL ? 0 : pNode->pNext->pNext->h;
					if(nextprevh > nextnexth)
						this->rotate_next(pNode->pNext);
					this->rotate_prev(pNode);
				}
			}
			GINL Node* front_node(Node* pNode)
			{
				if(pNode->pPrev != GNIL)
					return this->front_node(pNode->pPrev);
				else
					return pNode;
			}
			GINL Node* back_node(Node* pNode)
			{
				if(pNode->pNext != GNIL)
					return this->back_node(pNode->pNext);
				else
					return pNode;
			}
			GINL const Node* front_node(const Node* pNode) const
			{
				if(pNode->pPrev != GNIL)
					return this->front_node(pNode->pPrev);
				else
					return pNode;
			}
			GINL const Node* back_node(const Node* pNode) const
			{
				if(pNode->pNext != GNIL)
					return this->back_node(pNode->pNext);
				else
					return pNode;
			}
			GINL _DataType& findmin(Node* pNode) const
			{
				if(pNode->pPrev == GNIL)
					return pNode->t;
				return this->findmin(pNode->pPrev);
			}
			GINL _DataType& findmax(Node* pNode) const
			{
				if(pNode->pNext == GNIL)
					return pNode->t;
				return this->findmax(pNode->pNext);
			}
			GINL Node* insert_node(Node* pNode, Node** ppResultNode, const _DataType& t, GAIA::BL& bResult)
			{
				if(pNode == GNIL)
				{
					bResult = GAIA::True;
					Node* pNewNode = m_pool.alloc();
					pNewNode->t = t;
					pNewNode->h = 1;
					pNewNode->pPrev = GNIL;
					pNewNode->pNext = GNIL;
					pNewNode->pParent = GNIL;
					*ppResultNode = pNewNode;
					return pNewNode;
				}
				if(t < pNode->t)
				{
					pNode->pPrev = this->insert_node(pNode->pPrev, ppResultNode, t, bResult);
					if(pNode->pPrev != GNIL)
						pNode->pPrev->pParent = pNode;
				}
				else if(t > pNode->t)
				{
					pNode->pNext = this->insert_node(pNode->pNext, ppResultNode, t, bResult);
					if(pNode->pNext != GNIL)
						pNode->pNext->pParent = pNode;
				}
				else
				{
					*ppResultNode = pNode;
					return pNode;
				}
				pNode->h = this->height(pNode);
				this->balance(pNode);
				return pNode;
			}
			GINL Node* erase_node(Node* pNode, const _DataType& t, GAIA::BL& bResult)
			{
				if(pNode == GNIL)
					return GNIL;
				if(t < pNode->t)
				{
					pNode->pPrev = this->erase_node(pNode->pPrev, t, bResult);
					if(pNode->pPrev != GNIL)
						pNode->pPrev->pParent = pNode;
				}
				else if(t > pNode->t)
				{
					pNode->pNext = this->erase_node(pNode->pNext, t, bResult);
					if(pNode->pNext != GNIL)
						pNode->pNext->pParent = pNode;
				}
				else
				{
					bResult = GAIA::True;
					if(pNode->h == 1)
					{
						m_pool.release(pNode);
						return GNIL;
					}
					else if(pNode->pPrev != GNIL && pNode->pNext == GNIL)
					{
						pNode->pPrev->pParent = pNode->pParent;
						Node* pTemp = pNode;
						pNode = pNode->pPrev;
						m_pool.release(pTemp);
						return pNode;
					}
					else if(pNode->pPrev == GNIL && pNode->pNext != GNIL)
					{
						pNode->pNext->pParent = pNode->pParent;
						Node* pTemp = pNode;
						pNode = pNode->pNext;
						m_pool.release(pTemp);
						return pNode;
					}
					else if(pNode->pPrev != GNIL && pNode->pNext != GNIL)
					{
						const _DataType& mindata = this->findmin(pNode->pNext);
						pNode->t = mindata;
						pNode->pNext = this->erase_node(pNode->pNext, pNode->t, bResult);
						if(pNode->pNext != GNIL)
							pNode->pNext->pParent = pNode;
					}
				}
				pNode->h = this->height(pNode);
				this->balance(pNode);
				return pNode;
			}
			GINL _DataType* find_node(Node* pNode, const _DataType& t)
			{
				if(t < pNode->t)
				{
					if(pNode->pPrev != GNIL)
						return this->find_node(pNode->pPrev, t);
				}
				else if(t > pNode->t)
				{
					if(pNode->pNext != GNIL)
						return this->find_node(pNode->pNext, t);
				}
				else
					return &pNode->t;
				return GNIL;
			}
			GINL const _DataType* find_node(Node* pNode, const _DataType& t) const
			{
				if(t < pNode->t)
				{
					if(pNode->pPrev != GNIL)
						return this->find_node(pNode->pPrev, t);
				}
				else if(t > pNode->t)
				{
					if(pNode->pNext != GNIL)
						return this->find_node(pNode->pNext, t);
				}
				else
					return &pNode->t;
				return GNIL;
			}
			GINL Node* lower_bound_node(Node* pNode, const _DataType& t) const
			{
				if(pNode == GNIL)
					return GNIL;
				if(pNode->t < t)
					return this->lower_bound_node(pNode->pNext, t);
				else
				{
					Node* pNew = this->lower_bound_node(pNode->pPrev, t);
					if(pNew == GNIL)
						return pNode;
					return pNew;
				}
			}
			GINL Node* upper_bound_node(Node* pNode, const _DataType& t) const
			{
				if(pNode == GNIL)
					return GNIL;
				if(pNode->t > t)
					return this->upper_bound_node(pNode->pPrev, t);
				else
				{
					Node* pNew = this->upper_bound_node(pNode->pNext, t);
					if(pNew == GNIL)
						return pNode;
					return pNew;
				}
			}
		private:
			Node* m_pRoot;
			__PoolType m_pool;
		public:
		#ifdef GAIA_DEBUG_INTERNALROUTINE
			GINL GAIA::BL dbg_check_balance()
			{
				if(m_pRoot == GNIL)
					return GAIA::True;
				else
					return this->dbg_check_balance_node(m_pRoot);
			}
			GINL GAIA::BL dbg_check_balance_node(Node* pNode)
			{
				if(pNode == GNIL)
					return GAIA::True;
				_HeightType prevh = pNode->pPrev == GNIL ? 0 : pNode->pPrev->h;
				_HeightType nexth = pNode->pNext == GNIL ? 0 : pNode->pNext->h;
				if(prevh > nexth + 1 || nexth > prevh + 1)
					return GAIA::False;
				if(!this->dbg_check_balance_node(pNode->pPrev))
					return GAIA::False;
				if(!this->dbg_check_balance_node(pNode->pNext))
					return GAIA::False;
				return GAIA::True;
			}
			GINL GAIA::BL dbg_check_parent()
			{
				if(m_pRoot == GNIL)
					return GAIA::True;
				else
				{
					if(m_pRoot->pParent != GNIL)
						return GAIA::False;
					return this->dbg_check_parent_node(m_pRoot);
				}
			}
			GINL GAIA::BL dbg_check_parent_node(Node* pNode)
			{
				if(pNode == GNIL)
					return GAIA::True;
				if(pNode->pPrev != GNIL)
				{
					if(pNode->pPrev->pParent != pNode)
						return GAIA::False;
				}
				if(pNode->pNext != GNIL)
				{
					if(pNode->pNext->pParent != pNode)
						return GAIA::False;
				}
				if(!this->dbg_check_parent_node(pNode->pPrev))
					return GAIA::False;
				if(!this->dbg_check_parent_node(pNode->pNext))
					return GAIA::False;
				return GAIA::True;
			}
		#endif
		};
	}
}

#endif
