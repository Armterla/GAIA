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
		virtual GAIA::GVOID OnSentHead(GAIA::NETWORK::HTTP_METHOD method, const GAIA::NETWORK::HttpURL& url, const GAIA::CH* pszHttpVersion, const GAIA::NETWORK::HttpHead& head)
		{
			aSentHead++;
		}
		virtual GAIA::GVOID OnSentBody(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
		{
			aSentBody++;
			aSentBodySize += sDataSize;
		}
		virtual GAIA::GVOID OnRecvedHead(const GAIA::CH* pszHttpVersion, const GAIA::CH* pszHttpCode, const GAIA::CH* pszHttpCodeDesc, const GAIA::NETWORK::HttpHead& head)
		{
			aRecvedHead++;
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

		}
		GINL GAIA::GVOID CheckSmallGet(GAIA::LOG::Log& logobj)
		{

		}
		GINL GAIA::GVOID CheckSmallPost(GAIA::LOG::Log& logobj)
		{

		}
		GINL GAIA::GVOID CheckBigGet(GAIA::LOG::Log& logobj)
		{

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
		static const GAIA::CH* TEST_URL = "http://120.26.219.54:48010/httpstat?";
		static const GAIA::NUM SAMPLE_COUNT = 10;

		GAIA::CTN::Vector<MyHttpRequest*> listRequest;

		GAIA::NETWORK::Http http;
		TAST(!http.IsCreated());
		TAST(!http.IsBegin());

		GAIA::NETWORK::HttpDesc descHttp;
		descHttp.reset();
		TAST(http.Create(descHttp));
		{
			TAST(http.IsCreated());
			TAST(!http.IsBegin());

			// Empty request test.
			TAST(http.Begin());
			{
				for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				{
					MyHttpRequest* pRequest = gnew MyHttpRequest(http);
					pRequest->drop_ref();
				}
			}
			TAST(http.End());

			// Single request test.
			TAST(http.Begin());
			{
				MyHttpRequest* pRequest = gnew MyHttpRequest(http);
				pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
				pRequest->SetURL(TEST_URL);
				pRequest->Request();
				pRequest->Wait();
				pRequest->drop_ref();
			}
			TAST(http.End());

			// Serial request test.
			TAST(http.Begin());
			{
				for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				{
					MyHttpRequest* pRequest = gnew MyHttpRequest(http);
					pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
					pRequest->SetURL(TEST_URL);
					pRequest->Request();
					pRequest->Wait();
					pRequest->drop_ref();
				}
			}
			TAST(http.End());

			// Parallel request test.
			TAST(http.Begin());
			{
				listRequest.clear();
				for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				{
					MyHttpRequest* pRequest = gnew MyHttpRequest(http);
					pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
					pRequest->SetURL(TEST_URL);
					pRequest->Request();
					listRequest.push_back(pRequest);
				}

				for(GAIA::NUM x = 0; x < listRequest.size(); ++x)
				{
					MyHttpRequest* pRequest = listRequest[x];
					GAST(pRequest != GNIL);
					pRequest->Wait();
					pRequest->drop_ref();
				}
			}
			TAST(http.End());
		}
		TAST(http.Destroy());

		TAST(!http.IsCreated());
		TAST(!http.IsBegin());
	}
}