#ifndef 	__GAIA_XML_XMLREADER_H__
#define 	__GAIA_XML_XMLREADER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_xml_xmlbase.h"

namespace GAIA
{
	namespace XML
	{
		/*!
			@brief Basic xml reader.
				It used for high performance xml streamed read.
		*/
		template<typename _DataType, typename _SizeType, typename _DepthType, _DepthType _MaxDepth> class BasicXmlReader : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _DepthType _depthtype;
			typedef const _DataType* _DataTypePtr;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicXmlReader<_DataType, _SizeType, _DepthType, _MaxDepth> __MyType;

		public:
			/*!
				@brief Constructor.
			*/
			GINL BasicXmlReader(){this->init();}

			/*!
				@brief Destructor.
			*/
			GINL ~BasicXmlReader(){}

			/*!
				@brief Reset current XmlReader to default state(LIKE A NEW OBJECT).
			*/
			GINL GAIA::GVOID Reset(){this->init();}

			/*!
				@brief Set or unset a buffer for XmlReader read from.

				@param p [in] Specify a loaded xml buffer.
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
				@brief Get the buffer which used for XmlReader read from.

				@param size [out] Used for saving the size of buffer(return) in bytes.

				@return Return the buffer which had be set.
					If not call SetBuffer with a buffer pointer, will return GNIL, and parameter size be filled by 0.
			*/
			GINL const GAIA::GVOID* GetBuffer(_SizeType& size) const{size = m_size; return m_pFront;}

			/*!
				@brief Get the buffer's size in bytes which had be set to current XmlReader.

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

				@param pNext [out] Used for saving the next read location(read pointer)

				@return Return the node's name with out '\0'.

				@exception GAIA::ECT::EctIllegal
					If can't read a node, throw it.

				@exception GAIA::ECT::EctDataError
					If the source xml's format exist error, and can't peek, throw it.
			*/
			GINL const _DataType* Peek(GAIA::XML::XML_NODE& nt, _SizeType& nodenamelen, _DataTypePtr* pNext = GNIL)
			{
				m_pCursor = this->move_to_next(m_pCursor);
				if(m_pCursor == GNIL)
					GTHROW_RET(Illegal, GNIL);
				const _DataType* pLocalNext = GNIL;
				const _DataType* p = m_pCursor;
				const _DataType* pRet = GNIL;
				switch(*p)
				{
				case '=': // Value.
					{
						pLocalNext = p = this->move_to_next(p + 1);
						if(pLocalNext == GNIL)
							GTHROW_RET(DataError, GNIL);
						if(*pLocalNext != '\"')
							GTHROW_RET(DataError, GNIL);
						do
						{
							if(*pLocalNext == '\"' && pLocalNext > p && pLocalNext[-1] != '\\')
								break;
							++pLocalNext;
						}
						while(pLocalNext <= m_pBack);
						if(pLocalNext > m_pBack)
							GTHROW_RET(DataError, GNIL);
						nt = GAIA::XML::XML_NODE_VALUE;
						nodenamelen = (_SizeType)(pLocalNext - p - 1);
						pLocalNext = pLocalNext + 1;
						pRet = p + 1;
					}
					break;
				case '<': // Container begin or multi container begin or multi container end.
					{
						if(p == m_pBack)
							GTHROW_RET(DataError, GNIL);
						++p;
						if(*p == '/') // Multi container end.
						{
							++p;
							if(!GAIA::ALGO::isalpha(*p))
								GTHROW_RET(DataError, GNIL);
							do
							{
								if(*p > sizeof(ascii_xml_validnamechar) || !ascii_xml_validnamechar[*p])
									break;
								++p;
							}
							while(p <= m_pBack);
							if(p > m_pBack)
								GTHROW_RET(DataError, GNIL);
							nt = GAIA::XML::XML_NODE_MULTICONTAINER;
							nodenamelen = (_SizeType)(p - m_pCursor - 2);
							pRet = m_pCursor + 2;

							p = this->move_to_next(p);
							if(*p != '>')
								GTHROW_RET(DataError, GNIL);
							pLocalNext = p + 1;
						}
						else // Container begin or multi container begin.
						{
							if(!GAIA::ALGO::isalpha(*p))
								GTHROW_RET(DataError, GNIL);
							do
							{
								if(*p > sizeof(ascii_xml_validnamechar) || !ascii_xml_validnamechar[*p])
									break;
								++p;
							}
							while(p <= m_pBack);
							if(p > m_pBack)
								GTHROW_RET(DataError, GNIL);
							nt = GAIA::XML::XML_NODE_CONTAINER;
							nodenamelen = (_SizeType)(p - m_pCursor - 1);
							pLocalNext = p;
							pRet = m_pCursor + 1;

							// THIS CODE WILL COST MORE PERFORMANCE, IF NOT NEED, COULD CANCEL IT.
							// Check is multi container.
							{
								GAIA::BL bInQuote = GAIA::False;
								GAIA::BL bCheckMultiContainerComplete = GAIA::False;
								do
								{
									switch(*p)
									{
									case '/':
										{
											if(!bInQuote)
											{
												nt = GAIA::XML::XML_NODE_CONTAINER;
												bCheckMultiContainerComplete = GAIA::True;
											}
										}
										break;
									case '>':
										{
											if(!bInQuote)
											{
												nt = GAIA::XML::XML_NODE_MULTICONTAINER;
												bCheckMultiContainerComplete = GAIA::True;
											}
										}
										break;
									case '\"':
										bInQuote = !bInQuote;
										break;
									default:
										break;
									}
									if(bCheckMultiContainerComplete)
										break;
									++p;
								}
								while(p <= m_pBack);
								if(p > m_pBack)
									GTHROW_RET(DataError, GNIL);
							}
							// END.
						}
					}
					break;
				case '>': // Multi container body begin.
					{
						m_pCursor = this->move_to_next(m_pCursor + 1);
						pRet = this->Peek(nt, nodenamelen, &pLocalNext);
					}
					break;
				case '/': // Container end.
					{
						if(p == m_pBack)
							GTHROW_RET(DataError, GNIL);
						if(*(p + 1) != '>')
							GTHROW_RET(DataError, GNIL);
						nt = GAIA::XML::XML_NODE_CONTAINER;
						nodenamelen = 0;
						pLocalNext = p + 2;
						pRet = p;
					}
					break;
				default: // Name.
					{
						if(!GAIA::ALGO::isalpha(*p))
							GTHROW_RET(DataError, GNIL);
						do
						{
							if(*p > sizeof(ascii_xml_validnamechar) || !ascii_xml_validnamechar[*p])
								break;
							++p;
						}
						while(p <= m_pBack);
						if(p > m_pBack)
							GTHROW_RET(DataError, GNIL);
						nt = GAIA::XML::XML_NODE_NAME;
						nodenamelen = (_SizeType)(p - m_pCursor);
						pLocalNext = p;
						pRet = m_pCursor;
					}
					break;
				}
				if(pNext != GNIL)
					*pNext = pLocalNext;
				return pRet;
			}

			/*!
				@brief Try to read a container node or a multi container node.

				@param nt [out] Used for saving the container node type, would be XML_NODE_CONTAINER or XML_NODE_MULTICONTAINER.

				@param nodenamelen [out] Used for saving container node's name's length in characters.

				@return Return the container's name without '\0'.\n
					If current container node have not a name, it will return "{" or "[", in this case parameter nodenamelen will be filled by zero.\n
					If current container node have a name, it will return the name, and parameter nodenamelen will filled by length in characters.\n

				@exception GAIA::ECT::EctIllegal
					If can't read a container node, throw it.

				@exception GAIA::ECT::EctDataError
					If the source xml's format exist error, and can't read, throw it.
			*/
			GINL const _DataType* Begin(GAIA::XML::XML_NODE& nt, _SizeType& nodenamelen)
			{
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_CONTAINER:
					break;
				case GAIA::XML::XML_NODE_MULTICONTAINER:
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				m_pCursor = pNext;
				return pRet;
			}

			/*!
				@brief Try to end read a container node or a multi container node.

				@param nt [out] Used for saving the json node type.\n
					Default value is GNIL means this parameter is ignored.\n
					If current function success, it would be XML_NODE_CONTAINER or XML_NODE_MULTICONTAINER.\n
					If current function failed, it would be XML_NODE_CONTAINER or XML_NODE_MULTICONTAINER or XML_NODE_NAME or XML_NODE_VALUE.\n

				@return Return the container's name without '\0'.\n
					If current container is a single container, it will return "}" without '\0'.\n
					If current container is a multi container, it will return "]" without '\0'.\n
					Even if current container have a container name string, current function can't return the name string,
					so current function could return "}" or "]" with out '\0' only.

				@exception GAIA::ECT::EctIllegal
					If can't end read a container node, throw it.

				@exception GAIA::ECT::EctDataError
					If the source xml's format exist error, and can't read, throw it.
			*/
			GINL const _DataType* End(GAIA::XML::XML_NODE* nt = GNIL)
			{
				GAIA::XML::XML_NODE nodetype;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nodetype, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				if(nt != GNIL)
					*nt = nodetype;
				switch(nodetype)
				{
				case GAIA::XML::XML_NODE_CONTAINER:
					GAST(nodenamelen == 0);
					break;
				case GAIA::XML::XML_NODE_MULTICONTAINER:
					break;
				default:
					GTHROW_RET(Illegal, GNIL);
				}
				m_pCursor = pNext;
				return pRet;
			}

			/*!
				@brief Try to read a name node or a value node.

				@param nt [out] Used for saving the name node or the value node's type, would be XML_NODE_NAME or XML_NODE_VALUE.

				@param nodenamelen [out] Used for saving the name node or the value node's name's length in characters.

				@return Return the name node's name or the value node's name without '\0'.

				@exception GAIA::ECT::EctIllegal
					If can't read a name node or a value node's name, throw it.

				@exception GAIA::ECT::EctDataError
					If the source xml's format exist error, and can't read, throw it.
			*/
			GINL const _DataType* Read(GAIA::XML::XML_NODE& nt, _SizeType& nodenamelen)
			{
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_NAME:
					break;
				case GAIA::XML::XML_NODE_VALUE:
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
				GAIA::XML::XML_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_NAME:
					{
						if(maxlen != GINVALID && nodenamelen >= maxlen)
							GTHROW_RET(BufNotEnough, GNIL);
						GAIA::ALGO::gstrcpy(p, pRet, nodenamelen);
					}
					break;
				case GAIA::XML::XML_NODE_VALUE:
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
				GAIA::XML::XML_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_NAME:
					GTHROW_RET(Illegal, GNIL);
				case GAIA::XML::XML_NODE_VALUE:
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
				GAIA::XML::XML_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_NAME:
					GTHROW_RET(Illegal, GNIL);
				case GAIA::XML::XML_NODE_VALUE:
					{
						if(GAIA::ALGO::gstrcmp(pRet, "true", nodenamelen) == 0)
							v = GAIA::True;
						else if(GAIA::ALGO::gstrcmp(pRet, "false", nodenamelen) == 0)
							v = GAIA::False;
						else
							GTHROW_RET(Convert, v);
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
				GAIA::XML::XML_NODE nt;
				_SizeType nodenamelen;
				const _DataType* pNext;
				const _DataType* pRet = this->Peek(nt, nodenamelen, &pNext);
				if(pRet == GNIL)
					GTHROW_RET(Illegal, GNIL);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_NAME:
					GTHROW_RET(Illegal, GNIL);
				case GAIA::XML::XML_NODE_VALUE:
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
					if(*p >= sizeof(ascii_xml_validchar) || ascii_xml_validchar[*p])
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
		class XmlReaderA : public BasicXmlReader<GAIA::CH, GAIA::NUM, GAIA::NUM, 32>{public:};
		class XmlReaderW : public BasicXmlReader<GAIA::WCH, GAIA::NUM, GAIA::NUM, 32>{public:};
	}
}

#endif