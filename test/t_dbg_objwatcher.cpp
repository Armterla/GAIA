#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_dbg_objwatcher(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SAMPLE_COUNT = 4;

		GAIA::DBG::ObjWatcher ow;
		ow.EnableLog(GAIA::True);
		TAST(ow.GetLog() == GNIL);

		GAIA::Object o1, o2, o3, o4;
		GAIA::X128 u1, u2, u3, u4;

		for(GAIA::NUM x = 0; x < SAMPLE_COUNT; ++x)
		{
			u1.uuid();
			u2.uuid();
			u3.uuid();
			u4.uuid();

			TAST(!ow.IsExist(&ow));

			TAST(ow.Begin(&o1, &u1, 1));
			TAST(ow.IsBegin(&o1));
			TAST(ow.IsBegin(&o1, &u1));
			TAST(ow.IsBegin(&o1, &u1, 1));
			TAST(ow.Update(&o1, "Text 1"));
			TAST(ow.Update(&o1, "Text 2", &u1));
			TAST(ow.Update(&o1, "Text 3", &u1, 1));
			TAST(ow.End(&o1, &u1, 1));

			TAST(ow.Begin(&o2, &u2, 1));
			TAST(ow.IsBegin(&o2));
			TAST(ow.IsBegin(&o2, &u2));
			TAST(ow.IsBegin(&o2, &u2, 1));
			TAST(ow.Update(&o2, "Text 1"));
			TAST(ow.Update(&o2, "Text 2", &u2));
			TAST(ow.Update(&o2, "Text 3", &u2, 1));
			TAST(ow.End(&o2, &u2, 1));

			TAST(ow.Begin(&o3, &u3, 2));
			TAST(ow.IsBegin(&o3));
			TAST(ow.IsBegin(&o3, &u3));
			TAST(ow.IsBegin(&o3, &u3, 2));
			TAST(ow.Update(&o3, "Text 1"));
			TAST(ow.Update(&o3, "Text 2", &u3));
			TAST(ow.Update(&o3, "Text 3", &u3, 2));
			TAST(ow.End(&o3, &u3, 2));

			TAST(ow.Begin(&o4, &u4, 2));
			TAST(ow.IsBegin(&o4));
			TAST(ow.IsBegin(&o4, &u4));
			TAST(ow.IsBegin(&o4, &u4, 2));
			TAST(ow.Update(&o4, "Text 1"));
			TAST(ow.Update(&o4, "Text 2", &u4));
			TAST(ow.Update(&o4, "Text 3", &u4, 2));
			TAST(ow.End(&o4, &u4, 2));

			TAST(!ow.IsBegin(&o1));
			TAST(!ow.IsBegin(&o2, &u2));
			TAST(!ow.IsBegin(&o3, &u3, 2));
			TAST(!ow.IsBegin(&o4, &u4));

			TAST(ow.IsExist(&o1));
			TAST(ow.IsExist(&o2, &u2));
			TAST(ow.IsExist(&o3, &u3, 2));
			TAST(ow.IsExist(&o4, &u4));

			TAST(!ow.IsExist(&ow));

			ow.Recycle();

			GAIA::CTN::Vector<GAIA::DBG::ObjWatcher::Record> listResult;

			TAST(ow.CollectRecords(listResult));
			if(listResult.size() != (x + 1) * 5 * 4)
			{
				TERROR;
				break;
			}

			TAST(ow.CollectRecords(listResult, &o1, GNIL, 1));
			if(listResult.size() != (x + 1) * 5)
			{
				TERROR;
				break;
			}

			TAST(ow.CollectRecords(listResult, &o1, &u1, 1));
			if(listResult.size() != 5)
			{
				TERROR;
				break;
			}

			TAST(!ow.CollectRecords(listResult, &o1, &u1, 2));
			if(listResult.size() != 0)
			{
				TERROR;
				break;
			}
		}
	}
}
