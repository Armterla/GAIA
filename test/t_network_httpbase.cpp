#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_network_httpbase(GAIA::LOG::Log& logobj)
	{
		// HttpURL test.
		{
			GAIA::CH szProtocal[128];
			GAIA::CH szHostName[128];
			GAIA::CH szPort[128];
			GAIA::CH szPath[128];
			GAIA::CH szFullParam[128];
			GAIA::CH szFullQuery[128];
			GAIA::CH szFragment[128];
			GAIA::CH szRelativePart[128];

			static const GAIA::CH* TEST_URL1 = "127.0.0.1";
			static const GAIA::CH* TEST_URL2 = "127.0.0.1:1234";
			static const GAIA::CH* TEST_URL3 = "127.0.0.1/index.html";
			static const GAIA::CH* TEST_URL4 = "127.0.0.1:1234/path1/path2/path3?a=1&b=2&c=3";
			static const GAIA::CH* TEST_URL5 = "127.0.0.1/path/";
			static const GAIA::CH* TEST_URL6 = "127.0.0.1/";
			static const GAIA::CH* TEST_URL7 = "http://www.abc.com:4567/path1/path2/path3;param1;param2;param3?a=1&b=2&c=3#frag";
			static const GAIA::CH* TEST_URL8 = "https://www.abc.com:4567/path1/path2/path3;param1;param2;param3?a=1&b=2&c=3#frag";

			GAIA::NETWORK::HttpURL hu;

			hu.FromString(TEST_URL1);
			if(!hu.Analyze())
				TERROR;
			if(hu.GetHostName(szHostName) == GNIL || !GAIA::ALGO::gstrequal(szHostName, "127.0.0.1"))
				TERROR;
			if(hu.GetPath(szPath, sizeof(szPath)) != GNIL)
				TERROR;
			if(hu.GetRelativePart(szRelativePart, sizeof(szRelativePart)) != GNIL)
				TERROR;
			if(hu.GetRelativePart() != GNIL)
				TERROR;

			hu.FromString(TEST_URL2);
			if(!hu.Analyze())
				TERROR;
			if(hu.GetHostName(szHostName) == GNIL || !GAIA::ALGO::gstrequal(szHostName, "127.0.0.1"))
				TERROR;
			if(hu.GetRelativePart(szRelativePart, sizeof(szRelativePart)) != GNIL)
				TERROR;
			if(hu.GetRelativePart() != GNIL)
				TERROR;

			hu.FromString(TEST_URL3);
			if(!hu.Analyze())
				TERROR;
			if(hu.GetHostName(szHostName) == GNIL || !GAIA::ALGO::gstrequal(szHostName, "127.0.0.1"))
				TERROR;
			if(hu.GetPath(szPath, sizeof(szPath)) == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szPath, "/index.html"))
				TERROR;
			if(hu.GetRelativePart(szRelativePart, sizeof(szRelativePart)) == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szRelativePart, "/index.html"))
				TERROR;
			if(hu.GetRelativePart() == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(hu.GetRelativePart(), "/index.html"))
				TERROR;

			hu.FromString(TEST_URL4);
			if(!hu.Analyze())
				TERROR;
			if(hu.GetHostName(szHostName) == GNIL || !GAIA::ALGO::gstrequal(szHostName, "127.0.0.1"))
				TERROR;
			if(hu.GetPath(szPath, sizeof(szPath)) == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szPath, "/path1/path2/path3"))
				TERROR;
			if(hu.GetRelativePart(szRelativePart, sizeof(szRelativePart)) == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szRelativePart, "/path1/path2/path3?a=1&b=2&c=3"))
				TERROR;
			if(hu.GetRelativePart() == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(hu.GetRelativePart(), "/path1/path2/path3?a=1&b=2&c=3"))
				TERROR;

			hu.FromString(TEST_URL5);
			if(!hu.Analyze())
				TERROR;
			if(hu.GetHostName(szHostName) == GNIL || !GAIA::ALGO::gstrequal(szHostName, "127.0.0.1"))
				TERROR;

			hu.FromString(TEST_URL6);
			if(!hu.Analyze())
				TERROR;
			if(hu.GetHostName(szHostName) == GNIL || !GAIA::ALGO::gstrequal(szHostName, "127.0.0.1"))
				TERROR;
			if(hu.GetPath(szPath, sizeof(szPath)) == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szPath, "/"))
				TERROR;

			hu.Reset();

			TAST(hu.Empty());
			hu = TEST_URL7;
			TAST(!hu.Empty());

			GAIA::NUM sPracSize;
			if(hu.GetProtocal(szProtocal, sizeof(szProtocal), &sPracSize) != szProtocal)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szProtocal, "http"))
				TERROR;
			if(sPracSize != sizeof("http") - 1)
				TERROR;

			if(hu.GetHostName(szHostName, sizeof(szHostName), &sPracSize) != szHostName)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szHostName, "www.abc.com"))
				TERROR;
			if(sPracSize != sizeof("www.abc.com") - 1)
				TERROR;

			if(hu.GetPort(szPort, sizeof(szPort), &sPracSize) != szPort)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szPort, "4567"))
				TERROR;
			if(sPracSize != sizeof("4567") - 1)
				TERROR;
			if(hu.GetPort() != 4567)
				TERROR;

			if(hu.GetPath(szPath, sizeof(szPath), &sPracSize) != szPath)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szPath, "/path1/path2/path3"))
				TERROR;
			if(sPracSize != sizeof("/path1/path2/path3") - 1)
				TERROR;

			if(hu.GetFullParam(szFullParam, sizeof(szFullParam), &sPracSize) != szFullParam)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szFullParam, "param1;param2;param3"))
				TERROR;
			if(sPracSize != sizeof("param1;param2;param3") - 1)
				TERROR;

			if(hu.GetFullQuery(szFullQuery, sizeof(szFullQuery), &sPracSize) != szFullQuery)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szFullQuery, "a=1&b=2&c=3"))
				TERROR;
			if(sPracSize != sizeof("a=1&b=2&c=3") - 1)
				TERROR;

			if(hu.GetFragment(szFragment, sizeof(szFragment), &sPracSize) != szFragment)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szFragment, "frag"))
				TERROR;
			if(sPracSize != sizeof("frag") - 1)
				TERROR;

			TAST(hu.GetParamCount() == 3);
			for(GAIA::NUM x = 0; x < hu.GetParamCount(); ++x)
			{
				GAIA::CH szParam[32];
				GAIA::NUM sParamResultSize;
				if(!hu.GetParam(x, szParam, sizeof(szParam), &sParamResultSize))
				{
					TERROR;
					break;
				}
				if(x == 0)
				{
					if(!GAIA::ALGO::gstrequal(szParam, "param1"))
					{
						TERROR;
						break;
					}
				}
				else if(x == 1)
				{
					if(!GAIA::ALGO::gstrequal(szParam, "param2"))
					{
						TERROR;
						break;
					}
				}
				else if(x == 2)
				{
					if(!GAIA::ALGO::gstrequal(szParam, "param3"))
					{
						TERROR;
						break;
					}
				}
				if(sParamResultSize != 6)
				{
					TERROR;
					break;
				}
			}

			TAST(hu.GetQueryCount() == 3);
			for(GAIA::NUM x = 0; x < hu.GetQueryCount(); ++x)
			{
				GAIA::CH szName[32];
				GAIA::CH szValue[32];
				GAIA::NUM sNameResultSize;
				GAIA::NUM sValueResultSize;
				if(!hu.GetQuery(x, szName, szValue, sizeof(szName), sizeof(szValue), &sNameResultSize, &sValueResultSize))
				{
					TERROR;
					break;
				}
				if(x == 0)
				{
					if(!GAIA::ALGO::gstrequal(szName, "a"))
					{
						TERROR;
						break;
					}
					else if(!GAIA::ALGO::gstrequal(szValue, "1"))
					{
						TERROR;
						break;
					}
				}
				else if(x == 1)
				{
					if(!GAIA::ALGO::gstrequal(szName, "b"))
					{
						TERROR;
						break;
					}
					else if(!GAIA::ALGO::gstrequal(szValue, "2"))
					{
						TERROR;
						break;
					}
				}
				else if(x == 2)
				{
					if(!GAIA::ALGO::gstrequal(szName, "c"))
					{
						TERROR;
						break;
					}
					else if(!GAIA::ALGO::gstrequal(szValue, "3"))
					{
						TERROR;
						break;
					}
				}
				if(sNameResultSize != 1 || sValueResultSize != 1)
				{
					TERROR;
					break;
				}
				GAIA::CH szValueByName[32];
				GAIA::NUM sValueByNameResultSize;
				if(hu.GetQueryByName(szName, szValueByName, sizeof(szValueByName), &sValueByNameResultSize) == GNIL)
				{
					TERROR;
					break;
				}
				if(sValueByNameResultSize != 1)
				{
					TERROR;
					break;
				}
			}

			GAIA::NETWORK::HttpURL hu1 = hu;
			TAST(hu1 == hu);
			TAST(hu1 == TEST_URL7);

			GAIA::NETWORK::HttpURL hu2 = TEST_URL8;
			TAST(hu2 != hu);
			TAST(hu2 != TEST_URL7);

			const GAIA::CH* pszURL = hu;
			if(!GAIA::ALGO::gstrequal(pszURL, TEST_URL7))
				TERROR;

			hu.Reset();
			hu1.Reset();
			hu2.Reset();

			TAST(hu.Empty());
			TAST(hu1.Empty());
			TAST(hu2.Empty());	
		}

		// HttpHead test.
		{
			GAIA::NETWORK::HttpHead h;
			TAST(h.Empty());

			if(!h.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, "123"))
				TERROR;
			if(h.Size() != 1)
				TERROR;
			TAST(!h.Empty());
			if(!h.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE, "html"))
				TERROR;
			if(h.Size() != 2)
				TERROR;
			if(!h.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, "www.abc.com"))
				TERROR;
			if(h.Size() != 3)
				TERROR;

			const GAIA::CH* pszValue = h.GetValueByName(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "123"))
				TERROR;
			pszValue = h.GetValueByName(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "html"))
				TERROR;
			pszValue = h.GetValueByName(GAIA::NETWORK::HTTP_HEADNAME_HOST);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "www.abc.com"))
				TERROR;

			pszValue = h.Get(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "123"))
				TERROR;
			pszValue = h.Get(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "html"))
				TERROR;
			pszValue = h.Get(GAIA::NETWORK::HTTP_HEADNAME_HOST);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "www.abc.com"))
				TERROR;
			for(GAIA::NUM x = 0; x < h.Size(); ++x)
			{
				GAIA::CH* pszName;
				GAIA::CH* pszValue;
				if(!h.GetNameAndValue(x, &pszName, &pszValue))
				{
					TERROR;
					break;
				}
				if(GAIA::ALGO::gstrequal(pszName, GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH))
				{
					if(!GAIA::ALGO::gstrequal(pszValue, "123"))
					{
						TERROR;
						break;
					}
				}
				else if(GAIA::ALGO::gstrequal(pszName, GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE))
				{
					if(!GAIA::ALGO::gstrequal(pszValue, "html"))
					{
						TERROR;
						break;
					}
				}
				else if(GAIA::ALGO::gstrequal(pszName, GAIA::NETWORK::HTTP_HEADNAME_HOST))
				{
					if(!GAIA::ALGO::gstrequal(pszValue, "www.abc.com"))
					{
						TERROR;
						break;
					}
				}
				else
				{
					TERROR;
					break;
				}
			}

			if(!h.Exist(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH))
				TERROR;
			if(!h.Exist(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE))
				TERROR;
			if(!h.Exist(GAIA::NETWORK::HTTP_HEADNAME_HOST))
				TERROR;

			pszValue = h.GetValueByName(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "123"))
				TERROR;
			pszValue = h.GetValueByName(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "html"))
				TERROR;
			pszValue = h.GetValueByName(GAIA::NETWORK::HTTP_HEADNAME_HOST);
			if(pszValue == GNIL)
				TERROR;
			if(!GAIA::ALGO::gstrequal(pszValue, "www.abc.com"))
				TERROR;

			GAIA::NETWORK::HttpHead h1 = h;
			TAST(h1 == h);

			if(!h.Delete(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH))
				TERROR;
			if(h.Size() != 2)
				TERROR;

			h.Reset();
			TAST(h.Empty());
			TAST(h != h1);

			static const GAIA::CH HEAD_SAMPLE[] = "a: 1\r\nb: 2\r\nc: 3\r\n";
			h = HEAD_SAMPLE;
			h.FromString(HEAD_SAMPLE);
			if(h.GetStringSize() != sizeof(HEAD_SAMPLE) - 1)
				TERROR;
			GAIA::CH szTemp[256];
			GAIA::NUM sResultSize;
			GAIA::BL bResult;
			if(h.ToString(szTemp, sizeof(szTemp), &sResultSize, &bResult) != szTemp)
				TERROR;
			TAST(bResult);
			TAST(sResultSize == sizeof(HEAD_SAMPLE) - 1);
			TAST(GAIA::ALGO::gstrequal(HEAD_SAMPLE, szTemp));

			TAST(h.FromString("a:1\r\n"));
			TAST(!h.FromString("a=1\r\n"));

			h = "";
			TAST(h.Empty());
			TAST(h.Size() == 0);
			TAST(h.GetStringSize() == 0);
			if(h.ToString(szTemp, sizeof(szTemp), &sResultSize, &bResult) != szTemp)
				TERROR;
			if(GAIA::ALGO::gstrlen(szTemp) != 0)
				TERROR;

			h = GNIL;
			TAST(h.Empty());
			TAST(h.Size() == 0);
			TAST(h.GetStringSize() == 0);
			if(h.ToString(szTemp, sizeof(szTemp), &sResultSize, &bResult) != szTemp)
				TERROR;
			if(GAIA::ALGO::gstrlen(szTemp) != 0)
				TERROR;
		}
	}
}