#ifndef		__GAIA_HTML_HTMLNODE_H__
#define		__GAIA_HTML_HTMLNODE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_vector.h"
#include "gaia_html_htmlbase.h"

namespace GAIA
{
	namespace HTML
	{
		class HtmlNode : public HtmlFactoryHolder
		{
			friend class Html;

		public:
			GINL HtmlNode();
			GINL ~HtmlNode();

			GINL GAIA::GVOID Reset();

			GINL GAIA::BL IsRoot() const;

			GINL GAIA::GVOID SetType(GAIA::HTML::HTML_NODE nt);
			GINL GAIA::HTML::HTML_NODE GetType() const;
			GINL GAIA::GVOID SetName(const GAIA::TCH* pszName);
			GINL const GAIA::TCH* GetName() const;

			GINL GAIA::NUM InsertChild(HtmlNode& node, GAIA::NUM sIndex = GINVALID);
			GINL GAIA::BL DeleteChild(const GAIA::NUM& sIndex);
			GINL GAIA::GVOID DeleteChildAll();
			GINL GAIA::NUM GetChildCount() const;
			GINL HtmlNode* GetChild(const GAIA::NUM& nIndex) const;
			GINL GAIA::NUM SetParent(HtmlNode* pParent);
			GINL HtmlNode* GetParent() const;

			GINL GAIA::NUM GetDepth() const;
			GINL GAIA::NUM GetParentIndex() const;

		private:
			GINL GAIA::GVOID SetRoot(GAIA::BL bRoot);
			GINL GAIA::GVOID SetParentIndex(GAIA::NUM sIndex);

		private:
			GAIA::HTML::HTML_NODE m_nt;
			const GAIA::TCH* m_pszName;
			GAIA::BL m_bRoot;
			GAIA::HTML::HtmlNode* m_pParent;
			GAIA::NUM m_sParentIndex;
			GAIA::CTN::Vector<GAIA::HTML::HtmlNode*> m_childs;
			GAIA::CTN::Vector<GAIA::NUM> m_freestack;
		};
	}
}

#endif
