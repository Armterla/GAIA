#ifndef		__GAIA_SYNC_LOCKRW_H__
#define		__GAIA_SYNC_LOCKRW_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_lockfree.h"

namespace GAIA
{
	namespace SYNC
	{
		/*!
			@brief Sync recursive read write lock.
		*/
		class LockRW : public GAIA::Base
		{
		public:
			GINL LockRW(){m_nRCount = 0;}
			GINL GAIA::GVOID EnterRead()
			{
				m_lockr.Enter();
				{
					GAST(m_nRCount >= 0);
					if(m_nRCount == 0)
						m_lockw.Enter();
					m_nRCount++;
				}
				m_lockr.Leave();
			}
			GINL GAIA::GVOID LeaveRead()
			{
				m_lockr.Enter();
				{
					GAST(m_nRCount > 0);
					--m_nRCount;
					if(m_nRCount == 0)
						m_lockw.Leave();
				}
				m_lockr.Leave();
			}
			GINL GAIA::GVOID EnterWrite()
			{
				m_lockw.Enter();
			}
			GINL GAIA::GVOID LeaveWrite()
			{
				m_lockw.Leave();
			}

			/*!
				@brief get how many reader had entered the lock.

				@remarks
					This function only can be called when entered read lock or entered write lock.
			*/
			GINL GAIA::NM GetReaderCount() const{return m_nRCount;}
		private:
			GINL LockRW(const LockRW& src){}
		private:
			GAIA::SYNC::LockFree m_lockw;
			GAIA::SYNC::Lock m_lockr;
			GAIA::NM m_nRCount;
		};
	}
}

#endif
