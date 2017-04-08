#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class TimerCallBackBase : public GAIA::Base
	{
	public:
		GINL TimerCallBackBase(){this->init();}
		GINL GAIA::GVOID SetLock(GAIA::SYNC::Lock& lock){m_pLock = &lock;}
		GINL GAIA::SYNC::Lock& GetLock() const{return *m_pLock;}
		GINL GAIA::GVOID SetUpdateTimeSum(GAIA::TIME::Timer::__UpdateTimesType& nSum){m_pSum = &nSum;}
		GINL GAIA::TIME::Timer::__UpdateTimesType& GetUpdateTimeSum() const{return *m_pSum;}
		GINL GAIA::GVOID UpdateTimerBase(const GAIA::TIME::Timer::__MicroSecType& t)
		{
			if(m_nLastUpdateTime != GINVALID)
			{
				GAIA::NUM sDeltaTime = (GAIA::NUM)(t - m_nLastUpdateTime);
				m_nLastUpdateTimeSum += sDeltaTime;
				++m_nTimeIncreaseTimes;
			}
			m_nLastUpdateTime = t;
		}
		GINL GAIA::TIME::Timer::__MicroSecType GetAvgTime() const
		{
			if(m_nTimeIncreaseTimes == 0)
				return 0;
			return m_nLastUpdateTimeSum / m_nTimeIncreaseTimes;
		}
	private:
		GINL GAIA::GVOID init()
		{
			m_pLock = GNIL;
			m_pSum = GNIL;
			m_nLastUpdateTime = GINVALID;
			m_nLastUpdateTimeSum = 0;
			m_nTimeIncreaseTimes = 0;
		}
	private:
		GAIA::SYNC::Lock* m_pLock;
		GAIA::TIME::Timer::__UpdateTimesType* m_pSum;
		GAIA::TIME::Timer::__MicroSecType m_nLastUpdateTime;
		GAIA::TIME::Timer::__MicroSecType m_nLastUpdateTimeSum;
		GAIA::N64 m_nTimeIncreaseTimes;
	};
	class TimerCallBack1 : public GAIA::TIME::Timer::CallBack, public TimerCallBackBase
	{
	public:
		GINL TimerCallBack1(){this->init();}
		virtual GAIA::GVOID UpdateTimer(GAIA::TIME::Timer* pTimer, GAIA::TIME::Timer::FIRE_REASON reason)
		{
			GAIA::SYNC::Autolock al(this->GetLock());
			++this->GetUpdateTimeSum();
			this->UpdateTimerBase(pTimer->GetLastUpdateTime());
		}
	private:
		GINL GAIA::GVOID init(){}
	};

	class TimerCallBack2 : public GAIA::TIME::Timer::CallBack, public TimerCallBackBase
	{
	public:
		GINL TimerCallBack2(){this->init();}
		virtual GAIA::GVOID UpdateTimer(GAIA::TIME::Timer* pTimer, GAIA::TIME::Timer::FIRE_REASON reason)
		{
			GAIA::SYNC::Autolock al(this->GetLock());
			++this->GetUpdateTimeSum();
			this->UpdateTimerBase(pTimer->GetLastUpdateTime());
		}
	private:
		GINL GAIA::GVOID init(){}
	};

	class TimerCallBack3 : public GAIA::TIME::Timer::CallBack, public TimerCallBackBase
	{
	public:
		GINL TimerCallBack3(){this->init();}
		virtual GAIA::GVOID UpdateTimer(GAIA::TIME::Timer* pTimer, GAIA::TIME::Timer::FIRE_REASON reason)
		{
			GAIA::SYNC::Autolock al(this->GetLock());
			++this->GetUpdateTimeSum();
			this->UpdateTimerBase(pTimer->GetLastUpdateTime());
		}
	private:
		GINL GAIA::GVOID init(){}
	};

	class TimerCallBack4 : public GAIA::TIME::Timer::CallBack, public TimerCallBackBase
	{
	public:
		GINL TimerCallBack4(){this->init();}
		virtual GAIA::GVOID UpdateTimer(GAIA::TIME::Timer* pTimer, GAIA::TIME::Timer::FIRE_REASON reason)
		{
			GAIA::SYNC::Autolock al(this->GetLock());
			this->UpdateTimerBase(pTimer->GetLastUpdateTime());
		}
	private:
		GINL GAIA::GVOID init(){}
	};

	extern GAIA::GVOID t_time_timer(GAIA::LOG::Log& logobj)
	{
		/* Create common object. */
		GAIA::SYNC::Lock l;
		GAIA::TIME::Timer::__UpdateTimesType sum = 0;

		/* Create instance. */
		GAIA::TIME::TimerMgr* pTimerMgr = gnew GAIA::TIME::TimerMgr;
		GAIA::TIME::Timer* pTimer1 = gnew GAIA::TIME::Timer;
		GAIA::TIME::Timer* pTimer2 = gnew GAIA::TIME::Timer;
		GAIA::TIME::Timer* pTimer3 = gnew GAIA::TIME::Timer;
		GAIA::TIME::Timer* pTimer4 = gnew GAIA::TIME::Timer;

		/* Create timer manager. */
		GAIA::TIME::TimerMgr::Desc descTimerMgr;
		descTimerMgr.reset();
		pTimerMgr->Create(descTimerMgr);

		/* Create callback. */
		TimerCallBack1 cb1;
		cb1.SetLock(l);
		cb1.SetUpdateTimeSum(sum);

		TimerCallBack2 cb2;
		cb2.SetLock(l);
		cb2.SetUpdateTimeSum(sum);

		TimerCallBack3 cb3;
		cb3.SetLock(l);
		cb3.SetUpdateTimeSum(sum);

		TimerCallBack4 cb4;
		cb4.SetLock(l);
		cb4.SetUpdateTimeSum(sum);

		/* Create timer. */
		GAIA::TIME::Timer::Desc descTimer1;
		descTimer1.reset();
		descTimer1.nEscapeUSec = 1000 * 50;
		descTimer1.nMaxUpdateTimes = 10;
		descTimer1.pCallBack = &cb1;
		TAST(pTimer1->Create(descTimer1));

		GAIA::TIME::Timer::Desc descTimer2;
		descTimer2.reset();
		descTimer2.nEscapeUSec = 1000 * 10;
		descTimer2.nMaxUpdateTimes = 50;
		descTimer2.pCallBack = &cb2;
		TAST(pTimer2->Create(descTimer2));

		GAIA::TIME::Timer::Desc descTimer3;
		descTimer3.reset();
		descTimer3.nEscapeUSec = 1000 * 110;
		descTimer3.nMaxUpdateTimes = 2;
		descTimer3.pCallBack = &cb3;
		descTimer3.bAutoRelease = GAIA::False;
		TAST(pTimer3->Create(descTimer3));

		GAIA::TIME::Timer::Desc descTimer4;
		descTimer4.reset();
		descTimer4.nEscapeUSec = 0;
		descTimer4.nMaxUpdateTimes = GAIA::N32MAX;
		descTimer4.pCallBack = &cb4;
		TAST(pTimer4->Create(descTimer4));

		/* Regist timer. */
		TAST(pTimerMgr->Regist(*pTimer1));
		TAST(pTimerMgr->Regist(*pTimer2));
		TAST(pTimerMgr->Regist(*pTimer3));
		TAST(pTimerMgr->Regist(*pTimer4));

		/* Update. */
		GAIA::TIME::Timer::__MicroSecType tDelta = 0;
		GAIA::TIME::Timer::__MicroSecType t = GAIA::TIME::tick_time();
		for(;;)
		{
			GAIA::TIME::Timer::__MicroSecType tNew = GAIA::TIME::tick_time();
			pTimerMgr->Update(tNew - t);
			tDelta += tNew - t;
			t = tNew;
			if(tDelta > 500 * 1000)
				break;
		}
		pTimerMgr->Update(0);

		/* Check result. */
		if(sum != 62)
			TERROR;

		if(pTimer3->GetUpdateTimes() != 2)
			TERROR;

		GAIA::TIME::Timer::__MicroSecType nAveTime1 = cb1.GetAvgTime();
		if(nAveTime1 != descTimer1.nEscapeUSec)
			TERROR;
		GAIA::TIME::Timer::__MicroSecType nAveTime2 = cb2.GetAvgTime();
		if(nAveTime2 != descTimer2.nEscapeUSec)
			TERROR;
		GAIA::TIME::Timer::__MicroSecType nAveTime3 = cb3.GetAvgTime();
		if(nAveTime3 != descTimer3.nEscapeUSec)
			TERROR;
		GAIA::TIME::Timer::__MicroSecType nAveTime4 = cb4.GetAvgTime();
		if(nAveTime4 > 100)
			TERROR;

		/* Release instance. */
		GAIA_RELEASE_SAFE(pTimer3);
		gdel pTimerMgr;
	}
}
