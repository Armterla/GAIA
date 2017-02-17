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
		
		GAIA::TCH szFileName[GAIA::MAXPL];
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
