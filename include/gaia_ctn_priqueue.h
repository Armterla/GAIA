#ifndef		__GAIA_CTN_PRIQUEUE_H__
#define		__GAIA_CTN_PRIQUEUE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_avltree.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicPriQueue : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicPriQueue<_DataType, _SizeType, _ExtendType> __MyType;
			typedef BasicAVLTree<_DataType, _SizeType, _SizeType, _ExtendType> __AVLTreeType;
		public:
			GINL BasicPriQueue(){this->init();}
			GINL BasicPriQueue(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return m_avltree.empty();}
			GINL _SizeType size() const{return m_avltree.size();}
			GINL const _SizeType& capacity() const{return m_avltree.capacity();}
			GINL GAIA::GVOID resize(const _SizeType& size){GAST(size >= 0); return m_avltree.resize();}
			GINL GAIA::GVOID reserve(const _SizeType& size){GAST(size >= 0); return m_avltree.reserve();}
			GINL GAIA::GVOID clear(){return m_avltree.clear();}
			GINL GAIA::GVOID destroy(){return m_avltree.destroy();}
			GINL GAIA::BL insert(const _DataType& t){return m_avltree.insert(t);}
			GINL GAIA::BL erase(const _DataType& t){return m_avltree.erase(t);}
			GINL const _DataType* find(const _DataType& t) const{return m_avltree.find(t);}
			GINL _DataType* find(const _DataType& t){return m_avltree.find(t);}
			GINL GAIA::BL pop_front()
			{
				const _DataType* p = m_avltree.minimize();
				if(p == GNIL)
					return GNIL;
				return m_avltree.erase(*p);
			}
			GINL GAIA::BL pop_back()
			{
				const _DataType* p = m_avltree.maximize();
				if(p == GNIL)
					return GNIL;
				return m_avltree.erase(*p);
			}
			GINL const _DataType& front() const{return *m_avltree.minimize();}
			GINL _DataType& front(){return *m_avltree.minimize();}
			GINL const _DataType& back() const{return *m_avltree.maximize();}
			GINL _DataType& back(){return *m_avltree.maximize();}
			GINL __MyType& operator = (const __MyType& src){GAST(&src != this); m_avltree = src.m_avltree; return *this;}
			GCLASS_COMPARE(m_avltree, __MyType)
		private:
			GINL GAIA::GVOID init(){}
		private:
			__AVLTreeType m_avltree;
		};
	}
}

#endif
