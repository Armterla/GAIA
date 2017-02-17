#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_network_base(GAIA::LOG::Log& logobj)
	{
		GAIA::NETWORK::Addr addr;
		addr.fromstring("127.0.0.1:1234");
		char szTemp[1024];
		addr.tostring(szTemp);
		if(GAIA::ALGO::gstrcmp(szTemp, "127.0.0.1:1234") != 0)
			TERROR;
	}
}
