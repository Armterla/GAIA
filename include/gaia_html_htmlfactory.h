#ifndef		__GAIA_HTML_HTMLFACTORY_H__
#define		__GAIA_HTML_HTMLFACTORY_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_stringptrpool.h"
#include "gaia_ctn_staticstringptrpool.h"
#include "gaia_sync_lockrw.h"
#include "gaia_html_htmlfactorydesc.h"

namespace GAIA
{
	namespace HTML
	{
		class HTMLFactory : public GAIA::Base
		{
			friend class HTML;
			friend class HTMLNode;

		public:
			GINL HTMLFactory();
			GINL ~HTMLFactory();

			GINL GAIA::BL Create(const HTMLFactoryDesc& desc);
			GINL GAIA::BL Destroy();
			GINL GAIA::BL IsCreated() const;
			GINL const HTMLFactoryDesc& GetDesc() const;

			GINL HTMLNode& CreateNode();
			GINL HTML& CreateHTML();

			GINL GAIA::BL ReleaseNode(HTMLNode& node);
			GINL GAIA::BL ReleaseHTML(HTML& html);

		private:
			GINL const GAIA::TCH* AllocString(const GAIA::TCH* psz);
			GINL GAIA::BL ReleaseString(const GAIA::TCH* psz);
			GINL const GAIA::TCH* AllocStaticString(const GAIA::TCH* psz);
			GINL GAIA::BL ReleaseStaticString(const GAIA::TCH* psz);

		private:
			HTMLFactoryDesc m_desc;
			GAIA::BL m_bCreated;

			GAIA::SYNC::LockRW m_rwStringPool;
			GAIA::SYNC::LockRW m_rwStaticStringPool;

			GAIA::SYNC::LockRW m_rwNodePool;
			GAIA::SYNC::LockRW m_rwHTMLPool;

			GAIA::CTN::StringPtrPool<GAIA::TCH> m_strpool;
			GAIA::CTN::StaticStringPtrPool<GAIA::TCH> m_sstrpool;

			GAIA::CTN::Pool<HTMLNode> m_NodePool;
			GAIA::CTN::Pool<HTML> m_HTMLPool;
		};
	}
}

#endif
