#include <gaia_sysconfig.h>

#ifdef GAIA_FEATURE_SQLITE
#	include <sqlite3.h>
#endif

#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_sync_lockfree.h>
#include <gaia_ctn_string.h>
#include <gaia_ctn_charsstring.h>
#include <gaia_db_dblocal.h>
#include <gaia_type_impl.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace DB
	{
		GAIA::BL DBLocalCheckSQL(const GAIA::CH* pszSQL)
		{
			const GAIA::CH* pszTempSQL = pszSQL;
			while(*pszTempSQL != '\0')
			{
				if(GAIA::ALGO::isblank(*pszTempSQL) || GAIA::ALGO::isspecial(*pszTempSQL))
					++pszTempSQL;
				else
					break;
			}
			if(GAIA::ALGO::gstrstartwithi(pszTempSQL, "BEGIN"))
				return GAIA::False;
			return GAIA::True;
		}
		class DBLocalQueryContext : public GAIA::Base
		{
		public:
			DBLocalQueryContext()
			{
				pDB = GNIL;
			#ifdef GAIA_FEATURE_SQLITE
				pSqliteSTMT = GNIL;
			#endif
				sBindCursor = 0;
				sFieldCursor = 0;
				bSelectSQL = GAIA::False;
			}
		public:
			DBLocal* pDB;
		#ifdef GAIA_FEATURE_SQLITE
			sqlite3_stmt* pSqliteSTMT;
		#endif
			GAIA::NUM sBindCursor;
			GAIA::NUM sFieldCursor;
			GAIA::CTN::AString strSQL;
			GAIA::BL bSelectSQL;
		};
		class DBLocalContext : public GAIA::Base
		{
		public:
			DBLocalContext()
			{
			#ifdef GAIA_FEATURE_SQLITE
				pSqliteDB = GNIL;
			#endif
				syncmode = GAIA::DB::DB_SYNC_MODE_INVALID;
				sCacheSize = GINVALID;
				nTransactionRefCount = 0;
				sTransactionMethod = 0; // 0 means not init, 1 means commit, 2 means rollback.
			}
		public:
		#ifdef GAIA_FEATURE_SQLITE
			sqlite3* pSqliteDB;
		#endif
			GAIA::CTN::AString strDBName;
			GAIA::DB::DB_SYNC_MODE syncmode;
			GAIA::NUM sCacheSize;
			GAIA::SYNC::LockFree lrTransaction;
			GAIA::N64 nTransactionRefCount;
			GAIA::NUM sTransactionMethod;
		};
		DBLocalQuery::DBLocalQuery(GAIA::DB::DBLocal& db)
		{
		#ifdef GAIA_DEBUG_INSTANCECOUNT
			GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_DBLOCALQUERY, +1);
		#endif
			
			m_ctx = gnew DBLocalQueryContext;
			m_ctx->pDB = &db;
		}
		DBLocalQuery::~DBLocalQuery()
		{
			if(this->IsQueryed())
				this->Close();
			gdel m_ctx;
			
		#ifdef GAIA_DEBUG_INSTANCECOUNT
			GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_DBLOCALQUERY, -1);
		#endif
		}
		GAIA::DB::DBLocal* DBLocalQuery::GetDatabase()
		{
			return m_ctx->pDB;
		}
		GAIA::BL DBLocalQuery::IsQueryed() const
		{
		#ifdef GAIA_FEATURE_SQLITE
			return m_ctx->pSqliteSTMT != GNIL;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		const GAIA::CH* DBLocalQuery::GetSQL() const
		{
			return m_ctx->strSQL.fptr();
		}
		GAIA::BL DBLocalQuery::Close()
		{
			if(!this->IsQueryed())
				return GAIA::False;
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_finalize(m_ctx->pSqliteSTMT) != SQLITE_OK)
				return GAIA::False;
			m_ctx->pSqliteSTMT = GNIL;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
			
		#ifdef GAIA_DEBUG_INSTANCECOUNT
			GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_PREPAREDDBLOCALQUERY, -1);
		#endif
			
			return GAIA::True;
		}
		GAIA::BL DBLocalQuery::Prepare(const GAIA::CH* pszSQL)
		{
			GPCHR_NULLSTR_RET(pszSQL, GAIA::False);
			if(!DBLocalCheckSQL(pszSQL))
				return GAIA::False;

			m_ctx->sBindCursor = 0;
			m_ctx->strSQL.clear();
			m_ctx->bSelectSQL = GAIA::False;

			// Close.
			if(this->IsQueryed())
			{
				if(!this->Close())
					return GAIA::False;
			}

			// Create.
			if(pszSQL != GNIL)
			{
			#ifdef GAIA_FEATURE_SQLITE
				if(sqlite3_prepare_v2(m_ctx->pDB->m_ctx->pSqliteDB, pszSQL, -1, &m_ctx->pSqliteSTMT, GNIL) != SQLITE_OK)
					return GAIA::False;
				m_ctx->strSQL = pszSQL;
				const GAIA::CH* pszTemp = pszSQL;
				while(GAIA::ALGO::isblank(*pszTemp) || GAIA::ALGO::isspecial(*pszTemp))
					++pszTemp;
				if(GAIA::ALGO::gstrstartwithi(pszTemp, "SELECT"))
					m_ctx->bSelectSQL = GAIA::True;
				else if(GAIA::ALGO::gstrstartwithi(pszTemp, "PRAGMA"))
				{
					if(GAIA::ALGO::gstrch(pszTemp, '=') == GNIL)
						m_ctx->bSelectSQL = GAIA::True;
				}
			#else
				GTHROW_RET(NotSupport, GAIA::False);
			#endif
			}

		#ifdef GAIA_DEBUG_INSTANCECOUNT
			GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_PREPAREDDBLOCALQUERY, +1);
		#endif
			
			return GAIA::True;
		}
		GAIA::BL DBLocalQuery::Exec(const GAIA::CH* pszSQL)
		{
			GPCHR_NULLSTR_RET(pszSQL, GAIA::False);
			if(!DBLocalCheckSQL(pszSQL))
				return GAIA::False;
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_exec(m_ctx->pDB->m_ctx->pSqliteDB, pszSQL, GNIL, GNIL, GNIL) != SQLITE_OK)
				return GAIA::False;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
			return GAIA::True;
		}
		GAIA::BL DBLocalQuery::BindNull(GAIA::NUM sIndex)
		{
			if(!this->IsQueryed())
				return GAIA::False;
			if(sIndex != GINVALID)
				m_ctx->sBindCursor = sIndex;
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_bind_null(m_ctx->pSqliteSTMT, m_ctx->sBindCursor + 1) != SQLITE_OK)
				return GAIA::False;
			m_ctx->sBindCursor++;
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocalQuery::BindN32(GAIA::N32 n, GAIA::NUM sIndex)
		{
			if(!this->IsQueryed())
				return GAIA::False;
			if(sIndex != GINVALID)
				m_ctx->sBindCursor = sIndex;
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_bind_int(m_ctx->pSqliteSTMT, m_ctx->sBindCursor + 1, n) != SQLITE_OK)
				return GAIA::False;
			m_ctx->sBindCursor++;
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocalQuery::BindN64(GAIA::N64 l, GAIA::NUM sIndex)
		{
			if(!this->IsQueryed())
				return GAIA::False;
			if(sIndex != GINVALID)
				m_ctx->sBindCursor = sIndex;
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_bind_int64(m_ctx->pSqliteSTMT, m_ctx->sBindCursor + 1, l) != SQLITE_OK)
				return GAIA::False;
			m_ctx->sBindCursor++;
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocalQuery::BindF32(GAIA::F32 f, GAIA::NUM sIndex)
		{
			if(!this->IsQueryed())
				return GAIA::False;
			if(sIndex != GINVALID)
				m_ctx->sBindCursor = sIndex;
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_bind_double(m_ctx->pSqliteSTMT, m_ctx->sBindCursor + 1, (GAIA::F64)f) != SQLITE_OK)
				return GAIA::False;
			m_ctx->sBindCursor++;
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocalQuery::BindF64(GAIA::F64 d, GAIA::NUM sIndex)
		{
			if(!this->IsQueryed())
				return GAIA::False;
			if(sIndex != GINVALID)
				m_ctx->sBindCursor = sIndex;
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_bind_double(m_ctx->pSqliteSTMT, m_ctx->sBindCursor + 1, d) != SQLITE_OK)
				return GAIA::False;
			m_ctx->sBindCursor++;
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocalQuery::BindStringA(const GAIA::CH* psz, GAIA::NUM sIndex, GAIA::BL bStatic)
		{
			if(!this->IsQueryed())
				return GAIA::False;
			if(sIndex != GINVALID)
				m_ctx->sBindCursor = sIndex;
		#ifdef GAIA_FEATURE_SQLITE
			sqlite3_destructor_type destructor;
			if(bStatic)
				destructor = SQLITE_STATIC;
			else
				destructor = SQLITE_TRANSIENT;
			if(sqlite3_bind_text(m_ctx->pSqliteSTMT, m_ctx->sBindCursor + 1, psz, -1, destructor) != SQLITE_OK)
				return GAIA::False;
			m_ctx->sBindCursor++;
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocalQuery::BindStringW(const GAIA::WCH* psz, GAIA::NUM sIndex, GAIA::BL bStatic)
		{
			GAIA::CTN::ACharsString str;
			str.resize(GAIA::ALGO::gstrlen(psz) * 6);
			GAIA::NUM sNewLen = GAIA::LOCALE::w2m(psz, GINVALID, str.fptr(), str.size(), GAIA::CHARSET_TYPE_UTF8);
			str.resize(sNewLen);
			return this->BindStringA(str.fptr(), sIndex, GAIA::False);
		}
		GAIA::BL DBLocalQuery::BindBlob(const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::NUM sIndex, GAIA::BL bStatic)
		{
			if(!this->IsQueryed())
				return GAIA::False;
			if(sIndex != GINVALID)
				m_ctx->sBindCursor = sIndex;
		#ifdef GAIA_FEATURE_SQLITE
			sqlite3_destructor_type destructor;
			if(bStatic)
				destructor = SQLITE_STATIC;
			else
				destructor = SQLITE_TRANSIENT;
			if(sqlite3_bind_blob(m_ctx->pSqliteSTMT, m_ctx->sBindCursor + 1, p, sSize, destructor) != SQLITE_OK)
				return GAIA::False;
			m_ctx->sBindCursor++;
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocalQuery::Reset()
		{
			m_ctx->sBindCursor = 0;
			m_ctx->sFieldCursor = 0;
		#ifdef GAIA_FEATURE_SQLITE
			if(m_ctx->pSqliteSTMT != GNIL)
			{
				if(sqlite3_reset(m_ctx->pSqliteSTMT) != SQLITE_OK)
					return GAIA::False;
			}
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
			return GAIA::True;
		}
		GAIA::NUM DBLocalQuery::GetFieldCount()
		{
			if(!this->IsQueryed())
				return GINVALID;
		#ifdef GAIA_FEATURE_SQLITE
			return sqlite3_column_count(m_ctx->pSqliteSTMT);
		#else
			GTHROW_RET(NotSupport, GINVALID);
		#endif
		}
		GAIA::NUM DBLocalQuery::GetRecordCount()
		{
			return 0;
		}
		const GAIA::CH* DBLocalQuery::GetFieldName(GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				return GNIL;
			if(sColumn < GINVALID)
				return GNIL;
			if(sColumn != GINVALID)
				m_ctx->sFieldCursor = sColumn;
		#ifdef GAIA_FEATURE_SQLITE
			if(sColumn >= sqlite3_column_count(m_ctx->pSqliteSTMT))
				return GNIL;
			const GAIA::CH* pszRet = sqlite3_column_name(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			m_ctx->sFieldCursor++;
			return pszRet;
		#else
			GTHROW_RET(NotSupport, GNIL);
		#endif
		}
		GAIA::TYPEID DBLocalQuery::GetFieldType(GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				return GAIA::TYPEID_INVALID;
			if(sColumn < GINVALID)
				return GAIA::TYPEID_INVALID;
			if(sColumn != GINVALID)
				m_ctx->sFieldCursor = sColumn;
		#ifdef GAIA_FEATURE_SQLITE
			if(sColumn >= sqlite3_column_count(m_ctx->pSqliteSTMT))
				return GAIA::TYPEID_INVALID;
			GAIA::N32 sqlite_type = sqlite3_column_type(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			GAIA::NUM sTypeSize = sqlite3_column_bytes(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			m_ctx->sFieldCursor++;
			if(sqlite_type == SQLITE_INTEGER)
			{
				if(sTypeSize == sizeof(GAIA::N8))
					return GAIA::TYPEID_N8;
				else if(sTypeSize == sizeof(GAIA::N16))
					return GAIA::TYPEID_N16;
				else if(sTypeSize == sizeof(GAIA::N32))
					return GAIA::TYPEID_N32;
				else if(sTypeSize == sizeof(GAIA::N64))
					return GAIA::TYPEID_N64;
				return GAIA::TYPEID_INVALID;
			}
			else if(sqlite_type == SQLITE_FLOAT)
			{
				if(sTypeSize == sizeof(GAIA::F32))
					return GAIA::TYPEID_F32;
				else if(sTypeSize == sizeof(GAIA::F64))
					return GAIA::TYPEID_F32;
				return GAIA::TYPEID_INVALID;
			}
			else if(sqlite_type == SQLITE_TEXT)
				return GAIA::TYPEID_CONSTCHARPOINTER;
			else if(sqlite_type == SQLITE_BLOB)
				return GAIA::TYPEID_CONSTPOINTER;
			else if(sqlite_type == SQLITE_NULL)
				return GAIA::TYPEID_INVALID;
			return GAIA::TYPEID_INVALID;
		#else
			GTHROW_RET(NotSupport, GAIA::TYPEID_INVALID);
		#endif
		}
		GAIA::BL DBLocalQuery::SeekBind(GAIA::NUM sOffset)
		{
			if(!this->IsQueryed())
				return GAIA::False;
			if(sOffset < 0)
				return GAIA::False;
			m_ctx->sBindCursor = sOffset;
			return GAIA::True;
		}
		GAIA::BL DBLocalQuery::SeekField(GAIA::NUM sOffset)
		{
			if(!this->IsQueryed())
				return GAIA::False;
		#ifdef GAIA_FEATURE_SQLITE
			if(sOffset < 0 || sOffset >= sqlite3_column_count(m_ctx->pSqliteSTMT))
				return GAIA::False;
			m_ctx->sFieldCursor = sOffset;
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocalQuery::Step()
		{
			m_ctx->sBindCursor = 0;
			m_ctx->sFieldCursor = 0;
			if(!this->IsQueryed())
				return GAIA::False;
		#ifdef GAIA_FEATURE_SQLITE
			GAIA::N32 nRes = sqlite3_step(m_ctx->pSqliteSTMT);
			if(m_ctx->bSelectSQL)
			{
				if(nRes != SQLITE_ROW)
					return GAIA::False;
			}
			else
			{
				if(nRes != SQLITE_DONE)
					return GAIA::False;
			}
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
			return GAIA::True;
		}
		GAIA::N32 DBLocalQuery::GetN32(GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				GTHROW_RET(Illegal, 0);
			if(sColumn != GINVALID)
				m_ctx->sFieldCursor = sColumn;
		#ifdef GAIA_FEATURE_SQLITE
			GAIA::N32 nRet = sqlite3_column_int(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			m_ctx->sFieldCursor++;
			return nRet;
		#else
			GTHROW_RET(NotSupport, 0);
		#endif
		}
		GAIA::N64 DBLocalQuery::GetN64(GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				GTHROW_RET(Illegal, 0);
			if(sColumn != GINVALID)
				m_ctx->sFieldCursor = sColumn;
		#ifdef GAIA_FEATURE_SQLITE
			GAIA::N64 nRet = sqlite3_column_int64(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			m_ctx->sFieldCursor++;
			return nRet;
		#else
			GTHROW_RET(NotSupport, 0);
		#endif
		}
		GAIA::F32 DBLocalQuery::GetF32(GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				GTHROW_RET(Illegal, 0.0F);
			if(sColumn != GINVALID)
				m_ctx->sFieldCursor = sColumn;
		#ifdef GAIA_FEATURE_SQLITE
			GAIA::F32 fRet = (GAIA::F32)sqlite3_column_double(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			m_ctx->sFieldCursor++;
			return fRet;
		#else
			GTHROW_RET(NotSupport, 0.0F);
		#endif
		}
		GAIA::F64 DBLocalQuery::GetF64(GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				GTHROW_RET(Illegal, 0.0);
			if(sColumn != GINVALID)
				m_ctx->sFieldCursor = sColumn;
		#ifdef GAIA_FEATURE_SQLITE
			GAIA::F64 fRet = sqlite3_column_double(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			m_ctx->sFieldCursor++;
			return fRet;
		#else
			GTHROW_RET(NotSupport, 0.0);
		#endif
		}
		const GAIA::CH* DBLocalQuery::GetStringA(GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				GTHROW_RET(Illegal, GNIL);
			if(sColumn != GINVALID)
				m_ctx->sFieldCursor = sColumn;
		#ifdef GAIA_FEATURE_SQLITE
			const GAIA::CH* pszRet = (const GAIA::CH*)sqlite3_column_text(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			m_ctx->sFieldCursor++;
			return pszRet;
		#else
			GTHROW_RET(NotSupport, GNIL);
		#endif

		}
		const GAIA::WCH* DBLocalQuery::GetStringW(GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				GTHROW_RET(Illegal, GNIL);
			return GNIL;
		}
		const GAIA::GVOID* DBLocalQuery::GetBlob(GAIA::NUM& sSize, GAIA::NUM sColumn)
		{
			if(!this->IsQueryed())
				GTHROW_RET(Illegal, GNIL);
		#ifdef GAIA_FEATURE_SQLITE
			sSize = sqlite3_column_bytes(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			const GAIA::GVOID* pRet = sqlite3_column_blob(m_ctx->pSqliteSTMT, m_ctx->sFieldCursor);
			m_ctx->sFieldCursor++;
			return pRet;
		#else
			sSize = 0;
			GTHROW_RET(NotSupport, GNIL);
		#endif
		}
		const GAIA::CH* DBLocalQuery::GetErrorString() const
		{
			return m_ctx->pDB->GetErrorString();
		}
		DBLocal::DBLocal()
		{
		#ifdef GAIA_DEBUG_INSTANCECOUNT
			GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_DBLOCAL, +1);
		#endif
			
			m_ctx = gnew DBLocalContext;
		}
		DBLocal::~DBLocal()
		{
			if(this->IsOpen())
				this->Close();
			gdel m_ctx;
			
		#ifdef GAIA_DEBUG_INSTANCECOUNT
			GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_DBLOCAL, -1);
		#endif
		}
		GAIA::BL DBLocal::Open(const GAIA::CH* pszDBName)
		{
		#ifdef GAIA_FEATURE_SQLITE
			GAIA::N32 nThreadSafeFlag = sqlite3_threadsafe();
			if(nThreadSafeFlag != 1)
			{
				GASTFALSE;
				return GAIA::False;
			}
			if(this->IsOpen())
				this->Close();
			if(sqlite3_open(pszDBName, &m_ctx->pSqliteDB) != SQLITE_OK)
				return GAIA::False;
			m_ctx->strDBName = pszDBName;
			
		#	ifdef GAIA_DEBUG_INSTANCECOUNT
				GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_OPENNEDDBLOCAL, +1);
		#	endif
			
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocal::Close()
		{
			if(!this->IsOpen())
				return GAIA::False;
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_close(m_ctx->pSqliteDB) != SQLITE_OK)
				return GAIA::False;
			m_ctx->strDBName.clear();
			m_ctx->syncmode = GAIA::DB::DB_SYNC_MODE_INVALID;
			m_ctx->sCacheSize = GINVALID;
			
		#	ifdef GAIA_DEBUG_INSTANCECOUNT
				GAIA::ChangeInstanceCount(GAIA::INSTANCE_COUNT_OPENNEDDBLOCAL, -1);
		#	endif
			
			return GAIA::True;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		GAIA::BL DBLocal::IsOpen() const
		{
		#ifdef GAIA_FEATURE_SQLITE
			return m_ctx->pSqliteDB != GNIL;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
		}
		const GAIA::CH* DBLocal::GetDBName() const
		{
			return m_ctx->strDBName.fptr();
		}
		GAIA::BL DBLocal::SetSyncMode(GAIA::DB::DB_SYNC_MODE m)
		{
			if(!this->IsOpen())
				return GAIA::False;
			if(m_ctx->syncmode == m)
				return GAIA::True;
			DBLocalQuery q(*this);
			if(m == GAIA::DB::DB_SYNC_MODE_FASTEST)
				q.Exec("PRAGMA synchronous = OFF;");
			else if(m == GAIA::DB::DB_SYNC_MODE_NORMAL)
				q.Exec("PRAGMA synchronous = NORMAL;");
			else if(m == GAIA::DB::DB_SYNC_MODE_SAFEST)
				q.Exec("PRAGMA synchronous = FULL;");
			else
				return GAIA::False;
			m_ctx->syncmode = m;
			return GAIA::True;
		}
		GAIA::DB::DB_SYNC_MODE DBLocal::GetSyncMode() const
		{
			if(!this->IsOpen())
				return GAIA::DB::DB_SYNC_MODE_INVALID;
			if(m_ctx->syncmode == GAIA::DB::DB_SYNC_MODE_INVALID)
			{
				DBLocalQuery q(*GCCAST(GAIA::DB::DBLocal*)(this));
				if(!q.Prepare("PRAGMA synchronous;"))
					return GAIA::DB::DB_SYNC_MODE_INVALID;
				if(!q.Step())
					return GAIA::DB::DB_SYNC_MODE_INVALID;
				GAIA::N32 n = q.GetN32();
				if(n == 0)
					m_ctx->syncmode = GAIA::DB::DB_SYNC_MODE_FASTEST;
				else if(n == 1)
					m_ctx->syncmode = GAIA::DB::DB_SYNC_MODE_NORMAL;
				else if(n == 2)
					m_ctx->syncmode = GAIA::DB::DB_SYNC_MODE_SAFEST;
				else
					m_ctx->syncmode = GAIA::DB::DB_SYNC_MODE_INVALID;
			}
			return m_ctx->syncmode;
		}
		GAIA::BL DBLocal::SetCacheSize(GAIA::NUM sCacheSize)
		{
			if(!this->IsOpen())
				return GAIA::False;
			GAIA::CH szTemp[200];
			GAIA::CH szTemp1[64];
			GAIA::ALGO::castv(sCacheSize, szTemp1, sizeof(szTemp1));
			GAIA::ALGO::gstrcpy(szTemp, "PRAGMA cache_size=");
			GAIA::ALGO::gstrcat(szTemp, szTemp1);
			GAIA::ALGO::gstrcat(szTemp, ";");
			DBLocalQuery q(*this);
			if(!q.Exec(szTemp))
				return GAIA::False;
			m_ctx->sCacheSize = sCacheSize;
			return GAIA::True;
		}
		GAIA::NUM DBLocal::GetCacheSize() const
		{
			if(!this->IsOpen())
				return GAIA::False;
			if(m_ctx->sCacheSize == GINVALID)
			{
				DBLocalQuery q(*GCCAST(GAIA::DB::DBLocal*)(this));
				if(!q.Prepare("PRAGMA cache_size;"))
					return GINVALID;
				if(!q.Step())
					return GINVALID;
				m_ctx->sCacheSize = (GAIA::NUM)q.GetN32();
			}
			return m_ctx->sCacheSize;
		}
		GAIA::BL DBLocal::BeginTransaction()
		{
			if(!this->IsOpen())
				return GAIA::False;
			m_ctx->lrTransaction.Enter();
			if(m_ctx->nTransactionRefCount == 0)
			{
				m_ctx->sTransactionMethod = 0; // 0 means not init, 1 means commit, 2 means rollback.
			#ifdef GAIA_FEATURE_SQLITE
				if(sqlite3_exec(m_ctx->pSqliteDB, "BEGIN IMMEDIATE TRANSACTION", GNIL, GNIL, GNIL) != SQLITE_OK)
				{
					m_ctx->lrTransaction.Leave();
					return GAIA::False;
				}
			#else
				GTHROW_RET(NotSupport, GAIA::False);
			#endif
			}
			m_ctx->nTransactionRefCount++;
			return GAIA::True;
		}
		GAIA::BL DBLocal::EndTransaction(GAIA::BL bRollBack)
		{
			if(!this->IsOpen())
				return GAIA::False;
			m_ctx->nTransactionRefCount--;
			if(bRollBack && m_ctx->sTransactionMethod == +1)
			{
				m_ctx->lrTransaction.Leave();
				return GAIA::False;
			}
			else if(!bRollBack && m_ctx->sTransactionMethod == -1)
			{
				m_ctx->lrTransaction.Leave();
				return GAIA::False;
			}
			if(bRollBack)
				m_ctx->sTransactionMethod = -1;
			else
				m_ctx->sTransactionMethod = +1;
			if(m_ctx->nTransactionRefCount == 0)
			{
				if(bRollBack)
				{
				#ifdef GAIA_FEATURE_SQLITE
					if(sqlite3_exec(m_ctx->pSqliteDB, "ROLLBACK TRANSACTION", GNIL, GNIL, GNIL) != SQLITE_OK)
					{
						m_ctx->lrTransaction.Leave();
						return GAIA::False;
					}
				#else
					GTHROW_RET(NotSupport, GAIA::False);
				#endif
				}
				else
				{
				#ifdef GAIA_FEATURE_SQLITE
					if(sqlite3_exec(m_ctx->pSqliteDB, "COMMIT TRANSACTION", GNIL, GNIL, GNIL) != SQLITE_OK)
					{
						m_ctx->lrTransaction.Leave();
						return GAIA::False;
					}
				#else
					GTHROW_RET(NotSupport, GAIA::False);
				#endif
				}
			}
			m_ctx->sTransactionMethod = 0;
			m_ctx->lrTransaction.Leave();
			return GAIA::True;
		}
		GAIA::BL DBLocal::Optimize()
		{
		#ifdef GAIA_FEATURE_SQLITE
			if(sqlite3_exec(m_ctx->pSqliteDB, "VACUUM;", GNIL, GNIL, GNIL) != SQLITE_OK)
				return GAIA::False;
		#else
			GTHROW_RET(NotSupport, GAIA::False);
		#endif
			return GAIA::True;
		}
		GAIA::BL DBLocal::Checkup()
		{
			DBLocalQuery q(*this);
			if(!q.Prepare("PRAGMA integrity_check;"))
				return GAIA::False;
			if(!q.Step())
				return GAIA::False;
			const GAIA::CH* pszResult = q.GetStringA();
			if(pszResult == GNIL)
				return GAIA::False;
			if(!GAIA::ALGO::gstrequal(pszResult, "ok"))
				return GAIA::False;
			return GAIA::True;
		}
		const GAIA::CH* DBLocal::GetErrorString() const
		{
			if(!this->IsOpen())
				return GNIL;
		#ifdef GAIA_FEATURE_SQLITE
			return sqlite3_errmsg(m_ctx->pSqliteDB);
		#else
			GTHROW_RET(NotSupport, GNIL);
		#endif
		}
	}
}
