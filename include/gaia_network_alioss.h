#ifndef 	__GAIA_NETWORK_ALIOSS_H__
#define 	__GAIA_NETWORK_ALIOSS_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_buffer.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
#include "gaia_network_httpbase.h"
#include "gaia_network_http.h"

namespace GAIA
{
	namespace NETWORK
	{
		class AliOSSAccess : public GAIA::Base
		{
		public:
			GAIA::CTN::AChars chsPublicKey;
			GAIA::CTN::AChars chsPrivateKey;
		};
		
		class AliOSSAsync : public GAIA::Base
		{
		public:
			GINL AliOSSAsync();
			GINL ~AliOSSAsync();
			
			GINL virtual GAIA::GVOID OnBegin(){}
			GINL virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled){}
			GINL virtual GAIA::GVOID OnSendingData(const GAIA::CH* pszData, GAIA::N64 lOffset, GAIA::NUM sSize){}
			GINL virtual GAIA::GVOID OnRecvingData(const GAIA::CH* pszData, GAIA::N64 lOffset, GAIA::NUM sSize){}
			GINL virtual GAIA::GVOID OnProgress(GAIA::N64 lOffset, GAIA::N64 lTotal){}
			GINL virtual GAIA::GVOID OnComplete(GAIA::NETWORK::HTTP_CODE c){}
		};
		
		class AliOSS : public GAIA::Base
		{
		public:
			GINL AliOSS(GAIA::NETWORK::Http& http)
			{
				m_pHttp = &http;
			}
			
			GINL ~AliOSS()
			{
			}
			
			GINL GAIA::GVOID UploadData(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const GAIA::GVOID* pData, GAIA::NUM sSize, const AliOSSAccess& aoa, AliOSSAsync* pAsync = GNIL)
			{
				HttpRequestForAliOSS* pRequest = gnew HttpRequestForAliOSS(*m_pHttp);
				pRequest->pAsync = pAsync;
				
				GAIA::CTN::ACharsString strCombineURL;
				this->CombineURL(pszDomain, pszBucket, pszURL, strCombineURL);
				
				GAIA::NETWORK::HttpURL url;
				url.FromString(strCombineURL.fptr());
				
				GAIA::CTN::AChars chsHost;
				chsHost = pszBucket;
				chsHost += ".";
				chsHost += pszDomain;
				
				GAIA::CTN::AChars chsContentLength;
				chsContentLength = sSize;
				
				GAIA::CTN::ACharsString strDate;
				this->GetCurrentDate(strDate);
				
				GAIA::CTN::ACharsString strSignature;
				this->Signature(GAIA::NETWORK::HTTP_METHOD_PUT, strDate.fptr(), pszBucket, pszURL, aoa, strSignature);
				
				GAIA::NETWORK::HttpHead head;
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, chsHost.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, chsContentLength.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_DATE, strDate.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_AUTHORIZATION, strSignature.fptr());
				
				pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_PUT);
				pRequest->SetURL(url);
				pRequest->SetHead(head);
				
				pRequest->BindRequestBuffer(pData, sSize);
				
				pRequest->Request();
				
				if(pAsync == GNIL)
				{
					pRequest->Wait();
					pRequest->drop_ref();
				}
			}
			
			GINL GAIA::GVOID DownloadData(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, GAIA::CTN::Buffer& buf, const AliOSSAccess& aoa, AliOSSAsync* pAsync = GNIL)
			{
				
			}
			
			GINL GAIA::GVOID UploadFile(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const GAIA::CH* pszFile, const AliOSSAccess& aoa, AliOSSAsync* pAsync = GNIL)
			{
				
			}
			
			GINL GAIA::GVOID DownloadFile(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const GAIA::CH* pszFile, const AliOSSAccess& aoa, AliOSSAsync* pAsync = GNIL)
			{
				
			}
			
			GINL GAIA::GVOID Collect(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const AliOSSAccess& aoa, AliOSSAsync* pAsync = GNIL)
			{
				
			}
			
			GINL GAIA::GVOID Delete(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const AliOSSAccess& aoa, AliOSSAsync* pAsync = GNIL)
			{
				
			}
			
		private:
			class HttpRequestForAliOSS : public GAIA::NETWORK::HttpRequest
			{
			public:
				GINL HttpRequestForAliOSS(GAIA::NETWORK::Http& http)
					: HttpRequest(http)
				{
					pAsync = GNIL;
				}
				virtual GAIA::GVOID OnBegin()
				{
					
				}
				virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled)
				{
					
				}
				virtual GAIA::GVOID OnState(GAIA::NETWORK::HTTP_REQUEST_STATE newstate)
				{
					
				}
				virtual GAIA::GVOID OnPause()
				{
					
				}
				virtual GAIA::GVOID OnResume()
				{
					
				}
				virtual GAIA::GVOID OnSent(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
				{
					
				}
				virtual GAIA::GVOID OnRecved(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
				{
					
				}
				virtual GAIA::GVOID OnSentHead(const GAIA::CH* pszHttpVersion, GAIA::NETWORK::HTTP_METHOD method, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& head)
				{
					
				}
				virtual GAIA::GVOID OnSentBody(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
				{
					
				}
				virtual GAIA::GVOID OnRecvedHead(const GAIA::CH* pszHttpVersion, const GAIA::CH* pszHttpCode, const GAIA::CH* pszHttpCodeDesc, const GAIA::NETWORK::HttpHead& head)
				{

				}
				virtual GAIA::GVOID OnRecvedBody(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
				{

				}
				virtual GAIA::N64 OnRequestBodyData(GAIA::N64 lOffset, GAIA::GVOID* pData, GAIA::NUM sMaxDataSize)
				{
					return 0;
				}
			public:
				AliOSSAsync* pAsync;
			};
			
		private:
			GINL GAIA::GVOID GetCurrentDate(GAIA::CTN::ACharsString& strDate)
			{
				GAIA::TIME::Time t;
				t.gmttime();
				this->GetDate(t, strDate);
			}
			
			GINL GAIA::GVOID GetDate(const GAIA::TIME::Time& t, GAIA::CTN::ACharsString& strDate)
			{
				GAIA::CH szTime[64];
				t.to(szTime);
				GAIA::N32 nWeekDay = GAIA::TIME::weekday(t.y, t.mo, t.d);
				strDate = GAIA::TIME::WEEKDAY_NAME[nWeekDay];
				strDate += ", ";
				strDate += t.d;
				strDate += " ";
				strDate += GAIA::TIME::MONTH_NAME[t.mo - 1];
				strDate += " ";
				strDate += t.y;
				strDate += " ";
				strDate.append(szTime + 8, 2);
				strDate += ":";
				strDate.append(szTime + 10, 2);
				strDate += ":";
				strDate.append(szTime + 12, 2);
				strDate += " GMT";
			}
			
			GINL GAIA::GVOID CombineURL(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, GAIA::CTN::ACharsString& res)
			{
				res = "http://";
				res += pszBucket;
				res += ".";
				res += pszDomain;
				res += "/";
				res += pszURL;
			}
			
			GINL GAIA::GVOID Signature(GAIA::NETWORK::HTTP_METHOD method, const GAIA::CH* pszDate, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const AliOSSAccess& aoa, GAIA::CTN::ACharsString& res)
			{
				res = GAIA::NETWORK::HTTP_METHOD_STRING[method];
				res += "\n\n\n";
				res += pszDate;
				res += "\n";
				res += "/";
				res += pszBucket;
				res += "/";
				res += pszURL;
				GAIA::U8 dig[20];
				GAIA::DIGIT::hmacsha1(res.fptr(), res.size(), aoa.chsPrivateKey.fptr(), aoa.chsPrivateKey.size(), dig);
				GAIA::CH digbase64[128];
				GAIA::NUM sLength = GAIA::DIGIT::base64encode(digbase64, sizeof(digbase64), dig, sizeof(dig));
				digbase64[sLength] = '\0';

				res = "OSS ";
				res += aoa.chsPublicKey.fptr();
				res += ":";
				res += digbase64;
			}
			
		private:
			GAIA::NETWORK::Http* m_pHttp;
		};
	};
}

#endif
