#ifndef 	__GAIA_CTN_SET_H__
#define 	__GAIA_CTN_SET_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_avltree.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _HeightType, typename _ExtendType> class BasicSet : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _HeightType _heighttype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicSet<_DataType, _SizeType, _HeightType, _ExtendType> __MyType;
			typedef BasicAVLTree<_DataType, _SizeType, _HeightType, _ExtendType> __AVLTreeType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicSet;
			public:
				GINL it(){}
				GINL virtual GAIA::BL empty() const{return m_iter.empty();}
				GINL virtual GAIA::GVOID clear(){m_iter.clear();}
				GINL virtual GAIA::BL erase(){return m_iter.erase();}
				GINL virtual _DataType& operator * (){return *m_iter;}
				GINL virtual const _DataType& operator * () const{return *m_iter;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (){++m_iter; return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (){--m_iter; return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator = (const GAIA::ITERATOR::Iterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator == (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator != (*GSCAST(const it*)(&src));}
				GINL it& operator = (const it& src){GAST(&src != this); m_iter = src.m_iter; return *this;}
				GINL it& operator += (const _SizeType& c){m_iter += c; return *this;}
				GINL it& operator -= (const _SizeType& c){m_iter -= c; return *this;}
				GINL it operator + (const _SizeType& c) const{it ret = *this; ret += c; return ret;}
				GINL it operator - (const _SizeType& c) const{it ret = *this; ret -= c; return ret;}
				GINL _SizeType operator - (const it& src) const{return m_iter - src.m_iter;}
				GCLASS_COMPARE(m_iter, it)
			private:
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
			private:
				typename __AVLTreeType::it m_iter;
			};
			class const_it : public GAIA::ITERATOR::ConstIterator<_DataType>
			{
			private:
				friend class BasicSet;
			public:
				GINL const_it(){}
				GINL virtual GAIA::BL empty() const{return m_iter.empty();}
				GINL virtual GAIA::GVOID clear(){m_iter.clear();}
				GINL virtual const _DataType& operator * () const{return *m_iter;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (){++m_iter; return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (){--m_iter; return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator = (const GAIA::ITERATOR::ConstIterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator == (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator != (*GSCAST(const const_it*)(&src));}
				GINL const_it& operator = (const const_it& src){GAST(&src != this); m_iter = src.m_iter; return *this;}
				GINL const_it& operator += (const _SizeType& c){m_iter += c; return *this;}
				GINL const_it& operator -= (const _SizeType& c){m_iter -= c; return *this;}
				GINL const_it operator + (const _SizeType& c) const{const_it ret = *this; ret += c; return ret;}
				GINL const_it operator - (const _SizeType& c) const{const_it ret = *this; ret -= c; return ret;}
				GINL _SizeType operator - (const const_it& src) const{return m_iter - src.m_iter;}
				GCLASS_COMPARE(m_iter, const_it)
			private:
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
			private:
				typename __AVLTreeType::const_it m_iter;
			};
		public:
			GINL BasicSet(){}
			GINL BasicSet(const __MyType& src){this->operator = (src);}
			GINL GAIA::BL empty() const{return m_avltree.empty();}
			GINL _SizeType size() const{return m_avltree.size();}
			GINL const _SizeType& capacity() const{return m_avltree.capacity();}
			GINL GAIA::GVOID clear(){m_avltree.clear();}
			GINL GAIA::GVOID destroy(){m_avltree.destroy();}
			GINL GAIA::BL insert(const _DataType& t, typename __MyType::it* pIter = GNIL)
			{
				if(pIter != GNIL)
					return m_avltree.insert(t, &pIter->m_iter);
				else
					return m_avltree.insert(t, GNIL);
			}
			GINL GAIA::BL erase(const _DataType& t){return m_avltree.erase(t);}
			GINL _DataType* find(const _DataType& t){return m_avltree.find(t);}
			GINL const _DataType* find(const _DataType& t) const{return m_avltree.find(t);}
			GINL it upper_equal(const _DataType& t){it ret; ret.m_iter = m_avltree.upper_equal(t); return ret;}
			GINL it lower_equal(const _DataType& t){it ret; ret.m_iter = m_avltree.lower_equal(t); return ret;}
			GINL const_it upper_equal(const _DataType& t) const{const_it ret; ret.m_iter = m_avltree.upper_equal(t); return ret;}
			GINL const_it lower_equal(const _DataType& t) const{const_it ret; ret.m_iter = m_avltree.lower_equal(t); return ret;}
			GINL _DataType& front(){return *this->frontit();}
			GINL const _DataType& front() const{return *this->const_frontit();}
			GINL _DataType& back(){return *this->backit();}
			GINL const _DataType& back() const{return *this->const_backit();}
			GINL it frontit(){it ret; ret.m_iter = m_avltree.frontit(); return ret;}
			GINL it backit(){it ret; ret.m_iter = m_avltree.backit(); return ret;}
			GINL const_it const_frontit() const{const_it ret; ret.m_iter = m_avltree.const_frontit(); return ret;}
			GINL const_it const_backit() const{const_it ret; ret.m_iter = m_avltree.const_backit(); return ret;}
			GINL __MyType& operator = (const __MyType& src){GAST(&src != this); m_avltree = src.m_avltree; return *this;}
			GINL __MyType& operator += (const __MyType& src)
			{
				m_avltree += src.m_avltree;
				return *this;
			}
			GINL GAIA::N32 compare(const __MyType& src) const{return m_avltree.compare(src.m_avltree);}
			GCLASS_COMPARE(m_avltree, __MyType)
		private:
			__AVLTreeType m_avltree;
		};
		template<typename _DataType> class Set : public BasicSet<_DataType, GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
