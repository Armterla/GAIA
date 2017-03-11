#ifndef 	__GAIA_XML_XMLWRITER_H__
#define 	__GAIA_XML_XMLWRITER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
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
					m_pFront = (_DataType*)p;
					m_pBack = m_pFront + (size / sizeof(_DataType) - 1);
					m_pCursor = m_pFront;
					m_size = size;
				}
				m_CNTCursor = GINVALID;
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}
			GINL const GAIA::GVOID* GetBuffer(_SizeType& size) const{size = m_size; return m_pFront;}
			GINL _SizeType GetBufferSize() const{return m_size;}
			GINL _SizeType GetWriteSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}
			GINL _SizeType GetRemainSize() const{return this->GetBufferSize() - this->GetWriteSize();}

			GAIA::GVOID Begin(GAIA::XML::XML_NODE nt, const GAIA::CH* pszNodeName = GNIL, _SizeType nodenamelen = GINVALID)
			{
				GAST(nt > GAIA::XML::XML_NODE_INVALID && nt < GAIA::XML::XML_NODE_MAXENUMCOUNT);
				if(pszNodeName != GNIL)
				{
					if(!GAIA::XML::XmlCheckNodeName(nt, pszNodeName))
						GTHROW(InvalidParam);
				}
				if(nodenamelen == GINVALID && pszNodeName != GNIL)
					nodenamelen = GAIA::ALGO::gstrlen(pszNodeName);
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
							nn.len = nodenamelen;
							this->write(pszNodeName, nodenamelen);

						}
						else
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							GAST(m_CNTCursor + 1 < _MaxDepth);
							m_LastCNT[++m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = nodenamelen;
							this->write(pszNodeName, nodenamelen);
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
							nn.len = nodenamelen;
							this->write(pszNodeName, nodenamelen);
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
							nn.len = nodenamelen;
							this->write(pszNodeName, nodenamelen);
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
			GINL GAIA::GVOID End()
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
			template<typename _ParamDataType>  GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const _ParamDataType* pszNodeName, _SizeType nodenamelen = GINVALID)
			{
				GAST(nt > GAIA::XML::XML_NODE_INVALID && nt < GAIA::XML::XML_NODE_MAXENUMCOUNT);
				GAST(!GAIA::ALGO::gstremp(pszNodeName));
				if(!GAIA::XML::XmlCheckNodeName(nt, pszNodeName))
					GTHROW(InvalidParam);
				if(m_CNTCursor == GINVALID)
					GTHROW(Illegal);
				if(nodenamelen == GINVALID && pszNodeName != GNIL)
					nodenamelen = GAIA::ALGO::gstrlen(pszNodeName);
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
						this->write(pszNodeName, nodenamelen);
						this->write("=\"", sizeof("=\"") - 1);
						m_LastNNVT = GAIA::XML::XML_NODE_NAME;
					}
					break;
				case GAIA::XML::XML_NODE_VALUE:
					{
						if(m_LastNNVT != GAIA::XML::XML_NODE_NAME)
							GTHROW(Illegal);
						this->write(pszNodeName, nodenamelen);
						this->write("\"", sizeof("\"") - 1);
						m_LastNNVT = GAIA::XML::XML_NODE_VALUE;
					}
					break;
				default:
					GASTFALSE;
					break;
				}
			}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::BL& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::X128& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::U8& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::N8& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::U16& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::N16& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::U32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::N32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::U64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::N64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::F32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::F64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			__MyType& operator << (const GAIA::CH* p)
			{
				if(m_LastNNVT == GAIA::XML::XML_NODE_INVALID || m_LastNNVT == GAIA::XML::XML_NODE_VALUE)
					this->Write(GAIA::XML::XML_NODE_NAME, p);
				else if(m_LastNNVT == GAIA::XML::XML_NODE_NAME)
					this->Write(GAIA::XML::XML_NODE_VALUE, p);
				else
					GASTFALSE;
				return *this;
			}
			__MyType& operator << (const GAIA::WCH* p)
			{
				if(m_LastNNVT == GAIA::XML::XML_NODE_INVALID || m_LastNNVT == GAIA::XML::XML_NODE_VALUE)
					this->Write(GAIA::XML::XML_NODE_NAME, p);
				else if(m_LastNNVT == GAIA::XML::XML_NODE_NAME)
					this->Write(GAIA::XML::XML_NODE_VALUE, p);
				else
					GASTFALSE;
				return *this;
			}
			__MyType& operator << (const GAIA::BL& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::X128& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::U8& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::N8& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::U16& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::N16& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::U32& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::N32& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::U64& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::N64& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::F32& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::F64& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

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
				m_size = 0;
				m_CNTCursor = GINVALID;
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}
			template<typename _ParamDataType> GAIA::GVOID write(const _ParamDataType* p, _SizeType size) // parameter size is valid character count.
			{
				GAST(p != GNIL);
				GAST(size > 0);
				if(size * sizeof(_DataType) > this->GetRemainSize())
					GTHROW(BufNotEnough);
				for(_SizeType x = 0; x < size; ++x)
					*m_pCursor++ = p[x];
			}

		private:
			_DataType* m_pFront;
			_DataType* m_pBack; // Last valid character.
			_DataType* m_pCursor;
			_SizeType m_size; // Size in bytes.
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