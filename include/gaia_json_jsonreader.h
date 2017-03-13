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
				@brief

				@param
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
				@brief

				@param

				@return
			*/
			GINL const GAIA::GVOID* GetBuffer(_SizeType& size) const{size = m_size; return m_pFront;}

			/*!
				@brief

				@return
			*/
			GINL _SizeType GetBufferSize() const{return m_size;}

			/*!
				@brief

				@return
			*/
			GINL _SizeType GetReadSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}

			/*!
				@brief

				@return
			*/
			GINL _SizeType GetRemainSize() const{return this->GetBufferSize() - this->GetReadSize();}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL const _DataType* Peek(GAIA::JSON::JSON_NODE& nt, _SizeType& nodenamelen, _DataTypePtr* pNext = GNIL)
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
							GTHROW_RET(DataError, GNIL);
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
						if(*pLocalNext != '\"')
							GTHROW_RET(DataError, GNIL);
						while(pLocalNext <= m_pBack)
						{
							if(*pLocalNext == '\"' && pLocalNext > p && pLocalNext[-1] != '\\')
								break;
							++pLocalNext;
						}
						if(pLocalNext > m_pBack)
							GTHROW_RET(DataError, GNIL);
						nodenamelen = (_SizeType)(pLocalNext - p - 1);
						pLocalNext = pLocalNext + 1;
						pRet = p + 1;
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
					   *p != '\n')
						return p;
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