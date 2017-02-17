#ifndef		__GAIA_DATABASE_DBLOCAL_H__
#define		__GAIA_DATABASE_DBLOCAL_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace DB
	{
		extern GAIA::BL DBLocalCheckSQL(const GAIA::CH* pszSQL);

		class DBLocal;
		class DBLocalQueryContext;
		class DBLocalQuery : public GAIA::Base
		{
		public:
			/*!
				@brief
			*/
			DBLocalQuery(GAIA::DB::DBLocal& db);

			/*!
				@brief
			*/
			~DBLocalQuery();

			/*!
				@brief
			*/
			GAIA::DB::DBLocal* GetDatabase();

			/*!
				@brief
			*/
			GAIA::BL IsQueryed() const;

			/*!
				@brief
			*/
			const GAIA::CH* GetSQL() const;

			/*!
				@brief
			*/
			GAIA::BL Close();

			/*!
				@brief
			*/
			GAIA::BL Prepare(const GAIA::CH* pszSQL);

			/*!
				@brief
			*/
			GAIA::BL Exec(const GAIA::CH* pszSQL);

			/*!
				@brief
			*/
			GAIA::BL BindNull(GAIA::NUM sIndex = GINVALID);

			/*!
				@brief
			*/
			GAIA::BL BindN32(GAIA::N32 n, GAIA::NUM sIndex = GINVALID);

			/*!
				@brief
			*/
			GAIA::BL BindN64(GAIA::N64 l, GAIA::NUM sIndex = GINVALID);

			/*!
				@brief
			*/
			GAIA::BL BindF32(GAIA::F32 f, GAIA::NUM sIndex = GINVALID);

			/*!
				@brief
			*/
			GAIA::BL BindF64(GAIA::F64 d, GAIA::NUM sIndex = GINVALID);

			/*!
				@brief
			*/
			GAIA::BL BindStringA(const GAIA::CH* psz, GAIA::NUM sIndex = GINVALID, GAIA::BL bStatic = GAIA::True);

			/*!
				@brief
			*/
			GAIA::BL BindStringW(const GAIA::WCH* psz, GAIA::NUM sIndex = GINVALID, GAIA::BL bStatic = GAIA::True);

			/*!
				@brief
			*/
			GAIA::BL BindBlob(const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::NUM sIndex = GINVALID, GAIA::BL bStatic = GAIA::True);

			/*!
				@brief
			*/
			GAIA::BL Reset();

			/*!
				@brief
			*/
			GAIA::NUM GetFieldCount();

			/*!
				@brief
			*/
			GAIA::NUM GetRecordCount();

			/*!
				@brief
			*/
			const GAIA::CH* GetFieldName(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			GAIA::TYPEID GetFieldType(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			GAIA::BL SeekBind(GAIA::NUM sOffset);

			/*!
				@brief
			*/
			GAIA::BL SeekField(GAIA::NUM sOffset);

			/*!
				@brief
			*/
			GAIA::BL Step();

			/*!
				@brief
			*/
			GAIA::N32 GetN32(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			GAIA::N64 GetN64(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			GAIA::F32 GetF32(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			GAIA::F64 GetF64(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			const GAIA::CH* GetStringA(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			const GAIA::WCH* GetStringW(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			const GAIA::GVOID* GetBlob(GAIA::NUM& sSize, GAIA::NUM sColumn = GINVALID);

			/*!
				@brief
			*/
			const GAIA::CH* GetErrorString() const;

		private:
			DBLocalQueryContext* m_ctx;
		};

		GAIA_ENUM_BEGIN(DB_SYNC_MODE)
			DB_SYNC_MODE_FASTEST,
			DB_SYNC_MODE_NORMAL,
			DB_SYNC_MODE_SAFEST,
		GAIA_ENUM_END(DB_SYNC_MODE)

		class DBLocalContext;
		class DBLocal : public GAIA::Base
		{
			friend class DBLocalQuery;

		public:
			/*!
				@brief
			*/
			DBLocal();

			/*!
				@brief
			*/
			~DBLocal();

			/*!
				@brief
			*/
			GAIA::BL Open(const GAIA::CH* pszDBName);

			/*!
				@brief
			*/
			GAIA::BL Close();

			/*!
				@brief
			*/
			GAIA::BL IsOpen() const;

			/*!
				@brief
			*/
			const GAIA::CH* GetDBName() const;

			/*!
				@brief
			*/
			GAIA::BL SetSyncMode(GAIA::DB::DB_SYNC_MODE m);

			/*!
				@brief
			*/
			GAIA::DB::DB_SYNC_MODE GetSyncMode() const;

			/*!
				@brief
			*/
			GAIA::BL SetCacheSize(GAIA::NUM sCacheSize);

			/*!
				@brief
			*/
			GAIA::NUM GetCacheSize() const;

			/*!
				@brief
			*/
			GAIA::BL BeginTransaction();

			/*!
				@brief
			*/
			GAIA::BL EndTransaction(GAIA::BL bRollBack = GAIA::False);

			/*!
				@brief
			*/
			GAIA::BL Optimize();

			/*!
				@brief
			*/
			GAIA::BL Checkup();

			/*!
				@brief
			*/
			const GAIA::CH* GetErrorString() const;

		private:
			DBLocalContext* m_ctx;
		};

		class DBLocalAutoTransaction : public GAIA::Base
		{
		public:
			/*!
				@brief
			*/
			GINL DBLocalAutoTransaction(GAIA::DB::DBLocal& db)
			{
				m_pDBLocal = &db;
				if(!m_pDBLocal->BeginTransaction())
					GTHROW(Illegal);
				m_bRollBack = GAIA::False;
			}

			/*!
				@brief
			*/
			GINL ~DBLocalAutoTransaction()
			{
				if(!m_pDBLocal->EndTransaction(m_bRollBack))
					GTHROW(Illegal);
			}

			/*!
				@brief
			*/
			GINL GAIA::GVOID SetRollBack(GAIA::BL bRollBack){m_bRollBack = bRollBack;}

			/*!
				@brief
			*/
			GINL GAIA::BL GetRollBack() const{return m_bRollBack;}

		private:
			GAIA::DB::DBLocal* m_pDBLocal;
			GAIA::BL m_bRollBack;
		};
	}
}

#endif
