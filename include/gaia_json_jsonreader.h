#ifndef 	__GAIA_JSON_JSONREADER_H__
#define 	__GAIA_JSON_JSONREADER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_json_jsonbase.h"

namespace GAIA
{
	namespace JSON
	{
		template<typename _DataType, typename _SizeType, typename _DepthType, _DepthType _MaxDepth> class BasicJsonReader : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _DepthType _depthtype;
			typedef const _DataType* _DataTypePtr;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicJsonReader<_DataType, _SizeType, _DepthType, _MaxDepth> __MyType;

		public:
			/*!
				@brief Constructor.
			*/
			GINL BasicJsonReader(){this->init();}

			/*!
				@brief Destructor.
			*/
			GINL ~BasicJsonReader(){}

			/*!
				@brief Reset current JsonReader to default state(LIKE A NEW OBJECT).
			*/
			GINL GAIA::GVOID Reset(){this->init();}

			/*!
				@brief Set or unset a buffer for JsonReader read from.

				@param p [in] Specify a loaded json buffer.
					If GNIL, means unset the old buffer.

				@param size [in] Specify parameter p's size in bytes.
					If parameter p is GNIL, parameter size must be 0, or must above zero.
			*/
			GINL GAIA::GVOID SetBuffer(const GAIA::GVOID* p, _SizeType size)
			{
				if(p == GNIL)
				{
					GAST(size == 0);
					m_pFront = m_pBack = m_pCursor = GNIL;
					m_size = 0;
				}
				else
				{
					GAST(size > sizeof(_DataType));
					m_pFront = (const _DataType*)p;
					m_pBack = m_pFront + (size / sizeof(_DataType) - 1);
					m_pCursor = m_pFront;
					m_size = size;
				}
			}

			/*!
				@brief Get the buffer which used for JsonReader read from.

				@param size [out] Used for saving the size of buffer(return) in bytes.

				@return Return the buffer which had be set.
					If not call SetBuffer with a buffer pointer, will return GNIL, and parameter size be filled by 0.
			*/
			GINL const GAIA::GVOID* GetBuffer(_SizeType& size) const{size = m_size; return m_pFront;}

			/*!
				@brief Get the buffer's size in bytes which had be set to current JsonReader.

				@return Return the buffer's size in bytes.
			*/
			GINL _SizeType GetBufferSize() const{return m_size;}

			/*!
				@brief Get the buffer's readed size in bytes.

				@return Return the buffer's size in bytes.
			*/
			GINL _SizeType GetReadSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}

			/*!
				@brief Get the buffer's remain(not be readed) size in bytes.

				@return Return the buffer's remain size in bytes.
			*/
			GINL _SizeType GetRemainSize() const{return this->GetBufferSize() - this->GetReadSize();}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL const _DataType* Peek(GAIA::JSON::JSON_NODE& nt, _SizeType& nodenamelen, _DataTypePtr* pNext = GNIL, GAIA::BL* pValueIsString = GNIL)
			{
				m_pCursor = this->move_to_next(m_pCursor);
				if(m_pCursor == GNIL)
					GTHROW_RET(Illegal, GNIL);
				const _DataType* pLocalNext = GNIL;
				const _DataType* p = m_pCursor;
				const _DataType* pRet = GNIL;
				switch(*p)
				{
				case '{':
					{
						nt = GAIA::JSON::JSON_NODE_CONTAINER;
						nodenamelen = 0;
						pLocalNext = p + 1;
						pRet = p;
					}
					break;
				case '[':
					{
						nt = GAIA::JSON::JSON_NODE_MULTICONTAINER;
						nodenamelen = 0;
						pLocalNext = p + 1;
						pRet = p;
					}
					break;
				case '\"':
					{
						const _DataType* pLastQuote = GNIL;
						const _DataType* pColon = GNIL;
						GAIA::BL bInQuote = GAIA::False;
						while(p <= m_pBack)
						{
							if(*p == ':' && !bInQuote)
							{
								pColon = p;
								break;
							}
							else if(*p == '\"' && (p == m_pCursor || p[-1] != '\\'))
							{
								if(bInQuote)
									pLastQuote = p;
								bInQuote = !bInQuote;
							}
							++p;
						}
						if(pColon == GNIL)
							GTHROW_RET(DataError, GNIL);
						pLocalNext = this->move_to_next(pColon + 1);
						if(pLocalNext == GNIL)
							GTHROW_RET(DataError, GNIL);
						switch(*pLocalNext)
						{
						case '{':
							nt = GAIA::JSON::JSON_NODE_CONTAINER;
							break;
						case '[':
							nt = GAIA::JSON::JSON_NODE_MULTICONTAINER;
							break;
						case '\"':
							nt = GAIA::JSON::JSON_NODE_NAME;
							break;
						default:
							{
								if(*pLocalNext >= '0' && *pLocalNext <= '9' ||
								   *pLocalNext == 't' ||
								   *pLocalNext == 'f')
								{
									nt = GAIA::JSON::JSON_NODE_NAME;
									break;
								}
								else
									GTHROW_RET(DataError, GNIL);
							}
						}
						nodenamelen = (_SizeType)(pLastQuote - m_pCursor - 1);
						pLocalNext = pColon;
						pRet = m_pCursor + 1;
					}
					break;
				case ':':
					{
						nt = GAIA::JSON::JSON_NODE_VALUE;
						pLocalNext = p = this->move_to_next(p + 1);
						if(pLocalNext == GNIL)
							GTHROW_RET(DataError, GNIL);
						GAIA::BL bValueIsString = GAIA::True;
						if(*pLocalNext == '\"')
						{
							while(pLocalNext <= m_pBack)
							{
								if(*pLocalNext == '\"' && pLocalNext > p && pLocalNext[-1] != '\\')
									break;
								++pLocalNext;
							}
							p++;
							bValueIsString = GAIA::True;
						}
						else if(*pLocalNext >= '0' && *pLocalNext <= '9')
						{
							while(pLocalNext <= m_pBack)
							{
								if((*pLocalNext >= '0' && *pLocalNext <= '9') || *pLocalNext == '.')
									++pLocalNext;
								else
									break;
							}
							GAIA::BL bExistValidNumber = GAIA::False;
							_SizeType dotcnt = 0;
							_SizeType zerocntbefore = 0;
							for(const _DataType* pTemp = p; pTemp < pLocalNext; ++pTemp)
							{
								if(*pTemp >= '0' && *pTemp <= '9')
								{
									if(dotcnt == 0 && zerocntbefore != 0)
										GTHROW_RET(DataError, GNIL);
									bExistValidNumber = GAIA::True;
								}
								else if(*pTemp == '0' && !bExistValidNumber && dotcnt == 0)
								{
									if(++zerocntbefore > 1)
										GTHROW_RET(DataError, GNIL);
								}
								else if(*pTemp == '.')
								{
									dotcnt++;
									if(dotcnt > 1)
										GTHROW_RET(DataError, GNIL);
								}
							}
							bValueIsString = GAIA::False;
						}
						else if(*pLocalNext == 't')
						{
							if(pLocalNext + 3 <= m_pBack && pLocalNext[1] == 'r' && pLocalNext[2] == 'u' && pLocalNext[3] == 'e')
								pLocalNext = pLocalNext + 4;
							else
								GTHROW_RET(DataError, GNIL);
							bValueIsString = GAIA::False;
						}
						else if(*pLocalNext == 'f')
						{
							if(pLocalNext + 4 <= m_pBack && pLocalNext[1] == 'a' && pLocalNext[2] == 'l' && pLocalNext[3] == 's' && pLocalNext[4] == 'e')
								pLocalNext = pLocalNext + 5;
							else
								GTHROW_RET(DataError, GNIL);
							bValueIsString= GAIA::False;
						}
						else
							GTHROW_RET(DataError, GNIL);
						if(pLocalNext > m_pBack)
							GTHROW_RET(DataError, GNIL);
						nodenamelen = (_SizeType)(pLocalNext - p);
						if(bValueIsString)
							pLocalNext = pLocalNext + 1;
						if(pValueIsString != GNIL)
							*pValueIsString = bValueIsString;
						pRet = p;
					}
					break;
				case '}':
					{
						nt = GAIA::JSON::JSON_NODE_CONTAINER;
						nodenamelen = 0;
						pLocalNext = p + 1;
						pRet = p;
						break;
					}
					break;
				case ']':
					{
						nt = GAIA::JSON::JSON_NODE_MULTICONTAINER;
						nodenamelen = 0;
						pLocalNext = p + 1;
						pRet = p;
						break;
					}
					break;
				default:
					GTHROW_RET(DataError, GNIL);
				}
				if(pNext != GNIL)
					*pNext = pLocalNext;
				return pRet;
			}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL const _DataType* Begin(GAIA::JSON::JSON_NODE& nt, _SizeType& nodenamelen)
			{
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_CONTAINER:
					{
						if(*pRet != '{')
							GTHROW_RET(Illegal, GNIL);
					}
					break;
				case GAIA::JSON::JSON_NODE_MULTICONTAINER:
					{
						if(*pRet != '[')
							GTHROW_RET(Illegal, GNIL);
					}
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				GAST(nodenamelen == 0);
				m_pCursor = pNext;
				return pRet;
			}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL const _DataType* End()
			{
				GAIA::JSON::JSON_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_CONTAINER:
					{
						if(*pRet != '}')
							GTHROW_RET(Illegal, GNIL);
					}
					break;
				case GAIA::JSON::JSON_NODE_MULTICONTAINER:
					{
						if(*pRet != ']')
							GTHROW_RET(Illegal, GNIL);
					}
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				GAST(nodenamelen == 0);
				m_pCursor = pNext;
				return pRet;
			}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL const _DataType* Read(GAIA::JSON::JSON_NODE& nt, _SizeType& nodenamelen)
			{
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_NAME:
					break;
				case GAIA::JSON::JSON_NODE_VALUE:
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				GAST(nodenamelen != 0);
				m_pCursor = pNext;
				return pRet;
			}

		private:
			GINL GAIA::GVOID init()
			{
				m_pFront = m_pBack = m_pCursor = GNIL;
				m_size = 0;
			}
			GINL const _DataType* move_to_next(const _DataType* p)
			{
				while(p <= m_pBack)
				{
					if(*p != ' ' &&
					   *p != '\t' &&
					   *p != '\r' &&
					   *p != '\n' &&
					   *p != ',')
						return p;
					++p;
				}
				return GNIL;
			}

		private:
			const _DataType* m_pFront;
			const _DataType* m_pBack; // Last valid character.
			const _DataType* m_pCursor;
			_SizeType m_size; // Size in bytes.
		};
		class JsonReaderA : public BasicJsonReader<GAIA::CH, GAIA::NUM, GAIA::NUM, 32>{public:};
		class JsonReaderW : public BasicJsonReader<GAIA::WCH, GAIA::NUM, GAIA::NUM, 32>{public:};
	}
}

#endif