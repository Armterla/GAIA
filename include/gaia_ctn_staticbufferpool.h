#ifndef		__GAIA_CTN_STATICBUFFERPOOL_H__
#define		__GAIA_CTN_STATICBUFFERPOOL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_set.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _SizeType, typename _ExtendType> class BasicStaticBufferPool : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicStaticBufferPool<_SizeType, _ExtendType> __MyType;
		public:
			GINL BasicStaticBufferPool(){}
			GINL BasicStaticBufferPool(const __MyType& src){this->operator = (src);}
			GINL ~BasicStaticBufferPool(){this->destroy();}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_nodelist.size();}
			GINL _SizeType capacity() const{return m_nodelist.capacity();}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				this->destroy();
				m_nodelist.reserve(size);
			}
			GINL GAIA::GVOID clear()
			{
				this->destroy_content_all();
				m_nodelist.clear();
				m_nodeset.clear();
			}
			GINL GAIA::GVOID destroy()
			{
				this->destroy_content_all();
				m_nodelist.destroy();
				m_nodeset.destroy();
			}
			GINL const GAIA::U8* alloc(const GAIA::GVOID* p, const _SizeType& size)
			{
				GPCHR_NULL_RET(p, GNIL);
				GPCHR_BELOWEQUALZERO_RET(size, GNIL);
				Node finder;
				finder.data = (GAIA::U8*)p;
				finder.datasize = size;
				Node* pFinded = m_nodeset.find(finder);
				if(pFinded == GNIL)
				{
					finder.data = gnew GAIA::U8[size];
					GAIA::ALGO::gmemcpy(finder.data, p, size);
					m_nodeset.insert(finder);
					pFinded = m_nodeset.find(finder);
					GAST(pFinded != GNIL);
					m_nodelist.push_back(finder);
				}
				return pFinded->data;
			}
			GINL const GAIA::U8* request(const GAIA::GVOID* p, const _SizeType& size)
			{
				GPCHR_NULL_RET(p, GNIL);
				GPCHR_BELOWEQUALZERO_RET(size, GNIL);
				Node finder;
				finder.data = (GAIA::U8*)p;
				finder.datasize = size;
				Node* pFinded = m_nodeset.find(finder);
				if(pFinded == GNIL)
					return GNIL;
				return pFinded->data;
			}
			GINL const GAIA::U8* find(const GAIA::GVOID* p, const _SizeType& size) const
			{
				GPCHR_NULL_RET(p, GNIL);
				GPCHR_BELOWEQUALZERO_RET(size, GNIL);
				Node finder;
				finder.data = (GAIA::U8*)p;
				finder.datasize = size;
				Node* pFinded = m_nodeset.find(finder);
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
				for(typename __NodeSet::it it = m_nodeset.frontit(); !it.empty(); )
				{
					Node& nc = *it;
					GAST(nc.data != GNIL);
					GAST(nc.datasize >= 0);
					GAIA::U8* p = nc.data;
					it.erase();
					gdel[] p;
				}
			}
		private:
			class Node : public GAIA::Base
			{
			public:
				GINL GAIA::N32 compare(const Node& src) const
				{
					if(this->datasize < src.datasize)
						return +1;
					else if(this->datasize > src.datasize)
						return -1;
					return GAIA::ALGO::gmemcmp(this->data, src.data, this->datasize);
				}
				GCLASS_COMPARE_BYCOMPARE(Node)
			public:
				GAIA::U8* data;
				_SizeType datasize;
			};
		private:
			typedef GAIA::CTN::BasicVector<Node, _SizeType, _ExtendType> __NodeList;
			typedef GAIA::CTN::BasicSet<Node, _SizeType, _SizeType, _ExtendType> __NodeSet;
			typedef GAIA::CTN::BasicStack<_SizeType, _SizeType, _ExtendType> __FreeStack;
		private:
			__NodeList m_nodelist;
			__NodeSet m_nodeset;
		};
		class StaticBufferPool : public BasicStaticBufferPool<GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
