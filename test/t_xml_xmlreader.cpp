#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_xml_xmlreader(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM TEST_TIMES = 10000;
		static const GAIA::NUM SAMPLE_COUNT = 3;

		GAIA::XML::XmlReaderA xr;

		for(GAIA::NUM x = 0; x < TEST_TIMES; ++x)
		{
			// Depth1 test.
			{
				static const GAIA::CH SOURCE_COMPACT[] = "<RootNode Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/>";
				xr.SetBuffer(SOURCE_COMPACT, sizeof(SOURCE_COMPACT) - 1);
				GTRY
				{
					GAIA::XML::XML_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;

					psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

					psz = xr.Begin(nt, sNodeNameLen);
					TAST(psz != GNIL);
					TAST(sNodeNameLen == GAIA::ALGO::gstrlen("RootNode"));
					TAST(GAIA::ALGO::gstrequal(psz, "RootNode", sNodeNameLen));
					TAST(nt == GAIA::XML::XML_NODE_CONTAINER);
					{
						psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

						for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
						{
							GAIA::CH szTempIndexY[32];
							GAIA::ALGO::castv(x, szTempIndexY, sizeof(szTempIndexY));

							GAIA::CH szTempName[32] = "Prop";
							GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

							GAIA::CH szTempValue[32] = "Value";
							GAIA::ALGO::gstrcat(szTempValue, szTempIndexY);

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

							psz = xr.Read(nt, sNodeNameLen);
							TAST(psz != GNIL);
							TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
							TAST(GAIA::ALGO::gstrequal(szTempName, psz, sNodeNameLen));

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

							psz = xr.Read(nt, sNodeNameLen);
							TAST(psz != GNIL);
							TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempValue));
							TAST(GAIA::ALGO::gstrequal(szTempValue, psz, sNodeNameLen));

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
						}
					}
					psz = xr.End(&nt);
					TAST(nt == GAIA::XML::XML_NODE_CONTAINER);
					TAST(psz != GNIL);
					TAST(*psz == '/');
				}
				GCATCHALL
				{
					TERROR;
				}

				static const GAIA::CH SOURCE_USERWRITE[] = "<RootNode\n\tProp0=\"Value0\" \t Prop1=\"Value1\"\n\t Prop2=\"Value2\"\n\n  />";
				xr.SetBuffer(SOURCE_USERWRITE, sizeof(SOURCE_USERWRITE) - 1);
				GTRY
				{
					GAIA::XML::XML_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;

					psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

					psz = xr.Begin(nt, sNodeNameLen);
					TAST(psz != GNIL);
					TAST(sNodeNameLen == GAIA::ALGO::gstrlen("RootNode"));
					TAST(GAIA::ALGO::gstrequal(psz, "RootNode", sNodeNameLen));
					TAST(nt == GAIA::XML::XML_NODE_CONTAINER);
					{
						psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

						for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
						{
							GAIA::CH szTempIndexX[32];
							GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

							GAIA::CH szTempName[32] = "Prop";
							GAIA::ALGO::gstrcat(szTempName, szTempIndexX);

							GAIA::CH szTempValue[32] = "Value";
							GAIA::ALGO::gstrcat(szTempValue, szTempIndexX);

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

							psz = xr.Read(nt, sNodeNameLen);
							TAST(psz != GNIL);
							TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
							TAST(GAIA::ALGO::gstrequal(szTempName, psz, sNodeNameLen));

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

							psz = xr.Read(nt, sNodeNameLen);
							TAST(psz != GNIL);
							TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempValue));
							TAST(GAIA::ALGO::gstrequal(szTempValue, psz, sNodeNameLen));

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
						}
					}
					psz = xr.End(&nt);
					TAST(nt == GAIA::XML::XML_NODE_CONTAINER);
					TAST(psz != GNIL);
					TAST(*psz == '/');
				}
				GCATCHALL
				{
					TERROR;
				}
			}

			// Depth3 test.
			{
				static const GAIA::CH SOURCE[] = "<RootNode><MNode0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"><Node0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/></MNode0><MNode1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"><Node0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/></MNode1><MNode2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"><Node0 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node1 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/><Node2 Prop0=\"Value0\" Prop1=\"Value1\" Prop2=\"Value2\"/></MNode2></RootNode>";
				xr.SetBuffer(SOURCE, sizeof(SOURCE) - 1);
				GTRY
				{
					GAIA::XML::XML_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;

					psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

					psz = xr.Begin(nt, sNodeNameLen);
					TAST(psz != GNIL);
					TAST(sNodeNameLen == GAIA::ALGO::gstrlen("RootNode"));
					TAST(GAIA::ALGO::gstrequal(psz, "RootNode", sNodeNameLen));
					TAST(nt == GAIA::XML::XML_NODE_MULTICONTAINER);
					{
						psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

						for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
						{
							GAIA::CH szTempIndexX[32];
							GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

							GAIA::CH szTempNode[32] = "MNode";
							GAIA::ALGO::gstrcat(szTempNode, szTempIndexX);

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

							psz = xr.Begin(nt, sNodeNameLen);
							TAST(psz != GNIL);
							TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempNode));
							TAST(GAIA::ALGO::gstrequal(psz, szTempNode, sNodeNameLen));
							TAST(nt == GAIA::XML::XML_NODE_MULTICONTAINER);
							{
								psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

								for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
								{
									GAIA::CH szTempIndexY[32];
									GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

									GAIA::CH szTempName[32] = "Prop";
									GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

									GAIA::CH szTempValue[32] = "Value";
									GAIA::ALGO::gstrcat(szTempValue, szTempIndexY);

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

									psz = xr.Read(nt, sNodeNameLen);
									TAST(psz != GNIL);
									TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
									TAST(GAIA::ALGO::gstrequal(szTempName, psz, sNodeNameLen));

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

									psz = xr.Read(nt, sNodeNameLen);
									TAST(psz != GNIL);
									TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempValue));
									TAST(GAIA::ALGO::gstrequal(szTempValue, psz, sNodeNameLen));
								}

								for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
								{
									GAIA::CH szTempIndexY[32];
									GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

									GAIA::CH szTempName[32] = "Node";
									GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

									psz = xr.Begin(nt, sNodeNameLen);
									TAST(psz != GNIL);
									TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
									TAST(GAIA::ALGO::gstrequal(psz, szTempName, sNodeNameLen));
									TAST(nt == GAIA::XML::XML_NODE_CONTAINER);
									{
										psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

										for(GAIA::NUM z = 0; z < SAMPLE_COUNT; ++z)
										{
											GAIA::CH szTempIndexZ[32];
											GAIA::ALGO::castv(z, szTempIndexZ, sizeof(szTempIndexZ));

											GAIA::CH szTempName[32] = "Prop";
											GAIA::ALGO::gstrcat(szTempName, szTempIndexZ);

											GAIA::CH szTempValue[32] = "Value";
											GAIA::ALGO::gstrcat(szTempValue, szTempIndexZ);

											psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

											psz = xr.Read(nt, sNodeNameLen);
											TAST(psz != GNIL);
											TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
											TAST(GAIA::ALGO::gstrequal(szTempName, psz, sNodeNameLen));

											psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

											psz = xr.Read(nt, sNodeNameLen);
											TAST(psz != GNIL);
											TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempValue));
											TAST(GAIA::ALGO::gstrequal(szTempValue, psz, sNodeNameLen));

											psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
										}
									}
									psz = xr.End(&nt);
									TAST(nt == GAIA::XML::XML_NODE_CONTAINER);
									TAST(psz != GNIL);
									TAST(*psz == '/');

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
								}
							}
							psz = xr.End(&nt);
							TAST(nt == GAIA::XML::XML_NODE_MULTICONTAINER);
							TAST(psz != GNIL);
							TAST(*psz != '/');

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
						}
					}
					psz = xr.End(&nt);
					TAST(nt == GAIA::XML::XML_NODE_MULTICONTAINER);
					TAST(psz != GNIL);
					TAST(*psz != '/');
				}
				GCATCHALL
				{
					TERROR;
				}
			}

			// Template test.
			{
				static const GAIA::CH SOURCE[] = "<RootNode><MNode0 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"><Node0 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node1 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node2 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/></MNode0><MNode1 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"><Node0 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node1 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node2 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/></MNode1><MNode2 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"><Node0 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node1 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/><Node2 Prop0=\"0\" Prop1=\"1\" Prop2=\"2\"/></MNode2></RootNode>";
				xr.SetBuffer(SOURCE, sizeof(SOURCE) - 1);
				GTRY
				{
					GAIA::XML::XML_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;

					psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

					psz = xr.Begin(nt, sNodeNameLen);
					TAST(psz != GNIL);
					TAST(sNodeNameLen == GAIA::ALGO::gstrlen("RootNode"));
					TAST(GAIA::ALGO::gstrequal(psz, "RootNode", sNodeNameLen));
					TAST(nt == GAIA::XML::XML_NODE_MULTICONTAINER);
					{
						psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

						for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
						{
							GAIA::CH szTempIndexX[32];
							GAIA::ALGO::castv(x, szTempIndexX, sizeof(szTempIndexX));

							GAIA::CH szTempNode[32] = "MNode";
							GAIA::ALGO::gstrcat(szTempNode, szTempIndexX);

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

							psz = xr.Begin(nt, sNodeNameLen);
							TAST(psz != GNIL);
							TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempNode));
							TAST(GAIA::ALGO::gstrequal(psz, szTempNode, sNodeNameLen));
							TAST(nt == GAIA::XML::XML_NODE_MULTICONTAINER);
							{
								psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

								for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
								{
									GAIA::CH szTempIndexY[32];
									GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

									GAIA::CH szTempName[32] = "Prop";
									GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

									psz = xr.Read(nt, sNodeNameLen);
									TAST(psz != GNIL);
									TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
									TAST(GAIA::ALGO::gstrequal(szTempName, psz, sNodeNameLen));

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

									GAIA::N32 nValue = -1;
									TAST(xr.ReadValue(nValue) == y);
									TAST(nValue == y);

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
								}

								for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
								{
									GAIA::CH szTempIndexY[32];
									GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

									GAIA::CH szTempName[32] = "Node";
									GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

									psz = xr.Begin(nt, sNodeNameLen);
									TAST(psz != GNIL);
									TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
									TAST(GAIA::ALGO::gstrequal(psz, szTempName, sNodeNameLen));
									TAST(nt == GAIA::XML::XML_NODE_CONTAINER);
									{
										psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

										for(GAIA::NUM z = 0; z < SAMPLE_COUNT; ++z)
										{
											GAIA::CH szTempIndexZ[32];
											GAIA::ALGO::castv(z, szTempIndexZ, sizeof(szTempIndexZ));

											GAIA::CH szTempName[32] = "Prop";
											GAIA::ALGO::gstrcat(szTempName, szTempIndexZ);

											psz = xr.Peek(nt, sNodeNameLen); // Mix operation.

											psz = xr.Read(nt, sNodeNameLen);
											TAST(psz != GNIL);
											TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
											TAST(GAIA::ALGO::gstrequal(szTempName, psz, sNodeNameLen));

											GAIA::N32 nValue = -1;
											TAST(xr.ReadValue(nValue) == z);
											TAST(nValue == z);

											psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
										}
									}
									psz = xr.End(&nt);
									TAST(nt == GAIA::XML::XML_NODE_CONTAINER);
									TAST(psz != GNIL);
									TAST(*psz == '/');

									psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
								}
							}
							psz = xr.End(&nt);
							TAST(nt == GAIA::XML::XML_NODE_MULTICONTAINER);
							TAST(psz != GNIL);
							TAST(*psz != '/');

							psz = xr.Peek(nt, sNodeNameLen); // Mix operation.
						}
					}
					psz = xr.End(&nt);
					TAST(nt == GAIA::XML::XML_NODE_MULTICONTAINER);
					TAST(psz != GNIL);
					TAST(*psz != '/');
				}
				GCATCHALL
				{
					TERROR;
				}
			}
		}
	}
}
