#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	static GAIA::GVOID SaveLoadXML(GAIA::LOG::Log& logobj, GAIA::XML::Xml& xml)
	{
		GAIA::CTN::TChars strTemp, strTemp1, strTemp2;

		static const GAIA::NUM NODE_COUNT = 3;
		static const GAIA::NUM VALUE_COUNT = 3;

		GAIA::XML::Xml::Cursor cur;

		xml.BeginWriteNode(cur, GAIA::XML::XML_NODE_MULTICONTAINER, _T("Root"));

		for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
		{
			strTemp1 = "Name";
			strTemp1 += v;
			xml.BeginWriteNode(cur, GAIA::XML::XML_NODE_NAME, strTemp1);

			strTemp2 = "Value";
			strTemp2 += v;
			xml.WriteNode(cur, GAIA::XML::XML_NODE_VALUE, strTemp2);

			xml.EndWriteNode(cur);
		}

		xml.WriteNode(cur, GAIA::XML::XML_NODE_COMMENT, _T("Comment0"));

		for(GAIA::NUM x = 0; x < NODE_COUNT; ++x)
		{
			strTemp = "Node";
			strTemp += x;
			xml.BeginWriteNode(cur, GAIA::XML::XML_NODE_MULTICONTAINER, strTemp);
			xml.WriteNode(cur, GAIA::XML::XML_NODE_COMMENT, _T("Comment1"));
			for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
			{
				strTemp1 = strTemp + "_Name";
				strTemp1 += v;
				xml.BeginWriteNode(cur, GAIA::XML::XML_NODE_NAME, strTemp1);

				strTemp2 = strTemp + "_Value";
				strTemp2 += v;
				xml.WriteNode(cur, GAIA::XML::XML_NODE_VALUE, strTemp2);

				xml.EndWriteNode(cur);
			}
			for(GAIA::NUM y = 0; y < NODE_COUNT; ++y)
			{
				strTemp = "Node";
				strTemp += x;
				strTemp += "_";
				strTemp += y;
				xml.BeginWriteNode(cur, GAIA::XML::XML_NODE_MULTICONTAINER, strTemp);
				xml.WriteNode(cur, GAIA::XML::XML_NODE_COMMENT, _T("Comment2"));
				for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
				{
					strTemp1 = strTemp + "_Name";
					strTemp1 += v;
					xml.BeginWriteNode(cur, GAIA::XML::XML_NODE_NAME, strTemp1);

					strTemp2 = strTemp + "_Value";
					strTemp2 += v;
					xml.WriteNode(cur, GAIA::XML::XML_NODE_VALUE, strTemp2);

					xml.EndWriteNode(cur);
				}
				for(GAIA::NUM z = 0; z < NODE_COUNT; ++z)
				{
					strTemp = "Node";
					strTemp += x;
					strTemp += "_";
					strTemp += y;
					strTemp += "_";
					strTemp += z;
					xml.BeginWriteNode(cur, GAIA::XML::XML_NODE_MULTICONTAINER, strTemp);
					xml.WriteNode(cur, GAIA::XML::XML_NODE_COMMENT, _T("Comment3"));
					for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
					{
						strTemp1 = strTemp + "_Name";
						strTemp1 += v;
						xml.BeginWriteNode(cur, GAIA::XML::XML_NODE_NAME, strTemp1);

						strTemp2 = strTemp + "_Value";
						strTemp2 += v;
						xml.WriteNode(cur, GAIA::XML::XML_NODE_VALUE, strTemp2);

						xml.EndWriteNode(cur);
					}
					xml.EndWriteNode(cur);
				}
				xml.EndWriteNode(cur);
			}
			xml.EndWriteNode(cur);
		}

		xml.EndWriteNode(cur);
		
		GAIA::TCH szFileName[GAIA::MAXPL];
		GAIA::ALGO::gstrcpy(szFileName, g_gaia_appdocdir);
		GAIA::ALGO::gstrcat(szFileName, "test.xml");

		if(!xml.SaveToFile(szFileName, GAIA::XML::XML_SAVE_BESTREAD))
			TERROR;
		if(!xml.LoadFromFile(szFileName))
			TERROR;
		if(!xml.SaveToFile(szFileName, GAIA::XML::XML_SAVE_BESTREAD))
			TERROR;
		if(!xml.LoadFromFile(szFileName))
			TERROR;
		if(!xml.SaveToFile(szFileName, GAIA::XML::XML_SAVE_BESTREAD))
			TERROR;
		GAIA::FSYS::Dir dir;
		dir.RemoveFile(szFileName);
	}
	extern GAIA::GVOID t_xml_xml(GAIA::LOG::Log& logobj)
	{
		// Xml format test.
		{
			const GAIA::CH SRC[] = "<a>  <b c=\"1\" \t d = \"string\"/><bb cc=\"1.2\" dd=\"string value\"\t ><bbb ccc=\"123\"  ddd=\"a string value\"/>\n\n</bb></a>";
			const GAIA::CH DST[] = "<a><b c=\"1\" d=\"string\"/><bb cc=\"1.2\" dd=\"string value\"><bbb ccc=\"123\" ddd=\"a string value\"/></bb></a>";
			
			GAIA::CH szDst[1024];
			GAIA::CH szDst1[1024];
			
			GAIA::NUM sResult0 = GAIA::XML::XmlChangeFormat(SRC, sizeof(SRC) - 1, (GAIA::CH*)GNIL, 0, GAIA::XML::XML_SAVE_BESTSIZE);
			TAST(sResult0 != GINVALID);
			GAIA::NUM sResult1 = GAIA::XML::XmlChangeFormat(SRC, sizeof(SRC) - 1, szDst, sizeof(szDst), GAIA::XML::XML_SAVE_BESTSIZE);
			TAST(sResult1 != GINVALID);
			TAST(sResult0 == sResult1);
			TAST(sResult0 == sizeof(DST) - 1);
			szDst[sResult1] = '\0';
			TAST(GAIA::ALGO::gstrcmp(szDst, DST) == 0);
			
			sResult0 = GAIA::XML::XmlChangeFormat(DST, sizeof(DST) - 1, (GAIA::CH*)GNIL, 0, GAIA::XML::XML_SAVE_BESTREAD);
			TAST(sResult0 != GINVALID);
			sResult1 = GAIA::XML::XmlChangeFormat(DST, sizeof(DST) - 1, szDst, sizeof(szDst), GAIA::XML::XML_SAVE_BESTREAD);
			TAST(sResult1 != GINVALID);
			TAST(sResult0 == sResult1);
			szDst[sResult1] = '\0';
			
			GAIA::NUM sDstLen = sResult0;
			
			sResult0 = GAIA::XML::XmlChangeFormat(szDst, sDstLen, (GAIA::CH*)GNIL, 0, GAIA::XML::XML_SAVE_BESTSIZE);
			TAST(sResult0 != GINVALID);
			sResult1 = GAIA::XML::XmlChangeFormat(szDst, sDstLen, szDst1, sizeof(szDst1), GAIA::XML::XML_SAVE_BESTSIZE);
			TAST(sResult1 != GINVALID);
			TAST(sResult0 == sResult1);
			TAST(sResult0 == sizeof(DST) - 1);
			szDst1[sResult1] = '\0';
			
			GAIA::NUM sDst1Len = sResult0;
			
			szDst[sDstLen] = '\0';
			szDst1[sDst1Len] = '\0';
			
			TAST(GAIA::ALGO::gstrcmp(szDst1, DST) == 0);
		}
		
		GAIA::XML::XmlFactory fac;
		GAIA::XML::XmlFactoryDesc desc;
		desc.reset();
		fac.Create(desc);
		{
			GAIA::XML::Xml& xml = fac.CreateXml();
			{
				SaveLoadXML(logobj, xml);
			}
			fac.ReleaseXml(xml);
		}
		fac.Destroy();

		GAIA::XML::Xml* pXml = gnew GAIA::XML::Xml;
		{
			SaveLoadXML(logobj, *pXml);
		}
		gdel pXml;
	}
}
