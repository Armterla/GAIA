#ifndef		__GAIA_JSON_JSONBASE_H__
#define		__GAIA_JSON_JSONBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace JSON
	{
		GAIA_ENUM_BEGIN(JSON_NODE)
			JSON_NODE_CONTAINER,		// Like {}
			JSON_NODE_MULTICONTAINER,	// Like []
			JSON_NODE_NAME,				// Like "c":1 's "c" part
			JSON_NODE_VALUE,			// Like "c":1 's 1 part
		GAIA_ENUM_END(JSON_NODE)

		GAIA_ENUM_BEGIN(JSON_SAVE)
			JSON_SAVE_BESTSIZE,
			JSON_SAVE_BESTREAD,
		GAIA_ENUM_END(JSON_SAVE)

		static const GAIA::TCH JSON_DEFAULT_ROOT_NODE_NAME[] = _T("JSON_ROOT");

		GINL GAIA::BL JSONCheckNodeName(GAIA::JSON::JSON_NODE nt, const GAIA::TCH* pszNodeName)
		{
			switch(nt)
			{
			case GAIA::JSON::JSON_NODE_CONTAINER:
			case GAIA::JSON::JSON_NODE_MULTICONTAINER:
			case GAIA::JSON::JSON_NODE_NAME:
				{
					const GAIA::TCH* p = pszNodeName;
					while(*p != '\0')
					{
						if((*p >= 'a' && *p <= 'z') ||
						   (*p >= 'A' && *p <= 'Z') ||
						   (*p >= '0' && *p <= '9') ||
						   (*p == '_')){}
						else
							return GAIA::False;
						++p;
					}
				}
				break;
			case GAIA::JSON::JSON_NODE_VALUE:
				{
					const GAIA::TCH* p = pszNodeName;
					while(*p != '\0')
					{
						if(*p == '<' || *p == '>' || *p == '\"')
							return GAIA::False;
						++p;
					}
				}
				break;
			default:
				GASTFALSE;
				return GAIA::False;
			}
			return GAIA::True;
		}

		class JsonFactoryHolder : public GAIA::Base
		{
			friend class JsonFactory;
		public:
			GINL JsonFactoryHolder(){m_pFactory = GNIL;}
			GINL JsonFactory* GetFactory() const{return m_pFactory;}
		private:
			GINL GAIA::GVOID SetFactory(JsonFactory* pFactory){m_pFactory = pFactory;}
		private:
			JsonFactory* m_pFactory;
		};
	}
}

#endif
