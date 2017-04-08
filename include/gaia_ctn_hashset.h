#ifndef		__GAIA_CTN_HASHSET_H__
#define		__GAIA_CTN_HASHSET_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"
#include "gaia_algo_extend.h"
#include "gaia_algo_hash.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_pool.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _HashType, typename _ExtendType> class BasicHashSet : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _HashType _hashtype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicHashSet<_DataType, _SizeType, _HashType, _ExtendType> __MyType;
		private:
			class Node : public GAIA::Base
			{
			public:
				_DataType t;
				_HashType h;
				Node* pLocalPrev;
				Node* pLocalNext;
				Node* pGlobalPrev;
				Node* pGlobalNext;
			};
			class Group : public GAIA::Base
			{
			public:
				Node* pLocalFront; // If GNIL means not nodes in current group.
				_SizeType nodecnt; // Specify the nodes count.
			};
		private:
			typedef GAIA::CTN::BasicPool<Node, _SizeType, _ExtendType> __NodePoolType;
			typedef GAIA::CTN::BasicVector<Group, _SizeType, _ExtendType> __GroupType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicHashSet;
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
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (){m_pNode = m_pNode->pGlobalNext; return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (){m_pNode = m_pNode->pGlobalPrev; return *this;}
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
				friend class BasicHashSet;
			public:
				GINL const_it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pNode == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual const _DataType& operator * () const{return m_pNode->t;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (){m_pNode = m_pNode->pGlobalNext; return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (){m_pNode = m_pNode->pGlobalPrev; return *this;}
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
				Node* m_pNode;
			};
		public:
			GINL BasicHashSet(){this->init();}
			GINL BasicHashSet(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_nodepool.size();}
			GINL const _SizeType& capacity() const{return m_nodepool.capacity();}
			GINL GAIA::GVOID clear()
			{
				m_nodepool.clear();
				m_groups.clear();
				m_pFront = GNIL;
				m_pBack = GNIL;
			}
			GINL GAIA::GVOID destroy()
			{
				m_nodepool.destroy();
				m_groups.destroy();
				m_pFront = GNIL;
				m_pBack = GNIL;
			}
			GINL GAIA::BL insert(const _DataType& t)
			{
				this->optimize(this->size() + 1);
				_HashType h = (_HashType)GAIA::ALGO::hash(t);
				_SizeType hm = h % m_groups.size();
				Group& g = m_groups[hm];
				Node* pNode = m_nodepool.alloc();
				pNode->t = t;
				pNode->h = h;
				pNode->pLocalPrev = GNIL;
				pNode->pLocalNext = g.pLocalFront;
				if(g.pLocalFront != GNIL)
					g.pLocalFront->pLocalPrev = pNode;
				g.pLocalFront = pNode;
				g.nodecnt++;
				pNode->pGlobalPrev = GNIL;
				pNode->pGlobalNext = m_pFront;
				if(m_pFront != GNIL)
					m_pFront->pGlobalPrev = pNode;
				m_pFront = pNode;
				if(m_pBack == GNIL)
					m_pBack = pNode;
				return GAIA::True;
			}
			GINL GAIA::BL erase(const _DataType& t)
			{
				_HashType h = (_HashType)GAIA::ALGO::hash(t);
				_SizeType hm = h % m_groups.size();
				Group& g = m_groups[hm];
				Node* pNode = g.pLocalFront;
				GAIA::BL bExist = GAIA::False;
				while(pNode != GNIL)
				{
					if(pNode->t == t)
					{
						bExist = GAIA::True;
						if(pNode->pLocalPrev != GNIL)
							pNode->pLocalPrev->pLocalNext = pNode->pLocalNext;
						else
							g.pLocalFront = pNode->pLocalNext;
						if(pNode->pLocalNext != GNIL)
							pNode->pLocalNext->pLocalPrev = pNode->pLocalPrev;
						if(pNode->pGlobalPrev != GNIL)
							pNode->pGlobalPrev->pGlobalNext = pNode->pGlobalNext;
						else
							m_pFront = pNode->pGlobalNext;
						if(pNode->pGlobalNext != GNIL)
							pNode->pGlobalNext->pGlobalPrev = pNode->pGlobalPrev;
						else
							m_pBack = pNode->pGlobalPrev;
						m_nodepool.release(pNode);
						bExist = GAIA::True;
						break;
					}
					pNode = pNode->pLocalNext;
				}
				if(!bExist)
					return GAIA::False;
				this->optimize(this->size());
				return GAIA::True;
			}
			GINL GAIA::BL erase(it& iter)
			{
				if(iter.empty())
					return GAIA::False;
				Node* pNode = iter.m_pNode;
				iter.m_pNode = pNode->pGlobalNext;
				_HashType h = (_HashType)GAIA::ALGO::hash(pNode->t);
				_SizeType hm = h % m_groups.size();
				Group& g = m_groups[hm];
				if(pNode->pLocalPrev != GNIL)
					pNode->pLocalPrev->pLocalNext = pNode->pLocalNext;
				else
					g.pLocalFront = pNode->pLocalNext;
				if(pNode->pLocalNext != GNIL)
					pNode->pLocalNext->pLocalPrev = pNode->pLocalPrev;
				if(pNode->pGlobalPrev != GNIL)
					pNode->pGlobalPrev->pGlobalNext = pNode->pGlobalNext;
				else
					m_pFront = pNode->pGlobalNext;
				if(pNode->pGlobalNext != GNIL)
					pNode->pGlobalNext->pGlobalPrev = pNode->pGlobalPrev;
				else
					m_pBack = pNode->pGlobalPrev;
				m_nodepool.release(pNode);
				return GAIA::True;
			}
			GINL _DataType* find(const _DataType& t)
			{
				Node* pNode = this->findnode(t);
				if(pNode == GNIL)
					return GNIL;
				return &pNode->t;
			}
			GINL const _DataType* find(const _DataType& t) const
			{
				return GCCAST(__MyType*)(this)->find(t);
			}
			GINL it findit(const _DataType& t)
			{
				it ret;
				Node* pNode = this->findnode(t);
				if(pNode == GNIL)
					return ret;
				ret.m_pContainer = this;
				ret.m_pNode = pNode;
				return ret;
			}
			GINL const_it const_findit(const _DataType& t) const
			{
				const_it ret;
				Node* pNode = this->findnode(t);
				if(pNode == GNIL)
					return ret;
				ret.m_pNode = pNode;
				return ret;
			}
			GINL _DataType& front(){return m_pFront->t;}
			GINL const _DataType& front() const{return m_pFront->t;}
			GINL _DataType& back(){return m_pBack->t;}
			GINL const _DataType& back() const{return m_pBack->t;}
			GINL it frontit(){it ret; ret.m_pContainer = this; ret.m_pNode = m_pFront; return ret;}
			GINL it backit(){it ret; ret.m_pContainer = this; ret.m_pNode = m_pBack; return ret;}
			GINL const_it const_frontit() const{const_it ret; ret.m_pNode = m_pFront; return ret;}
			GINL const_it const_backit() const{const_it ret; ret.m_pNode = m_pBack; return ret;}
			GINL __MyType& operator += (const __MyType& src)
			{
				const_it it = src.const_frontit();
				_SizeType size = src.size();
				for(; !it.empty(); ++it)
				{
					this->insert(*it);
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
					this->insert(*iter);
				return *this;
			}
		private:
			GINL GAIA::GVOID init(){m_pFront = GNIL; m_pBack = GNIL;}
			GINL Node* findnode(const _DataType& t) const
			{
				if(m_groups.empty())
					return GNIL;
				_HashType h = (_HashType)GAIA::ALGO::hash(t);
				_SizeType hm = h % m_groups.size();
				const Group& g = m_groups[hm];
				Node* pNode = g.pLocalFront;
				while(pNode != GNIL)
				{
					if(pNode->t == t)
						return pNode;
					pNode = pNode->pLocalNext;
				}
				return GNIL;
			}
			GINL GAIA::GVOID optimize(const _SizeType& targetsize)
			{
				if(targetsize == this->size())
					return;
				__GroupType oldgroups;
				if(targetsize >= m_groups.size() * 10) // Expend.
				{
					oldgroups = m_groups;
					_SizeType newsize = m_groups.size() * 10;
					if(newsize == 0)
						newsize = 1;
					m_groups.resize(newsize);
				}
				else if(targetsize <= m_groups.size() / 10) // Reduce.
				{
					oldgroups = m_groups;
					_SizeType newsize = m_groups.size() / 10;
					m_groups.resize(newsize);
				}
				else
					return;
				for(_SizeType x = 0; x < m_groups.size(); ++x)
				{
					Group& g = m_groups[x];
					g.pLocalFront = GNIL;
					g.nodecnt = 0;
				}
				for(_SizeType x = 0; x < oldgroups.size(); ++x)
				{
					Group& gsrc = oldgroups[x];
					Node* pNode = gsrc.pLocalFront;
					while(pNode != GNIL)
					{
						Node* pNextNode = pNode->pLocalNext;
						GAST(m_groups.size() > 0);
						_SizeType hm = pNode->h % m_groups.size();
						Group& gdst = m_groups[hm];
						pNode->pLocalNext = gdst.pLocalFront;
						if(gdst.pLocalFront != GNIL)
							gdst.pLocalFront->pLocalPrev = pNode;
						gdst.pLocalFront = pNode;
						gdst.nodecnt++;
						pNode = pNextNode;
					}
				}
			}
		private:
			__NodePoolType m_nodepool;
			__GroupType m_groups;
			Node* m_pFront;
			Node* m_pBack;
		};
		template<typename _DataType> class HashSet : public BasicHashSet<_DataType, GAIA::NUM, GAIA::U64, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
