﻿#ifndef		__GAIA_HTML_HTMLNODE_IMPL_H__
#define		__GAIA_HTML_HTMLNODE_IMPL_H__

#include "gaia_html_htmlnode.h"
#include "gaia_html_htmlfactory.h"

namespace GAIA
{
	namespace HTML
	{
		GINL HTMLNode::HTMLNode()
		{
			m_nt = GAIA::HTML::HTML_NODE_MULTICONTAINER;
			m_pszName = GNIL;
			m_bRoot = GAIA::False;
			m_pParent = GNIL;
			m_sParentIndex = GINVALID;
		}
		GINL HTMLNode::~HTMLNode()
		{
			this->Reset();
		}
		GINL GAIA::GVOID HTMLNode::Reset()
		{
			this->SetType(GAIA::HTML::HTML_NODE_INVALID);
			this->SetName(GNIL);
			this->DeleteChildAll();
			this->SetParent(GNIL);
		}
		GINL GAIA::BL HTMLNode::IsRoot() const
		{
			return m_bRoot;
		}
		GINL GAIA::GVOID HTMLNode::SetType(GAIA::HTML::HTML_NODE nt)
		{
			if(m_bRoot)
			{
				GAST(nt == GAIA::HTML::HTML_NODE_INVALID ||
					 nt == GAIA::HTML::HTML_NODE_CONTAINER ||
					 nt == GAIA::HTML::HTML_NODE_MULTICONTAINER);
				if(nt != GAIA::HTML::HTML_NODE_INVALID &&
						nt != GAIA::HTML::HTML_NODE_CONTAINER &&
						nt != GAIA::HTML::HTML_NODE_MULTICONTAINER)
					return;
			}
			m_nt = nt;
		}
		GINL GAIA::HTML::HTML_NODE HTMLNode::GetType() const
		{
			return m_nt;
		}
		GINL GAIA::GVOID HTMLNode::SetName(const GAIA::TCH* pszName)
		{
			GAIA::HTML::HTMLFactory* pFactory = this->GetFactory();
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
		GINL const GAIA::TCH* HTMLNode::GetName() const
		{
			return m_pszName;
		}
		GINL GAIA::NUM HTMLNode::InsertChild(HTMLNode& node, GAIA::NUM sIndex)
		{
			if(m_nt == GAIA::HTML::HTML_NODE_VALUE)
				return GINVALID;
			if(m_nt == GAIA::HTML::HTML_NODE_COMMENT)
				return GINVALID;
			if(node.IsRoot())
				return GINVALID;
			if(m_nt == GAIA::HTML::HTML_NODE_NAME &&
					node.m_nt != GAIA::HTML::HTML_NODE_VALUE)
				return GINVALID;
			if(node.m_nt == GAIA::HTML::HTML_NODE_VALUE &&
					m_nt != GAIA::HTML::HTML_NODE_NAME &&
					m_nt != GAIA::HTML::HTML_NODE_MULTICONTAINER)
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
			if(node.m_nt == GAIA::HTML::HTML_NODE_CONTAINER ||
					node.m_nt == GAIA::HTML::HTML_NODE_MULTICONTAINER)
			{
				if(m_nt == GAIA::HTML::HTML_NODE_CONTAINER)
					m_nt = GAIA::HTML::HTML_NODE_MULTICONTAINER;
			}
			return node.GetParentIndex();
		}
		GINL GAIA::BL HTMLNode::DeleteChild(const GAIA::NUM& sIndex)
		{
			if(sIndex >= m_childs.size())
				return GAIA::False;
			GAIA::HTML::HTMLNode* pChild = m_childs[sIndex];
			if(pChild == GNIL)
				return GAIA::False;
			if(pChild->GetFactory() != GNIL)
				pChild->GetFactory()->ReleaseNode(*pChild);
			else
				gdel pChild;
			m_childs[sIndex] = GNIL;
			return GAIA::True;
		}
		GINL GAIA::GVOID HTMLNode::DeleteChildAll()
		{
			for(GAIA::NUM x = 0; x < m_childs.size(); ++x)
			{
				GAIA::HTML::HTMLNode* pChild = m_childs[x];
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
		GINL GAIA::NUM HTMLNode::GetChildCount() const
		{
			return m_childs.size();
		}
		GINL HTMLNode* HTMLNode::GetChild(const GAIA::NUM& nIndex) const
		{
			return m_childs[nIndex];
		}
		GINL GAIA::NUM HTMLNode::SetParent(HTMLNode* pParent)
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
		GINL HTMLNode* HTMLNode::GetParent() const
		{
			return m_pParent;
		}
		GINL GAIA::NUM HTMLNode::GetDepth() const
		{
			GAIA::NUM sDepth = 0;
			const HTMLNode* pNode = this;
			while(pNode->m_pParent != GNIL)
			{
				sDepth++;
				pNode = pNode->m_pParent;
			}
			return sDepth;
		}
		GINL GAIA::NUM HTMLNode::GetParentIndex() const
		{
			return m_sParentIndex;
		}
		GINL GAIA::GVOID HTMLNode::SetRoot(GAIA::BL bRoot)
		{
			m_bRoot = bRoot;
		}
		GINL GAIA::GVOID HTMLNode::SetParentIndex(GAIA::NUM sIndex)
		{
			m_sParentIndex = sIndex;
		}
	}
}

#endif
