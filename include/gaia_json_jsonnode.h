#ifndef		__GAIA_JSON_JSONNODE_H__
#define		__GAIA_JSON_JSONNODE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_vector.h"
#include "gaia_json_jsonbase.h"

namespace GAIA
{
	namespace JSON
	{
		class JsonNode : public JsonFactoryHolder
		{
			friend class Json;

		public:
			GINL JsonNode();
			GINL ~JsonNode();

			GINL GAIA::GVOID Reset();

			GINL GAIA::BL IsRoot() const;

			GINL GAIA::GVOID SetType(GAIA::JSON::JSON_NODE nt);
			GINL GAIA::JSON::JSON_NODE GetType() const;
			GINL GAIA::GVOID SetName(const GAIA::TCH* pszName);
			GINL const GAIA::TCH* GetName() const;

			GINL GAIA::NUM InsertChild(JsonNode& node, GAIA::NUM sIndex = GINVALID);
			GINL GAIA::BL DeleteChild(const GAIA::NUM& sIndex);
			GINL GAIA::GVOID DeleteChildAll();
			GINL GAIA::NUM GetChildCount() const;
			GINL JsonNode* GetChild(const GAIA::NUM& nIndex) const;
			GINL GAIA::NUM SetParent(JsonNode* pParent);
			GINL JsonNode* GetParent() const;

			GINL GAIA::NUM GetDepth() const;
			GINL GAIA::NUM GetParentIndex() const;

		private:
			GINL GAIA::GVOID SetRoot(GAIA::BL bRoot);
			GINL GAIA::GVOID SetParentIndex(GAIA::NUM sIndex);

		private:
			GAIA::JSON::JSON_NODE m_nt;
			const GAIA::TCH* m_pszName;
			GAIA::BL m_bRoot;
			GAIA::JSON::JsonNode* m_pParent;
			GAIA::NUM m_sParentIndex;
			GAIA::CTN::Vector<GAIA::JSON::JsonNode*> m_childs;
			GAIA::CTN::Vector<GAIA::NUM> m_freestack;
		};
	}
}

#endif
