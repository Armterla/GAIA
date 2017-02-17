#ifndef		__GAIA_NETWORK_SERVER_H__
#define		__GAIA_NETWORK_SERVER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"

namespace GAIA
{
	namespace NETWORK
	{
		class Server : public GAIA::Base
		{
		public:
			class ServerDesc;
			class ProtocalDesc;
			class CallBack;
			class Link;
			class Protocal;

		public:
			GAIA_ENUM_BEGIN(PROTOCAL_TYPE)
				PROTOCAL_TYPE_TCP,
				PROTOCAL_TYPE_UDP,
				PROTOCAL_TYPE_SUDP,
				PROTOCAL_TYPE_HTTP,
			GAIA_ENUM_END(PROTOCAL_TYPE)

			class ServerDesc : public GAIA::Base
			{
			public:
			};

			class ProtocalDesc : public GAIA::Base
			{
			public:
				const GAIA::CH* pszName;
			};

			class CallBack : public GAIA::Entity
			{
			public:
				CallBack();
				~CallBack();

			protected:
				virtual GAIA::GVOID OnRecv(GAIA::NETWORK::Server::Link& l){};
			};

			class Link : public GAIA::Base
			{
			public:
				Link();
				~Link();
			};

			class Protocal : public GAIA::Base
			{
			public:
				Protocal();
				~Protocal();

				GAIA::BL Create(const ProtocalDesc& desc);
				GAIA::BL Destroy();
				GAIA::BL IsCreated() const;
				const ProtocalDesc& GetDesc() const;

				GAIA::BL AddServiceAddr(const GAIA::NETWORK::Addr& addr);
				GAIA::BL RemoveServiceAddr(const GAIA::NETWORK::Addr& addr);
				GAIA::BL RemoveServiceAddrAll() const;
				GAIA::BL IsExistServiceAddr(const GAIA::NETWORK::Addr& addr);
				GAIA::NUM GetServiceAddrCount() const;
				const GAIA::NETWORK::Addr* GetServiceAddr(GAIA::NUM sIndex);

				GAIA::BL AddCallBack(CallBack& cb);
				GAIA::BL RemoveCallBack(CallBack& cb);
				GAIA::BL RemoveCallBackAll();
				GAIA::BL IsExistCallBack(CallBack& cb) const;
				GAIA::NUM GetCallBackCount() const;
				CallBack* GetCallBack(GAIA::NUM sIndex) const;

			public:

			};

		public:
			Server();
			~Server();

			GAIA::BL Create(const ServerDesc& desc);
			GAIA::BL Destroy();
			GAIA::BL IsCreated() const;
			const ServerDesc& GetDesc() const;

			GAIA::BL Startup();
			GAIA::BL Shutdown();
			GAIA::BL IsStartuped() const;

			GAIA::BL AddProtocal(Protocal& proto);
			GAIA::BL RemoveProtocal(Protocal& proto);
			GAIA::BL RemoveProtocalAll();
			GAIA::BL IsExistProtocal(Protocal& proto);
			GAIA::NUM GetProtocalCount() const;
			Protocal* GetProtocal(GAIA::NUM sIndex) const;
		};
	}
}

#endif
