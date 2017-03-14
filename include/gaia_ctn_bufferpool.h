#ifndef		__GAIA_CTN_BUFFERPOOL_H__
#define		__GAIA_CTN_BUFFERPOOL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_stack.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_set.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _SizeType, typename _RefCounterType, typename _ExtendType> class BasicBufferPool : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
			typedef _RefCounterType _refcountertype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicBufferPool<_SizeType, _RefCounterType, _ExtendType> __MyType;
		public:
			GINL BasicBufferPool(){}
			GINL BasicBufferPool(const __MyType& src){this->operator = (src);}
			GINL ~BasicBufferPool(){this->destroy();}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_nodelist.size();}
			GINL _SizeType capacity() const{return m_nodelist.capacity();}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				this->destroy();
				m_nodelist.reserve(size);
				m_freestack.reserve(size);
			}
			GINL GAIA::GVOID clear()
			{
				this->destroy_content_all();
				m_nodelist.clear();
				m_nodeset.clear();
				m_freestack.clear();
			}
			GINL GAIA::GVOID destroy()
			{
				this->destroy_content_all();
				m_nodelist.destroy();
				m_nodeset.destroy();
				m_freestack.destroy();
			}
			GINL const GAIA::U8* alloc(const GAIA::GVOID* p, const _SizeType& size)
			{
				GPCHR_NULL_RET(p, GNIL);
				GPCHR_BELOWEQUALZERO_RET(size, GNIL);
				NodeCmp finder;
				finder.data = (GAIA::U8*)p;
				finder.datasize = size;
				NodeCmp* pFinded = m_nodeset.find(finder);
				if(pFinded == GNIL)
				{
					finder.data = gnew GAIA::U8[size];
					GAIA::ALGO::gmemcpy(finder.data, p, size);
					finder.sRefCnt = 1;
					if(m_freestack.empty())
						finder.index = m_nodelist.size();
					else
					{
						finder.index = m_freestack.back();
						m_freestack.pop_back();
					}
					m_nodeset.insert(finder);
					pFinded = m_nodeset.find(finder);
					GAST(pFinded != GNIL);
					Node n;
					n.data = finder.data;
					n.datasize = size;
					if(finder.index == m_nodelist.size())
						m_nodelist.push_back(n);
					else
						m_nodelist[finder.index] = n;
				}
				else
					pFinded->sRefCnt++;
				return pFinded->data;
			}
			GINL const GAIA::U8* request(const GAIA::GVOID* p, const _SizeType& size)
			{
				GPCHR_NULL_RET(p, GNIL);
				GPCHR_BELOWEQUALZERO_RET(size, GNIL);
				NodeCmp finder;
				finder.data = (GAIA::U8*)p;
				finder.datasize = size;
				NodeCmp* pFinded = m_nodeset.find(finder);
				if(pFinded == GNIL)
					return GNIL;
				pFinded->sRefCnt++;
				return pFinded->data;
			}
			GINL GAIA::BL release(const GAIA::GVOID* p, const _SizeType& size)
			{
				GPCHR_NULL_RET(p, GNIL);
				GPCHR_BELOWEQUALZERO_RET(size, GNIL);
				NodeCmp finder;
				finder.data = (GAIA::U8*)p;
				finder.datasize = size;
				typename __NodeSet::it it = m_nodeset.lower_equal(finder);
				if(it.empty())
					return GAIA::False;
				NodeCmp& finded = *it;
				if(finded.data != p)
					return GAIA::False;
				if(finded.datasize != size)
					return GAIA::False;
				finded.sRefCnt--;
				if(finded.sRefCnt == 0)
				{
					Node& n = m_nodelist[finded.index];
					n.data = GNIL;
					n.datasize = 0;
					m_freestack.push_back(finded.index);
					GAIA::U8* pData = finded.data;
					if(!it.erase())
						return GAIA::False;
					gdel[] pData;
				}
				return GAIA::True;
			}
			GINL const GAIA::U8* find(const GAIA::GVOID* p, const _SizeType& size) const
			{
				GPCHR_NULL_RET(p, GNIL);
				GPCHR_BELOWEQUALZERO_RET(size, GNIL);
				NodeCmp finder;
				finder.data = (GAIA::U8*)p;
				finder.datasize = size;
				NodeCmp* pFinded = m_nodeset.find(finder);
				if(pFinded == GNIL)
					return GNIL;
				return pFinded->data;
			}
			GINL _SizeType buffer_size() const{return m_nodelist.size();}
			GINL const GAIA::U8* get(const _SizeType& index, _SizeType& size) const
			{
				const Node& n = m_nodelist[index];
				size = n.datasize;
				return n.data;
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				this->clear();
				for(_SizeType x = 0; x < src.buffer_size(); ++x)
				{
					_SizeType sBufferSize;
					const GAIA::U8* p = src.get(x, sBufferSize);
					if(p == GNIL)
						continue;
					this->alloc(p, sBufferSize);
				}
				return *this;
			}
		private:
			GINL GAIA::GVOID destroy_content_all()
			{
				for(typename __NodeSet::it it = m_nodeset.frontit(); !it.empty(); ++it)
				{
					NodeCmp& nc = *it;
					GAST(nc.data != GNIL);
					GAST(nc.datasize >= 0);
					gdel[] nc.data;
				}
			}
		private:
			class Node : public GAIA::Base
			{
			public:
				GAIA::U8* data;
				_SizeType datasize;
			};
			class NodeCmp : public Node
			{
			public:
				GINL GAIA::N32 compare(const NodeCmp& src) const
				{
					if(this->datasize < src.datasize)
						return +1;
					else if(this->datasize > src.datasize)
						return -1;
					return GAIA::ALGO::gmemcmp(this->data, src.data, this->datasize);
				}
				GCLASS_COMPARE_BYCOMPARE(NodeCmp)
			public:
				_SizeType index;
				_RefCounterType sRefCnt;
			};
		private:
			typedef GAIA::CTN::BasicVector<Node, _SizeType, _ExtendType> __NodeList;
			typedef GAIA::CTN::BasicSet<NodeCmp, _SizeType, _SizeType, _ExtendType> __NodeSet;
			typedef GAIA::CTN::BasicStack<_SizeType, _SizeType, _ExtendType> __FreeStack;
		private:
			__NodeList m_nodelist;
			__NodeSet m_nodeset;
			__FreeStack m_freestack;
		};
		class BufferPool : public BasicBufferPool<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
