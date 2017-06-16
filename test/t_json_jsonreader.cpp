#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_json_jsonreader(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM TEST_TIMES = 1000;
		static const GAIA::NUM SAMPLE_COUNT = 3;

		GAIA::JSON::JsonReaderA jr;

		for(GAIA::NUM x = 0; x < TEST_TIMES; ++x)
		{
			// Depth1 test.
			{
				static const GAIA::CH SOURCE_COMPACT[] = "{\"a\":\"1\",\"b\":\"-2\",\"c\":-3}";
				jr.SetBuffer(SOURCE_COMPACT, sizeof(SOURCE_COMPACT) - 1);
				GTRY
				{
					GAIA::JSON::JSON_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;

					psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
					GAST(psz != GNIL);

					psz = jr.Begin(nt, sNodeNameLen);
					TAST(psz != GNIL);
					TAST(*psz == '{');
					TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
					TAST(sNodeNameLen == 0);
					{
						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'a');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '1');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'b');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 2);
						TAST(psz[0] == '-' && psz[1] == '2');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'c');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 2);
						TAST(psz[0] == '-' && psz[1] == '3');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);
					}
					psz = jr.End(&nt);
					TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
					TAST(psz != GNIL);
					TAST(*psz == '}');
				}
				GCATCHALL
				{
					TERROR;
				}

				static const GAIA::CH SOURCE_USERWRITE[] = " { \n\t\"a\": \"1\",\n\"b\":\"2\" \n\t\n\"c\":3} \n";
				jr.SetBuffer(SOURCE_USERWRITE, sizeof(SOURCE_USERWRITE) - 1);
				GTRY
				{
					GAIA::JSON::JSON_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;

					psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
					GAST(psz != GNIL);

					psz = jr.Begin(nt, sNodeNameLen);
					TAST(psz != GNIL);
					TAST(*psz == '{');
					TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
					TAST(sNodeNameLen == 0);
					{
						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'a');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '1');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'b');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '2');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'c');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '3');

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);
					}
					psz = jr.End(&nt);
					TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
					TAST(psz != GNIL);
					TAST(*psz == '}');
				}
				GCATCHALL
				{
					TERROR;
				}
			}

			// Depth3 test.
			{
				static const GAIA::CH SOURCE[] = "[{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\",\"Node0\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node1\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node2\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"}},{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\",\"Node0\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node1\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node2\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"}},{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\",\"Node0\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node1\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"},\"Node2\":{\"Prop0\":\"Value0\",\"Prop1\":\"Value1\",\"Prop2\":\"Value2\"}}]";
				jr.SetBuffer(SOURCE, sizeof(SOURCE) - 1);
				GTRY
				{
					GAIA::JSON::JSON_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;

					psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
					GAST(psz != GNIL);

					psz = jr.Begin(nt, sNodeNameLen);
					TAST(psz != GNIL);
					TAST(*psz == '[');
					TAST(nt == GAIA::JSON::JSON_NODE_MULTICONTAINER);
					TAST(sNodeNameLen == 0);
					{
						for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
						{
							psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
							GAST(psz != GNIL);

							psz = jr.Begin(nt, sNodeNameLen);
							TAST(psz != GNIL);
							TAST(*psz == '{');
							TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
							TAST(sNodeNameLen == 0);
							{
								psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
								GAST(psz != GNIL);

								for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
								{
									GAIA::CH szTempIndexY[32];
									GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

									GAIA::CH szTempName[32] = "Prop";
									GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

									GAIA::CH szTempValue[32] = "Value";
									GAIA::ALGO::gstrcat(szTempValue, szTempIndexY);

									psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
									GAST(psz != GNIL);

									psz = jr.Read(nt, sNodeNameLen);
									TAST(psz != GNIL);
									TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
									TAST(GAIA::ALGO::gstrcmp(szTempName, psz, sNodeNameLen) == 0);

									psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
									GAST(psz != GNIL);

									psz = jr.Read(nt, sNodeNameLen);
									TAST(psz != GNIL);
									TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempValue));
									TAST(GAIA::ALGO::gstrcmp(szTempValue, psz, sNodeNameLen) == 0);

									psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
									GAST(psz != GNIL);
								}

								for(GAIA::NUM y = 0; y < SAMPLE_COUNT; ++y)
								{
									GAIA::CH szTempIndexY[32];
									GAIA::ALGO::castv(y, szTempIndexY, sizeof(szTempIndexY));

									GAIA::CH szTempName[32] = "Node";
									GAIA::ALGO::gstrcat(szTempName, szTempIndexY);

									psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
									GAST(psz != GNIL);

									psz = jr.Begin(nt, sNodeNameLen);
									TAST(psz != GNIL);
									TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
									TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
									TAST(GAIA::ALGO::gstrcmp(psz, szTempName, sNodeNameLen) == 0);
									{
										for(GAIA::NUM z = 0; z < SAMPLE_COUNT; ++z)
										{
											GAIA::CH szTempIndexZ[32];
											GAIA::ALGO::castv(z, szTempIndexZ, sizeof(szTempIndexZ));

											GAIA::CH szTempName[32] = "Prop";
											GAIA::ALGO::gstrcat(szTempName, szTempIndexZ);

											GAIA::CH szTempValue[32] = "Value";
											GAIA::ALGO::gstrcat(szTempValue, szTempIndexZ);

											psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
											GAST(psz != GNIL);

											psz = jr.Read(nt, sNodeNameLen);
											TAST(psz != GNIL);
											TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempName));
											TAST(GAIA::ALGO::gstrcmp(szTempName, psz, sNodeNameLen) == 0);

											psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
											GAST(psz != GNIL);

											psz = jr.Read(nt, sNodeNameLen);
											TAST(psz != GNIL);
											TAST(sNodeNameLen == GAIA::ALGO::gstrlen(szTempValue));
											TAST(GAIA::ALGO::gstrcmp(szTempValue, psz, sNodeNameLen) == 0);

											psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
											GAST(psz != GNIL);
										}
									}
									psz = jr.End(&nt);
									TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
									TAST(psz != GNIL);
									TAST(*psz == '}');

									psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
									GAST(psz != GNIL);
								}
							}
							psz = jr.End(&nt);
							TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
							TAST(psz != GNIL);
							TAST(*psz == '}');

							psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
							GAST(psz != GNIL);
						}
					}
					psz = jr.End(&nt);
					TAST(nt == GAIA::JSON::JSON_NODE_MULTICONTAINER);
					TAST(psz != GNIL);
					TAST(*psz == ']');
				}
				GCATCHALL
				{
					TERROR;
				}
			}

			// Template test.
			{
				static const GAIA::CH SOURCE[] = "{\"aa\":1,\"bb\":true,\"cc\":\"12345678123456781234567812345678\"}";
				jr.SetBuffer(SOURCE, sizeof(SOURCE) - 1);
				GTRY
				{
					GAIA::JSON::JSON_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;

					psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
					GAST(psz != GNIL);

					psz = jr.Begin(nt, sNodeNameLen);
					TAST(psz != GNIL);
					TAST(*psz == '{');
					TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
					TAST(sNodeNameLen == 0);
					{
						GAIA::CH szName[64];

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						jr.ReadName(szName);
						TAST(GAIA::ALGO::gstrequal(szName, "aa"));
						GAIA::N32 n;
						n = jr.ReadValue(n);
						TAST(n == 1);

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						jr.ReadName(szName, sizeof(szName));
						TAST(GAIA::ALGO::gstrequal(szName, "bb"));
						GAIA::BL b;
						b = jr.ReadValue(b);
						TAST(b == GAIA::True);

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);

						jr.ReadName(szName, sizeof(szName));
						TAST(GAIA::ALGO::gstrequal(szName, "cc"));
						GAIA::X128 x128;
						x128 = jr.ReadValue(x128);
						GAIA::X128 x128Source;
						x128Source.fromstring("12345678123456781234567812345678");
						TAST(x128 == x128Source);

						psz = jr.Peek(nt, sNodeNameLen); // Mix operation.
						GAST(psz != GNIL);
					}
					psz = jr.End(&nt);
					TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
					TAST(psz != GNIL);
					TAST(*psz == '}');
				}
				GCATCHALL
				{
					TERROR;
				}
			}
		}
		
		// Special test.
		{
			GTRY
			{
				jr.Reset();
				static const GAIA::CH SPECIAL_JSON[] = "{\"postid\":10,\"replyid\":0,\"content\":\"<!DOCTYPE html PUBLIC \\\"-\\/\\/W3C\\/\\/DTD XHTML 1.0 Strict\\/\\/EN\\\" \\\"http:\\/\\/www.w3.org\\/TR\\/xhtml1\\/DTD\\/xhtml1-strict.dtd\\\"><html><head><meta charset=\\\"utf-8\\\"\\/><title>template title<\\/title><\\/head><body><h8><p>&nbsp&nbsp&nbsp&nbsp￦ﾗﾶ￥ﾀﾙ<\\/p><\\/h8><\\/body><\\/html>\",\"pic_infos\":[]}";
				GAIA::JSON::JSON_NODE nt;
				GAIA::NUM sNodeNameLen;
				const GAIA::CH* psz;
				jr.SetBuffer(SPECIAL_JSON, sizeof(SPECIAL_JSON));
				psz = jr.Begin(nt, sNodeNameLen);
				{
					jr.ReadNameByName<GAIA::CH>();
					psz = jr.Read(nt, sNodeNameLen);
					
					jr.ReadNameByName("replyid");
					psz = jr.Read(nt, sNodeNameLen);
					
					psz = jr.Read(nt, sNodeNameLen);
					psz = jr.Read(nt, sNodeNameLen);
					
					psz = jr.Begin(nt, sNodeNameLen);
					{
					}
					psz = jr.End(&nt);
				}
				psz = jr.End(&nt);
			}
			GCATCHBASE
			{
				TERROR;
			}
		}
	}
}
