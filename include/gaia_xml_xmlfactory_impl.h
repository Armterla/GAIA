#ifndef		__GAIA_XML_XMLFACTORY_IMPL_H__
#define		__GAIA_XML_XMLFACTORY_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_autolockr.h"
#include "gaia_sync_autolockw.h"
#include "gaia_xml_xmlbase.h"
#include "gaia_xml_xmlnode.h"
#include "gaia_xml_xml.h"
#include "gaia_xml_xmlfactory.h"

namespace GAIA
{
	namespace XML
	{
		GINL XMLFactory::XMLFactory()
		{
			m_desc.reset();
			m_bCreated = GAIA::False;
		}
		GINL XMLFactory::~XMLFactory()
		{
			if(this->IsCreated())
				this->Destroy();
		}
		GINL GAIA::BL XMLFactory::Create(const XMLFactoryDesc& desc)
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
		GINL GAIA::BL XMLFactory::Destroy()
		{
			GAST(this->IsCreated());
			if(!this->IsCreated())
				return GAIA::False;
			m_desc.reset();
			m_strpool.destroy();
			m_sstrpool.destroy();
			m_NodePool.destroy();
			m_XMLPool.destroy();
			m_bCreated = GAIA::False;
			return GAIA::True;
		}
		GINL GAIA::BL XMLFactory::IsCreated() const
		{
			return m_bCreated;
		}
		GINL const XMLFactoryDesc& XMLFactory::GetDesc() const
		{
			GAST(this->IsCreated());
			return m_desc;
		}
		GINL XMLNode& XMLFactory::CreateNode()
		{
			GAST(this->IsCreated());
			XMLNode* pNode;
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
				pNode = gnew XMLNode;
			pNode->SetFactory(this);
			return *pNode;
		}
		GINL XML& XMLFactory::CreateXML()
		{
			GAST(this->IsCreated());
			XML* pXML;
			if(m_desc.bObjectPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwXMLPool);
					pXML = m_XMLPool.alloc();
				}
				else
					pXML = m_XMLPool.alloc();
			}
			else
				pXML = gnew XML;
			pXML->SetFactory(this);
			return *pXML;
		}
		GINL GAIA::BL XMLFactory::ReleaseNode(XMLNode& node)
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
		GINL GAIA::BL XMLFactory::ReleaseXML(XML& xml)
		{
			GAST(xml.GetFactory() == this);
			if(xml.GetFactory() != this)
				return GAIA::False;
			GAST(this->IsCreated());
			if(!this->IsCreated())
				return GAIA::False;
			xml.Reset();
			if(m_desc.bObjectPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwXMLPool);
					m_XMLPool.release(&xml);
				}
				else
					m_XMLPool.release(&xml);
			}
			else
				gdel &xml;
			return GAIA::True;
		}
		GINL const GAIA::TCH* XMLFactory::AllocString(const GAIA::TCH* psz)
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
		GINL GAIA::BL XMLFactory::ReleaseString(const GAIA::TCH* psz)
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
		GINL const GAIA::TCH* XMLFactory::AllocStaticString(const GAIA::TCH* psz)
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
		GINL GAIA::BL XMLFactory::ReleaseStaticString(const GAIA::TCH* psz)
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
