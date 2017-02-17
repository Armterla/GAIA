#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	static GAIA::GVOID SaveLoadHTML(GAIA::LOG::Log& logobj, GAIA::HTML::HTML& html)
	{
		GAIA::CTN::TChars strTemp, strTemp1, strTemp2;

		static const GAIA::NUM NODE_COUNT = 3;
		static const GAIA::NUM VALUE_COUNT = 3;

		GAIA::HTML::HTML::Cursor cur;

		html.BeginWriteNode(cur, GAIA::HTML::HTML_NODE_MULTICONTAINER, _T("Root"));

		for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
		{
			strTemp1 = "Name";
			strTemp1 += v;
			html.BeginWriteNode(cur, GAIA::HTML::HTML_NODE_NAME, strTemp1);

			strTemp2 = "Value";
			strTemp2 += v;
			html.WriteNode(cur, GAIA::HTML::HTML_NODE_VALUE, strTemp2);

			html.EndWriteNode(cur);
		}

		html.WriteNode(cur, GAIA::HTML::HTML_NODE_COMMENT, _T("Comment0"));

		for(GAIA::NUM x = 0; x < NODE_COUNT; ++x)
		{
			strTemp = "Node";
			strTemp += x;
			html.BeginWriteNode(cur, GAIA::HTML::HTML_NODE_MULTICONTAINER, strTemp);
			html.WriteNode(cur, GAIA::HTML::HTML_NODE_COMMENT, _T("Comment1"));
			for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
			{
				strTemp1 = strTemp + "_Name";
				strTemp1 += v;
				html.BeginWriteNode(cur, GAIA::HTML::HTML_NODE_NAME, strTemp1);

				strTemp2 = strTemp + "_Value";
				strTemp2 += v;
				html.WriteNode(cur, GAIA::HTML::HTML_NODE_VALUE, strTemp2);

				html.EndWriteNode(cur);
			}
			for(GAIA::NUM y = 0; y < NODE_COUNT; ++y)
			{
				strTemp = "Node";
				strTemp += x;
				strTemp += "_";
				strTemp += y;
				html.BeginWriteNode(cur, GAIA::HTML::HTML_NODE_MULTICONTAINER, strTemp);
				html.WriteNode(cur, GAIA::HTML::HTML_NODE_COMMENT, _T("Comment2"));
				for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
				{
					strTemp1 = strTemp + "_Name";
					strTemp1 += v;
					html.BeginWriteNode(cur, GAIA::HTML::HTML_NODE_NAME, strTemp1);

					strTemp2 = strTemp + "_Value";
					strTemp2 += v;
					html.WriteNode(cur, GAIA::HTML::HTML_NODE_VALUE, strTemp2);

					html.EndWriteNode(cur);
				}
				for(GAIA::NUM z = 0; z < NODE_COUNT; ++z)
				{
					strTemp = "Node";
					strTemp += x;
					strTemp += "_";
					strTemp += y;
					strTemp += "_";
					strTemp += z;
					html.BeginWriteNode(cur, GAIA::HTML::HTML_NODE_MULTICONTAINER, strTemp);
					html.WriteNode(cur, GAIA::HTML::HTML_NODE_COMMENT, _T("Comment3"));
					for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
					{
						strTemp1 = strTemp + "_Name";
						strTemp1 += v;
						html.BeginWriteNode(cur, GAIA::HTML::HTML_NODE_NAME, strTemp1);

						strTemp2 = strTemp + "_Value";
						strTemp2 += v;
						html.WriteNode(cur, GAIA::HTML::HTML_NODE_VALUE, strTemp2);

						html.EndWriteNode(cur);
					}
					html.EndWriteNode(cur);
				}
				html.EndWriteNode(cur);
			}
			html.EndWriteNode(cur);
		}

		html.EndWriteNode(cur);

		GAIA::TCH szFileName[GAIA::MAXPL];
		GAIA::ALGO::gstrcpy(szFileName, g_gaia_appdocdir);
		GAIA::ALGO::gstrcat(szFileName, "test.html");
		
		if(!html.SaveToFile(szFileName, GAIA::HTML::HTML_SAVE_BESTREAD))
			TERROR;
		if(!html.LoadFromFile(szFileName))
			TERROR;
		if(!html.SaveToFile(szFileName, GAIA::HTML::HTML_SAVE_BESTREAD))
			TERROR;
		if(!html.LoadFromFile(szFileName))
			TERROR;
		if(!html.SaveToFile(szFileName, GAIA::HTML::HTML_SAVE_BESTREAD))
			TERROR;
		GAIA::FSYS::Dir dir;
		dir.RemoveFile(szFileName);
	}
	extern GAIA::GVOID t_html_html(GAIA::LOG::Log& logobj)
	{
		GAIA::HTML::HTMLFactory fac;
		GAIA::HTML::HTMLFactoryDesc desc;
		desc.reset();
		fac.Create(desc);
		{
			GAIA::HTML::HTML& html = fac.CreateHTML();
			{
				SaveLoadHTML(logobj, html);
			}
			fac.ReleaseHTML(html);
		}
		fac.Destroy();

		GAIA::HTML::HTML* pHTML = gnew GAIA::HTML::HTML;
		{
			SaveLoadHTML(logobj, *pHTML);
		}
		gdel pHTML;
	}
}
