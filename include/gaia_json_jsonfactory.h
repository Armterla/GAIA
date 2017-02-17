#ifndef		__GAIA_JSON_JSONFACTORY_H__
#define		__GAIA_JSON_JSONFACTORY_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_stringptrpool.h"
#include "gaia_ctn_staticstringptrpool.h"
#include "gaia_sync_lockrw.h"
#include "gaia_json_jsonfactorydesc.h"

namespace GAIA
{
	namespace JSON
	{
		class JsonFactory : public GAIA::Base
		{
			friend class Json;
			friend class JsonNode;

		public:
			GINL JsonFactory();
			GINL ~JsonFactory();

			GINL GAIA::BL Create(const JsonFactoryDesc& desc);
			GINL GAIA::BL Destroy();
			GINL GAIA::BL IsCreated() const;
			GINL const JsonFactoryDesc& GetDesc() const;

			GINL JsonNode& CreateNode();
			GINL Json& CreateJson();

			GINL GAIA::BL ReleaseNode(JsonNode& node);
			GINL GAIA::BL ReleaseJson(Json& json);

		private:
			GINL const GAIA::TCH* AllocString(const GAIA::TCH* psz);
			GINL GAIA::BL ReleaseString(const GAIA::TCH* psz);
			GINL const GAIA::TCH* AllocStaticString(const GAIA::TCH* psz);
			GINL GAIA::BL ReleaseStaticString(const GAIA::TCH* psz);

		private:
			JsonFactoryDesc m_desc;
			GAIA::BL m_bCreated;

			GAIA::SYNC::LockRW m_rwStringPool;
			GAIA::SYNC::LockRW m_rwStaticStringPool;

			GAIA::SYNC::LockRW m_rwNodePool;
			GAIA::SYNC::LockRW m_rwJsonPool;

			GAIA::CTN::StringPtrPool<GAIA::TCH> m_strpool;
			GAIA::CTN::StaticStringPtrPool<GAIA::TCH> m_sstrpool;

			GAIA::CTN::Pool<JsonNode> m_NodePool;
			GAIA::CTN::Pool<Json> m_JsonPool;
		};
	}
}

#endif
