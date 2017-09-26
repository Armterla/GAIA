/*
	SUPPORTED JSON

	{
		"a":1,
		"b":[
				{"c":1, "d":"f"},
				{"c":2, "d":"g"}
			],
		"h":[1,2,3,"4",{"k":5}],
		"i":{"m":"n"},
		"x":[],
		"y":{}
	}
*/

#ifndef		__GAIA_JSON_JSON_H__
#define		__GAIA_JSON_JSON_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_string.h"
#include "gaia_fsys_file.h"
#include "gaia_json_jsonbase.h"
#include "gaia_json_jsonnode.h"

namespace GAIA
{
	namespace JSON
	{
		class Json : public JsonFactoryHolder
		{
		public:
			class Cursor : public GAIA::Base
			{
				friend class Json;
			public:
				GINL Cursor(){pCurrentNode = GNIL;}
			private:
				JsonNode* pCurrentNode;
			};
		public:
			GINL Json();
			GINL ~Json();

			GINL GAIA::GVOID Reset();

			GINL GAIA::BL LoadFromFile(const GAIA::CH* pszFileName);
			GINL GAIA::BL SaveToFile(const GAIA::CH* pszFileName, GAIA::JSON::JSON_SAVE st = GAIA::JSON::JSON_SAVE_BESTREAD);
			GINL GAIA::BL LoadFromMem(const GAIA::TCH* pszSrc, const GAIA::NUM& sSize);
			GINL GAIA::BL SaveToMem(GAIA::TCH* pszDst, const GAIA::NUM& sSize, GAIA::JSON::JSON_SAVE st = GAIA::JSON::JSON_SAVE_BESTREAD);

			GINL JsonNode& GetRootNode();
			GINL const JsonNode& GetRootNode() const;

			GINL GAIA::BL BeginWriteNode(GAIA::JSON::Json::Cursor& cur, GAIA::JSON::JSON_NODE nt, const GAIA::TCH* pszNodeName);
			GINL GAIA::BL EndWriteNode(GAIA::JSON::Json::Cursor& cur);
			GINL GAIA::BL WriteNode(GAIA::JSON::Json::Cursor& cur, GAIA::JSON::JSON_NODE nt, const GAIA::TCH* pszNodeName);

			GINL const GAIA::TCH* BeginReadNode(GAIA::JSON::Json::Cursor& cur) const;
			GINL GAIA::BL EndReadNode(GAIA::JSON::Json::Cursor& cur) const;
			GINL const GAIA::TCH* ReadNode(GAIA::JSON::Json::Cursor& cur) const;

		private:
			GINL GAIA::GVOID write_linebreak(GAIA::JSON::JSON_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID write_indent(GAIA::JSON::JsonNode& n, GAIA::JSON::JSON_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID write_string(GAIA::JSON::JSON_SAVE st, GAIA::FSYS::FileBase& f, const GAIA::CH* p, GAIA::NUM sLen = GINVALID);
			GINL GAIA::GVOID write_string(GAIA::JSON::JSON_SAVE st, GAIA::FSYS::FileBase& f, const GAIA::WCH* p, GAIA::NUM sLen = GINVALID);
			GINL GAIA::GVOID save_node(GAIA::JSON::JsonNode& n, GAIA::JSON::JSON_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID save_child_node(GAIA::JSON::JsonNode& n, GAIA::JSON::JSON_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID save_child_node_value(GAIA::JSON::JsonNode& n, GAIA::JSON::JSON_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::NUM load_node(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::JSON::JSON_NODE& nt, GAIA::U8*& pBegin, GAIA::U8*& pEnd, GAIA::BL& bBegin) const;
			GINL GAIA::NUM load_head(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_comment(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_node_name(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_name(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_value(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::GVOID read_string(const GAIA::U8* pBegin, const GAIA::U8* pEnd, GAIA::CTN::WString& strResult) const;

		private:
			GAIA::JSON::JsonNode m_root;
			GAIA::CTN::AString m_strTempA;
			GAIA::CTN::WString m_strTempW;
		};
	}
}

#endif
