#ifndef		__GAIA_DATABASE_DBLOCAL_H__
#define		__GAIA_DATABASE_DBLOCAL_H__

#include "gaia_type.h"
#include "gaia_assert.h"

namespace GAIA
{
	namespace DB
	{
		/*!
			@brief Check DBLocal's SQL is valid or not.
		 
			@return If the SQL is valid, return GAIA::True, or will return GAIA::False.
		*/
		extern GAIA::BL DBLocalCheckSQL(const GAIA::CH* pszSQL);

		class DBLocal;
		class DBLocalQueryContext;
		class DBLocalQuery : public GAIA::Base
		{
		public:
			/*!
				@brief Constructor.
			*/
			DBLocalQuery(GAIA::DB::DBLocal& db);

			/*!
				@brief Destructor.
			*/
			~DBLocalQuery();

			/*!
				@brief Get owner database.
			*/
			GAIA::DB::DBLocal* GetDatabase();

			/*!
				@brief Check current DBLocalQuery is queried or not.
			*/
			GAIA::BL IsQueryed() const;

			/*!
				@brief Get current DBLocalQuery's SQL which had be prepared by DBLocalQuery::Prepare.
			*/
			const GAIA::CH* GetSQL() const;

			/*!
				@brief Close current query.
			*/
			GAIA::BL Close();

			/*!
				@brief Prepare current query.
			*/
			GAIA::BL Prepare(const GAIA::CH* pszSQL);

			/*!
				@brief Execute current query.
			*/
			GAIA::BL Exec(const GAIA::CH* pszSQL);

			/*!
				@brief Bind a Null value to current prepared statements.
			*/
			GAIA::BL BindNull(GAIA::NUM sIndex = GINVALID);

			/*!
				@brief Bind a GAIA::N32 value to current prepared statements.
			*/
			GAIA::BL BindN32(GAIA::N32 n, GAIA::NUM sIndex = GINVALID);

			/*!
				@brief Bind a GAIA::N64 value to current prepared statements.
			*/
			GAIA::BL BindN64(GAIA::N64 l, GAIA::NUM sIndex = GINVALID);

			/*!
				@brief Bind a GAIA::F32 value to current prepared statements.
			*/
			GAIA::BL BindF32(GAIA::F32 f, GAIA::NUM sIndex = GINVALID);

			/*!
				@brief Bind a GAIA::F64 value to current prepared statements.
			*/
			GAIA::BL BindF64(GAIA::F64 d, GAIA::NUM sIndex = GINVALID);

			/*!
				@brief Bind a char string value to current prepared statements.
			*/
			GAIA::BL BindStringA(const GAIA::CH* psz, GAIA::NUM sIndex = GINVALID, GAIA::BL bStatic = GAIA::True);

			/*!
				@brief Bind a wide char string value to current prepared statements.
			*/
			GAIA::BL BindStringW(const GAIA::WCH* psz, GAIA::NUM sIndex = GINVALID, GAIA::BL bStatic = GAIA::True);

			/*!
				@brief Bind a binary buffer value to current prepared statements.
			*/
			GAIA::BL BindBlob(const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::NUM sIndex = GINVALID, GAIA::BL bStatic = GAIA::True);

			/*!
				@brief Reset current DBLocalQuery.
			*/
			GAIA::BL Reset();

			/*!
				@brief Get field count of query result.
			*/
			GAIA::NUM GetFieldCount();

			/*!
				@brief Get result count of query result.
			*/
			GAIA::NUM GetRecordCount();

			/*!
				@brief Get field name of query result.
			*/
			const GAIA::CH* GetFieldName(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief Get field type of query result.
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
				@brief Step to next query result for read-query or execute query for write-query.
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
				@brief Get query error string.
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
				@brief Constructor.
			*/
			DBLocal();

			/*!
				@brief Destructor.
			*/
			~DBLocal();

			/*!
				@brief Open a database.
			*/
			GAIA::BL Open(const GAIA::CH* pszDBName);

			/*!
				@brief Close current database.
			*/
			GAIA::BL Close();

			/*!
				@brief Check current database is openned or not.
			 
				@return If current database is openned, return GAIA::True, or will return GAIA::False.
			*/
			GAIA::BL IsOpen() const;

			/*!
				@brief Get current database's name.
			*/
			const GAIA::CH* GetDBName() const;

			/*!
				@brief Set database's sync mode.
			*/
			GAIA::BL SetSyncMode(GAIA::DB::DB_SYNC_MODE m);

			/*!
				@brief Get database's sync mode.
			*/
			GAIA::DB::DB_SYNC_MODE GetSyncMode() const;

			/*!
				@brief Set database's RAM cache size.
			*/
			GAIA::BL SetCacheSize(GAIA::NUM sCacheSize);

			/*!
				@brief Get database's RAM cache size.
			*/
			GAIA::NUM GetCacheSize() const;

			/*!
				@brief Begin transaction.
			*/
			GAIA::BL BeginTransaction();

			/*!
				@brief End transaction.
			*/
			GAIA::BL EndTransaction(GAIA::BL bRollBack = GAIA::False);

			/*!
				@brief Optimize the database for smaller storage and quicker execute performance.
			*/
			GAIA::BL Optimize();

			/*!
				@brief Check correctness of database's data.
			*/
			GAIA::BL Checkup();

			/*!
				@brief Get database error string.
			*/
			const GAIA::CH* GetErrorString() const;

		private:
			DBLocalContext* m_ctx;
		};

		class DBLocalAutoTransaction : public GAIA::Base
		{
		public:
			/*!
				@brief Constructor.
			*/
			GINL DBLocalAutoTransaction(GAIA::DB::DBLocal& db)
			{
				m_pDBLocal = &db;
				if(!m_pDBLocal->BeginTransaction())
					GTHROW(Illegal);
				m_bRollBack = GAIA::False;
			}

			/*!
				@brief Destructor.
			*/
			GINL ~DBLocalAutoTransaction()
			{
				if(!m_pDBLocal->EndTransaction(m_bRollBack))
				{
				#if GAIA_OS == GAIA_OS_WINDOWS
				#	pragma warning(push)
				#	pragma warning(disable : 4297)
				#endif
					GASTFALSE;
				#if GAIA_OS == GAIA_OS_WINDOWS
				#	pragma warning(pop)
				#endif
				}
			}

			/*!
				@brief Set need roll back when destructor called.
			 
				@param bRollBack [in] Specify need or not need rollback when destructor called. 
			*/
			GINL GAIA::GVOID SetRollBack(GAIA::BL bRollBack){m_bRollBack = bRollBack;}

			/*!
				@brief Get need roll back when destructor called.
			 
				@return If the transaction will be roll back when destructor called, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL GetRollBack() const{return m_bRollBack;}

		private:
			GAIA::DB::DBLocal* m_pDBLocal;
			GAIA::BL m_bRollBack;
		};
	}
}

#endif
