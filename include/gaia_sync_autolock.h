#ifndef 	__GAIA_SYNC_AUTOLOCK_H__
#define 	__GAIA_SYNC_AUTOLOCK_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"

namespace GAIA
{
	namespace SYNC
	{
		class Autolock : public GAIA::Base
		{
		public:
			GINL Autolock(Lock& l)
			{
				m_pLock = &l;
				m_pLock->Enter();
			}
			GINL Autolock(Lock& l, GAIA::BL bLock)
			{
				if(bLock)
				{
					m_pLock = &l;
					m_pLock->Enter();
				}
				else
					m_pLock = GNIL;
			}
			GINL ~Autolock()
			{
				if(m_pLock != GNIL)
					m_pLock->Leave();
			}
		private:
			Lock* m_pLock;
		};
	}
}

#endif
