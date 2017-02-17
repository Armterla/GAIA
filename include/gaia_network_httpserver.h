#ifndef	 __GAIA_NETWORK_HTTPSERVER_H__
#define	 __GAIA_NETWORK_HTTPSERVER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
//#include "gaia_sync_lock.h"
//#include "gaia_sync_autolock.h"
//#include "gaia_sync_lockrw.h"
//#include "gaia_sync_autolockr.h"
//#include "gaia_sync_autolockw.h"
//#include "gaia_ctn_ref.h"
//#include "gaia_ctn_vector.h"
//#include "gaia_ctn_queue.h"
//#include "gaia_ctn_buffer.h"
//#include "gaia_ctn_pool.h"
//#include "gaia_ctn_list.h"
//#include "gaia_ctn_set.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
//#include "gaia_network_base.h"
//#include "gaia_network_socket.h"
#include "gaia_network_asyncsocket.h"
#include "gaia_network_asyncdispatcher.h"
#include "gaia_network_httpbase.h"

namespace GAIA
{
	namespace NETWORK
	{
		class HttpServerDesc : public GAIA::Base
		{
		public:
		};

		class HttpServerCallBack : public GAIA::Base
		{
		public:

		protected:
			virtual GAIA::BL OnRecv(GAIA::NETWORK::AsyncSocket& sock, const GAIA::GVOID* p, GAIA::NUM sSize){return GAIA::False;}
			virtual GAIA::BL OnRecv(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead){return GAIA::False;}
		};

		class HttpServer : public GAIA::Base
		{
		public:
			HttpServer();
			~HttpServer();

			GAIA::BL Create(const HttpServerDesc& desc);
			GAIA::BL Destroy();
			GAIA::BL IsCreated() const;
			const HttpServerDesc& GetDesc() const;

			GAIA::BL Startup();
			GAIA::BL Shutdown();
			GAIA::BL IsStartuped() const;

			GAIA::BL OpenAddr(const GAIA::NETWORK::Addr& addr);
			GAIA::BL CloseAddr(const GAIA::NETWORK::Addr& addr);
			GAIA::BL CloseAddrAll();
			GAIA::BL IsOpennedAddr() const;
			GAIA::NUM GetOpennedAddrCount() const;
			const GAIA::NETWORK::Addr* GetOpennedAddr(GAIA::NUM sIndex) const;

		private:
		};
	}
}

#endif
