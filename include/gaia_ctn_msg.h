#ifndef		__GAIA_CTN_MSG_H__
#define		__GAIA_CTN_MSG_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_compare.h"
#include "gaia_algo_string.h"

namespace GAIA
{
	namespace CTN
	{
		template<typename _SizeType> class BasicMsg;
		template<typename _SizeType> class BasicMsgPopper
		{
			friend class BasicMsg<_SizeType>;
		public:
			template<typename _ParamDataType> operator _ParamDataType();
			template<typename _ParamDataType> operator const _ParamDataType&() const;
			template<typename _ParamDataType> operator const _ParamDataType*();
		private:
			BasicMsg<_SizeType>* m_pMsg;
		};
		template<typename _SizeType> class BasicMsg
		{
			friend class BasicMsgPopper<_SizeType>;
		public:
			typedef _SizeType _sizetype;
		public:
			typedef BasicMsg<_SizeType> __MyType;
			typedef BasicMsgPopper<_SizeType> __PopperType;
		public:
			GINL BasicMsg(){this->init();}
			GINL BasicMsg(const __MyType& src){this->init(); *this = src;}
			GINL ~BasicMsg(){this->destroy();}
			GINL GAIA::GVOID proxy(GAIA::U8* pBegin, GAIA::U8* pEnd)
			{
				if(pBegin != GNIL && pEnd == GNIL)
					GTHROW(InvalidParam);
				if(pBegin == GNIL && pEnd != GNIL)
					GTHROW(InvalidParam);
				if(pEnd < pBegin)
					GTHROW(InvalidParam);
				this->destroy();
				m_bHeapOwner = GAIA::False;
				m_p = pBegin;
				m_nSize = (GAIA::N32)(pEnd - pBegin + 1);
				m_nCapacity = m_nSize;
				m_nCursor = 0;
				if(m_p == GNIL)
					m_bHeapOwner = GAIA::True;
			}
			GINL GAIA::GVOID proxy(GAIA::U8* pBegin, GAIA::N32 nSize)
			{
				if(pBegin != GNIL && nSize <= 0)
					GTHROW(InvalidParam);
				if(pBegin == GNIL && nSize != 0)
					GTHROW(InvalidParam);
				this->destroy();
				m_bHeapOwner = GAIA::False;
				m_p = pBegin;
				m_nSize = nSize;
				m_nCapacity = m_nSize;
				m_nCursor = 0;
				if(m_p == GNIL)
					m_bHeapOwner = GAIA::True;
			}
			GINL GAIA::BL proxy(){return !m_bHeapOwner;}
			GINL GAIA::GVOID clear(){m_nSize = 0; m_nCursor = 0;}
			GINL GAIA::GVOID destroy()
			{
				if(m_p != GNIL)
				{
					if(m_bHeapOwner)
						gdel[] m_p;
					m_bHeapOwner = GAIA::True;
					m_p = GNIL;
					m_nSize = 0;
					m_nCapacity = 0;
					m_nCursor = 0;
				}
				else
					GAST(m_bHeapOwner);
			}
			GINL GAIA::BL empty() const{return this->size() == 0;}
			GINL GAIA::BL nill() const{return m_p == GNIL;}
			GINL GAIA::N32 size() const{return m_nSize;}
			GINL GAIA::N32 capacity() const{return m_nCapacity;}
			GINL GAIA::GVOID resize(GAIA::N32 nSize)
			{
				if(nSize < 0)
					GTHROW(InvalidParam);
				if(nSize == m_nSize)
					return;
				if(nSize < m_nSize)
				{
					m_nSize = nSize;
					if(m_nCursor > m_nSize)
						m_nCursor = m_nSize;
					return;
				}
				if(nSize <= m_nCapacity)
				{
					m_nSize = nSize;
					return;
				}
				this->reserve(nSize);
				m_nSize = nSize;
			}
			GINL GAIA::GVOID resize_keep(GAIA::N32 nSize)
			{
				if(nSize < 0)
					GTHROW(InvalidParam);
				if(nSize == m_nSize)
					return;
				if(nSize < m_nSize)
				{
					m_nSize = nSize;
					if(m_nCursor > m_nSize)
						m_nCursor = m_nSize;
					return;
				}
				if(nSize <= m_nCapacity)
				{
					m_nSize = nSize;
					return;
				}
				this->reserve_keep(nSize);
				m_nSize = nSize;
			}
			GINL GAIA::GVOID reserve(GAIA::N32 nSize)
			{
				if(nSize < 0)
					GTHROW(InvalidParam);
				if(nSize == m_nCapacity)
					return;
				if(m_p != GNIL)
				{
					if(m_bHeapOwner)
						gdel[] m_p;
					m_bHeapOwner = GAIA::True;
					m_p = GNIL;
				}
				else
					GAST(m_bHeapOwner);
				if(nSize != 0)
					m_p = gnew GAIA::U8[nSize];
				m_nSize = 0;
				m_nCapacity = nSize;
				m_nCursor = 0;
			}
			GINL GAIA::GVOID reserve_keep(GAIA::N32 nSize)
			{
				if(nSize < 0)
					GTHROW(InvalidParam);
				if(nSize == m_nCapacity)
					return;
				if(nSize != 0)
				{
					GAIA::U8* pNew = gnew GAIA::U8[nSize];
					if(m_nSize != 0)
					{
						GAST(m_p != GNIL);
						GAIA::ALGO::gmemcpy(pNew, m_p, GAIA::ALGO::gmin(m_nSize, nSize));
						if(m_bHeapOwner)
							gdel[] m_p;
						m_bHeapOwner = GAIA::True;
					}
					else
					{
						GAST(m_p == GNIL);
						GAST(m_bHeapOwner);
					}
					m_p = pNew;
					m_nCapacity = nSize;
					if(m_nSize > m_nCapacity)
						m_nSize = m_nCapacity;
					if(m_nCursor > m_nSize)
						m_nCursor = m_nSize;
				}
				else
				{
					if(m_p != GNIL)
					{
						if(m_bHeapOwner)
							gdel[] m_p;
						m_bHeapOwner = GAIA::True;
						m_p = GNIL;
						m_nSize = 0;
						m_nCapacity = 0;
						m_nCursor = 0;
					}
					else
						GAST(m_bHeapOwner);
				}
			}
			GINL GAIA::N32 tell(){return m_nCursor;}
			GINL GAIA::GVOID seek(GAIA::N32 nCursor)
			{
				if(nCursor == GINVALID)
					nCursor = this->size();
				if(nCursor < 0)
					GTHROW(InvalidParam);
				if(nCursor > m_nSize)
					GTHROW(InvalidParam);
				m_nCursor = nCursor;
			}
			GINL GAIA::U8* fptr(){return m_p;}
			GINL GAIA::U8* bptr(){if(this->empty()) return GNIL; return m_p + m_nSize - 1;}
			GINL const GAIA::U8* fptr() const{return m_p;}
			GINL const GAIA::U8* bptr() const{return m_p + m_nSize - 1;}
			GINL const GAIA::U8* cursor_ptr() const{return m_p + m_nCursor;}
			GINL __MyType& operator = (const __MyType& src)
			{
				if(src.empty())
				{
					this->clear();
					return *this;
				}
				if(src.size() != this->size())
					this->resize(src.size());
				GAIA::ALGO::gmemcpy(m_p, src.m_p, src.size());
				m_nCursor = 0;
				return *this;
			}
			template<typename _ParamDataType> GAIA::GVOID push(const _ParamDataType& t)
			{
			#ifdef NEG_DEBUG_SELFCHECK
				_ParamDataType tTest = 0;
				tTest = 1;
			#endif
				GAIA::U8 type_id = (GAIA::U8)datatotype(t);
				if(type_id == GAIA::TYPEID_INVALID)
					GTHROW(InvalidParam);
				if(type_id == GAIA::TYPEID_POINTER || type_id == GAIA::TYPEID_CONSTPOINTER)
					GTHROW(InvalidParam);
				if(type_id == GAIA::TYPEID_CHARPOINTER || type_id == GAIA::TYPEID_CONSTCHARPOINTER)
					GTHROW(InvalidParam);
				if(type_id == GAIA::TYPEID_WCHARPOINTER || type_id == GAIA::TYPEID_CONSTWCHARPOINTER)
					GTHROW(InvalidParam);
				GAIA::N32 nTypeSize = sizeof(GAIA::U8) + sizeof(t);
				if(m_nCapacity - m_nSize < nTypeSize)
				{
					GAIA::ALGO::ExtendGold<_SizeType> extend;
					this->reserve_keep(GAIA::ALGO::gmax(m_nSize + nTypeSize, extend.Increase(m_nCapacity)));
				}
				m_p[m_nSize++] = type_id;
				GAIA::ALGO::gmemcpy(m_p + m_nSize, &t, sizeof(t));
				m_nSize += sizeof(t);
			}
			template<typename _ParamDataType> GAIA::GVOID push(const _ParamDataType* p)
			{
				GAIA::U8 type_id = (GAIA::U8)datatotype(p);
				if(type_id == GAIA::TYPEID_INVALID)
					GTHROW(InvalidParam);
				if(type_id != GAIA::TYPEID_CHARPOINTER && type_id != GAIA::TYPEID_CONSTCHARPOINTER &&
					type_id != GAIA::TYPEID_WCHARPOINTER && type_id != GAIA::TYPEID_CONSTWCHARPOINTER)
					GTHROW(InvalidParam);
				GAIA::N32 nLength;
				if(p == GNIL)
					nLength = 0;
				else
					nLength = GAIA::ALGO::gstrlen(p) + 1;
				if(nLength > 0xFFFF)
					GTHROW(InvalidParam);
				GAIA::N32 nStringByteSize = nLength * sizeof(_ParamDataType);
				GAIA::N32 nTypeSize = sizeof(GAIA::U8) + sizeof(GAIA::U16) + nStringByteSize;
				if(m_nCapacity - m_nSize < nTypeSize)
				{
					GAIA::ALGO::ExtendGold<_SizeType> extend;
					this->reserve_keep(GAIA::ALGO::gmax(m_nSize + nTypeSize, extend.Increase(m_nCapacity)));
				}
				m_p[m_nSize++] = type_id;
				GAIA::U16 u16Length = (GAIA::U16)nLength;
				GAIA::ALGO::gmemcpy(m_p + m_nSize, &u16Length, sizeof(GAIA::U16));
				m_nSize += sizeof(GAIA::U16);
				if(p != GNIL)
					GAIA::ALGO::gmemcpy(m_p + m_nSize, p, nStringByteSize);
				m_nSize += nStringByteSize;
			}
			template<typename _ParamDataType> GAIA::GVOID push(const _ParamDataType* p, GAIA::N32 nSize)
			{
				if(nSize < 0)
					GTHROW(InvalidParam);
				GAIA::U8 type_id = (GAIA::U8)datatotype(p);
				if(type_id == GAIA::TYPEID_INVALID)
					GTHROW(InvalidParam);
				if(type_id != GAIA::TYPEID_POINTER && type_id != GAIA::TYPEID_CONSTPOINTER &&
					type_id != GAIA::TYPEID_CHARPOINTER && type_id != GAIA::TYPEID_CONSTCHARPOINTER &&
					type_id != GAIA::TYPEID_WCHARPOINTER && type_id != GAIA::TYPEID_CONSTWCHARPOINTER)
					GTHROW(InvalidParam);
				if(type_id == GAIA::TYPEID_POINTER || type_id == GAIA::TYPEID_CONSTPOINTER)
				{
					GAIA::N32 nTypeSize = sizeof(GAIA::U8) + sizeof(GAIA::U32) + nSize;
					if(m_nCapacity - m_nSize < nTypeSize)
					{
						GAIA::ALGO::ExtendGold<_SizeType> extend;
						this->reserve_keep(GAIA::ALGO::gmax(m_nSize + nTypeSize, extend.Increase(m_nCapacity)));
					}
					m_p[m_nSize++] = type_id;
					GAIA::ALGO::gmemcpy(m_p + m_nSize, &nSize, sizeof(nSize));
					m_nSize += sizeof(nSize);
					if(p != GNIL)
						GAIA::ALGO::gmemcpy(m_p + m_nSize, p, nSize);
					m_nSize += nSize;
				}
				else
				{
					GAIA::N32 nStringByteSize = nSize;
					if(type_id == GAIA::TYPEID_WCHARPOINTER || type_id == GAIA::TYPEID_CONSTWCHARPOINTER)
						nStringByteSize *= sizeof(GAIA::WCH);
					GAIA::N32 nTypeSize = sizeof(GAIA::U8) + sizeof(GAIA::U16) + nStringByteSize;
					if(p != GNIL)
					{
						if(type_id == GAIA::TYPEID_WCHARPOINTER || type_id == GAIA::TYPEID_CONSTWCHARPOINTER)
							nTypeSize += sizeof(GAIA::WCH);
					}
					if(m_nCapacity - m_nSize < nTypeSize)
					{
						GAIA::ALGO::ExtendGold<_SizeType> extend;
						this->reserve_keep(GAIA::ALGO::gmax(m_nSize + nTypeSize, extend.Increase(m_nCapacity)));
					}
					m_p[m_nSize++] = type_id;
					GAIA::U16 u16Length;
					if(p == GNIL)
						u16Length = 0;
					else
						u16Length = (GAIA::U16)nSize + 1;
					GAIA::ALGO::gmemcpy(m_p + m_nSize, &u16Length, sizeof(GAIA::U16));
					m_nSize += sizeof(GAIA::U16);
					if(p != GNIL)
						GAIA::ALGO::gmemcpy(m_p + m_nSize, p, nStringByteSize);
					m_nSize += nStringByteSize;
					if(p != GNIL)
					{
						if(type_id == GAIA::TYPEID_WCHARPOINTER || type_id == GAIA::TYPEID_CONSTWCHARPOINTER)
						{
							*(GAIA::WCH*)(m_p + m_nSize) = '\0';
							m_nSize += sizeof(GAIA::WCH);
						}
						else
						{
							*(GAIA::N8*)(m_p + m_nSize) = '\0';
							m_nSize += sizeof(GAIA::N8);
						}
					}
				}
			}
			GINL __PopperType pop()
			{
				__PopperType popper;
				popper.m_pMsg = this;
				return popper;
			}
			GINL const GAIA::GVOID* pop(GAIA::N32& nSize)
			{
				const GAIA::GVOID* ret = GNIL;
				GAIA::U8 type_id = (GAIA::U8)datatotype(ret);
				if(type_id != GAIA::TYPEID_POINTER && type_id != GAIA::TYPEID_CONSTPOINTER)
					GTHROW(InvalidParam);
				if(m_nSize - m_nCursor < sizeof(GAIA::U8) + sizeof(GAIA::N32))
					GTHROW(MemOverflow);
				if(type_id != m_p[m_nCursor])
					GTHROW(Convert);
				++m_nCursor;
				GAIA::N32 nBufferSize = *(GAIA::N32*)&m_p[m_nCursor];
				m_nCursor += sizeof(GAIA::N32);
				if(nBufferSize == 0)
					ret = GNIL;
				else
					ret = m_p + m_nCursor;
				nSize = nBufferSize;
				m_nCursor += nBufferSize;
				return ret;
			}
			GINL GAIA::BL exist() const{return m_nCursor < m_nSize;}
			GINL GAIA::TYPEID type_id() const
			{
				if(m_nCursor >= m_nSize)
					GTHROW(Illegal);
				return (GAIA::TYPEID)m_p[m_nCursor];
			}
			GAIA::GVOID jump(GAIA::N32 nStep)
			{
				if(nStep < 0)
					GTHROW(InvalidParam);
				if(nStep == 0)
					return;
				GAIA::N32 nCursorOld = m_nCursor;
				for(GAIA::N32 x = 0; x < nStep; ++x)
				{
					if(m_nCursor >= m_nSize)
					{
						m_nCursor = nCursorOld;
						GTHROW(InvalidParam);
					}
					GAIA::TYPEID tid = this->type_id();
				#ifdef __APPLE__
				#	pragma clang diagnostic push
				#	pragma clang diagnostic ignored"-Wunused-value"
				#	pragma clang diagnostic ignored"-Wunused-variable"
				#endif
					switch(tid)
					{
					case TYPEID_NM:
						(GAIA::NM)this->pop();
						break;
					case TYPEID_UM:
						(GAIA::UM)this->pop();
						break;
					case TYPEID_BL:
						(GAIA::BL)this->pop();
						break;
					case TYPEID_N8:
						(GAIA::N8)this->pop();
						break;
					case TYPEID_U8:
						(GAIA::U8)this->pop();
						break;
					case TYPEID_N16:
						(GAIA::N16)this->pop();
						break;
					case TYPEID_U16:
						(GAIA::U16)this->pop();
						break;
					case TYPEID_N32:
						(GAIA::N32)this->pop();
						break;
					case TYPEID_U32:
						(GAIA::U32)this->pop();
						break;
					case TYPEID_N64:
						(GAIA::N64)this->pop();
						break;
					case TYPEID_U64:
						(GAIA::U64)this->pop();
						break;
					case TYPEID_X128:
						{
							GAIA::X128 x128 = this->pop();
						}
						break;
					case TYPEID_F32:
						(GAIA::F32)this->pop();
						break;
					case TYPEID_F64:
						(GAIA::F64)this->pop();
						break;
					case TYPEID_CHARPOINTER:
						(const GAIA::CH*)this->pop();
						break;
					case TYPEID_CONSTCHARPOINTER:
						(const GAIA::CH*)this->pop();
						break;
					case TYPEID_WCHARPOINTER:
						(const GAIA::WCH*)this->pop();
						break;
					case TYPEID_CONSTWCHARPOINTER:
						(const GAIA::WCH*)this->pop();
						break;
					case TYPEID_POINTER:
						{
							GAIA::N32 nSize;
							(const GAIA::GVOID*)this->pop(nSize);
						}
						break;
					case TYPEID_CONSTPOINTER:
						{
							GAIA::N32 nSize;
							(const GAIA::GVOID*)this->pop(nSize);
						}
						break;
					default:
						GTHROW(Illegal);
					}
				#ifdef __APPLE__
				#	pragma clang diagnostic pop
				#endif
				}
			}
			template<typename _ParamDataType> __MyType& operator << (const _ParamDataType& t){this->push(t); return *this;}
			template<typename _ParamDataType> __MyType& operator >> (_ParamDataType& t){t = this->pop(); return *this;}
			template<typename _ParamDataType> __MyType& operator << (const _ParamDataType* p){this->push(p); return *this;}
			template<typename _ParamDataType> __MyType& operator >> (_ParamDataType* p){p = this->pop(); return *this;}
			template<typename _ParamDataType> GAIA::N32 tostring(_ParamDataType* p, GAIA::N32 nSize)
			{
				if(p != GNIL)
				{
					if(nSize == 0 || nSize < GINVALID)
						GTHROW(InvalidParam);
				}

				GAIA::N32 nOldCursor = m_nCursor;
				GAIA::N32 nRet = 0;

				this->seek(0);
				while(this->tell() != this->size())
				{
					if(nRet != 0)
					{
						if(p != GNIL)
							p[nRet] = ' ';
						nRet++;
					}

				#define MSG_TOSTRING_BASETYPE(type) \
					{\
						type v = this->pop();\
						if(p != GNIL)\
						{\
							GAIA::N32 nTypeLen = GAIA::ALGO::gmin(nSize - nRet, GAIA::ALGO::gstrlen(GAIA::TYPEID_ANAME[type_id]));\
							GAIA::ALGO::gstrcpy(p + nRet, GAIA::TYPEID_ANAME[type_id], nTypeLen);\
							nRet += nTypeLen;\
							if(nRet == nSize)\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
							p[nRet++] = ' ';\
							if(nRet == nSize)\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
							if(!GAIA::ALGO::castv(v, p + nRet, nSize - nRet))\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
							nRet += GAIA::ALGO::gstrlen(p + nRet);\
							if(nRet == nSize)\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
						}\
						else\
						{\
							nRet += GAIA::ALGO::gstrlen(GAIA::TYPEID_ANAME[type_id]);\
							nRet++;\
							GAIA::TCH szTemp[128];\
							GAIA::ALGO::castv(v, szTemp, 128);\
							nRet += GAIA::ALGO::gstrlen(szTemp);\
						}\
					}

				#define MSG_TOSTRING_STRINGTYPE(type) \
					{\
						type pTemp = this->pop();\
						if(p != GNIL)\
						{\
							GAIA::N32 nTypeLen = GAIA::ALGO::gmin(nSize - nRet, GAIA::ALGO::gstrlen(GAIA::TYPEID_ANAME[type_id]));\
							GAIA::ALGO::gstrcpy(p + nRet, GAIA::TYPEID_ANAME[type_id], nTypeLen);\
							nRet += nTypeLen;\
							if(nRet == nSize)\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
							p[nRet++] = ' ';\
							if(nRet == nSize)\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
							p[nRet++] = '\"';\
							if(nRet == nSize)\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
							if(pTemp == GNIL)\
							{\
								GAIA::N32 nStringLen = GAIA::ALGO::gmin(nSize - nRet, GAIA::ALGO::gstrlen("(NullStringPtr)"));\
								GAIA::ALGO::gstrcpy(p + nRet, "(NullStringPtr)", nStringLen);\
								nRet += nStringLen;\
							}\
							else\
							{\
								GAIA::N32 nStringLen = GAIA::ALGO::gmin(nSize - nRet, GAIA::ALGO::gstrlen(pTemp));\
								GAIA::ALGO::gstrcpy(p + nRet, pTemp, nStringLen);\
								nRet += nStringLen;\
							}\
							if(nRet == nSize)\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
							p[nRet++] = '\"';\
							if(nRet == nSize)\
							{\
								m_nCursor = nOldCursor;\
								return nRet;\
							}\
						}\
						else\
						{\
							nRet += GAIA::ALGO::gstrlen(GAIA::TYPEID_ANAME[type_id]);\
							nRet++;\
							nRet++;\
							if(pTemp == GNIL)\
								nRet += GAIA::ALGO::gstrlen("(NullStringPtr)");\
							else\
								nRet += GAIA::ALGO::gstrlen(pTemp);\
							nRet++;\
						}\
					}

					GAIA::TYPEID type_id = (GAIA::TYPEID)this->type_id();
					switch(type_id)
					{
					case GAIA::TYPEID_NM:
						MSG_TOSTRING_BASETYPE(GAIA::NM)
							break;
					case GAIA::TYPEID_UM:
						MSG_TOSTRING_BASETYPE(GAIA::UM)
							break;
					case GAIA::TYPEID_BL:
						MSG_TOSTRING_BASETYPE(GAIA::BL)
							break;
					case GAIA::TYPEID_N8:
						MSG_TOSTRING_BASETYPE(GAIA::N8)
							break;
					case GAIA::TYPEID_U8:
						MSG_TOSTRING_BASETYPE(GAIA::U8)
							break;
					case GAIA::TYPEID_N16:
						MSG_TOSTRING_BASETYPE(GAIA::N16)
							break;
					case GAIA::TYPEID_U16:
						MSG_TOSTRING_BASETYPE(GAIA::U16)
							break;
					case GAIA::TYPEID_N32:
						MSG_TOSTRING_BASETYPE(GAIA::N32)
							break;
					case GAIA::TYPEID_U32:
						MSG_TOSTRING_BASETYPE(GAIA::U32)
							break;
					case GAIA::TYPEID_N64:
						MSG_TOSTRING_BASETYPE(GAIA::N64)
							break;
					case GAIA::TYPEID_U64:
						MSG_TOSTRING_BASETYPE(GAIA::U64)
							break;
					case GAIA::TYPEID_X128:
						MSG_TOSTRING_BASETYPE(GAIA::X128)
							break;
					case GAIA::TYPEID_F32:
						MSG_TOSTRING_BASETYPE(GAIA::F32)
							break;
					case GAIA::TYPEID_F64:
						MSG_TOSTRING_BASETYPE(GAIA::F64)
							break;
					case GAIA::TYPEID_CHARPOINTER:
						MSG_TOSTRING_STRINGTYPE(GAIA::N8*)
							break;
					case GAIA::TYPEID_CONSTCHARPOINTER:
						MSG_TOSTRING_STRINGTYPE(const GAIA::N8*)
							break;
					case GAIA::TYPEID_WCHARPOINTER:
						MSG_TOSTRING_STRINGTYPE(GAIA::WCH*)
							break;
					case GAIA::TYPEID_CONSTWCHARPOINTER:
						MSG_TOSTRING_STRINGTYPE(const GAIA::WCH*)
							break;
					case GAIA::TYPEID_POINTER:
					case GAIA::TYPEID_CONSTPOINTER:
						{
							GAIA::N32 nBufferSize;
							const GAIA::GVOID* pTemp = this->pop(nBufferSize);
							if(p != GNIL)
							{
								GAIA::N32 nTypeLen = GAIA::ALGO::gmin(nSize - nRet, GAIA::ALGO::gstrlen(GAIA::TYPEID_ANAME[type_id]));
								GAIA::ALGO::gstrcpy(p + nRet, GAIA::TYPEID_ANAME[type_id], nTypeLen);
								nRet += nTypeLen;
								if(nRet == nSize)
								{
									m_nCursor = nOldCursor;
									return nRet;
								}
								p[nRet++] = ' ';
								if(nRet == nSize)
								{
									m_nCursor = nOldCursor;
									return nRet;
								}

								if(!GAIA::ALGO::castv(nBufferSize, p + nRet, nSize - nRet))
								{
									m_nCursor = nOldCursor;
									return nRet;
								}
								nRet += GAIA::ALGO::gstrlen(p + nRet);
								if(nRet == nSize)
								{
									m_nCursor = nOldCursor;
									return nRet;
								}
								p[nRet++] = ' ';
								if(nRet == nSize)
								{
									m_nCursor = nOldCursor;
									return nRet;
								}

								GAIA::N32 nPracBufferSize = GAIA::ALGO::gmin((nSize - nRet) / 2, nBufferSize);
								if(pTemp == GNIL)
								{
									GAIA::N32 nStringLen = GAIA::ALGO::gmin(nSize - nRet, GAIA::ALGO::gstrlen("(NullPtr)"));
									GAIA::ALGO::gstrcpy(p + nRet, "(NullPtr)", nStringLen);
									nRet += nStringLen;
								}
								else
									GAIA::ALGO::hex2str((const GAIA::U8*)pTemp, nPracBufferSize, p + nRet);
								nRet += nPracBufferSize * 2;
								if(nRet == nSize)
								{
									m_nCursor = nOldCursor;
									return nRet;
								}
							}
							else
							{
								nRet += GAIA::ALGO::gstrlen(GAIA::TYPEID_ANAME[type_id]);
								nRet++;
								GAIA::TCH szTemp[128];
								GAIA::ALGO::castv(nBufferSize, szTemp, 128);
								nRet += GAIA::ALGO::gstrlen(szTemp);
								nRet++;
								if(pTemp == GNIL)
									nRet += GAIA::ALGO::gstrlen("(NullPtr)");
								else
									nRet += nBufferSize * 2;
							}
						}
						break;
					default:
						break;
					}
				}

				if(p != GNIL)
				{
					if(nRet != nSize)
						p[nRet] = '\0';
				}

				m_nCursor = nOldCursor;
				return nRet;
			}
			template<typename _ParamDataType> GAIA::BL fromstring(const _ParamDataType* p, GAIA::N32 nSize)
			{
				if(p == GNIL)
					GTHROW(InvalidParam);
				if(nSize < GINVALID || nSize == 0)
					GTHROW(InvalidParam);
				if(nSize == GINVALID)
					nSize = GAIA::ALGO::gstrlen(p);
				this->clear();
				GAIA::N32 nCurrent = 0;

				// Get max min type name len.
				GAIA::N32 nMaxTypeNameLen = 0;
				GAIA::N32 nMinTypeNameLen = 99999;
				for(GAIA::N32 x = 0; x < sizeof(GAIA::TYPEID_ANAME) / sizeof(GAIA::TYPEID_ANAME[0]); ++x)
				{
					GAIA::N32 nLen = GAIA::ALGO::gstrlen(GAIA::TYPEID_ANAME[x]);
					if(nLen > nMaxTypeNameLen)
						nMaxTypeNameLen = nLen;
					if(nLen < nMinTypeNameLen)
						nMinTypeNameLen = nLen;
				}

				//
				while(nCurrent < nSize)
				{
					// Get type name.
					GAIA::N32 nTypeEndIndex = GINVALID;
					for(GAIA::N32 x = nMinTypeNameLen; x <= nMaxTypeNameLen; ++x)
					{
						if(p[nCurrent + x] == ' ')
						{
							nTypeEndIndex = x;
							break;
						}
					}
					if(nTypeEndIndex == GINVALID)
					{
						this->clear();
						return GAIA::False;
					}
					GAIA::TCH szTypeName[128];
					GAST(nMaxTypeNameLen < 128);
					GAIA::ALGO::gstrcpy(szTypeName, p + nCurrent, nTypeEndIndex);
					nCurrent += nTypeEndIndex;
					if(nCurrent == nSize)
					{
						this->clear();
						return GAIA::False;
					}
					if(p[nCurrent] != ' ')
					{
						this->clear();
						return GAIA::False;
					}
					nCurrent++;
					if(nCurrent == nSize)
					{
						this->clear();
						return GAIA::False;
					}

					// Get GAIA::TYPEID.
					GAIA::TYPEID type_id = GAIA::TYPEID_INVALID;
					for(GAIA::N32 x = 1; x < sizeof(GAIA::TYPEID_ANAME) / sizeof(GAIA::TYPEID_ANAME[0]); ++x)
					{
						if(GAIA::ALGO::gstrcmp(GAIA::TYPEID_ANAME[x], szTypeName) == 0)
						{
							type_id = (GAIA::TYPEID)x;
							break;
						}
					}
					if(type_id == GAIA::TYPEID_INVALID)
					{
						this->clear();
						return GAIA::False;
					}

				#define MSG_FROMSTRING_BASETYPE(type) \
					{\
						type v = GAIA::ALGO::acasts(p + nCurrent);\
											while(p[nCurrent] != ' ' && nCurrent != nSize)\
							++nCurrent;\
						this->push(v);\
						if(nCurrent != nSize)\
							++nCurrent;\
					}

				#define MSG_FROMSTRING_STRINGTYPE \
					if(p[nCurrent] != '\"')\
					{\
						this->clear();\
						return GAIA::False;\
					}\
					nCurrent++;\
					if(nCurrent == nSize)\
					{\
						this->clear();\
						return GAIA::False;\
					}\
					GAIA::N32 nStartPos = nCurrent;\
					while(p[nCurrent] != '\"')\
					{\
						if(nCurrent == nSize)\
											{\
							this->clear();\
							return GAIA::False;\
											}\
						++nCurrent;\
						if(nCurrent == nSize)\
						{\
							this->clear();\
							return GAIA::False;\
						}\
					}\
					GAIA::N32 nStringLen = nCurrent - nStartPos;\
					if(nCurrent != nSize)\
						++nCurrent;\
					if(nCurrent != nSize)\
					{\
						if(p[nCurrent] != ' ')\
						{\
							this->clear();\
							return GAIA::False;\
						}\
						++nCurrent;\
					}

					//
					switch(type_id)
					{
					case GAIA::TYPEID_NM:
						MSG_FROMSTRING_BASETYPE(GAIA::NM)
							break;
					case GAIA::TYPEID_UM:
						MSG_FROMSTRING_BASETYPE(GAIA::UM)
							break;
					case GAIA::TYPEID_BL:
						MSG_FROMSTRING_BASETYPE(GAIA::BL)
							break;
					case GAIA::TYPEID_N8:
						MSG_FROMSTRING_BASETYPE(GAIA::N8)
							break;
					case GAIA::TYPEID_U8:
						MSG_FROMSTRING_BASETYPE(GAIA::U8)
							break;
					case GAIA::TYPEID_N16:
						MSG_FROMSTRING_BASETYPE(GAIA::N16)
							break;
					case GAIA::TYPEID_U16:
						MSG_FROMSTRING_BASETYPE(GAIA::U16)
							break;
					case GAIA::TYPEID_N32:
						MSG_FROMSTRING_BASETYPE(GAIA::N32)
							break;
					case GAIA::TYPEID_U32:
						MSG_FROMSTRING_BASETYPE(GAIA::U32)
							break;
					case GAIA::TYPEID_N64:
						MSG_FROMSTRING_BASETYPE(GAIA::N64)
							break;
					case GAIA::TYPEID_U64:
						MSG_FROMSTRING_BASETYPE(GAIA::U64)
							break;
					case GAIA::TYPEID_X128:
						MSG_FROMSTRING_BASETYPE(GAIA::X128)
							break;
					case GAIA::TYPEID_F32:
						MSG_FROMSTRING_BASETYPE(GAIA::F32)
							break;
					case GAIA::TYPEID_F64:
						MSG_FROMSTRING_BASETYPE(GAIA::F64)
							break;
					case GAIA::TYPEID_CHARPOINTER:
					case GAIA::TYPEID_CONSTCHARPOINTER:
						{
							MSG_FROMSTRING_STRINGTYPE
						#if GAIA_COMPILER == GAIA_COMPILER_VC
						#	pragma warning(disable : 4127)
						#endif
								if(sizeof(_ParamDataType) != sizeof(GAIA::N8))
						#if GAIA_COMPILER == GAIA_COMPILER_VC
						#	pragma warning(default : 4127)
						#endif
								{
									GAIA::N8* pNew = gnew GAIA::N8[nStringLen + 1];
									GAIA::ALGO::gstrcpy(pNew, p + nStartPos, nStringLen);
									if(GAIA::ALGO::gstrcmp(pNew, "(NullStringPtr)") == 0)
										this->push((const GAIA::N8*)GNIL);
									else
										this->push((const GAIA::N8*)pNew);
									gdel[] pNew;
								}
								else
									this->push(p + nStartPos, nStringLen);
						}
						break;
					case GAIA::TYPEID_WCHARPOINTER:
					case GAIA::TYPEID_CONSTWCHARPOINTER:
						{
							MSG_FROMSTRING_STRINGTYPE
						#if GAIA_COMPILER == GAIA_COMPILER_VC
						#	pragma warning(disable : 4127)
						#endif
								if(sizeof(_ParamDataType) != sizeof(GAIA::WCH))
						#if GAIA_COMPILER == GAIA_COMPILER_VC
						#	pragma warning(default : 4127)
						#endif
								{
									GAIA::WCH* pNew = gnew GAIA::WCH[nStringLen + 1];
									GAIA::ALGO::gstrcpy(pNew, p + nStartPos, nStringLen);
									if(GAIA::ALGO::gstrcmp(pNew, "(NullStringPtr)") == 0)
										this->push((const GAIA::N8*)GNIL);
									else
										this->push((const GAIA::N8*)pNew);
									gdel[] pNew;
								}
								else
									this->push(p + nStartPos, nStringLen);
						}
						break;
					case GAIA::TYPEID_POINTER:
					case GAIA::TYPEID_CONSTPOINTER:
						{
							GAIA::N32 v = GAIA::ALGO::acasts(p + nCurrent);
							while(p[nCurrent] != ' ' && nCurrent != nSize)
								++nCurrent;
							if(nCurrent == nSize)
							{
								this->clear();
								return GAIA::False;
							}
							nCurrent++;
							if(nCurrent == nSize)
							{
								this->clear();
								return GAIA::False;
							}
							GAIA::N32 nStartPos = nCurrent;
							while(p[nCurrent] != ' ' && nCurrent != nSize)
								++nCurrent;
							GAIA::N32 nLength = nCurrent - nStartPos;
							if(nCurrent != nSize)
								++nCurrent;
							if(nLength != v * 2 && v != 0)
							{
								this->clear();
								return GAIA::False;
							}
							if(nLength == 0)
							{
								this->push((const GAIA::GVOID*)GNIL, 0);
							}
							else
							{
								if(v == 0)
								{
									GAST(GAIA::ALGO::gstrmch(p + nStartPos, "(NullPtr)") != GNIL);
									this->push((const GAIA::GVOID*)GNIL, 0);
								}
								else
								{
									GAIA::U8* pTemp = gnew GAIA::U8[v];
									GAIA::ALGO::str2hex(p + nStartPos, v, pTemp);
									this->push((const GAIA::GVOID*)pTemp, v);
									gdel[] pTemp;
								}
							}
						}
						break;
					default:
						this->clear();
						return GAIA::False;
					}
				}
				return GAIA::True;
			}
			template<typename _ParamDataType> __MyType& operator += (const __MyType& src)
			{
				this->resize_keep(this->size() + src.size());
				GAIA::ALGO::gmemcpy((GAIA::U8*)this->fptr() - src.size(), src.fptr(), src.size());
				return *this;
			}
			GINL GAIA::N32 compare(const __MyType& src) const
			{
				if(m_nSize < src.m_nSize)
					return -1;
				else if(m_nSize > src.m_nSize)
					return +1;
				if(m_nSize == 0)
					return 0;
				return GAIA::ALGO::gmemcmp(m_p, src.m_p, m_nSize);
			}
			GCLASS_COMPARE_BYCOMPARE(__MyType)
		private:
			GINL GAIA::GVOID init()
			{
				m_p = GNIL;
				m_nSize = 0;
				m_nCapacity = 0;
				m_nCursor = 0;
				m_bHeapOwner = GAIA::True;
			}
		private:
			GAIA::U8* m_p;
			GAIA::N32 m_nSize;
			GAIA::N32 m_nCapacity;
			GAIA::N32 m_nCursor;
			GAIA::U8 m_bHeapOwner : 1;
		};
		template<typename _SizeType> template<typename _ParamDataType> BasicMsgPopper<_SizeType>::operator _ParamDataType()
		{
			if(m_pMsg == GNIL)
				GTHROW(Illegal);
			_ParamDataType t = 0;
			GAIA::U8 type_id = (GAIA::U8)datatotype(t);
			if(type_id == GAIA::TYPEID_INVALID)
				GTHROW(InvalidParam);
			if(type_id == GAIA::TYPEID_POINTER || type_id == GAIA::TYPEID_CONSTPOINTER)
				GTHROW(InvalidParam);
			if(type_id == GAIA::TYPEID_CHARPOINTER || type_id == GAIA::TYPEID_CONSTCHARPOINTER)
				GTHROW(InvalidParam);
			if(type_id == GAIA::TYPEID_WCHARPOINTER || type_id == GAIA::TYPEID_CONSTWCHARPOINTER)
				GTHROW(InvalidParam);
			if(sizeof(GAIA::U8) + sizeof(t) > m_pMsg->m_nSize - m_pMsg->m_nCursor)
				GTHROW(MemOverflow);
			if(m_pMsg->m_p[m_pMsg->m_nCursor] != type_id)
				GTHROW(Convert);
			const _ParamDataType& ret = *(_ParamDataType*)&m_pMsg->m_p[m_pMsg->m_nCursor + sizeof(GAIA::U8)];
			m_pMsg->m_nCursor += sizeof(GAIA::U8) + sizeof(t);
			return ret;
		}
		template<typename _SizeType> template<typename _ParamDataType> BasicMsgPopper<_SizeType>::operator const _ParamDataType&() const
		{
			if(m_pMsg == GNIL)
				GTHROW(Illegal);
			_ParamDataType t = 0;
			GAIA::U8 type_id = (GAIA::U8)datatotype(t);
			if(type_id == GAIA::TYPEID_INVALID)
				GTHROW(InvalidParam);
			if(type_id == GAIA::TYPEID_POINTER || type_id == GAIA::TYPEID_CONSTPOINTER)
				GTHROW(InvalidParam);
			if(type_id == GAIA::TYPEID_CHARPOINTER || type_id == GAIA::TYPEID_CONSTCHARPOINTER)
				GTHROW(InvalidParam);
			if(type_id == GAIA::TYPEID_WCHARPOINTER || type_id == GAIA::TYPEID_CONSTWCHARPOINTER)
				GTHROW(InvalidParam);
			if(sizeof(GAIA::U8) + sizeof(t) > m_pMsg->m_nSize - m_pMsg->m_nCursor)
				GTHROW(MemOverflow);
			if(m_pMsg->m_p[m_pMsg->m_nCursor] != type_id)
				GTHROW(Convert);
			const _ParamDataType& ret = *(_ParamDataType*)&m_pMsg->m_p[m_pMsg->m_nCursor + sizeof(GAIA::U8)];
			m_pMsg->m_nCursor += sizeof(GAIA::U8) + sizeof(t);
			return ret;
		}
		template<typename _SizeType> template<typename _ParamDataType> BasicMsgPopper<_SizeType>::operator const _ParamDataType*()
		{
			if(m_pMsg == GNIL)
				GTHROW(Illegal);
			const _ParamDataType* ret = GNIL;
			GAIA::U8 type_id = (GAIA::U8)datatotype(ret);
			if(type_id != GAIA::TYPEID_CHARPOINTER && type_id != GAIA::TYPEID_CONSTCHARPOINTER &&
				type_id != GAIA::TYPEID_WCHARPOINTER && type_id != GAIA::TYPEID_CONSTWCHARPOINTER)
				GTHROW(InvalidParam);
			if(m_pMsg->m_nSize - m_pMsg->m_nCursor <= sizeof(GAIA::U8) + sizeof(GAIA::U16))
				GTHROW(MemOverflow);
			if(m_pMsg->m_p[m_pMsg->m_nCursor] != type_id)
				GTHROW(Convert);
			GAIA::U16 u16Length = *(GAIA::U16*)(m_pMsg->m_p + m_pMsg->m_nCursor + sizeof(GAIA::U8));
			if(u16Length == 0)
				ret = GNIL;
			else
				ret = (const _ParamDataType*)(m_pMsg->m_p + m_pMsg->m_nCursor + sizeof(GAIA::U8) + sizeof(GAIA::U16));
			m_pMsg->m_nCursor += sizeof(GAIA::U8) + sizeof(GAIA::U16) + u16Length * sizeof(_ParamDataType);
			return ret;
		}
		class Msg : public BasicMsg<GAIA::NUM>{public:};
	}
}

#endif
