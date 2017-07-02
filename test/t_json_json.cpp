#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	static GAIA::GVOID SaveLoadJson(GAIA::LOG::Log& logobj, GAIA::JSON::Json& json)
	{
		GAIA::CTN::TChars strTemp, strTemp1, strTemp2;

		static const GAIA::NUM NODE_COUNT = 3;
		static const GAIA::NUM VALUE_COUNT = 3;

		GAIA::JSON::Json::Cursor cur;

		json.BeginWriteNode(cur, GAIA::JSON::JSON_NODE_MULTICONTAINER, _T("Root"));

		for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
		{
			strTemp1 = "Name";
			strTemp1 += v;
			json.BeginWriteNode(cur, GAIA::JSON::JSON_NODE_NAME, strTemp1);

			strTemp2 = "Value";
			strTemp2 += v;
			json.WriteNode(cur, GAIA::JSON::JSON_NODE_VALUE, strTemp2);

			json.EndWriteNode(cur);
		}

		for(GAIA::NUM x = 0; x < NODE_COUNT; ++x)
		{
			strTemp = "Node";
			strTemp += x;
			json.BeginWriteNode(cur, GAIA::JSON::JSON_NODE_MULTICONTAINER, strTemp);
			for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
			{
				strTemp1 = strTemp + "_Name";
				strTemp1 += v;
				json.BeginWriteNode(cur, GAIA::JSON::JSON_NODE_NAME, strTemp1);

				strTemp2 = strTemp + "_Value";
				strTemp2 += v;
				json.WriteNode(cur, GAIA::JSON::JSON_NODE_VALUE, strTemp2);

				json.EndWriteNode(cur);
			}
			for(GAIA::NUM y = 0; y < NODE_COUNT; ++y)
			{
				strTemp = "Node";
				strTemp += x;
				strTemp += "_";
				strTemp += y;
				json.BeginWriteNode(cur, GAIA::JSON::JSON_NODE_MULTICONTAINER, strTemp);
				for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
				{
					strTemp1 = strTemp + "_Name";
					strTemp1 += v;
					json.BeginWriteNode(cur, GAIA::JSON::JSON_NODE_NAME, strTemp1);

					strTemp2 = strTemp + "_Value";
					strTemp2 += v;
					json.WriteNode(cur, GAIA::JSON::JSON_NODE_VALUE, strTemp2);

					json.EndWriteNode(cur);
				}
				for(GAIA::NUM z = 0; z < NODE_COUNT; ++z)
				{
					strTemp = "Node";
					strTemp += x;
					strTemp += "_";
					strTemp += y;
					strTemp += "_";
					strTemp += z;
					json.BeginWriteNode(cur, GAIA::JSON::JSON_NODE_MULTICONTAINER, strTemp);
					for(GAIA::NUM v = 0; v < VALUE_COUNT; ++v)
					{
						strTemp1 = strTemp + "_Name";
						strTemp1 += v;
						json.BeginWriteNode(cur, GAIA::JSON::JSON_NODE_NAME, strTemp1);

						strTemp2 = strTemp + "_Value";
						strTemp2 += v;
						json.WriteNode(cur, GAIA::JSON::JSON_NODE_VALUE, strTemp2);

						json.EndWriteNode(cur);
					}
					json.EndWriteNode(cur);
				}
				json.EndWriteNode(cur);
			}
			json.EndWriteNode(cur);
		}

		json.EndWriteNode(cur);
		
		GAIA::CH szFileName[GAIA::MAXPL];
		GAIA::ALGO::gstrcpy(szFileName, g_gaia_appdocdir);
		GAIA::ALGO::gstrcat(szFileName, "test.json");

		if(!json.SaveToFile(szFileName, GAIA::JSON::JSON_SAVE_BESTREAD))
			TERROR;
		if(!json.LoadFromFile(szFileName))
			TERROR;
		if(!json.SaveToFile(szFileName, GAIA::JSON::JSON_SAVE_BESTREAD))
			TERROR;
		if(!json.LoadFromFile(szFileName))
			TERROR;
		if(!json.SaveToFile(szFileName, GAIA::JSON::JSON_SAVE_BESTREAD))
			TERROR;
		GAIA::FSYS::Dir dir;
		dir.RemoveFile(szFileName);
	}
	extern GAIA::GVOID t_json_json(GAIA::LOG::Log& logobj)
	{
		// Json format test.
		{
			const GAIA::CH SRC[] = "\t\t\n{\"aa\":true,\t\t\"bb\" : false,\"cc\": [ { \"a\":123,\"b\":456,\"c\":[],\"d\":{\"aaa\":\"aaavalue\"\t},\"e\":{\n\t}}]}";
			const GAIA::CH DST[] = "{\"aa\":true,\"bb\":false,\"cc\":[{\"a\":123,\"b\":456,\"c\":[],\"d\":{\"aaa\":\"aaavalue\"},\"e\":{}}]}";
			
			GAIA::CH szDst[1024];
			GAIA::CH szDst1[1024];
			
			GAIA::NUM sResult0 = GAIA::JSON::JsonChangeFormat(SRC, sizeof(SRC) - 1, (GAIA::CH*)GNIL, 0, GAIA::JSON::JSON_SAVE_BESTSIZE);
			TAST(sResult0 != GINVALID);
			GAIA::NUM sResult1 = GAIA::JSON::JsonChangeFormat(SRC, sizeof(SRC) - 1, szDst, sizeof(szDst), GAIA::JSON::JSON_SAVE_BESTSIZE);
			TAST(sResult1 != GINVALID);
			TAST(sResult0 == sResult1);
			TAST(sResult0 == sizeof(DST) - 1);
			szDst[sResult1] = '\0';
			TAST(GAIA::ALGO::gstrcmp(szDst, DST) == 0);
			
			sResult0 = GAIA::JSON::JsonChangeFormat(DST, sizeof(DST) - 1, (GAIA::CH*)GNIL, 0, GAIA::JSON::JSON_SAVE_BESTREAD);
			TAST(sResult0 != GINVALID);
			sResult1 = GAIA::JSON::JsonChangeFormat(DST, sizeof(DST) - 1, szDst, sizeof(szDst), GAIA::JSON::JSON_SAVE_BESTREAD);
			TAST(sResult1 != GINVALID);
			TAST(sResult0 == sResult1);
			szDst[sResult1] = '\0';
			
			GAIA::NUM sDstLen = sResult0;
			
			sResult0 = GAIA::JSON::JsonChangeFormat(szDst, sDstLen, (GAIA::CH*)GNIL, 0, GAIA::JSON::JSON_SAVE_BESTSIZE);
			TAST(sResult0 != GINVALID);
			sResult1 = GAIA::JSON::JsonChangeFormat(szDst, sDstLen, szDst1, sizeof(szDst1), GAIA::JSON::JSON_SAVE_BESTSIZE);
			TAST(sResult1 != GINVALID);
			TAST(sResult0 == sResult1);
			TAST(sResult0 == sizeof(DST) - 1);
			szDst1[sResult1] = '\0';
			
			GAIA::NUM sDst1Len = sResult0;
			
			szDst[sDstLen] = '\0';
			szDst1[sDst1Len] = '\0';
			
			TAST(GAIA::ALGO::gstrcmp(szDst1, DST) == 0);
		}
		
		//
		GAIA::JSON::JsonFactory fac;
		GAIA::JSON::JsonFactoryDesc desc;
		desc.reset();
		fac.Create(desc);
		{
			GAIA::JSON::Json& json = fac.CreateJson();
			{
				SaveLoadJson(logobj, json);
			}
			fac.ReleaseJson(json);
		}
		fac.Destroy();

		GAIA::JSON::Json* pJson = gnew GAIA::JSON::Json;
		{
			SaveLoadJson(logobj, *pJson);
		}
		gdel pJson;
	}
}
