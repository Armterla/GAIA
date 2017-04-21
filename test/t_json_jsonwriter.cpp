#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_json_jsonwriter(GAIA::LOG::Log& logobj)
	{
		static const GAIA::BL ENABLE_DEBUG_OUTPUT = GAIA::False;
		static const GAIA::NUM TEST_TIMES = 1000;
		static const GAIA::NUM SAMPLE_COUNT = 3;

		GAIA::JSON::JsonWriterA jw;
		GAIA::NUM sBufferSize;
		if(jw.GetBuffer(sBufferSize) != GNIL)
			TERROR;
		if(sBufferSize != 0)
			TERROR;
		if(jw.GetWriteSize() != 0)
			TERROR;
		if(jw.GetRemainSize() != 0)
			TERROR;

		GAIA::CTN::AString strResult;
		GAIA::CTN::Buffer buf;
		buf.resize(1024 * 1024 * 4);

		for(GAIA::NUM t = 0; t < TEST_TIMES; ++t)
		{
			// Depth1 test.
			{
				jw.SetBuffer(buf.fptr(), buf.write_size());
				jw.Begin(GAIA::JSON::JSON_NODE_CONTAINER);
				{
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						GAIA::CH szTempIndexX[32];
						GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

						GAIA::CH szTempName[32] = "Prop";
						GAIA::ALGO::gstrcat(szTempName, szTempIndexX);

						GAIA::CH szTempValue[32] = "Value";
						GAIA::ALGO::gstrcat(szTempValue, szTempIndexX);

						jw.Write(GAIA::JSON::JSON_NODE_NAME, szTempName);
						jw.Write(GAIA::JSON::JSON_NODE_VALUE, szTempValue);
					}
				}
				jw.End();

				GAIA::NUM sWriteSize = jw.GetWriteSize();

				strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
				if(ENABLE_DEBUG_OUTPUT)
					TLOG(strResult.fptr());
				if(strResult != "{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"}")
					TERROR;
			}

			// Depth3 test.
			{
				jw.SetBuffer(buf.fptr(), buf.write_size());
				jw.Begin(GAIA::JSON::JSON_NODE_MULTICONTAINER);
				{
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						jw.Begin(GAIA::JSON::JSON_NODE_CONTAINER, GNIL);
						{
							for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
							{
								GAIA::CH szTempIndexY[32];
								GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

								GAIA::CH szTempName[32] = "Prop";
								GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

								GAIA::CH szTempValue[32] = "Value";
								GAIA::ALGO::gstrcat(szTempValue, szTempIndexY);

								jw.Write(GAIA::JSON::JSON_NODE_NAME, szTempName);
								jw.Write(GAIA::JSON::JSON_NODE_VALUE, szTempValue);
							}

							for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
							{
								GAIA::CH szTempIndexY[32];
								GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

								GAIA::CH szTempName[32] = "Node";
								GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

								jw.Begin(GAIA::JSON::JSON_NODE_CONTAINER, szTempName);
								{
									for(GAIA::NUM z = 0; z < SAMPLE_COUNT; ++z)
									{
										GAIA::CH szTempIndexZ[32];
										GAIA::ALGO::castv(z, szTempIndexZ, sizeof(szTempIndexZ));

										GAIA::CH szTempName[32] = "Prop";
										GAIA::ALGO::gstrcat(szTempName, szTempIndexZ);

										GAIA::CH szTempValue[32] = "Value";
										GAIA::ALGO::gstrcat(szTempValue, szTempIndexZ);

										jw.Write(GAIA::JSON::JSON_NODE_NAME, szTempName);
										jw.Write(GAIA::JSON::JSON_NODE_VALUE, szTempValue);
									}
								}
								jw.End();
							}
						}
						jw.End();
					}
				}
				jw.End();

				GAIA::NUM sWriteSize = jw.GetWriteSize();

				strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
				if(ENABLE_DEBUG_OUTPUT)
					TLOG(strResult.fptr());
				if(strResult != "[{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\",\"Node0\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node1\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node2\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"}},{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\",\"Node0\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node1\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node2\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"}},{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\",\"Node0\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node1\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node2\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"}}]")
					TERROR;
			}

			// Template test.
			{
				jw.SetBuffer(buf.fptr(), buf.write_size());
				jw.Begin(GAIA::JSON::JSON_NODE_MULTICONTAINER);
				{
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						jw.Begin(GAIA::JSON::JSON_NODE_CONTAINER, GNIL);
						{
							for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
							{
								GAIA::CH szTempIndexY[32];
								GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

								GAIA::WCH szTempName[32] = L"Prop";
								GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

								jw << szTempName << y;
							}

							for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
							{
								GAIA::CH szTempIndexY[32];
								GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

								GAIA::CH szTempName[32] = "Node";
								GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

								jw.Begin(GAIA::JSON::JSON_NODE_CONTAINER, szTempName);
								{
									for(GAIA::NUM z = 0; z < SAMPLE_COUNT; ++z)
									{
										GAIA::CH szTempIndexZ[32];
										GAIA::ALGO::castv(z, szTempIndexZ, sizeof(szTempIndexZ));

										GAIA::CH szTempName[32] = "Prop";
										GAIA::ALGO::gstrcat(szTempName, szTempIndexZ);

										GAIA::CH szTempValue[32] = "Value";
										GAIA::ALGO::gstrcat(szTempValue, szTempIndexZ);

										jw << szTempName << z;
									}
								}
								jw.End();
							}
						}
						jw.End();
					}
				}
				jw.End();

				GAIA::NUM sWriteSize = jw.GetWriteSize();

				strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
				if(ENABLE_DEBUG_OUTPUT)
					TLOG(strResult.fptr());
				if(strResult != "[{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2,\"Node0\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2},\"Node1\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2},\"Node2\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2}},{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2,\"Node0\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2},\"Node1\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2},\"Node2\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2}},{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2,\"Node0\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2},\"Node1\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2},\"Node2\":{\"Prop0\":0,\"Prop1\":1,\"Prop2\":2}}]")
					TERROR;
			}
		}
		
		// Empty value test.
		{
			jw.SetBuffer(buf.fptr(), buf.write_size());
			jw.Begin(GAIA::JSON::JSON_NODE_CONTAINER);
			{
				jw.Write(GAIA::JSON::JSON_NODE_NAME, "name1");
				jw.Write(GAIA::JSON::JSON_NODE_VALUE, "");
				
				jw.Write(GAIA::JSON::JSON_NODE_NAME, "name2");
				jw.Write(GAIA::JSON::JSON_NODE_VALUE, (const GAIA::CH*)GNIL);
			}
			jw.End();
			
			GAIA::NUM sWriteSize = jw.GetWriteSize();
			
			strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
			if(ENABLE_DEBUG_OUTPUT)
				TLOG(strResult.fptr());
			if(strResult != "{\"name1\":\"\",\"name2\":\"\"}")
				TERROR;
		}
	}
}
