#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_network_httpbase(GAIA::LOG::Log& logobj)
	{
		// HttpURL test.
		{
			GAIA::CH szProtocal[32];
			GAIA::CH szHostName[32];
			GAIA::CH szPort[32];
			GAIA::CH szPath[32];
			GAIA::CH szFullParam[32];
			GAIA::CH szFullQuery[32];
			GAIA::CH szFragment[32];

			const GAIA::CH* TEST_URL1 = "http://www.abc.com:4567/path1/path2/path3/param1;param2;param3;?a=1&b=2&c=3#frag";
			const GAIA::CH* TEST_URL2 = "https://www.abc.com:4567/path1/path2/path3/param1;param2;param3;?a=1&b=2&c=3#frag";

			GAIA::NETWORK::HttpURL hu;
			TAST(hu.Empty());
			hu = TEST_URL1;
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

			if(hu.GetPath(szPath, sizeof(szPath), &sPracSize) != szPath)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szPath, "/path1/path2/path3/"))
				TERROR;
			if(sPracSize != sizeof("/path1/path2/path3/") - 1)
				TERROR;

			if(hu.GetFullParam(szFullParam, sizeof(szFullParam), &sPracSize) != szFullParam)
				TERROR;
			if(!GAIA::ALGO::gstrequal(szFullParam, "param1;param2;param3;"))
				TERROR;
			if(sPracSize != sizeof("param1;param2;param3;") - 1)
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
			TAST(hu1 == TEST_URL1);

			GAIA::NETWORK::HttpURL hu2 = TEST_URL2;
			TAST(hu2 != hu);
			TAST(hu2 != TEST_URL1);

			const GAIA::CH* pszURL = hu;
			if(!GAIA::ALGO::gstrequal(pszURL, TEST_URL1))
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

			const GAIA::CH* pszValue = h.Get(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH);
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

			GAIA::NETWORK::HttpHead h1 = h;
			TAST(h1 == h);

			if(!h.Delete(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH))
				TERROR;
			if(h.Size() != 2)
				TERROR;

			h.Reset();
			TAST(h.Empty());
			TAST(h != h1);
		}
	}
}