#ifndef		__GAIA_XML_XMLBASE_H__
#define		__GAIA_XML_XMLBASE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"

namespace GAIA
{
	namespace XML
	{
		/*!
		 	@brief Specify the xml node type.
		*/
		GAIA_ENUM_BEGIN(XML_NODE)
			XML_NODE_CONTAINER,		// Like <NODE1/>
			XML_NODE_MULTICONTAINER,// Like <NODE1></NODE1>
			XML_NODE_NAME,			// Like a="abc" 's a part
			XML_NODE_VALUE,			// Like a="abc" 's "abc" part
			XML_NODE_HEAD,			// Like <?xml version="1.0" encoding="utf-8"?>
			XML_NODE_COMMENT,		// Like <!--comment content-->
		GAIA_ENUM_END(XML_NODE)

		/*!
		 	@brief Specify the xml save text mode.
		*/
		GAIA_ENUM_BEGIN(XML_SAVE)
			XML_SAVE_BESTSIZE, /*!< Remove all invalid characters(' ', '\t', '\r', '\n' and etc) */
			XML_SAVE_BESTREAD, /*!< Add line break and indent. */
		GAIA_ENUM_END(XML_SAVE)

		static const GAIA::TCH XML_DEFAULT_ROOT_NODE_NAME[] = _T("XML_ROOT");

		static const GAIA::U8 ascii_xml_validchar[128] =
		{
			/* 1-8 */
			0	, //	'\0'	NUL(null)
			0	, //			SOH(start of headline)
			0	, //			STX(start of text)
			0	, //			ETX(end of text)
			0	, //			EOT(end of transmission)
			0	, //			ENQ(enquiry)
			0	, //			ACK(acknowledge)
			0	, //			BEL(bell)

			/* 9-16 */
			0	, //			BS(backspace)
			0	, //			HT(horizontal tab)
			0	, //	'\n'	LF(NL line feed, new line)
			0	, //			VT(vertical tab)
			0	, //			FF(NP form feed, new page)
			0	, //	'\r'	CR(carriage return)
			0	, //			SO(shift out)
			0	, //			SI(shift in)

			/* 17-24 */
			0	, //			DLE(data link escape)
			0	, //			DC1(device control 1)
			0	, //			DC2(device control 2)
			0	, //			DC3(device control 3)
			0	, //			DC4(device control 4)
			0	, //			NAK(negative acknowledge)
			0	, //			SYN(synchronous idle)
			0	, //			ETB(end of trans. block)

			/* 25-32 */
			0	, //			CAN(cancel)
			0	, //			EM(end of medium)
			0	, //			SUB(substitute)
			0	, //			ESC(escape)
			0	, //			FS(file separator)
			0	, //			GS(group separator)
			0	, //			RS(record separator)
			0	, //			US(unit separator)

			/* 33-40 */
			0	, //	' '		(space)
			1	, //	'!'		CH
			1	, //	'\"'	CH
			1	, //	'#'		CH
			1	, //	'$'		CH
			1	, //	'%'		CH
			1	, //	'&'		CH
			1	, //	'\''	CH

			/* 41-48 */
			1	, //	'('		CH
			1	, //	')'		CH
			1	, //	'*'		CH
			1	, //	'+'		CH
			1	, //	','		CH
			1	, //	'-'		CH
			1	, //	'.'		CH
			1	, //	'/'		CH

			/* 49-56 */
			1	, //	'0'		CH
			1	, //	'1'		CH
			1	, //	'2'		CH
			1	, //	'3'		CH
			1	, //	'4'		CH
			1	, //	'5'		CH
			1	, //	'6'		CH
			1	, //	'7'		CH

			/* 57-64 */
			1	, //	'8'		CH
			1	, //	'9'		CH
			1	, //	':'		CH
			1	, //	';'		CH
			1	, //	'<'		CH
			1	, //	'='		CH
			1	, //	'>'		CH
			1	, //	'?'		CH

			/* 65-72 */
			1	, //	'@'		CH
			1	, //	'A'		CH
			1	, //	'B'		CH
			1	, //	'C'		CH
			1	, //	'D'		CH
			1	, //	'E'		CH
			1	, //	'F'		CH
			1	, //	'G'		CH

			/* 73-80 */
			1	, //	'H'		CH
			1	, //	'I'		CH
			1	, //	'J'		CH
			1	, //	'K'		CH
			1	, //	'L'		CH
			1	, //	'M'		CH
			1	, //	'N'		CH
			1	, //	'O'		CH

			/* 81-88 */
			1	, //	'P'		CH
			1	, //	'Q'		CH
			1	, //	'R'		CH
			1	, //	'S'		CH
			1	, //	'T'		CH
			1	, //	'U'		CH
			1	, //	'V'		CH
			1	, //	'W'		CH

			/* 89-96 */
			1	, //	'X'		CH
			1	, //	'Y'		CH
			1	, //	'Z'		CH
			1	, //	'['		CH
			1	, //	'\\'	CH
			1	, //	']'		CH
			1	, //	'^'		CH
			1	, //	'_'		CH

			/* 97-104 */
			1	, //	'`'		CH
			1	, //	'a'		CH
			1	, //	'b'		CH
			1	, //	'c'		CH
			1	, //	'd'		CH
			1	, //	'e'		CH
			1	, //	'f'		CH
			1	, //	'g'		CH

			/* 105-112 */
			1	, //	'h'		CH
			1	, //	'i'		CH
			1	, //	'j'		CH
			1	, //	'k'		CH
			1	, //	'l'		CH
			1	, //	'm'		CH
			1	, //	'n'		CH
			1	, //	'o'		CH

			/* 113-120 */
			1	, //	'p'		CH
			1	, //	'q'		CH
			1	, //	'r'		CH
			1	, //	's'		CH
			1	, //	't'		CH
			1	, //	'u'		CH
			1	, //	'v'		CH
			1	, //	'w'		CH

			/* 121-128 */
			1	, //	'x'		CH
			1	, //	'y'		CH
			1	, //	'z'		CH
			1	, //	'{'		CH
			1	, //	'|'		CH
			1	, //	'}'		CH
			1	, //	'~'		CH
			0	, //			DEL(delete)
		};

		static const GAIA::U8 ascii_xml_validnamechar[128] =
		{
			/* 1-8 */
			0	, //	'\0'	NUL(null)
			0	, //			SOH(start of headline)
			0	, //			STX(start of text)
			0	, //			ETX(end of text)
			0	, //			EOT(end of transmission)
			0	, //			ENQ(enquiry)
			0	, //			ACK(acknowledge)
			0	, //			BEL(bell)

			/* 9-16 */
			0	, //			BS(backspace)
			0	, //			HT(horizontal tab)
			0	, //	'\n'	LF(NL line feed, new line)
			0	, //			VT(vertical tab)
			0	, //			FF(NP form feed, new page)
			0	, //	'\r'	CR(carriage return)
			0	, //			SO(shift out)
			0	, //			SI(shift in)

			/* 17-24 */
			0	, //			DLE(data link escape)
			0	, //			DC1(device control 1)
			0	, //			DC2(device control 2)
			0	, //			DC3(device control 3)
			0	, //			DC4(device control 4)
			0	, //			NAK(negative acknowledge)
			0	, //			SYN(synchronous idle)
			0	, //			ETB(end of trans. block)

			/* 25-32 */
			0	, //			CAN(cancel)
			0	, //			EM(end of medium)
			0	, //			SUB(substitute)
			0	, //			ESC(escape)
			0	, //			FS(file separator)
			0	, //			GS(group separator)
			0	, //			RS(record separator)
			0	, //			US(unit separator)

			/* 33-40 */
			0	, //	' '		(space)
			0	, //	'!'		CH
			0	, //	'\"'	CH
			0	, //	'#'		CH
			0	, //	'$'		CH
			0	, //	'%'		CH
			0	, //	'&'		CH
			0	, //	'\''	CH

			/* 41-48 */
			0	, //	'('		CH
			0	, //	')'		CH
			0	, //	'*'		CH
			0	, //	'+'		CH
			0	, //	','		CH
			1	, //	'-'		CH
			0	, //	'.'		CH
			0	, //	'/'		CH

			/* 49-56 */
			1	, //	'0'		CH
			1	, //	'1'		CH
			1	, //	'2'		CH
			1	, //	'3'		CH
			1	, //	'4'		CH
			1	, //	'5'		CH
			1	, //	'6'		CH
			1	, //	'7'		CH

			/* 57-64 */
			1	, //	'8'		CH
			1	, //	'9'		CH
			0	, //	':'		CH
			0	, //	';'		CH
			0	, //	'<'		CH
			0	, //	'='		CH
			0	, //	'>'		CH
			0	, //	'?'		CH

			/* 65-72 */
			0	, //	'@'		CH
			1	, //	'A'		CH
			1	, //	'B'		CH
			1	, //	'C'		CH
			1	, //	'D'		CH
			1	, //	'E'		CH
			1	, //	'F'		CH
			1	, //	'G'		CH

			/* 73-80 */
			1	, //	'H'		CH
			1	, //	'I'		CH
			1	, //	'J'		CH
			1	, //	'K'		CH
			1	, //	'L'		CH
			1	, //	'M'		CH
			1	, //	'N'		CH
			1	, //	'O'		CH

			/* 81-88 */
			1	, //	'P'		CH
			1	, //	'Q'		CH
			1	, //	'R'		CH
			1	, //	'S'		CH
			1	, //	'T'		CH
			1	, //	'U'		CH
			1	, //	'V'		CH
			1	, //	'W'		CH

			/* 89-96 */
			1	, //	'X'		CH
			1	, //	'Y'		CH
			1	, //	'Z'		CH
			0	, //	'['		CH
			0	, //	'\\'	CH
			0	, //	']'		CH
			0	, //	'^'		CH
			1	, //	'_'		CH

			/* 97-104 */
			0	, //	'`'		CH
			1	, //	'a'		CH
			1	, //	'b'		CH
			1	, //	'c'		CH
			1	, //	'd'		CH
			1	, //	'e'		CH
			1	, //	'f'		CH
			1	, //	'g'		CH

			/* 105-112 */
			1	, //	'h'		CH
			1	, //	'i'		CH
			1	, //	'j'		CH
			1	, //	'k'		CH
			1	, //	'l'		CH
			1	, //	'm'		CH
			1	, //	'n'		CH
			1	, //	'o'		CH

			/* 113-120 */
			1	, //	'p'		CH
			1	, //	'q'		CH
			1	, //	'r'		CH
			1	, //	's'		CH
			1	, //	't'		CH
			1	, //	'u'		CH
			1	, //	'v'		CH
			1	, //	'w'		CH

			/* 121-128 */
			1	, //	'x'		CH
			1	, //	'y'		CH
			1	, //	'z'		CH
			0	, //	'{'		CH
			0	, //	'|'		CH
			0	, //	'}'		CH
			0	, //	'~'		CH
			0	, //			DEL(delete)
		};

		/*!
		 	@brief Check string is a valid xml node name or not.
		 
		 	@param nt [in] Specify the xml node type.
		 
		 	@param pszNodeName [in] Specify the xml node name.
		 
		 	@param sLen [in] Specify the length of parameter pszNode in characters.
		 		This parameter could be GINVALID, then all the characters in parameter pszNodeName will be checked until to '\0'.
		 
		 	@return If characters of parameter pszNodeName are all valid, return GAIA::True, or will return GAIA::False.
		*/
		template<typename _DataType> GAIA::BL XmlCheckNodeName(GAIA::XML::XML_NODE nt, const _DataType* pszNodeName, GAIA::NUM sLen = GINVALID)
		{
			switch(nt)
			{
			case GAIA::XML::XML_NODE_CONTAINER:
			case GAIA::XML::XML_NODE_MULTICONTAINER:
			case GAIA::XML::XML_NODE_NAME:
				{
					if(GAIA::ALGO::gstremp(pszNodeName))
						return GAIA::False;
					if(sLen == GINVALID)
					{
						const _DataType* p = pszNodeName;
						while(*p != '\0')
						{
							if(*p >= sizeof(ascii_xml_validnamechar) || !ascii_xml_validnamechar[(GAIA::U8)*p])
								return GAIA::False;
							++p;
						}
					}
					else
					{
						for(GAIA::NUM x = 0; x < sLen; ++x)
						{
							if(pszNodeName[x] >= sizeof(ascii_xml_validnamechar) || !ascii_xml_validnamechar[(GAIA::U8)pszNodeName[x]])
								return GAIA::False;
						}
					}
				}
				break;
			case GAIA::XML::XML_NODE_VALUE:
				{
					if(!GAIA::ALGO::gstremp(pszNodeName))
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
							GAST(sLen > 0);
							for(GAIA::NUM x = 0; x < sLen; ++x)
							{
								if(pszNodeName[x] == '<' || pszNodeName[x] == '>' || pszNodeName[x] == '\"')
									return GAIA::False;
							}
						}
					}
					else
						GAST(sLen == GINVALID || sLen == 0);
				}
				break;
			default:
				GASTFALSE;
				return GAIA::False;
			}
			return GAIA::True;
		}
		
		/*!
		 	@brief Change xml text format.
		 
		 	@param pSrc [in] Specify the source xml text string.
		 		This parameter can't be NULL.
		 
		 	@param sSrcLen [in] Specify the source xml text string length in characters.
		 		This parameter must above 0.
		 
		 	@param pDst [out] Used for saving the converted result.
		 		This parameter could be GNIL when current function used for calculate length of the result only, then the parameter sDstLen must be GINVALID.
		 		
		 
		 	@param sDstLen [in] Specify the max length of parameter pDst in characters.
				This parameter could be GINVALID when current function used for calculate length of the result only, then the parameter pDst must be GNIL.
		 		If parameter pDst is not GNIL, this parameter must above 0.
		 
		 	@param savetype [in] Specify the target xml save type.
		 
		 	@return If success, return the practice count of result characters.\n
		 		If failed, return GINVALID.
		 
		 	@remarks This function not clean invalid characters when parameter savetype is XML_SAVE_BESTREAD,
		 		In this situation, you could call this function with parameter savetype filled by XML_SAVE_BESTSIZE first to cleanup invalid characters. 
		*/
		template<typename _DataType> GAIA::NUM XmlChangeFormat(const _DataType* pSrc, GAIA::NUM sSrcLen, _DataType* pDst, GAIA::NUM sDstLen, GAIA::XML::XML_SAVE savetype)
		{
		#define XMLCHANGEFORMAT_STEPRESULT(v) \
				do\
				{\
					chLast = v;\
					if(pDst == GNIL)\
						sRet++;\
					else\
					{\
						if(sRet >= sDstLen)\
							return GINVALID;\
						pDst[sRet++] = v;\
					}\
				}\
				while(0)
		#define XMLCHANGEFORMAT_LINEBREAKTAB \
			do\
			{\
				for(GAIA::NUM d = 0; d < sizeof(szLineBreak) - 1; ++d)\
					XMLCHANGEFORMAT_STEPRESULT(szLineBreak[d]);\
				for(GAIA::NUM d = 0; d < sDepth; ++d)\
					XMLCHANGEFORMAT_STEPRESULT('\t');\
			}\
			while(0)
			
			// Parameter checkup.
			GPCHR_FALSE_RET(pSrc != GNIL, GINVALID);
			GPCHR_FALSE_RET(sSrcLen > 0, GINVALID);
			GPCHR_FALSE_RET(savetype > GAIA::XML::XML_SAVE_INVALID && savetype < GAIA::XML::XML_SAVE_MAXENUMCOUNT, GINVALID);
			if(pDst == GNIL)
				GPCHR_FALSE_RET(sDstLen == 0, GINVALID);
			else
				GPCHR_FALSE_RET(sDstLen != 0, GINVALID);
			GPCHR_FALSE_RET(sDstLen >= 0, GINVALID);
			
			// Local variables.
			GAIA::NUM sRet = 0;
			_DataType chLast = '\0';
			
			// 
			if(savetype == GAIA::XML::XML_SAVE_BESTSIZE)
			{
				GAIA::BL bInString = GAIA::False;
				for(GAIA::NUM x = 0; x < sSrcLen; ++x)
				{
					if(pSrc[x] == '\"')
					{
						if(x == 0 || pSrc[x - 1] != '\\')
							bInString = !bInString;
						XMLCHANGEFORMAT_STEPRESULT(pSrc[x]);
					}
					else
					{
						if(bInString)
							XMLCHANGEFORMAT_STEPRESULT(pSrc[x]);
						else
						{
							if(pSrc[x] > ' ')
							{
								if(chLast == '\"')
								{
									GAIA::BL bCurrentIsContentChar = GAIA::ALGO::isalpha(pSrc[x]) || GAIA::ALGO::isdigit(pSrc[x]) || pSrc[x] == '_' || pSrc[x] == '-';
									if(bCurrentIsContentChar)
										XMLCHANGEFORMAT_STEPRESULT(' ');
								}
								XMLCHANGEFORMAT_STEPRESULT(pSrc[x]);
							}
							else if(pSrc[x] == ' ' || pSrc[x] == '\t')
							{
								if(x > 0 && x + 1 < sSrcLen)
								{
									_DataType chprev = pSrc[x - 1];
									_DataType chnext = pSrc[x + 1];
									GAIA::BL bPrevIsContentChar = GAIA::ALGO::isalpha(chprev) || GAIA::ALGO::isdigit(chprev) || chprev == '_' || chprev == '-';
									GAIA::BL bNextIsContentChar = GAIA::ALGO::isalpha(chnext) || GAIA::ALGO::isdigit(chnext) || chnext == '_' || chnext == '-';
									if(bPrevIsContentChar && bNextIsContentChar)
										XMLCHANGEFORMAT_STEPRESULT(' ');
								}
							}
						}
					}
				}
			}
			else if(savetype == GAIA::XML::XML_SAVE_BESTREAD)
			{
				GAIA::BL bInString = GAIA::False;
				GAIA::NUM sDepth = 0;
				const GAIA::CH szLineBreak[] = GAIA_FILELINEBREAK;
				for(GAIA::NUM x = 0; x < sSrcLen; ++x)
				{
					if(pSrc[x] == '\"')
					{
						if(x == 0 || pSrc[x - 1] != '\\')
							bInString = !bInString;
					}
					else if(!bInString)
					{
						if(pSrc[x] == '<')
						{
							if(x + 1 < sSrcLen && pSrc[x + 1] == '/')
							{
								sDepth--;
								XMLCHANGEFORMAT_LINEBREAKTAB;
							}
							else
							{
								if(sDepth != 0)
									XMLCHANGEFORMAT_LINEBREAKTAB;
								sDepth++;
							}
						}
						else if(pSrc[x] == '/')
							sDepth--;
					}
					XMLCHANGEFORMAT_STEPRESULT(pSrc[x]);
				}
			}
			else
				GASTFALSE;
			
			// Return.
			return sRet;
		}
		
		GINL GAIA::NUM XmlChangeFormat(const GAIA::TCH* pszSrcFileName, const GAIA::TCH* pszDstFileName, GAIA::XML::XML_SAVE savetype)
		{
			// Parameter checkup.
			GPCHR_FALSE_RET(!GAIA::ALGO::gstremp(pszSrcFileName), GINVALID);
			GPCHR_FALSE_RET(savetype > GAIA::XML::XML_SAVE_INVALID && savetype < GAIA::XML::XML_SAVE_MAXENUMCOUNT, GINVALID);

			// Local variables.
			GAIA::NUM sRet = 0;
			
			// 
			if(savetype == GAIA::XML::XML_SAVE_BESTSIZE)
			{
			}
			else if(savetype == GAIA::XML::XML_SAVE_BESTREAD)
			{
			}
			else
				GASTFALSE;
			
			// Return.
			return sRet;
		}

		class XmlFactoryHolder : public GAIA::Base
		{
			friend class XmlFactory;
		public:
			GINL XmlFactoryHolder(){m_pFactory = GNIL;}
			GINL XmlFactory* GetFactory() const{return m_pFactory;}
		private:
			GINL GAIA::GVOID SetFactory(XmlFactory* pFactory){m_pFactory = pFactory;}
		private:
			XmlFactory* m_pFactory;
		};
	}
}

#endif
