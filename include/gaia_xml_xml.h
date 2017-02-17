/*
	SUPPORTED XML

	<?xml version="1.0" encoding="utf-8"?>
	<ROOT>
		<!--comment content-->
		<NODE1/>
		<NODE1/>
		<NODE1></NODE1>
		<NODE2 a="abc"/>
		<NODE3>
			abcdef
		</NODE3>
		<NODE4 a="abc" b="efg">
			abcdef
		</NODE4>
	</ROOT>
*/

#ifndef		__GAIA_XML_XML_H__
#define		__GAIA_XML_XML_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_string.h"
#include "gaia_fsys_file.h"
#include "gaia_xml_xmlbase.h"
#include "gaia_xml_xmlnode.h"

namespace GAIA
{
	namespace XML
	{
		class XML : public XMLFactoryHolder
		{
		public:
			class Cursor : public GAIA::Base
			{
				friend class XML;
			public:
				GINL Cursor(){pCurrentNode = GNIL;}
			private:
				XMLNode* pCurrentNode;
			};
		public:
			GINL XML();
			GINL ~XML();

			GINL GAIA::GVOID Reset();

			GINL GAIA::BL LoadFromFile(const GAIA::TCH* pszFileName);
			GINL GAIA::BL SaveToFile(const GAIA::TCH* pszFileName, GAIA::XML::XML_SAVE st = GAIA::XML::XML_SAVE_BESTREAD);
			GINL GAIA::BL LoadFromMem(const GAIA::TCH* pszSrc, const GAIA::NUM& sSize);
			GINL GAIA::BL SaveToMem(GAIA::TCH* pszDst, const GAIA::NUM& sSize, GAIA::XML::XML_SAVE st = GAIA::XML::XML_SAVE_BESTREAD);

			GINL XMLNode& GetRootNode();
			GINL const XMLNode& GetRootNode() const;

			GINL GAIA::BL BeginWriteNode(GAIA::XML::XML::Cursor& cur, GAIA::XML::XML_NODE nt, const GAIA::TCH* pszNodeName);
			GINL GAIA::BL EndWriteNode(GAIA::XML::XML::Cursor& cur);
			GINL GAIA::BL WriteNode(GAIA::XML::XML::Cursor& cur, GAIA::XML::XML_NODE nt, const GAIA::TCH* pszNodeName);

			GINL const GAIA::TCH* BeginReadNode(GAIA::XML::XML::Cursor& cur) const;
			GINL GAIA::BL EndReadNode(GAIA::XML::XML::Cursor& cur) const;
			GINL const GAIA::TCH* ReadNode(GAIA::XML::XML::Cursor& cur) const;

		private:
			GINL GAIA::GVOID write_linebreak(GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID write_indent(GAIA::XML::XMLNode& n, GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID write_string(GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f, const GAIA::CH* p, GAIA::NUM sLen = GINVALID);
			GINL GAIA::GVOID write_string(GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f, const GAIA::WCH* p, GAIA::NUM sLen = GINVALID);
			GINL GAIA::GVOID save_node(GAIA::XML::XMLNode& n, GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID save_child_node(GAIA::XML::XMLNode& n, GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID save_child_node_value(GAIA::XML::XMLNode& n, GAIA::XML::XML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::NUM load_node(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::XML::XML_NODE& nt, GAIA::U8*& pBegin, GAIA::U8*& pEnd, GAIA::BL& bBegin) const;
			GINL GAIA::NUM load_head(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_comment(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_node_name(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_name(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_value(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::GVOID read_string(const GAIA::U8* pBegin, const GAIA::U8* pEnd, GAIA::CTN::WString& strResult) const;

		private:
			GAIA::XML::XMLNode m_root;
			GAIA::CTN::AString m_strTempA;
			GAIA::CTN::WString m_strTempW;
		};
	}
}

#endif
