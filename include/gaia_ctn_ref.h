#ifndef		__GAIA_CTN_REF_H__
#define		__GAIA_CTN_REF_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_compare.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType> class Ref : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
		public:
			typedef Ref<_DataType> __MyType;
		public:
			GINL Ref(){this->init();}
			GINL Ref(const __MyType& src){this->init(); this->operator = (src);}
			GINL Ref(_DataType* p){this->init(); this->operator = (p);}
			GINL GAIA::BL empty() const{return m_p == GNIL;}
			GINL __MyType& operator = (const __MyType& src){GAST(&src != this); m_p = src.m_p; return *this;}
			GINL __MyType& operator = (_DataType* p){m_p = p; return *this;}
			GINL _DataType& operator * (){return *m_p;}
			GINL const _DataType& operator * () const{return *m_p;}
			template<typename _ParamSizeType> _DataType& operator [] (const _ParamSizeType& index){return m_p[index];}
			template<typename _ParamSizeType> const _DataType& operator [] (const _ParamSizeType& index) const{return m_p[index];}
			GINL GAIA::BL operator == (const __MyType& src) const{return GAIA::ALGO::cmpp(m_p, src.m_p) == 0;}
			GINL GAIA::BL operator != (const __MyType& src) const{return GAIA::ALGO::cmpp(m_p, src.m_p) != 0;}
			GINL GAIA::BL operator >= (const __MyType& src) const{return GAIA::ALGO::cmpp(m_p, src.m_p) >= 0;}
			GINL GAIA::BL operator <= (const __MyType& src) const{return GAIA::ALGO::cmpp(m_p, src.m_p) <= 0;}
			GINL GAIA::BL operator > (const __MyType& src) const{return GAIA::ALGO::cmpp(m_p, src.m_p) > 0;}
			GINL GAIA::BL operator < (const __MyType& src) const{return GAIA::ALGO::cmpp(m_p, src.m_p) < 0;}
			GINL GAIA::BL operator == (const _DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) == 0;}
			GINL GAIA::BL operator != (const _DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) != 0;}
			GINL GAIA::BL operator >= (const _DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) >= 0;}
			GINL GAIA::BL operator <= (const _DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) <= 0;}
			GINL GAIA::BL operator > (const _DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) > 0;}
			GINL GAIA::BL operator < (const _DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) < 0;}
			GINL GAIA::BL operator == (_DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) == 0;}
			GINL GAIA::BL operator != (_DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) != 0;}
			GINL GAIA::BL operator >= (_DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) >= 0;}
			GINL GAIA::BL operator <= (_DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) <= 0;}
			GINL GAIA::BL operator > (_DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) > 0;}
			GINL GAIA::BL operator < (_DataType* p) const{return GAIA::ALGO::cmpp(m_p, p) < 0;}
			GINL GAIA::BL operator == (const _DataType& t) const{return GAIA::ALGO::cmpp(m_p, &t) == 0;}
			GINL GAIA::BL operator != (const _DataType& t) const{return GAIA::ALGO::cmpp(m_p, &t) != 0;}
			GINL GAIA::BL operator >= (const _DataType& t) const{return GAIA::ALGO::cmpp(m_p, &t) >= 0;}
			GINL GAIA::BL operator <= (const _DataType& t) const{return GAIA::ALGO::cmpp(m_p, &t) <= 0;}
			GINL GAIA::BL operator > (const _DataType& t) const{return GAIA::ALGO::cmpp(m_p, &t) > 0;}
			GINL GAIA::BL operator < (const _DataType& t) const{return GAIA::ALGO::cmpp(m_p, &t) < 0;}
			GINL operator const _DataType*() const{return m_p;}
			GINL operator _DataType*(){return m_p;}
		private:
			GINL GAIA::GVOID init(){m_p = GNIL;}
		private:
			_DataType* m_p;
		};
	}
}

#endif
