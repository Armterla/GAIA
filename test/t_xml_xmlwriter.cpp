#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_xml_xmlwriter(GAIA::LOG::Log& logobj)
	{
		static const GAIA::BL ENABLE_DEBUG_OUTPUT = GAIA::False;
		static const GAIA::NUM TEST_TIMES = 1000;
		static const GAIA::NUM SAMPLE_COUNT = 3;

		GAIA::XML::XmlWriterA xw;
		GAIA::NUM sBufferSize;
		if(xw.GetBuffer(sBufferSize) != GNIL)
			TERROR;
		if(sBufferSize != 0)
			TERROR;
		if(xw.GetWriteSize() != 0)
			TERROR;
		if(xw.GetRemainSize() != 0)
			TERROR;

		GAIA::CTN::AString strResult;
		GAIA::CTN::Buffer buf;
		buf.resize(1024 * 1024 * 4);

		for(GAIA::NUM t = 0; t < TEST_TIMES; ++t)
		{
			// Depth1 test.
			{
				xw.SetBuffer(buf.fptr(), buf.write_size());
				xw.Begin(GAIA::XML::XML_NODE_CONTAINER, "RootNode");
				{
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						GAIA::CH szTempIndexX[32];
						GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

						GAIA::CH szTempName[32] = "Prop";
						GAIA::ALGO::gstrcat(szTempName, szTempIndexX);

						GAIA::CH szTempValue[32] = "Value";
						GAIA::ALGO::gstrcat(szTempValue, szTempIndexX);

						xw.Write(GAIA::XML::XML_NODE_NAME, szTempName);
						xw.Write(GAIA::XML::XML_NODE_VALUE, szTempValue);
					}
				}
				xw.End();

				GAIA::NUM sWriteSize = xw.GetWriteSize();

				strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
				if(ENABLE_DEBUG_OUTPUT)
					TLOG(strResult.fptr());
				if(strResult != "<RootNode Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/>")
					TERROR;
			}

			// Depth3 test.
			{
				xw.SetBuffer(buf.fptr(), buf.write_size());
				xw.Begin(GAIA::XML::XML_NODE_MULTICONTAINER, "RootNode");
				{
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						GAIA::CH szTempIndexX[32];
						GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

						GAIA::CH szTempNode[32] = "MNode";
						GAIA::ALGO::gstrcat(szTempNode, szTempIndexX);

						xw.Begin(GAIA::XML::XML_NODE_MULTICONTAINER, szTempNode);
						{
							for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
							{
								GAIA::CH szTempIndexY[32];
								GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

								GAIA::CH szTempName[32] = "Prop";
								GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

								GAIA::CH szTempValue[32] = "Value";
								GAIA::ALGO::gstrcat(szTempValue, szTempIndexY);

								xw.Write(GAIA::XML::XML_NODE_NAME, szTempName);
								xw.Write(GAIA::XML::XML_NODE_VALUE, szTempValue);
							}

							for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
							{
								GAIA::CH szTempIndexY[32];
								GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

								GAIA::CH szTempName[32] = "Node";
								GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

								xw.Begin(GAIA::XML::XML_NODE_CONTAINER, szTempName);
								{
									for(GAIA::NUM z = 0; z < SAMPLE_COUNT; ++z)
									{
										GAIA::CH szTempIndexZ[32];
										GAIA::ALGO::castv(z, szTempIndexZ, sizeof(szTempIndexZ));

										GAIA::CH szTempName[32] = "Prop";
										GAIA::ALGO::gstrcat(szTempName, szTempIndexZ);

										GAIA::CH szTempValue[32] = "Value";
										GAIA::ALGO::gstrcat(szTempValue, szTempIndexZ);

										xw.Write(GAIA::XML::XML_NODE_NAME, szTempName);
										xw.Write(GAIA::XML::XML_NODE_VALUE, szTempValue);
									}
								}
								xw.End();
							}
						}
						xw.End();
					}
				}
				xw.End();

				GAIA::NUM sWriteSize = xw.GetWriteSize();

				strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
				if(ENABLE_DEBUG_OUTPUT)
					TLOG(strResult.fptr());
				if(strResult != "<RootNode><MNode0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"><Node0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/></MNode0><MNode1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"><Node0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/></MNode1><MNode2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"><Node0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/></MNode2></RootNode>")
					TERROR;
			}

			// Template test.
			{
				xw.SetBuffer(buf.fptr(), buf.write_size());
				xw.Begin(GAIA::XML::XML_NODE_MULTICONTAINER, "RootNode");
				{
					for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
					{
						GAIA::CH szTempIndexX[32];
						GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

						GAIA::CH szTempNode[32] = "MNode";
						GAIA::ALGO::gstrcat(szTempNode, szTempIndexX);

						xw.Begin(GAIA::XML::XML_NODE_MULTICONTAINER, szTempNode);
						{
							for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
							{
								GAIA::CH szTempIndexY[32];
								GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

								GAIA::WCH szTempName[32] = L"Prop";
								GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

								xw << szTempName << y;
							}

							for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
							{
								GAIA::CH szTempIndexY[32];
								GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

								GAIA::CH szTempName[32] = "Node";
								GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

								xw.Begin(GAIA::XML::XML_NODE_CONTAINER, szTempName);
								{
									for(GAIA::NUM z = 0; z < SAMPLE_COUNT; ++z)
									{
										GAIA::CH szTempIndexZ[32];
										GAIA::ALGO::castv(z, szTempIndexZ, sizeof(szTempIndexZ));

										GAIA::CH szTempName[32] = "Prop";
										GAIA::ALGO::gstrcat(szTempName, szTempIndexZ);

										GAIA::CH szTempValue[32] = "Value";
										GAIA::ALGO::gstrcat(szTempValue, szTempIndexZ);

										xw << szTempName << z;
									}
								}
								xw.End();
							}
						}
						xw.End();
					}
				}
				xw.End();

				GAIA::NUM sWriteSize = xw.GetWriteSize();

				strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
				if(ENABLE_DEBUG_OUTPUT)
					TLOG(strResult.fptr());
				if(strResult != "<RootNode><MNode0 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"><Node0 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node1 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node2 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/></MNode0><MNode1 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"><Node0 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node1 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node2 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/></MNode1><MNode2 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"><Node0 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node1 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node2 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/></MNode2></RootNode>")
					TERROR;
			}
		}
	}
}
