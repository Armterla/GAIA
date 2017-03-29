#ifndef 	__GAIA_CTN_STRINGPOOL_H__
#define 	__GAIA_CTN_STRINGPOOL_H__

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
		template<typename _DataType, typename _SizeType, typename _RefCounterType, typename _ExtendType> class BasicStringPool : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _RefCounterType _refcountertype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicStringPool<_DataType, _SizeType, _RefCounterType, _ExtendType> __MyType;
		public:
			GINL BasicStringPool(){}
			GINL BasicStringPool(const __MyType& src){this->operator = (src);}
			GINL ~BasicStringPool(){this->destroy();}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_nodelist.size();}
			GINL _SizeType practice_size() const{return m_nodelist.size() - m_freestack.size();}
			GINL _SizeType capacity() const{return m_nodelist.capacity();}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				GAST(size >= 0);
				this->destroy();
				m_nodelist.reserve(size);
				m_freestack.reserve(size);
			}
			GINL GAIA::GVOID clear()
			{
				this->destroy_content_all();
				m_nodelist.clear();
				m_freestack.clear();
				m_nodeset.clear();
			}
			GINL GAIA::GVOID destroy()
			{
				this->destroy_content_all();
				m_nodelist.destroy();
				m_freestack.destroy();
				m_nodeset.destroy();
			}
			GINL _SizeType alloc(const _DataType* p)
			{
				GPCHR_NULLSTR_RET(p, GINVALID);
				Node finder;
				finder.data = GCCAST(_DataType*)(p);
				NodeCmp* pFinded = m_nodeset.find(finder);
				if(pFinded != GNIL)
				{
					++m_nodelist[pFinded->index].refcounter;
					return pFinded->index;
				}
				finder.data = GAIA::ALGO::gstrnew(p);
				finder.refcounter = 1;
				if(m_freestack.empty())
				{
					finder.index = m_nodelist.size();
					m_nodelist.push_back(finder);
				}
				else
				{
					finder.index = m_freestack.back();
					m_nodelist[finder.index] = finder;
					m_freestack.pop_back();
				}
				m_nodeset.insert(finder);
				return finder.index;
			}
			GINL _SizeType request(const _DataType* p)
			{
				GPCHR_NULLSTR_RET(p, GINVALID);
				Node finder;
				finder.data = GCCAST(_DataType*)(p);
				NodeCmp* pFinded = m_nodeset.find(finder);
				if(pFinded != GNIL)
				{
					++m_nodelist[pFinded->index].refcounter;
					return pFinded->index;
				}
				return GINVALID;
			}
			GINL GAIA::BL release(const _SizeType& index)
			{
				Node& finder = m_nodelist[index];
				GAST(finder.refcounter != 0);
				if(finder.refcounter == 0)
					return GAIA::False;
				--finder.refcounter;
				if(finder.refcounter == 0)
				{
					m_nodeset.erase(finder);
					m_freestack.push_back(finder.index);
					gdel[] finder.data;
					finder.data = GNIL;
				}
				return GAIA::True;
			}
			GINL _SizeType find(const _DataType* p) // The find result not need release.
			{
				GPCHR_NULLSTR_RET(p, GINVALID);
				Node finder;
				finder.data = GCCAST(_DataType*)(p);
				NodeCmp* pFinded = m_nodeset.find(finder);
				if(pFinded != GNIL)
				{
					++m_nodelist[pFinded->index].refcounter;
					return pFinded->index;
				}
				return GINVALID;
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
			GCLASS_COMPARE(m_nodeset, __MyType)
		private:
			class NodeCmp : public GAIA::Base
			{
			public:
				GINL NodeCmp& operator = (const NodeCmp& src){data = src.data; index = src.index; return *this;}
				GINL GAIA::N32 compare(const NodeCmp& src) const
				{
					GAST(data != GNIL);
					GAST(src.data != GNIL); 
					return GAIA::ALGO::gstrcmp(data, src.data);
				}
				GCLASS_COMPARE_BYCOMPARE(NodeCmp)
			public:
				_DataType* data;
				_SizeType index;
			};
			class Node : public NodeCmp
			{
			public:
				GINL Node& operator = (const Node& src){this->NodeCmp::operator = (src); refcounter = src.refcounter; return *this;}
			public:
				_RefCounterType refcounter;
			};
			typedef GAIA::CTN::BasicVector<Node, _SizeType, _ExtendType> __NodeListType;
			typedef GAIA::CTN::BasicStack<_SizeType, _SizeType, _ExtendType> __FreeIndexStackType;
			typedef GAIA::CTN::BasicSet<NodeCmp, _SizeType, _SizeType, _ExtendType> __NodeSetType;
		private:
			GINL GAIA::GVOID destroy_content_all()
			{
				for(typename __NodeSetType::it it = m_nodeset.frontit(); !it.empty(); )
				{
					_DataType* p = (*it).data;
					it.erase();
					gdel[] p;
				}
			}
		private:
			__NodeListType m_nodelist;
			__FreeIndexStackType m_freestack;
			__NodeSetType m_nodeset;
		};
		template<typename _DataType> class StringPool : public GAIA::CTN::BasicStringPool<_DataType, GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
