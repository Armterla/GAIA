#ifndef		__GAIA_DIGIT_PERCENTAGE_H__
#define		__GAIA_DIGIT_PERCENTAGE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace DIGIT
	{
		static const GAIA::U8 ascii_need_percentage[128] =
		{
			/* 1-8 */
			1	, //	'\0'	NUL(null)
			1	, //			SOH(start of headline)
			1	, //			STX(start of text)
			1	, //			ETX(end of text)
			1	, //			EOT(end of transmission)
			1	, //			ENQ(enquiry)
			1	, //			ACK(acknowledge)
			1	, //			BEL(bell)

			/* 9-16 */
			1	, //			BS(backspace)
			1	, //			HT(horizontal tab)
			1	, //	'\n'	LF(NL line feed, new line)
			1	, //			VT(vertical tab)
			1	, //			FF(NP form feed, new page)
			1	, //	'\r'	CR(carriage return)
			1	, //			SO(shift out)
			1	, //			SI(shift in)

			/* 17-24 */
			1	, //			DLE(data link escape)
			1	, //			DC1(device control 1)
			1	, //			DC2(device control 2)
			1	, //			DC3(device control 3)
			1	, //			DC4(device control 4)
			1	, //			NAK(negative acknowledge)
			1	, //			SYN(synchronous idle)
			1	, //			ETB(end of trans. block)

			/* 25-32 */
			1	, //			CAN(cancel)
			1	, //			EM(end of medium)
			1	, //			SUB(substitute)
			1	, //			ESC(escape)
			1	, //			FS(file separator)
			1	, //			GS(group separator)
			1	, //			RS(record separator)
			1	, //			US(unit separator)

			/* 33-40 */
			1	, //	' '		(space)
			0	, //	'!'		CH
			1	, //	'\"'	CH
			1	, //	'#'		CH
			1	, //	'$'		CH
			1	, //	'%'		CH
			1	, //	'&'		CH
			1	, //	'\''	CH

			/* 41-48 */
			0	, //	'('		CH
			0	, //	')'		CH
			0	, //	'*'		CH
			1	, //	'+'		CH
			1	, //	','		CH
			0	, //	'-'		CH
			0	, //	'.'		CH
			1	, //	'/'		CH

			/* 49-56 */
			0	, //	'0'		CH
			0	, //	'1'		CH
			0	, //	'2'		CH
			0	, //	'3'		CH
			0	, //	'4'		CH
			0	, //	'5'		CH
			0	, //	'6'		CH
			0	, //	'7'		CH

			/* 57-64 */
			0	, //	'8'		CH
			0	, //	'9'		CH
			1	, //	':'		CH
			1	, //	';'		CH
			1	, //	'<'		CH
			1	, //	'='		CH
			1	, //	'>'		CH
			1	, //	'?'		CH

			/* 65-72 */
			1	, //	'@'		CH
			0	, //	'A'		CH
			0	, //	'B'		CH
			0	, //	'C'		CH
			0	, //	'D'		CH
			0	, //	'E'		CH
			0	, //	'F'		CH
			0	, //	'G'		CH

			/* 73-80 */
			0	, //	'H'		CH
			0	, //	'I'		CH
			0	, //	'J'		CH
			0	, //	'K'		CH
			0	, //	'L'		CH
			0	, //	'M'		CH
			0	, //	'N'		CH
			0	, //	'O'		CH

			/* 81-88 */
			0	, //	'P'		CH
			0	, //	'Q'		CH
			0	, //	'R'		CH
			0	, //	'S'		CH
			0	, //	'T'		CH
			0	, //	'U'		CH
			0	, //	'V'		CH
			0	, //	'W'		CH

			/* 89-96 */
			0	, //	'X'		CH
			0	, //	'Y'		CH
			0	, //	'Z'		CH
			1	, //	'['		CH
			1	, //	'\\'	CH
			1	, //	']'		CH
			1	, //	'^'		CH
			0	, //	'_'		CH

			/* 97-104 */
			1	, //	'`'		CH
			0	, //	'a'		CH
			0	, //	'b'		CH
			0	, //	'c'		CH
			0	, //	'd'		CH
			0	, //	'e'		CH
			0	, //	'f'		CH
			0	, //	'g'		CH

			/* 105-112 */
			0	, //	'h'		CH
			0	, //	'i'		CH
			0	, //	'j'		CH
			0	, //	'k'		CH
			0	, //	'l'		CH
			0	, //	'm'		CH
			0	, //	'n'		CH
			0	, //	'o'		CH

			/* 113-120 */
			0	, //	'p'		CH
			0	, //	'q'		CH
			0	, //	'r'		CH
			0	, //	's'		CH
			0	, //	't'		CH
			0	, //	'u'		CH
			0	, //	'v'		CH
			0	, //	'w'		CH

			/* 121-128 */
			0	, //	'x'		CH
			0	, //	'y'		CH
			0	, //	'z'		CH
			1	, //	'{'		CH
			1	, //	'|'		CH
			1	, //	'}'		CH
			1	, //	'~'		CH
			1	, //			DEL(delete)
		};

		GINL GAIA::NUM percentage_encode(GAIA::CH ch, GAIA::CH* pszPercentage, GAIA::NUM sMaxPercentageLen)
		{
			if(((GAIA::U8)ch) < 128 && !ascii_need_percentage[ch])
			{
				if(pszPercentage != GNIL)
				{
					if(sMaxPercentageLen > 1)
					{
						pszPercentage[0] = ch;
						pszPercentage[1] = '\0';
					}
					else
						return GINVALID;
				}
				return 1;
			}

			if(ch == ' ')
			{
				if(pszPercentage != GNIL)
				{
					if(sMaxPercentageLen > 1)
					{
						pszPercentage[0] = '+';
						pszPercentage[1] = '\0';
					}
					else
						return GINVALID;
				}
				return 1;
			}
			else
			{
				if(pszPercentage != GNIL)
				{
					if(sMaxPercentageLen > 3)
					{
						//
						pszPercentage[0] = '%';

						// Calculate high 4 bit.
						GAIA::U8 u0 = (((GAIA::U8)ch) >> 4) & 0x0F;
						if(u0 < 10)
							pszPercentage[1] = '0' + u0;
						else
							pszPercentage[1] = 'A' + u0 - 10;

						// Calculate low 4 bit.
						GAIA::U8 u1 = ((GAIA::U8)ch) & 0x0F;
						if(u1 < 10)
							pszPercentage[2] = '0' + u1;
						else
							pszPercentage[2] = 'A' + u1 - 10;

						//
						pszPercentage[3] = '\0';
					}
					else
						return GINVALID;
				}
				return 3;
			}
		}

		GINL GAIA::BL percentage_decode(GAIA::CH* pszPercentage, GAIA::NUM sPercentageLen, GAIA::CH& ch)
		{
			GAST(pszPercentage != GNIL);
			if(pszPercentage == GNIL)
				return GAIA::False;
			GAST(sPercentageLen > 0);
			if(sPercentageLen <= 0)
				return GAIA::False;

			if(pszPercentage[0] == '%')
			{
				// Calculate high 4 bit.
				GAIA::U8 u0;
				if(pszPercentage[1] >= '0' && pszPercentage[1] <= '9')
					u0 = pszPercentage[1] - '0';
				else if(pszPercentage[1] >= 'a' && pszPercentage[1] <= 'f')
					u0 = pszPercentage[1] - 'a' + 10;
				else if(pszPercentage[1] >= 'A' && pszPercentage[1] <= 'F')
					u0 = pszPercentage[1] - 'A' + 10;
				else
					return GAIA::False;

				// Calculate low 4 bit.
				GAIA::U8 u1;
				if(pszPercentage[2] >= '0' && pszPercentage[2] <= '9')
					u1 = pszPercentage[2] - '0';
				else if(pszPercentage[2] >= 'a' && pszPercentage[2] <= 'f')
					u1 = pszPercentage[2] - 'a' + 10;
				else if(pszPercentage[2] >= 'A' && pszPercentage[2] <= 'F')
					u1 = pszPercentage[2] - 'A' + 10;
				else
					return GAIA::False;

				ch = GSCAST(GAIA::U8)((u0 << 4) + u1);
			}
			else if(pszPercentage[0] == '+')
				ch = ' ';
			else
				ch = pszPercentage[0];

			return GAIA::True;
		}

		GINL GAIA::NUM percentage_encode(const GAIA::CH* pszLocal, GAIA::NUM sLocalLen, GAIA::CH* pszPercentage, GAIA::NUM sMaxPercentageLen)
		{
			// Parameter checkup.
			GAST(pszLocal != GNIL);
			if(pszLocal == GNIL)
				return GINVALID;

			GAST(sLocalLen >= 0);
			if(sLocalLen < 0)
				return GINVALID;

			if(pszPercentage == GNIL)
			{
				GAST(sMaxPercentageLen == 0);
				if(sMaxPercentageLen != 0)
					return GINVALID;
			}
			else
			{
				GAST(sMaxPercentageLen >= 0);
				if(sMaxPercentageLen < 0)
					return GINVALID;
			}

			//
			GAIA::NUM sRet = 0;

			// Calculate length.
			if(pszPercentage == GNIL)
			{
				for(GAIA::NUM x = 0; x < sLocalLen; ++x)
				{
					if(((GAIA::U8)pszLocal[x]) < 128)
					{
						if(!ascii_need_percentage[pszLocal[x]])
							sRet++;
						else if(pszLocal[x] == ' ')
							sRet++;
						else
							sRet += 3;
					}
					else
						sRet += 3;
				}
				return sRet;
			}

			// Encode.
			for(GAIA::NUM x = 0; x < sLocalLen; ++x)
			{
				GAIA::U8 u = pszLocal[x];
				if(u < 128)
				{
					if(!ascii_need_percentage[u])
					{
						if(sRet >= sMaxPercentageLen)
							return GINVALID;
						pszPercentage[sRet++] = u;
					}
					else if(u == ' ')
					{
						if(sRet >= sMaxPercentageLen)
							return GINVALID;
						pszPercentage[sRet++] = '+';
					}
					else
					{
						if(sRet + 2 >= sMaxPercentageLen)
							return GINVALID;

						//
						pszPercentage[sRet] = '%';

						// Calculate high 4 bit.
						GAIA::U8 u0 = (u >> 4) & 0x0F;
						if(u0 < 10)
							pszPercentage[sRet + 1] = '0' + u0;
						else
							pszPercentage[sRet + 1] = 'A' + u0 - 10;

						// Calculate low 4 bit.
						GAIA::U8 u1 = u & 0x0F;
						if(u1 < 10)
							pszPercentage[sRet + 2] = '0' + u1;
						else
							pszPercentage[sRet + 2] = 'A' + u1 - 10;

						sRet += 3;
					}
				}
				else
				{
					if(sRet + 2 >= sMaxPercentageLen)
						return GINVALID;

					//
					pszPercentage[sRet] = '%';

					// Calculate high 4 bit.
					GAIA::U8 u0 = (u >> 4) & 0x0F;
					if(u0 < 10)
						pszPercentage[sRet + 1] = '0' + u0;
					else
						pszPercentage[sRet + 1] = 'A' + u0 - 10;

					// Calculate low 4 bit.
					GAIA::U8 u1 = u & 0x0F;
					if(u1 < 10)
						pszPercentage[sRet + 2] = '0' + u1;
					else
						pszPercentage[sRet + 2] = 'A' + u1 - 10;

					sRet += 3;
				}
			}

			if(sRet >= sMaxPercentageLen)
				return GINVALID;
			pszPercentage[sRet] = '\0';

			return sRet;
		}

		GINL GAIA::NUM percentage_decode(const GAIA::CH* pszPercentage, GAIA::NUM sPercentageLen, GAIA::CH* pszLocal, GAIA::NUM sMaxLocalLen)
		{
			// Parameter checkup.
			GAST(pszPercentage != GNIL);
			if(pszPercentage == GNIL)
				return GINVALID;

			GAST(sPercentageLen >= 0);
			if(sPercentageLen < 0)
				return GINVALID;

			if(pszLocal == GNIL)
			{
				GAST(sMaxLocalLen == 0);
				if(sMaxLocalLen != 0)
					return GINVALID;
			}
			else
			{
				GAST(sMaxLocalLen >= 0);
				if(sMaxLocalLen < 0)
					return GINVALID;
			}

			//
			GAIA::NUM sRet = 0;

			// Calculate length.
			if(pszLocal == GNIL)
			{
				for(GAIA::NUM x = 0; x < sPercentageLen; ++x)
				{
					if(pszPercentage[x] == '%' && x + 2 < sPercentageLen)
					{
						GAIA::CH ch1 = pszPercentage[x + 1];
						GAIA::CH ch2 = pszPercentage[x + 2];
						GAIA::BL b1 = (ch1 >= '0' && ch1 <= '9') || (ch1 >= 'a' && ch1 <= 'f') || (ch1 >= 'A' && ch1 <= 'F');
						GAIA::BL b2 = (ch2 >= '0' && ch2 <= '9') || (ch2 >= 'a' && ch2 <= 'f') || (ch2 >= 'A' && ch2 <= 'F');
						if(b1 && b2)
							x += 2;
					}
					sRet++;
				}
				return sRet;
			}

			// Decode.
			for(GAIA::NUM x = 0; x < sPercentageLen; ++x)
			{
				if(pszPercentage[x] == '%' && x + 2 < sPercentageLen)
				{
					const GAIA::U8* p = (const GAIA::U8*)&pszPercentage[x + 1];
					GAIA::BL b1 = (p[0] >= '0' && p[0] <= '9') || (p[0] >= 'a' && p[0] <= 'f') || (p[0] >= 'A' && p[0] <= 'F');
					GAIA::BL b2 = (p[1] >= '0' && p[1] <= '9') || (p[1] >= 'a' && p[1] <= 'f') || (p[1] >= 'A' && p[1] <= 'F');
					if(b1 && b2)
					{
						if(sRet >= sMaxLocalLen)
							return GINVALID;

						// Calculate high 4 bit.
						GAIA::U8 u0;
						if(p[0] >= '0' && p[0] <= '9')
							u0 = p[0] - '0';
						else if(p[0] >= 'a' && p[0] <= 'f')
							u0 = p[0] - 'a' + 10;
						else if(p[0] >= 'A' && p[0] <= 'F')
							u0 = p[0] - 'A' + 10;
						else
						{
							GASTFALSE;
							u0 = 0;
						}

						// Calculate low 4 bit.
						GAIA::U8 u1;
						if(p[1] >= '0' && p[1] <= '9')
							u1 = p[1] - '0';
						else if(p[1] >= 'a' && p[1] <= 'f')
							u1 = p[1] - 'a' + 10;
						else if(p[1] >= 'A' && p[1] <= 'F')
							u1 = p[1] - 'A' + 10;
						else
						{
							GASTFALSE;
							u1 = 0;
						}

						pszLocal[sRet] = GSCAST(GAIA::U8)((u0 << 4) + u1);

						x += 2;
					}
					else
					{
						if(sRet >= sMaxLocalLen)
							return GINVALID;
						pszLocal[sRet] = pszPercentage[x];
					}
				}
				else
				{
					if(sRet >= sMaxLocalLen)
						return GINVALID;
					if(pszPercentage[x] == '+')
						pszLocal[sRet] = ' ';
					else
						pszLocal[sRet] = pszPercentage[x];
				}
				sRet++;
			}

			if(sRet >= sMaxLocalLen)
				return GINVALID;
			pszLocal[sRet] = '\0';

			return sRet;
		}
	}
}

#endif
