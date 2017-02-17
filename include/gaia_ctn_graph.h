#ifndef		__GAIA_CTN_GRAPH_H__
#define		__GAIA_CTN_GRAPH_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_stack.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_tree.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicGraph : public GAIA::Base
		{
		public:
			class Node : public GAIA::Base
			{
			private:
				friend class BasicGraph;
			public:
				GINL GAIA::GVOID enter_traveling() const
				{
					GAST(!m_traveling);
					(const_cast<Node*>(this))->m_traveling = GAIA::True;
				}
				GINL GAIA::GVOID leave_traveling() const
				{
					GAST(m_traveling);
					(const_cast<Node*>(this))->m_traveling = GAIA::False;
				}
				GINL _DataType& operator * (){return m_t;}
				GINL const _DataType& operator * () const{return m_t;}
			private:
				_DataType m_t;
				BasicVector<Node*, _SizeType, _ExtendType> m_links;
				GAIA::N8 m_traveling : 1;
			};
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicGraph<_DataType, _SizeType, _ExtendType> __MyType;
			typedef BasicVector<Node*, _SizeType, _ExtendType> __NodeListType;
			typedef BasicTree<Node*, _SizeType, _ExtendType> __PathTreeType;
			typedef BasicPool<Node, _SizeType, _ExtendType> __PoolType;
			typedef BasicVector<Pair<Node*, Node*>, _SizeType, GAIA::ALGO::ExtendGold<_SizeType> > __LinkListType;
			typedef BasicStack<Node*, _SizeType, _ExtendType> __TravelingStackType;
		public:
			GINL BasicGraph(){this->init();}
			GINL BasicGraph(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return m_pRoot == GNIL;}
			GINL _SizeType size() const{return m_pool.size();}
			GINL const _SizeType& capacity() const{return m_pool.capacity();}
			GINL GAIA::GVOID destroy(){m_pRoot = GNIL; m_pool.destroy(); m_tstack.destroy();}
			GINL GAIA::GVOID clear(){m_pRoot = GNIL; m_pool.clear();}
			GINL Node* root(){return m_pRoot;}
			GINL const Node* root() const{return m_pRoot;}
			GINL GAIA::GVOID root(Node* pNewRoot){m_pRoot = pNewRoot;}
			GINL Node* insert(const _DataType& t, Node* pNode)
			{
				Node* pNew = m_pool.alloc();
				pNew->m_t = t;
				pNew->m_links.clear();
				pNew->m_traveling = GAIA::False;
				if(pNode == GNIL)
				{
					if(m_pRoot == GNIL)
						m_pRoot = pNew;
					else
						this->link(*m_pRoot, *pNew);
				}
				else
				{
					GAST(!!m_pRoot);
					this->link(*pNode, *pNew);
				}
				return pNew;
			}
			GINL GAIA::GVOID erase(Node& n)
			{
				for(_SizeType x = 0; x < n.m_links.size(); ++x)
				{
					Node* pNode = n.m_links[x];
					if(pNode != GNIL)
					{
						for(_SizeType y = 0; y < pNode->m_links.size(); ++y)
						{
							if(pNode->m_links[y] == &n)
							{
								pNode->m_links[y] = GNIL;
								break;
							}
						}
					}
				}
				n.m_links.clear();
				m_pool.release(&n);
				if(&n == m_pRoot)
					m_pRoot = GNIL;
			}
			GINL GAIA::BL link(Node& n1, Node& n2)
			{
				if(this->islinked(n1, n2))
					return GAIA::False;
				Node** ppn1 = GNIL;
				Node** ppn2 = GNIL;
				for(_SizeType x = 0; x < n1.m_links.size(); ++x)
				{
					if(n1.m_links[x] == GNIL)
					{
						ppn1 = &n1.m_links[x];
						break;
					}
				}
				for(_SizeType x = 0; x < n2.m_links.size(); ++x)
				{
					if(n2.m_links[x] == GNIL)
					{
						ppn2 = &n2.m_links[x];
						break;
					}
				}
				if(ppn1 == GNIL)
					n1.m_links.push_back(&n2);
				else
					*ppn1 = &n2;
				if(ppn2 == GNIL)
					n2.m_links.push_back(&n1);
				else
					*ppn2 = &n1;
				return GAIA::True;
			}
			GINL GAIA::BL unlink(Node& n1, Node& n2)
			{
				GAIA::BL bRet = GAIA::False;
				for(_SizeType x = 0; x < n1.m_links.size(); ++x)
				{
					if(n1.m_links[x] == &n2)
					{
						n1.m_links[x] = GNIL;
						bRet = GAIA::True;
						break;
					}
				}
				if(bRet)
				{
					for(_SizeType x = 0; x < n2.m_links.size(); ++x)
					{
						if(n2.m_links[x] == &n1)
						{
							n2.m_links[x] = GNIL;
							break;
						}
					}
				}
				return bRet;
			}
			GINL GAIA::BL islinked(const Node& n1, const Node& n2) const
			{
				for(_SizeType x = 0; x < n1.m_links.size(); ++x)
				{
					if(n1.m_links[x] == &n2)
						return GAIA::True;
				}
				return GAIA::False;
			}
			GINL _SizeType getlinksize(const Node& n) const{return n.m_links.size();}
			GINL Node* getlink(const Node& n, const _SizeType& index) const{return n.m_links[index];}
			GINL GAIA::BL exist(const _DataType& t) const;
			GINL _SizeType count(const _DataType& t) const;
			GINL GAIA::GVOID find(const Node* pSrc, const _DataType& t, __NodeListType& result) const
			{
				if(pSrc == GNIL)
					pSrc = m_pRoot;
				if(pSrc == GNIL)
					return;
				this->find_node(pSrc, t, result);
				if(!result.empty())
				{
					result.sort();
					result.unique();
				}
			}
			GINL GAIA::GVOID paths(const Node& src, const Node& dst, __PathTreeType& result) const
			{
				result.clear();
				this->paths_node(src, dst, result, GNIL);
			}
			GINL GAIA::GVOID paths(const Node& src, const _DataType& t, __PathTreeType& result) const
			{
				result.clear();
				this->paths_node(src, t, result, GNIL);
			}
			GINL GAIA::GVOID anypath(const Node& src, const Node& dst, __NodeListType& result) const
			{
				result.clear();
				if(this->anypath_node(src, dst, result))
					GAIA::ALGO::inverse(result.fptr(), result.bptr());
				this->reset_tstack();
			}
			GINL GAIA::GVOID anypath(const Node& src, const _DataType& t, __NodeListType& result) const
			{
				result.clear();
				if(this->anypath_node(src, t, result))
					GAIA::ALGO::inverse(result.fptr(), result.bptr());
				this->reset_tstack();
			}
			template<typename _KeyType, _SizeType _MaxLinkCount> GAIA::GVOID navpath(const Node& src, const Node& dst, const _SizeType& navtimes, __NodeListType& result) const
			{
				GAST(navtimes >= 0);
				result.clear();
				if(this->navpath_node<_KeyType, _MaxLinkCount>(src, dst, navtimes, result))
					GAIA::ALGO::inverse(result.fptr(), result.bptr());
				this->reset_tstack();
			}
			template<typename _KeyType, _SizeType _MaxLinkCount> GAIA::GVOID navpath(const Node& src, const _DataType& t, const _SizeType& navtimes, __NodeListType& result) const
			{
				GAST(navtimes >= 0);
				result.clear();
				if(this->navpath_node<_KeyType, _MaxLinkCount>(src, t, navtimes, result))
					GAIA::ALGO::inverse(result.fptr(), result.bptr());
				this->reset_tstack();
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				return *this;
			}
		private:
			GINL GAIA::GVOID init(){m_pRoot = GNIL;}
			GINL GAIA::GVOID reset_tstack() const
			{
				while(!m_tstack.empty())
				{
					Node* pNode = m_tstack.back();
					pNode->leave_traveling();
					const_cast<__MyType*>(this)->m_tstack.pop_back();
				}
			}
			GINL GAIA::GVOID reset_tstack(const _SizeType& size) const
			{
				GAST(size >= 0);
				for(_SizeType x = 0; x < size; ++x)
				{
					Node* pNode = m_tstack.back();
					pNode->leave_traveling();
					const_cast<__MyType*>(this)->m_tstack.pop_back();
				}
			}
			GINL GAIA::GVOID find_node(const Node* pSrc, const _DataType& t, __NodeListType& result) const
			{
				if(pSrc->m_traveling)
					return;
				if(pSrc->m_t == t)
				{
					result.push_back(const_cast<Node*>(pSrc));
					return;
				}
				pSrc->enter_traveling();
				{
					for(_SizeType x = 0; x < pSrc->m_links.size(); ++x)
					{
						const Node* pNode = pSrc->m_links[x];
						if(pNode == GNIL)
							continue;
						this->find_node(pNode, t, result);
					}
				}
				pSrc->leave_traveling();
			}
			GINL GAIA::GVOID paths_node(const Node& src, const Node& dst, __PathTreeType& result, typename __PathTreeType::Node* pParentTreeNode) const
			{
				if(src.m_traveling)
					return;
				if(&dst == &src)
				{
					result.insert(const_cast<Node*>(&src), pParentTreeNode);
					return;
				}
				src.enter_traveling();
				{
					typename __PathTreeType::Node* pNewTreeNode = result.insert(const_cast<Node*>(&src), pParentTreeNode);
					for(_SizeType x = 0; x < src.m_links.size(); ++x)
					{
						const Node* pNode = src.m_links[x];
						if(pNode == GNIL)
							continue;
						this->paths_node(*pNode, dst, result, pNewTreeNode);
					}
					if(result.leaf(*pNewTreeNode))
						result.erase(*pNewTreeNode);
				}
				src.leave_traveling();
			}
			GINL GAIA::GVOID paths_node(const Node& src, const _DataType& t, __PathTreeType& result, typename __PathTreeType::Node* pParentTreeNode) const
			{
				if(src.m_traveling)
					return;
				if(src.m_t == t)
				{
					result.insert(const_cast<Node*>(&src), pParentTreeNode);
					return;
				}
				src.enter_traveling();
				{
					typename __PathTreeType::Node* pNewTreeNode = result.insert(const_cast<Node*>(&src), pParentTreeNode);
					for(_SizeType x = 0; x < src.m_links.size(); ++x)
					{
						const Node* pNode = src.m_links[x];
						if(pNode == GNIL)
							continue;
						this->paths_node(*pNode, t, result, pNewTreeNode);
					}
					if(result.leaf(*pNewTreeNode))
						result.erase(*pNewTreeNode);
				}
				src.leave_traveling();
			}
			GINL GAIA::BL anypath_node(const Node& src, const Node& dst, __NodeListType& result) const
			{
				if(src.m_traveling)
					return GAIA::False;
				if(&dst == &src)
				{
					result.push_back(const_cast<Node*>(&src));
					return GAIA::True;
				}
				src.enter_traveling();
				{
					for(_SizeType x = 0; x < src.m_links.size(); ++x)
					{
						const Node* pNode = src.m_links[x];
						if(pNode == GNIL)
							continue;
						if(this->anypath_node(*pNode, dst, result))
						{
							result.push_back(const_cast<Node*>(&src));
							src.leave_traveling();
							return GAIA::True;
						}
					}
				}
				if(src.m_links.size() - src.m_links.count(GNIL) <= 2)
					const_cast<__MyType*>(this)->m_tstack.push_back(const_cast<Node*>(&src));
				else
					src.leave_traveling();
				return GAIA::False;
			}
			GINL GAIA::BL anypath_node(const Node& src, const _DataType& t, __NodeListType& result) const
			{
				if(src.m_traveling)
					return GAIA::False;
				if(src.m_t == t)
				{
					result.push_back(const_cast<Node*>(&src));
					return GAIA::True;
				}
				src.enter_traveling();
				{
					for(_SizeType x = 0; x < src.m_links.size(); ++x)
					{
						const Node* pNode = src.m_links[x];
						if(pNode == GNIL)
							continue;
						if(this->anypath_node(*pNode, t, result))
						{
							result.push_back(const_cast<Node*>(&src));
							src.leave_traveling();
							return GAIA::True;
						}
					}
				}
				if(src.m_links.size() - src.m_links.count(GNIL) <= 2)
					const_cast<__MyType*>(this)->m_tstack.push_back(const_cast<Node*>(&src));
				else
					src.leave_traveling();
				return GAIA::False;
			}
			GINL const Node& navfind(const Node& src, const Node& dst, const _SizeType& navtimes, _SizeType& traveling_count) const
			{
				if(traveling_count == 0)
					return dst;
				if(dst.m_links.size() - dst.m_links.count(GNIL) != 2)
					return dst;
				for(_SizeType x = 0; x < dst.m_links.size(); ++x)
				{
					if(dst.m_links[x] == GNIL)
						continue;
					if(dst.m_links[x] == &src)
						continue;
					if(dst.m_links[x]->m_traveling)
						continue;
					dst.m_links[x]->enter_traveling();
					const_cast<__MyType*>(this)->m_tstack.push_back(const_cast<Node*>(&src));
					++traveling_count;
					_SizeType newnavtimes = navtimes - 1;
					return this->navfind(dst, *dst.m_links[x], newnavtimes, traveling_count);
				}
				return dst;
			}
			template<typename _KeyType, _SizeType _MaxLinkCount> GAIA::BL navpath_node(const Node& src, const Node& dst, const _SizeType& navtimes, __NodeListType& result) const
			{
				if(src.m_traveling)
					return GAIA::False;
				if(&dst == &src)
				{
					result.push_back(const_cast<Node*>(&src));
					return GAIA::True;
				}
				src.enter_traveling();
				{
					BasicArray<Pair<_KeyType, const Node*>, _SizeType, _MaxLinkCount> arrlink;
					for(_SizeType x = 0; x < src.m_links.size(); ++x)
					{
						const Node* pNode = src.m_links[x];
						if(pNode == GNIL)
							continue;
						_SizeType traveling_count = 0;
						const Node& navdst = this->navfind(src, *pNode, navtimes, traveling_count);
						this->reset_tstack(traveling_count);
						GAST(arrlink.size() < arrlink.capacity());
						arrlink.push_back(Pair<_KeyType, const Node*>(dst.m_t - navdst.m_t, pNode));
						if(arrlink.size() == arrlink.capacity())
							break;
					}
					arrlink.sort();
					for(_SizeType x = 0; x < arrlink.size(); ++x)
					{
						const Node* pNode = arrlink[x].back();
						if(pNode == GNIL)
							continue;
						if(this->navpath_node<_KeyType, _MaxLinkCount>(*pNode, dst, navtimes, result))
						{
							result.push_back(const_cast<Node*>(&src));
							src.leave_traveling();
							return GAIA::True;
						}
					}
				}
				if(src.m_links.size() - src.m_links.count(GNIL) <= 2)
					const_cast<__MyType*>(this)->m_tstack.push_back(const_cast<Node*>(&src));
				else
					src.leave_traveling();
				return GAIA::False;
			}
			template<typename _KeyType, _SizeType _MaxLinkCount> GAIA::BL navpath_node(const Node& src, const _DataType& t, const _SizeType& navtimes, __NodeListType& result) const
			{
				if(src.m_traveling)
					return GAIA::False;
				if(src.m_t == t)
				{
					result.push_back(const_cast<Node*>(&src));
					return GAIA::True;
				}
				src.enter_traveling();
				{
					BasicArray<Pair<_KeyType, const Node*>, _SizeType, _MaxLinkCount> arrlink;
					for(_SizeType x = 0; x < src.m_links.size(); ++x)
					{
						const Node* pNode = src.m_links[x];
						if(pNode == GNIL)
							continue;
						_SizeType traveling_count = 0;
						const Node& navdst = this->navfind(src, *pNode, navtimes, traveling_count);
						this->reset_tstack(traveling_count);
						GAST(arrlink.size() < arrlink.capacity());
						arrlink.push_back(Pair<_KeyType, const Node*>(t - navdst.m_t, pNode));
						if(arrlink.size() == arrlink.capacity())
							break;
					}
					arrlink.sort();
					for(_SizeType x = 0; x < arrlink.size(); ++x)
					{
						const Node* pNode = arrlink[x].back();
						if(pNode == GNIL)
							continue;
						if(this->navpath_node<_KeyType, _MaxLinkCount>(*pNode, t, navtimes, result))
						{
							result.push_back(const_cast<Node*>(&src));
							src.leave_traveling();
							return GAIA::True;
						}
					}
				}
				if(src.m_links.size() - src.m_links.count(GNIL) <= 2)
					const_cast<__MyType*>(this)->m_tstack.push_back(const_cast<Node*>(&src));
				else
					src.leave_traveling();
				return GAIA::False;
			}
		private:
			Node* m_pRoot;
			__PoolType m_pool;
			__TravelingStackType m_tstack;
		};
	}
}

#endif
