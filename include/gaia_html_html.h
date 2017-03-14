/*
	SUPPORTED HTML

	<!DOCTYPE html>
	<html>
		<!--comment content-->
		<NODE1/>
		<NODE1/>
		<NODE1></NODE1>
		<NODE1>
		<NODE2 a="abc"/>
		<NODE3>
			abcdef
		</NODE3>
		<NODE4 a="abc" b="efg">
			abcdef
		</NODE4>
	</html>
*/

#ifndef		__GAIA_HTML_HTML_H__
#define		__GAIA_HTML_HTML_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_string.h"
#include "gaia_fsys_file.h"
#include "gaia_html_htmlbase.h"
#include "gaia_html_htmlnode.h"

namespace GAIA
{
	namespace HTML
	{
		class Html : public HtmlFactoryHolder
		{
		public:
			class Cursor : public GAIA::Base
			{
				friend class Html;
			public:
				GINL Cursor(){pCurrentNode = GNIL;}
			private:
				HtmlNode* pCurrentNode;
			};
		public:
			GINL Html();
			GINL ~Html();

			GINL GAIA::GVOID Reset();

			GINL GAIA::BL LoadFromFile(const GAIA::TCH* pszFileName);
			GINL GAIA::BL SaveToFile(const GAIA::TCH* pszFileName, GAIA::HTML::HTML_SAVE st = GAIA::HTML::HTML_SAVE_BESTREAD);
			GINL GAIA::BL LoadFromMem(const GAIA::TCH* pszSrc, const GAIA::NUM& sSize);
			GINL GAIA::BL SaveToMem(GAIA::TCH* pszDst, const GAIA::NUM& sSize, GAIA::HTML::HTML_SAVE st = GAIA::HTML::HTML_SAVE_BESTREAD);

			GINL HtmlNode& GetRootNode();
			GINL const HtmlNode& GetRootNode() const;

			GINL GAIA::BL BeginWriteNode(GAIA::HTML::Html::Cursor& cur, GAIA::HTML::HTML_NODE nt, const GAIA::TCH* pszNodeName);
			GINL GAIA::BL EndWriteNode(GAIA::HTML::Html::Cursor& cur);
			GINL GAIA::BL WriteNode(GAIA::HTML::Html::Cursor& cur, GAIA::HTML::HTML_NODE nt, const GAIA::TCH* pszNodeName);

			GINL const GAIA::TCH* BeginReadNode(GAIA::HTML::Html::Cursor& cur) const;
			GINL GAIA::BL EndReadNode(GAIA::HTML::Html::Cursor& cur) const;
			GINL const GAIA::TCH* ReadNode(GAIA::HTML::Html::Cursor& cur) const;

		private:
			GINL GAIA::GVOID write_linebreak(GAIA::HTML::HTML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID write_indent(GAIA::HTML::HtmlNode& n, GAIA::HTML::HTML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID write_string(GAIA::HTML::HTML_SAVE st, GAIA::FSYS::FileBase& f, const GAIA::CH* p, GAIA::NUM sLen = GINVALID);
			GINL GAIA::GVOID write_string(GAIA::HTML::HTML_SAVE st, GAIA::FSYS::FileBase& f, const GAIA::WCH* p, GAIA::NUM sLen = GINVALID);
			GINL GAIA::GVOID save_node(GAIA::HTML::HtmlNode& n, GAIA::HTML::HTML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID save_child_node(GAIA::HTML::HtmlNode& n, GAIA::HTML::HTML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::GVOID save_child_node_value(GAIA::HTML::HtmlNode& n, GAIA::HTML::HTML_SAVE st, GAIA::FSYS::FileBase& f);
			GINL GAIA::NUM load_node(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::HTML::HTML_NODE& nt, GAIA::U8*& pBegin, GAIA::U8*& pEnd, GAIA::BL& bBegin) const;
			GINL GAIA::NUM load_head(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_comment(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_node_name(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_name(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::NUM load_value(GAIA::U8* pBuf, GAIA::NUM sSize, GAIA::NUM sOffset, GAIA::NUM& sNext) const;
			GINL GAIA::GVOID read_string(const GAIA::U8* pBegin, const GAIA::U8* pEnd, GAIA::CTN::WString& strResult) const;

		private:
			GAIA::HTML::HtmlNode m_root;
			GAIA::CTN::AString m_strTempA;
			GAIA::CTN::WString m_strTempW;
		};
	}
}

#endif
