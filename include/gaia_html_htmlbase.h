#ifndef		__GAIA_HTML_HTMLBASE_H__
#define		__GAIA_HTML_HTMLBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"

namespace GAIA
{
	namespace HTML
	{
		/*!
		 	@brief Specify the html node type.
		*/
		GAIA_ENUM_BEGIN(HTML_NODE)
			HTML_NODE_CONTAINER,		// Like <NODE1/>
			HTML_NODE_MULTICONTAINER,	// Like <NODE1></NODE1>
			HTML_NODE_NAME,				// Like a="abc" 's a part
			HTML_NODE_VALUE,			// Like a="abc" 's "abc" part
			HTML_NODE_HEAD,				// <!DOCTYPE ...>
			HTML_NODE_COMMENT,			// Like <!--comment content-->
		GAIA_ENUM_END(HTML_NODE)

		/*!
		 	@brief Specify the html save text mode.
		*/
		GAIA_ENUM_BEGIN(HTML_SAVE)
			HTML_SAVE_BESTSIZE, /*!< Remove all invalid characters(' ', '\t', '\r', '\n' and etc) */
			HTML_SAVE_BESTREAD, /*!< Add line break and indent. */
		GAIA_ENUM_END(HTML_SAVE)

		static const GAIA::TCH HTML_DEFAULT_ROOT_NODE_NAME[] = _T("html");

		/*!
		 	@brief Check string is a valid html node name or not.
		 
		 	@param nt [in] Specify the html node type.
		 
		 	@param pszNodeName [in] Specify the html node name.
		 
		 	@param sLen [in] Specify the length of parameter pszNode in characters.
		 		This parameter could be GINVALID, then all the characters in parameter pszNodeName will be checked until to '\0'.
		 
		 	@return If characters of parameter pszNodeName are all valid, return GAIA::True, or will return GAIA::False.
		*/
		template<typename _DataType> GAIA::BL HtmlCheckNodeName(GAIA::HTML::HTML_NODE nt, const _DataType* pszNodeName, GAIA::NUM sLen = GINVALID)
		{
			switch(nt)
			{
			case GAIA::HTML::HTML_NODE_CONTAINER:
			case GAIA::HTML::HTML_NODE_MULTICONTAINER:
			case GAIA::HTML::HTML_NODE_NAME:
				{
					if(sLen == GINVALID)
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
					else
					{
						for(GAIA::NUM x = 0; x < sLen; ++x)
						{
							const _DataType* p = &pszNodeName[x];
							if((*p >= 'a' && *p <= 'z') ||
							   (*p >= 'A' && *p <= 'Z') ||
							   (*p >= '0' && *p <= '9') ||
							   (*p == '_')){}
							else
								return GAIA::False;
						}
					}
				}
				break;
			case GAIA::HTML::HTML_NODE_VALUE:
				{
					if(sLen == GINVALID)
					{
						const _DataType* p = pszNodeName;
						while(*p != '\0')
						{
							if(*p == '<' || *p == '>' || *p == '\"')
								return GAIA::False;
							++p;
						}
					}
					else
					{
						for(GAIA::NUM x = 0; x < sLen; ++x)
						{
							const _DataType* p = &pszNodeName[x];
							if(*p == '<' || *p == '>' || *p == '\"')
								return GAIA::False;
						}
					}
				}
				break;
			default:
				GASTFALSE;
				return GAIA::False;
			}
			return GAIA::True;
		}
		
		/*!
		 	@brief Change html text format.
		 
		 	@param pSrc [in] Specify the source html text string.
		 		This parameter can't be NULL.
		 
		 	@param sSrcLen [in] Specify the source html text string length in characters.
		 		This parameter must above 0.
		 
		 	@param pDst [out] Used for saving the converted result.
		 		This parameter could be GNIL when current function used for calculate length of the result only, then the parameter sDstLen must be GINVALID.
		 		
		 
		 	@param sDstLen [in] Specify the max length of parameter pDst in characters.
				This parameter could be GINVALID when current function used for calculate length of the result only, then the parameter pDst must be GNIL.
		 		If parameter pDst is not GNIL, this parameter must above 0.
		 
		 	@param savetype [in] Specify the target html save type.
		 
		 	@return If success, return the practice count of result characters.\n
		 		If failed, return GINVALID.
		 
		 	@remarks This function not clean invalid characters when parameter savetype is HTML_SAVE_BESTREAD,
		 		In this situation, you could call this function with parameter savetype filled by HTML_SAVE_BESTSIZE first to cleanup invalid characters. 
		*/
		template<typename _DataType> GAIA::NUM HtmlChangeFormat(const _DataType* pSrc, GAIA::NUM sSrcLen, _DataType* pDst, GAIA::NUM sDstLen, GAIA::HTML::HTML_SAVE savetype)
		{
			GAIA::NUM sRet = 0;
			return sRet;
		}
		
		GINL GAIA::NUM HtmlChangeFormat(const GAIA::TCH* pszSrcFileName, const GAIA::TCH* pszDstFileName, GAIA::HTML::HTML_SAVE savetype)
		{
			GAIA::NUM sRet = 0;
			return sRet;
		}

		class HtmlFactoryHolder : public GAIA::Base
		{
			friend class HtmlFactory;
		public:
			GINL HtmlFactoryHolder(){m_pFactory = GNIL;}
			GINL HtmlFactory* GetFactory() const{return m_pFactory;}
		private:
			GINL GAIA::GVOID SetFactory(HtmlFactory* pFactory){m_pFactory = pFactory;}
		private:
			HtmlFactory* m_pFactory;
		};
	}
}

#endif
