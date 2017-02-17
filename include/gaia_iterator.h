#ifndef		__GAIA_ITERATOR_H__
#define		__GAIA_ITERATOR_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace ITERATOR
	{
		template<typename _DataType> class Iterator : public GAIA::Base
		{
		public:
			virtual ~Iterator(){}
			virtual GAIA::BL empty() const = 0;
			virtual GAIA::GVOID clear() = 0;
			virtual GAIA::BL erase(){GASTFALSE; return GAIA::False;}
			virtual _DataType& operator * () = 0;
			virtual const _DataType& operator * () const = 0;
			virtual Iterator<_DataType>& operator ++ () = 0;
			virtual Iterator<_DataType>& operator -- () = 0;
			virtual Iterator<_DataType>& operator = (const Iterator<_DataType>& src) = 0;
			virtual GAIA::BL operator == (const Iterator<_DataType>& src) const = 0;
			virtual GAIA::BL operator != (const Iterator<_DataType>& src) const = 0;
			virtual GAIA::BL operator ! (){return this->empty();}
		private:
			virtual Iterator<_DataType>& operator ++ (GAIA::N32) = 0;
			virtual Iterator<_DataType>& operator -- (GAIA::N32) = 0;
		};

		template<typename _DataType> class ConstIterator : public GAIA::Base
		{
		public:
			virtual ~ConstIterator(){}
			virtual GAIA::BL empty() const = 0;
			virtual GAIA::GVOID clear() = 0;
			virtual const _DataType& operator * () const = 0;
			virtual ConstIterator<_DataType>& operator ++ () = 0;
			virtual ConstIterator<_DataType>& operator -- () = 0;
			virtual ConstIterator<_DataType>& operator = (const ConstIterator<_DataType>& src) = 0;
			virtual GAIA::BL operator == (const ConstIterator<_DataType>& src) const = 0;
			virtual GAIA::BL operator != (const ConstIterator<_DataType>& src) const = 0;
			virtual GAIA::BL operator ! (){return this->empty();}
		private:
			virtual ConstIterator<_DataType>& operator ++ (GAIA::N32) = 0;
			virtual ConstIterator<_DataType>& operator -- (GAIA::N32) = 0;
		};

		template<typename _DataType, typename _IteratorType> class InverseIterator : public GAIA::Base
		{
		public:
			GINL InverseIterator(const InverseIterator<_DataType, _IteratorType>& src){this->operator = (src);}
			GINL InverseIterator(const _IteratorType& src){this->operator = (src);}
			GINL GAIA::BL empty() const{return m_it.empty();}
			GINL GAIA::GVOID clear(){m_it.clear();}
			GINL GAIA::BL erase()
			{
				if(this->empty())
					return GAIA::False;
				_IteratorType itprev = m_it;
				--itprev;
				if(!m_it.erase())
					return GAIA::False;
				m_it = itprev;
				return GAIA::True;
			}
			GINL _DataType& operator * (){return *m_it;}
			GINL const _DataType& operator * () const{return *m_it;}
			GINL InverseIterator<_DataType, _IteratorType>& operator ++ (){--m_it; return *this;}
			GINL InverseIterator<_DataType, _IteratorType>& operator -- (){++m_it; return *this;}
			GINL InverseIterator<_DataType, _IteratorType>& operator = (const InverseIterator<_DataType, _IteratorType>& src){m_it = src.m_it; return *this;}
			GINL InverseIterator<_DataType, _IteratorType>& operator = (const _IteratorType& src){m_it = src; return *this;}
			GINL GAIA::BL operator == (const InverseIterator<_DataType, _IteratorType>& src) const{return m_it == src.m_it;}
			GINL GAIA::BL operator != (const InverseIterator<_DataType, _IteratorType>& src) const{return m_it != src.m_it;}
			GINL GAIA::BL operator ! (){return this->empty();}
		private:
			GINL InverseIterator<_DataType, _IteratorType>& operator ++ (GAIA::N32){++(*this); return *this;}
			GINL InverseIterator<_DataType, _IteratorType>& operator -- (GAIA::N32){++(*this); return *this;}
		private:
			_IteratorType m_it;
		};

		template<typename _DataType, typename _IteratorType> class ConstInverseIterator : public GAIA::Base
		{
		public:
			GINL ConstInverseIterator(const ConstInverseIterator<_DataType, _IteratorType>& src){this->operator = (src);}
			GINL ConstInverseIterator(const _IteratorType& src){this->operator = (src);}
			GINL GAIA::BL empty() const{return m_it.empty();}
			GINL GAIA::GVOID clear(){m_it.clear();}
			GINL const _DataType& operator * () const{return *m_it;}
			GINL ConstInverseIterator<_DataType, _IteratorType>& operator ++ (){--m_it; return *this;}
			GINL ConstInverseIterator<_DataType, _IteratorType>& operator -- (){++m_it; return *this;}
			GINL ConstInverseIterator<_DataType, _IteratorType>& operator = (const ConstInverseIterator<_DataType, _IteratorType>& src){m_it = src.m_it; return *this;}
			GINL ConstInverseIterator<_DataType, _IteratorType>& operator = (const _IteratorType& src){m_it = src; return *this;}
			GINL GAIA::BL operator == (const ConstInverseIterator<_DataType, _IteratorType>& src) const{return m_it == src.m_it;}
			GINL GAIA::BL operator != (const ConstInverseIterator<_DataType, _IteratorType>& src) const{return m_it != src.m_it;}
			GINL GAIA::BL operator ! (){return this->empty();}
		private:
			GINL ConstInverseIterator<_DataType, _IteratorType>& operator ++ (GAIA::N32){++(*this); return *this;}
			GINL ConstInverseIterator<_DataType, _IteratorType>& operator -- (GAIA::N32){++(*this); return *this;}
		private:
			_IteratorType m_it;
		};
	}
}

#endif
