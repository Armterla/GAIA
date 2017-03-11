#ifndef		__GAIA_XML_XMLBASE_H__
#define		__GAIA_XML_XMLBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"

namespace GAIA
{
	namespace XML
	{
		GAIA_ENUM_BEGIN(XML_NODE)
			XML_NODE_CONTAINER,		// Like <NODE1/>
			XML_NODE_MULTICONTAINER,// Like <NODE1></NODE1>
			XML_NODE_NAME,			// Like a="abc" 's a part
			XML_NODE_VALUE,			// Like a="abc" 's "abc" part
			XML_NODE_HEAD,			// Like <?xml version="1.0" encoding="utf-8"?>
			XML_NODE_COMMENT,		// Like <!--comment content-->
		GAIA_ENUM_END(XML_NODE)

		GAIA_ENUM_BEGIN(XML_SAVE)
			XML_SAVE_BESTSIZE,
			XML_SAVE_BESTREAD,
		GAIA_ENUM_END(XML_SAVE)

		static const GAIA::TCH XML_DEFAULT_ROOT_NODE_NAME[] = _T("XML_ROOT");

		template<typename _DataType> GAIA::BL XmlCheckNodeName(GAIA::XML::XML_NODE nt, const _DataType* pszNodeName)
		{
			switch(nt)
			{
			case GAIA::XML::XML_NODE_CONTAINER:
			case GAIA::XML::XML_NODE_MULTICONTAINER:
			case GAIA::XML::XML_NODE_NAME:
				{
					const _DataType* p = pszNodeName;
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
			case GAIA::XML::XML_NODE_VALUE:
				{
					const _DataType* p = pszNodeName;
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

		class XMLFactoryHolder : public GAIA::Base
		{
			friend class XMLFactory;
		public:
			GINL XMLFactoryHolder(){m_pFactory = GNIL;}
			GINL XMLFactory* GetFactory() const{return m_pFactory;}
		private:
			GINL GAIA::GVOID SetFactory(XMLFactory* pFactory){m_pFactory = pFactory;}
		private:
			XMLFactory* m_pFactory;
		};
	}
}

#endif
