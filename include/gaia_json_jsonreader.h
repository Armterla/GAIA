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
		template<typename _DataType> class JsonReader : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;

		public:
			typedef JsonReader<_DataType> __MyType;

		public:
			GINL JsonReader(){this->init();}
			GINL ~JsonReader(){}

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
	}
}

#endif