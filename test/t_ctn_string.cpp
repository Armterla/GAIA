#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_string(GAIA::LOG::Log& logobj)
	{
		typedef GAIA::CTN::TString __StrType;
		__StrType str;
		{
			str.resize(10);
			if(str.size() != 10)
				TERROR;
			if(str.capacity() != 10)
				TERROR;
			str.reserve(20);
			if(str.size() != 0)
				TERROR;
			if(str.capacity() != 20)
				TERROR;
			str.resize(20);
			if(str.size() != 20)
				TERROR;
			if(str.capacity() != 20)
				TERROR;
			str.clear();
		}
		__StrType::it newit;
		__StrType::const_it newcit;
		newit = str.frontit();
		TAST(newit.empty());
		newit = str.backit();
		TAST(newit.empty());
		newcit = str.const_frontit();
		TAST(newcit.empty());
		newcit = str.const_backit();
		TAST(newcit.empty());
		TAST(str.empty());
		str.destroy(); // Try to destroy new string.
		str = _T("Hello World!");
		if(str.empty())
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello World!")))
			TERROR;
		if(str.capacity() < str.size())
			TERROR;
		if(str != _T("Hello World!"))
			TERROR;
		__StrType str1 = str;
		if(str != str1)
			TERROR;
		str = (GAIA::NM)-123456789;
		if(str != _T("-123456789"))
			TERROR;
		if((GAIA::NM)str != -123456789)
			TERROR;
		str = (GAIA::UM)123456789;
		if(str != _T("123456789"))
			TERROR;
		if((GAIA::UM)str != 123456789)
			TERROR;
		str = (GAIA::N8)-123;
		if(str != _T("-123"))
			TERROR;
		if((GAIA::N8)str != -123)
			TERROR;
		str = (GAIA::U8)123;
		if(str != _T("123"))
			TERROR;
		if((GAIA::U8)str != 123)
			TERROR;
		str = (GAIA::N16)-12345;
		if(str != _T("-12345"))
			TERROR;
		if((GAIA::N16)str != -12345)
			TERROR;
		str = (GAIA::U16)12345;
		if(str != _T("12345"))
			TERROR;
		if((GAIA::U16)str != 12345)
			TERROR;
		str = (GAIA::N32)-123456789;
		if(str != _T("-123456789"))
			TERROR;
		if((GAIA::N32)str != -123456789)
			TERROR;
		str = (GAIA::U32)123456789;
		if(str != _T("123456789"))
			TERROR;
		if((GAIA::U32)str != 123456789)
			TERROR;
		str = (GAIA::N64)-123456789123456789;
		if(str != _T("-123456789123456789"))
			TERROR;
		if((GAIA::N64)str != -123456789123456789)
			TERROR;
		str = (GAIA::U64)123456789123456789;
		if(str != _T("123456789123456789"))
			TERROR;
		if((GAIA::U64)str != 123456789123456789)
			TERROR;
		str = (GAIA::F32)1234.567F;
		str.resize(8);
		if(str != _T("1234.567"))
			TERROR;
		if((GAIA::F32)str != 1234.567F)
			TERROR;
		str = (GAIA::F64)-1234567.12345;
		str.resize(14);
		if(str != _T("-1234567.12345"))
			TERROR;
		if((GAIA::F64)str != -1234567.12345)
			TERROR;
		str = GSCAST(GAIA::N32)(GSCAST(GAIA::BL)(GAIA::True));
		if(str != _T("1"))
			TERROR;
		TAST((GAIA::BL)str);
		str = GSCAST(GAIA::N32)(GSCAST(GAIA::BL)(GAIA::False));
		if(str != _T("0"))
			TERROR;
		TAST((GAIA::BL)str);
		GAIA::X128 x128;
		GAIA::ALGO::gmemset(&x128, 17, sizeof(x128));
		str = x128;
		if(str != _T("11111111111111111111111111111111"))
			TERROR;
		if(str.operator GAIA::X128() != x128)
			TERROR;
		str = _T("HelloWorld");
		str1 = str;
		TAST(str1 == str && str1 >= str && str1 <= str);
		if(str1 != str || str1 > str || str1 < str)
			TERROR;
		TAST(str == _T("HelloWorld") && str >= _T("HelloWorld") && str <= _T("HelloWorld"));
		if(str != _T("HelloWorld") || str > _T("HelloWorld") || str < _T("HelloWorld"))
			TERROR;
		str.resize(5);
		if(str != _T("Hello"))
			TERROR;
		str.reserve(1000);
		TAST(str.empty());
		str.destroy();
		str = _T("HelloWorld");
		str.insert('!', str.size());
		str.insert('%', str.size() / 2);
		str.insert('?', 0);
		if(str != _T("?Hello%World!"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("?Hello%World!")))
			TERROR;
		if(str.size() > str.capacity())
			TERROR;
		str = _T("HelloWorld";)
		str.insert(_T("Yes"), str.size());
		str.insert(_T("No"), str.size() / 2);
		str.insert(_T("Cancel"), 0);
		if(str != _T("CancelHelloWNoorldYes"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("CancelHelloWNoorldYes")))
			TERROR;
		if(str.size() > str.capacity())
			TERROR;
		str = _T("HelloWorld");
		str.insert((__StrType)_T("Yes"), str.size());
		str.insert((__StrType)_T("No"), str.size() / 2);
		str.insert((__StrType)_T("Cancel"), 0);
		if(str != _T("CancelHelloWNoorldYes"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("CancelHelloWNoorldYes")))
			TERROR;
		if(str.size() > str.capacity())
			TERROR;
		str = _T("HelloWorld");
		if(str.isallupper())
			TERROR;
		if(str.isalllower())
			TERROR;
		TAST(str.isexistupper());
		TAST(str.isexistlower());
		str.tolower();
		TAST(str.isalllower());
		str.toupper();
		TAST(str.isallupper());
		str = _T("HelloWorld");
		str1 = str + _T("HelloWorld");
		if(str1 != _T("HelloWorldHelloWorld"))
			TERROR;
		if(str1.size() != GAIA::ALGO::gstrlen(_T("HelloWorldHelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str += _T("HelloWorld");
		if(str != _T("HelloWorldHelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorldHelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str1.clear();
		str = str + str1;
		if(str != _T("HelloWorld"))
			TERROR;
		str += str1;
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		if(str[0] != 'H' ||
			str[1] != 'e' ||
			str[2] != 'l' ||
			str[3] != 'l' ||
			str[4] != 'o' ||
			str[5] != 'W' ||
			str[6] != 'o' ||
			str[7] != 'r' ||
			str[8] != 'l' ||
			str[9] != 'd')
		{
			TERROR;
		}
		str = _T("HelloWorld");
		const GAIA::TCH* cpsz = str;
		if(GAIA::ALGO::gstrcmp(cpsz, _T("HelloWorld")) != 0)
			TERROR;
		GAIA::TCH* psz = str;
		if(GAIA::ALGO::gstrcmp(psz, _T("HelloWorld")) != 0)
			TERROR;
		str = _T("HelloWorld");
		str = _T("HelloWorld") + str;
		if(str != _T("HelloWorldHelloWorld"))
			TERROR;
		str = _T("HelloWorld");
		if(*str.fptr() != 'H')
			TERROR;
		if(*str.bptr() != 'd')
			TERROR;
		__StrType::it itfront = str.frontit();
		__StrType::it itback = str.backit();
		__StrType::const_it citfront = str.const_frontit();
		__StrType::const_it citback = str.const_backit();
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
			szTemp[citfront - str.const_frontit()] = *citfront;
			szTemp[citfront - str.const_frontit() + 1] = 0;
		}
		if(str != szTemp)
			TERROR;
		for(; !itfront.empty(); ++itfront)
		{
			szTemp[itfront - str.frontit()] = *itfront;
			szTemp[itfront - str.frontit() + 1] = 0;
		}
		if(str != szTemp)
			TERROR;
		str = _T("Hello World! I am gaia, i am all object's base!");
		citfront = str.const_frontit();
		itfront = str.frontit();
		if((citfront + 10) - citfront != 10)
			TERROR;
		if((itfront + 10) - itfront != 10)
			TERROR;
		citfront += 10;
		itfront += 10;
		if(citfront - str.const_frontit() != 10)
			TERROR;
		if(itfront - str.frontit() != 10)
			TERROR;
		citfront -= 20;
		itfront -= 20;
		TAST(citfront.empty());
		TAST(itfront.empty());
		str = _T("Hello World! I am gaia, i am all object's base!");
		str.erase_index(0);
		str.erase_index(str.size() - 1);
		str.erase_index(3);
		if(str != _T("ell World! I am gaia, i am all object's base"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("ell World! I am gaia, i am all object's base")))
			TERROR;
		str = _T("Hello World! I am gaia, i am all object's base!");
		str.erase_index(6, 10);
		if(str != _T("Hello ! I am gaia, i am all object's base!"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello ! I am gaia, i am all object's base!")))
			TERROR;
		str = _T("Hello World! I am gaia, i am all object's base!");
		if(str.erase_all('a', 0) != 6)
			TERROR;
		if(str != _T("Hello World! I m gi, i m ll object's bse!"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello World! I m gi, i m ll object's bse!")))
			TERROR;
		str = _T("Hello World! I am gaia, i am all object's base!");
		if(str.erase_all(_T("am "), 1) != 2)
			TERROR;
		if(str != _T("Hello World! I gaia, i all object's base!"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello World! I gaia, i all object's base!")))
			TERROR;
		str = _T("Hello World! I am gaia, i am all object's base!");
		if(str.erase_all((__StrType)_T("am "), 1) != 2)
			TERROR;
		if(str != _T("Hello World! I gaia, i all object's base!"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello World! I gaia, i all object's base!")))
			TERROR;
		str = _T("Hello World! I am gaia, i am all object's base!");
		if(str.erase_all(_T("Hello World! "), 0) != 1)
			TERROR;
		if(str != _T("I am gaia, i am all object's base!"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("I am gaia, i am all object's base!")))
			TERROR;
		str = _T("Hello World! I am gaia, i am all object's base!");
		if(str.erase_all(_T("base!"), 0) != 1)
			TERROR;
		if(str != _T("Hello World! I am gaia, i am all object's "))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello World! I am gaia, i am all object's ")))
			TERROR;
		str = _T("Hello World! I am gaia, i am all object's base!");
		if(str.erase_all(_T("Hello World! I am gaia, i am all object's base!"), 0) != 1)
			TERROR;
		TAST(str.empty());
		str = _T("HelloWorld");
		str.left(5);
		if(str != _T("Hello"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello")))
			TERROR;
		str = _T("HelloWorld");
		str.right(4);
		if(str != _T("World"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("World")))
			TERROR;
		str = _T("HelloWorld");
		str.mid(5, 6);
		if(str != _T("Wo"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Wo")))
			TERROR;
		str = _T("HelloWorld");
		str.left(0);
		TAST(str.empty());
		str = _T("HelloWorld");
		str.right(str.size() - 1);
		TAST(str.empty());
		str = _T("HelloWorld");
		str.mid(0, 0);
		if(str != _T("H"))
			TERROR;
		str = _T("HelloWorld");
		str.mid(str.size() - 1, str.size() - 1);
		if(str != _T("d"))
			TERROR;
		str = _T("HelloWorld");
		str.mid(str.size() / 2, str.size() / 2);
		if(str != _T("W"))
			TERROR;
		str = _T("HelloWorld");
		str.trim_left_index(5);
		if(str != _T("World"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("World")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_left(_T("H"));
		if(str != _T("elloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("elloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_left(_T("a"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_left(_T("He"));
		if(str != _T("lloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("lloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_left(_T("Ab"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_left((__StrType)_T("Hello"));
		if(str != _T("World"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("World")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_left((__StrType)_T("Ab"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_lefti(_T("h"));
		if(str != _T("elloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("elloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_lefti(_T("a"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_lefti(_T("he"));
		if(str != _T("lloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("lloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_lefti(_T("ab"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_lefti((__StrType)_T("hello"));
		if(str != _T("World"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("World")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_lefti((__StrType)_T("ab"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_right_index(4);
		if(str != _T("Hello"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_right(_T("d"));
		if(str != _T("HelloWorl"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorl")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_right(_T("a"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_right(_T("ld"));
		if(str != _T("HelloWor"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWor")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_right(_T("Ab"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_right((__StrType)_T("World"));
		if(str != _T("Hello"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_right((__StrType)_T("Ab"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_righti(_T("D"));
		if(str != _T("HelloWorl"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorl")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_righti(_T("a"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_righti(_T("LD"));
		if(str != _T("HelloWor"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWor")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_righti(_T("AB"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_righti((__StrType)_T("WORLD"));
		if(str != _T("Hello"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("Hello")))
			TERROR;
		str = _T("HelloWorld");
		str.trim_righti((__StrType)_T("AB"));
		if(str != _T("HelloWorld"))
			TERROR;
		if(str.size() != GAIA::ALGO::gstrlen(_T("HelloWorld")))
			TERROR;
		str = _T("HelloWorldHelloWorld");
		if(str.rfind('l', str.size() - 1) != 18)
			TERROR;
		if(str.rfind('d', str.size() - 1) != 19)
			TERROR;
		if(str.rfind('H', str.size() - 1) != 10)
			TERROR;
		if(str.rfind('x', str.size() - 1) != GINVALID)
			TERROR;
		if(str.rfind(_T("Hello"), str.size() - 1) != 10)
			TERROR;
		if(str.rfind(_T("ld"), str.size() - 1) != 18)
			TERROR;
		if(str.rfind(_T("HelloWorldH"), str.size() - 1) != 0)
			TERROR;
		if(str.rfind(_T("rr"), str.size() - 1) != GINVALID)
			TERROR;
		if(str.rfind((__StrType)_T("r"), str.size() - 1) != 17)
			TERROR;
		if(str.rfind((__StrType)_T("rl"), str.size() - 1) != 17)
			TERROR;
		if(str.rfind((__StrType)_T("ld"), str.size() - 1) != 18)
			TERROR;
		if(str.rfind((__StrType)_T("HelloWorldH"), str.size() - 1) != 0)
			TERROR;
		if(str.rfind((__StrType)_T("rr"), str.size() - 1) != GINVALID)
			TERROR;
		str = _T("HelloWorld");
		if(!str.contains("Hello"))
			TERROR;
		if(!str.contains("World"))
			TERROR;
		if(!str.contains("oW"))
			TERROR;
		if(!str.contains('W'))
			TERROR;
		if(str.contains('A'))
			TERROR;
		if(!str.contains(str))
			TERROR;
		str = _T("HelloWorld");
		if(str.compare("HelloWorld") != 0)
			TERROR;
		if(str.comparei("helloworld") != 0)
			TERROR;
		if(str.compare("HelloKitty") == 0)
			TERROR;
		if(str.comparei("HelloKitty") == 0)
			TERROR;
		if(!str.equals("HelloWorld"))
			TERROR;
		if(!str.equalsi("helloworld"))
			TERROR;
		if(str.equals("HelloKitty"))
			TERROR;
		if(str.equalsi("hellokitty"))
			TERROR;
		str = _T("HelloWorld");
		if(!str.startwith('H'))
			TERROR;
		if(!str.startwith("Hello"))
			TERROR;
		if(!str.startwith((__StrType)"Hello"))
			TERROR;
		if(!str.endwith('d'))
			TERROR;
		if(!str.endwith("World"))
			TERROR;
		if(!str.endwith((__StrType)"World"))
			TERROR;
		if(!str.startwithi('h'))
			TERROR;
		if(!str.startwithi("hello"))
			TERROR;
		if(!str.startwithi((__StrType)"hello"))
			TERROR;
		if(!str.endwithi('D'))
			TERROR;
		if(!str.endwithi("world"))
			TERROR;
		if(!str.endwithi((__StrType)"world"))
			TERROR;
		if(str.startwith('a'))
			TERROR;
		if(str.startwith("abc"))
			TERROR;
		if(str.startwith((__StrType)"abc"))
			TERROR;
		if(str.endwith('a'))
			TERROR;
		if(str.endwith("abc"))
			TERROR;
		if(str.endwith((__StrType)"abc"))
			TERROR;
		if(str.startwithi('a'))
			TERROR;
		if(str.startwithi("abc"))
			TERROR;
		if(str.startwithi((__StrType)"abc"))
			TERROR;
		if(str.endwithi('a'))
			TERROR;
		if(str.endwithi("abc"))
			TERROR;
		if(str.endwithi((__StrType)"abc"))
			TERROR;
		str = _T("HelloWorld");
		str.replace('l', 'x');
		if(str != _T("HexxoWorxd"))
			TERROR;
		str = _T("HelloWorldHelloWorld");
		str.replace(_T("Hello"), _T("abc"));
		if(str != _T("abcWorldabcWorld"))
			TERROR;
		str = _T("HelloWorldHelloWorld");
		str.replace(_T("World"), _T("abc"));
		if(str != _T("HelloabcHelloabc"))
			TERROR;
		str = _T("HelloWorldHelloWorld");
		str.replace(_T("oW"), _T("ab"));
		if(str != _T("HellaborldHellaborld"))
			TERROR;
		str = _T("HelloWorldHelloWorld");
		str.replace((__StrType)_T("Hello"), (__StrType)_T("abc"));
		if(str != _T("abcWorldabcWorld"))
			TERROR;
		str = _T("HelloWorldHelloWorld");
		str.replace((__StrType)_T("World"), (__StrType)_T("abc"));
		if(str != _T("HelloabcHelloabc"))
			TERROR;
		str = _T("HelloWorldHelloWorld");
		str.replace((__StrType)_T("oW"), (__StrType)_T("ab"));
		if(str != _T("HellaborldHellaborld"))
			TERROR;
		str = _T("HelloWorldHelloWorld");
		str.replace(_T("Hello"), _T(""));
		if(str != _T("WorldWorld"))
			TERROR;
		str = _T("Hello");
		str = 10 + str;
		if(str != _T("10Hello"))
			TERROR;
		str = _T("1234567890ABCDEF1234567890ABCDEF");
		if(str.type() != (GAIA::STRING_TYPE_RID | GAIA::STRING_TYPE_RID128))
			TERROR;
		str = _T("Hello world 123.4F");
		if(str.type() != GAIA::STRING_TYPE_STRING)
			TERROR;
		str = _T("-123.4F");
		TAST(str.type() & GAIA::STRING_TYPE_REAL);
		str = _T("+123123123123");
		TAST(str.type() & GAIA::STRING_TYPE_INTEGER);

		str = "Hello World!";
		str1.clear();
		if(str.extract(3, 4, str1) != 2)
			TERROR;
		if(str1 != "lo")
			TERROR;
		str1 = str.extract(3, 4);
		if(str1 != "lo")
			TERROR;
		if(str.extract(3, 4, szTemp, sizeofarray(szTemp)) != 2)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szTemp, "lo") != 0)
			TERROR;
		str1.clear();
		if(str.extract_left(5, str1) != 5)
			TERROR;
		if(str1 != "Hello")
			TERROR;
		str1.clear();
		if(str.extract_right(5, str1) != 6)
			TERROR;
		if(str1 != "World!")
			TERROR;
		str1.clear();
		if(str.extract_mid(4, 6, str1) != 3)
			TERROR;
		if(str1 != "o W")
			TERROR;
		str1 = str.extract_left(5);
		if(str1 != "Hello")
			TERROR;
		str1 = str.extract_right(5);
		if(str1 != "World!")
			TERROR;
		str1 = str.extract_mid(4, 6);
		if(str1 != "o W")
			TERROR;
		if(str.extract_left(5, szTemp, sizeofarray(szTemp)) != 5)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szTemp, "Hello") != 0)
			TERROR;
		if(str.extract_right(5, szTemp, sizeofarray(szTemp)) != 6)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szTemp, "World!") != 0)
			TERROR;
		if(str.extract_mid(4, 6, szTemp, sizeofarray(szTemp)) != 3)
			TERROR;
		if(GAIA::ALGO::gstrcmp(szTemp, "o W") != 0)
			TERROR;
		str.destroy();
		str.resize_keep(10);
		if(str.size() != 10)
			TERROR;
		if(str.capacity() < 10)
			TERROR;
		str = _T("HelloWorld");
		if(str.size() != 10)
			TERROR;
		if(str.capacity() < 10)
			TERROR;
		str.resize_keep(20);
		if(str.find("HelloWorld", 0) != 0)
			TERROR;
		if(str.capacity() < 20)
			TERROR;
		str.resize_keep(5);
		if(str != "Hello")
			TERROR;
		if(str.capacity() < 20)
			TERROR;

		str.reserve_keep(100);
		if(str.size() != 5)
			TERROR;
		if(str != "Hello")
			TERROR;
		if(str.capacity() != 100)
			TERROR;
		str = "HelloWorld";
		if(str.size() != 10)
			TERROR;
		str.reserve_keep(5);
		if(str != "Hello")
			TERROR;
		if(str.size() != 5)
			TERROR;
		if(str.capacity() != 5)
			TERROR;

		x128.fromstring("12345678123456781234567812345678");
		str.clear();
		str += (GAIA::N8)-1;
		str += (GAIA::U8)1;
		str += (GAIA::N16)-1;
		str += (GAIA::U16)1;
		str += (GAIA::N32)-1;
		str += (GAIA::U32)1;
		str += (GAIA::N64)-1;
		str += (GAIA::U64)1;
		str += (GAIA::NM)-1;
		str += (GAIA::UM)1;
		str += (GAIA::F32)-1;
		str += (GAIA::F64)-1;
		str += (GAIA::WCH)1;
		str += (GAIA::BL)GAIA::True;
		str += (GAIA::BL)GAIA::False;
		str += x128;
		if(str != "-11-11-11-11-11-1.0-1.011012345678123456781234567812345678")
			TERROR;

		str.clear();
		str << "Hello" << "World" << 1 << GAIA::True;
		if(str != "HelloWorld11")
			TERROR;

		str.clear();
		str = str + (GAIA::N8)-1;
		str = str + (GAIA::U8)1;
		str = str + (GAIA::N16)-1;
		str = str + (GAIA::U16)1;
		str = str + (GAIA::N32)-1;
		str = str + (GAIA::U32)1;
		str = str + (GAIA::N64)-1;
		str = str + (GAIA::U64)1;
		str = str + (GAIA::NM)-1;
		str = str + (GAIA::UM)1;
		str = str + (GAIA::F32)-1;
		str = str + (GAIA::F64)-1;
		str = str + (GAIA::WCH)1;
		str = str + (GAIA::BL)GAIA::True;
		str = str + (GAIA::BL)GAIA::False;
		str = str + x128;
		if(str != "-11-11-11-11-11-1.0-1.011012345678123456781234567812345678")
			TERROR;

		str = (const GAIA::TCH*)GNIL;
		if((const GAIA::TCH*)str == GNIL)
			TERROR;
		if(str.fptr() == GNIL)
			TERROR;
		if(str != (const GAIA::TCH*)GNIL)
			TERROR;

		str = _T("Hello World, 测试utf8字符串！");
		GAIA::CTN::AString strUtf8 = str.toUtf8();
		GAIA::CTN::WString strWString = strUtf8.toWString(GAIA::CHARSET_TYPE_UTF8);
		if(strWString != str)
			TERROR;

		GAIA::CTN::WString strLocale(_T("Hello World, 测试utf8字符串！"));
		GAIA::CTN::AString strLocale1(strLocale.fptr(), "UTF-8");
		GAIA::CTN::WString strLocale2(strLocale1.fptr(), "UTF-8");
		if(strLocale2 != strLocale)
			TERROR;
	}
}
