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
				static const GAIA::CH SOURCE[] = "{\"a\":\"1\"}";
				jr.SetBuffer(SOURCE, sizeof(SOURCE) - 1);
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
