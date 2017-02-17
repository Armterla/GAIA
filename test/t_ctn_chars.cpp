#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_chars(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::TChars __TCharsType;
		__TCharsType chs;
		{
			chs.resize(10);
			if(chs.size() != 10)
				TERROR;
			chs.resize(20);
			if(chs.size() != 20)
				TERROR;
			chs.clear();
		}
		TAST(chs.empty());
		__TCharsType::it newit;
		__TCharsType::const_it newcit;
		newit = chs.frontit();
		TAST(newit.empty());
		newit = chs.backit();
		TAST(newit.empty());
		newcit = chs.const_frontit();
		TAST(newcit.empty());
		newcit = chs.const_backit();
		TAST(newcit.empty());
		chs.clear();
		chs = _T("Hello World!");
		if(chs.empty())
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello World!")))
			TERROR;
		if(chs.capacity() < chs.size())
			TERROR;
		if(chs != _T("Hello World!"))
			TERROR;
		__TCharsType chs1 = chs;
		if(chs != chs1)
			TERROR;
		chs = (GAIA::NM)-123456789;
		if(chs != _T("-123456789"))
			TERROR;
		if((GAIA::NM)chs != -123456789)
			TERROR;
		chs = (GAIA::UM)123456789;
		if(chs != _T("123456789"))
			TERROR;
		if((GAIA::UM)chs != 123456789)
			TERROR;
		chs = (GAIA::N8)-123;
		if(chs != _T("-123"))
			TERROR;
		if((GAIA::N8)chs != -123)
			TERROR;
		chs = (GAIA::U8)123;
		if(chs != _T("123"))
			TERROR;
		if((GAIA::U8)chs != 123)
			TERROR;
		chs = (GAIA::N16)-12345;
		if(chs != _T("-12345"))
			TERROR;
		if((GAIA::N16)chs != -12345)
			TERROR;
		chs = (GAIA::U16)12345;
		if(chs != _T("12345"))
			TERROR;
		if((GAIA::U16)chs != 12345)
			TERROR;
		chs = (GAIA::N32)-123456789;
		if(chs != _T("-123456789"))
			TERROR;
		if((GAIA::N32)chs != -123456789)
			TERROR;
		chs = (GAIA::U32)123456789;
		if(chs != _T("123456789"))
			TERROR;
		if((GAIA::U32)chs != 123456789)
			TERROR;
		chs = (GAIA::N64)-123456789123456789;
		if(chs != _T("-123456789123456789"))
			TERROR;
		if((GAIA::N64)chs != -123456789123456789)
			TERROR;
		chs = (GAIA::U64)123456789123456789;
		if(chs != _T("123456789123456789"))
			TERROR;
		if((GAIA::U64)chs != 123456789123456789)
			TERROR;
		chs = 1234.567F;
		chs.resize(8);
		if(chs != _T("1234.567"))
			TERROR;
		if((GAIA::F32)chs != 1234.567F)
			TERROR;
		chs = (GAIA::F64)-1234567.12345;
		chs.resize(14);
		if(chs != _T("-1234567.12345"))
			TERROR;
		if((GAIA::F64)chs != -1234567.12345)
			TERROR;
		chs = GSCAST(GAIA::N32)(GSCAST(GAIA::BL)(GAIA::True));
		if(chs != _T("1"))
			TERROR;
		TAST((GAIA::BL)chs);
		chs = GSCAST(GAIA::N32)(GSCAST(GAIA::BL)(GAIA::False));
		if(chs != _T("0"))
			TERROR;
		TAST((GAIA::BL)chs);
		GAIA::X128 x128;
		GAIA::ALGO::gmemset(&x128, 17, sizeof(x128));
		chs = x128;
		if(chs != _T("11111111111111111111111111111111"))
			TERROR;
		if(chs.operator GAIA::X128() != x128)
			TERROR;
		chs = _T("HelloWorld";)
		chs1 = chs;
		TAST(chs1 == chs && chs1 >= chs && chs1 <= chs);
		if(chs1 != chs || chs1 > chs || chs1 < chs)
			TERROR;
		TAST(chs == _T("HelloWorld") && chs >= _T("HelloWorld") && chs <= _T("HelloWorld"));
		if(chs != _T("HelloWorld") || chs > _T("HelloWorld") || chs < _T("HelloWorld"))
			TERROR;
		chs.resize(5);
		if(chs != _T("Hello"))
			TERROR;
		chs.clear();
		chs = _T("HelloWorld");
		chs.insert('!', chs.size());
		chs.insert('%', chs.size() / 2);
		chs.insert('?', 0);
		if(chs != _T("?Hello%World!"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("?Hello%World!")))
			TERROR;
		if(chs.size() > chs.capacity())
			TERROR;
		chs = _T("HelloWorld");
		chs.insert(_T("Yes"), chs.size());
		chs.insert(_T("No"), chs.size() / 2);
		chs.insert(_T("Cancel"), 0);
		if(chs != _T("CancelHelloWNoorldYes"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("CancelHelloWNoorldYes")))
			TERROR;
		if(chs.size() > chs.capacity())
			TERROR;
		chs = _T("HelloWorld");
		chs.insert((__TCharsType)_T("Yes"), chs.size());
		chs.insert((__TCharsType)_T("No"), chs.size() / 2);
		chs.insert((__TCharsType)_T("Cancel"), 0);
		if(chs != _T("CancelHelloWNoorldYes"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("CancelHelloWNoorldYes")))
			TERROR;
		if(chs.size() > chs.capacity())
			TERROR;
		chs = _T("HelloWorld");
		if(chs.isallupper())
			TERROR;
		if(chs.isalllower())
			TERROR;
		TAST(chs.isexistupper());
		TAST(chs.isexistlower());
		chs.tolower();
		TAST(chs.isalllower());
		chs.toupper();
		TAST(chs.isallupper());
		chs = _T("HelloWorld");
		chs1 = chs + _T("HelloWorld");
		if(chs1 != _T("HelloWorldHelloWorld"))
			TERROR;
		if(chs1.size() != GAIA::ALGO::gstrlen(_T("HelloWorldHelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs += _T("HelloWorld");
		if(chs != _T("HelloWorldHelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorldHelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs1.clear();
		chs = chs + chs1;
		if(chs != _T("HelloWorld"))
			TERROR;
		chs += chs1;
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		if(chs[0] != 'H' ||
			chs[1] != 'e' ||
			chs[2] != 'l' ||
			chs[3] != 'l' ||
			chs[4] != 'o' ||
			chs[5] != 'W' ||
			chs[6] != 'o' ||
			chs[7] != 'r' ||
			chs[8] != 'l' ||
			chs[9] != 'd')
		{
			TERROR;
		}
		chs = _T("HelloWorld");
		const GAIA::TCH* cpsz = chs;
		if(GAIA::ALGO::gstrcmp(cpsz, _T("HelloWorld")) != 0)
			TERROR;
		GAIA::TCH* psz = chs;
		if(GAIA::ALGO::gstrcmp(psz, _T("HelloWorld")) != 0)
			TERROR;
		chs = _T("HelloWorld");
		chs = _T("HelloWorld") + chs;
		if(chs != _T("HelloWorldHelloWorld"))
			TERROR;
		chs = _T("HelloWorld");
		if(*chs.fptr() != 'H')
			TERROR;
		if(*chs.bptr() != 'd')
			TERROR;
		__TCharsType::it itfront = chs.frontit();
		__TCharsType::it itback = chs.backit();
		__TCharsType::const_it citfront = chs.const_frontit();
		__TCharsType::const_it citback = chs.const_backit();
		if(itfront.empty() || itback.empty() || citfront.empty() || citback.empty())
			TERROR;
		if(*itfront != 'H')
			TERROR;
		if(*itback != 'd')
			TERROR;
		if(*citfront != 'H')
			TERROR;
		if(*citback != 'd')
			TERROR;
		GAIA::TCH szTemp[128];
		for(; !citfront.empty(); ++citfront)
		{
			szTemp[citfront - chs.const_frontit()] = *citfront;
			szTemp[citfront - chs.const_frontit() + 1] = 0;
		}
		if(chs != szTemp)
			TERROR;
		for(; !itfront.empty(); ++itfront)
		{
			szTemp[itfront - chs.frontit()] = *itfront;
			szTemp[itfront - chs.frontit() + 1] = 0;
		}
		if(chs != szTemp)
			TERROR;
		chs = _T("Hello World! I am gaia, i am all object's base!");
		citfront = chs.const_frontit();
		itfront = chs.frontit();
		if((citfront + 10) - citfront != 10)
			TERROR;
		if((itfront + 10) - itfront != 10)
			TERROR;
		citfront += 10;
		itfront += 10;
		if(citfront - chs.const_frontit() != 10)
			TERROR;
		if(itfront - chs.frontit() != 10)
			TERROR;
		citfront -= 20;
		itfront -= 20;
		TAST(citfront.empty());
		TAST(itfront.empty());
		chs = _T("Hello World! I am gaia, i am all object's base!");
		chs.erase_index(0);
		chs.erase_index(chs.size() - 1);
		chs.erase_index(3);
		if(chs != _T("ell World! I am gaia, i am all object's base"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("ell World! I am gaia, i am all object's base")))
			TERROR;
		chs = _T("Hello World! I am gaia, i am all object's base!");
		chs.erase_index(6, 10);
		if(chs != _T("Hello ! I am gaia, i am all object's base!"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello ! I am gaia, i am all object's base!")))
			TERROR;
		chs = _T("Hello World! I am gaia, i am all object's base!");
		if(chs.erase_all('a', 0) != 6)
			TERROR;
		if(chs != _T("Hello World! I m gi, i m ll object's bse!"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello World! I m gi, i m ll object's bse!")))
			TERROR;
		chs = _T("Hello World! I am gaia, i am all object's base!");
		if(chs.erase_all(_T("am "), 1) != 2)
			TERROR;
		if(chs != _T("Hello World! I gaia, i all object's base!"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello World! I gaia, i all object's base!")))
			TERROR;
		chs = _T("Hello World! I am gaia, i am all object's base!");
		if(chs.erase_all((__TCharsType)_T("am "), 1) != 2)
			TERROR;
		if(chs != _T("Hello World! I gaia, i all object's base!"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello World! I gaia, i all object's base!")))
			TERROR;
		chs = _T("Hello World! I am gaia, i am all object's base!");
		if(chs.erase_all(_T("Hello World! "), 0) != 1)
			TERROR;
		if(chs != _T("I am gaia, i am all object's base!"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("I am gaia, i am all object's base!")))
			TERROR;
		chs = _T("Hello World! I am gaia, i am all object's base!");
		if(chs.erase_all(_T("base!"), 0) != 1)
			TERROR;
		if(chs != _T("Hello World! I am gaia, i am all object's "))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello World! I am gaia, i am all object's ")))
			TERROR;
		chs = _T("Hello World! I am gaia, i am all object's base!");
		if(chs.erase_all(_T("Hello World! I am gaia, i am all object's base!"), 0) != 1)
			TERROR;
		TAST(chs.empty());
		chs = _T("HelloWorld");
		chs.left(5);
		if(chs != _T("Hello"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello")))
			TERROR;
		chs = _T("HelloWorld");
		chs.right(4);
		if(chs != _T("World"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("World")))
			TERROR;
		chs = _T("HelloWorld");
		chs.mid(5, 6);
		if(chs != _T("Wo"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Wo")))
			TERROR;
		chs = _T("HelloWorld");
		chs.left(0);
		TAST(chs.empty());
		chs = _T("HelloWorld");
		chs.right(chs.size() - 1);
		TAST(chs.empty());
		chs = _T("HelloWorld");
		chs.mid(0, 0);
		if(chs != _T("H"))
			TERROR;
		chs = _T("HelloWorld");
		chs.mid(chs.size() - 1, chs.size() - 1);
		if(chs != _T("d"))
			TERROR;
		chs = _T("HelloWorld");
		chs.mid(chs.size() / 2, chs.size() / 2);
		if(chs != _T("W"))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_left_index(5);
		if(chs != _T("World"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("World")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_left(_T("H"));
		if(chs != _T("elloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("elloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_left(_T("a"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_left(_T("He"));
		if(chs != _T("lloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("lloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_left(_T("Ab"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_left((__TCharsType)_T("Hello"));
		if(chs != _T("World"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("World")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_left((__TCharsType)_T("Ab"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_lefti(_T("h"));
		if(chs != _T("elloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("elloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_lefti(_T("a"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_lefti(_T("he"));
		if(chs != _T("lloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("lloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_lefti(_T("ab"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_lefti((__TCharsType)_T("hello"));
		if(chs != _T("World"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("World")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_lefti((__TCharsType)_T("ab"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_right_index(4);
		if(chs != _T("Hello"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_right(_T("d"));
		if(chs != _T("HelloWorl"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorl")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_right(_T("a"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_right(_T("ld"));
		if(chs != _T("HelloWor"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWor")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_right(_T("Ab"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_right((__TCharsType)_T("World"));
		if(chs != _T("Hello"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_right((__TCharsType)_T("Ab"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_righti(_T("D"));
		if(chs != _T("HelloWorl"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorl")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_righti(_T("a"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_righti(_T("LD"));
		if(chs != _T("HelloWor"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWor")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_righti(_T("AB"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_righti((__TCharsType)_T("WORLD"));
		if(chs != _T("Hello"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("Hello")))
			TERROR;
		chs = _T("HelloWorld");
		chs.trim_righti((__TCharsType)_T("AB"));
		if(chs != _T("HelloWorld"))
			TERROR;
		if(chs.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		chs = _T("HelloWorldHelloWorld");
		if(chs.rfind('l', chs.size() - 1) != 18)
			TERROR;
		if(chs.rfind('d', chs.size() - 1) != 19)
			TERROR;
		if(chs.rfind('H', chs.size() - 1) != 10)
			TERROR;
		if(chs.rfind('x', chs.size() - 1) != GINVALID)
			TERROR;
		if(chs.rfind(_T("Hello"), chs.size() - 1) != 10)
			TERROR;
		if(chs.rfind(_T("ld"), chs.size() - 1) != 18)
			TERROR;
		if(chs.rfind(_T("HelloWorldH"), chs.size() - 1) != 0)
			TERROR;
		if(chs.rfind(_T("rr"), chs.size() - 1) != GINVALID)
			TERROR;
		if(chs.rfind((__TCharsType)_T("r"), chs.size() - 1) != 17)
			TERROR;
		if(chs.rfind((__TCharsType)_T("rl"), chs.size() - 1) != 17)
			TERROR;
		if(chs.rfind((__TCharsType)_T("ld"), chs.size() - 1) != 18)
			TERROR;
		if(chs.rfind((__TCharsType)_T("HelloWorldH"), chs.size() - 1) != 0)
			TERROR;
		if(chs.rfind((__TCharsType)_T("rr"), chs.size() - 1) != GINVALID)
			TERROR;
		chs = _T("HelloWorld");
		if(!chs.contains("Hello"))
			TERROR;
		if(!chs.contains("World"))
			TERROR;
		if(!chs.contains("oW"))
			TERROR;
		if(!chs.contains('W'))
			TERROR;
		if(chs.contains('A'))
			TERROR;
		if(!chs.contains(chs))
			TERROR;
		chs = _T("HelloWorld");
		if(chs.compare("HelloWorld") != 0)
			TERROR;
		if(chs.comparei("helloworld") != 0)
			TERROR;
		if(chs.compare("HelloKitty") == 0)
			TERROR;
		if(chs.comparei("HelloKitty") == 0)
			TERROR;
		if(!chs.equals("HelloWorld"))
			TERROR;
		if(!chs.equalsi("helloworld"))
			TERROR;
		if(chs.equals("HelloKitty"))
			TERROR;
		if(chs.equalsi("hellokitty"))
			TERROR;
		chs = _T("HelloWorld");
		if(!chs.startwith('H'))
			TERROR;
		if(!chs.startwith("Hello"))
			TERROR;
		if(!chs.startwith((__TCharsType)"Hello"))
			TERROR;
		if(!chs.endwith('d'))
			TERROR;
		if(!chs.endwith("World"))
			TERROR;
		if(!chs.endwith((__TCharsType)"World"))
			TERROR;
		if(!chs.startwithi('h'))
			TERROR;
		if(!chs.startwithi("hello"))
			TERROR;
		if(!chs.startwithi((__TCharsType)"hello"))
			TERROR;
		if(!chs.endwithi('D'))
			TERROR;
		if(!chs.endwithi("world"))
			TERROR;
		if(!chs.endwithi((__TCharsType)"world"))
			TERROR;
		if(chs.startwith('a'))
			TERROR;
		if(chs.startwith("abc"))
			TERROR;
		if(chs.startwith((__TCharsType)"abc"))
			TERROR;
		if(chs.endwith('a'))
			TERROR;
		if(chs.endwith("abc"))
			TERROR;
		if(chs.endwith((__TCharsType)"abc"))
			TERROR;
		if(chs.startwithi('a'))
			TERROR;
		if(chs.startwithi("abc"))
			TERROR;
		if(chs.startwithi((__TCharsType)"abc"))
			TERROR;
		if(chs.endwithi('a'))
			TERROR;
		if(chs.endwithi("abc"))
			TERROR;
		if(chs.endwithi((__TCharsType)"abc"))
			TERROR;
		chs = _T("HelloWorld");
		chs.replace('l', 'x');
		if(chs != _T("HexxoWorxd"))
			TERROR;
		chs = _T("HelloWorldHelloWorld");
		chs.replace(_T("Hello"), _T("abc"));
		if(chs != _T("abcWorldabcWorld"))
			TERROR;
		chs = _T("HelloWorldHelloWorld");
		chs.replace(_T("World"), _T("abc"));
		if(chs != _T("HelloabcHelloabc"))
			TERROR;
		chs = _T("HelloWorldHelloWorld");
		chs.replace(_T("oW"), _T("ab"));
		if(chs != _T("HellaborldHellaborld"))
			TERROR;
		chs = _T("HelloWorldHelloWorld");
		chs.replace((__TCharsType)_T("Hello"), (__TCharsType)_T("abc"));
		if(chs != _T("abcWorldabcWorld"))
			TERROR;
		chs = _T("HelloWorldHelloWorld");
		chs.replace((__TCharsType)_T("World"), (__TCharsType)_T("abc"));
		if(chs != _T("HelloabcHelloabc"))
			TERROR;
		chs = _T("HelloWorldHelloWorld");
		chs.replace((__TCharsType)_T("oW"), (__TCharsType)_T("ab"));
		if(chs != _T("HellaborldHellaborld"))
			TERROR;
		chs = _T("HelloWorldHelloWorld");
		chs.replace(_T("Hello"), _T(""));
		if(chs != _T("WorldWorld"))
			TERROR;
		chs = _T("Hello");
		chs = 10 + chs;
		if(chs != _T("10Hello"))
			TERROR;
		chs = _T("1234567890ABCDEF1234567890ABCDEF");
		if(chs.type() != (GAIA::STRING_TYPE_RID | GAIA::STRING_TYPE_RID128))
			TERROR;
		chs = _T("Hello world 123.4F");
		if(chs.type() != GAIA::STRING_TYPE_STRING)
			TERROR;
		chs = _T("-123.4F");
		TAST(chs.type() & GAIA::STRING_TYPE_REAL);
		chs = _T("+123123123123");
		TAST(chs.type() & GAIA::STRING_TYPE_INTEGER);

		chs = "Hello World!";
		if(chs.extract(3, 4, chs1) != 2)
			TERROR;
		if(chs1 != "lo")
			TERROR;
		if(chs.extract(3, 4, szTemp, sizeofarray(szTemp)) != 2)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szTemp, "lo") != 0)
			TERROR;

		x128.fromstring("12345678123456781234567812345678");
		chs.clear();
		chs += (GAIA::N8) - 1;
		chs += (GAIA::U8)1;
		chs += (GAIA::N16) - 1;
		chs += (GAIA::U16)1;
		chs += (GAIA::N32) - 1;
		chs += (GAIA::U32)1;
		chs += (GAIA::N64) - 1;
		chs += (GAIA::U64)1;
		chs += (GAIA::NM) - 1;
		chs += (GAIA::UM)1;
		chs += (GAIA::F32) - 1;
		chs += (GAIA::F64) - 1;
		chs += (GAIA::WCH)1;
		chs += (GAIA::BL)GAIA::True;
		chs += (GAIA::BL)GAIA::False;
		chs += x128;
		if(chs != "-11-11-11-11-11-1.0-1.011012345678123456781234567812345678")
			TERROR;

		chs.clear();
		chs << "Hello" << "World" << 1 << GAIA::True;
		if(chs != "HelloWorld11")
			TERROR;

		chs.clear();
		chs = chs + (GAIA::N8) - 1;
		chs = chs + (GAIA::U8)1;
		chs = chs + (GAIA::N16) - 1;
		chs = chs + (GAIA::U16)1;
		chs = chs + (GAIA::N32) - 1;
		chs = chs + (GAIA::U32)1;
		chs = chs + (GAIA::N64) - 1;
		chs = chs + (GAIA::U64)1;
		chs = chs + (GAIA::NM) - 1;
		chs = chs + (GAIA::UM)1;
		chs = chs + (GAIA::F32) - 1;
		chs = chs + (GAIA::F64) - 1;
		chs = chs + (GAIA::WCH)1;
		chs = chs + (GAIA::BL)GAIA::True;
		chs = chs + (GAIA::BL)GAIA::False;
		chs = chs + x128;
		if(chs != "-11-11-11-11-11-1.0-1.011012345678123456781234567812345678")
			TERROR;

		chs = (const GAIA::TCH*)GNIL;
		if((const GAIA::TCH*)chs == GNIL)
			TERROR;
		if(chs.fptr() == GNIL)
			TERROR;
		if(chs != (const GAIA::TCH*)GNIL)
			TERROR;

		chs = _T("Hello World, 测试utf8字符串！");
		GAIA::CTN::AChars chsUtf8 = chs.toUtf8();
		GAIA::CTN::WChars chsWChars = chs.toWString(GAIA::CHARSET_TYPE_UTF8);
		if(chsWChars != chs)
			TERROR;

		GAIA::CTN::WChars chsLocale(_T("Hello World, 测试utf8字符串！"));
		GAIA::CTN::AChars chsLocale1(chsLocale.fptr(), "UTF-8");
		GAIA::CTN::WChars chsLocale2(chsLocale1.fptr(), "UTF-8");
		if(chsLocale2 != chsLocale)
			TERROR;
	}
}
