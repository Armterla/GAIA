#ifndef		__GAIA_CTN_STATICSTRINGPTRPOOL_H__
#define		__GAIA_CTN_STATICSTRINGPTRPOOL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_set.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicStaticStringPtrPool : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicStaticStringPtrPool<_DataType, _SizeType, _ExtendType> __MyType;
		public:
			GINL BasicStaticStringPtrPool(){}
			GINL BasicStaticStringPtrPool(const __MyType& src){this->operator = (src);}
			GINL ~BasicStaticStringPtrPool(){this->destroy();}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_nodelist.size();}
			GINL _SizeType capacity(){return m_nodelist.capacity();}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				this->destroy();
				m_nodelist.reserve(size);
			}
			GINL GAIA::GVOID clear()
			{
				for(typename __NodesType::it it = m_nodes.frontit(); !it.empty(); )
				{
					_DataType* p = (*it).data;
					it.erase();
					gdel[] p;
				}
				m_nodelist.clear();
				m_nodes.clear();
			}
			GINL GAIA::GVOID destroy()
			{
				for(typename __NodesType::it it = m_nodes.frontit(); !it.empty(); )
				{
					_DataType* p = (*it).data;
					it.erase();
					gdel[] p;
				}
				m_nodelist.destroy();
				m_nodes.destroy();
			}
			GINL const _DataType* alloc(const _DataType* p)
			{
				GAST(p != GNIL);
				Node n;
				n.data = GCCAST(_DataType*)(p);
				Node* pFinded = m_nodes.find(n);
				if(pFinded == GNIL)
				{
					n.data = GAIA::ALGO::gstrnew(p);
					n.idx = m_nodelist.size();
					m_nodelist.push_back(n);
					m_nodes.insert(n);
					return n.data;
				}
				return pFinded->data;
			}
			GINL const _DataType* request(const _DataType* p)
			{
				GAST(p != GNIL);
				Node n;
				n.data = GCCAST(_DataType*)(p);
				Node* pFinded = m_nodes.find(n);
				if(pFinded == GNIL)
					return GNIL;
				return pFinded->data;
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
				_SizeType idx;
			};
		private:
			typedef GAIA::CTN::BasicVector<Node, _SizeType, _ExtendType> __NodeListType;
			typedef GAIA::CTN::BasicSet<Node, _SizeType, _SizeType, _ExtendType> __NodesType;
		private:
			__NodeListType m_nodelist;
			__NodesType m_nodes;
		};
		template<typename _DataType> class StaticStringPtrPool : public GAIA::CTN::BasicStaticStringPtrPool<_DataType, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
