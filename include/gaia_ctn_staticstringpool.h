#ifndef 	__GAIA_CTN_STATICSTRINGPOOL_H__
#define 	__GAIA_CTN_STATICSTRINGPOOL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_set.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicStaticStringPool : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicStaticStringPool<_DataType, _SizeType, _ExtendType> __MyType;
		public:
			GINL BasicStaticStringPool(){}
			GINL BasicStaticStringPool(const __MyType& src){this->operator = (src);}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL _SizeType size() const{return m_datalist.size();}
			GINL _SizeType capacity() const{return m_datalist.capacity();}
			GINL GAIA::GVOID reserve(const _SizeType& size){GAST(size >= 0); this->destroy(); m_datalist.reserve(size);}
			GINL GAIA::GVOID clear(){m_datalist.clear(); m_nodeset.clear(); m_nodelist.clear();}
			GINL GAIA::GVOID destroy(){m_datalist.destroy(); m_nodeset.destroy(); m_nodelist.destroy();}
			GINL _SizeType string_size() const{return m_nodelist.size();}
			GINL const _DataType* get(const _SizeType& index) const{return m_datalist.fptr() + m_nodelist[index].offset;}
			GINL _SizeType alloc(const _DataType* p)
			{
				GPCHR_NULLSTR_RET(p, GINVALID);
				Node finder;
				finder.pContainer = this;
				GAIA::BL bSelfData;
				if(!m_datalist.empty() && p >= m_datalist.fptr() && p <= m_datalist.bptr())
				{
					finder.offset = (_SizeType)(p - m_datalist.fptr());
					bSelfData = GAIA::True;
				}
				else
				{
					finder.offset = m_datalist.size();
					_SizeType len = GAIA::ALGO::gstrlen(p);
					m_datalist.push_back(p, len + 1);
					bSelfData = GAIA::False;
				}
				Node* pFinded = m_nodeset.find(finder);
				if(pFinded == GNIL)
				{
					GAST(!bSelfData);
					finder.index = m_nodelist.size();
					m_nodelist.push_back(finder);
					m_nodeset.insert(finder);
					return finder.index;
				}
				else
				{
					if(!bSelfData)
						m_datalist.resize(finder.offset);
					return pFinded->index;
				}
			}
			GINL _SizeType request(const _DataType* p)
			{
				GPCHR_NULLSTR_RET(p, GINVALID);
				Node finder;
				finder.pContainer = this;
				GAIA::BL bSelfData;
				if(!m_datalist.empty() && p >= m_datalist.fptr() && p <= m_datalist.bptr())
				{
					finder.offset = (_SizeType)(p - m_datalist.fptr());
					bSelfData = GAIA::True;
				}
				else
				{
					finder.offset = m_datalist.size();
					_SizeType len = GAIA::ALGO::gstrlen(p);
					m_datalist.push_back(p, len + 1);
					bSelfData = GAIA::False;
				}
				Node* pFinded = m_nodeset.find(finder);
				if(!bSelfData)
					m_datalist.resize(finder.offset);
				if(pFinded == GNIL)
					return GINVALID;
				return pFinded->index;
			}
			GINL _SizeType find(const _DataType* p)
			{
				GPCHR_NULLSTR_RET(p, GINVALID);
				Node finder;
				finder.pContainer = this;
				GAIA::BL bSelfData;
				if(!m_datalist.empty() && p >= m_datalist.fptr() && p <= m_datalist.bptr())
				{
					finder.offset = p - m_datalist.fptr();
					bSelfData = GAIA::True;
				}
				else
				{
					finder.offset = m_datalist.size();
					_SizeType len = GAIA::ALGO::gstrlen(p);
					m_datalist.push_back(p, len + 1);
					bSelfData = GAIA::False;
				}
				Node* pFinded = m_nodeset.find(finder);
				if(pFinded == GNIL)
				{
					GAST(!bSelfData);
					return GINVALID;
				}
				else
				{
					if(!bSelfData)
						m_datalist.resize(finder.offset);
					return pFinded->index;
				}
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				this->clear();
				for(_SizeType x = 0; x < src.string_size(); ++x)
					this->alloc(src.get(x));
				return *this;
			}
			GINL const _DataType& operator[](const _SizeType& index) const{return m_datalist[index];}
			GINL _DataType& operator[](const _SizeType& index){return m_datalist[index];}
			GINL _DataType* fptr(){return &this->operator[](0);}
			GINL const _DataType* fptr() const{return &this->operator[](0);}
			GINL _DataType* bptr(){return &this->operator[](this->size() - 1);}
			GINL const _DataType* bptr() const{return &this->operator[](this->size() - 1);}
			GCLASS_COMPARE(m_nodeset, __MyType)
		private:
			class Node : public GAIA::Base
			{
			public:
				GINL Node& operator = (const Node& src){pContainer = src.pContainer; offset = src.offset; index = src.index; return *this;}
				GINL GAIA::N32 compare(const Node& src) const
				{
					GAST(offset < pContainer->m_datalist.size());
					GAST(src.offset < src.pContainer->m_datalist.size());
					return GAIA::ALGO::gstrcmp(
						pContainer->m_datalist.fptr() + offset,
						src.pContainer->m_datalist.fptr() + src.offset);
				}
				GCLASS_COMPARE_BYCOMPARE(Node)
			public:
				__MyType* pContainer;
				_SizeType offset;
				_SizeType index;
			};
			typedef GAIA::CTN::BasicVector<_DataType, _SizeType, _ExtendType> __DataListType;
			typedef GAIA::CTN::BasicSet<Node, _SizeType, _SizeType, _ExtendType> __NodeSetType;
			typedef GAIA::CTN::BasicVector<Node, _SizeType, _ExtendType> __NodeListType;
		private:
			__DataListType m_datalist;
			__NodeSetType m_nodeset;
			__NodeListType m_nodelist;
		};
		template<typename _DataType> class StaticStringPool : public GAIA::CTN::BasicStaticStringPool<_DataType, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
