#ifndef		__GAIA_CTN_STRINGREF_H__
#define		__GAIA_CTN_STRINGREF_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_iterator.h"
#include "gaia_algo_compare.h"
#include "gaia_algo_string.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType> class BasicStringRef : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
		public:
			typedef BasicStringRef<_DataType, _SizeType> __MyType;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicStringRef;
			public:
				GINL it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pContainer == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual _DataType& operator * (){GAST(!!m_pContainer); return (*m_pContainer)[m_index];}
				GINL virtual const _DataType& operator * () const{GAST(!!m_pContainer); return (*m_pContainer)[m_index];}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (){GAST(!!m_pContainer); ++m_index; if(m_index >= m_pContainer->size()) this->init(); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (){GAST(!!m_pContainer); --m_index; if(m_index >= m_pContainer->size() || m_index < 0) this->init(); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator = (const GAIA::ITERATOR::Iterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator == (*GSCAST(const it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::Iterator<_DataType>& src) const{return this->operator != (*GSCAST(const it*)(&src));}
				GINL it& operator = (const it& src){GAST(&src != this); m_index = src.m_index; m_pContainer = src.m_pContainer; return *this;}
				GINL it& operator += (const _SizeType& c)
				{
					if(m_pContainer == GNIL)
						return *this;
					m_index += c;
					if(m_index >= m_pContainer->size() || m_index < 0)
						this->init();
					return *this;
				}
				GINL it& operator -= (const _SizeType& c)
				{
					if(m_pContainer == GNIL)
						return *this;
					m_index -= c;
					if(m_index >= m_pContainer->size() || m_index < 0)
						this->init();
					return *this;
				}
				GINL it operator + (const _SizeType& c) const
				{
					it ret = *this;
					ret += c;
					return ret;
				}
				GINL it operator - (const _SizeType& c) const
				{
					it ret = *this;
					ret -= c;
					return ret;
				}
				GINL _SizeType operator - (const it& src) const
				{
					if(this->empty() || src.empty())
						return 0;
					return this->m_index - src.m_index;
				}
				GCLASS_COMPARE(m_index, it)
			private:
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::Iterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
			private:
				GINL GAIA::GVOID init(){m_pContainer = GNIL; m_index = 0;}
			private:
				_SizeType m_index;
				__MyType* m_pContainer;
			};
			class const_it : public GAIA::ITERATOR::ConstIterator<_DataType>
			{
			private:
				friend class BasicStringRef;
			public:
				GINL const_it(){this->init();}
				GINL virtual GAIA::BL empty() const{return m_pContainer == GNIL;}
				GINL virtual GAIA::GVOID clear(){this->init();}
				GINL virtual const _DataType& operator * () const{GAST(!!m_pContainer); return (*m_pContainer)[m_index];}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (){GAST(!!m_pContainer); ++m_index; if(m_index >= m_pContainer->size()) this->init(); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (){GAST(!!m_pContainer); --m_index; if(m_index >= m_pContainer->size() || m_index < 0) this->init(); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator = (const GAIA::ITERATOR::ConstIterator<_DataType>& src){GAST(&src != this); return this->operator = (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator == (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator == (*GSCAST(const const_it*)(&src));}
				GINL virtual GAIA::BL operator != (const GAIA::ITERATOR::ConstIterator<_DataType>& src) const{return this->operator != (*GSCAST(const const_it*)(&src));}
				GINL const_it& operator = (const const_it& src){GAST(&src != this); m_index = src.m_index; m_pContainer = src.m_pContainer; return *this;}
				GINL const_it& operator += (const _SizeType& c)
				{
					if(m_pContainer == GNIL)
						return *this;
					m_index += c;
					if(m_index >= m_pContainer->size() || m_index < 0)
						this->init();
					return *this;
				}
				GINL const_it& operator -= (const _SizeType& c)
				{
					if(m_pContainer == GNIL)
						return *this;
					m_index -= c;
					if(m_index >= m_pContainer->size() || m_index < 0)
						this->init();
					return *this;
				}
				GINL const_it operator + (const _SizeType& c) const
				{
					const_it ret = *this;
					ret += c;
					return ret;
				}
				GINL const_it operator - (const _SizeType& c) const
				{
					const_it ret = *this;
					ret -= c;
					return ret;
				}
				GINL _SizeType operator - (const const_it& src) const
				{
					if(this->empty() || src.empty())
						return 0;
					return this->m_index - src.m_index;
				}
				GCLASS_COMPARE(m_index, const_it)
			private:
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator ++ (GAIA::N32){++(*this); return *this;}
				GINL virtual GAIA::ITERATOR::ConstIterator<_DataType>& operator -- (GAIA::N32){--(*this); return *this;}
			private:
				GINL GAIA::GVOID init(){m_pContainer = GNIL; m_index = 0;}
			private:
				_SizeType m_index;
				const __MyType* m_pContainer;
			};
		public:
			GINL BasicStringRef(){this->init();}
			GINL BasicStringRef(const __MyType& src){this->init(); this->operator = (src);}
			GINL BasicStringRef(_DataType* p){this->init(); this->operator = (p);}
			GINL BasicStringRef(_DataType* pFront, _DataType* pBack){this->init(); this->proxy(pFront, pBack);}
			GINL BasicStringRef(_DataType* pFront, const _SizeType& size){this->init(); this->proxy(pFront, size);}
			GINL GAIA::BL proxy(_DataType* pFront, _DataType* pBack)
			{
				if(pFront == GNIL && pBack != GNIL)
					return GAIA::False;
				if(pFront != GNIL && pBack == GNIL)
					return GAIA::False;
				if(pBack < pFront)
					return GAIA::False;
				m_pFront = pFront;
				m_pBack = pBack;
				return GAIA::True;
			}
			GINL GAIA::BL proxy(_DataType* pFront)
			{
				if(pFront == GNIL)
				{
					m_pFront = GNIL;
					m_pBack = GNIL;
				}
				else
				{
					m_pFront = pFront;
					m_pBack = GAIA::ALGO::gstrend(pFront) - 1;
				}
				return GAIA::True;
			}
			GINL GAIA::BL proxy(_DataType* pFront, const _SizeType& size)
			{
				if(pFront == GNIL)
				{
					m_pFront = GNIL;
					m_pBack = GNIL;
				}
				else
				{
					if(size <= 0)
						return GAIA::False;
					m_pFront = pFront;
					m_pBack = pFront + size - 1;
				}
				return GAIA::True;
			}
			GINL GAIA::BL empty() const{return m_pFront == GNIL;}
			GINL _SizeType size() const{if(m_pFront == GNIL) return 0; return (_SizeType)(m_pBack - m_pFront + 1);}
			GINL GAIA::GVOID clear(){m_pFront = m_pBack = GNIL;}
			GINL _DataType* fptr(){return m_pFront;}
			GINL const _DataType* fptr() const{return m_pFront;}
			GINL _DataType* bptr(){return m_pBack;}
			GINL const _DataType* bptr() const{return m_pBack;}
			GINL _DataType& front(){return *this->fptr();}
			GINL const _DataType& front() const{return *this->fptr();}
			GINL _DataType& back(){return *this->bptr();}
			GINL const _DataType& back() const{return *this->bptr();}
			GINL it frontit()
			{
				it ret;
				if(this->empty())
				{
					ret.m_index = 0;
					ret.m_pContainer = GNIL;
				}
				else
				{
					ret.m_index = 0;
					ret.m_pContainer = this;
				}
				return ret;
			}
			GINL it backit()
			{
				it ret;
				if(this->empty())
				{
					ret.m_index = 0;
					ret.m_pContainer = GNIL;
				}
				else
				{
					ret.m_index = this->size() - 1;
					ret.m_pContainer = this;
				}
				return ret;
			}
			GINL const_it const_frontit() const
			{
				const_it ret;
				if(this->empty())
				{
					ret.m_index = 0;
					ret.m_pContainer = GNIL;
				}
				else
				{
					ret.m_index = 0;
					ret.m_pContainer = this;
				}
				return ret;
			}
			GINL const_it const_backit() const
			{
				const_it ret;
				if(this->empty())
				{
					ret.m_index = 0;
					ret.m_pContainer = GNIL;
				}
				else
				{
					ret.m_index = this->size() - 1;
					ret.m_pContainer = this;
				}
				return ret;
			}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				if(this->empty() && src.empty())
					return 0;
				else if(!this->empty() && src.empty())
					return -1;
				else if(this->empty() && !src.empty())
					return +1;
				else
					return GAIA::ALGO::gstrcmp(this->fptr(), src.fptr(), GAIA::ALGO::gmin(this->size(), src.size()));
			}
			template<typename _ParamDataType> GAIA::N32 compare(const _ParamDataType* p) const
			{
				if(this->empty() && GAIA::ALGO::gstremp(p))
					return 0;
				else if(!this->empty() && GAIA::ALGO::gstremp(p))
					return -1;
				else if(this->empty() && !GAIA::ALGO::gstremp(p))
					return +1;
				else
					return GAIA::ALGO::gstrcmp(this->fptr(), p, GAIA::ALGO::gmin(this->size(), GAIA::ALGO::gstrlen(p)));
			}
			GINL __MyType& operator = (const __MyType& src)
			{
				m_pFront = src.m_pFront;
				m_pBack = src.m_pBack;
				return *this;
			}
			__MyType& operator = (const _DataType* p)
			{
				this->proxy(p);
				return *this;
			}
			GCLASS_COMPARE_BYCOMPARE(__MyType)
			template<typename _ParamDataType> GAIA::BL operator == (_ParamDataType p) const{return this->compare(p) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (_ParamDataType p) const{return this->compare(p) != 0;}
			template<typename _ParamDataType> GAIA::BL operator >= (_ParamDataType p) const{return this->compare(p) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (_ParamDataType p) const{return this->compare(p) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (_ParamDataType p) const{return this->compare(p) > 0;}
			template<typename _ParamDataType> GAIA::BL operator < (_ParamDataType p) const{return this->compare(p) < 0;}
			template<typename _ParamSizeType> const _DataType& operator[](const _ParamSizeType& index) const{GAST(index <= this->size()); return m_pFront[index];}
			template<typename _ParamSizeType> _DataType& operator[](const _ParamSizeType& index){GAST(index <= this->size()); return m_pFront[index];}
			GINL operator _DataType*(){return this->fptr();}
			GINL operator const _DataType*() const{return this->fptr();}
			GINL operator GAIA::NM() const{return GAIA::ALGO::casts<NM>(m_pFront);}
			GINL operator GAIA::N8() const{return GAIA::ALGO::casts<N8>(m_pFront);}
			GINL operator GAIA::N16() const{return GAIA::ALGO::casts<N16>(m_pFront);}
			GINL operator GAIA::N32() const{return GAIA::ALGO::casts<N32>(m_pFront);}
			GINL operator GAIA::N64() const{return GAIA::ALGO::casts<N64>(m_pFront);}
			GINL operator GAIA::UM() const{return GAIA::ALGO::casts<UM>(m_pFront);}
			GINL operator GAIA::U8() const{return GAIA::ALGO::casts<U8>(m_pFront);}
			GINL operator GAIA::U16() const{return GAIA::ALGO::casts<U16>(m_pFront);}
			GINL operator GAIA::U32() const{return GAIA::ALGO::casts<U32>(m_pFront);}
			GINL operator GAIA::U64() const{return GAIA::ALGO::casts<U64>(m_pFront);}
			GINL operator GAIA::X128() const
			{
				X128 ret;
				if(this->empty())
				{
					ret.clear();
					return ret;
				}
				GAIA::ALGO::str2hex(m_pFront, sizeof(ret), (GAIA::U8*)&ret);
				return ret;
			}
			GINL operator GAIA::F32() const{return GAIA::ALGO::casts<F32>(m_pFront);}
			GINL operator GAIA::F64() const{return GAIA::ALGO::casts<F64>(m_pFront);}
			GINL operator GAIA::BL() const{return GAIA::ALGO::casts<BL>(m_pFront);}
		private:
			GINL GAIA::GVOID init()
			{
				m_pFront = GNIL;
				m_pBack = GNIL;
			}
		private:
			_DataType* m_pFront;
			_DataType* m_pBack;
		};
		typedef GAIA::CTN::BasicStringRef<GAIA::CH, GAIA::NUM> StringRef;
		typedef GAIA::CTN::BasicStringRef<GAIA::WCH, GAIA::NUM> WStringRef;
		typedef GAIA::CTN::BasicStringRef<GAIA::TCH, GAIA::NUM> TStringRef;
	}
}

#endif
