#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_xml_xmlwriter(GAIA::LOG::Log& logobj)
	{
		static const GAIA::BL ENABLE_DEBUG_OUTPUT = GAIA::True;
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

		GAIA::CTN::Buffer buf;
		buf.resize(1024 * 1024 * 4);

		// Depth1 test.
		{
			xw.SetBuffer(buf.fptr(), buf.write_size());
			xw.BeginWriteNode(GAIA::XML::XML_NODE_CONTAINER, "RootNode");
			{
				for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				{
					GAIA::CH szTempIndexX[32];
					GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

					GAIA::CH szTempName[32] = "Prop";
					GAIA::ALGO::gstrcat(szTempName, szTempIndexX);

					GAIA::CH szTempValue[32] = "Value";
					GAIA::ALGO::gstrcat(szTempValue, szTempIndexX);

					xw.WriteNode(GAIA::XML::XML_NODE_NAME, szTempName);
					xw.WriteNode(GAIA::XML::XML_NODE_VALUE, szTempName);
				}
			}
			xw.EndWriteNode();

			GAIA::NUM sWriteSize = xw.GetWriteSize();

			GAIA::CTN::AString strResult;
			strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
			if(ENABLE_DEBUG_OUTPUT)
				TLOG(strResult.fptr());
			if(strResult != "<RootNode Prop0=\"Prop0\" Prop1=\"Prop1\" Prop2=\"Prop2\"/>")
				TERROR;
		}

		// Depth2 test.
		{
			xw.SetBuffer(buf.fptr(), buf.write_size());
			xw.BeginWriteNode(GAIA::XML::XML_NODE_MULTICONTAINER, "RootNode");
			{
				for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
				{
					GAIA::CH szTempIndexX[32];
					GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

					GAIA::CH szTempNode[32] = "Node";
					GAIA::ALGO::gstrcat(szTempNode, szTempIndexX);


					xw.BeginWriteNode(GAIA::XML::XML_NODE_CONTAINER, szTempNode);
					{
						for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
						{
							GAIA::CH szTempIndexY[32];
							GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

							GAIA::CH szTempName[32] = "Prop";
							GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

							GAIA::CH szTempValue[32] = "Value";
							GAIA::ALGO::gstrcat(szTempValue, szTempIndexY);

							xw.WriteNode(GAIA::XML::XML_NODE_NAME, szTempName);
							xw.WriteNode(GAIA::XML::XML_NODE_VALUE, szTempValue);
						}
					}
					xw.EndWriteNode();
				}
			}
			xw.EndWriteNode();

			GAIA::NUM sWriteSize = xw.GetWriteSize();

			GAIA::CTN::AString strResult;
			strResult.assign((const GAIA::CH*)buf.fptr(), sWriteSize);
			if(ENABLE_DEBUG_OUTPUT)
				TLOG(strResult.fptr());
			if(strResult != "<RootNode><Node0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/></RootNode>")
				TERROR;
		}
	}
}
