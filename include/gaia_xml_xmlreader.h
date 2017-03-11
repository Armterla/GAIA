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
		template<typename _DataType, typename _SizeType, typename _DepthType, _DepthType _MaxDepth> class BasicXmlReader : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _DepthType _depthtype;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicXmlReader<_DataType, _SizeType, _DepthType, _MaxDepth> __MyType;

		public:
			GINL BasicXmlReader(){this->init();}
			GINL ~BasicXmlReader(){}

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
			GINL const GAIA::GVOID* GetBuffer(GAIA::NUM& sSize) const{sSize = m_sSize; return m_pFront;}
			GINL GAIA::NUM GetBufferSize() const{return m_sSize;}
			GINL GAIA::NUM GetReadSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}
			GINL GAIA::NUM GetRemainSize() const{return this->GetBufferSize() - this->GetReadSize();}

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
		class XmlReaderA : public BasicXmlReader<GAIA::CH, GAIA::NUM, GAIA::NUM, 32>{public:};
		class XmlReaderW : public BasicXmlReader<GAIA::WCH, GAIA::NUM, GAIA::NUM, 32>{public:};
	}
}

#endif