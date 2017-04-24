#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_digit_percentage(GAIA::LOG::Log& logobj)
	{
		// Single character convert.
		{
			GAIA::CH szEncoded[32];
			GAIA::CH chDecoded;

			TAST(GAIA::DIGIT::percentage_encode('A', szEncoded, sizeof(szEncoded)) == 1);
			if(szEncoded[0] != 'A' || szEncoded[1] != '\0')
				TERROR;

			TAST(GAIA::DIGIT::percentage_encode('5', szEncoded, sizeof(szEncoded)) == 1);
			if(szEncoded[0] != '5' || szEncoded[1] != '\0')
				TERROR;

			TAST(GAIA::DIGIT::percentage_encode('%', szEncoded, sizeof(szEncoded)) == 3);
			if(szEncoded[0] != '%' || szEncoded[1] != '2' || szEncoded[2] != '5')
				TERROR;

			TAST(GAIA::DIGIT::percentage_encode(' ', szEncoded, sizeof(szEncoded)) == 1);
			if(szEncoded[0] != '+' || szEncoded[1] != '\0')
				TERROR;

			TAST(GAIA::DIGIT::percentage_decode("A", 1, chDecoded));
			TAST(chDecoded == 'A');

			TAST(GAIA::DIGIT::percentage_decode("5", 1, chDecoded));
			TAST(chDecoded == '5');

			TAST(GAIA::DIGIT::percentage_decode("%25", 1, chDecoded));
			TAST(chDecoded == '%');

			TAST(GAIA::DIGIT::percentage_decode("+", 1, chDecoded));
			TAST(chDecoded == ' ');
		}

		// String convert.
		{
			static const GAIA::CH TEST_CASE1[] = " 1`~!@#$%^&*()-=_+,.<>/?;':\"[]{}\\|";

			GAIA::CTN::AString strUtf8;
			GAIA::FSYS::File f;
			if(f.Open(_T("../testres/LOCALE/iamgaia.txt"), GAIA::FSYS::FileBase::OPEN_TYPE_READ))
			{
				strUtf8.resize((GAIA::N32)f.Size());
				f.Read(strUtf8.fptr(), (GAIA::N32)f.Size());
				f.Close();
			}

			GAIA::CH szEncoded[256];
			GAIA::CH szDecoded[256];
			GAIA::NUM sEncodedSize;
			GAIA::NUM sDecodedSize;

			sEncodedSize = GAIA::DIGIT::percentage_encode(TEST_CASE1, sizeof(TEST_CASE1) - 1, szEncoded, sizeof(szEncoded));
			if(sEncodedSize != GAIA::DIGIT::percentage_encode(TEST_CASE1, sizeof(TEST_CASE1) - 1, GNIL, 0))
				TERROR;
			if(sEncodedSize != GAIA::ALGO::gstrlen("+1%60%7E!%40%23%24%25%5E%26*()-%3D_%2B%2C.%3C%3E%2F%3F%3B%27%3A%22%5B%5D%7B%7D%5C%7C"))
				TERROR;
			if(!GAIA::ALGO::gstrequal(szEncoded, "+1%60%7E!%40%23%24%25%5E%26*()-%3D_%2B%2C.%3C%3E%2F%3F%3B%27%3A%22%5B%5D%7B%7D%5C%7C"))
				TERROR;

			sDecodedSize = GAIA::DIGIT::percentage_decode(szEncoded, sEncodedSize, szDecoded, sizeof(szDecoded));
			if(sDecodedSize != GAIA::DIGIT::percentage_decode(szEncoded, sEncodedSize, GNIL, 0))
				TERROR;
			if(sDecodedSize != sizeof(TEST_CASE1) - 1)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szDecoded, TEST_CASE1))
				TERROR;

			sEncodedSize = GAIA::DIGIT::percentage_encode(strUtf8.fptr(), strUtf8.size(), szEncoded, sizeof(szEncoded));
			if(sEncodedSize != GAIA::DIGIT::percentage_encode(strUtf8.fptr(), strUtf8.size(), GNIL, 0))
				TERROR;
			if(sEncodedSize != GAIA::ALGO::gstrlen("%E6%88%91%E6%98%AFGAIA"))
				TERROR;
			if(!GAIA::ALGO::gstrequal(szEncoded, "%E6%88%91%E6%98%AFGAIA"))
				TERROR;

			sDecodedSize = GAIA::DIGIT::percentage_decode(szEncoded, sEncodedSize, szDecoded, sizeof(szDecoded));
			if(sDecodedSize != GAIA::DIGIT::percentage_decode(szEncoded, sEncodedSize, GNIL, 0))
				TERROR;
			if(sDecodedSize != strUtf8.size())
				TERROR;
			if(!GAIA::ALGO::gstrequal(szDecoded, strUtf8.fptr()))
				TERROR;
		}
	}
}
