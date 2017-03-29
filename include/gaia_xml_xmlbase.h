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
						if(*p >= sizeof(ascii_xml_validnamechar) || !ascii_xml_validnamechar[*p])
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
