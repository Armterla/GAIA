#ifndef		__GAIA_JSON_JSONFACTORY_IMPL_H__
#define		__GAIA_JSON_JSONFACTORY_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_autolockr.h"
#include "gaia_sync_autolockw.h"
#include "gaia_json_jsonbase.h"
#include "gaia_json_jsonnode.h"
#include "gaia_json_json.h"
#include "gaia_json_jsonfactory.h"

namespace GAIA
{
	namespace JSON
	{
		GINL JsonFactory::JsonFactory()
		{
			m_desc.reset();
			m_bCreated = GAIA::False;
		}
		GINL JsonFactory::~JsonFactory()
		{
			if(this->IsCreated())
				this->Destroy();
		}
		GINL GAIA::BL JsonFactory::Create(const JsonFactoryDesc& desc)
		{
			GAST(!this->IsCreated());
			if(this->IsCreated())
				return GAIA::False;
			GAST(desc.check());
			if(!desc.check())
				return GAIA::False;
			m_desc = desc;
			m_bCreated = GAIA::True;
			return GAIA::True;
		}
		GINL GAIA::BL JsonFactory::Destroy()
		{
			GAST(this->IsCreated());
			if(!this->IsCreated())
				return GAIA::False;
			m_desc.reset();
			m_strpool.destroy();
			m_sstrpool.destroy();
			m_NodePool.destroy();
			m_JsonPool.destroy();
			m_bCreated = GAIA::False;
			return GAIA::True;
		}
		GINL GAIA::BL JsonFactory::IsCreated() const
		{
			return m_bCreated;
		}
		GINL const JsonFactoryDesc& JsonFactory::GetDesc() const
		{
			GAST(this->IsCreated());
			return m_desc;
		}
		GINL JsonNode& JsonFactory::CreateNode()
		{
			GAST(this->IsCreated());
			JsonNode* pNode;
			if(m_desc.bObjectPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwNodePool);
					pNode = m_NodePool.alloc();
				}
				else
					pNode = m_NodePool.alloc();
			}
			else
				pNode = gnew JsonNode;
			pNode->SetFactory(this);
			return *pNode;
		}
		GINL Json& JsonFactory::CreateJson()
		{
			GAST(this->IsCreated());
			Json* pJson;
			if(m_desc.bObjectPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwJsonPool);
					pJson = m_JsonPool.alloc();
				}
				else
					pJson = m_JsonPool.alloc();
			}
			else
				pJson = gnew Json;
			pJson->SetFactory(this);
			return *pJson;
		}
		GINL GAIA::BL JsonFactory::ReleaseNode(JsonNode& node)
		{
			GAST(node.GetFactory() == this);
			if(node.GetFactory() != this)
				return GAIA::False;
			GAST(this->IsCreated());
			if(!this->IsCreated())
				return GAIA::False;
			node.Reset();
			if(m_desc.bObjectPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwNodePool);
					m_NodePool.release(&node);
				}
				else
					m_NodePool.release(&node);
			}
			else
				gdel &node;
			return GAIA::True;
		}
		GINL GAIA::BL JsonFactory::ReleaseJson(Json& json)
		{
			GAST(json.GetFactory() == this);
			if(json.GetFactory() != this)
				return GAIA::False;
			GAST(this->IsCreated());
			if(!this->IsCreated())
				return GAIA::False;
			json.Reset();
			if(m_desc.bObjectPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwJsonPool);
					m_JsonPool.release(&json);
				}
				else
					m_JsonPool.release(&json);
			}
			else
				gdel &json;
			return GAIA::True;
		}
		GINL const GAIA::TCH* JsonFactory::AllocString(const GAIA::TCH* psz)
		{
			GAST(psz != GNIL);
			GAST(this->IsCreated());
			if(m_desc.bStringPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwStringPool);
					return m_strpool.alloc(psz);
				}
				else
					return m_strpool.alloc(psz);
			}
			else
				return GAIA::ALGO::gstrnew(psz);
		}
		GINL GAIA::BL JsonFactory::ReleaseString(const GAIA::TCH* psz)
		{
			GAST(psz != GNIL);
			GAST(this->IsCreated());
			if(m_desc.bStringPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwStringPool);
					return m_strpool.release(psz);
				}
				else
					return m_strpool.release(psz);
			}
			else
				gdel[] psz;
			return GAIA::True;
		}
		GINL const GAIA::TCH* JsonFactory::AllocStaticString(const GAIA::TCH* psz)
		{
			GAST(psz != GNIL);
			GAST(this->IsCreated());
			if(m_desc.bStaticStringPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwStaticStringPool);
					return m_sstrpool.alloc(psz);
				}
				else
					return m_sstrpool.alloc(psz);
			}
			else
				return GAIA::ALGO::gstrnew(psz);
		}
		GINL GAIA::BL JsonFactory::ReleaseStaticString(const GAIA::TCH* psz)
		{
			GAST(psz != GNIL);
			GAST(this->IsCreated());
			if(!m_desc.bStaticStringPoolEnable)
				gdel[] psz;
			return GAIA::True;
		}
	}
}

#endif
