#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class MyHttpRequest : public GAIA::NETWORK::HttpRequest
	{
	public:
		MyHttpRequest(GAIA::NETWORK::Http& http)
			: GAIA::NETWORK::HttpRequest(http)
		{
			this->init();
		}
		~MyHttpRequest()
		{
		}

	public:
		virtual GAIA::GVOID OnBegin()
		{
			aBegin++;
		}
		virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled)
		{
			aEnd++;
			if(bCanceled)
				aEndWithCancel++;
		}
		virtual GAIA::GVOID OnState(GAIA::NETWORK::HTTP_REQUEST_STATE newstate)
		{
			aState++;
			aOldStateList[this->GetState()]++;
			aNewStateList[newstate]++;
		}
		virtual GAIA::GVOID OnPause()
		{
			aPause++;
		}
		virtual GAIA::GVOID OnResume()
		{
			aResume++;
		}
		virtual GAIA::GVOID OnSent(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
		{
			aSent++;
			aSentSize += sDataSize;
		}
		virtual GAIA::GVOID OnRecved(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
		{
			aRecved++;
			aRecvedSize += sDataSize;
		}
		virtual GAIA::GVOID OnSentHead(const GAIA::CH* pszHttpVersion, GAIA::NETWORK::HTTP_METHOD method, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& head)
		{
			aSentHead++;
			aSentHeadSize += head.GetStringSize();
		}
		virtual GAIA::GVOID OnSentBody(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
		{
			aSentBody++;
			aSentBodySize += sDataSize;
		}
		virtual GAIA::GVOID OnRecvedHead(const GAIA::CH* pszHttpVersion, const GAIA::CH* pszHttpCode, const GAIA::CH* pszHttpCodeDesc, const GAIA::NETWORK::HttpHead& head)
		{
			aRecvedHead++;
			aRecvedHeadSize += head.GetStringSize();
		}
		virtual GAIA::GVOID OnRecvedBody(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
		{
			aRecvedBody++;
			aRecvedBodySize += sDataSize;
		}
		virtual GAIA::N64 OnRequestBodyData(GAIA::N64 lOffset, GAIA::GVOID* pData, GAIA::NUM sMaxDataSize)
		{
			GAIA::N64 lRet = 0;
			aRequestBodyData++;
			aRequestBodyDataSize += lRet;
			return lRet;
		}

	public:
		GINL GAIA::GVOID CheckNotUsed(GAIA::LOG::Log& logobj)
		{
			TAST(aBegin == 0);
			TAST(aEnd == 0);
			TAST(aEndWithCancel == 0);
			TAST(aState == 0);
			TAST(aPause == 0);
			TAST(aResume == 0);
			TAST(aSent == 0);
			TAST(aRecved == 0);
			TAST(aSentHead == 0);
			TAST(aSentBody == 0);
			TAST(aRecvedHead == 0);
			TAST(aRecvedBody == 0);
			TAST(aRequestBodyData == 0);
			for(GAIA::NUM x = 0; x < sizeofarray(aOldStateList); ++x)
			{
				if(aOldStateList[x] != 0)
				{
					TERROR;
					break;
				}
			}
			for(GAIA::NUM x = 0; x < sizeofarray(aNewStateList); ++x)
			{
				if(aNewStateList[x] != 0)
				{
					TERROR;
					break;
				}
			}
			TAST(aSentSize == 0);
			TAST(aRecvedSize == 0);
			TAST(aSentHeadSize == 0);
			TAST(aSentBodySize == 0);
			TAST(aRecvedHeadSize == 0);
			TAST(aRecvedBodySize == 0);
			TAST(aRequestBodyDataSize == 0);
		}
		GINL GAIA::GVOID CheckSmallGet(GAIA::LOG::Log& logobj)
		{
			TAST(aBegin == 1);
			TAST(aEnd == 1);
			TAST(aEndWithCancel == 0);
			TAST(aState > 0);
			TAST(aPause == 0);
			TAST(aResume == 0);
			TAST(aSent > 0);
			TAST(aRecved > 0);
			TAST(aSentHead > 0);
			TAST(aRecvedHead > 0);
			TAST(aRecvedBody > 0);
			TAST(aRequestBodyData == 1);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID] == 0);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID] == 0);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_READY] == 1);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_READY] == 0);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING] == 1);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING] == 1);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING] == 1);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING] == 1);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE] == 0);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE] == 1);
			TAST(aSentSize > 0);
			TAST(aRecvedSize > 0);
			TAST(aSentHeadSize > 0);
			TAST(aSentBodySize == 0);
			TAST(aRecvedHeadSize > 0);
			TAST(aRecvedBodySize > 0);
			TAST(aRequestBodyDataSize == 0);
		}
		GINL GAIA::GVOID CheckSmallPost(GAIA::LOG::Log& logobj)
		{

		}
		GINL GAIA::GVOID CheckBigGet(GAIA::LOG::Log& logobj)
		{
			TAST(aBegin == 1);
			TAST(aEnd == 1);
			TAST(aEndWithCancel == 0);
			TAST(aState > 0);
			TAST(aPause == 0);
			TAST(aResume == 0);
			TAST(aSent > 0);
			TAST(aRecved > 0);
			TAST(aSentHead > 0);
			TAST(aRecvedHead > 0);
			TAST(aRecvedBody > 0);
			TAST(aRequestBodyData == 0);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID] == 0);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID] == 0);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_READY] == 1);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_READY] == 0);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING] == 1);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING] == 1);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING] == 1);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING] == 1);
			TAST(aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE] == 0);
			TAST(aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE] == 1);
			TAST(aSentSize > 0);
			TAST(aRecvedSize > 0);
			TAST(aSentHeadSize > 0);
			TAST(aSentBodySize == GAIA::ALGO::gstrlen("Hello world! and hello kitty!"));
			TAST(aRecvedHeadSize > 0);
			TAST(aRecvedBodySize > 0);
			TAST(aRequestBodyDataSize == 0);
		}
		GINL GAIA::GVOID CheckBigPut(GAIA::LOG::Log& logobj)
		{

		}

	private:
		GINL GAIA::GVOID init()
		{
		}

	public:
		GAIA::SYNC::Atomic aBegin;
		GAIA::SYNC::Atomic aEnd;
		GAIA::SYNC::Atomic aEndWithCancel;
		GAIA::SYNC::Atomic aState;
		GAIA::SYNC::Atomic aPause;
		GAIA::SYNC::Atomic aResume;
		GAIA::SYNC::Atomic aSent;
		GAIA::SYNC::Atomic aRecved;
		GAIA::SYNC::Atomic aSentHead;
		GAIA::SYNC::Atomic aSentBody;
		GAIA::SYNC::Atomic aRecvedHead;
		GAIA::SYNC::Atomic aRecvedBody;
		GAIA::SYNC::Atomic aRequestBodyData;

		GAIA::SYNC::Atomic aOldStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_MAXENUMCOUNT];
		GAIA::SYNC::Atomic aNewStateList[GAIA::NETWORK::HTTP_REQUEST_STATE_MAXENUMCOUNT];

		GAIA::SYNC::Atomic aSentSize;
		GAIA::SYNC::Atomic aRecvedSize;
		GAIA::SYNC::Atomic aSentHeadSize;
		GAIA::SYNC::Atomic aSentBodySize;
		GAIA::SYNC::Atomic aRecvedHeadSize;
		GAIA::SYNC::Atomic aRecvedBodySize;
		GAIA::SYNC::Atomic aRequestBodyDataSize;
	};

	extern GAIA::GVOID t_network_http(GAIA::LOG::Log& logobj)
	{
		static const GAIA::CH* TEST_URL = "http://www.qu.la/book/176/143187.html";
		static const GAIA::CH* TEST_HOST = "www.qu.la";
		static const GAIA::NUM SAMPLE_COUNT = 10;
		static const GAIA::NUM NETWORK_THREAD_COUNT = GINVALID;
		static const GAIA::NUM WORK_THREAD_COUNT = GINVALID;

		GAIA::NETWORK::HttpServerDesc descServer;
		descServer.reset();
		descServer.pszRootPath = "../testres/HTTPSERVER/";
		descServer.bEnableSocketTCPNoDelay = GAIA::True;
		descServer.bEnableSocketNoBlock = GAIA::True;
		descServer.bEnableSocketReuseAddr = GAIA::True;
		descServer.nListenSocketSendBufferSize = 1024 * 64;
		descServer.nListenSocketRecvBufferSize = 1024 * 64;
		descServer.nAcceptedSocketSendBufferSize = 1024 * 64;
		descServer.nAcceptedSocketRecvBufferSize = 1024 * 64;
		if(NETWORK_THREAD_COUNT != GINVALID)
			descServer.sNetworkThreadCount = NETWORK_THREAD_COUNT;
		if(WORK_THREAD_COUNT != GINVALID)
			descServer.sWorkThreadCount = WORK_THREAD_COUNT;
		GAIA::NETWORK::HttpServer svr;
		GAIA::NETWORK::HttpServerCallBackForInfo cbi(svr);
		GAIA::NETWORK::HttpServerCallBackForStaticResource cbsf(svr);
		svr.EnableLog(TMODULE_LOG_ENABLED);
		TAST(svr.Create(descServer));
		{
			((GAIA::NETWORK::AsyncDispatcher*)svr.GetAsyncDispatcher())->EnableLog(TMODULE_LOG_ENABLED);

			TAST(svr.RegistCallBack(cbi));
			TAST(svr.RegistCallBack(cbsf));
			TAST(svr.UnregistCallBack(cbi));
			TAST(svr.UnregistCallBack(cbsf));
			TAST(svr.RegistCallBack(cbi));
			TAST(svr.RegistCallBack(cbsf));
			TAST(svr.UnregistCallBackAll());
			TAST(svr.RegistCallBack(cbi));
			TAST(svr.RegistCallBack(cbsf));

			TAST(svr.Begin());
			{
				GAIA::CTN::Vector<GAIA::NETWORK::IP> listHostIP;
				GAIA::NETWORK::GetHostIPList(GAIA::NETWORK::GAIA_LOCAL_HOST, listHostIP);

				GAIA::NETWORK::Addr addrService1;
				if(listHostIP.empty())
					addrService1 = "127.0.0.1:8903";
				else
				{
					addrService1.ip = listHostIP[0];
					addrService1.uPort = 8903;
				}
				GAIA::CH szMyAddress[32];
				GAIA::CH szMyUrl[64];

				if(GAIA::True)
				{
					addrService1.tostring(szMyAddress);
					GAIA::ALGO::gstrcpy(szMyUrl, "http://");
					GAIA::ALGO::gstrcat(szMyUrl, szMyAddress);
					GAIA::ALGO::gstrcat(szMyUrl, "/httpinfo");
				}
				else
				{
					GAIA::ALGO::gstrcpy(szMyAddress, TEST_HOST);
					GAIA::ALGO::gstrcpy(szMyUrl, TEST_URL);
				}

				TAST(svr.OpenAddr(addrService1));
				{
					GAIA::NETWORK::HttpHead head;
					head.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, szMyAddress);
					head.Set(GAIA::NETWORK::HTTP_HEADNAME_USERAGENT, "Gaia/0.0.2");
					head.Set(GAIA::NETWORK::HTTP_HEADNAME_ACCEPT, "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
					head.Optimize();

					GAIA::NETWORK::Http http;
					TAST(!http.IsCreated());
					TAST(!http.IsBegin());
					http.EnableLog(TMODULE_LOG_ENABLED);

					GAIA::NETWORK::HttpDesc descHttp;
					descHttp.reset();
					descHttp.bEnableSocketReuseAddr = GAIA::True;
					descHttp.nSocketSendBufferSize = 1024 * 4;
					descHttp.nSocketRecvBufferSize = 1024 * 64;
					if(NETWORK_THREAD_COUNT != GINVALID)
						descHttp.sNetworkThreadCount = NETWORK_THREAD_COUNT;
					if(WORK_THREAD_COUNT != GINVALID)
						descHttp.sWorkThreadCount = WORK_THREAD_COUNT;
					TAST(http.Create(descHttp));
					{
						((GAIA::NETWORK::AsyncDispatcher*)http.GetAsyncDispatcher())->EnableLog(TMODULE_LOG_ENABLED);

						TAST(http.IsCreated());
						TAST(!http.IsBegin());

						// Empty request test.
						TAST(http.Begin());
						{
							for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
							{
								MyHttpRequest* pRequest = gnew MyHttpRequest(http);
								pRequest->CheckNotUsed(logobj);
								pRequest->drop_ref();
							}
						}
						TAST(http.End());

						// Single request test.
						TAST(http.Begin());
						{
							MyHttpRequest* pRequest = gnew MyHttpRequest(http);
							pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
							pRequest->SetURL(szMyUrl);
							pRequest->SetHead(head);
							pRequest->Request();
							pRequest->Wait();
							pRequest->CheckSmallGet(logobj);
							pRequest->drop_ref();
						}
						TAST(http.End());
						
						// Add Content-Length to head.
						GAIA::CH szBody[32] = "Hello World! And hello kitty!";
						GAIA::CTN::AChars chsTemp;
						head.Reset();
						chsTemp = GAIA::ALGO::gstrlen(szBody);
						head.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, chsTemp.fptr());
						head.Optimize();

						// Serial request test.
						TAST(http.Begin());
						{
							for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
							{
								MyHttpRequest* pRequest = gnew MyHttpRequest(http);
								pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
								pRequest->SetURL(szMyUrl);
								pRequest->SetHead(head);
								pRequest->BindRequestBuffer(szBody, GAIA::ALGO::gstrlen(szBody));
								pRequest->Request();
								pRequest->Wait();
								pRequest->CheckBigGet(logobj);
								pRequest->drop_ref();
							}
						}
						TAST(http.End());

						// Parallel request test.
						GAIA::CTN::Vector<MyHttpRequest*> listRequest;
						TAST(http.Begin());
						{
							listRequest.clear();
							for(GAIA::NUM x = 0; x < SAMPLE_COUNT * 10; ++x)
							{
								MyHttpRequest* pRequest = gnew MyHttpRequest(http);
								pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
								pRequest->SetURL(szMyUrl);
								pRequest->SetHead(head);
								pRequest->BindRequestBuffer(szBody, GAIA::ALGO::gstrlen(szBody));
								pRequest->Request();
								listRequest.push_back(pRequest);
							}

							for(GAIA::NUM x = 0; x < listRequest.size(); ++x)
							{
								MyHttpRequest* pRequest = listRequest[x];
								GAST(pRequest != GNIL);
								pRequest->Wait();
								pRequest->CheckBigGet(logobj);
								pRequest->drop_ref();
							}
						}
						TAST(http.End());
					}
					TAST(http.Destroy());

					TAST(!http.IsCreated());
					TAST(!http.IsBegin());
				}
				TAST(svr.CloseAddr(addrService1));
			}
			TAST(svr.End());
			
			TAST(svr.UnregistCallBack(cbi));
			TAST(svr.UnregistCallBack(cbsf));
		}
		TAST(svr.Destroy());
	}
}
