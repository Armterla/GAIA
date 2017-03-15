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
		static const GAIA::U8 ascii_json_validchar[128] =
		{
			/* 1-8 */
			0	, //	'\0'	NUL(null)
			0	, //			SOH(start of headline)
			0	, //			STX(start of text)
			0	, //			ETX(end of text)
			0	, //			EOT(end of transmission)
			0	, //			ENQ(enquiry)
			0	, //			ACK(acknowledge)
			0	, //			BEL(bell)

			/* 9-16 */
			0	, //			BS(backspace)
			0	, //			HT(horizontal tab)
			0	, //	'\n'	LF(NL line feed, new line)
			0	, //			VT(vertical tab)
			0	, //			FF(NP form feed, new page)
			0	, //	'\r'	CR(carriage return)
			0	, //			SO(shift out)
			0	, //			SI(shift in)

			/* 17-24 */
			0	, //			DLE(data link escape)
			0	, //			DC1(device control 1)
			0	, //			DC2(device control 2)
			0	, //			DC3(device control 3)
			0	, //			DC4(device control 4)
			0	, //			NAK(negative acknowledge)
			0	, //			SYN(synchronous idle)
			0	, //			ETB(end of trans. block)

			/* 25-32 */
			0	, //			CAN(cancel)
			0	, //			EM(end of medium)
			0	, //			SUB(substitute)
			0	, //			ESC(escape)
			0	, //			FS(file separator)
			0	, //			GS(group separator)
			0	, //			RS(record separator)
			0	, //			US(unit separator)

			/* 33-40 */
			0	, //	' '		(space)
			1	, //	'!'		CH
			1	, //	'\"'	CH
			1	, //	'#'		CH
			1	, //	'$'		CH
			1	, //	'%'		CH
			1	, //	'&'		CH
			1	, //	'\''	CH

			/* 41-48 */
			1	, //	'('		CH
			1	, //	')'		CH
			1	, //	'*'		CH
			1	, //	'+'		CH
			0	, //	','		CH
			1	, //	'-'		CH
			1	, //	'.'		CH
			1	, //	'/'		CH

			/* 49-56 */
			1	, //	'0'		CH
			1	, //	'1'		CH
			1	, //	'2'		CH
			1	, //	'3'		CH
			1	, //	'4'		CH
			1	, //	'5'		CH
			1	, //	'6'		CH
			1	, //	'7'		CH

			/* 57-64 */
			1	, //	'8'		CH
			1	, //	'9'		CH
			1	, //	':'		CH
			1	, //	';'		CH
			1	, //	'<'		CH
			1	, //	'='		CH
			1	, //	'>'		CH
			1	, //	'?'		CH

			/* 65-72 */
			1	, //	'@'		CH
			1	, //	'A'		CH
			1	, //	'B'		CH
			1	, //	'C'		CH
			1	, //	'D'		CH
			1	, //	'E'		CH
			1	, //	'F'		CH
			1	, //	'G'		CH

			/* 73-80 */
			1	, //	'H'		CH
			1	, //	'I'		CH
			1	, //	'J'		CH
			1	, //	'K'		CH
			1	, //	'L'		CH
			1	, //	'M'		CH
			1	, //	'N'		CH
			1	, //	'O'		CH

			/* 81-88 */
			1	, //	'P'		CH
			1	, //	'Q'		CH
			1	, //	'R'		CH
			1	, //	'S'		CH
			1	, //	'T'		CH
			1	, //	'U'		CH
			1	, //	'V'		CH
			1	, //	'W'		CH

			/* 89-96 */
			1	, //	'X'		CH
			1	, //	'Y'		CH
			1	, //	'Z'		CH
			1	, //	'['		CH
			1	, //	'\\'	CH
			1	, //	']'		CH
			1	, //	'^'		CH
			1	, //	'_'		CH

			/* 97-104 */
			1	, //	'`'		CH
			1	, //	'a'		CH
			1	, //	'b'		CH
			1	, //	'c'		CH
			1	, //	'd'		CH
			1	, //	'e'		CH
			1	, //	'f'		CH
			1	, //	'g'		CH

			/* 105-112 */
			1	, //	'h'		CH
			1	, //	'i'		CH
			1	, //	'j'		CH
			1	, //	'k'		CH
			1	, //	'l'		CH
			1	, //	'm'		CH
			1	, //	'n'		CH
			1	, //	'o'		CH

			/* 113-120 */
			1	, //	'p'		CH
			1	, //	'q'		CH
			1	, //	'r'		CH
			1	, //	's'		CH
			1	, //	't'		CH
			1	, //	'u'		CH
			1	, //	'v'		CH
			1	, //	'w'		CH

			/* 121-128 */
			1	, //	'x'		CH
			1	, //	'y'		CH
			1	, //	'z'		CH
			1	, //	'{'		CH
			1	, //	'|'		CH
			1	, //	'}'		CH
			1	, //	'~'		CH
			0	, //			DEL(delete)
		};

		/*!
			@brief Basic json reader.
				It used for high performance json streamed read.
		*/
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
				@brief Try to peek next node.

				@param nt [out] Used for saving the node type.

				@param nodenamelen [out] Used for saving the node name's length in characters.

				@param pNext [out] Used for saving the next read location(read pointer).\n
					This parameter could be GNIL means ignored.\n

				@param pValueIsString [out] Used for saving the value node's string type.\n
					If value is a string value, it will be filled by GAIA::True, or will be filled by GAIA::False.\n
					If the next node is not a value node, this parameter is be ignored.\n
					This parameter could be GNIL means ignored.\n

				@return Return the node's name with out '\0'.

				@exception GAIA::ECT::EctIllegal
					If can't read a node, throw it.

				@exception GAIA::ECT::EctDataError
					If the source json's format exist error, and can't peek, throw it.
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
				case '\"':
					{
						const _DataType* pLastQuote = GNIL;
						const _DataType* pColon = GNIL;
						GAIA::BL bInQuote = GAIA::False;
						do
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
						while(p <= m_pBack);
						if(p > m_pBack)
							GTHROW_RET(DataError, GNIL);
						if(pColon == GNIL)
							GTHROW_RET(DataError, GNIL);
						pLocalNext = this->move_to_next(pColon + 1);
						if(pLocalNext == GNIL)
							GTHROW_RET(DataError, GNIL);
						switch(*pLocalNext)
						{
						case '\"':
							{
								nt = GAIA::JSON::JSON_NODE_NAME;
								pLocalNext = pColon;
							}
							break;
						case '{':
							{
								nt = GAIA::JSON::JSON_NODE_CONTAINER;
								pLocalNext = pLocalNext + 1;
							}
							break;
						case '[':
							{
								nt = GAIA::JSON::JSON_NODE_MULTICONTAINER;
								pLocalNext = pLocalNext + 1;
							}
							break;
						default:
							{
								if(*pLocalNext >= '0' && *pLocalNext <= '9' ||
								   *pLocalNext == 't' ||
								   *pLocalNext == 'f')
								{
									nt = GAIA::JSON::JSON_NODE_NAME;
									pLocalNext = pColon;
									break;
								}
								else
									GTHROW_RET(DataError, GNIL);
							}
							break;
						}
						nodenamelen = (_SizeType)(pLastQuote - m_pCursor - 1);
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
							do
							{
								if(*pLocalNext == '\"' && pLocalNext > p && pLocalNext[-1] != '\\')
									break;
								++pLocalNext;
							}
							while(pLocalNext <= m_pBack);
							if(pLocalNext > m_pBack)
								GTHROW_RET(DataError, GNIL);
							p++;
							bValueIsString = GAIA::True;
						}
						else if(*pLocalNext >= '0' && *pLocalNext <= '9')
						{
							do
							{
								if((*pLocalNext >= '0' && *pLocalNext <= '9') || *pLocalNext == '.')
									++pLocalNext;
								else
									break;
							}
							while(pLocalNext <= m_pBack);
							if(pLocalNext > m_pBack)
								GTHROW_RET(DataError, GNIL);
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
				@brief Try to read a container node or a multi container node.

				@param nt [out] Used for saving the container node type, would be JSON_NODE_CONTAINER or JSON_NODE_MULTICONTAINER.

				@param nodenamelen [out] Used for saving container node's name's length in characters.

				@return Return the container's name without '\0'.\n
					If current container node have not a name, it will return "{" or "[", in this case parameter nodenamelen will be filled by zero.\n
					If current container node have a name, it will return the name, and parameter nodenamelen will filled by length in characters.\n

				@exception GAIA::ECT::EctIllegal
					If can't read a container node, throw it.

				@exception GAIA::ECT::EctDataError
					If the source json's format exist error, and can't read, throw it.
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
					GAST(nodenamelen >= 0);
					break;
				case GAIA::JSON::JSON_NODE_MULTICONTAINER:
					{
						GAST(nodenamelen == 0);
						if(*pRet != '[')
							GTHROW_RET(Illegal, GNIL);
					}
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				m_pCursor = pNext;
				return pRet;
			}

			/*!
				@brief Try to end read a container node or a multi container node.

				@return Return the container's name without '\0'.\n
					If current container is a single container, it will return "}" without '\0'.\n
					If current container is a multi container, it will return "]" without '\0'.\n
					Even if current container have a container name string, current function can't return the name string,
					so current function could return "}" or "]" with out '\0' only.

				@exception GAIA::ECT::EctIllegal
					If can't end read a container node, throw it.

				@exception GAIA::ECT::EctDataError
					If the source json's format exist error, and can't read, throw it.
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
				@brief Try to read a name node or a value node.

				@param nt [out] Used for saving the name node or the value node's type, would be JSON_NODE_NAME or JSON_NODE_VALUE.

				@param nodenamelen [out] Used for saving the name node or the value node's name's length in characters.

				@param pValueIsString [out] Used for saving the value is a string or not.\n
					If node is not a value node, this parameter is ignored.\n
					This parameter could be GNIL means ignored.\n

				@return Return the name node's name or the value node's name without '\0'.

				@exception GAIA::ECT::EctIllegal
					If can't read a name node or a value node's name, throw it.

				@exception GAIA::ECT::EctDataError
					If the source json's format exist error, and can't read, throw it.
			*/
			GINL const _DataType* Read(GAIA::JSON::JSON_NODE& nt, _SizeType& nodenamelen, GAIA::BL* pValueIsString = GNIL)
			{
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext, pValueIsString);
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

			/*!
				@brief Read a name node's name.

				@param p [out] Used for saving name node's name.

				@param maxlen [in] Specify the parameter p's buffer size in characters.

				@return If success return parameter p.

				@exception GAIA::ECT::EctInvalidParam
					If parameter p is GNIL or (maxlen != GINVALID and maxlen below equal zero), throw it.

				@exception GAIA::ECT::EctIllegal
					If can't read a name node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a name node, throw it.

				@exception GAIA::ECT::EctBufNotEnough
					If parameter maxlen is not GINVALID and buffer is not enough for save the name node's name, throw it.
			*/
			template<typename _ParamDataType> _ParamDataType* ReadName(_ParamDataType* p, _SizeType maxlen = GINVALID)
			{
				if(p == GNIL)
					GTHROW_RET(InvalidParam, GNIL);
				if(maxlen != GINVALID && maxlen <= 0)
					GTHROW_RET(InvalidParam, GNIL);
				GAIA::JSON::JSON_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_NAME:
					{
						if(maxlen != GINVALID && nodenamelen >= maxlen)
							GTHROW_RET(BufNotEnough, GNIL);
						GAIA::ALGO::gstrcpy(p, pRet, nodenamelen);
					}
					break;
				case GAIA::JSON::JSON_NODE_VALUE:
					GTHROW_RET(Illegal, GNIL);
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				GAST(nodenamelen != 0);
				m_pCursor = pNext;
				return p;
			}

			/*!
				@brief Read a value node's name.

				@param p [out] Used for saving value node's name.

				@param maxlen [in] Specify the parameter p's buffer size in characters.

				@return If success return parameter p.

				@exception GAIA::ECT::EctInvalidParam
					If parameter p is GNIL or (maxlen != GINVALID and maxlen below equal zero), throw it.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctBufNotEnough
					If parameter maxlen is not GINVALID and buffer is not enough for save the value node's name, throw it.
			*/
			template<typename _ParamDataType> _ParamDataType* ReadValue(_ParamDataType* p, _SizeType maxlen = GINVALID)
			{
				if(p == GNIL)
					GTHROW_RET(InvalidParam, GNIL);
				if(maxlen != GINVALID && maxlen <= 0)
					GTHROW_RET(InvalidParam, GNIL);
				GAIA::JSON::JSON_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_NAME:
					GTHROW_RET(Illegal, GNIL);
				case GAIA::JSON::JSON_NODE_VALUE:
					{
						if(maxlen != GINVALID && nodenamelen >= maxlen)
							GTHROW_RET(BufNotEnough, GNIL);
						GAIA::ALGO::gstrcpy(p, pRet, nodenamelen);
					}
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				GAST(nodenamelen != 0);
				m_pCursor = pNext;
				return p;
			}

			/*!
				@brief Read GAIA::BL value node.

				@param v [out] Used for saving the GAIA::BL value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::BL& ReadValue(GAIA::BL& v)
			{
				GAIA::JSON::JSON_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_NAME:
					GTHROW_RET(Illegal, GNIL);
				case GAIA::JSON::JSON_NODE_VALUE:
					{
						if(GAIA::ALGO::gstrcmp(pRet, "true", nodenamelen) == 0)
							v = GAIA::True;
						else if(GAIA::ALGO::gstrcmp(pRet, "false", nodenamelen) == 0)
							v = GAIA::False;
						else
						{
							v = GAIA::False;
							GTHROW_RET(Convert, v);
						}
					}
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				GAST(nodenamelen != 0);
				m_pCursor = pNext;
				return v;
			}

			/*!
				@brief Read a GAIA::X128 value node.

				@param v [out] Used for saving the GAIA::X128 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::X128& ReadValue(GAIA::X128& v)
			{
				GAIA::JSON::JSON_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_NAME:
					GTHROW_RET(Illegal, GNIL);
				case GAIA::JSON::JSON_NODE_VALUE:
					{
						if(nodenamelen != 32)
							GTHROW_RET(Convert, GNIL);
						GAIA::CH sz[33];
						GAIA::ALGO::gstrcpy(sz, pRet, nodenamelen);
						if(!v.check(sz))
							GTHROW_RET(Convert, GNIL);
						v.fromstring(sz);
					}
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				GAST(nodenamelen != 0);
				m_pCursor = pNext;
				return v;
			}

			/*!
				@brief Read a GAIA::U8 value node.

				@param v [out] Used for saving the GAIA::U8 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::U8& ReadValue(GAIA::U8& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::N8 value node.

				@param v [out] Used for saving the GAIA::N8 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::N8& ReadValue(GAIA::N8& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::U16 value node.

				@param v [out] Used for saving the GAIA::U16 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::U16& ReadValue(GAIA::U16& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::N16 value node.

				@param v [out] Used for saving the GAIA::N16 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::N16& ReadValue(GAIA::N16& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::U32 value node.

				@param v [out] Used for saving the GAIA::U32 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::U32& ReadValue(GAIA::U32& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::N32 value node.

				@param v [out] Used for saving the GAIA::N32 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::N32& ReadValue(GAIA::N32& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::U64 value node.

				@param v [out] Used for saving the GAIA::U64 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::U64& ReadValue(GAIA::U64& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::N64 value node.

				@param v [out] Used for saving the GAIA::N64 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::N64& ReadValue(GAIA::N64& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::F32 value node.

				@param v [out] Used for saving the GAIA::F32 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::F32& ReadValue(GAIA::F32& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

			/*!
				@brief Read a GAIA::U64 value node.

				@param v [out] Used for saving the GAIA::F64 value.

				@exception GAIA::ECT::EctIllegal
					If can't read a value node's name, throw it.

				@exception GAIA::ECT::EctIllegal
					If the node is not a value node, throw it.

				@exception GAIA::ECT::EctConvert
					If the string can't convert to type of parameter v, throw it.
			*/
			GINL GAIA::F64& ReadValue(GAIA::F64& v){GAIA::CH szValue[64]; this->ReadValue(szValue, sizeof(szValue)); v = GAIA::ALGO::acasts(szValue); return v;}

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
					if(*p >= sizeof(ascii_json_validchar) || ascii_json_validchar[*p])
						return p;
					++p;
				}
				GTHROW_RET(DataError, GNIL);
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