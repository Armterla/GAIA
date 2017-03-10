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
		template<typename _DataType, typename _DepthType, _DepthType _MaxDepth> class BasicXMLWriter : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _DepthType _depthtype;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicXMLWriter<_DataType, _DepthType, _MaxDepth> __MyType;

		public:
			GINL BasicXMLWriter(){this->init();}
			GINL ~BasicXMLWriter(){}

			GINL GAIA::GVOID SetBuffer(const GAIA::GVOID* p, GAIA::NUM sSize)
			{
				if(p == m_pFront && sSize == m_sSize)
					return;
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
				m_LastContainerNodeType = GAIA::XML::XML_NODE_INVALID;
				m_LastNodeType = GAIA::XML::XML_NODE_INVALID;
			}
			GINL const GAIA::GVOID* GetBuffer(GAIA::NUM& sSize) const
			{
				sSize = m_sSize;
				return m_pFront;
			}

			GINL GAIA::GVOID BeginWriteNode(GAIA::XML::XML_NODE nt, const _DataType* pszNodeName)
			{
				GAST(nt > GAIA::XML::XML_NODE_INVALID && nt < GAIA::XML::XML_NODE_MAXENUMCOUNT);
				GAST(!GAIA::ALGO::gstremp(pszNodeName));
			}
			GINL GAIA::GVOID EndWriteNode()
			{

			}
			GINL GAIA::GVOID WriteNode(GAIA::XML::XML_NODE nt, const _DataType* pszNodeName)
			{

			}

			GINL GAIA::NUM GetWriteSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}

		private:
			GINL GAIA::GVOID init()
			{
				m_pFront = m_pBack = m_pCursor = GNIL;
				m_sSize = 0;
				m_LastContainerNodeType = GAIA::XML::XML_NODE_INVALID;
				m_LastNodeType = GAIA::XML::XML_NODE_INVALID;
			}

		private:
			_DataType* m_pFront;
			_DataType* m_pBack; // Last valid character.
			_DataType* m_pCursor;
			GAIA::NUM m_sSize; // Size in bytes.
			GAIA::XML::XML_NODE m_LastContainerNodeType;
			GAIA::XML::XML_NODE m_LastNodeType;
		};
		class XMLWriterA : public BasicXMLWriter<GAIA::CH, GAIA::NUM, 64>{public:};
		class XMLWriterW : public BasicXMLWriter<GAIA::WCH, GAIA::NUM, 64>{public:};
	}
}

#endif