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
		class XmlFactory : public GAIA::Base
		{
			friend class Xml;
			friend class XmlNode;

		public:
			GINL XmlFactory();
			GINL ~XmlFactory();

			GINL GAIA::BL Create(const XmlFactoryDesc& desc);
			GINL GAIA::BL Destroy();
			GINL GAIA::BL IsCreated() const;
			GINL const XmlFactoryDesc& GetDesc() const;

			GINL XmlNode& CreateNode();
			GINL Xml& CreateXml();

			GINL GAIA::BL ReleaseNode(XmlNode& node);
			GINL GAIA::BL ReleaseXml(Xml& xml);

		private:
			GINL const GAIA::TCH* AllocString(const GAIA::TCH* psz);
			GINL GAIA::BL ReleaseString(const GAIA::TCH* psz);
			GINL const GAIA::TCH* AllocStaticString(const GAIA::TCH* psz);
			GINL GAIA::BL ReleaseStaticString(const GAIA::TCH* psz);

		private:
			XmlFactoryDesc m_desc;
			GAIA::BL m_bCreated;

			GAIA::SYNC::LockRW m_rwStringPool;
			GAIA::SYNC::LockRW m_rwStaticStringPool;

			GAIA::SYNC::LockRW m_rwNodePool;
			GAIA::SYNC::LockRW m_rwXmlPool;

			GAIA::CTN::StringPtrPool<GAIA::TCH> m_strpool;
			GAIA::CTN::StaticStringPtrPool<GAIA::TCH> m_sstrpool;

			GAIA::CTN::Pool<XmlNode> m_NodePool;
			GAIA::CTN::Pool<Xml> m_XmlPool;
		};
	}
}

#endif
