#ifndef		__GAIA_XML_XMLNODE_IMPL_H__
#define		__GAIA_XML_XMLNODE_IMPL_H__

#include "gaia_xml_xmlbase.h"
#include "gaia_xml_xmlnode.h"
#include "gaia_xml_xmlfactory.h"

namespace GAIA
{
	namespace XML
	{
		GINL XmlNode::XmlNode()
		{
			m_nt = GAIA::XML::XML_NODE_MULTICONTAINER;
			m_pszName = GNIL;
			m_bRoot = GAIA::False;
			m_pParent = GNIL;
			m_sParentIndex = GINVALID;
		}
		GINL XmlNode::~XmlNode()
		{
			this->Reset();
		}
		GINL GAIA::GVOID XmlNode::Reset()
		{
			this->SetType(GAIA::XML::XML_NODE_INVALID);
			this->SetName(GNIL);
			this->DeleteChildAll();
			this->SetParent(GNIL);
		}
		GINL GAIA::BL XmlNode::IsRoot() const
		{
			return m_bRoot;
		}
		GINL GAIA::GVOID XmlNode::SetType(GAIA::XML::XML_NODE nt)
		{
			if(m_bRoot)
			{
				GAST(nt == GAIA::XML::XML_NODE_INVALID ||
					 nt == GAIA::XML::XML_NODE_CONTAINER ||
					 nt == GAIA::XML::XML_NODE_MULTICONTAINER);
				if(nt != GAIA::XML::XML_NODE_INVALID &&
						nt != GAIA::XML::XML_NODE_CONTAINER &&
						nt != GAIA::XML::XML_NODE_MULTICONTAINER)
					return;
			}
			m_nt = nt;
		}
		GINL GAIA::XML::XML_NODE XmlNode::GetType() const
		{
			return m_nt;
		}
		GINL GAIA::GVOID XmlNode::SetName(const GAIA::TCH* pszName)
		{
			GAIA::XML::XmlFactory* pFactory = this->GetFactory();
			if(GAIA::ALGO::gstrcmpnil(m_pszName, pszName) == 0)
				return;
			if(pszName == GNIL)
			{
				if(m_pszName != GNIL)
				{
					if(pFactory == GNIL)
						gdel[] m_pszName;
					m_pszName = GNIL;
				}
				return;
			}
			if(pFactory == GNIL)
				m_pszName = GAIA::ALGO::gstrnew(pszName);
			else
				m_pszName = pFactory->AllocStaticString(pszName);
		}
		GINL const GAIA::TCH* XmlNode::GetName() const
		{
			return m_pszName;
		}
		GINL GAIA::NUM XmlNode::InsertChild(XmlNode& node, GAIA::NUM sIndex)
		{
			if(m_nt == GAIA::XML::XML_NODE_VALUE)
				return GINVALID;
			if(m_nt == GAIA::XML::XML_NODE_COMMENT)
				return GINVALID;
			if(node.IsRoot())
				return GINVALID;
			if(m_nt == GAIA::XML::XML_NODE_NAME &&
					node.m_nt != GAIA::XML::XML_NODE_VALUE)
				return GINVALID;
			if(node.m_nt == GAIA::XML::XML_NODE_VALUE &&
					m_nt != GAIA::XML::XML_NODE_NAME &&
					m_nt != GAIA::XML::XML_NODE_MULTICONTAINER)
				return GINVALID;
			if(node.m_pParent != GNIL)
				node.SetParent(GNIL);
			else
			{
				if(node.IsRoot())
					return GINVALID;
			}
			if(sIndex == GINVALID)
			{
				if(!m_freestack.empty())
				{
					sIndex = m_freestack.back();
					m_freestack.pop_back();
				}
				else
					sIndex = m_childs.size();
			}
			else if(sIndex > m_childs.size())
				sIndex = m_childs.size();
			m_childs.insert(&node, sIndex);
			node.SetParentIndex(sIndex);
			node.m_pParent = this;
			if(node.m_nt == GAIA::XML::XML_NODE_CONTAINER ||
					node.m_nt == GAIA::XML::XML_NODE_MULTICONTAINER)
			{
				if(m_nt == GAIA::XML::XML_NODE_CONTAINER)
					m_nt = GAIA::XML::XML_NODE_MULTICONTAINER;
			}
			return node.GetParentIndex();
		}
		GINL GAIA::BL XmlNode::DeleteChild(const GAIA::NUM& sIndex)
		{
			if(sIndex >= m_childs.size())
				return GAIA::False;
			GAIA::XML::XmlNode* pChild = m_childs[sIndex];
			if(pChild == GNIL)
				return GAIA::False;
			if(pChild->GetFactory() != GNIL)
				pChild->GetFactory()->ReleaseNode(*pChild);
			else
				gdel pChild;
			m_childs[sIndex] = GNIL;
			return GAIA::True;
		}
		GINL GAIA::GVOID XmlNode::DeleteChildAll()
		{
			for(GAIA::NUM x = 0; x < m_childs.size(); ++x)
			{
				GAIA::XML::XmlNode* pChild = m_childs[x];
				if(pChild == GNIL)
					continue;
				if(pChild->GetFactory() != GNIL)
					pChild->GetFactory()->ReleaseNode(*pChild);
				else
					gdel pChild;
			}
			m_childs.clear();
			m_freestack.clear();
		}
		GINL GAIA::NUM XmlNode::GetChildCount() const
		{
			return m_childs.size();
		}
		GINL XmlNode* XmlNode::GetChild(const GAIA::NUM& nIndex) const
		{
			return m_childs[nIndex];
		}
		GINL GAIA::NUM XmlNode::SetParent(XmlNode* pParent)
		{
			if(m_pParent == pParent)
				return this->GetParentIndex();
			if(m_pParent != GNIL)
			{
				GAST(m_sParentIndex != GINVALID);
				m_pParent->m_freestack.push_back(m_sParentIndex);
				m_pParent->m_childs[m_sParentIndex] = GNIL;
				m_pParent = GNIL;
				m_sParentIndex = GINVALID;
			}
			if(pParent != GNIL)
			{
				if(this->IsRoot())
					return GINVALID;
				pParent->InsertChild(*this);
				return m_sParentIndex;
			}
			return GINVALID;
		}
		GINL XmlNode* XmlNode::GetParent() const
		{
			return m_pParent;
		}
		GINL GAIA::NUM XmlNode::GetDepth() const
		{
			GAIA::NUM sDepth = 0;
			const XmlNode* pNode = this;
			while(pNode->m_pParent != GNIL)
			{
				sDepth++;
				pNode = pNode->m_pParent;
			}
			return sDepth;
		}
		GINL GAIA::NUM XmlNode::GetParentIndex() const
		{
			return m_sParentIndex;
		}
		GINL GAIA::GVOID XmlNode::SetRoot(GAIA::BL bRoot)
		{
			m_bRoot = bRoot;
		}
		GINL GAIA::GVOID XmlNode::SetParentIndex(GAIA::NUM sIndex)
		{
			m_sParentIndex = sIndex;
		}
	}
}

#endif
