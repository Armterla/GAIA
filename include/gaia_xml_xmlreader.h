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
		template<typename _DataType, typename _DepthType, _DepthType _MaxDepth> class BasicXMLReader : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _DepthType _depthtype;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicXMLReader<_DataType, _DepthType, _MaxDepth> __MyType;

		public:
			GINL BasicXMLReader(){this->init();}
			GINL ~BasicXMLReader(){}

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
					m_pFront = (const _DataType*)p;
					m_pBack = m_pFront + (sSize / sizeof(_DataType) - 1);
					m_pCursor = m_pFront;
					m_sSize = sSize;
				}
			}
			GINL const GAIA::GVOID* GetBuffer(GAIA::NUM& sSize) const
			{
				sSize = m_sSize;
				return m_pFront;
			}

		private:
			GINL GAIA::GVOID init()
			{
				m_pFront = m_pBack = m_pCursor = GNIL;
				m_sSize = 0;
			}

		private:
			const _DataType* m_pFront;
			const _DataType* m_pBack; // Last valid character.
			const _DataType* m_pCursor;
			GAIA::NUM m_sSize; // Size in bytes.
		};
		class XMLReaderA : public BasicXMLReader<GAIA::CH, GAIA::NUM, 64>{public:};
		class XMLReaderW : public BasicXMLReader<GAIA::WCH, GAIA::NUM, 64>{public:};
	}
}

#endif