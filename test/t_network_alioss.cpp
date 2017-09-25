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
			GAIA::NETWORK::AliOSSAccess aoa;
			aoa.chsPublicKey = "LTAIWwT2NHTU6nQt";
			aoa.chsPrivateKey = "c4AdBTE7E2kcinmFsCO7zVw106IkxD";
			
			GAIA::NETWORK::AliOSS oss(http);
			oss.UploadData("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Product/SecretChat/helloworld.txt", "0123456789", 10, aoa);
			
			GAIA::CTN::Buffer buf;
			oss.DownloadData("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Product/SecretChat/helloworld.txt", buf, aoa);
			
			if(buf.write_size() != 10)
				TERROR;
			GAIA::CTN::AChars chsData;
			chsData.assign(buf.fptr(), buf.write_size());
			if(chsData != "0123456789")
				TERROR;
			
			oss.Delete("oss-cn-hangzhou.aliyuncs.com", "com-t2j-pub", "Product/SecretChat/helloworld.txt", aoa);
		}
		http.End();
		
		http.Destroy();
	}
}
