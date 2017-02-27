#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_httpserver.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		HttpServerLink::HttpServerLink(GAIA::NETWORK::HttpServer& svr)
		{
			this->init();
		}

		HttpServerLink::~HttpServerLink()
		{

		}

		GAIA::GVOID HttpServerLink::Response(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize, const GAIA::U64& uCacheTime)
		{

		}

		GAIA::GVOID HttpServerLink::Close()
		{

		}

		HttpServerCallBack::HttpServerCallBack(HttpServer& svr)
		{
			this->init();
		}

		HttpServerCallBack::~HttpServerCallBack()
		{

		}

		HttpServer::HttpServer()
		{
			this->init();
		}

		HttpServer::~HttpServer()
		{
			if(this->IsCreated())
				this->Destroy();
		}

		GAIA::BL HttpServer::RegistCallBack(GAIA::NETWORK::HttpServerCallBack& cb)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::SYNC::AutolockW al(m_rwCBS);
			if(this->IsRegistedCallBack(cb))
				return GAIA::False;
			m_cbs.push_back(&cb);
			cb.rise_ref();
			return GAIA::True;
		}

		GAIA::BL HttpServer::UnregistCallBack(GAIA::NETWORK::HttpServerCallBack& cb)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::SYNC::AutolockW al(m_rwCBS);
			for(GAIA::NUM x = 0; x < m_cbs.size(); ++x)
			{
				if(m_cbs[x] == &cb)
				{
					m_cbs.erase(x);
					cb.drop_ref();
					return GAIA::True;
				}
			}
			return GAIA::False;
		}

		GAIA::BL HttpServer::UnregistCallBackAll()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::SYNC::AutolockW al(m_rwCBS);
			for(GAIA::NUM x = 0; x < m_cbs.size(); ++x)
			{
				GAIA::NETWORK::HttpServerCallBack* pCB = m_cbs[x];
				pCB->drop_ref();
			}
			m_cbs.clear();
		}

		GAIA::BL HttpServer::IsRegistedCallBack(GAIA::NETWORK::HttpServerCallBack& cb)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::SYNC::AutolockR al(m_rwCBS);
			for(GAIA::NUM x = 0; x < m_cbs.size(); ++x)
			{
				if(&cb == m_cbs[x])
					return GAIA::True;
			}
			return GAIA::False;
		}

		GAIA::BL HttpServer::Create(const GAIA::NETWORK::HttpServerDesc& desc)
		{
			if(this->IsCreated())
				return GAIA::False;

			// Create async dispatcher.

			m_bCreated = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL HttpServer::Destroy()
		{
			if(!this->IsCreated())
				return GAIA::False;

			//
			if(this->IsStartuped())
				this->Shutdown();

			// Destroy async dispatcher.

			//
			this->UnregistCallBackAll();

			m_bCreated = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL HttpServer::Startup()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			if(this->IsStartuped())
				return GAIA::False;

			// Startup async dispatcher.

			m_bStartuped = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL HttpServer::Shutdown()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			if(!this->IsStartuped())
				return GAIA::False;

			// Shutdown async dispatcher.

			m_bStartuped = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL HttpServer::OpenAddr(const GAIA::NETWORK::Addr& addr)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GPCHR_FALSE_RET(addr.check(), GAIA::False);

			//
		}

		GAIA::BL HttpServer::CloseAddr(const GAIA::NETWORK::Addr& addr)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GPCHR_FALSE_RET(addr.check(), GAIA::False);
		}

		GAIA::BL HttpServer::CloseAddrAll()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
		}

		GAIA::BL HttpServer::IsOpennedAddr(const GAIA::NETWORK::Addr& addr) const
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
		}

		GAIA::NUM HttpServer::GetOpennedAddrCount() const
		{
			GPCHR_FALSE_RET(this->IsCreated(), 0);
		}

		const GAIA::NETWORK::Addr* HttpServer::GetOpennedAddr(GAIA::NUM sIndex) const
		{
			GPCHR_FALSE_RET(this->IsCreated(), GNIL);
		}

		GAIA::GVOID HttpServer::EnableDynamicResponseCache(GAIA::BL bEnable)
		{
			m_bEnableDynamicResponseCache = bEnable;
		}

		GAIA::BL HttpServer::IsEnableDynamicResponseCache() const
		{
			return m_bEnableDynamicResponseCache;
		}

		GAIA::GVOID HttpServer::EnableStaticResponseCache(GAIA::BL bEnable)
		{
			m_bEnableStaticResponseCache = bEnable;
		}

		GAIA::BL HttpServer::IsEnableStaticResponseCache() const
		{
			return m_bEnableStaticResponseCache;
		}

		GAIA::GVOID HttpServer::SetBlackWhiteMode(GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE mode)
		{
			m_blackwhitemode = mode;
		}

		GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE HttpServer::GetBlackWhiteMode() const
		{
			return m_blackwhitemode;
		}

		GAIA::GVOID HttpServer::AddBlackList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uTime)
		{
			GAIA::SYNC::AutolockW al(m_rwBlackList);
		}

		GAIA::GVOID HttpServer::RemoveBlackList(const GAIA::NETWORK::IP& ip)
		{
			GAIA::SYNC::AutolockW al(m_rwBlackList);
		}

		GAIA::GVOID HttpServer::RemoveBlackListAll()
		{
			GAIA::SYNC::AutolockW al(m_rwBlackList);
		}

		GAIA::GVOID HttpServer::AddWhiteList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uTime)
		{
			GAIA::SYNC::AutolockW al(m_rwWhiteList);
		}

		GAIA::GVOID HttpServer::RemoveWhiteList(const GAIA::NETWORK::IP& ip)
		{
			GAIA::SYNC::AutolockW al(m_rwWhiteList);
		}

		GAIA::GVOID HttpServer::RemoveWhiteListAll()
		{
			GAIA::SYNC::AutolockW al(m_rwWhiteList);
		}

		GAIA::GVOID HttpServer::AddDynamicCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize)
		{
			GAIA::SYNC::AutolockW al(m_rwDynamicCache);
		}

		GAIA::GVOID HttpServer::RemoveDynamicCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead)
		{
			GAIA::SYNC::AutolockW al(m_rwDynamicCache);
		}

		GAIA::GVOID HttpServer::RemoveDynamicCacheAll()
		{
			GAIA::SYNC::AutolockW al(m_rwDynamicCache);
		}

		GAIA::GVOID HttpServer::RequestStaticCache(const GAIA::NETWORK::HttpURL& url)
		{
			GAIA::SYNC::AutolockW al(m_rwStaticCache);
		}

		GAIA::GVOID HttpServer::RemoveStaticCache(const GAIA::NETWORK::HttpURL& url)
		{
			GAIA::SYNC::AutolockW al(m_rwStaticCache);
		}

		GAIA::GVOID HttpServer::RemoveStaticCacheAll()
		{
			GAIA::SYNC::AutolockW al(m_rwStaticCache);
		}
	}
}