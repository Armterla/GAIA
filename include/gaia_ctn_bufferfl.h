#ifndef		__GAIA_CTN_BUFFERFL_H__
#define		__GAIA_CTN_BUFFERFL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"
#include "gaia_algo_string.h"

namespace GAIA
{
	namespace CTN
	{
		/*!
			@brief Fixed-Length-Buffer.

			FL means Fixed-Length.
		*/
		template<typename _SizeType> class BasicBufferFL : public GAIA::Base
		{
		public:
			typedef _SizeType _sizetype;
		public:
			typedef BasicBufferFL<_SizeType> __MyType;
		public:
			GINL BasicBufferFL(){this->init();}
			GINL GAIA::GVOID proxy(GAIA::U8* pFront, GAIA::U8* pBack)
			{
				m_pFront = pFront;
				m_pBack = pBack;
				m_pWrite = pFront;
				m_pRead = pFront;
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
			GINL BasicBufferFL(const __MyType& src){this->init(); this->operator = (src);}
			GINL GAIA::BL empty() const{return this->write_size() == 0;}
			GINL GAIA::BL nill() const{return m_pFront == GNIL;}
			GINL _SizeType capacity() const{return GSCAST(_SizeType)(m_pBack - m_pFront);}
			GINL GAIA::GVOID clear(){m_pWrite = m_pRead = m_pFront;}
			GINL GAIA::GVOID destroy(){m_pFront = m_pBack = m_pWrite = m_pRead = GNIL;}
			GINL GAIA::BL resize(const _SizeType& size)
			{
				GAST(size >= 0);
				if(this->capacity() < size)
					return GAIA::False;
				m_pWrite = m_pRead = m_pFront;
				m_pWrite += size;
				return GAIA::True;
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
			GINL GAIA::BL write(const GAIA::GVOID* p, const _SizeType& size)
			{
				GAST(!!p);
				if(p == GNIL)
					return GAIA::False;
				GAST(size != 0);
				if(size == 0)
					return GAIA::False;
				if(size > this->capacity() - this->write_size())
					GTHROW_RET(ExceedContent, GAIA::False);
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
					return GAIA::True;
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
				return this->write(psz, bytes);
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
			template<typename _ParamDataType> GAIA::BL write(const _ParamDataType& obj)
			{
				return this->write(&obj, sizeof(obj));
			}
			template<typename _ParamDataType> GAIA::BL read(_ParamDataType& obj)
			{
				return this->read(&obj, sizeof(obj));
			}
			template<typename _ParamLenType, typename _ParamDataType> GAIA::BL writex(const _ParamDataType* psz)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return GAIA::False;
				_ParamLenType l = GAIA::ALGO::gstrlen(psz);
				if(!this->write(l))
					return GAIA::False;
				if(l > 0)
				{
					if(!this->write(psz, l * sizeof(_ParamDataType)))
						return GAIA::False;
				}
				return GAIA::True;
			}
			template<typename _ParamLenType, typename _ParamDataType> GAIA::BL writex(const _ParamDataType* psz, const _ParamLenType& l)
			{
				GAST(!!psz);
				if(psz == GNIL)
					return GAIA::False;
				if(l < 0)
					return GAIA::False;
				if(!this->write(l))
					return GAIA::False;
				if(l > 0)
				{
					if(!this->write(psz, l * sizeof(_ParamDataType)))
						return GAIA::False;
				}
				return GAIA::True;
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
				return GSCAST(_SizeType)(pFinded - this->read_ptr());
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
				if(this->capacity() < src.write_size())
					GTHROW_RET(BufNotEnough, *this);
				if(src.write_size() > 0)
				{
					GAIA::ALGO::gmemcpy(this->fptr(), src.fptr(), src.write_size());
					m_pWrite = m_pFront + src.write_size();
					m_pRead = m_pFront;
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
		private:
			GAIA::U8* m_pFront;
			GAIA::U8* m_pBack;
			GAIA::U8* m_pWrite;
			GAIA::U8* m_pRead;
		};
		class BufferFL : public BasicBufferFL<GAIA::NUM>{public:};
	}
}

#endif
