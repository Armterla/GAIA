#ifndef 	__GAIA_XML_XMLWRITER_H__
#define 	__GAIA_XML_XMLWRITER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"
#include "gaia_algo_string.h"
#include "gaia_xml_xmlbase.h"

namespace GAIA
{
	namespace XML
	{
		template<typename _DataType, typename _SizeType, typename _DepthType, _DepthType _MaxDepth> class BasicXmlWriter : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _DepthType _depthtype;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicXmlWriter<_DataType, _SizeType, _DepthType, _MaxDepth> __MyType;

		public:
			GINL BasicXmlWriter(){this->init();}
			GINL ~BasicXmlWriter(){}

			GINL GAIA::GVOID SetBuffer(const GAIA::GVOID* p, _SizeType sSize)
			{
				if(p == GNIL)
				{
					GAST(sSize == 0);
					m_pFront = m_pBack = m_pCursor = GNIL;
					m_sSize = 0;
				}
				else
				{
					GAST(sSize > sizeof(_DataType));
					m_pFront = (_DataType*)p;
					m_pBack = m_pFront + (sSize / sizeof(_DataType) - 1);
					m_pCursor = m_pFront;
					m_sSize = sSize;
				}
				m_CNTCursor = GINVALID;
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}
			GINL const GAIA::GVOID* GetBuffer(_SizeType& sSize) const{sSize = m_sSize; return m_pFront;}
			GINL _SizeType GetBufferSize() const{return m_sSize;}
			GINL _SizeType GetWriteSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}
			GINL _SizeType GetRemainSize() const{return this->GetBufferSize() - this->GetWriteSize();}

			GINL GAIA::GVOID BeginWriteNode(GAIA::XML::XML_NODE nt, const _DataType* pszNodeName = GNIL, _SizeType sNodeNameLen = GINVALID)
			{
				GAST(nt > GAIA::XML::XML_NODE_INVALID && nt < GAIA::XML::XML_NODE_MAXENUMCOUNT);
				if(pszNodeName != GNIL)
				{
					if(!GAIA::XML::XmlCheckNodeName(nt, pszNodeName))
						GTHROW(InvalidParam);
				}
				if(sNodeNameLen == GINVALID && pszNodeName != GNIL)
					sNodeNameLen = GAIA::ALGO::gstrlen(pszNodeName);
				if(m_LastNNVT == GAIA::XML::XML_NODE_NAME)
					GTHROW(Illegal);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_CONTAINER:
					{
						if(m_CNTCursor == GINVALID)
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							m_CNTCursor = 0;
							m_LastCNT[m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = sNodeNameLen;
							this->write(pszNodeName, sNodeNameLen);

						}
						else
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							GAST(m_CNTCursor + 1 < _MaxDepth);
							m_LastCNT[++m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = sNodeNameLen;
							this->write(pszNodeName, sNodeNameLen);
						}
					}
					break;
				case GAIA::XML::XML_NODE_MULTICONTAINER:
					{
						if(m_CNTCursor == GINVALID)
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							m_CNTCursor = 0;
							m_LastCNT[m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = sNodeNameLen;
							this->write(pszNodeName, sNodeNameLen);
							this->write(">", sizeof(">") - 1);
						}
						else
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							GAST(m_CNTCursor + 1 < _MaxDepth);
							m_LastCNT[++m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = sNodeNameLen;
							this->write(pszNodeName, sNodeNameLen);
							this->write(">", sizeof(">") - 1);
						}
					}
					break;
				case GAIA::XML::XML_NODE_NAME:
					GTHROW(Illegal);
					break;
				case GAIA::XML::XML_NODE_VALUE:
					GTHROW(Illegal);
					break;
				default:
					GASTFALSE;
					break;
				}
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}
			GINL GAIA::GVOID EndWriteNode()
			{
				if(m_CNTCursor == GINVALID)
					GTHROW(Illegal);
				if(m_LastNNVT == GAIA::XML::XML_NODE_NAME)
					GTHROW(Illegal);
				switch(m_LastCNT[m_CNTCursor])
				{
				case GAIA::XML::XML_NODE_CONTAINER:
					{
						this->write("/>", sizeof("/>") - 1);
					}
					break;
				case GAIA::XML::XML_NODE_MULTICONTAINER:
					{
						this->write("</", sizeof("</") - 1);
						NodeName& nn = m_NodeNameStack[m_CNTCursor];
						this->write(nn.p, nn.len);
						this->write(">", sizeof(">") - 1);
					}
					break;
				case GAIA::XML::XML_NODE_NAME:
					GTHROW(Illegal);
					break;
				case GAIA::XML::XML_NODE_VALUE:
					GTHROW(Illegal);
					break;
				default:
					GASTFALSE;
					break;
				}
				m_CNTCursor--;
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}
			GINL GAIA::GVOID WriteNode(GAIA::XML::XML_NODE nt, const _DataType* pszNodeName, _SizeType sNodeNameLen = GINVALID)
			{
				GAST(nt > GAIA::XML::XML_NODE_INVALID && nt < GAIA::XML::XML_NODE_MAXENUMCOUNT);
				GAST(!GAIA::ALGO::gstremp(pszNodeName));
				if(!GAIA::XML::XmlCheckNodeName(nt, pszNodeName))
					GTHROW(InvalidParam);
				if(m_CNTCursor == GINVALID)
					GTHROW(Illegal);
				if(sNodeNameLen == GINVALID && pszNodeName != GNIL)
					sNodeNameLen = GAIA::ALGO::gstrlen(pszNodeName);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_CONTAINER:
					GTHROW(Illegal);
					break;
				case GAIA::XML::XML_NODE_MULTICONTAINER:
					GTHROW(Illegal);
					break;
				case GAIA::XML::XML_NODE_NAME:
					{
						if(m_LastNNVT != GAIA::XML::XML_NODE_INVALID &&
						   m_LastNNVT != GAIA::XML::XML_NODE_VALUE)
							GTHROW(Illegal);
						this->write(" ", sizeof(" ") - 1);
						this->write(pszNodeName, sNodeNameLen);
						this->write("=\"", sizeof("=\"") - 1);
						m_LastNNVT = GAIA::XML::XML_NODE_NAME;
					}
					break;
				case GAIA::XML::XML_NODE_VALUE:
					{
						if(m_LastNNVT != GAIA::XML::XML_NODE_NAME)
							GTHROW(Illegal);
						this->write(pszNodeName, sNodeNameLen);
						this->write("\"", sizeof("\"") - 1);
						m_LastNNVT = GAIA::XML::XML_NODE_VALUE;
					}
					break;
				default:
					GASTFALSE;
					break;
				}
			}

		private:
			class NodeName : public GAIA::Base
			{
			public:
				_DataType* p;
				_SizeType len;
			};

		private:
			GINL GAIA::GVOID init()
			{
				GAST(_MaxDepth > 0);
				m_pFront = m_pBack = m_pCursor = GNIL;
				m_sSize = 0;
				m_CNTCursor = GINVALID;
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}
			GINL GAIA::GVOID write(const _DataType* p, _SizeType sSize) // sSize is valid character count.
			{
				GAST(p != GNIL);
				GAST(sSize > 0);
				if(sSize * sizeof(_DataType) > this->GetRemainSize())
					GTHROW(BufNotEnough);
				GAIA::ALGO::gmemcpy(m_pCursor, p, sSize * sizeof(_DataType));
				m_pCursor += sSize;
			}

		private:
			_DataType* m_pFront;
			_DataType* m_pBack; // Last valid character.
			_DataType* m_pCursor;
			_SizeType m_sSize; // Size in bytes.
			GAIA::XML::XML_NODE m_LastCNT[_MaxDepth];
			_DepthType m_CNTCursor;
			GAIA::XML::XML_NODE m_LastNNVT; // NNVT means node name value type.
			NodeName m_NodeNameStack[_MaxDepth];
		};
		class XmlWriterA : public BasicXmlWriter<GAIA::CH, GAIA::NUM, GAIA::NUM, 32>{public:};
		class XmlWriterW : public BasicXmlWriter<GAIA::WCH, GAIA::NUM, GAIA::NUM, 32>{public:};
	}
}

#endif