#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_json_jsonreader(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM TEST_TIMES = 10000;

		GAIA::JSON::JsonReaderA jr;

		for(GAIA::NUM x = 0; x < TEST_TIMES; ++x)
		{
			// Depth1 test.
			{
				static const GAIA::CH SOURCE_COMPACT[] = "{\"a\":\"1\",\"b\":\"2\",\"c\":3}";
				jr.SetBuffer(SOURCE_COMPACT, sizeof(SOURCE_COMPACT) - 1);
				GTRY
				{
					GAIA::JSON::JSON_NODE nt;
					GAIA::NUM sNodeNameLen;
					const GAIA::CH* psz;
					psz = jr.Begin(nt, sNodeNameLen);
					{
						TAST(psz != GNIL);
						TAST(*psz == '{');
						TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
						TAST(sNodeNameLen == 0);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'a');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '1');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'b');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '2');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'c');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '3');
					}
					psz = jr.End();
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
					psz = jr.Begin(nt, sNodeNameLen);
					{
						TAST(psz != GNIL);
						TAST(*psz == '{');
						TAST(nt == GAIA::JSON::JSON_NODE_CONTAINER);
						TAST(sNodeNameLen == 0);

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'a');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '1');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'b');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '2');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_NAME);
						TAST(sNodeNameLen == 1);
						TAST(*psz == 'c');

						psz = jr.Read(nt, sNodeNameLen);
						TAST(psz != GNIL);
						TAST(nt == GAIA::JSON::JSON_NODE_VALUE);
						TAST(sNodeNameLen == 1);
						TAST(*psz == '3');
					}
					psz = jr.End();
					TAST(psz != GNIL);
					TAST(*psz == '}');
				}
				GCATCHALL
				{
					TERROR;
				}
			}

			// Depth2 test.
			{

			}

			// Template test.
			{

			}
		}
	}
}
