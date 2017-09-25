#ifndef 	__GAIA_NETWORK_ALIOSS_H__
#define 	__GAIA_NETWORK_ALIOSS_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_buffer.h"
#include "gaia_ctn_chars.h"
#include "gaia_ctn_string.h"
#include "gaia_ctn_charsstring.h"
#include "gaia_fsys_file.h"
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
			GINL GAIA::GVOID reset()
			{
				chsPublicKey.clear();
				chsPrivateKey.clear();
			}
			GINL GAIA::BL check() const
			{
				if(chsPublicKey.empty())
					return GAIA::False;
				if(chsPrivateKey.empty())
					return GAIA::False;
				return GAIA::True;
			}
		public:
			GAIA::CTN::AChars chsPublicKey;
			GAIA::CTN::AChars chsPrivateKey;
		};
		
		class AliOSS : public GAIA::Base
		{
		public:
			class HttpRequestForAliOSS : public GAIA::NETWORK::HttpRequest
			{
			public:
				GINL HttpRequestForAliOSS(GAIA::NETWORK::Http& http)
					: HttpRequest(http)
				{
					pDownloadBuf = GNIL;
					pListResult = GNIL;
				}
				GINL ~HttpRequestForAliOSS()
				{
				}
				GINL virtual GAIA::GVOID OnBegin()
				{
					
				}
				GINL virtual GAIA::GVOID OnEnd(GAIA::BL bCanceled)
				{
					
				}
				GINL virtual GAIA::GVOID OnState(GAIA::NETWORK::HTTP_REQUEST_STATE newstate)
				{
					
				}
				GINL virtual GAIA::GVOID OnPause()
				{
					
				}
				GINL virtual GAIA::GVOID OnResume()
				{
					
				}
				GINL virtual GAIA::GVOID OnSent(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
				{
					
				}
				GINL virtual GAIA::GVOID OnRecved(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
				{
					
				}
				GINL virtual GAIA::GVOID OnSentHead(const GAIA::CH* pszHttpVersion, GAIA::NETWORK::HTTP_METHOD method, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& head)
				{
					
				}
				GINL virtual GAIA::GVOID OnSentBody(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
				{
					
				}
				GINL virtual GAIA::GVOID OnRecvedHead(const GAIA::CH* pszHttpVersion, const GAIA::CH* pszHttpCode, const GAIA::CH* pszHttpCodeDesc, const GAIA::NETWORK::HttpHead& head)
				{
					resphead = head;
				}
				GINL virtual GAIA::GVOID OnRecvedBody(GAIA::N64 lOffset, const GAIA::GVOID* pData, GAIA::NUM sDataSize)
				{
					if(pDownloadBuf != GNIL)
						pDownloadBuf->write(pData, sDataSize);
					if(!file.IsOpen())
						file.Write(pData, sDataSize);
				}
				GINL virtual GAIA::N64 OnRequestBodyData(GAIA::N64 lOffset, GAIA::GVOID* pData, GAIA::NUM sMaxDataSize)
				{
					return 0;
				}
			public:
				GAIA::NETWORK::HttpHead resphead;
				GAIA::CTN::Buffer bufDownload;
				GAIA::CTN::Buffer* pDownloadBuf;
				GAIA::CTN::ACharsString strFileName;
				GAIA::FSYS::File file;
				GAIA::CTN::Vector<GAIA::CTN::ACharsString> listResult;
				GAIA::CTN::Vector<GAIA::CTN::ACharsString>* pListResult;
			};
			
		public:
			GINL AliOSS(GAIA::NETWORK::Http& http)
			{
				m_pHttp = &http;
			}
			
			GINL ~AliOSS()
			{
			}
			
			GINL GAIA::GVOID UploadData(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const GAIA::GVOID* pData, GAIA::NUM sSize, const AliOSSAccess& aoa, HttpRequestForAliOSS* pUserRequest = GNIL)
			{
				// Checkup parameters.
				if(GAIA::ALGO::gstremp(pszDomain))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszBucket))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszURL))
					GTHROW(InvalidParam);
				if(sSize < 0)
					GTHROW(InvalidParam);
				if(sSize == 0)
				{
					if(pData != GNIL)
						GTHROW(InvalidParam);
				}
				else
				{
					if(pData == GNIL)
						GTHROW(InvalidParam);
				}
				if(!aoa.check())
					GTHROW(InvalidParam);
				
				// New request.
				HttpRequestForAliOSS* pRequest;
				if(pUserRequest == GNIL)
					pRequest = gnew HttpRequestForAliOSS(*m_pHttp);
				else
					pRequest = pUserRequest;
				
				// Calculate URL.
				GAIA::CTN::ACharsString strCombineURL;
				this->CombineURL(pszDomain, pszBucket, pszURL, strCombineURL);
				GAIA::NETWORK::HttpURL url;
				url.FromString(strCombineURL.fptr());
				
				// Calculate host.
				GAIA::CTN::AChars chsHost;
				chsHost = pszBucket;
				chsHost += ".";
				chsHost += pszDomain;
				
				// Calculate content length.
				GAIA::CTN::AChars chsContentLength;
				chsContentLength = sSize;
				
				// Calculate date.
				GAIA::CTN::ACharsString strDate;
				this->GetCurrentDate(strDate);
				
				// Calculate signature.
				GAIA::CTN::ACharsString strSignature;
				this->Signature(GAIA::NETWORK::HTTP_METHOD_PUT, strDate.fptr(), pszBucket, pszURL, aoa, strSignature);
				
				// Initialize head.
				GAIA::NETWORK::HttpHead head;
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, chsHost.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, chsContentLength.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_DATE, strDate.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_AUTHORIZATION, strSignature.fptr());
				
				// Setup request.
				pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_PUT);
				pRequest->SetURL(url);
				pRequest->SetHead(head);
				pRequest->BindRequestBuffer(pData, sSize);
				
				// Request.
				pRequest->Request();
				
				// Wait.
				if(pUserRequest == GNIL)
				{
					pRequest->Wait();
					pRequest->drop_ref();
					pRequest = GNIL;
				}
			}
			
			GINL GAIA::GVOID DownloadData(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, GAIA::CTN::Buffer& buf, const AliOSSAccess& aoa, HttpRequestForAliOSS* pUserRequest = GNIL)
			{
				// Checkup parameters.
				if(GAIA::ALGO::gstremp(pszDomain))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszBucket))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszURL))
					GTHROW(InvalidParam);
				if(!aoa.check())
					GTHROW(InvalidParam);
				
				// New request.
				HttpRequestForAliOSS* pRequest;
				if(pUserRequest == GNIL)
				{
					pRequest = gnew HttpRequestForAliOSS(*m_pHttp);
					pRequest->pDownloadBuf = &buf;
					buf.clear();
				}
				else
				{
					pRequest = pUserRequest;
					pRequest->pDownloadBuf = &pRequest->bufDownload;
				}
				
				// Calculate URL.
				GAIA::CTN::ACharsString strCombineURL;
				this->CombineURL(pszDomain, pszBucket, pszURL, strCombineURL);
				GAIA::NETWORK::HttpURL url;
				url.FromString(strCombineURL.fptr());
				
				// Calculate host.
				GAIA::CTN::AChars chsHost;
				chsHost = pszBucket;
				chsHost += ".";
				chsHost += pszDomain;
				
				// Calculate date.
				GAIA::CTN::ACharsString strDate;
				this->GetCurrentDate(strDate);
				
				// Calculate signature.
				GAIA::CTN::ACharsString strSignature;
				this->Signature(GAIA::NETWORK::HTTP_METHOD_GET, strDate.fptr(), pszBucket, pszURL, aoa, strSignature);

				// Initialize head.
				GAIA::NETWORK::HttpHead head;
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, chsHost.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_DATE, strDate.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_AUTHORIZATION, strSignature.fptr());
				
				// Setup request.
				pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
				pRequest->SetURL(url);
				pRequest->SetHead(head);
				
				// Request.
				pRequest->Request();
				
				// Wait.
				if(pUserRequest == GNIL)
				{
					pRequest->Wait();
					pRequest->drop_ref();
					pRequest = GNIL;
				}
			}
			
			GINL GAIA::GVOID UploadFile(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const GAIA::CH* pszFile, const AliOSSAccess& aoa, HttpRequestForAliOSS* pUserRequest = GNIL)
			{
				// Checkup parameters.
				if(GAIA::ALGO::gstremp(pszDomain))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszBucket))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszURL))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszFile))
					GTHROW(InvalidParam);
				if(!aoa.check())
					GTHROW(InvalidParam);
				
				// New request.
				HttpRequestForAliOSS* pRequest;
				if(pUserRequest == GNIL)
					pRequest = gnew HttpRequestForAliOSS(*m_pHttp);
				else
					pRequest = pUserRequest;
				pRequest->strFileName = pszFile;
				
				// Open file.
				if(!pRequest->file.Open(pszFile, GAIA::FSYS::File::OPEN_TYPE_READ))
				{
					pRequest->drop_ref();
					pRequest = GNIL;
					GTHROW(IO);
				}
				
				// Calculate URL.
				GAIA::CTN::ACharsString strCombineURL;
				this->CombineURL(pszDomain, pszBucket, pszURL, strCombineURL);
				GAIA::NETWORK::HttpURL url;
				url.FromString(strCombineURL.fptr());
				
				// Calculate host.
				GAIA::CTN::AChars chsHost;
				chsHost = pszBucket;
				chsHost += ".";
				chsHost += pszDomain;
				
				// Calculate content length.
				GAIA::CTN::AChars chsContentLength;
				chsContentLength = pRequest->file.Size();
				
				// Calculate date.
				GAIA::CTN::ACharsString strDate;
				this->GetCurrentDate(strDate);
				
				// Calculate signature.
				GAIA::CTN::ACharsString strSignature;
				this->Signature(GAIA::NETWORK::HTTP_METHOD_PUT, strDate.fptr(), pszBucket, pszURL, aoa, strSignature);
				
				// Initialize head.
				GAIA::NETWORK::HttpHead head;
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, chsHost.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, chsContentLength.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_DATE, strDate.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_AUTHORIZATION, strSignature.fptr());
				
				// Setup request.
				pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_PUT);
				pRequest->SetURL(url);
				pRequest->SetHead(head);
				
				// Request.
				pRequest->Request();
				
				// Wait.
				if(pUserRequest == GNIL)
				{
					pRequest->Wait();
					pRequest->drop_ref();
					pRequest = GNIL;
				}
			}
			
			GINL GAIA::GVOID DownloadFile(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const GAIA::CH* pszFile, const AliOSSAccess& aoa, HttpRequestForAliOSS* pUserRequest = GNIL)
			{
				// Checkup parameters.
				if(GAIA::ALGO::gstremp(pszDomain))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszBucket))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszURL))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszFile))
					GTHROW(InvalidParam);
				if(!aoa.check())
					GTHROW(InvalidParam);
				
				// New request.
				HttpRequestForAliOSS* pRequest;
				if(pUserRequest == GNIL)
					pRequest = gnew HttpRequestForAliOSS(*m_pHttp);
				else
					pRequest = pUserRequest;
				pRequest->strFileName = pszFile;
				
				// Open file.
				if(!pRequest->file.Open(pszFile, GAIA::FSYS::File::OPEN_TYPE_WRITE | GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS))
				{
					pRequest->drop_ref();
					pRequest = GNIL;
					GTHROW(IO);
				}
				
				// Calculate URL.
				GAIA::CTN::ACharsString strCombineURL;
				this->CombineURL(pszDomain, pszBucket, pszURL, strCombineURL);
				GAIA::NETWORK::HttpURL url;
				url.FromString(strCombineURL.fptr());
				
				// Calculate host.
				GAIA::CTN::AChars chsHost;
				chsHost = pszBucket;
				chsHost += ".";
				chsHost += pszDomain;
				
				// Calculate date.
				GAIA::CTN::ACharsString strDate;
				this->GetCurrentDate(strDate);
				
				// Calculate signature.
				GAIA::CTN::ACharsString strSignature;
				this->Signature(GAIA::NETWORK::HTTP_METHOD_GET, strDate.fptr(), pszBucket, pszURL, aoa, strSignature);
				
				// Initialize head.
				GAIA::NETWORK::HttpHead head;
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, chsHost.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_DATE, strDate.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_AUTHORIZATION, strSignature.fptr());
				
				// Setup request.
				pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
				pRequest->SetURL(url);
				pRequest->SetHead(head);
				
				// Request.
				pRequest->Request();
				
				// Wait.
				if(pUserRequest == GNIL)
				{
					pRequest->Wait();
					pRequest->drop_ref();
					pRequest = GNIL;
				}
			}
			
			GINL GAIA::GVOID Collect(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const GAIA::CH* pszPrefix, GAIA::N64 lStart, GAIA::N64 lCount, GAIA::CTN::Vector<GAIA::CTN::ACharsString>& listResult, const AliOSSAccess& aoa, HttpRequestForAliOSS* pUserRequest = GNIL)
			{
				// Checkup parameters.
				if(GAIA::ALGO::gstremp(pszDomain))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszBucket))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszURL))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszPrefix))
					GTHROW(InvalidParam);
				if(lStart < 0)
					GTHROW(InvalidParam);
				if(lCount <= 0)
					GTHROW(InvalidParam);
				if(!aoa.check())
					GTHROW(InvalidParam);
				
				// New request.
				HttpRequestForAliOSS* pRequest;
				if(pUserRequest == GNIL)
				{
					pRequest = gnew HttpRequestForAliOSS(*m_pHttp);
					pRequest->pListResult = &listResult;
					listResult.clear();
				}
				else
				{
					pRequest = pUserRequest;
					pRequest->pListResult = &pRequest->listResult;
				}
				
				// Calculate URL.
				GAIA::CTN::ACharsString strCombineURL;
				this->CombineURL(pszDomain, pszBucket, "", strCombineURL);
				strCombineURL += "?prefix=";
				strCombineURL += pszPrefix;
				strCombineURL += "&marker=";
				strCombineURL += lStart;
				strCombineURL += "&delimiter=/&max-keys=";
				strCombineURL += lCount;
				GAIA::NETWORK::HttpURL url;
				url.FromString(strCombineURL.fptr());
				
				// Calculate host.
				GAIA::CTN::AChars chsHost;
				chsHost = pszBucket;
				chsHost += ".";
				chsHost += pszDomain;
				
				// Calculate date.
				GAIA::CTN::ACharsString strDate;
				this->GetCurrentDate(strDate);
				
				// Calculate signature.
				GAIA::CTN::ACharsString strSignature;
				this->Signature(GAIA::NETWORK::HTTP_METHOD_GET, strDate.fptr(), pszBucket, pszURL, aoa, strSignature);
				
				// Initialize head.
				GAIA::NETWORK::HttpHead head;
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, chsHost.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_DATE, strDate.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_AUTHORIZATION, strSignature.fptr());

				// Setup request.
				pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_GET);
				pRequest->SetURL(url);
				pRequest->SetHead(head);
				
				// Request.
				pRequest->Request();
				
				// Wait.
				if(pUserRequest == GNIL)
				{
					pRequest->Wait();
					pRequest->drop_ref();
					pRequest = GNIL;
				}
			}
			
			GINL GAIA::GVOID Delete(const GAIA::CH* pszDomain, const GAIA::CH* pszBucket, const GAIA::CH* pszURL, const AliOSSAccess& aoa, HttpRequestForAliOSS* pUserRequest = GNIL)
			{
				// Checkup parameters.
				if(GAIA::ALGO::gstremp(pszDomain))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszBucket))
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszURL))
					GTHROW(InvalidParam);
				if(!aoa.check())
					GTHROW(InvalidParam);
				
				// New request.
				HttpRequestForAliOSS* pRequest;
				if(pUserRequest == GNIL)
					pRequest = gnew HttpRequestForAliOSS(*m_pHttp);
				else
					pRequest = pUserRequest;
				
				// Calculate URL.
				GAIA::CTN::ACharsString strCombineURL;
				this->CombineURL(pszDomain, pszBucket, pszURL, strCombineURL);
				GAIA::NETWORK::HttpURL url;
				url.FromString(strCombineURL.fptr());
				
				// Calculate host.
				GAIA::CTN::AChars chsHost;
				chsHost = pszBucket;
				chsHost += ".";
				chsHost += pszDomain;
				
				// Calculate date.
				GAIA::CTN::ACharsString strDate;
				this->GetCurrentDate(strDate);
				
				// Calculate signature.
				GAIA::CTN::ACharsString strSignature;
				this->Signature(GAIA::NETWORK::HTTP_METHOD_DELETE, strDate.fptr(), pszBucket, pszURL, aoa, strSignature);
				
				// Initialize head.
				GAIA::NETWORK::HttpHead head;
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_HOST, chsHost.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_DATE, strDate.fptr());
				head.Set(GAIA::NETWORK::HTTP_HEADNAME_AUTHORIZATION, strSignature.fptr());
				
				// Setup request.
				pRequest->SetMethod(GAIA::NETWORK::HTTP_METHOD_DELETE);
				pRequest->SetURL(url);
				pRequest->SetHead(head);
				
				// Request.
				pRequest->Request();
				
				// Wait.
				if(pUserRequest == GNIL)
				{
					pRequest->Wait();
					pRequest->drop_ref();
					pRequest = GNIL;
				}
			}
			
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
				GAIA::NUM sLength = GAIA::DIGIT::base64encode(digbase64, (GAIA::NUM)sizeof(digbase64), dig, (GAIA::NUM)sizeof(dig));
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
