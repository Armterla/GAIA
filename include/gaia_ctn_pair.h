#ifndef		__GAIA_CTN_PAIR_H__
#define		__GAIA_CTN_PAIR_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _FrontDataType, typename _BackDataType> class Pair : public GAIA::Base
		{
		public:
			typedef _FrontDataType _frontdatatype;
			typedef _BackDataType _backdatatype;
		public:
			typedef Pair<_FrontDataType, _BackDataType> __MyType;
		public:
			GINL Pair(){}
			GINL Pair(const _FrontDataType& front, const _BackDataType& back){m_front = front; m_back = back;}
			GINL Pair(const Pair<_FrontDataType, _BackDataType>& src){this->operator = (src);}
			GINL GAIA::GVOID front(const _FrontDataType& front){m_front = front;}
			GINL GAIA::GVOID back(const _BackDataType& back){m_back = back;}
			GINL GAIA::GVOID frontback(const _FrontDataType& front, const _BackDataType& back){m_front = front; m_back = back;}
			GINL _FrontDataType& front(){return m_front;}
			GINL const _FrontDataType& front() const{return m_front;}
			GINL _BackDataType& back(){return m_back;}
			GINL const _BackDataType& back() const{return m_back;}
			GINL __MyType& operator = (const __MyType& src){GAST(&src != this); m_front = src.m_front; m_back = src.m_back; return *this;}
			GCLASS_COMPARE2(m_front, m_back, __MyType)
		private:
			_FrontDataType m_front;
			_BackDataType m_back;
		};
	}
}

#endif
