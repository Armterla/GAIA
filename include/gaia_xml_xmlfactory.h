#ifndef		__GAIA_XML_XMLFACTORY_H__
#define		__GAIA_XML_XMLFACTORY_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_stringptrpool.h"
#include "gaia_ctn_staticstringptrpool.h"
#include "gaia_sync_lockrw.h"
#include "gaia_xml_xmlfactorydesc.h"

namespace GAIA
{
	namespace XML
	{
		class XMLFactory : public GAIA::Base
		{
			friend class XML;
			friend class XMLNode;

		public:
			GINL XMLFactory();
			GINL ~XMLFactory();

			GINL GAIA::BL Create(const XMLFactoryDesc& desc);
			GINL GAIA::BL Destroy();
			GINL GAIA::BL IsCreated() const;
			GINL const XMLFactoryDesc& GetDesc() const;

			GINL XMLNode& CreateNode();
			GINL XML& CreateXML();

			GINL GAIA::BL ReleaseNode(XMLNode& node);
			GINL GAIA::BL ReleaseXML(XML& xml);

		private:
			GINL const GAIA::TCH* AllocString(const GAIA::TCH* psz);
			GINL GAIA::BL ReleaseString(const GAIA::TCH* psz);
			GINL const GAIA::TCH* AllocStaticString(const GAIA::TCH* psz);
			GINL GAIA::BL ReleaseStaticString(const GAIA::TCH* psz);

		private:
			XMLFactoryDesc m_desc;
			GAIA::BL m_bCreated;

			GAIA::SYNC::LockRW m_rwStringPool;
			GAIA::SYNC::LockRW m_rwStaticStringPool;

			GAIA::SYNC::LockRW m_rwNodePool;
			GAIA::SYNC::LockRW m_rwXMLPool;

			GAIA::CTN::StringPtrPool<GAIA::TCH> m_strpool;
			GAIA::CTN::StaticStringPtrPool<GAIA::TCH> m_sstrpool;

			GAIA::CTN::Pool<XMLNode> m_NodePool;
			GAIA::CTN::Pool<XML> m_XMLPool;
		};
	}
}

#endif
