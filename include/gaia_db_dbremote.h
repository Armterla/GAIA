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
		class DBRemoteQuery : public GAIA::Base
		{
		public:
			/*!
				@brief
			*/
			DBRemoteQuery(DBRemote& db);

			/*!
				@brief
			*/
			~DBRemoteQuery();

			/*!
				@brief
			*/
			GAIA::DB::DBRemote* GetDatabase();

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
			DBRemoteQueryContext* m_ctx;
		};
		class DBRemoteContext;
		class DBRemote : public GAIA::Base
		{
			friend class DBRemoteQuery;

		public:
			/*!
				@brief
			*/
			DBRemote();

			/*!
				@brief
			*/
			~DBRemote();

			/*!
				@brief
			*/
			GAIA::BL Connect(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief
			*/
			GAIA::BL Disconnect();

			/*!
				@brief
			*/
			GAIA::BL IsConnect() const;

			/*!
				@brief
			*/
			const GAIA::NETWORK::Addr& GetServerAddr() const;

			/*!
				@brief
			*/
			GAIA::BL Open(const GAIA::CH* pszDBName, const GAIA::CH* pszUserName, const GAIA::CH* pszPassword);

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
			const GAIA::CH* GetUserName() const;

			/*!
				@brief
			*/
			const GAIA::CH* GetPassword() const;

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
		};
		class DBRemoteAutoTransaction : public GAIA::Base
		{
		public:
			/*!
				@brief
			*/
			GINL DBRemoteAutoTransaction(GAIA::DB::DBRemote& db)
			{
				m_pDBRemote = &db;
				if(!m_pDBRemote->BeginTransaction())
					GTHROW(Illegal);
				m_bRollBack = GAIA::False;
			}

			/*!
				@brief
			*/
			GINL ~DBRemoteAutoTransaction()
			{
				if(!m_pDBRemote->EndTransaction(m_bRollBack))
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
			GAIA::DB::DBRemote* m_pDBRemote;
			GAIA::BL m_bRollBack;
		};
	}
}

#endif
