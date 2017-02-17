#ifndef		__GAIA_CTN_CHARSSTRING_H__
#define		__GAIA_CTN_CHARSSTRING_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_locale.h"
#include "gaia_iterator.h"
#include "gaia_algo_string.h"
#include "gaia_ctn_chars.h"
#include "gaia_algo_compare.h"
#include "gaia_ctn_string.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType, _SizeType _Size> class BasicCharsString : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
		public:
			typedef BasicCharsString<_DataType, _SizeType, _Size> __MyType;
		private:
			static const GAIA::U32 GAIA_DIGIT_TOSTRING_LEN = 64;
			static const _SizeType _size = _Size;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicCharsString;
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
				friend class BasicCharsString;
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
			GINL BasicCharsString(){}
			template<typename _ParamDataType> BasicCharsString(const _ParamDataType* p){this->operator = (p);}
			template<typename _ParamDataType> BasicCharsString(const _ParamDataType* p, const GAIA::CH* pszLocale)
			{
				if(GAIA::ALGO::gstremp(p))
					return;
				if(GAIA::ALGO::gstremp(pszLocale))
					GTHROW(Constructor);
				if(sizeof(_DataType) == sizeof(GAIA::CH))
				{
					if(sizeof(_ParamDataType) == sizeof(GAIA::WCH))
					{
						GAIA::CHARSET_TYPE ct;
						if(GAIA::ALGO::gstrequal(pszLocale, "UTF-8"))
							ct = GAIA::CHARSET_TYPE_UTF8;
						else if(GAIA::ALGO::gstrequal(pszLocale, "SYS"))
							ct = GAIA::CHARSET_TYPE_SYS;
						else
							GTHROW(Constructor);
						GAIA::NUM sSrcLen = GAIA::ALGO::gstrlen(p);
						this->resize(sSrcLen * 6 + 1);
						GAIA::NUM sLen = GAIA::LOCALE::w2m((const GAIA::WCH*)p, sSrcLen, this->fptr(), this->size(), ct);
						if(sLen == 0)
						{
							this->clear();
							return;
						}
						this->resize(sLen);
					}
					else
						(*this) = (const _DataType*)p;
				}
				else if(sizeof(_DataType) == sizeof(GAIA::WCH))
				{
					if(sizeof(_ParamDataType) == sizeof(GAIA::WCH))
						(*this) = (const _DataType*)p;
					else
					{
						GAIA::CHARSET_TYPE ct;
						if(GAIA::ALGO::gstrequal(pszLocale, "UTF-8"))
							ct = GAIA::CHARSET_TYPE_UTF8;
						else if(GAIA::ALGO::gstrequal(pszLocale, "SYS"))
							ct = GAIA::CHARSET_TYPE_SYS;
						else
							GTHROW(Constructor);
						GAIA::NUM sSrcLen = GAIA::ALGO::gstrlen(p);
						this->resize(sSrcLen + 1);
						GAIA::NUM sLen = GAIA::LOCALE::m2w(p, sSrcLen, (GAIA::WCH*)this->fptr(), sSrcLen, ct);
						if(sLen == 0)
						{
							this->clear();
							return;
						}
						this->resize(sLen);
					}
				}
				else
					GTHROW(Constructor);
			}
			GINL BasicCharsString(const __MyType& src){this->operator = (src);}
			GINL BasicCharsString(const GAIA::NM& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::N8& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::N16& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::N32& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::N64& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::UM& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::U8& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::U16& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::U32& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::U64& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::X128& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::F32& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::F64& t){this->operator = (t);}
			GINL BasicCharsString(const GAIA::BL& t){this->operator = (t);}
			template<typename _ParamDataType1, typename _ParamSizeType1, typename _ParamDataType2, typename _ParamSizeType2> BasicCharsString(const _ParamDataType1* p1, const _ParamSizeType1& size1, const _ParamDataType2* p2, const _ParamSizeType2& size2)
			{
				if(size1 + size2 > m_chars.capacity())
					m_string.combine(p1, size1, p2, size2);
				else
					m_chars.combine(p1, size1, p2, size2);
			}
			template<typename _ParamSizeType, _ParamSizeType _ParamSize> BasicCharsString(const GAIA::CTN::BasicChars<_DataType, _ParamSizeType, _ParamSize>& src){this->operator = (src);}
			template<typename _ParamSizeType> BasicCharsString(const GAIA::CTN::BasicString<_DataType, _ParamSizeType>& src){this->operator = (src);}
			GINL ~BasicCharsString(){this->destroy();}
			GINL GAIA::GVOID proxy(const _DataType* p, const _SizeType& size, const _SizeType& capacity)
			{
				this->destroy();
				m_string.proxy(p, size, capacity);
			}
			GINL GAIA::GVOID proxy(const _DataType* p)
			{
				this->destroy();
				m_string.proxy(p);
			}
			GINL GAIA::BL empty() const{if(m_string.capacity() != 0) return m_string.empty(); return m_chars.empty();}
			GINL _SizeType size() const{if(m_string.capacity() != 0) return m_string.size(); return m_chars.size();}
			GINL _SizeType capacity() const{return GAIA::ALGO::gmax(m_string.capacity(), m_chars.capacity());}
			GINL _SizeType capacity_chars() const{return m_chars.capacity();}
			GINL _SizeType typesize() const{return sizeof(_DataType);}
			GINL _SizeType datasize() const{return this->typesize() * this->size();}
			GINL GAIA::GVOID resize(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size > m_chars.capacity())
				{
					m_chars.clear();
					m_string.resize(size);
				}
				else
				{
					m_string.clear();
					m_chars.resize(size);
				}
			}
			GINL GAIA::GVOID resize_keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size > m_chars.capacity())
				{
					if(m_string.capacity() == 0 && !m_chars.empty())
						m_string = m_chars.fptr();
					m_string.resize_keep(size);
				}
				else
				{
					if(m_string.capacity() != 0 && !m_string.empty())
						m_chars = m_string.fptr();
					m_chars.resize_keep(size);
				}
			}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size > m_chars.capacity())
				{
					m_chars.clear();
					m_string.reserve(size);
				}
				else
				{
					m_string.destroy();
					m_chars.clear();
				}
			}
			GINL GAIA::GVOID clear(){m_chars.clear(); m_string.clear();}
			GINL GAIA::GVOID destroy()
			{
				m_chars.clear();
				m_string.destroy();
			}
			GINL GAIA::U32 type() const{if(m_string.capacity() != 0) return m_string.type(); return m_chars.type();}
			template<typename _ParamDataType> __MyType& assign(const _ParamDataType* p, const _SizeType& size)
			{
				if(p == GNIL || size == 0)
				{
					this->clear();
					return *this;
				}
				this->clear();
				if(size > m_chars.capacity())
					m_string.assign(p, size);
				else
					m_chars.assign(p, size);
				return *this;
			}
			GINL _DataType* fptr(){if(m_string.capacity() != 0) return m_string.fptr(); return m_chars.fptr();}
			GINL _DataType* bptr(){if(m_string.capacity() != 0) return m_string.bptr(); return m_chars.bptr();}
			GINL const _DataType* fptr() const{if(m_string.capacity() != 0) return m_string.fptr(); return m_chars.fptr();}
			GINL const _DataType* bptr() const{if(m_string.capacity() != 0) return m_string.bptr(); return m_chars.bptr();}
			GINL _DataType& front(){if(m_string.capacity() != 0) return m_string.front(); return m_chars.front();}
			GINL const _DataType& front() const{if(m_string.capacity() != 0) return m_string.front(); return m_chars.front();}
			GINL _DataType& back(){if(m_string.capacity() != 0) return m_string.back(); return m_chars.back();}
			GINL const _DataType& back() const{if(m_string.capacity() != 0) return m_string.back(); return m_chars.back();}
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
			GINL GAIA::GVOID inverse(){if(m_string.capacity() != 0) m_string.inverse(); else m_chars.inverse();}
			template<typename _ParamDataType> GAIA::BL insert(const _ParamDataType& t, const _SizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.insert(t, index);
				else
				{
					if(m_chars.size() + 1 > m_chars.capacity())
					{
						if(!m_chars.empty())
							m_string = m_chars.fptr();

						if(m_string.insert(t, index))
						{
							m_chars.clear();
							return GAIA::True;
						}
						else
						{
							m_string.clear();
							return GAIA::False;
						}
					}
					else
						return m_chars.insert(t, index);
				}
			}
			template<typename _ParamDataType> GAIA::BL insert(const _ParamDataType* p, const _SizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.insert(p, index);
				else
				{
					if(GAIA::ALGO::gstrlen(p) + m_chars.size() > m_chars.capacity())
					{
						if(!m_chars.empty())
							m_string = m_chars.fptr();

						if(m_string.insert(p, index))
						{
							m_chars.clear();
							return GAIA::True;
						}
						else
						{
							m_string.clear();
							return GAIA::False;
						}
					}
					else
						return m_chars.insert(p, index);
				}
			}
			GINL GAIA::BL insert(const __MyType& src, const _SizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.insert(src.fptr(), index);
				else
				{
					if(src.size() + m_chars.size() > m_chars.capacity())
					{
						if(!m_chars.empty())
							m_string = m_chars.fptr();

						if(m_string.insert(src.fptr(), index))
						{
							m_chars.clear();
							return GAIA::True;
						}
						else
						{
							m_string.clear();
							return GAIA::False;
						}
					}
					else
						return m_chars.insert(src.fptr(), index);
				}
			}
			template<typename _ParamSizeType> GAIA::BL erase_index(const _ParamSizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.erase_index(index);
				return m_chars.erase_index(index);
			}
			template<typename _ParamSizeType1, typename _ParamSizeType2> GAIA::BL erase_index(const _ParamSizeType1& index_start, const _ParamSizeType2& index_end)
			{
				if(m_string.capacity() != 0)
					return m_string.erase_index(index_start, index_end);
				return m_chars.erase_index(index_start, index_end);
			}
			template<typename _ParamDataType> _SizeType erase_all(const _ParamDataType& t, const _SizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.erase_all(t, index);
				return m_chars.erase_all(t, index);
			}
			template<typename _ParamDataType> _SizeType erase_all(const _ParamDataType* p, const _SizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.erase_all(p, index);
				return m_chars.erase_all(p, index);
			}
			GINL _SizeType erase_all(const __MyType& src, const _SizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.erase_all(src.fptr(), index);
				return m_chars.erase_all(src.fptr(), index);
			}
			template<typename _ParamDataType> _SizeType find(const _ParamDataType& t, const _SizeType& index = 0) const
			{
				if(m_string.capacity() != 0)
					return m_string.find(t, index);
				return m_chars.find(t, index);
			}
			template<typename _ParamDataType> _SizeType find(const _ParamDataType* p, const _SizeType& index = 0) const
			{
				if(m_string.capacity() != 0)
					return m_string.find(p, index);
				return m_chars.find(p, index);
			}
			GINL _SizeType find(const __MyType& src, const _SizeType& index = 0) const
			{
				if(m_string.capacity() != 0)
					return m_string.find(src, index);
				return m_chars.find(src, index);
			}
			template<typename _ParamDataType> _SizeType rfind(const _ParamDataType& t, const _SizeType& index = 0) const
			{
				if(m_string.capacity() != 0)
					return m_string.rfind(t, index);
				return m_chars.rfind(t, index);
			}
			template<typename _ParamDataType> _SizeType rfind(const _ParamDataType* p, const _SizeType& index = 0) const
			{
				if(m_string.capacity() != 0)
					return m_string.rfind(p, index);
				return m_chars.rfind(p, index);
			}
			GINL _SizeType rfind(const __MyType& src, const _SizeType& index = 0) const
			{
				if(m_string.capacity() != 0)
					return m_string.rfind(src.fptr(), index);
				return m_chars.rfind(src.fptr(), index);
			}
			template<typename _ParamDataType> GAIA::BL contains(const _ParamDataType& t) const
			{
				return this->find(t, 0) != GINVALID;
			}
			template<typename _ParamDataType> GAIA::BL contains(const _ParamDataType* p) const
			{
				return this->find(p, 0) != GINVALID;
			}
			GINL GAIA::BL contains(const __MyType& src) const
			{
				return this->find(src.fptr(), 0) != GINVALID;
			}
			template<typename _ParamDataType> GAIA::BL startwith(const _ParamDataType& t) const
			{
				if(this->empty())
					return GAIA::False;
				if(this->front() != t)
					return GAIA::False;
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL startwith(const _ParamDataType* p) const
			{
				GAST(!!p);
				if(p == GNIL)
					return GAIA::True;
				if(this->empty())
				{
					if(GAIA::ALGO::gstremp(p))
						return GAIA::True;
					return GAIA::False;
				}
				return GAIA::ALGO::gstrstartwith(this->fptr(), p);
			}
			GINL GAIA::BL startwith(const __MyType& src) const
			{
				if(src.empty())
					return GAIA::True;
				if(this->empty())
					return GAIA::False;
				return GAIA::ALGO::gstrstartwith(this->fptr(), src.fptr());
			}
			template<typename _ParamDataType> GAIA::BL endwith(const _ParamDataType& t) const
			{
				if(this->empty())
					return GAIA::False;
				if(this->back() != t)
					return GAIA::False;
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL endwith(const _ParamDataType* p) const
			{
				GAST(!!p);
				if(p == GNIL)
					return GAIA::True;
				if(this->empty())
				{
					if(GAIA::ALGO::gstremp(p))
						return GAIA::True;
					return GAIA::False;
				}
				return GAIA::ALGO::gstrendwith(this->fptr(), p);
			}
			GINL GAIA::BL endwith(const __MyType& src) const
			{
				if(src.empty())
					return GAIA::True;
				if(this->empty())
					return GAIA::False;
				return GAIA::ALGO::gstrendwith(this->fptr(), src.fptr());
			}
			template<typename _ParamDataType> GAIA::BL startwithi(const _ParamDataType& t) const
			{
				if(this->empty())
					return GAIA::False;
				if(GAIA::ALGO::tolower(this->front()) != GAIA::ALGO::tolower(t))
					return GAIA::False;
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL startwithi(const _ParamDataType* p) const
			{
				GAST(!!p);
				if(p == GNIL)
					return GAIA::True;
				if(this->empty())
				{
					if(GAIA::ALGO::gstremp(p))
						return GAIA::True;
					return GAIA::False;
				}
				return GAIA::ALGO::gstrstartwithi(this->fptr(), p);
			}
			GINL GAIA::BL startwithi(const __MyType& src) const
			{
				if(src.empty())
					return GAIA::True;
				if(this->empty())
					return GAIA::False;
				return GAIA::ALGO::gstrstartwithi(this->fptr(), src.fptr());
			}
			template<typename _ParamDataType> GAIA::BL endwithi(const _ParamDataType& t) const
			{
				if(this->empty())
					return GAIA::False;
				if(GAIA::ALGO::tolower(this->back()) != GAIA::ALGO::tolower(t))
					return GAIA::False;
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL endwithi(const _ParamDataType* p) const
			{
				GAST(!!p);
				if(p == GNIL)
					return GAIA::True;
				if(this->empty())
				{
					if(GAIA::ALGO::gstremp(p))
						return GAIA::True;
					return GAIA::False;
				}
				return GAIA::ALGO::gstrendwithi(this->fptr(), p);
			}
			GINL GAIA::BL endwithi(const __MyType& src) const
			{
				if(src.empty())
					return GAIA::True;
				if(this->empty())
					return GAIA::False;
				return GAIA::ALGO::gstrendwithi(this->fptr(), src.fptr());
			}
			GINL _SizeType replace(const _DataType& src, const _DataType& dst)
			{
				if(m_string.capacity() != 0)
					return m_string.replace(src, dst);
				return m_chars.replace(src, dst);
			}
			template<typename _ParamDataType> _SizeType replace(const _ParamDataType* pSrc, const _ParamDataType* pDst)
			{
				if(m_string.capacity() != 0)
					return m_string.replace(pSrc, pDst);
				return m_chars.replace(pSrc, pDst);
			}
			GINL _SizeType replace(const __MyType& src, const __MyType& dst)
			{
				if(m_string.capacity() != 0)
					return m_string.replace(src.fptr(), dst.fptr());
				return m_chars.replace(src.fptr(), dst.fptr());
			}
			GINL __MyType& left(const _SizeType& index)
			{
				if(m_string.capacity() != 0)
					m_string.left(index);
				else
					m_chars.left(index);
				return *this;
			}
			GINL __MyType& right(const _SizeType& index)
			{
				if(m_string.capacity() != 0)
					m_string.right(index);
				else
					m_chars.right(index);
				return *this;
			}
			GINL __MyType& mid(const _SizeType& index_start, const _SizeType& index_end)
			{
				if(m_string.capacity() != 0)
					m_string.mid(index_start, index_end);
				else
					m_chars.mid(index_start, index_end);
				return *this;
			}
			template<typename _ParamDataType> _SizeType extract(_SizeType index_start, _SizeType index_end, _ParamDataType* pResult, const _SizeType& nResultSize) const
			{
				if(m_string.capacity() != 0)
					return m_string.extract(index_start, index_end, pResult, nResultSize);
				else
					return m_chars.extract(index_start, index_end, pResult, nResultSize);
			}
			GINL _SizeType extract(_SizeType index_start, _SizeType index_end, __MyType& strResult) const
			{
				if(index_start < GINVALID || index_start >= this->size())
					return GINVALID;
				if(index_end < GINVALID || index_end >= this->size())
					return GINVALID;
				if(index_start == GINVALID)
					index_start = 0;
				if(index_end == GINVALID)
					index_end = this->size() - 1;
				if(index_start > index_end)
					return GINVALID;
				if(this->empty())
					return 0;

				_SizeType require = index_end - index_start + 1;
				_SizeType old_length = strResult.size();
				strResult.resize_keep(strResult.size() + require);

				_SizeType ret;
				if(m_string.capacity() != 0)
					ret = m_string.extract(index_start, index_end, strResult.fptr() + old_length, require);
				else
					ret = m_chars.extract(index_start, index_end, strResult.fptr() + old_length, require);
				return ret;
			}
			template<typename _ParamSizeType> GAIA::BL trim_left_index(const _ParamSizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_left_index(index);
				return m_chars.trim_left_index(index);
			}
			template<typename _ParamDataType> GAIA::BL trim_left(const _ParamDataType* p)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_left(p);
				return m_chars.trim_left(p);
			}
			GINL GAIA::BL trim_left(const __MyType& src)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_left(src.fptr());
				return m_chars.trim_left(src.fptr());
			}
			template<typename _ParamDataType> GAIA::BL trim_lefti(const _ParamDataType* p)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_lefti(p);
				return m_chars.trim_lefti(p);
			}
			GINL GAIA::BL trim_lefti(const __MyType& src)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_lefti(src.fptr());
				return m_chars.trim_lefti(src.fptr());
			}
			template<typename _ParamSizeType> GAIA::BL trim_right_index(const _ParamSizeType& index)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_right_index(index);
				return m_chars.trim_right_index(index);
			}
			template<typename _ParamDataType> GAIA::BL trim_right(const _ParamDataType* p)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_right(p);
				return m_chars.trim_right(p);
			}
			GINL GAIA::BL trim_right(const __MyType& src)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_right(src.fptr());
				return m_chars.trim_right(src.fptr());
			}
			template<typename _ParamDataType> GAIA::BL trim_righti(const _ParamDataType* p)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_righti(p);
				return m_chars.trim_righti(p);
			}
			GINL GAIA::BL trim_righti(const __MyType& src)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_righti(src.fptr());
				return m_chars.trim_righti(src.fptr());
			}
			template<typename _ParamDataType> GAIA::BL trims_left(const _ParamDataType& t)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_left(t);
				return m_chars.trim_left(t);
			}
			template<typename _ParamDataType> GAIA::BL trims_lefti(const _ParamDataType& t)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_lefti(t);
				return m_chars.trim_lefti(t);
			}
			template<typename _ParamDataType> GAIA::BL trims_right(const _ParamDataType& t)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_right(t);
				return m_chars.trim_right(t);
			}
			template<typename _ParamDataType> GAIA::BL trims_righti(const _ParamDataType& t)
			{
				if(m_string.capacity() != 0)
					return m_string.trim_righti(t);
				return m_chars.trim_righti(t);
			}
			template<typename _ParamDataType> GAIA::BL trims(const _ParamDataType& t)
			{
				GAIA::BL bTrimLeft = this->trims_left(t);
				GAIA::BL bTrimRight = this->trims_right(t);
				return bTrimLeft || bTrimRight;
			}
			template<typename _ParamDataType> GAIA::BL trimsi(const _ParamDataType& t)
			{
				GAIA::BL bTrimLeft = this->trims_lefti(t);
				GAIA::BL bTrimRight = this->trims_righti(t);
				return bTrimLeft || bTrimRight;
			}
			GINL GAIA::GVOID tolower()
			{
				if(m_string.capacity() != 0)
					m_string.tolower();
				else
					m_chars.tolower();
			}
			GINL GAIA::GVOID toupper()
			{
				if(m_string.capacity() != 0)
					m_string.toupper();
				else
					m_chars.toupper();
			}
			GINL GAIA::BL isexistlower() const
			{
				if(m_string.capacity() != 0)
					return m_string.isexistlower();
				return m_chars.isexistlower();
			}
			GINL GAIA::BL isexistupper() const
			{
				if(m_string.capacity() != 0)
					return m_string.isexistupper();
				return m_chars.isexistupper();
			}
			GINL GAIA::BL isalllower() const
			{
				if(m_string.capacity() != 0)
					return m_string.isalllower();
				return m_chars.isalllower();
			}
			GINL GAIA::BL isallupper() const
			{
				if(m_string.capacity() != 0)
					return m_string.isallupper();
				return m_chars.isallupper();
			}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				if(this->size() < src.size() && this->empty())
					return -1;
				else if(this->size() > src.size() && src.empty())
					return +1;
				if(this->empty() && src.empty())
					return 0;
				return GAIA::ALGO::gstrcmp(this->fptr(), src.fptr());
			}
			template<typename _ParamDataType> GAIA::N32 compare(const _ParamDataType* p) const
			{
				GAIA::NUM l = GAIA::ALGO::gstrlennil(p);
				if(this->size() < l)
					return -1;
				else if(this->size() > l)
					return +1;
				else if(this->empty())
					return 0;
				return GAIA::ALGO::gstrcmp(this->fptr(), p);
			}
			GINL GAIA::N32 comparei(const __MyType& src) const
			{
				if(this->size() < src.size() && this->empty())
					return -1;
				else if(this->size() > src.size() && src.empty())
					return +1;
				if(this->empty() && src.empty())
					return 0;
				return GAIA::ALGO::gstricmp(this->fptr(), src.fptr());
			}
			template<typename _ParamDataType> GAIA::N32 comparei(const _ParamDataType* p) const
			{
				GAIA::NUM l = GAIA::ALGO::gstrlennil(p);
				if(this->size() < l)
					return -1;
				else if(this->size() > l)
					return +1;
				else if(this->empty())
					return 0;
				return GAIA::ALGO::gstricmp(this->fptr(), p);
			}
			GINL GAIA::BL equals(const __MyType& src) const
			{
				return this->compare(src) == 0;
			}
			template<typename _ParamDataType> GAIA::BL equals(const _ParamDataType* p) const
			{
				return this->compare(p) == 0;
			}
			GINL GAIA::BL equalsi(const __MyType& src) const
			{
				return this->comparei(src) == 0;
			}
			template<typename _ParamDataType> GAIA::BL equalsi(const _ParamDataType* p) const
			{
				return this->comparei(p) == 0;
			}
			GINL __MyType& operator = (const __MyType& src){GAST(&src != this); this->assign(src.fptr(), src.size()); return *this;}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p)
			{
				_SizeType len;
				if(p == GNIL)
					len = 0;
				else
					len = (_SizeType)GAIA::ALGO::gstrlen(p);
				this->assign(p, len);
				return *this;
			}
			GINL __MyType& operator = (const GAIA::NM& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::N8& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::N16& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::N32& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::N64& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::UM& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::U8& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::U16& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::U32& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::U64& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::int2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::X128& t)
			{
				this->resize(32);
				GAIA::ALGO::hex2str((const GAIA::U8*)&t, sizeof(t), this->fptr());
				return *this;
			}
			GINL __MyType& operator = (const GAIA::F32& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::real2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::F64& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::real2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::BL& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; sz[0] = t ? '1' : '0'; sz[1] = '\0'; return this->operator = (sz);}
			template<typename _ParamSizeType, _ParamSizeType _ParamSize> __MyType& operator = (const GAIA::CTN::BasicChars<_DataType, _ParamSizeType, _ParamSize>& src){this->operator = (src.fptr()); return *this;}
			template<typename _ParamSizeType> __MyType& operator = (const GAIA::CTN::BasicString<_DataType, _ParamSizeType>& src){this->operator = (src.fptr()); return *this;}
			GINL __MyType& operator += (const __MyType& src)
			{
				if(m_string.capacity() != 0)
					m_string += src.fptr();
				else
				{
					if(m_chars.size() + src.size() > m_chars.capacity())
					{
						m_string = m_chars.fptr();
						m_string += src.fptr();
						m_chars.clear();
					}
					else
						m_chars += src.fptr();
				}
				return *this;
			}
			template<typename _ParamDataType> __MyType& operator += (const _ParamDataType* p)
			{
				if(m_string.capacity() != 0)
					m_string += p;
				else
				{
					if(m_chars.size() + GAIA::ALGO::gstrlen(p) > m_chars.capacity())
					{
						m_string = m_chars.fptr();
						m_string += p;
						m_chars.clear();
					}
					else
						m_chars += p;
				}
				return *this;
			}
			GINL __MyType operator + (const __MyType& src) const{return __MyType(this->fptr(), this->size(), src.fptr(), src.size());}
			template<typename _ParamDataType> __MyType operator + (const _ParamDataType* p) const{return __MyType(this->fptr(), this->size(), p, GAIA::ALGO::gstrlen(p));}
			GINL __MyType& operator += (GAIA::N8 v)
			{
				_DataType temp[8];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (GAIA::U8 v)
			{
				_DataType temp[8];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (GAIA::N16 v)
			{
				_DataType temp[16];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (GAIA::U16 v)
			{
				_DataType temp[16];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (GAIA::N32 v)
			{
				_DataType temp[32];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (GAIA::U32 v)
			{
				_DataType temp[32];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (const GAIA::N64& v)
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (const GAIA::U64& v)
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (const GAIA::NM& v)
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (const GAIA::UM& v)
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (GAIA::F32 v)
			{
				_DataType temp[32];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (const GAIA::F64& v)
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (GAIA::WCH v)
			{
				_DataType temp[32];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (GAIA::BL v)
			{
				_DataType temp[8];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				(*this) += temp;
				return *this;
			}
			GINL __MyType& operator += (const GAIA::X128& v)
			{
				_DataType temp[33];
				v.tostring(temp);
				(*this) += temp;
				return *this;
			}
			GINL __MyType operator + (GAIA::N8 v) const
			{
				_DataType temp[8];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (GAIA::U8 v) const
			{
				_DataType temp[8];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (GAIA::N16 v) const
			{
				_DataType temp[16];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (GAIA::U16 v) const
			{
				_DataType temp[16];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (GAIA::N32 v) const
			{
				_DataType temp[32];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (GAIA::U32 v) const
			{
				_DataType temp[32];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (const GAIA::N64& v) const
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (const GAIA::U64& v) const
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (const GAIA::NM& v) const
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (const GAIA::UM& v) const
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (GAIA::F32 v) const
			{
				_DataType temp[32];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (const GAIA::F64& v) const
			{
				_DataType temp[64];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (GAIA::WCH v) const
			{
				_DataType temp[32];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (GAIA::BL v) const
			{
				_DataType temp[8];
				GAIA::ALGO::castv(v, temp, sizeofarray(temp));
				__MyType ret;
				ret.reserve(this->size() + GAIA::ALGO::gstrlen(temp));
				ret = *this;
				ret += v;
				return ret;
			}
			GINL __MyType operator + (const GAIA::X128& v) const
			{
				_DataType temp[33];
				v.tostring(temp);
				__MyType ret;
				ret.reserve(this->size() + 32);
				ret = *this;
				ret += v;
				return ret;
			}
			GCLASS_COMPARE_BYCOMPARE(__MyType)
			template<typename _ParamDataType> GAIA::BL operator == (_ParamDataType p) const{return this->compare(p) == 0;}
			template<typename _ParamDataType> GAIA::BL operator != (_ParamDataType p) const{return this->compare(p) != 0;}
			template<typename _ParamDataType> GAIA::BL operator >= (_ParamDataType p) const{return this->compare(p) >= 0;}
			template<typename _ParamDataType> GAIA::BL operator <= (_ParamDataType p) const{return this->compare(p) <= 0;}
			template<typename _ParamDataType> GAIA::BL operator > (_ParamDataType p) const{return this->compare(p) > 0;}
			template<typename _ParamDataType> GAIA::BL operator < (_ParamDataType p) const{return this->compare(p) < 0;}
			template<typename _ParamSizeType> const _DataType& operator[](const _ParamSizeType& index) const{GAST(index <= this->size()); return this->fptr()[index];}
			template<typename _ParamSizeType> _DataType& operator[](const _ParamSizeType& index){GAST(index <= this->size()); return this->fptr()[index];}
			GINL operator _DataType*(){return this->fptr();}
			GINL operator const _DataType*() const{return this->fptr();}
			GINL operator GAIA::NM() const{return GAIA::ALGO::casts<NM>(this->fptr());}
			GINL operator GAIA::N8() const{return GAIA::ALGO::casts<N8>(this->fptr());}
			GINL operator GAIA::N16() const{return GAIA::ALGO::casts<N16>(this->fptr());}
			GINL operator GAIA::N32() const{return GAIA::ALGO::casts<N32>(this->fptr());}
			GINL operator GAIA::N64() const{return GAIA::ALGO::casts<N64>(this->fptr());}
			GINL operator GAIA::UM() const{return GAIA::ALGO::casts<UM>(this->fptr());}
			GINL operator GAIA::U8() const{return GAIA::ALGO::casts<U8>(this->fptr());}
			GINL operator GAIA::U16() const{return GAIA::ALGO::casts<U16>(this->fptr());}
			GINL operator GAIA::U32() const{return GAIA::ALGO::casts<U32>(this->fptr());}
			GINL operator GAIA::U64() const{return GAIA::ALGO::casts<U64>(this->fptr());}
			GINL operator GAIA::X128() const
			{
				X128 ret;
				if(this->empty())
				{
					ret.clear();
					return ret;
				}
				GAIA::ALGO::str2hex(this->fptr(), sizeof(ret), (GAIA::U8*)&ret);
				return ret;
			}
			GINL operator GAIA::F32() const{return GAIA::ALGO::casts<F32>(this->fptr());}
			GINL operator GAIA::F64() const{return GAIA::ALGO::casts<F64>(this->fptr());}
			GINL operator GAIA::BL() const{return GAIA::ALGO::casts<BL>(this->fptr());}
			template<typename _ParamDataType> __MyType& operator << (_ParamDataType t)
			{
				(*this) += t;
				return *this;
			}
			GINL GAIA::CTN::BasicCharsString<GAIA::CH, _SizeType, _Size> toUtf8(GAIA::CHARSET_TYPE t = GAIA::CHARSET_TYPE_SYS) const
			{
				GAIA::CTN::BasicCharsString<GAIA::CH, _SizeType, _Size> ret;
				if(this->empty())
					return ret;
				if(sizeof(_DataType) == sizeof(GAIA::CH))
				{
					if(t == GAIA::CHARSET_TYPE_UTF8)
						return (const GAIA::CH*)this->fptr();
					else
					{
						GAIA::CTN::BasicCharsString<GAIA::WCH, _SizeType, _Size> wstr = this->toWString(t);
						return wstr.toUtf8();
					}
				}
				else if(sizeof(_DataType) == sizeof(GAIA::WCH))
				{
					ret.resize(this->size() * 6 + 1);
					GAIA::NUM sLen = GAIA::LOCALE::w2m((const GAIA::WCH*)this->fptr(), this->size(), ret.fptr(), ret.size(), GAIA::CHARSET_TYPE_UTF8);
					if(sLen == 0)
					{
						ret.clear();
						return ret;
					}
					ret.resize(sLen);
				}
				else
					GASTFALSE;
				return ret;
			}
			GINL GAIA::CTN::BasicCharsString<GAIA::WCH, _SizeType, _Size> toWString(GAIA::CHARSET_TYPE t = GAIA::CHARSET_TYPE_SYS) const
			{
				GAIA::CTN::BasicCharsString<GAIA::WCH, _SizeType, _Size> ret;
				if(this->empty())
					return ret;
				if(sizeof(_DataType) == sizeof(GAIA::WCH))
				{
					if(this->empty())
						return ret;
					return (const GAIA::WCH*)this->fptr();
				}
				else if(sizeof(_DataType) == sizeof(GAIA::CH)){}
				else
					GASTFALSE;
				ret.resize(this->size() + 1);
				GAIA::NUM sLen = GAIA::LOCALE::m2w(this->fptr(), this->size(), ret.fptr(), ret.size(), t);
				if(sLen == 0)
				{
					ret.clear();
					return ret;
				}
				ret.resize(sLen);
				return ret;
			}
			GINL GAIA::CTN::BasicCharsString<GAIA::CH, _SizeType, _Size> toSys(GAIA::CHARSET_TYPE t = GAIA::CHARSET_TYPE_UTF8) const
			{
				GAIA::CTN::BasicCharsString<GAIA::CH, _SizeType, _Size> ret;
				if(this->empty())
					return ret;
				if(sizeof(_DataType) == sizeof(GAIA::CH))
				{
					if(t == GAIA::CHARSET_TYPE_SYS)
						return (const GAIA::CH*)this->fptr();
					else
					{
						GAIA::CTN::BasicCharsString<GAIA::WCH, _SizeType, _Size> wstr = this->toWString(t);
						return wstr.toSys();
					}
				}
				else if(sizeof(_DataType) == sizeof(GAIA::WCH))
				{
					ret.resize(this->size() * 4 + 1);
					GAIA::NUM sLen = GAIA::LOCALE::w2m((const GAIA::WCH*)this->fptr(), this->size(), ret.fptr(), ret.size(), GAIA::CHARSET_TYPE_SYS);
					if(sLen == 0)
					{
						ret.clear();
						return ret;
					}
					ret.resize(sLen);
				}
				else
					GASTFALSE;
				return ret;
			}
		private:
			GAIA::CTN::BasicChars<_DataType, _SizeType, _Size> m_chars;
			GAIA::CTN::BasicString<_DataType, _SizeType> m_string;
		};
		template<typename _DataType1, typename _DataType2, typename _SizeType, _SizeType _Size> GAIA::CTN::BasicCharsString<_DataType2, _SizeType, _Size>
			operator + (_DataType1 p, const GAIA::CTN::BasicCharsString<_DataType2, _SizeType, _Size>& str)
		{
			GAIA::CTN::BasicCharsString<_DataType2, _SizeType, _Size> ret = p;
			ret += str;
			return ret;
		}
		typedef GAIA::CTN::BasicCharsString<GAIA::CH, GAIA::NUM, GAIA::MAXPL> ACharsString;
		typedef GAIA::CTN::BasicCharsString<GAIA::WCH, GAIA::NUM, GAIA::MAXPL> WCharsString;
		typedef GAIA::CTN::BasicCharsString<GAIA::TCH, GAIA::NUM, GAIA::MAXPL> TCharsString;
	}
}

#endif
