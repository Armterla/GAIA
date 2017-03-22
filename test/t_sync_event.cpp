#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class ThdWaitEvent : public GAIA::THREAD::Thread
	{
	public:
		GINL ThdWaitEvent(GAIA::SYNC::Event* pEvent, GAIA::NUM sFireCount)
		{
			this->init();
			m_pEvent = pEvent;
			m_sFireCount = sFireCount;
		}
		virtual GAIA::GVOID Run()
		{
			for(GAIA::NUM x = 0; x < m_sFireCount; ++x)
			{
				m_pEvent->Wait((GAIA::U32)GINVALID);
				if(this->GetState() == GAIA::THREAD::Thread::STATE_RUNNING)
					++m_sData;
				GAIA::SYNC::gsleep(1);
			}
		}
		GINL GAIA::NUM GetData() const{return m_sData;}
	private:
		GINL GAIA::GVOID init()
		{
			m_sData = 0;
			m_pEvent = GNIL;
			m_sFireCount = 0;
		}
	private:
		GAIA::NUM m_sData;
		GAIA::SYNC::Event* m_pEvent;
		GAIA::NUM m_sFireCount;
	};

	class ThdFireEvent : public GAIA::THREAD::Thread
	{
	public:
		GINL ThdFireEvent(GAIA::SYNC::Event* pEvent, GAIA::NUM sFireCount)
		{
			this->init();
			m_pEvent = pEvent;
			m_sFireCount = sFireCount;
		}
		virtual GAIA::GVOID Run()
		{
			for(GAIA::NUM x = 0; x < m_sFireCount; ++x)
			{
				m_pEvent->Fire();
				GAIA::SYNC::gsleep(3);
			}
		}
	private:
		GINL GAIA::GVOID init()
		{
			m_pEvent = GNIL;
			m_sFireCount = 0;
		}
	private:
		GAIA::SYNC::Event* m_pEvent;
		GAIA::NUM m_sFireCount;
	};
	
	class MultiThdWaitEvent : public GAIA::THREAD::Thread
	{
	public:
		GINL MultiThdWaitEvent()
		{
			this->init();
		}
		GINL GAIA::GVOID SetEvent(GAIA::SYNC::Event* pEvent)
		{
			GAST(pEvent != GNIL);
			m_pEvent = pEvent;
		}
		GINL GAIA::BL IsComplete() const{return m_bComplete;}
		virtual GAIA::GVOID Run()
		{
			m_pEvent->Wait((GAIA::U32)GINVALID);
			m_bComplete = GAIA::True;
		}
	private:
		GINL GAIA::GVOID init()
		{
			m_pEvent = GNIL;
			m_bComplete = GAIA::False;
		}
	private:
		GAIA::SYNC::Event* m_pEvent;
		GAIA::BL m_bComplete;
	};

	extern GAIA::GVOID t_sync_event(GAIA::LOG::Log& logobj)
	{
		GAIA::SYNC::Event e;
		if(e.Wait(10))
			TERROR;
		e.Fire();
		TAST(e.Wait((GAIA::U32)GINVALID));
		
		e.Fire();
		e.Fire();
		TAST(e.Wait(10));
		TAST(e.Wait(10));

		static const GAIA::NUM FIRE_COUNT = 10;
		ThdWaitEvent tWait(&e, FIRE_COUNT);
		ThdFireEvent tFire(&e, FIRE_COUNT);
		if(!tWait.Start() || !tFire.Start())
			TERROR;
		if(!tWait.Wait() || !tFire.Wait())
			TERROR;
		if(tWait.GetData() != FIRE_COUNT)
			TERROR;
		
		MultiThdWaitEvent thds[4];
		for(GAIA::NUM x = 0; x < sizeofarray(thds); ++x)
			thds[x].SetEvent(&e);
		for(GAIA::NUM x = 0; x < sizeofarray(thds); ++x)
			thds[x].Start();
		e.Fire();
		GAIA::SYNC::gsleep(50);
		GAIA::NUM sCompleteThreadCount = 0;
		for(GAIA::NUM x = 0; x < sizeofarray(thds); ++x)
		{
			if(thds[x].IsComplete())
				++sCompleteThreadCount;
		}
		if(sCompleteThreadCount != 1)
			TERROR;
		for(GAIA::NUM x = 0; x < sizeofarray(thds) - 1; ++x)
			e.Fire();
		for(GAIA::NUM x = 0; x < sizeofarray(thds); ++x)
			thds[x].Wait();
	}
}
