#ifndef		__GAIA_CTN_STRING_H__
#define		__GAIA_CTN_STRING_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_locale.h"
#include "gaia_iterator.h"
#include "gaia_algo_compare.h"
#include "gaia_algo_extend.h"
#include "gaia_algo_string.h"
#include "gaia_ctn.h"
#include "gaia_ctn_chars.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _DataType, typename _SizeType> class BasicString : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
		public:
			typedef BasicString<_DataType, _SizeType> __MyType;
		private:
			static const GAIA::U32 GAIA_DIGIT_TOSTRING_LEN = 64;
		public:
			class it : public GAIA::ITERATOR::Iterator<_DataType>
			{
			private:
				friend class BasicString;
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
				friend class BasicString;
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
			GINL BasicString(){this->init();}
			template<typename _ParamDataType> BasicString(const _ParamDataType* p){this->init(); this->operator = (p);}
			template<typename _ParamDataType> BasicString(const _ParamDataType* p, const GAIA::CH* pszLocale)
			{
				this->init();
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
			GINL BasicString(const __MyType& src){this->init(); this->operator = (src);}
			GINL BasicString(const GAIA::NM& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::N8& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::N16& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::N32& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::N64& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::UM& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::U8& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::U16& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::U32& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::U64& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::X128& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::F32& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::F64& t){this->init(); this->operator = (t);}
			GINL BasicString(const GAIA::BL& t){this->init(); this->operator = (t);}
			template<typename _ParamDataType1, typename _ParamSizeType1, typename _ParamDataType2, typename _ParamSizeType2> BasicString(const _ParamDataType1* p1, const _ParamSizeType1& size1, const _ParamDataType2* p2, const _ParamSizeType2& size2){this->init(); this->combine(p1, size1, p2, size2);}
			template<typename _ParamSizeType, _ParamSizeType _Size> BasicString(const GAIA::CTN::BasicChars<_DataType, _ParamSizeType, _Size>& src){this->operator = (src);}
			GINL ~BasicString(){this->destroy();}
			GAIA_CTN_PROXY
			GINL GAIA::GVOID proxy(const _DataType* p)
			{
				m_pFront = GCCAST(_DataType*)(p);
				if(m_pFront != GNIL)
					m_capacity = m_size = GAIA::ALGO::gstrlen(m_pFront) + 1;
				else
					m_capacity = m_size = 0;
			}
			GINL GAIA::BL empty() const{if(this->size() == 0) return GAIA::True; return m_pFront[0] == 0;}
			GINL GAIA::BL nill() const{return m_pFront == GNIL;}
			GINL _SizeType size() const{if(m_size == 0) return 0; return m_size - 1;}
			GINL _SizeType capacity() const{if(m_capacity == 0) return 0; return m_capacity - 1;}
			GINL _SizeType typesize() const{return sizeof(_DataType);}
			GINL _SizeType datasize() const{return this->typesize() * this->size();}
			GINL GAIA::GVOID resize(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size == 0)
					m_size = 0;
				else
				{
					if(size > this->capacity())
						this->reserve(size);
					m_size = size + 1;
				}
				if(m_pFront != GNIL)
					m_pFront[size] = '\0';
			}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				GAST(size >= 0);
				this->destroy();
				if(size > 0)
				{
					m_capacity = size + 1;
					m_size = 0;
					m_pFront = gnew _DataType[m_capacity];
					m_pFront[size] = '\0';
				}
			}
			GINL GAIA::GVOID resize_keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size <= this->capacity())
				{
					if(size == 0)
						m_size = 0;
					else
						m_size = size + 1;
					if(m_pFront != GNIL)
						m_pFront[size] = '\0';
				}
				else
				{
					if(this->size() == 0)
						this->resize(size);
					else
					{
						_DataType* pNew = gnew _DataType[size + 1];
						GAIA::ALGO::copy(pNew, m_pFront, this->size());
						gdel[] m_pFront;
						pNew[size] = '\0';
						m_pFront = pNew;
						m_capacity = m_size = size + 1;
					}
				}
			}
			GINL GAIA::GVOID reserve_keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(this->capacity() == size)
					return;
				if(size > 0)
				{
					_DataType* pNew = gnew _DataType[size + 1];
					_SizeType validsize = GAIA::ALGO::gmin(size, this->size());
					if(!this->empty())
						GAIA::ALGO::copy(pNew, m_pFront, validsize);
					pNew[validsize] = '\0';
					this->destroy();
					m_pFront = pNew;
					m_capacity = size + 1;
					m_size = validsize + 1;
				}
				else
					this->destroy();
			}
			GINL GAIA::GVOID clear(){m_size = 0; if(m_pFront != GNIL) m_pFront[0] = 0;}
			GINL GAIA::GVOID destroy()
			{
				m_size = m_capacity = 0;
				if(m_pFront != GNIL)
				{
					gdel[] m_pFront;
					m_pFront = GNIL;
				}
			}
			GINL GAIA::U32 type() const{return GAIA::ALGO::strtype(m_pFront);}
			template<typename _ParamDataType> __MyType& assign(const _ParamDataType* p, const _SizeType& size)
			{
				GAST(size >= 0);
				GAST((const GAIA::GVOID*)(p + size) <= (const GAIA::GVOID*)m_pFront || 
					(const GAIA::GVOID*)(m_pFront + this->capacity()) <= (const GAIA::GVOID*)p);
				if(p == GNIL || size == 0)
				{
					this->clear();
					return *this;
				}
				this->clear();
				this->resize(size);
				GAIA::ALGO::gstrcpy(m_pFront, p, size);
				return *this;
			}
			GINL _DataType* fptr()
			{
				static _DataType t = '\0';
				if(m_pFront == GNIL)
					return &t;
				return m_pFront;
			}
			GINL _DataType* bptr(){if(this->empty()) return GNIL; return m_pFront + this->size() - 1;}
			GINL const _DataType* fptr() const
			{
				static const _DataType t = '\0';
				if(m_pFront == GNIL)
					return &t;
				return m_pFront;
			}
			GINL const _DataType* bptr() const{if(this->empty()) return GNIL; return m_pFront + this->size() - 1;}
			GINL _DataType& front(){return *m_pFront;}
			GINL const _DataType& front() const{return *m_pFront;}
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
			GINL GAIA::GVOID inverse(){if(this->size() > 1) GAIA::ALGO::inverse(m_pFront, m_pFront + this->size() - 1);}
			template<typename _ParamDataType> GAIA::BL insert(const _ParamDataType& t, const _SizeType& index)
			{
				if(index > this->size())
					return GAIA::False;
				if(this->size() == this->capacity())
					this->exten(1);
				this->resize(this->size() + 1);
				if(this->size() - 1 - index != 0)
				{
					GAIA::ALGO::move_right(
						m_pFront + this->size() - 1,
 						this->size() - 1 - index);
				}
				this->operator[](index) = t;
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL insert(const _ParamDataType* p, const _SizeType& index)
			{
				if(index > this->size())
					return GAIA::False;
				GAST(!!p);
				if(p == GNIL)
					return GAIA::False;
				if(*p == 0)
					return GAIA::True;
				_SizeType newsize = GAIA::ALGO::gstrlen(p);
				if(this->size() + newsize > this->capacity())
					this->exten(this->size() + newsize - this->capacity());
				this->resize(this->size() + newsize);
				if(this->size() - newsize - index != 0)
				{
					GAIA::ALGO::move_right(
						m_pFront + this->size() - 1,
						m_pFront + this->size() - newsize - 1,
						this->size() - newsize - index);
				}
				GAIA::ALGO::copy(m_pFront + index, p, newsize);
				return GAIA::True;
			}
			GINL GAIA::BL insert(const __MyType& src, const _SizeType& index)
			{
				if(index > this->size())
					return GAIA::False;
				if(src.empty())
					return GAIA::True;
				if(this->size() + src.size() > this->capacity())
					this->exten(this->size() + src.size() - this->capacity());
				this->resize(this->size() + src.size());
				if(this->size() - src.size() - index != 0)
				{
					GAIA::ALGO::move_right(
						m_pFront + this->size() - 1,
						m_pFront + this->size() - src.size() - 1,
						this->size() - src.size() - index);
				}
				GAIA::ALGO::copy(m_pFront + index, src.m_pFront, src.size());
				return GAIA::True;
			}
			template<typename _ParamSizeType> GAIA::BL erase_index(const _ParamSizeType& index)
			{
				if(index >= 0 && index < this->size())
				{
					if(this->size() - index - 1 != 0)
						GAIA::ALGO::move_left(m_pFront + index, this->size() - index - 1);
					this->resize(this->size() - 1);
				}
				return GAIA::False;
			}
			template<typename _ParamSizeType1, typename _ParamSizeType2> GAIA::BL erase_index(const _ParamSizeType1& index_start, const _ParamSizeType2& index_end)
			{
				GAST(index_start <= index_end);
				if(index_start >= 0 && index_start < this->size() &&
					index_end >= index_start && index_end < this->size())
				{
					if(this->size() - index_start - 1 != 0)
					{
						GAIA::ALGO::move_left(
							m_pFront + index_start,
							m_pFront + index_end + 1,
							this->size() - index_end - 1);
					}
					this->resize(this->size() - (index_end - index_start) - 1);
					return GAIA::True;
				}
				return GAIA::False;
			}
			template<typename _ParamDataType> _SizeType erase_all(const _ParamDataType& t, const _SizeType& index)
			{
				GAST(index >= 0 && index < this->size());
				_SizeType ret = 0;
				if(index >= 0 && index < this->size())
				{
					_SizeType tindex = index;
					while((tindex = this->find(t, tindex)) != GINVALID)
					{
						this->erase_index(tindex);
						++ret;
						++tindex;
					}
				}
				return ret;
			}
			template<typename _ParamDataType> _SizeType erase_all(const _ParamDataType* p, const _SizeType& index)
			{
				GAST(!!p);
				if(!p) return 0;
				GAST(index >= 0 && index < this->size());
				_SizeType ret = 0;
				if(index >= 0 && index < this->size())
				{
					_SizeType tindex = index;
					_SizeType len = GAIA::ALGO::gstrlen(p);
					while((tindex = this->find(p, tindex)) != GINVALID)
					{
						this->erase_index(tindex, tindex + len - 1);
						++ret;
						tindex += len;
					}
				}
				return ret;
			}
			GINL _SizeType erase_all(const __MyType& src, const _SizeType& index)
			{
				if(src.empty())
					return 0;
				GAST(index >= 0 && index < this->size());
				_SizeType ret = 0;
				if(index >= 0 && index < this->size())
				{
					_SizeType tindex = index;
					while((tindex = this->find(src, tindex)) != GINVALID)
					{
						this->erase_index(tindex, tindex + src.size() - 1);
						++ret;
						tindex += src.size();
					}
				}
				return ret;
			}
			template<typename _ParamDataType> _SizeType find(const _ParamDataType& t, const _SizeType& index = 0) const
			{
				if(this->empty())
					return (_SizeType)GINVALID;
				if(index >= this->size())
					return (_SizeType)GINVALID;
				const _DataType* pFinded = GAIA::ALGO::find((const _DataType*)m_pFront + index, this->bptr() + 1, t);
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_pFront);
			}
			template<typename _ParamDataType> _SizeType find(const _ParamDataType* p, const _SizeType& index = 0) const
			{
				GAST(!!p);
				if(p == GNIL)
					return (_SizeType)GINVALID;
				if(this->empty())
					return (_SizeType)GINVALID;
				if(index >= this->size())
					return (_SizeType)GINVALID;
				const _DataType* pFinded = GAIA::ALGO::gstrstr(m_pFront + index, p);
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_pFront);
			}
			GINL _SizeType find(const __MyType& src, const _SizeType& index = 0) const
			{
				GAST(!src.empty());
				if(src.empty())
					return (_SizeType)GINVALID;
				if(this->empty())
					return (_SizeType)GINVALID;
				if(index >= this->size())
					return (_SizeType)GINVALID;
				const _DataType* pFinded = GAIA::ALGO::gstrstr(m_pFront + index, src.m_pFront);
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_pFront);
			}
			template<typename _ParamDataType> _SizeType rfind(const _ParamDataType& t, const _SizeType& index = 0) const
			{
				if(this->empty())
					return (_SizeType)GINVALID;
				if(index >= this->size())
					return (_SizeType)GINVALID;
				const _DataType* pFinded = GAIA::ALGO::rfind(m_pFront, m_pFront + index, t);
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_pFront);
			}
			template<typename _ParamDataType> _SizeType rfind(const _ParamDataType* p, const _SizeType& index = 0) const
			{
				GAST(!!p);
				if(p == GNIL)
					return (_SizeType)GINVALID;
				if(this->empty())
					return (_SizeType)GINVALID;
				if(index >= this->size())
					return (_SizeType)GINVALID;
				const _DataType* pFinded = GAIA::ALGO::rfinds(m_pFront, m_pFront + index, p, GAIA::ALGO::gstrlen(p));
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_pFront);
			}
			GINL _SizeType rfind(const __MyType& src, const _SizeType& index = 0) const
			{
				GAST(!src.empty());
				if(src.empty())
					return (_SizeType)GINVALID;
				if(this->empty())
					return (_SizeType)GINVALID;
				if(index >= this->size())
					return (_SizeType)GINVALID;
				const _DataType* pFinded = GAIA::ALGO::rfinds(m_pFront, m_pFront + index, src.m_pFront, src.size());
				if(pFinded == GNIL)
					return (_SizeType)GINVALID;
				return (_SizeType)(pFinded - m_pFront);
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
				return this->find(src, 0) != GINVALID;
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
				if(this->empty())
					return 0;
				return GAIA::ALGO::replace(m_pFront, this->size(), src, dst);
			}
			template<typename _ParamDataType> _SizeType replace(const _ParamDataType* pSrc, const _ParamDataType* pDst)
			{
				GAST(!!pSrc);
				GAST(!!pDst);
				if(this->empty())
					return 0;
				if(GAIA::ALGO::gstremp(pSrc))
					return 0;
				_SizeType srclen = GAIA::ALGO::gstrlen(pSrc);
				if(srclen == 0)
					return 0;
				_SizeType dstlen = GAIA::ALGO::gstrlen(pDst);
				_SizeType ret = 0;
				_SizeType tindex = 0;
				while((tindex = this->find(pSrc, tindex)) != GINVALID)
				{
					this->erase_index(tindex, tindex + srclen - 1);
					if(dstlen != 0)
						this->insert(pDst, tindex);
					tindex += dstlen + 1;
					++ret;
				}
				return ret;
			}
			GINL _SizeType replace(const __MyType& src, const __MyType& dst)
			{
				if(this->empty())
					return 0;
				if(src.size() == 0)
					return 0;
				_SizeType ret = 0;
				_SizeType tindex = 0;
				while((tindex = this->find(src, tindex)) != GINVALID)
				{
					this->erase_index(tindex, tindex + src.size() - 1);
					if(dst.size() != 0)
						this->insert(dst, tindex);
					tindex += dst.size() + 1;
					++ret;
				}
				return ret;
			}
			GINL __MyType& left(const _SizeType& index)
			{
				if(index >= this->size())
					return *this;
				this->operator[](index) = '\0';
				this->resize(index);
				return *this;
			}
			GINL __MyType& right(const _SizeType& index)
			{
				if(index >= this->size())
					return *this;
				GAIA::ALGO::move_left(m_pFront, m_pFront + index + 1, this->size() - index);
				this->resize(this->size() - index - 1);
				return *this;
			}
			GINL __MyType& mid(const _SizeType& index_start, const _SizeType& index_end)
			{
				GAST(index_start <= index_end);
				if(index_start > index_end)
					return *this;
				if(index_start >= this->size())
					return *this;
				if(index_end >= this->size())
					return *this;
				_SizeType tempsize = index_end - index_start + 1;
				GAIA::ALGO::move_left(m_pFront, m_pFront + index_start, tempsize);
				this->operator[](tempsize) = '\0';
				this->resize(tempsize);
				return *this;
			}
			template<typename _ParamDataType> _SizeType extract(_SizeType index_start, _SizeType index_end, _ParamDataType* pResult, const _SizeType& nResultSize) const
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
				if(pResult == GNIL)
					return GINVALID;
				if(this->empty())
					return 0;
				_SizeType nResult = GAIA::ALGO::gmin(index_end - index_start + 1, nResultSize);
				GAIA::ALGO::gstrcpy(pResult, m_pFront + index_start, nResult);
				return nResult;
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
				strResult.resize_keep(old_length + require);
				return this->extract(index_start, index_end, strResult.m_pFront + old_length, require);
			}
			template<typename _ParamSizeType> GAIA::BL trim_left_index(const _ParamSizeType& index)
			{
				if(this->empty())
					return GAIA::False;
				if(index >= 0 && index <= this->size())
				{
					if(index != 0)
						this->right(index - 1);
					return GAIA::True;
				}
				return GAIA::False;
			}
			template<typename _ParamDataType> GAIA::BL trim_left(const _ParamDataType* p)
			{
				GAST(!!p);
				if(this->empty())
					return GAIA::False;
				_DataType* pTemp = m_pFront;
				while(*pTemp != 0 && *p != 0)
				{
					if(*pTemp != *p)
						break;
					++pTemp;
					++p;
				}
				if(pTemp == m_pFront)
					return GAIA::False;
				_SizeType newsize = this->size() - (_SizeType)(pTemp - m_pFront);
				GAIA::ALGO::move_left(m_pFront, pTemp, newsize + 1);
				this->resize(newsize);
				return GAIA::True;
			}
			GINL GAIA::BL trim_left(const __MyType& src)
			{
				if(src.empty())
					return GAIA::False;
				return this->trim_left(src.m_pFront);
			}
			template<typename _ParamDataType> GAIA::BL trim_lefti(const _ParamDataType* p)
			{
				GAST(!!p);
				if(this->empty())
					return GAIA::False;
				_DataType* pTemp = m_pFront;
				while(*pTemp != 0 && *p != 0)
				{
					if(GAIA::ALGO::tolower(*pTemp) != GAIA::ALGO::tolower(*p))
						break;
					++pTemp;
					++p;
				}
				if(pTemp == m_pFront)
					return GAIA::False;
				_SizeType newsize = this->size() - (_SizeType)(pTemp - m_pFront);
				GAIA::ALGO::move_left(m_pFront, pTemp, newsize + 1);
				this->resize(newsize);
				return GAIA::True;
			}
			GINL GAIA::BL trim_lefti(const __MyType& src)
			{
				if(src.empty())
					return GAIA::False;
				return this->trim_lefti(src.m_pFront);
			}
			template<typename _ParamSizeType> GAIA::BL trim_right_index(const _ParamSizeType& index)
			{
				if(this->empty())
					return GAIA::False;
				if(index >= 0 && index < this->size())
				{
					this->left(index + 1);
					return GAIA::True;
				}
				return GAIA::False;
			}
			template<typename _ParamDataType> GAIA::BL trim_right(const _ParamDataType* p)
			{
				GAST(!!p);
				if(this->empty())
					return GAIA::False;
				_SizeType len = GAIA::ALGO::gstrlen(p);
				if(len > this->size())
					return GAIA::False;
				_DataType* pTemp = m_pFront + this->size() - len;
				if(GAIA::ALGO::gstrcmp(pTemp, p) == 0)
				{
					*pTemp = '\0';
					this->resize((_SizeType)(pTemp - m_pFront));
					return GAIA::True;
				}
				return GAIA::True;
			}
			GINL GAIA::BL trim_right(const __MyType& src)
			{
				if(src.empty())
					return GAIA::False;
				if(src.size() > this->size())
					return GAIA::False;
				_DataType* pTemp = m_pFront + this->size() - src.size();
				if(GAIA::ALGO::gstrcmp(pTemp, src.m_pFront) == 0)
				{
					*pTemp = '\0';
					this->resize((_SizeType)(pTemp - m_pFront));
					return GAIA::True;
				}
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL trim_righti(const _ParamDataType* p)
			{
				GAST(!!p);
				if(this->empty())
					return GAIA::False;
				_SizeType len = GAIA::ALGO::gstrlen(p);
				if(len > this->size())
					return GAIA::False;
				_DataType* pTemp = m_pFront + this->size() - len;
				if(GAIA::ALGO::gstricmp(pTemp, p) == 0)
				{
					*pTemp = '\0';
					this->resize((_SizeType)(pTemp - m_pFront));
					return GAIA::True;
				}
				return GAIA::True;
			}
			GINL GAIA::BL trim_righti(const __MyType& src)
			{
				if(src.empty())
					return GAIA::False;
				if(src.size() > this->size())
					return GAIA::False;
				_DataType* pTemp = m_pFront + this->size() - src.size();
				if(GAIA::ALGO::gstricmp(pTemp, src.m_pFront) == 0)
				{
					*pTemp = '\0';
					this->resize((_SizeType)(pTemp - m_pFront));
					return GAIA::True;
				}
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL trims_left(const _ParamDataType& t)
			{
				if(this->empty())
					return GAIA::False;
				_DataType* pTemp = m_pFront;
				while(*pTemp != 0)
				{
					if(*pTemp != t)
						break;
					++pTemp;
				}
				if(pTemp == m_pFront)
					return GAIA::False;
				_SizeType newsize = this->size() - (_SizeType)(pTemp - m_pFront);
				GAIA::ALGO::move_left(m_pFront, pTemp, newsize + 1);
				this->resize(newsize);
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL trims_lefti(const _ParamDataType& t)
			{
				if(this->empty())
					return GAIA::False;
				_DataType* pTemp = m_pFront;
				while(*pTemp != 0)
				{
					if(GAIA::ALGO::tolower(*pTemp) != GAIA::ALGO::tolower(t))
						break;
					++pTemp;
				}
				if(pTemp == m_pFront)
					return GAIA::False;
				_SizeType newsize = this->size() - (_SizeType)(pTemp - m_pFront);
				GAIA::ALGO::move_left(m_pFront, pTemp, newsize + 1);
				this->resize(newsize);
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL trims_right(const _ParamDataType& t)
			{
				if(this->empty())
					return GAIA::False;
				it it = this->backit();
				for(; !it.empty(); --it)
				{
					if(*it != t)
						break;
					*it = '\0';
				}
				this->resize(this->size() - (this->backit() - it));
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL trims_righti(const _ParamDataType& t)
			{
				if(this->empty())
					return GAIA::False;
				it it = this->backit();
				for(; !it.empty(); --it)
				{
					if(GAIA::ALGO::tolower(*it) != GAIA::ALGO::tolower(t))
						break;
					*it = '\0';
				}
				this->resize(this->size() - (this->backit() - it));
				return GAIA::True;
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
			GINL GAIA::GVOID tolower(){if(m_pFront != GNIL) GAIA::ALGO::tolowers(m_pFront);}
			GINL GAIA::GVOID toupper(){if(m_pFront != GNIL) GAIA::ALGO::touppers(m_pFront);}
			GINL GAIA::BL isexistlower() const{if(m_pFront != GNIL) return GAIA::ALGO::isexistlower(m_pFront); return GAIA::False;}
			GINL GAIA::BL isexistupper() const{if(m_pFront != GNIL) return GAIA::ALGO::isexistupper(m_pFront); return GAIA::False;}
			GINL GAIA::BL isalllower() const{if(m_pFront != GNIL) return GAIA::ALGO::isalllower(m_pFront); return GAIA::False;}
			GINL GAIA::BL isallupper() const{if(m_pFront != GNIL) return GAIA::ALGO::isallupper(m_pFront); return GAIA::False;}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				if(this->size() < src.size() && this->empty())
					return -1;
				else if(this->size() > src.size() && src.empty())
					return +1;
				if(this->empty() && src.empty())
					return 0;
				return GAIA::ALGO::gstrcmp(m_pFront, src.m_pFront);
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
				return GAIA::ALGO::gstrcmp(m_pFront, p);
			}
			GINL GAIA::N32 comparei(const __MyType& src) const
			{
				if(this->size() < src.size() && this->empty())
					return -1;
				else if(this->size() > src.size() && src.empty())
					return +1;
				if(this->empty() && src.empty())
					return 0;
				return GAIA::ALGO::gstricmp(m_pFront, src.m_pFront);
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
				return GAIA::ALGO::gstricmp(m_pFront, p);
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
			GINL __MyType& operator = (const __MyType& src){GAST(&src != this); this->assign(src.m_pFront, src.size()); return *this;}
			template<typename _ParamDataType> __MyType& operator = (const _ParamDataType* p)
			{
				GAST((GAIA::U64)p != (GAIA::U64)m_pFront || p == GNIL);
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
				GAIA::ALGO::hex2str((const GAIA::U8*)&t, sizeof(t), m_pFront);
				return *this;
			}
			GINL __MyType& operator = (const GAIA::F32& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::real2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::F64& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; GAIA::ALGO::real2str(t, sz); return this->operator = (sz);}
			GINL __MyType& operator = (const GAIA::BL& t){_DataType sz[GAIA_DIGIT_TOSTRING_LEN]; sz[0] = t ? '1' : '0'; sz[1] = '\0'; return this->operator = (sz);}
			template<typename _ParamSizeType, _ParamSizeType _Size> __MyType& operator = (const GAIA::CTN::BasicChars<_DataType, _ParamSizeType, _Size>& src);
			GINL __MyType& operator += (const __MyType& src){return this->combine(src.m_pFront, src.size());}
			template<typename _ParamDataType> __MyType& operator += (const _ParamDataType* p){return this->combine(p, GAIA::ALGO::gstrlen(p));}
			GINL __MyType operator + (const __MyType& src) const{return __MyType(m_pFront, this->size(), src.m_pFront, src.size());}
			template<typename _ParamDataType> __MyType operator + (const _ParamDataType* p) const{return __MyType(m_pFront, this->size(), p, GAIA::ALGO::gstrlen(p));}
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
			template<typename _ParamDataType> __MyType& operator << (_ParamDataType t)
			{
				(*this) += t;
				return *this;
			}
			template<typename _ParamDataType, typename _ParamSizeType> __MyType& combine(const _ParamDataType* p, const _ParamSizeType& size)
			{
				if(p == GNIL)
					return *this;
				if(*p == 0)
					return *this;
				if(size == 0)
					return *this;
				if(this->size() + size > this->capacity())
					this->exten(this->size() + size - this->capacity());
				GAIA::ALGO::gstrcpy(m_pFront + this->size(), p);
				this->resize(this->size() + size);
				return *this;
			}
			template<typename _ParamDataType1, typename _ParamSizeType1, typename _ParamDataType2, typename _ParamSizeType2> __MyType& combine(const _ParamDataType1* p1, const _ParamSizeType1& size1, const _ParamDataType2* p2, const _ParamSizeType2& size2)
			{
				GAIA::BL bEmpty1 = p1 == GNIL || *p1 == 0 || size1 == 0;
				GAIA::BL bEmpty2 = p2 == GNIL || *p2 == 0 || size2 == 0;
				if(bEmpty1 && bEmpty2)
					return *this;
				else if(!bEmpty1 && bEmpty2)
				{
					this->operator = (p1);
					return *this;
				}
				else if(bEmpty1 && !bEmpty2)
				{
					this->operator = (p2);
					return *this;
				}
				else
				{
					this->reserve(size1 + size2);
					this->operator = (p1);
					GAIA::ALGO::gstrcpy(m_pFront + this->size(), p2);
					this->resize(size1 + size2);
					return *this;
				}
			}
			GINL GAIA::CTN::BasicString<GAIA::CH, _SizeType> toUtf8(GAIA::CHARSET_TYPE t = GAIA::CHARSET_TYPE_SYS) const
			{
				GAIA::CTN::BasicString<GAIA::CH, _SizeType> ret;
				if(this->empty())
					return ret;
				if(sizeof(_DataType) == sizeof(GAIA::CH))
				{
					if(t == GAIA::CHARSET_TYPE_UTF8)
						return (const GAIA::CH*)this->fptr();
					else
					{
						GAIA::CTN::BasicString<GAIA::WCH, _SizeType> wstr = this->toWString(t);
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
			GINL GAIA::CTN::BasicString<GAIA::WCH, _SizeType> toWString(GAIA::CHARSET_TYPE t = GAIA::CHARSET_TYPE_SYS) const
			{
				GAIA::CTN::BasicString<GAIA::WCH, _SizeType> ret;
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
			GINL GAIA::CTN::BasicString<GAIA::CH, _SizeType> toSys(GAIA::CHARSET_TYPE t = GAIA::CHARSET_TYPE_UTF8) const
			{
				GAIA::CTN::BasicString<GAIA::CH, _SizeType> ret;
				if(this->empty())
					return ret;
				if(sizeof(_DataType) == sizeof(GAIA::CH))
				{
					if(t == GAIA::CHARSET_TYPE_SYS)
						return (const GAIA::CH*)this->fptr();
					else
					{
						GAIA::CTN::BasicString<GAIA::WCH, _SizeType> wstr = this->toWString(t);
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
			GINL GAIA::GVOID init(){m_pFront = GNIL; m_size = m_capacity = 0;}
			GINL GAIA::GVOID exten(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size == 0)
					return;
				_DataType* pNew = gnew _DataType[this->capacity() + size + 1];
				if(!this->empty())
					GAIA::ALGO::gstrcpy(pNew, m_pFront);
				m_capacity = this->capacity() + size + 1;
				if(m_pFront != GNIL)
					gdel[] m_pFront;
				m_pFront = pNew;
			}
		private:
			_DataType* m_pFront;
			_SizeType m_size;
			_SizeType m_capacity;
		};
		template<typename _DataType1, typename _DataType2, typename _SizeType> GAIA::CTN::BasicString<_DataType2, _SizeType>
			operator + (_DataType1 p, const GAIA::CTN::BasicString<_DataType2, _SizeType>& str)
		{
			GAIA::CTN::BasicString<_DataType2, _SizeType> ret = p;
			ret += str;
			return ret;
		}
		typedef GAIA::CTN::BasicString<GAIA::CH, GAIA::NUM> AString;
		typedef GAIA::CTN::BasicString<GAIA::WCH, GAIA::N32> WString;
		typedef GAIA::CTN::BasicString<GAIA::TCH, GAIA::NUM> TString;
	}
}

#endif
