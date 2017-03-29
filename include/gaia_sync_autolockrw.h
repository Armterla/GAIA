#ifndef		__GAIA_SYNC_AUTOLOCKRW_H__
#define		__GAIA_SYNC_AUTOLOCKRW_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lockrw.h"

namespace GAIA
{
	namespace SYNC
	{
		class AutolockRW : public GAIA::Base
		{
		public:
			GINL AutolockRW(LockRW& l, GAIA::BL bReadTrueWriteFalse)
			{
				m_pLock = &l;
				m_bReadTrueWriteFalse = bReadTrueWriteFalse;
				if(m_bReadTrueWriteFalse)
					m_pLock->EnterRead();
				else
					m_pLock->EnterWrite();
			}
			GINL AutolockRW(LockRW& l, GAIA::BL bReadTrueWriteFalse, GAIA::BL bLock)
			{
				if(bLock)
				{
					m_pLock = &l;
					m_bReadTrueWriteFalse = bReadTrueWriteFalse;
					if(m_bReadTrueWriteFalse)
						m_pLock->EnterRead();
					else
						m_pLock->EnterWrite();
				}
				else
					m_pLock = GNIL;
			}
			GINL ~AutolockRW()
			{
				if(m_pLock != GNIL)
				{
					if(m_bReadTrueWriteFalse)
						m_pLock->LeaveRead();
					else
						m_pLock->LeaveWrite();
				}
			}
		private:
			LockRW* m_pLock;
			GAIA::BL m_bReadTrueWriteFalse;
		};
	}
}

#endif
