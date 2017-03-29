#ifndef		__GAIA_ALGO_STRING_H__
#define		__GAIA_ALGO_STRING_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_math_base.h"
#include "gaia_algo_base.h"

namespace GAIA
{
	namespace ALGO
	{
		template<typename _DataType> _DataType tolower(const _DataType& c){if(c >= 'A' && c <= 'Z') return c - 'A' + 'a'; return c;}
		template<typename _DataType> _DataType toupper(const _DataType& c){if(c >= 'a' && c <= 'z') return c - 'a' + 'A'; return c;}
		template<typename _DataType> _DataType tolowers(_DataType p){GAST(!!p); _DataType ret = p; while(*p != '\0'){*p = GAIA::ALGO::tolower(*p); p++;} return ret;}
		template<typename _DataType> _DataType touppers(_DataType p){GAST(!!p); _DataType ret = p; while(*p != '\0'){*p = GAIA::ALGO::toupper(*p); p++;} return ret;}
		template<typename _DataType> GAIA::BL islower(const _DataType& c){return c >= 'a' && c <= 'z';}
		template<typename _DataType> GAIA::BL isupper(const _DataType& c){return c >= 'A' && c <= 'Z';}
		template<typename _DataType> GAIA::BL isexistlower(_DataType p){GAST(!!p); while(*p != '\0'){if(GAIA::ALGO::islower(*p)) return GAIA::True; p++;} return GAIA::False;}
		template<typename _DataType> GAIA::BL isexistupper(_DataType p){GAST(!!p); while(*p != '\0'){if(GAIA::ALGO::isupper(*p)) return GAIA::True; p++;} return GAIA::False;}
		template<typename _DataType> GAIA::BL isalllower(_DataType p){GAST(!!p); while(*p != '\0'){if(!GAIA::ALGO::islower(*p)) return GAIA::False; p++;} return GAIA::True;}
		template<typename _DataType> GAIA::BL isallupper(_DataType p){GAST(!!p); while(*p != '\0'){if(!GAIA::ALGO::isupper(*p)) return GAIA::False; p++;} return GAIA::True;}
		template<typename _DataType> GAIA::BL isdigit(const _DataType& c){return c >= '0' && c <= '9';}
		template<typename _DataType> GAIA::BL isalpha(const _DataType& c){if(c < 'A' || c > 'z') return GAIA::False; return c <= 'Z' || c >= 'a';}
		template<typename _DataType> GAIA::BL isspecial(const _DataType& c){return c == '\r' || c == '\n';}
		template<typename _DataType> GAIA::BL isblank(const _DataType& c){return c == ' ' || c == '\t';}
		template<typename _DataType> GAIA::BL ispunc(const _DataType& c)
		{
			static const GAIA::U8 puns[] =
			{
				0, /* 0 '\0' */ 0, /* 1 '' */ 0, /* 2 '' */ 0, /* 3 '' */ 0, /* 4 '' */ 0, /* 5 '' */ 0, /* 6 '' */ 0, /* 7 '' */ 0, /* 8 ' */ 0, /* 9 '       ' */
				0, /* 10 '\n' */ 0, /* 11 '' */ 0, /* 12 '' */ 0, /* 13 '\r' */ 0, /* 14 '' */ 0, /* 15 '' */ 0, /* 16 '' */ 0, /* 17 '' */ 0, /* 18 '' */ 0, /* 19 '' */
				0, /* 20 '' */ 0, /* 21 '' */ 0, /* 22 '' */ 0, /* 23 '' */ 0, /* 24 '' */ 0, /* 25 '' */ 0, /* 26 '' */ 0, /* 27 '' */ 0, /* 28 '' */ 0, /* 29 '' */
				0, /* 30 '' */ 0, /* 31 '' */ 0, /* 32 ' ' */ 1, /* 33 '!' */ 1, /* 34 '"' */ 1, /* 35 '#' */ 1, /* 36 '$' */ 1, /* 37 '%' */ 1, /* 38 '&' */ 1, /* 39 ''' */
				1, /* 40 '(' */ 1, /* 41 ')' */ 1, /* 42 '*' */ 1, /* 43 '+' */ 1, /* 44 ',' */ 1, /* 45 '-' */ 1, /* 46 '.' */ 1, /* 47 '/' */ 0, /* 48 '0' */ 0, /* 49 '1' */
				0, /* 50 '2' */ 0, /* 51 '3' */ 0, /* 52 '4' */ 0, /* 53 '5' */ 0, /* 54 '6' */ 0, /* 55 '7' */ 0, /* 56 '8' */ 0, /* 57 '9' */ 1, /* 58 ':' */ 1, /* 59 ';' */
				1, /* 60 '<' */ 1, /* 61 '=' */ 1, /* 62 '>' */ 1, /* 63 '?' */ 1, /* 64 '@' */ 0, /* 65 'A' */ 0, /* 66 'B' */ 0, /* 67 'C' */ 0, /* 68 'D' */ 0, /* 69 'E' */
				0, /* 70 'F' */ 0, /* 71 'G' */ 0, /* 72 'H' */ 0, /* 73 'I' */ 0, /* 74 'J' */ 0, /* 75 'K' */ 0, /* 76 'L' */ 0, /* 77 'M' */ 0, /* 78 'N' */ 0, /* 79 'O' */
				0, /* 80 'P' */ 0, /* 81 'Q' */ 0, /* 82 'R' */ 0, /* 83 'S' */ 0, /* 84 'T' */ 0, /* 85 'U' */ 0, /* 86 'V' */ 0, /* 87 'W' */ 0, /* 88 'X' */ 0, /* 89 'Y' */
				0, /* 90 'Z' */ 1, /* 91 '[' */ 1, /* 92 '\\' */ 1, /* 93 ']' */ 1, /* 94 '^' */ 1, /* 95 '_' */ 1, /* 96 '`' */ 0, /* 97 'a' */ 0, /* 98 'b' */ 0, /* 99 'c' */
				0, /* 100 'd' */ 0, /* 101 'e' */ 0, /* 102 'f' */ 0, /* 103 'g' */ 0, /* 104 'h' */ 0, /* 105 'i' */ 0, /* 106 'j' */ 0, /* 107 'k' */ 0, /* 108 'l' */ 0, /* 109 'm' */
				0, /* 110 'n' */ 0, /* 111 'o' */ 0, /* 112 'p' */ 0, /* 113 'q' */ 0, /* 114 'r' */ 0, /* 115 's' */ 0, /* 116 't' */ 0, /* 117 'u' */ 0, /* 118 'v' */ 0, /* 119 'w' */
				0, /* 120 'x' */ 0, /* 121 'y' */ 0, /* 122 'z' */ 1, /* 123 '{' */ 1, /* 124 '|' */ 1, /* 125 '}' */ 1, /* 126 '~' */ 0, /* 127 '' */
			};
			if(c >= sizeof(puns))
				return GAIA::False;
			return puns[c];
		}
		template<typename _DataType> GAIA::BL isansi(const _DataType& c)
		{
			if(c >= 0 && c < 128)
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType> GAIA::BL isexistansi(_DataType p)
		{
			while(*p != '\0')
			{
				if(*p >= 0 && *p < 128)
					return GAIA::True;
				++p;
			}
			return GAIA::False;
		}
		template<typename _DataType> GAIA::BL isallansi(_DataType p)
		{
			while(*p != '\0')
			{
				if(*p < 0 || *p >= 128)
					return GAIA::False;
				++p;
			}
			return GAIA::True;
		}
		template<typename _DataType> GAIA::BL gstremp(_DataType p)
		{
			if(p == GNIL || *p == '\0')
				return GAIA::True;
			return GAIA::False;
		}
		template<typename _DataType> _DataType gstrend(_DataType p)
		{
			while(*p != '\0') ++p;
			return p;
		}
		template<typename _DataType1, typename _DataType2> _DataType1 gstrch(_DataType1 p, const _DataType2& c)
		{
			while(*p != '\0')
			{
				if(*p == c)
					return p;
				++p;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2> _DataType1 gstrich(_DataType1 p, const _DataType2& c)
		{
			while(*p != '\0')
			{
				if(GAIA::ALGO::tolower(*p) == GAIA::ALGO::tolower(c))
					return p;
				++p;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2> _DataType1 gstrchs(_DataType1 p, _DataType2 key)
		{
			while(*p != '\0')
			{
				_DataType2 pTemp = key;
				while(*pTemp != '\0')
				{
					if(*p == *pTemp)
						return p;
					++pTemp;
				}
				++p;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2> _DataType1 gstrichs(_DataType1 p, _DataType2 key)
		{
			while(*p != '\0')
			{
				_DataType2 pTemp = key;
				while(*pTemp != '\0')
				{
					if(GAIA::ALGO::tolower(*p) == GAIA::ALGO::tolower(*pTemp))
						return p;
					++pTemp;
				}
				++p;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2> GAIA::BL gstrstartwith(_DataType1 p1, _DataType2 p2)
		{
			while(*p2 != '\0')
			{
				if(*p1++ != *p2++)
					return GAIA::False;
			}
			return GAIA::True;
		}
		template<typename _DataType1, typename _DataType2> GAIA::BL gstrendwith(_DataType1 p1, _DataType2 p2)
		{
			_DataType1 e1 = GAIA::ALGO::gstrend(p1);
			_DataType2 e2 = GAIA::ALGO::gstrend(p2);
			while(e2 >= p2)
			{
				if(*e1-- != *e2--)
					return GAIA::False;
			}
			return GAIA::True;
		}
		template<typename _DataType1, typename _DataType2> GAIA::BL gstrstartwithi(_DataType1 p1, _DataType2 p2)
		{
			while(*p2 != '\0')
			{
				if(GAIA::ALGO::tolower(*p1++) != GAIA::ALGO::tolower(*p2++))
					return GAIA::False;
			}
			return GAIA::True;
		}
		template<typename _DataType1, typename _DataType2> GAIA::BL gstrendwithi(_DataType1 p1, _DataType2 p2)
		{
			_DataType1 e1 = GAIA::ALGO::gstrend(p1);
			_DataType2 e2 = GAIA::ALGO::gstrend(p2);
			while(e2 >= p2)
			{
				if(GAIA::ALGO::tolower(*e1--) != GAIA::ALGO::tolower(*e2--))
					return GAIA::False;
			}
			return GAIA::True;
		}
		template<typename _DataType> GAIA::NUM gstrlen(_DataType p){GAST(!!p); GAIA::NUM ret = 0; while(p[ret] != '\0') ret++; return ret;}
		template<typename _DataType> GAIA::NUM gstrlennil(_DataType p){if(p == GNIL) return 0; return GAIA::ALGO::gstrlen(p);}
		template<typename _DataType1, typename _DataType2> GAIA::NUM gstrcnt(_DataType1 p, const _DataType2& key)
		{
			GAIA::NUM ret = 0;
			while(*p != '\0')
			{
				if(*p == key)
					++ret;
				++p;
			}
			return ret;
		}
		template<typename _DataType1, typename _DataType2> GAIA::NUM gstrcnts(_DataType1 p, _DataType2 keys)
		{
			GAIA::NUM ret = 0;
			while(*keys != '\0')
			{
				ret += GAIA::ALGO::gstrcnt(p, *keys);
				++keys;
			}
			return ret;
		}
		template<typename _DataType1, typename _DataType2> _DataType1 gstrcpy(_DataType1 pDst, _DataType2 pSrc)
		{
			GAST(!!pDst);
			GAST(!!pSrc);
			_DataType1 ret = pDst;
			while(*pSrc != '\0')
			{
				*pDst = *pSrc;
				++pDst;
				++pSrc;
			}
			*pDst = '\0';
			return ret;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		_DataType1 gstrcpy(_DataType1 pDst, _DataType2 pSrc, _SizeType size)
		{
			GAST(!!pDst);
			GAST(!!pSrc);
			GAST(size >= 0);
			_DataType1 ret = pDst;
			while(size != 0)
			{
				*pDst = *pSrc;
				++pDst;
				++pSrc;
				--size;
			}
			*pDst = '\0';
			return ret;
		}
		template<typename _DataType1, typename _DataType2> _DataType1 gstradd(_DataType1 pDst, _DataType2 pSrc)
		{
			GAST(!!pDst);
			GAST(!!pSrc);
			while(*pSrc != '\0')
			{
				*pDst = *pSrc;
				++pDst;
				++pSrc;
			}
			*pDst = '\0';
			return pDst;
		}
		template<typename _DataType1, typename _DataType2> _DataType1 gstradd_notend(_DataType1 pDst, _DataType2 pSrc)
		{
			GAST(!!pDst);
			GAST(!!pSrc);
			while(*pSrc != '\0')
			{
				*pDst = *pSrc;
				++pDst;
				++pSrc;
			}
			return pDst;
		}
		template<typename _DataType> _DataType* gstrnew(const _DataType* p)
		{
			_DataType* ret = gnew _DataType[GAIA::ALGO::gstrlen(p) + 1];
			GAIA::ALGO::gstrcpy(ret, p);
			return ret;
		}
		template<typename _DataTypeDst, typename _DataTypeSrc>
		_DataTypeDst* gstrnewex(const _DataTypeSrc* p)
		{
			_DataTypeDst* ret = gnew _DataTypeDst[GAIA::ALGO::gstrlen(p) + 1];
			GAIA::ALGO::gstrcpy(ret, p);
			return ret;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstrcat(_DataType1 pDst, _DataType2 pSrc)
		{
			GAST(!!pDst);
			GAST(!!pSrc);
			_DataType1 ret = pDst;
			pDst = pDst + GAIA::ALGO::gstrlen(pDst);
			GAIA::ALGO::gstrcpy(pDst, pSrc);
			return ret;
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 gstrcmp(_DataType1 p1, _DataType2 p2)
		{
			GAST(!!p1);
			GAST(!!p2);
			for(;;)
			{
				if(*p1 < *p2)
					return -1;
				else if(*p1 > *p2)
					return +1;
				else
				{
					if(*p1 == '\0')
						return 0;
					++p1;
					++p2;
				}
			}
		#if GAIA_COMPILER != GAIA_COMPILER_VC // For CL C4702 Warning.
			return 0;
		#endif
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 gstricmp(_DataType1 p1, _DataType2 p2)
		{
			GAST(!!p1);
			GAST(!!p2);
			for(;;)
			{
				if(GAIA::ALGO::tolower(*p1) < GAIA::ALGO::tolower(*p2))
					return -1;
				else if(GAIA::ALGO::tolower(*p1) > GAIA::ALGO::tolower(*p2))
					return +1;
				else
				{
					if(*p1 == '\0')
						return 0;
					++p1;
					++p2;
				}
			}
		#if GAIA_COMPILER != GAIA_COMPILER_VC // For CL C4702 Warning.
			return 0;
		#endif
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::N32 gstrcmp(_DataType1 p1, _DataType2 p2, _SizeType size)
		{
			for(; size != 0; --size)
			{
				if(*p1 < *p2)
					return -1;
				else if(*p1 > *p2)
					return +1;
				else
				{
					if(*p1 == '\0')
						return 0;
					++p1;
					++p2;
				}
			}
			return 0;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::N32 gstricmp(_DataType1 p1, _DataType2 p2, _SizeType size)
		{
			for(; size != 0; --size)
			{
				if(GAIA::ALGO::tolower(*p1) < GAIA::ALGO::tolower(*p2))
					return -1;
				else if(GAIA::ALGO::tolower(*p1) > GAIA::ALGO::tolower(*p2))
					return +1;
				else
				{
					if(*p1 == '\0')
						return 0;
					++p1;
					++p2;
				}
			}
			return 0;
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 gstrcmpnil(_DataType1 p1, _DataType2 p2)
		{
			if(p1 == GNIL && p2 == GNIL)
				return 0;
			else if(p1 != GNIL && p2 == GNIL)
				return +1;
			else if(p1 == GNIL && p2 != GNIL)
				return -1;
			else
				return GAIA::ALGO::gstrcmp(p1, p2);
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::N32 gstricmpnil(_DataType1 p1, _DataType2 p2)
		{
			if(p1 == GNIL && p2 == GNIL)
				return 0;
			if(p1 == GNIL && p2 == GNIL)
				return 0;
			else if(p1 != GNIL && p2 == GNIL)
				return +1;
			else if(p1 == GNIL && p2 != GNIL)
				return -1;
			else
				return GAIA::ALGO::gstricmp(p1, p2);
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::BL gstrequal(_DataType1 p1, _DataType2 p2)
		{
			GAST(!!p1);
			GAST(!!p2);
			return GAIA::ALGO::gstrcmp(p1, p2) == 0;
		}
		template<typename _DataType1, typename _DataType2>
		GAIA::BL gstriequal(_DataType1 p1, _DataType2 p2)
		{
			GAST(!!p1);
			GAST(!!p2);
			return GAIA::ALGO::gstricmp(p1, p2) == 0;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::BL gstrequal(_DataType1 p1, _DataType2 p2, _SizeType size)
		{
			GAST(!!p1);
			GAST(!!p2);
			return GAIA::ALGO::gstrcmp(p1, p2, size) == 0;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		GAIA::BL gstriequal(_DataType1 p1, _DataType2 p2, _SizeType size)
		{
			GAST(!!p1);
			GAST(!!p2);
			return GAIA::ALGO::gstricmp(p1, p2, size) == 0;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstrstr(_DataType1 p1, _DataType2 p2)
		{
			GAST(!!p1);
			GAST(!!p2);
			while(*p1 != '\0')
			{
				_DataType1 po = p1;
				_DataType2 p = p2;
				while(*po != '\0' && *p != '\0')
				{
					if(*p != *po)
						break;
					++po;
					++p;
				}
				if(*p == '\0')
					return p1;
				++p1;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstristr(_DataType1 p1, _DataType2 p2)
		{
			GAST(!!p1);
			GAST(!!p2);
			while(*p1 != '\0')
			{
				_DataType1 po = p1;
				_DataType2 p = p2;
				while(*po != '\0' && *p != '\0')
				{
					if(GAIA::ALGO::tolower(*p) != GAIA::ALGO::tolower(*po))
						break;
					++po;
					++p;
				}
				if(*p == '\0')
					return p1;
				++p1;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2, typename _DataType3>
		_DataType1 gstrstr(_DataType1 p1, _DataType2 p2, _DataType3 size)
		{
			GAST(!!p1);
			GAST(!!p2);
			_DataType3 p2size = GAIA::ALGO::gstrlen(p2);
			_DataType1 p1head = p1;
			_DataType3 p1validsize = size - p2size;
			while(*p1 != '\0' && p1 - p1head <= p1validsize)
			{
				_DataType1 po = p1;
				_DataType2 p = p2;
				while(*p != '\0')
				{
					if(*p != *po)
						break;
					++po;
					++p;
				}
				if(p - p2 == p2size)
					return p1;
				++p1;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2, typename _DataType3>
		_DataType1 gstristr(_DataType1 p1, _DataType2 p2, _DataType3 size)
		{
			GAST(!!p1);
			GAST(!!p2);
			_DataType3 p2size = GAIA::ALGO::gstrlen(p2);
			_DataType1 p1head = p1;
			_DataType3 p1validsize = size - p2size;
			while(*p1 != '\0' && p1 - p1head <= p1validsize)
			{
				_DataType1 po = p1;
				_DataType2 p = p2;
				while(*p != '\0')
				{
					if(GAIA::ALGO::tolower(*p) != GAIA::ALGO::tolower(*po))
						break;
					++po;
					++p;
				}
				if(p - p2 == p2size)
					return p1;
				++p1;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1* gstrwrd(const _DataType1* p1, const _DataType2* p2)
		{
			const _DataType1* p = p1;
			while((p = GAIA::ALGO::gstrstr(p, p2)) != GNIL)
			{
				if(p != p1)
				{
					_DataType1 t = *(p - 1);
					if(GAIA::ALGO::isalpha(t) || GAIA::ALGO::isdigit(t))
					{
						++p;
						continue;
					}
				}
				_DataType1 t = *(p + GAIA::ALGO::gstrlen(p2));
				if(GAIA::ALGO::isalpha(t) || GAIA::ALGO::isdigit(t))
				{
					++p;
					continue;
				}
				return const_cast<_DataType1*>(p);
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1* gstriwrd(const _DataType1* p1, const _DataType2* p2)
		{
			const _DataType1* p = p1;
			while((p = GAIA::ALGO::gstristr(p, p2)) != GNIL)
			{
				if(p != p1)
				{
					_DataType1 t = *(p - 1);
					if(GAIA::ALGO::isalpha(t) || GAIA::ALGO::isdigit(t))
					{
						++p;
						continue;
					}
				}
				_DataType1 t = *(p + GAIA::ALGO::gstrlen(p2));
				if(GAIA::ALGO::isalpha(t) || GAIA::ALGO::isdigit(t))
				{
					++p;
					continue;
				}
				return const_cast<_DataType1*>(p);
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstrmch(_DataType1 p1, _DataType2 p2)
		{
			GAST(!!p1);
			GAST(!!p2);

			_DataType1 p = p1;
			while(*p != '\0' && *p2 != '\0')
			{
				if(*p2 != *p)
					return GNIL;
				++p;
				++p2;
			}
			if(*p2 == '\0')
				return p1;
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstrimch(_DataType1 p1, _DataType2 p2)
		{
			GAST(!!p1);
			GAST(!!p2);

			_DataType1 p = p1;
			while(GAIA::ALGO::tolower(*p) != '\0' && GAIA::ALGO::tolower(*p2) != '\0')
			{
				if(GAIA::ALGO::tolower(*p2) != GAIA::ALGO::tolower(*p))
					return GNIL;
				++p;
				++p2;
			}
			if(*p2 == '\0')
				return p1;
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstrdrop(_DataType1 p, _DataType2 pKeys)
		{
			while(*p != '\0')
			{
				_DataType2 p2 = pKeys;
				while(*p2 != '\0')
				{
					if(*p == *p2)
						return p;
					++p2;
				}
				++p;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstridrop(_DataType1 p, _DataType2 pKeys)
		{
			while(*p != '\0')
			{
				_DataType2 p2 = pKeys;
				while(*p2 != '\0')
				{
					if(GAIA::ALGO::tolower(*p) == GAIA::ALGO::tolower(*p2))
						return p;
					++p2;
				}
				++p;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstrdropr(_DataType1 p, _DataType2 pKeys)
		{
			_DataType1 p1 = GAIA::ALGO::gstrend(p);
			while(p1 > p)
			{
				_DataType2 p2 = pKeys;
				while(*p2 != '\0')
				{
					if(*p1 == *p2)
					{
						*(p1 + 1) = '\0';
						return p1;
					}
					++p2;
				}
				--p1;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstridropr(_DataType1 p, _DataType2 pKeys)
		{
			_DataType1 p1 = GAIA::ALGO::gstrend(p);
			while(p1 > p)
			{
				_DataType2 p2 = pKeys;
				while(*p2 != '\0')
				{
					if(GAIA::ALGO::tolower(*p1) == GAIA::ALGO::tolower(*p2))
					{
						*(p1 + 1) = '\0';
						return p1;
					}
					++p2;
				}
				--p1;
			}
			return GNIL;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstrtrim(_DataType1 p, _DataType2 pKeys)
		{
			while(GAIA::ALGO::gstrch(pKeys, *p) != GNIL)
				++p;
			return p;
		}
		template<typename _DataType1, typename _DataType2>
		_DataType1 gstritrim(_DataType1 p, _DataType2 pKeys)
		{
			while(GAIA::ALGO::gstrich(pKeys, *p) != GNIL)
				++p;
			return p;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		_SizeType gstrfillleft(_DataType1 p, _DataType2 fill, const _SizeType& size)
		{
			GPCHR_NULL_RET(p, 0);
			GPCHR_BELOWEQUALZERO_RET(size, 0);
			_SizeType l = GSCAST(_SizeType)(GAIA::ALGO::gstrlen(p));
			if(l >= size)
				return 0;
			GAIA::ALGO::move_right(p + size, p + l, l + 1);
			_SizeType fsize = size - l;
			for(_SizeType x = 0; x < fsize; ++x)
				p[x] = fill;
			GAIA::ALGO::gstrlen(p);
			return fsize;
		}
		template<typename _DataType1, typename _DataType2, typename _SizeType>
		_SizeType gstrfillright(_DataType1 p, _DataType2 fill, const _SizeType& size)
		{
			GPCHR_NULL_RET(p, 0);
			GPCHR_BELOWEQUALZERO_RET(size, 0);
			_SizeType l = GSCAST(_SizeType)(GAIA::ALGO::gstrlen(p));
			if(l >= size)
				return 0;
			for(_SizeType x = l; x < size; ++x)
				p[x] = fill;
			p[size] = '\0';
			return size - l;
		}
		template<typename _DataType> _DataType gstrext(_DataType p)
		{
			_DataType pEnd = GAIA::ALGO::gstrend(p);
			--pEnd;
			while(pEnd >= p)
			{
				if(*pEnd == '\\' || *pEnd == '/')
					return GNIL;
				if(*pEnd == '.')
				{
					if(pEnd[1] != '\0')
						return pEnd + 1;
				}
				--pEnd;
			}
			return GNIL;
		}
		template<typename _DataType> _DataType gstrfilename(_DataType p)
		{
			_DataType pEnd = GAIA::ALGO::gstrend(p);
			--pEnd;
			while(pEnd >= p)
			{
				if(*pEnd == '\\' || *pEnd == '/')
				{
					if(pEnd[1] != '\0')
						return pEnd + 1;
				}
				--pEnd;
			}
			return GNIL;
		}
		template<typename _DataType> _DataType gstrpath(_DataType p)
		{
			_DataType pTemp = GAIA::ALGO::gstrfilename(p);
			if(pTemp == GNIL)
				return GNIL;
			*pTemp = '\0';
			return p;
		}
		template<typename _DstDataType> _DstDataType hex2ch(GAIA::U8 u, _DstDataType pDst, GAIA::NUM sDstLen)
		{
			GAST(!!pDst);
			GAST(sDstLen >= 3);

			// Calculate high 4 bit.
			GAIA::U8 u0 = (u >> 4) & 0x0F;
			if(u0 < 10)
				pDst[0] = '0' + u0;
			else
				pDst[0] = 'A' + u0 - 10;

			// Calculate low 4 bit.
			GAIA::U8 u1 = u & 0x0F;
			if(u1 < 10)
				pDst[1] = '0' + u1;
			else
				pDst[1] = 'A' + u1 - 10;

			//
			pDst[2] = '\0';

			return pDst;
		}
		template<typename _SrcDataType> GAIA::U8 ch2hex(_SrcDataType pSrc, GAIA::NUM sSrcLen)
		{
			GAST(!!pSrc);
			GAST(sSrcLen >= 2);

			// Calculate high 4 bit.
			GAIA::U8 u0;
			if(pSrc[0] >= '0' && pSrc[0] <= '9')
				u0 = pSrc[0] - '0';
			else if(pSrc[0] >= 'a' && pSrc[0] <= 'f')
				u0 = pSrc[0] - 'a' + 10;
			else if(pSrc[0] >= 'A' && pSrc[0] <= 'F')
				u0 = pSrc[0] - 'A' + 10;
			else
			{
				GASTFALSE;
				u0 = 0;
			}

			// Calculate low 4 bit.
			GAIA::U8 u1;
			if(pSrc[1] >= '0' && pSrc[1] <= '9')
				u1 = pSrc[1] - '0';
			else if(pSrc[1] >= 'a' && pSrc[1] <= 'f')
				u1 = pSrc[1] - 'a' + 10;
			else if(pSrc[1] >= 'A' && pSrc[1] <= 'F')
				u1 = pSrc[1] - 'A' + 10;
			else
			{
				GASTFALSE;
				u1 = 0;
			}

			return GSCAST(GAIA::U8)((u0 << 4) + u1);
		}
		template<typename _SrcDataType, typename _DstDataType>
		_DstDataType int2str(const _SrcDataType& src, _DstDataType pDst)
		{
			GAST(!!pDst);
			_SrcDataType tsrc = src;
			_DstDataType p = pDst;
			if(src < (_SrcDataType)0)
			{
			#ifdef GAIA_DEBUG_WARNING
			#	if GAIA_COMPILER == GAIA_COMPILER_VC
			#		pragma warning(disable : 4146)
			#	endif
			#endif
				tsrc = -tsrc;
			#ifdef GAIA_DEBUG_WARNING
			#	if GAIA_COMPILER == GAIA_COMPILER_VC
			#		pragma warning(default : 4146)
			#	endif
			#endif
				*p = '-';
				++p;
			}
			while(tsrc > 0)
			{
				*p = (tsrc % 10 + '0');
				tsrc /= 10;
				++p;
			}
			if(p == pDst)
			{
				*p = '0';
				++p;
			}
			else
				GAIA::ALGO::inverse(*pDst == '-' ? pDst + 1 : pDst, p - 1);
			*p = '\0';
			return p + 1;
		}
		template<typename _SrcDataType, typename _DstDataType>
		_DstDataType real2str(const _SrcDataType& src, _DstDataType pDst)
		{
			GAST(!!pDst);
			GAIA::N64 left = (GAIA::N64)src;
			_SrcDataType right = src - (_SrcDataType)left;
			right = GAIA::MATH::gabs(right);
			for(GAIA::NUM x = 0; x < sizeof(src) * 2; ++x)
			{
				if(right - GSCAST(_SrcDataType)(GSCAST(GAIA::N64)(right)) == 0)
					break;
				right *= R(10.0);
			}
			_DstDataType p = GAIA::ALGO::int2str(left, pDst);
			p[-1] = '.';
			p = GAIA::ALGO::int2str((GAIA::N64)right, p);
			return p;
		}
		template<typename _DstDataType, typename _SizeType>
		_DstDataType hex2str(const GAIA::U8* pSrc, const _SizeType& size, _DstDataType pDst)
		{
			GAST(!!pSrc);
			GAST(!!pDst);
			GAST(size > 0);
			_SizeType sizet = size;
			_DstDataType p = pDst;
			pSrc = pSrc + size - 1;
			while(sizet > 0)
			{
				// Calculate high 4 bit.
				GAIA::U8 u0 = ((*pSrc) >> 4) & 0x0F;
				if(u0 < 10)
					*p = '0' + u0;
				else
					*p = 'A' + u0 - 10;
				++p;

				// Calculate low 4 bit.
				GAIA::U8 u1 = (*pSrc) & 0x0F;
				if(u1 < 10)
					*p = '0' + u1;
				else
					*p = 'A' + u1 - 10;
				++p;

				// Combine.
				--pSrc;
				--sizet;
			}
			*p = '\0';
			return p + 1;
		}
		template<typename _DstDataType> _DstDataType bool2str(GAIA::BL b, _DstDataType pDst)
		{
			GAST(!!pDst);
			if(b)
			{
				pDst[0] = '1';
				pDst[1] = '\0';
			}
			else
			{
				pDst[0] = '0';
				pDst[1] = '\0';
			}
			return pDst + 2;
		}
		template<typename _SrcDataType, typename _SizeType>
		GAIA::U8* str2hex(_SrcDataType pSrc, const _SizeType& size, GAIA::U8* pDst)
		{
			GAST(!!pSrc);
			GAST(!!pDst);
			GAST(size > 0);
			GAST(size % 2 == 0);
			_SizeType sizet = size;
			GAIA::U8* p = pDst + size - 1;
			while(sizet > 0)
			{
				// Calculate high 4 bit.
				GAIA::U8 u0;
				if(*pSrc >= '0' && *pSrc <= '9')
					u0 = *pSrc - '0';
				else if(*pSrc >= 'a' && *pSrc <= 'f')
					u0 = *pSrc - 'a' + 10;
				else if(*pSrc >= 'A' && *pSrc <= 'F')
					u0 = *pSrc - 'A' + 10;
				else
				{
					GASTFALSE;
					u0 = 0;
				}
				++pSrc;

				// Calculate low 4 bit.
				GAIA::U8 u1;
				if(*pSrc >= '0' && *pSrc <= '9')
					u1 = *pSrc - '0';
				else if(*pSrc >= 'a' && *pSrc <= 'f')
					u1 = *pSrc - 'a' + 10;
				else if(*pSrc >= 'A' && *pSrc <= 'F')
					u1 = *pSrc - 'A' + 10;
				else
				{
					GASTFALSE;
					u1 = 0;
				}
				++pSrc;

				// Combine.
				*p = GSCAST(GAIA::U8)((u0 << 4) + u1);
				--p;
				--sizet;
			}
			return pDst;
		}
		template<typename _SrcDataType, typename _DstDataType>
		_SrcDataType str2int(_SrcDataType pSrc, _DstDataType& dst)
		{
			GAST(!!pSrc);
			dst = 0;
			GAIA::BL bSign;
			if(*pSrc == '-')
			{
				bSign = GAIA::True;
				++pSrc;
			}
			else
				bSign = GAIA::False;
			while(*pSrc != '\0')
			{
				if(*pSrc < '0' || *pSrc > '9')
					break;
				dst = GSCAST(_DstDataType)(dst * 10);
				dst = GSCAST(_DstDataType)(dst + (*pSrc - '0'));
				++pSrc;
			}
			if(bSign)
				dst = 0 - dst;
			return pSrc;
		}
		template<typename _SrcDataType, typename _DstDataType>
		_SrcDataType str2real(_SrcDataType pDst, _DstDataType& dst)
		{
			GAST(!!pDst);
			GAIA::BL bSign;
			if(*pDst == '-')
			{
				bSign = GAIA::True;
				++pDst;
			}
			else
				bSign = GAIA::False;
			_DstDataType left = 0;
			_DstDataType right = 0;
			_DstDataType right_dst;
			_DstDataType* pTarget = &left;
			while(*pDst != '\0')
			{
				if(*pDst < '0' || *pDst > '9')
					break;
				*pTarget *= 10;
				*pTarget += (*pDst - '0');
				++pDst;
			}
			if(*pDst == '.')
			{
				++pDst;
				_DstDataType right_div = 1;
				pTarget = &right;
				while(*pDst != '\0')
				{
					if(*pDst < '0' || *pDst > '9')
						break;
					*pTarget *= 10;
					right_div *= 10;
					*pTarget += (*pDst - '0');
					++pDst;
				}
				right_dst = *pTarget / right_div;
			}
			else
				right_dst = R(0.0);
			dst = left + right_dst;
			if(bSign)
				dst = -dst;
			return pDst;
		}
		template<typename _SrcDataType> _SrcDataType str2bool(_SrcDataType pSrc, GAIA::BL& b)
		{
			GAST(!!pSrc);
			if(pSrc[0] == '0')
				b = GAIA::False;
			else
				b = GAIA::True;
			return pSrc + 1;
		}
		template<typename _DstDataType> _DstDataType x1282str(const X128& src, _DstDataType pDst)
		{
			GAST(pDst != GNIL);
			GAIA::ALGO::hex2str((const GAIA::U8*)&src, sizeof(src), pDst);
			return pDst + sizeof(src) * 2;
		}

		template<typename _SrcDataType> _SrcDataType str2x128(_SrcDataType pSrc, X128& dst)
		{
			GAST(pSrc != GNIL);
			GAIA::ALGO::str2hex(pSrc, sizeof(dst), (GAIA::U8*)&dst);
			return pSrc + sizeof(dst) * 2;
		}
		template<typename _DataType> GAIA::U32 strtype(_DataType p)
		{
			if(GAIA::ALGO::gstremp(p))
				return GAIA::STRING_TYPE_INVALID;

			/* Check up the string is RID. */
			GAIA::NUM len = GAIA::ALGO::gstrlen(p);
			if(len == 8 || len == 16 || len == 32)
			{
				GAIA::BL bMatch = GAIA::True;
				_DataType pTemp = p;
				while(*pTemp != '\0')
				{
					if(	(*pTemp >= 'a' && *pTemp <= 'f') ||
						(*pTemp >= 'A' && *pTemp <= 'F') ||
						(*pTemp >= '0' && *pTemp <= '9')){}
					else
					{
						bMatch = GAIA::False;
						break;
					}
					++pTemp;
				}
				if(bMatch)
				{
					if(len == 8)
						return GAIA::STRING_TYPE_RID | GAIA::STRING_TYPE_RID32;
					else if(len == 16)
						return GAIA::STRING_TYPE_RID | GAIA::STRING_TYPE_RID64;
					else if(len == 32)
						return GAIA::STRING_TYPE_RID | GAIA::STRING_TYPE_RID128;
				}
			}

			/* Check up the string is real. */
			{
				GAIA::BL bMatch = GAIA::True;
				GAIA::BL bExistPoint = GAIA::False;
				_DataType pTemp = p;
				while(*pTemp != '\0')
				{
					if((*pTemp == '.') ||
						(*pTemp >= '0' && *pTemp <= '9') ||
						(pTemp == p && (*pTemp == '-' || *pTemp == '+')) ||
						(pTemp[1] == '\0' && (*pTemp == 'f' || *pTemp == 'F')))
					{
						if(*pTemp == '.')
							bExistPoint = GAIA::True;
					}
					else
					{
						bMatch = GAIA::False;
						break;
					}
					++pTemp;
				}
				if(bMatch)
				{
					if(bExistPoint)
						return GAIA::STRING_TYPE_REAL;
				}
			}

			/* Check up the string is integer. */
			{
				GAIA::BL bMatch = GAIA::True;
				_DataType pTemp = p;
				while(*pTemp != '\0')
				{
					if((*pTemp >= '0' && *pTemp <= '9') ||
						(pTemp == p && (*pTemp == '-' || *pTemp == '+'))){}
					else
					{
						bMatch = GAIA::False;
						break;
					}
					++pTemp;
				}
				if(bMatch)
					return GAIA::STRING_TYPE_INTEGER;
			}

			return GAIA::STRING_TYPE_STRING;
		}
		template<typename _DataType> class casts;
		#define GAIA_DECLARATION_STRINGCAST(type, convert_func) \
		template<> class casts<type>\
		{\
		public:\
			casts(const GAIA::CH* psz){m_pCH = psz; m_pWCH = GNIL;}\
			casts(const GAIA::WCH* psz){m_pCH = GNIL; m_pWCH = psz;}\
			operator type() const\
			{\
				type ret;\
				if(m_pCH != GNIL)\
					GAIA::ALGO::convert_func(m_pCH, ret);\
				else if(m_pWCH != GNIL)\
					GAIA::ALGO::convert_func(m_pWCH, ret);\
				else\
				{\
					GASTFALSE;\
					ret = 0;\
				}\
				return ret;\
			}\
		private:\
			const GAIA::CH* m_pCH;\
			const GAIA::WCH* m_pWCH;\
		};
		GAIA_DECLARATION_STRINGCAST(GAIA::NM, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::UM, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::N8, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::N16, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::N32, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::N64, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::U8, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::U16, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::U32, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::U64, str2int)
		GAIA_DECLARATION_STRINGCAST(GAIA::BL, str2bool)
		GAIA_DECLARATION_STRINGCAST(GAIA::F32, str2real)
		GAIA_DECLARATION_STRINGCAST(GAIA::F64, str2real)
		GAIA_DECLARATION_STRINGCAST(GAIA::X128, str2x128)
		template<typename _ValueType, typename _CharType, typename _SizeType>
		GAIA::BL castv(const _ValueType& v, _CharType* p, const _SizeType& size);
		template<typename _CharType, typename _SizeType>
		GAIA::BL castv(const GAIA::F32& v, _CharType* p, const _SizeType& size)
		{
			GAIA::ALGO::real2str(v, p);
			GAST(GAIA::ALGO::gstrlen(p) < (GAIA::NUM)size);
			return GAIA::True;
		}
		template<typename _CharType, typename _SizeType>
		GAIA::BL castv(const GAIA::F64& v, _CharType* p, const _SizeType& size)
		{
			GAIA::ALGO::real2str(v, p);
			GAST(GAIA::ALGO::gstrlen(p) < (GAIA::NUM)size);
			return GAIA::True;
		}
		template<typename _CharType, typename _SizeType>
		GAIA::BL castv(GAIA::BL v, _CharType* p, const _SizeType& size)
		{
			GAIA::ALGO::bool2str(v, p);
			GAST(GAIA::ALGO::gstrlen(p) < size);
			return GAIA::True;
		}
		template<typename _CharType, typename _SizeType>
		GAIA::BL castv(const X128& v, _CharType* p, const _SizeType& size)
		{
			GAIA::ALGO::hex2str((const GAIA::U8*)&v, sizeof(v), p);
			GAST(GAIA::ALGO::gstrlen(p) < (GAIA::NUM)size);
			return GAIA::True;
		}
		template<typename _ValueType, typename _CharType, typename _SizeType>
		GAIA::BL castv(const _ValueType& v, _CharType* p, const _SizeType& size)
		{
			GAIA::ALGO::int2str(v, p);
			GAST(GAIA::ALGO::gstrlen(p) < (GAIA::NUM)size);
			return GAIA::True;
		}
		class acasts
		{
		public:
			GINL acasts(const GAIA::CH* psz){m_pCH = psz; m_pWCH = GNIL;}
			GINL acasts(const GAIA::WCH* psz){m_pCH = GNIL; m_pWCH = psz;}
			#define GAIA_DECLARATION_STRINGAUTOCAST(type) \
			GINL operator type()\
			{\
				type ret;\
				if(m_pCH != GNIL)\
					return casts<type>(m_pCH);\
				else if(m_pWCH != GNIL)\
					return casts<type>(m_pWCH);\
				else\
				{\
					GASTFALSE;\
					ret = 0;\
				}\
				return ret;\
			}
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::NM)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::UM)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::N8)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::N16)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::N32)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::N64)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::U8)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::U16)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::U32)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::U64)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::BL)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::F32)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::F64)
			GAIA_DECLARATION_STRINGAUTOCAST(GAIA::X128)
		private:
			const GAIA::CH* m_pCH;
			const GAIA::WCH* m_pWCH;
		};

		template<typename _DataType1, typename _DataType2>
		_DataType1 gstrbycapacity(_DataType1 dst, _DataType2 storagesize)
		{
			GAIA::CH szUnit[32];
			if(storagesize >= 1024 * 1024 * 1024)
			{
				storagesize = storagesize / 1024 / 1024 * 1000 / 1024;
				GAIA::ALGO::gstrcpy(szUnit, "(GB)");
			}
			else if(storagesize >= 1024 * 1024)
			{
				storagesize = storagesize / 1024 * 100 / 1024;
				GAIA::ALGO::gstrcpy(szUnit, "(MB)");
			}
			else if(storagesize >= 1024)
			{
				storagesize = storagesize * 10 / 1024;
				GAIA::ALGO::gstrcpy(szUnit, "(KB)");
			}
			else
				GAIA::ALGO::gstrcpy(szUnit, "(Bytes)");
			GAIA::ALGO::castv(storagesize, dst, 32);
			GAIA::NUM sLen = GAIA::ALGO::gstrlen(dst);
			if(szUnit[1] == 'G')
			{
				insert(dst, sLen, '.', sLen - 3);
				++sLen;
			}
			else if(szUnit[1] == 'M')
			{
				insert(dst, sLen, '.', sLen - 2);
				++sLen;
			}
			else if(szUnit[1] == 'K')
			{
				insert(dst, sLen, '.', sLen - 1);
				++sLen;
			}
			GAIA::ALGO::gstrcpy(dst + sLen, szUnit);
			return dst;
		}
	}
}

#endif
