#ifndef		__GAIA_CTN_STACKBUFFER_H__
#define		__GAIA_CTN_STACKBUFFER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _SizeType, _SizeType _Size> class BasicStackBuffer : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
		public:
			static const _SizeType _size = _Size;
		public:
			typedef BasicStackBuffer<_SizeType, _Size> __MyType;
		public:
			GINL BasicStackBuffer(){this->init();}
			GINL BasicStackBuffer(const __MyType& src)
			{
				this->init();
				this->operator = (src);
			}
			GINL GAIA::BL assign(const GAIA::GVOID* p, const _SizeType& size)
			{
				GAST(p != GNIL);
				GAST(size > 0);
				if(!this->resize(size))
					return GAIA::False;
				GAIA::ALGO::gmemcpy(m_pFront, p, size);
				return GAIA::True;
			}
			GINL GAIA::GVOID clear(){m_pWrite = m_pRead = m_pFront;}
			GINL GAIA::BL empty() const{return this->write_size() == 0;}
			GINL _SizeType capacity() const{return GSCAST(_SizeType)(this->bptr() - this->fptr());}
			GINL GAIA::BL resize(const _SizeType& size){if(size > this->capacity()) return GAIA::False; m_pRead = m_pWrite = m_pFront; m_pWrite += size; return GAIA::True;}
			GINL const GAIA::U8* fptr() const{return m_pFront;}
			GINL GAIA::U8* fptr(){return m_pFront;}
			GINL const GAIA::U8* bptr() const{return &m_pFront[_Size];}
			GINL GAIA::U8* bptr(){return &m_pFront[_Size];}
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
			GINL GAIA::BL write(const GAIA::GVOID* p, const _SizeType& size)
			{
				GAST(!!p);
				if(p == GNIL)
					return GAIA::False;
				GAST(size != 0);
				if(size == 0)
					return GAIA::False;
				if(size > this->capacity() - this->write_size())
					return GAIA::False;
				GAIA::ALGO::gmemcpy(m_pWrite, p, size);
				m_pWrite += size;
				return GAIA::True;
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
			GINL GAIA::BL write(const __MyType& src)
			{
				if(src.write_size() == 0)
					return GAIA::False;
				return this->write(src.fptr(), src.write_size());
			}
			GINL GAIA::BL read(__MyType& src)
			{
				if(src.write_size() == 0)
					return GAIA::False;
				return this->read(src.fptr(), src.write_size());
			}
			template<typename _ParamDataType> GAIA::BL write(const _ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return GAIA::False;
				GAIA::NUM bytes = GAIA::ALGO::gstrlen(psz) * sizeof(_ParamDataType) + sizeof(_ParamDataType);
				if(bytes > this->capacity() - this->write_size())
					return GAIA::False;
				GAIA::ALGO::gmemcpy(m_pWrite, psz, bytes);
				m_pWrite += bytes;
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return GAIA::False;
				if(m_pRead == this->bptr())
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
				return GAIA::False;
			}
			template<typename _ParamDataType> GAIA::BL write(const _ParamDataType& obj)
			{
				if(sizeof(obj) > this->capacity() - this->write_size())
					return GAIA::False;
				GAIA::ALGO::gmemcpy(m_pWrite, &obj, sizeof(obj));
				m_pWrite += sizeof(obj);
				return GAIA::True;
			}
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType& obj)
			{
				if(sizeof(obj) > this->capacity() - this->read_size())
					GTHROW_RET(ExceedContent, GAIA::False);
				GAIA::ALGO::gmemcpy(&obj, m_pRead, sizeof(obj));
				m_pRead += sizeof(obj);
				return GAIA::True;
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
			template<typename _ParamObjType> const GAIA::U8& operator [] (const _ParamObjType& index) const{GAST(index < this->write_size()); return this->fptr()[index];}
			template<typename _ParamObjType> GAIA::U8& operator [] (const _ParamObjType& index){GAST(index < this->write_size()); return this->fptr()[index];}
			GINL __MyType& operator = (const __MyType& src)
			{
				GAST(&src != this);
				this->clear();
				if(src.write_size() > 0)
				{
					GAIA::ALGO::gmemcpy(this->fptr(), src.fptr(), src.write_size());
					m_pWrite = m_pFront + src.write_size();
				}
				return *this;
			}
			GINL operator GAIA::U8*(){return m_pFront;}
		private:
			GINL GAIA::GVOID init()
			{
				m_pRead = m_pWrite = m_pFront;
			}
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
						p = this->bptr() - size;
					}
					break;
				case SEEK_TYPE_FORWARD:
					{
						if(GSCAST(_SizeType)(this->bptr() - p) < size)
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
		private:
			GAIA::U8 m_pFront[_Size];
			GAIA::U8* m_pWrite;
			GAIA::U8* m_pRead;
		};
		template<GAIA::NUM _Size> class StackBuffer : public BasicStackBuffer<GAIA::NUM, _Size>{public:};
	}
}

#endif
