#ifndef 	__GAIA_SYNC_AUTOLOCKPURE_H__
#define 	__GAIA_SYNC_AUTOLOCKPURE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lockpure.h"

namespace GAIA
{
	namespace SYNC
	{
		class AutolockPure : public GAIA::Base
		{
		public:
			GINL AutolockPure(LockPure& l)
			{
				m_pLock = &l;
				m_pLock->Enter();
			}
			GINL AutolockPure(LockPure& l, GAIA::BL bLock)
			{
				if(bLock)
				{
					m_pLock = &l;
					m_pLock->Enter();
				}
				else
					m_pLock = GNIL;
			}
			GINL ~AutolockPure()
			{
				if(m_pLock != GNIL)
					m_pLock->Leave();
			}
		private:
			LockPure* m_pLock;
		};
	}
}

#endif
