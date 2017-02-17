#ifndef		__GAIA_SYNC_AUTOLOCKR_H__
#define		__GAIA_SYNC_AUTOLOCKR_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lockrw.h"

namespace GAIA
{
	namespace SYNC
	{
		class AutolockR : public GAIA::Base
		{
		public:
			GINL AutolockR(LockRW& l)
			{
				m_pLock = &l;
				m_pLock->EnterRead();
			}
			GINL AutolockR(LockRW& l, GAIA::BL bLock)
			{
				if(bLock)
				{
					m_pLock = &l;
					m_pLock->EnterRead();
				}
				else
					m_pLock = GNIL;
			}
			GINL ~AutolockR()
			{
				if(m_pLock != GNIL)
					m_pLock->LeaveRead();
			}
		private:
			LockRW* m_pLock;
		};
	}
}

#endif
