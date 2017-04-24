#ifndef		__GAIA_TIMER_TIMER_IMPL_H__
#define		__GAIA_TIMER_TIMER_IMPL_H__

#include "gaia_timer_timer.h"

namespace GAIA
{
	namespace TIME
	{
		GINL GAIA::GVOID Timer::Destroy()
		{
			m_bPaused = GAIA::False;
			m_pTimerMgr = GNIL;
			m_nRegistTime = 0;
			m_nLastUpdateTime = 0;
			m_nUpdateTimes = 0;
			m_desc.reset();
		}
		GINL GAIA::BL Timer::Pause()
		{
			/* Pause. */
			m_bPaused = GAIA::True;

			/* Pause fire. */
			if(m_desc.descFire.bPauseFire)
				this->Update(GAIA::TIME::Timer::FIRE_REASON_PAUSE);

			return GAIA::True;
		}
		GINL GAIA::BL Timer::Resume()
		{
			/* Resume. */
			m_bPaused = GAIA::False;

			/* Resume fire. */
			if(m_desc.descFire.bResumeFire)
				this->Update(GAIA::TIME::Timer::FIRE_REASON_RESUME);

			return GAIA::True;
		}
		GINL GAIA::BL Timer::Update(GAIA::TIME::Timer::FIRE_REASON reason)
		{
			/* Update update times. */
			if(reason == GAIA::TIME::Timer::FIRE_REASON_UPDATE)
				this->SetUpdateTimes(this->GetUpdateTimes() + 1);

			/* Update. */
			if(m_desc.pCallBack != GNIL)
				m_desc.pCallBack->UpdateTimer(this, reason);

			/* If complete, unregist and release(if user specified). */
			GAIA::BL bUpdateComplete;
			if(reason == GAIA::TIME::Timer::FIRE_REASON_UPDATE &&
			   this->GetUpdateTimes() == this->GetDesc().nMaxUpdateTimes)
				bUpdateComplete = GAIA::True;
			else
				bUpdateComplete = GAIA::False;
			return bUpdateComplete;
		}
	}
}

#endif
