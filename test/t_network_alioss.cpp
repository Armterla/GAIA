#include "preheader.h"
#include "t_common.h"

namespace TEST
{	
	extern GAIA::GVOID t_network_alioss(GAIA::LOG::Log& logobj)
	{
		GAIA::NETWORK::Http http;
		
		GAIA::NETWORK::HttpDesc descHttp;
		descHttp.reset();
		http.Create(descHttp);
		
		http.Begin();
		{
			// Initialize aoa.
			GAIA::NETWORK::AliOSSAccess aoa;
			aoa.chsPublicKey = "LTAIIT6WlOVjWQT7";
			aoa.chsPrivateKey = "";
			
			GAIA::NETWORK::AliOSS oss(http);
			
			// Sync mode
			{
				// Upload data test.
				oss.UploadData("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Test/helloworld.txt", "0123456789", 10, aoa);
				
				// Download data test.
				GAIA::CTN::Buffer buf;
				oss.DownloadData("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Test/helloworld.txt", buf, aoa);
				if(buf.write_size() != 10)
					TERROR;
				GAIA::CTN::AChars chsData;
				chsData.assign(buf.fptr(), buf.write_size());
				if(chsData != "0123456789")
					TERROR;
				
				// Upload file test.
				GAIA::FSYS::File f;
				if(!f.Open("./hellokitty.txt", GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS | GAIA::FSYS::File::OPEN_TYPE_WRITE))
					TERROR;
				else
				{
					f.Write("9876543210", 10);
					f.Close();
				}
				oss.UploadFile("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Test/hellokitty.txt", "./hellokitty.txt", aoa);
				
				// Download file test.
				oss.DownloadFile("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Test/hellokitty.txt", "./hellokitty1.txt", aoa);
				if(!f.Open("./hellokitty1.txt", GAIA::FSYS::File::OPEN_TYPE_READ))
					TERROR;
				else
				{
					buf.clear();
					f.ReadRemainAll(buf);
					chsData.assign(buf.fptr(), buf.write_size());
					if(chsData != "9876543210")
						TERROR;
				}
				
				// Collect file test.
				GAIA::CTN::Vector<GAIA::NETWORK::AliOSSObjInfo> listResult;
				oss.Collect("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Test/hello", "", 0, 1000, listResult, aoa);
				
				// Delete file test.
				oss.Delete("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Test/helloworld.txt", aoa);
				oss.Delete("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Test/hellokitty.txt", aoa);
			}
			
			// Async mode.
			{
				static const GAIA::NUM SAMPLE_COUNT = 10;
				
				GAIA::CTN::Vector<GAIA::NETWORK::AliOSS::HttpRequestForAliOSS*> listRequest;

				// Upload.
				for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				{
					GAIA::CTN::AString strName = "Test/helloworld";
					strName += x;
					strName += ".txt";
					
					GAIA::CTN::AString strContent = "hellokitty";
					strContent += x;
					
					GAIA::NETWORK::AliOSS::HttpRequestForAliOSS* pRequest = gnew GAIA::NETWORK::AliOSS::HttpRequestForAliOSS(http);
					listRequest.push_back(pRequest);
					
					oss.UploadData("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", strName.fptr(), strContent.fptr(), strContent.size(), aoa, pRequest);
				}
				for(GAIA::NUM x = 0; x < listRequest.size(); ++x)
				{
					GAIA::NETWORK::AliOSS::HttpRequestForAliOSS* pRequest = listRequest[x];
					pRequest->Wait();
					pRequest->drop_ref();
					pRequest = GNIL;
				}
				listRequest.clear();
				
				// Download.
				for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				{
					GAIA::CTN::AString strName = "Test/helloworld";
					strName += x;
					strName += ".txt";
					
					GAIA::NETWORK::AliOSS::HttpRequestForAliOSS* pRequest = gnew GAIA::NETWORK::AliOSS::HttpRequestForAliOSS(http);
					listRequest.push_back(pRequest);
					
					GAIA::CTN::Buffer buf;
					oss.DownloadData("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", strName.fptr(), buf, aoa, pRequest);
				}
				for(GAIA::NUM x = 0; x < listRequest.size(); ++x)
				{
					GAIA::NETWORK::AliOSS::HttpRequestForAliOSS* pRequest = listRequest[x];
					pRequest->Wait();
					
					GAIA::CTN::AString strContent = "hellokitty";
					strContent += x;
					
					GAIA::CTN::AChars chsData;
					chsData.assign(pRequest->bufDownload.fptr(), pRequest->bufDownload.write_size());
					if(chsData != strContent.fptr())
						TERROR;
					pRequest->drop_ref();
					pRequest = GNIL;
				}
				listRequest.clear();
				
				// Delete.
				for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				{
					GAIA::CTN::AString strName = "Test/helloworld";
					strName += x;
					strName += ".txt";
					
					GAIA::NETWORK::AliOSS::HttpRequestForAliOSS* pRequest = gnew GAIA::NETWORK::AliOSS::HttpRequestForAliOSS(http);
					listRequest.push_back(pRequest);
					
					oss.Delete("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", strName.fptr(), aoa, pRequest);
				}
				for(GAIA::NUM x = 0; x < listRequest.size(); ++x)
				{
					GAIA::NETWORK::AliOSS::HttpRequestForAliOSS* pRequest = listRequest[x];
					pRequest->Wait();
					pRequest->drop_ref();
					pRequest = GNIL;
				}
				listRequest.clear();
			}
		}
		http.End();
		
		http.Destroy();
		
		// Delete local file.
		GAIA::FSYS::Dir dir;
		dir.RemoveFile("./helloworld.txt");
		dir.RemoveFile("./hellokitty.txt");
	}
}
