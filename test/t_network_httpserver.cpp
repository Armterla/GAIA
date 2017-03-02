#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class MyHttpServerCallBack : public GAIA::NETWORK::HttpServerCallBack
	{
	public:
		MyHttpServerCallBack(GAIA::NETWORK::HttpServer& svr)
			: GAIA::NETWORK::HttpServerCallBack(svr)
		{
		}
		virtual ~MyHttpServerCallBack()
		{
		}

	protected:
		virtual GAIA::BL OnRequest(
				GAIA::NETWORK::HttpServerLink& l,
				GAIA::NETWORK::HTTP_METHOD method,
				const GAIA::NETWORK::HttpURL& url,
				const GAIA::NETWORK::HttpHead& httphead,
				const GAIA::GVOID* p,
				GAIA::NUM sSize)
		{
			if(method != GAIA::NETWORK::HTTP_METHOD_GET)
				return GAIA::False;

			if(url.IsPure())
				return GAIA::False;

			GAIA::NETWORK::HttpHead head;
			l.Response(GAIA::NETWORK::HTTP_CODE_NOTFOUND, GNIL,
					   GAIA::NETWORK::HTTP_CODE_DESCRIPTION[GAIA::NETWORK::HTTP_CODE_NOTFOUND],
					GAIA::NETWORK::HTTP_CODE_DESCRIPTION_LENGTH[GAIA::NETWORK::HTTP_CODE_NOTFOUND]);
			l.Close();

			return GAIA::True;
		}

	private:
	};

	extern GAIA::GVOID t_network_httpserver(GAIA::LOG::Log& logobj)
	{
		GAIA::NETWORK::HttpServerDesc descServer;
		descServer.reset();
		descServer.pszRootPath = "../testres/HTTPSERVER/";

		GAIA::NETWORK::HttpServer svr;
		MyHttpServerCallBack cb1(svr), cb2(svr), cb3(svr);
		GAIA::NETWORK::HttpServerCallBack_Info cbi(svr);
		GAIA::NETWORK::HttpServerCallBack_StaticFile cbsf(svr);

		TAST(!svr.IsCreated());
		TAST(svr.Create(descServer));
		{
			TAST(svr.RegistCallBack(cb1));
			TAST(svr.RegistCallBack(cb2));
			TAST(svr.RegistCallBack(cb3));
			TAST(svr.RegistCallBack(cbi));
			TAST(svr.RegistCallBack(cbsf));
			TAST(!svr.RegistCallBack(cb1));
			TAST(!svr.RegistCallBack(cb2));
			TAST(!svr.RegistCallBack(cb3));
			TAST(!svr.RegistCallBack(cbi));
			TAST(!svr.RegistCallBack(cbsf));

			TAST(svr.IsCreated());
			TAST(!svr.IsBegin());
			TAST(svr.Begin());
			{
				TAST(svr.IsBegin());

				GAIA::NETWORK::Addr addrService1 = "127.0.0.1:9800";
				GAIA::NETWORK::Addr addrService2 = "127.0.0.1:9801";
				TAST(svr.OpenAddr(addrService1));
				TAST(svr.OpenAddr(addrService2));
				TAST(!svr.OpenAddr(addrService1));
				TAST(!svr.OpenAddr(addrService2));
				{
					// Sleep.
					for(GAIA::NUM x = 0; x < 100000; ++x)
						GAIA::SYNC::gsleep(1000);
				}
				TAST(svr.CloseAddr(addrService1));
				TAST(svr.CloseAddrAll());
				TAST(!svr.CloseAddr(addrService1));
				TAST(!svr.CloseAddr(addrService2));
				TAST(!svr.CloseAddrAll());
			}
			TAST(svr.End());
			TAST(svr.IsCreated());

			TAST(svr.UnregistCallBack(cb1));
			TAST(svr.UnregistCallBack(cb2));
			TAST(svr.UnregistCallBackAll());
			TAST(!svr.UnregistCallBack(cb1));
			TAST(!svr.UnregistCallBack(cb2));
			TAST(!svr.UnregistCallBackAll());
		}
		TAST(svr.Destroy());
	}
}