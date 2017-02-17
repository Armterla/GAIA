#ifndef 	__GAIA_CTN_STORAGE_H__
#define 	__GAIA_CTN_STORAGE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_avltree.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _SizeType, typename _ExtendType, _SizeType _PageSize> class BasicStorage : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
		public:
			static const _SizeType _pagesize = _PageSize;
		public:
			class Node : public GAIA::Base
			{
			private:
				friend class BasicStorage;
				friend class NodeSize;
			public:
				GINL Node(){}
				GINL Node(const Node& src){this->operator = (src);}
				GINL const _SizeType& head() const{return m_head;}
				GINL const _SizeType& size() const{return m_size;}
				GINL const _SizeType& capacity() const{return m_capacity;}
				GINL Node& operator = (const Node& src){GAST(&src != this); m_head = src.m_head; m_size = src.m_size; m_capacity = src.m_capacity; return *this;}
				GCLASS_COMPARE(m_head, Node)
			private:
				_SizeType m_head;
				_SizeType m_size;
				_SizeType m_capacity;
			};
			class NodeSize : public GAIA::Base
			{
			private:
				friend class BasicStorage;
			public:
				GINL NodeSize(){this->init();}
				GINL NodeSize(const NodeSize& src){this->operator = (src);}
				GINL NodeSize(const Node& src){this->operator = (src);}
				GINL const Node& node() const{return m_n;}
				GINL NodeSize& operator = (const NodeSize& src){GAST(&src != this); m_n = src.m_n; return *this;}
				GINL NodeSize& operator = (const Node& src){m_n = src; return *this;}
				GCLASS_COMPARE2(m_n.m_capacity, m_n.m_head, NodeSize)
			private:
				GINL GAIA::GVOID init(){}
			private:
				Node m_n;
			};
		public:
			typedef BasicStorage<_SizeType, _ExtendType, _PageSize> __MyType;
			typedef BasicAVLTree<Node, _SizeType, _SizeType, _ExtendType> __AddrAVLTreeType;
			typedef BasicAVLTree<NodeSize, _SizeType, _SizeType, _ExtendType> __SizeAVLTreeType;
		public:
			GINL BasicStorage(){this->init();}
			GINL BasicStorage(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return m_size == 0;}
			GINL const _SizeType& size() const{return m_size;}
			GINL const _SizeType& capacity() const{return m_capacity;}
			GINL GAIA::BL increase_reserve(const _SizeType& head, const _SizeType& size)
			{
				GAST(size > 0);
				if(size <= 0)
					return GAIA::False;
				GAST(size % _PageSize == 0);
				if(size % _PageSize != 0)
					return GAIA::False;
				Node n;
				n.m_head = head;
				typename __AddrAVLTreeType::it it = m_origin_a.upper_equal(n);
				if(!it.empty())
				{
					if(head + size > (*it).head())
						return GAIA::False;
				}
				it = m_origin_a.lower_equal(n);
				if(!it.empty())
				{
					if((*it).m_head + (*it).m_capacity > head)
						return GAIA::False;
				}
				n.m_size = 0;
				n.m_capacity = size;
				NodeSize ns;
				ns.m_n = n;
				m_free_s.insert(ns);
				m_free_a.insert(n);
				m_origin_a.insert(n);
				m_capacity += size;
				return GAIA::True;
			}
			GINL GAIA::GVOID clear()
			{
				m_size = 0;
				m_using_a.clear();
				m_free_a.clear();
				m_free_s.clear();
				typename __AddrAVLTreeType::it it = m_origin_a.frontit();
				for(; !it.empty(); ++it)
				{
					NodeSize ns;
					ns.m_n = *it;
					m_free_a.insert(ns.m_n);
					m_free_s.insert(ns);
				}
			}
			GINL GAIA::GVOID destroy()
			{
				m_origin_a.destroy();
				m_using_a.destroy();
				m_free_a.destroy();
				m_free_s.destroy();
				this->init();
			}
			GINL GAIA::BL insert(const _SizeType& size, Node& result)
			{
				GAST(size > 0);
				if(size <= 0)
					return GAIA::False;
				NodeSize ns;
				ns.m_n.m_size = size;
				ns.m_n.m_capacity = this->alignpage(size);
				typename __SizeAVLTreeType::it it = m_free_s.upper_equal(ns);
				if(it.empty())
					return GAIA::False;
				NodeSize nsr = *it;
				ns.m_n.m_head = nsr.m_n.m_head;
				if(!m_using_a.insert(ns.m_n))
					return GAIA::False;
				m_free_s.erase(nsr);
				if(nsr.m_n.m_capacity == ns.m_n.m_capacity)
				{
					m_free_a.erase(ns.m_n);
					result = ns.m_n;
					m_size += ns.m_n.m_capacity;
					GAST(m_size <= m_capacity);
					return GAIA::True;
				}
				else
				{
					NodeSize freens = nsr;
					freens.m_n.m_head += ns.m_n.m_capacity;
					freens.m_n.m_capacity -= ns.m_n.m_capacity;
					freens.m_n.m_size = 0;
					m_free_s.insert(freens);
					Node* p = m_free_a.find(nsr.m_n);
					GAST(!!p);
					p->m_head = freens.m_n.m_head;
					p->m_capacity = freens.m_n.m_capacity;
					result = ns.m_n;
					m_size += ns.m_n.m_capacity;
					GAST(m_size <= m_capacity);
					return GAIA::True;
				}
			}
			GINL GAIA::BL erase(const _SizeType& head)
			{
				NodeSize ns;
				ns.m_n.m_head = head;
				Node* pNode = m_using_a.find(ns.m_n);
				if(pNode == GNIL)
					return GAIA::False;
				ns.m_n.m_size = 0;
				ns.m_n.m_capacity = pNode->m_capacity;
				if(!m_using_a.erase(ns.m_n))
					return GAIA::False;
				typename __AddrAVLTreeType::it ita_prev = m_free_a.lower_equal(ns.m_n);
				typename __AddrAVLTreeType::it ita_next;
				if(ita_prev.empty())
					ita_next = m_free_a.upper_equal(ns.m_n);
				else
				{
					ita_next = ita_prev;
					++ita_next;
				}
				GAIA::BL bPrev = GAIA::True;
				GAIA::BL bNext = GAIA::True;
				if(ita_prev.empty())
					bPrev = GAIA::False;
				else if((*ita_prev).m_head + (*ita_prev).m_capacity != ns.m_n.m_head)
					bPrev = GAIA::False;
				if(ita_next.empty())
					bNext = GAIA::False;
				else if((*ita_next).m_head != ns.m_n.m_head + ns.m_n.m_capacity)
					bNext = GAIA::False;
				if(bPrev && bNext)
				{
					NodeSize prev = *ita_prev;
					NodeSize next = *ita_next;
					m_free_s.erase(prev);
					m_free_s.erase(next);
					m_free_a.erase(next.m_n);
					NodeSize newns;
					newns.m_n.m_head = prev.m_n.m_head;
					newns.m_n.m_size = 0;
					newns.m_n.m_capacity = prev.m_n.m_capacity + ns.m_n.m_capacity + next.m_n.m_capacity;
					m_free_s.insert(newns);
					(*ita_prev).m_capacity = newns.m_n.m_capacity;
				}
				else if(!bPrev && bNext)
				{
					NodeSize next = *ita_next;
					m_free_s.erase(next);
					NodeSize newns;
					newns.m_n.m_head = ns.m_n.m_head;
					newns.m_n.m_size = 0;
					newns.m_n.m_capacity = ns.m_n.m_capacity + next.m_n.m_capacity;
					m_free_s.insert(newns);
					(*ita_next).m_head = newns.m_n.m_head;
					(*ita_next).m_capacity = newns.m_n.m_capacity;
				}
				else if(bPrev && !bNext)
				{
					NodeSize prev = *ita_prev;
					m_free_s.erase(prev);
					NodeSize newns;
					newns.m_n.m_head = prev.m_n.m_head;
					newns.m_n.m_size = 0;
					newns.m_n.m_capacity = prev.m_n.m_capacity + ns.m_n.m_capacity;
					m_free_s.insert(newns);
					(*ita_prev).m_capacity = newns.m_n.m_capacity;
				}
				else
				{
					m_free_s.insert(ns);
					m_free_a.insert(ns.m_n);
				}
				m_size -= ns.m_n.m_capacity;
				GAST(m_size >= 0);
				return GAIA::True;
			}
			GINL const Node* find(const _SizeType& head) const
			{
				Node n;
				n.m_head = head;
				return m_using_a.find(n);
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				m_origin_a = src.m_origin_a;
				m_using_a = src.m_using_a;
				m_free_a = src.m_free_a;
				m_free_s = src.m_free_s;
				m_size = src.m_size;
				m_capacity = src.m_capacity;
				return *this;
			}
		private:
			GINL GAIA::GVOID init(){m_size = 0; m_capacity = 0;}
			GINL _SizeType alignpage(const _SizeType& size) const
			{
				_SizeType k = size % _PageSize;
				if(k == 0)
					return size;
				return size / _PageSize * _PageSize + _PageSize;
			}
		private:
			__AddrAVLTreeType m_origin_a;
			__AddrAVLTreeType m_using_a;
			__AddrAVLTreeType m_free_a;
			__SizeAVLTreeType m_free_s;
			_SizeType m_size;
			_SizeType m_capacity;
		};
		template<GAIA::NUM _PageSize> class Storage : public BasicStorage<GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM>, _PageSize>{public:};
	}
}

#endif
