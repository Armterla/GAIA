#ifndef 	__GAIA_CTN_POOL_H__
#define 	__GAIA_CTN_POOL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_extend.h"
#include "gaia_ctn_stack.h"
#include "gaia_ctn_vector.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, typename _ExtendType> class BasicPool : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicPool<_DataType, _SizeType, _ExtendType> __MyType;
		public:
			GINL BasicPool(){this->init();}
			GINL ~BasicPool(){this->destroy();}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL GAIA::GVOID clear()
			{
				m_free.clear();
				for(_SizeType x = 0; x < m_buf.size(); ++x)
				{
					_DataType* p = m_buf[x];
					_SizeType elecount = this->group_count(x);
					for(_SizeType y = 0; y < elecount; ++y)
					{
						GAST(m_free.capacity() > m_free.size());
						m_free.push_back(p + y);
					}
				}
			}
			GINL _SizeType size() const{return this->capacity() - m_free.size();}
			GINL const _SizeType& capacity() const{return m_free.capacity();}
			GINL GAIA::GVOID destroy()
			{
				for(_SizeType x = 0; x < m_buf.size(); ++x)
					gdel[] m_buf[x];
				m_buf.destroy();
				m_free.destroy();
			}
			GINL _DataType* alloc()
			{
				if(this->size() == this->capacity())
					this->alloc_group();
				_DataType* pTop = m_free.back();
				m_free.pop_back();
				return pTop;
			}
			GINL GAIA::GVOID release(_DataType* p)
			{
				GAST(m_free.capacity() > m_free.size());
				m_free.push_back(p);
			}
		private:
			GINL GAIA::GVOID init(){}
			GINL GAIA::GVOID alloc_group()
			{
				_SizeType newsize = this->group_count(m_buf.size());
				GAST(newsize > 0);
				_DataType* p = gnew _DataType[newsize];
				m_buf.push_back(p);

				m_free.reserve_keep(m_free.capacity() + newsize);
				for(_SizeType x = 0; x < newsize; ++x)
				{
					GAST(m_free.capacity() > m_free.size());
					m_free.push_back(p + x);
				}
			}
			GINL _SizeType group_count(const _SizeType& t) const{return t + 1;}
		private:
			BasicVector<_DataType*, _SizeType, _ExtendType> m_buf;
			BasicStack<_DataType*, _SizeType, _ExtendType> m_free;
		};
		template<typename _DataType> class Pool : public BasicPool<_DataType, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
