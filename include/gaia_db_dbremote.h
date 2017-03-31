#ifndef		__GAIA_DATABASE_DBREMOTE_H__
#define		__GAIA_DATABASE_DBREMOTE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_network_addr.h"

namespace GAIA
{
	namespace DB
	{
		class DBRemote;
		class DBRemoteCallBack : public GAIA::Base
		{
		public:
		};
		class DBRemoteQueryContext;

		/*!
			@brief DBRemoteQuery class.
		*/
		class DBRemoteQuery : public GAIA::Base
		{
		public:
			/*!
				@brief Constructor.
			*/
			DBRemoteQuery(DBRemote& db);

			/*!
				@brief Destructor.
			*/
			~DBRemoteQuery();

			/*!
				@brief Get owner database.
			*/
			GAIA::DB::DBRemote* GetDatabase();

			/*!
				@brief Check current DBRemoteQuery is queried or not.
			*/
			GAIA::BL IsQueryed() const;

			/*!
				@brief Get current DBRemoteQuery's SQL which had be prepared by DBRemoteQuery::Prepare.
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
				@brief Reset current DBRemoteQuery.
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
				@brief Seek the bind cursor for bind parameter.
			*/
			GAIA::BL SeekBind(GAIA::NUM sOffset);

			/*!
				@brief Seek the field cursor for get query result.
			*/
			GAIA::BL SeekField(GAIA::NUM sOffset);

			/*!
				@brief Step to next query result for read-query or execute query for write-query.
			*/
			GAIA::BL Step();

			/*!
				@brief Get a GAIA::N32 column value of query result record.
			*/
			GAIA::N32 GetN32(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief Get a GAIA::N64 column value of query result record.
			*/
			GAIA::N64 GetN64(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief Get a GAIA::F32 column value of query result record.
			*/
			GAIA::F32 GetF32(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief Get a GAIA::F64 column value of query result record.
			*/
			GAIA::F64 GetF64(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief Get a char string column value of query result record.
			*/
			const GAIA::CH* GetStringA(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief Get a wchar string column value of query result record.
			*/
			const GAIA::WCH* GetStringW(GAIA::NUM sColumn = GINVALID);

			/*!
				@brief Get a blob column value of query result record.
			*/
			const GAIA::GVOID* GetBlob(GAIA::NUM& sSize, GAIA::NUM sColumn = GINVALID);

			/*!
				@brief Get query error string.
			*/
			const GAIA::CH* GetErrorString() const;

		private:
			DBRemoteQueryContext* m_ctx;
		};
		class DBRemoteContext;

		/*!
			@brief DBRemote class.
		*/
		class DBRemote : public GAIA::Base
		{
			friend class DBRemoteQuery;

		public:
			/*!
				@brief Constructor.
			*/
			DBRemote();

			/*!
				@brief Destructor.
			*/
			~DBRemote();

			/*!
				@brief Connect to server.
			*/
			GAIA::BL Connect(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Disconnect from server
			*/
			GAIA::BL Disconnect();

			/*!
				@brief Check the DBRemote is connected to server or not.
			*/
			GAIA::BL IsConnect() const;

			/*!
				@brief Get current connected server address.
			*/
			const GAIA::NETWORK::Addr& GetServerAddr() const;

			/*!
				@brief Open a database.
			*/
			GAIA::BL Open(const GAIA::CH* pszDBName, const GAIA::CH* pszUserName, const GAIA::CH* pszPassword);

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
				@brief Get current database's user name.
			*/
			const GAIA::CH* GetUserName() const;

			/*!
				@brief Get current database's user password.
			*/
			const GAIA::CH* GetPassword() const;

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
		};

		/*!
			@brief DBRemote auto transcation class.
		*/
		class DBRemoteAutoTransaction : public GAIA::Base
		{
		public:
			/*!
				@brief Constructor.
			*/
			GINL DBRemoteAutoTransaction(GAIA::DB::DBRemote& db)
			{
				m_pDBRemote = &db;
				if(!m_pDBRemote->BeginTransaction())
					GTHROW(Illegal);
				m_bRollBack = GAIA::False;
			}

			/*!
				@brief Destructor.
			*/
			GINL ~DBRemoteAutoTransaction()
			{
				if(!m_pDBRemote->EndTransaction(m_bRollBack))
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
			GAIA::DB::DBRemote* m_pDBRemote;
			GAIA::BL m_bRollBack;
		};
	}
}

#endif
