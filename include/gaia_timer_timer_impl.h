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
			if(this->IsRegisted())
				m_pTimerMgr->Unregist(*this);
			m_pTimerMgr = GNIL;
			m_nRegistTime = 0;
			m_nLastUpdateTime = 0;
			m_nUpdateTimes = 0;
			m_desc.reset();
		}
		GINL GAIA::TIME::TimerMgr* Timer::GetTimerMgr() const
		{
			if(m_pTimerMgr != GNIL)
				m_pTimerMgr->rise_ref();
			return m_pTimerMgr;
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
		GINL GAIA::GVOID Timer::Update(GAIA::TIME::Timer::FIRE_REASON reason)
		{
			this->rise_ref();
			{
				/* Get timer manager. */
				GAIA::TIME::TimerMgr* pTimerMgr = this->GetTimerMgr();

				/* Update update times. */
				this->SetUpdateTimes(this->GetUpdateTimes() + 1);

				/* Update. */
				if(m_desc.pCallBack != GNIL)
					m_desc.pCallBack->UpdateTimer(this, reason);

				/* If complete, unregist and release(if user specified. */
				if(this->GetUpdateTimes() == this->GetDesc().nMaxUpdateTimes)
				{
					if(this->IsRegisted())
					{
						if(!pTimerMgr->Unregist(*this))
							GASTFALSE;
					}
					if(this->GetDesc().bAutoRelease)
					{
						if(this->get_ref() > 1)
							this->drop_ref();
					}
				}

				/* Release timer manager. */
				pTimerMgr->drop_ref();
			}
			this->drop_ref();
		}
		GINL GAIA::GVOID Timer::SetTimerMgr(GAIA::TIME::TimerMgr* pTimerMgr)
		{
			m_pTimerMgr = pTimerMgr;
		}
	}
}

#endif
