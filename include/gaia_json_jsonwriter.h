#ifndef 	__GAIA_JSON_JSONWRITER_H__
#define 	__GAIA_JSON_JSONWRITER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_json_jsonbase.h"

namespace GAIA
{
	namespace JSON
	{
		template<typename _DataType> class JsonWriter : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;

		public:
			typedef JsonWriter<_DataType> __MyType;

		public:
			GINL JsonWriter(){this->init();}
			GINL ~JsonWriter(){}

			GINL GAIA::GVOID SetBuffer(GAIA::GVOID* p, GAIA::NUM sSize)
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
				m_LastContainerNodeType = GAIA::JSON::JSON_NODE_INVALID;
				m_LastNodeType = GAIA::JSON::JSON_NODE_INVALID;
			}
			GINL GAIA::GVOID* GetBuffer(GAIA::NUM& sSize) const
			{
				sSize = m_sSize;
				return m_pFront;
			}

			GINL GAIA::GVOID BeginWriteNode(GAIA::JSON::JSON_NODE nt, const _DataType* pszNodeName)
			{
				GAST(nt > GAIA::JSON::JSON_NODE_INVALID && nt < GAIA::JSON::JSON_NODE_MAXENUMCOUNT);
				GAST(!GAIA::ALGO::gstremp(pszNodeName));
			}
			GINL GAIA::GVOID EndWriteNode()
			{

			}
			GINL GAIA::GVOID WriteNode(GAIA::JSON::JSON_NODE nt, const _DataType* pszNodeName)
			{
				GAST(nt > GAIA::JSON::JSON_NODE_INVALID && nt < GAIA::JSON::JSON_NODE_MAXENUMCOUNT);
				GAST(!GAIA::ALGO::gstremp(pszNodeName));
			}

			GINL GAIA::NUM GetWriteSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}

		private:
			GINL GAIA::GVOID init()
			{
				m_pFront = m_pBack = m_pCursor = GNIL;
				m_sSize = 0;
				m_LastContainerNodeType = GAIA::JSON::JSON_NODE_INVALID;
				m_LastNodeType = GAIA::JSON::JSON_NODE_INVALID;
			}

		private:
			_DataType* m_pFront;
			_DataType* m_pBack; // Last valid character.
			_DataType* m_pCursor;
			GAIA::NUM m_sSize; // Size in bytes.
			GAIA::JSON::JSON_NODE m_LastContainerNodeType;
			GAIA::JSON::JSON_NODE m_LastNodeType;
		};
	}
}

#endif