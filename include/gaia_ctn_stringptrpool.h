#ifndef		__GAIA_CTN_STRINGPTRPOOL_H__
#define		__GAIA_CTN_STRINGPTRPOOL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_stack.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_set.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _RefCounterType, typename _ExtendType> class BasicStringPtrPool : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _RefCounterType _refcountertype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicStringPtrPool<_DataType, _SizeType, _RefCounterType, _ExtendType> __MyType;
		public:
			GINL BasicStringPtrPool(){}
			GINL BasicStringPtrPool(const __MyType& src){this->operator = (src);}
			GINL ~BasicStringPtrPool(){this->destroy();}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_nodelist.size();}
			GINL _SizeType practice_size() const{return m_nodelist.size() - m_freestack.size();}
			GINL _SizeType capacity(){return m_nodelist.capacity();}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				this->destroy();
				m_nodelist.reserve(size);
				m_freestack.reserve(size);
			}
			GINL GAIA::GVOID clear()
			{
				for(typename __NodesType::it it = m_nodes.frontit(); !it.empty(); ++it)
				{
					Node& n = *it;
					NodeByLen n1;
					n1.data = n.data;
					n1.len = n.len;
					n1.idx = GINVALID;
					n1.r = 0;
					m_frees.insert(n1);
				}
				m_nodes.clear();
				m_nodelist.clear();
				m_freestack.clear();
			}
			GINL GAIA::GVOID destroy()
			{
				for(typename __NodesType::it it = m_nodes.frontit(); !it.empty(); )
				{
					_DataType* p = (*it).data;
					it.erase();
					gdel[] p;
				}
				for(typename __FreesType::it it = m_frees.frontit(); !it.empty(); )
				{
					_DataType* p = (*it).data;
					it.erase();
					gdel[] p;
				}
				m_nodelist.destroy();
				m_freestack.destroy();
				m_nodes.destroy();
				m_frees.destroy();
			}
			GINL const _DataType* alloc(const _DataType* p)
			{
				GAST(p != GNIL);
				NodeByLen n;
				n.data = GCCAST(_DataType*)(p);
				Node* pFinded = m_nodes.find(n);
				if(pFinded == GNIL)
				{
					n.r = 0;
					n.len = GAIA::ALGO::gstrlen(p);
					typename __FreesType::it itfinded = m_frees.upper_equal(n);
					if(!itfinded.empty())
					{
						NodeByLen k = *itfinded;
						if(k.len >= n.len && k.len < n.len * 2)
						{
							n.data = k.data;
							n.len = k.len;
							GAIA::ALGO::gstrcpy(n.data, p);
							m_frees.erase(k);
						}
					}
					if(n.data == p)
						n.data = GAIA::ALGO::gstrnew(p);
					if(!m_freestack.empty())
					{
						n.idx = m_freestack.back();
						m_freestack.pop_back();
						m_nodelist[n.idx] = n;
					}
					else
					{
						n.idx = m_nodelist.size();
						m_nodelist.push_back(n);
					}
					m_nodes.insert(n);
					pFinded = m_nodes.find(n);
				}
				++pFinded->r;
				return pFinded->data;
			}
			GINL const _DataType* request(const _DataType* p)
			{
				GAST(p != GNIL);
				NodeByLen n;
				n.data = GCCAST(_DataType*)(p);
				Node* pFinded = m_nodes.find(n);
				if(pFinded == GNIL)
					return GNIL;
				++pFinded->r;
				return pFinded->data;
			}
			GINL GAIA::BL release(const _DataType* p)
			{
				GAST(p != GNIL);
				NodeByLen n;
				n.data = GCCAST(_DataType*)(p);
				Node* pFinded = m_nodes.find(n);
				if(pFinded == GNIL)
					return GAIA::False;
				--pFinded->r;
				if(pFinded->r == 0)
				{
					n.data = pFinded->data;
					n.len = pFinded->len;
					n.r = 0;
					n.idx = GINVALID;
					m_nodelist[pFinded->idx].data = GNIL;
					m_freestack.push_back(pFinded->idx);
					m_nodes.erase(n);
					m_frees.insert(n);
				}
				return GAIA::True;
			}
			GINL GAIA::GVOID recycle_bysize(_SizeType recyclesize)
			{
				typename __FreesType::it it = m_frees.frontit();
				while(recyclesize > 0)
				{
					if(it.empty())
						break;
					NodeByLen& n = *it;
					recyclesize -= n.len;
					_DataType* p = n.data;
					it.erase();
					gdel[] p;
				}
			}
			GINL GAIA::GVOID recycle_bycount(_SizeType recyclecount)
			{
				typename __FreesType::it it = m_frees.backit();
				while(recyclecount > 0)
				{
					if(it.empty())
						break;
					NodeByLen& n = *it;
					recyclecount--;
					_DataType* p = n.data;
					it.erase();
					gdel[] p;
				}
			}
			GINL GAIA::GVOID recycle()
			{
				for(typename __FreesType::it it = m_frees.frontit(); !it.empty(); )
				{
					_DataType* p = (*it).data;
					it.erase();
					gdel[] p;
				}
				m_frees.clear();
			}
			GINL const _DataType* find(const _DataType* p) const
			{
				GAST(p != GNIL);
				Node n;
				n.data = GCCAST(_DataType*)(p);
				const Node* pFinded = m_nodes.find(n);
				if(pFinded == GNIL)
					return GNIL;
				return pFinded->data;
			}
			GINL _SizeType string_size() const{return m_nodelist.size();}
			GINL const _DataType* get(const _SizeType& index) const{return m_nodelist[index].data;}
			GINL __MyType& operator = (const __MyType& src)
			{
				this->clear();
				for(_SizeType x = 0; x < src.string_size(); ++x)
					this->alloc(src.get(x));
				return *this;
			}
		private:
			class Node : public GAIA::Base
			{
			public:
				GINL GAIA::NUM compare(const Node& src) const{return GAIA::ALGO::gstrcmp(data, src.data);}
				GCLASS_COMPARE_BYCOMPARE(Node)
			public:
				_DataType* data;
				_SizeType len;
				_SizeType idx;
				_RefCounterType r;
			};
			class NodeByLen : public Node
			{
			public:
				GINL GAIA::NUM compare(const NodeByLen& src) const
				{
					if(this->len > src.len)
						return -1;
					else if(this->len < src.len)
						return +1;
					else
					{
						if(this->data < src.data)
							return -1;
						else if(this->data > src.data)
							return +1;
						return 0;
					}
				}
				GCLASS_COMPARE_BYCOMPARE(NodeByLen)
			};
		private:
			typedef GAIA::CTN::BasicVector<Node, _SizeType, _ExtendType> __NodeListType;
			typedef GAIA::CTN::BasicStack<_SizeType, _SizeType, _ExtendType> __FreeIndexStackType;
			typedef BasicSet<Node, _SizeType, _SizeType, _ExtendType> __NodesType;
			typedef BasicSet<NodeByLen, _SizeType, _SizeType, _ExtendType> __FreesType;
		private:
			__NodeListType m_nodelist;
			__FreeIndexStackType m_freestack;
			__NodesType m_nodes;
			__FreesType m_frees;
		};
		template<typename _DataType> class StringPtrPool : public GAIA::CTN::BasicStringPtrPool<_DataType, GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
