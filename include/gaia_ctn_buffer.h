#ifndef		__GAIA_CTN_BUFFER_H__
#define		__GAIA_CTN_BUFFER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"
#include "gaia_algo_extend.h"
#include "gaia_algo_string.h"
#include "gaia_algo_search.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _SizeType, typename _ExtendType> class BasicBuffer : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
			typedef _ExtendType _extendtype;
		public:
			typedef BasicBuffer<_SizeType, _ExtendType> __MyType;
		public:
			GINL BasicBuffer(){this->init();}
			GINL BasicBuffer(const __MyType& src){this->init(); this->operator = (src);}
			GINL ~BasicBuffer(){if(m_pFront != GNIL) gdel[] m_pFront;}
			GINL GAIA::GVOID proxy(GAIA::U8* pFront, GAIA::U8* pBack)
			{
				m_pFront = pFront;
				m_pBack = pBack;
				m_pWrite = pFront;
				m_pRead = pFront;
			}
			GINL GAIA::GVOID assign(const GAIA::GVOID* p, const _SizeType& size)
			{
				GAST(p != GNIL);
				GAST(size > 0);
				this->resize(size);
				GAIA::ALGO::gmemcpy(m_pFront, p, size);
			}
			GINL GAIA::BL empty() const{return this->write_size() == 0;}
			GINL GAIA::BL nill() const{return m_pFront == GNIL;}
			GINL _SizeType capacity() const{return GSCAST(_SizeType)(m_pBack - m_pFront);}
			GINL GAIA::GVOID clear(){m_pWrite = m_pRead = m_pFront;}
			GINL GAIA::GVOID destroy(){if(m_pFront != GNIL){gdel[] m_pFront; this->init();}}
			GINL GAIA::GVOID reserve(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size == this->capacity())
				{
					m_pWrite = m_pRead = m_pFront;
					return;
				}
				this->destroy();
				if(size > 0)
				{
					this->alloc(size);
					m_pWrite = m_pRead = m_pFront;
				}
			}
			GINL GAIA::GVOID reserve_keep(const _SizeType& size)
			{
				if(size == this->capacity())
					return;
				if(size > 0)
				{
					GAIA::U8* pNew = gnew GAIA::U8[size];
					_SizeType validsize = GAIA::ALGO::gmin(size, this->write_size());
					if(validsize != 0)
						GAIA::ALGO::gmemcpy(pNew, m_pFront, validsize);
					_SizeType readsize = GAIA::ALGO::gmin(this->read_size(), validsize);
					this->destroy();
					m_pFront = pNew;
					m_pBack = m_pFront + size;
					m_pRead = m_pFront + readsize;
					m_pWrite = m_pFront + validsize;
				}
				else
					this->destroy();
			}
			GINL GAIA::GVOID resize(const _SizeType& size)
			{
				GAST(size >= 0);
				if(this->capacity() < size)
					this->reserve(size);
				m_pWrite = m_pRead = m_pFront;
				m_pWrite += size;
			}
			GINL GAIA::GVOID resize_keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size <= this->capacity())
				{
					m_pWrite = m_pRead = m_pFront;
					m_pWrite += size;
				}
				else
				{
					if(this->write_size() == 0)
						this->resize(size);
					else
					{
						GAIA::U8* pNew = gnew GAIA::U8[size];
						GAIA::ALGO::gmemcpy(pNew, this->fptr(), this->write_size());
						gdel m_pFront;
						m_pFront = pNew;
						m_pBack = m_pWrite = m_pFront + size;
						m_pRead = m_pFront;
					}
				}
			}
			GINL const GAIA::U8* fptr() const{return m_pFront;}
			GINL GAIA::U8* fptr(){return m_pFront;}
			GINL const GAIA::U8* bptr() const{return m_pBack;}
			GINL GAIA::U8* bptr(){return m_pBack;}
			GINL const GAIA::U8* write_ptr() const{return m_pWrite;}
			GINL GAIA::U8* write_ptr(){return m_pWrite;}
			GINL const GAIA::U8* read_ptr() const{return m_pRead;}
			GINL GAIA::U8* read_ptr(){return m_pRead;}
			GINL GAIA::BL seek_write(const _SizeType& size, SEEK_TYPE seektype = GAIA::SEEK_TYPE_BEGIN){return this->seek_ptr(size, seektype, m_pWrite);}
			GINL GAIA::BL seek_read(const _SizeType& size, SEEK_TYPE seektype = GAIA::SEEK_TYPE_BEGIN){return this->seek_ptr(size, seektype, m_pRead);}
			GINL _SizeType write_size() const{return GSCAST(_SizeType)(m_pWrite - m_pFront);}
			GINL _SizeType read_size() const{return GSCAST(_SizeType)(m_pRead - m_pFront);}
			GINL _SizeType remain() const{return GSCAST(_SizeType)(m_pWrite - m_pRead);}
			GINL GAIA::BL keep(const _SizeType& size)
			{
				GAST(size >= 0);
				if(size < 0)
					return GAIA::False;
				GAST(size <= this->write_size());
				if(size > this->write_size())
					return GAIA::False;
				if(size != 0 && size != this->write_size())
				{
					GAIA::U8* pOffset = m_pWrite - size;
					GAIA::ALGO::gmemcpy(m_pFront, pOffset, size);
				}
				m_pRead = m_pFront;
				m_pWrite = m_pFront + size;
				return GAIA::True;
			}
			GINL GAIA::GVOID keep_remain(){this->keep(this->remain());}
			GINL GAIA::U8* alloc(const _SizeType& size)
			{
				if(size == 0)
					return GNIL;
				GAST(m_pBack >= m_pWrite);
				if(this->capacity() - this->write_size() >= size)
				{
					GAIA::U8* pRet = m_pWrite;
					m_pWrite += size;
					return pRet;
				}
				_ExtendType increaser;
				_SizeType newsize = GAIA::ALGO::gmax(
					increaser.Increase(this->write_size()),
					this->write_size() + size);
				GAIA::U8* pNew = gnew GAIA::U8[newsize];
				if(m_pWrite != m_pFront)
					GAIA::ALGO::gmemcpy(pNew, m_pFront, this->write_size());
				if(m_pFront != GNIL)
					gdel[] m_pFront;
				m_pWrite = pNew + this->write_size();
				m_pRead = pNew + this->read_size();
				m_pFront = pNew;
				m_pBack = pNew + newsize;
				U8* pRet = m_pWrite;
				m_pWrite += size;
				return pRet;
			}
			GINL GAIA::GVOID write(const GAIA::GVOID* p, const _SizeType& size)
			{
				GAST(!!p);
				if(p == GNIL)
					return;
				GAST(size != 0);
				if(size == 0)
					return;
				GAIA::U8* pNew = this->alloc(size);
				GAIA::ALGO::gmemcpy(pNew, p, size);
			}
			GINL GAIA::BL read(GAIA::GVOID* p, const _SizeType& size)
			{
				GAST(!!p);
				if(p == GNIL)
					return GAIA::False;
				GAST(size != 0);
				if(size == 0)
					return GAIA::False;
				if(size > this->capacity() - this->read_size())
					GTHROW_RET(ExceedContent, GAIA::False);
				GAIA::ALGO::gmemcpy(p, m_pRead, size);
				m_pRead += size;
				return GAIA::True;
			}
			GINL GAIA::GVOID write(const __MyType& src)
			{
				if(src.write_size() == 0)
					return;
				this->write(src.fptr(), src.write_size());
			}
			GINL GAIA::BL read(__MyType& src)
			{
				if(src.write_size() == 0)
					return GAIA::False;
				return this->read(src.fptr(), src.write_size());
			}
			template<typename _ParamDataType> GAIA::GVOID write(const _ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return;
				GAIA::NUM bytes = GAIA::ALGO::gstrlen(psz) * sizeof(_ParamDataType) + sizeof(_ParamDataType);
				this->write(psz, bytes);
			}
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return GAIA::False;
				if(m_pRead == m_pBack)
					return GAIA::False;
				_ParamDataType* p = psz;
				while(this->capacity() - this->read_size() >= sizeof(_ParamDataType))
				{
					*p = *(_ParamDataType*)m_pRead;
					m_pRead += sizeof(_ParamDataType);
					if(*p == 0)
						return GAIA::True;
					++p;
				}
				*p = '\0';
				GTHROW_RET(ExceedContent, GAIA::False);
			}
			template<typename _ParamDataType> GAIA::GVOID write(const _ParamDataType& obj)
			{
				this->write(&obj, sizeof(obj));
			}
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType& obj)
			{
				return this->read(&obj, sizeof(obj));
			}
			template<typename _ParamLenType, typename _ParamDataType> GAIA::GVOID writex(const _ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return;
				_ParamLenType l = GAIA::ALGO::gstrlen(psz);
				this->write(l);
				if(l > 0)
					this->write(psz, l * sizeof(_ParamDataType));
			}
			template<typename _ParamLenType, typename _ParamDataType> GAIA::GVOID writex(const _ParamDataType* psz, const _ParamLenType& l)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return;
				if(l < 0)
					return;
				this->write(l);
				if(l > 0)
					this->write(psz, l * sizeof(_ParamDataType));
			}
			template<typename _ParamDataType> _ParamDataType readx()
			{
				_ParamDataType t;
				if(!this->read(t))
					GTHROW_RET(ExceedContent, GNIL);
				return t;
			}
			template<typename _ParamLenType, typename _ParamDataType> _ParamLenType readx(_ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return (_ParamLenType)GINVALID;
				_ParamLenType l;
				if(!this->read(l))
					return (_ParamLenType)GINVALID;
				if(l < 0)
					return (_ParamLenType)GINVALID;
				if(l > 0)
				{
					if(!this->read(psz, l * sizeof(_ParamDataType)))
						return (_ParamLenType)GINVALID;
				}
				psz[l] = '\0';
				return l;
			}
			template<typename _ParamLenType, typename _ParamDataType> _ParamLenType readx(_ParamDataType* psz, const _ParamLenType& size)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return (_ParamLenType)GINVALID;
				_ParamLenType l;
				if(!this->read(l))
					return (_ParamLenType)GINVALID;
				if(l < 0)
					return (_ParamLenType)GINVALID;
				if(l > 0)
				{
					if(l >= size)
						return (_ParamLenType)GINVALID;
					if(!this->read(psz, l * sizeof(_ParamDataType)))
						return (_ParamLenType)GINVALID;
				}
				psz[l] = '\0';
				return l;
			}
			template<typename _ParamSizeType, typename _ParamDataType> _SizeType find(_ParamDataType p, const _ParamSizeType& size, const _SizeType& index = 0) const
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GINVALID;
				GAST(size > 0);
				if(size <= 0)
					return GINVALID;
				GAST(index >= 0);
				if(index < 0)
					return GINVALID;
				if(index >= this->remain())
					return GINVALID;
				const GAIA::U8* pFinded = GAIA::ALGO::finds(this->read_ptr() + index, this->write_ptr(), p, size);
				if(pFinded == GNIL)
					return GINVALID;
				return (_SizeType)(pFinded - this->read_ptr());
			}
			template<typename _ParamSizeType, typename _ParamDataType> GAIA::BL startwith(_ParamDataType p, const _ParamSizeType& size) const
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GAIA::False;
				GAST(size > 0);
				if(size <= 0)
					return GAIA::False;
				if(this->remain() == 0)
					return GAIA::False;
				return GAIA::ALGO::startwith(this->read_ptr(), this->remain(), p, size);
			}
			template<typename _ParamSizeType, typename _ParamDataType> GAIA::BL endwith(_ParamDataType p, const _ParamSizeType& size) const
			{
				GAST(p != GNIL);
				if(p == GNIL)
					return GAIA::False;
				GAST(size > 0);
				if(size <= 0)
					return GAIA::False;
				if(this->remain() == 0)
					return GAIA::False;
				return GAIA::ALGO::endwith(this->read_ptr(), this->remain(), p, size);
			}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				if(this->write_size() < src.write_size())
					return -1;
				else if(this->write_size() > src.write_size())
					return +1;
				if(this->write_size() == 0)
					return 0;
				return GAIA::ALGO::gmemcmp(this->fptr(), src.fptr(), src.write_size());
			}
			GCLASS_COMPARE_BYCOMPARE(__MyType)
			template<typename _ParamDataType> __MyType& operator << (const _ParamDataType* psz){this->write(psz); return *this;}
			template<typename _ParamDataType> __MyType& operator >> (_ParamDataType* psz){this->read(psz); return *this;}
			template<typename _ParamDataType> __MyType& operator << (const _ParamDataType& obj){this->write(obj); return *this;}
			template<typename _ParamDataType> __MyType& operator >> (_ParamDataType& obj){this->read(obj); return *this;}
			template<typename _ParamDataType> const GAIA::U8& operator [] (const _ParamDataType& index) const{GAST(index < this->write_size()); return this->fptr()[index];}
			template<typename _ParamDataType> GAIA::U8& operator [] (const _ParamDataType& index){GAST(index < this->write_size()); return this->fptr()[index];}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				if(this->capacity() >= src.write_size())
					this->clear();
				else
					this->reserve(src.write_size());
				if(src.write_size() > 0)
				{
					GAIA::ALGO::gmemcpy(this->fptr(), src.fptr(), src.write_size());
					m_pWrite = m_pFront + src.write_size();
				}
				return *this;
			}
			GINL operator GAIA::U8*(){return m_pFront;}
		private:
			GINL GAIA::GVOID init(){m_pFront = m_pBack = m_pWrite = m_pRead = GNIL;}
			GINL GAIA::BL seek_ptr(const _SizeType& size, SEEK_TYPE seektype, GAIA::U8*& p)
			{
				switch(seektype)
				{
				case SEEK_TYPE_BEGIN:
					{
						if(size < 0 || size > this->capacity())
							GTHROW_RET(ExceedContent, GAIA::False);
						p = m_pFront + size;
					}
					break;
				case SEEK_TYPE_END:
					{
						if(size < 0 || size > this->capacity())
							GTHROW_RET(ExceedContent, GAIA::False);
						p = m_pBack - size;
					}
					break;
				case SEEK_TYPE_FORWARD:
					{
						if(GSCAST(_SizeType)(m_pBack - p) < size)
							GTHROW_RET(ExceedContent, GAIA::False);
						p += size;
					}
					break;
				case SEEK_TYPE_BACKWARD:
					{
						if(GSCAST(_SizeType)(p - m_pFront) < size)
							GTHROW_RET(ExceedContent, GAIA::False);
						p -= size;
					}
					break;
				default:
					return GAIA::False;
				}
				return GAIA::True;
			}
	#ifdef GAIA_DEBUG_MACHINELENGTH
		private: // Protect for 32-64bit error.
			GINL GAIA::GVOID write(const GAIA::NM& obj){}
			GINL GAIA::BL read(GAIA::NM& obj){return GAIA::False;}
			GINL GAIA::GVOID write(const GAIA::UM& obj){}
			GINL GAIA::BL read(GAIA::UM& obj){return GAIA::False;}
			GINL GAIA::GVOID write(const GAIA::WCH& obj){}
			GINL GAIA::BL read(GAIA::WCH& obj){return GAIA::False;}
			GINL GAIA::GVOID write(const GAIA::NM* psz){}
			GINL GAIA::BL read(GAIA::NM* psz){return GAIA::False;}
			GINL GAIA::GVOID write(const GAIA::UM* psz){}
			GINL GAIA::BL read(GAIA::UM* psz){return GAIA::False;}
			GINL __MyType& operator << (const GAIA::NM& obj){return *this;}
			GINL __MyType& operator >> (GAIA::NM& obj){return *this;}
			GINL __MyType& operator << (const GAIA::UM& obj){return *this;}
			GINL __MyType& operator >> (GAIA::UM& obj){return *this;}
			GINL __MyType& operator << (const GAIA::NM* obj){return *this;} GINL __MyType& operator << (GAIA::NM* obj){return *this;}
			GINL __MyType& operator >> (GAIA::NM* obj){return *this;}
			GINL __MyType& operator << (const GAIA::UM* obj){return *this;} GINL __MyType& operator << (GAIA::UM* obj){return *this;}
			GINL __MyType& operator >> (GAIA::UM* obj){return *this;}
	#endif
		private:
			GAIA::U8* m_pFront;
			GAIA::U8* m_pBack;
			GAIA::U8* m_pWrite;
			GAIA::U8* m_pRead;
		};
		class Buffer : public BasicBuffer<GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> >{public:};
	}
}

#endif
