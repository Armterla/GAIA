#ifndef		__GAIA_XML_XMLNODE_H__
#define		__GAIA_XML_XMLNODE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_vector.h"
#include "gaia_xml_xmlbase.h"

namespace GAIA
{
	namespace XML
	{
		class XMLNode : public XMLFactoryHolder
		{
			friend class XML;

		public:
			GINL XMLNode();
			GINL ~XMLNode();

			GINL GAIA::GVOID Reset();

			GINL GAIA::BL IsRoot() const;

			GINL GAIA::GVOID SetType(GAIA::XML::XML_NODE nt);
			GINL GAIA::XML::XML_NODE GetType() const;
			GINL GAIA::GVOID SetName(const GAIA::TCH* pszName);
			GINL const GAIA::TCH* GetName() const;

			GINL GAIA::NUM InsertChild(XMLNode& node, GAIA::NUM sIndex = GINVALID);
			GINL GAIA::BL DeleteChild(const GAIA::NUM& sIndex);
			GINL GAIA::GVOID DeleteChildAll();
			GINL GAIA::NUM GetChildCount() const;
			GINL XMLNode* GetChild(const GAIA::NUM& nIndex) const;
			GINL GAIA::NUM SetParent(XMLNode* pParent);
			GINL XMLNode* GetParent() const;

			GINL GAIA::NUM GetDepth() const;
			GINL GAIA::NUM GetParentIndex() const;

		private:
			GINL GAIA::GVOID SetRoot(GAIA::BL bRoot);
			GINL GAIA::GVOID SetParentIndex(GAIA::NUM sIndex);

		private:
			GAIA::XML::XML_NODE m_nt;
			const GAIA::TCH* m_pszName;
			GAIA::BL m_bRoot;
			GAIA::XML::XMLNode* m_pParent;
			GAIA::NUM m_sParentIndex;
			GAIA::CTN::Vector<GAIA::XML::XMLNode*> m_childs;
			GAIA::CTN::Vector<GAIA::NUM> m_freestack;
		};
	}
}

#endif
