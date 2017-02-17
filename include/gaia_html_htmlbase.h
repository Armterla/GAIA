#ifndef		__GAIA_HTML_HTMLBASE_H__
#define		__GAIA_HTML_HTMLBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace HTML
	{
		GAIA_ENUM_BEGIN(HTML_NODE)
			HTML_NODE_CONTAINER,		// Like <NODE1/>
			HTML_NODE_MULTICONTAINER,	// Like <NODE1></NODE1>
			HTML_NODE_NAME,				// Like a="abc" 's a part
			HTML_NODE_VALUE,			// Like a="abc" 's "abc" part
			HTML_NODE_HEAD,				// <!DOCTYPE ...>
			HTML_NODE_COMMENT,			// Like <!--comment content-->
		GAIA_ENUM_END(HTML_NODE)

		GAIA_ENUM_BEGIN(HTML_SAVE)
			HTML_SAVE_BESTSIZE,
			HTML_SAVE_BESTREAD,
		GAIA_ENUM_END(HTML_SAVE)

		static const GAIA::TCH HTML_DEFAULT_ROOT_NODE_NAME[] = _T("html");

		GINL GAIA::BL HTMLCheckNodeName(GAIA::HTML::HTML_NODE nt, const GAIA::TCH* pszNodeName)
		{
			switch(nt)
			{
			case GAIA::HTML::HTML_NODE_CONTAINER:
			case GAIA::HTML::HTML_NODE_MULTICONTAINER:
			case GAIA::HTML::HTML_NODE_NAME:
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
			case GAIA::HTML::HTML_NODE_VALUE:
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

		class HTMLFactoryHolder : public GAIA::Base
		{
			friend class HTMLFactory;
		public:
			GINL HTMLFactoryHolder(){m_pFactory = GNIL;}
			GINL HTMLFactory* GetFactory() const{return m_pFactory;}
		private:
			GINL GAIA::GVOID SetFactory(HTMLFactory* pFactory){m_pFactory = pFactory;}
		private:
			HTMLFactory* m_pFactory;
		};
	}
}

#endif
