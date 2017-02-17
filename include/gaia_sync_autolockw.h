#ifndef		__GAIA_SYNC_AUTOLOCKW_H__
#define		__GAIA_SYNC_AUTOLOCKW_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lockrw.h"

namespace GAIA
{
	namespace SYNC
	{
		class AutolockW : public GAIA::Base
		{
		public:
			GINL AutolockW(LockRW& l)
			{
				m_pLock = &l;
				m_pLock->EnterWrite();
			}
			GINL AutolockW(LockRW& l, GAIA::BL bLock)
			{
				if(bLock)
				{
					m_pLock = &l;
					m_pLock->EnterWrite();
				}
				else
					m_pLock = GNIL;
			}
			GINL ~AutolockW()
			{
				if(m_pLock != GNIL)
					m_pLock->LeaveWrite();
			}
		private:
			LockRW* m_pLock;
		};
	}
}

#endif
