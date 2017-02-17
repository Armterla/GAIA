#ifndef		__GAIA_HTML_HTMLFACTORY_IMPL_H__
#define		__GAIA_HTML_HTMLFACTORY_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_autolockr.h"
#include "gaia_sync_autolockw.h"
#include "gaia_html_htmlbase.h"
#include "gaia_html_htmlnode.h"
#include "gaia_html_html.h"
#include "gaia_html_htmlfactory.h"

namespace GAIA
{
	namespace HTML
	{
		GINL HTMLFactory::HTMLFactory()
		{
			m_desc.reset();
			m_bCreated = GAIA::False;
		}
		GINL HTMLFactory::~HTMLFactory()
		{
			if(this->IsCreated())
				this->Destroy();
		}
		GINL GAIA::BL HTMLFactory::Create(const HTMLFactoryDesc& desc)
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
		GINL GAIA::BL HTMLFactory::Destroy()
		{
			GAST(this->IsCreated());
			if(!this->IsCreated())
				return GAIA::False;
			m_desc.reset();
			m_strpool.destroy();
			m_sstrpool.destroy();
			m_NodePool.destroy();
			m_HTMLPool.destroy();
			m_bCreated = GAIA::False;
			return GAIA::True;
		}
		GINL GAIA::BL HTMLFactory::IsCreated() const
		{
			return m_bCreated;
		}
		GINL const HTMLFactoryDesc& HTMLFactory::GetDesc() const
		{
			GAST(this->IsCreated());
			return m_desc;
		}
		GINL HTMLNode& HTMLFactory::CreateNode()
		{
			GAST(this->IsCreated());
			HTMLNode* pNode;
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
				pNode = gnew HTMLNode;
			pNode->SetFactory(this);
			return *pNode;
		}
		GINL HTML& HTMLFactory::CreateHTML()
		{
			GAST(this->IsCreated());
			HTML* pHTML;
			if(m_desc.bObjectPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwHTMLPool);
					pHTML = m_HTMLPool.alloc();
				}
				else
					pHTML = m_HTMLPool.alloc();
			}
			else
				pHTML = gnew HTML;
			pHTML->SetFactory(this);
			return *pHTML;
		}
		GINL GAIA::BL HTMLFactory::ReleaseNode(HTMLNode& node)
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
		GINL GAIA::BL HTMLFactory::ReleaseHTML(HTML& html)
		{
			GAST(html.GetFactory() == this);
			if(html.GetFactory() != this)
				return GAIA::False;
			GAST(this->IsCreated());
			if(!this->IsCreated())
				return GAIA::False;
			html.Reset();
			if(m_desc.bObjectPoolEnable)
			{
				if(m_desc.bThreadSafe)
				{
					GAIA::SYNC::AutolockW al(m_rwHTMLPool);
					m_HTMLPool.release(&html);
				}
				else
					m_HTMLPool.release(&html);
			}
			else
				gdel &html;
			return GAIA::True;
		}
		GINL const GAIA::TCH* HTMLFactory::AllocString(const GAIA::TCH* psz)
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
		GINL GAIA::BL HTMLFactory::ReleaseString(const GAIA::TCH* psz)
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
		GINL const GAIA::TCH* HTMLFactory::AllocStaticString(const GAIA::TCH* psz)
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
		GINL GAIA::BL HTMLFactory::ReleaseStaticString(const GAIA::TCH* psz)
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
