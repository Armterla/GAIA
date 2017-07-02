#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_db_local(GAIA::LOG::Log& logobj)
	{
		GAIA::CH szFileName[GAIA::MAXPL];
		GAIA::ALGO::gstrcpy(szFileName, g_gaia_appdocdir);
		GAIA::ALGO::gstrcat(szFileName, "test.db");
		
		GAIA::DB::DBLocal db;
		if(!db.Open(szFileName))
			TERROR;
		else
		{
			if(!db.SetCacheSize(1024 * 1024))
				TERROR;
			if(db.GetCacheSize() != 1024 * 1024)
				TERROR;
			if(!db.SetSyncMode(GAIA::DB::DB_SYNC_MODE_FASTEST))
				TERROR;
			if(db.GetSyncMode() != GAIA::DB::DB_SYNC_MODE_FASTEST)
				TERROR;
			if(!db.BeginTransaction())
				TERROR;
			GAIA::DB::DBLocalQuery q(db);
			if(!q.Exec("CREATE TABLE IF NOT EXISTS test_table(k VARCHAR(32) PRIMARY KEY NOT NULL, v INTEGER);"))
				TERROR;
			if(!q.Prepare("INSERT INTO test_table(k, v) VALUES(?, ?);"))
				TERROR;
			for(GAIA::NUM x = 0; x < 10000; ++x)
			{
				GAIA::CH szKey[33];
				GAIA::ALGO::castv(x, szKey, 32);

				if(!q.BindStringA(szKey))
				{
					TERROR;
					break;
				}
				if(!q.BindN32(x))
				{
					TERROR;
					break;
				}
				if(!q.Step())
				{
					TERROR;
					break;
				}
				if(!q.Reset())
				{
					TERROR;
					break;
				}
			}
			if(!db.EndTransaction())
				TERROR;
			if(!q.Prepare("SELECT * FROM test_table ORDER BY v ASC;"))
				TERROR;
			for(GAIA::NUM x = 0; x < 10000; ++x)
			{
				if(!q.Step())
				{
					TERROR;
					break;
				}
				if(q.GetFieldCount() != 2)
				{
					TERROR;
					break;
				}
				if(GAIA::ALGO::gstrcmp(q.GetFieldName(0), "k") != 0)
				{
					TERROR;
					break;
				}
				if(GAIA::ALGO::gstrcmp(q.GetFieldName(1), "v") != 0)
				{
					TERROR;
					break;
				}
				if(!q.SeekField(0))
				{
					TERROR;
					break;
				}
				if(q.GetFieldType() != GAIA::TYPEID_CONSTCHARPOINTER)
				{
					TERROR;
					break;
				}
				if(q.GetFieldType(0) != GAIA::TYPEID_CONSTCHARPOINTER)
				{
					TERROR;
					break;
				}
				q.SeekField(0);
				const GAIA::CH* psz = q.GetStringA();
				GAIA::N32 v = q.GetN32();
				if(x != (GAIA::N32)GAIA::ALGO::acasts(psz))
				{
					TERROR;
					break;
				}
				if(v != x)
				{
					TERROR;
					break;
				}
				if(!q.SeekField(0))
				{
					TERROR;
					break;
				}
				psz = q.GetStringA();
				v = q.GetN32();
				if(x != (GAIA::N32)GAIA::ALGO::acasts(psz))
				{
					TERROR;
					break;
				}
				if(v != x)
				{
					TERROR;
					break;
				}
			}
			if(!q.Reset())
				TERROR;
			if(!q.Exec("DROP TABLE test_table;"))
				TERROR;
			if(!q.Close())
				TERROR;
			if(!db.Close())
				TERROR;
			GAIA::FSYS::Dir dir;
			if(!dir.RemoveFile(szFileName))
				TERROR;
		}
	}
}
