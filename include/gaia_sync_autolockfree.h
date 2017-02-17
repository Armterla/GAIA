#ifndef		__GAIA_SYNC_AUTOLOCKFREE_H__
#define		__GAIA_SYNC_AUTOLOCKFREE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lockfree.h"

namespace GAIA
{
	namespace SYNC
	{
		class AutolockFree : public GAIA::Base
		{
		public:
			GINL AutolockFree(LockFree& l)
			{
				m_pLock = &l;
				m_pLock->Enter();
			}
			GINL AutolockFree(LockFree& l, GAIA::BL bLock)
			{
				if(bLock)
				{
					m_pLock = &l;
					m_pLock->Enter();
				}
				else
					m_pLock = GNIL;
			}
			GINL AutolockFree()
			{
				if(m_pLock != GNIL)
					m_pLock->Leave();
			}
		private:
			LockFree* m_pLock;
		};
	}
}

#endif
