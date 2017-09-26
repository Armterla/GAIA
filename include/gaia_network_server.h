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
			class ProtocolDesc;
			class CallBack;
			class Link;
			class Protocol;

		public:
			GAIA_ENUM_BEGIN(PROTOCOL_TYPE)
				PROTOCOL_TYPE_TCP,
				PROTOCOL_TYPE_UDP,
				PROTOCOL_TYPE_SUDP,
				PROTOCOL_TYPE_HTTP,
			GAIA_ENUM_END(PROTOCOL_TYPE)

			class ServerDesc : public GAIA::Base
			{
			public:
			};

			class ProtocolDesc : public GAIA::Base
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

			class Protocol : public GAIA::Base
			{
			public:
				Protocol();
				~Protocol();

				GAIA::BL Create(const ProtocolDesc& desc);
				GAIA::BL Destroy();
				GAIA::BL IsCreated() const;
				const ProtocolDesc& GetDesc() const;

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

			GAIA::BL AddProtocol(Protocol& proto);
			GAIA::BL RemoveProtocol(Protocol& proto);
			GAIA::BL RemoveProtocolAll();
			GAIA::BL IsExistProtocol(Protocol& proto);
			GAIA::NUM GetProtocolCount() const;
			Protocol* GetProtocol(GAIA::NUM sIndex) const;
		};
	}
}

#endif
