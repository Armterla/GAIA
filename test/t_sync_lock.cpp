#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class ThdLock : public GAIA::THREAD::Thread
	{
	public:
		GINL ThdLock(){this->init();}
		virtual GAIA::GVOID Run()
		{
			for(GAIA::NUM x = 0; x < 10; ++x)
			{
				GAIA::SYNC::Autolock al(*m_pLock);
				for(GAIA::NUM y = 0; y < 10; ++y)
				{
					GAIA::SYNC::Autolock al(*m_pLock);
					for(GAIA::NUM z = 0; z < 10; ++z)
					{
						GAIA::SYNC::Autolock al(*m_pLock);
						*m_p += 1;
					}
				}
			}
		}
		GINL GAIA::GVOID SetParam(GAIA::SYNC::Lock* pLock, GAIA::U32* p){m_pLock = pLock; m_p = p;}
	private:
		GINL GAIA::GVOID init(){m_pLock = GNIL; m_p = GNIL;}
	private:
		GAIA::SYNC::Lock* m_pLock;
		GAIA::U32* m_p;
	};

	class ThdLockPure : public GAIA::THREAD::Thread
	{
	public:
		GINL ThdLockPure(){this->init();}
		virtual GAIA::GVOID Run()
		{
			for(GAIA::NUM x = 0; x < 10; ++x)
			{
				GAIA::SYNC::AutolockPure al(*m_pLock);
				for(GAIA::NUM y = 0; y < 10; ++y)
				{
					for(GAIA::NUM z = 0; z < 10; ++z)
						*m_p += 1;
				}
			}
		}
		GINL GAIA::GVOID SetParam(GAIA::SYNC::LockPure* pLock, GAIA::U32* p){m_pLock = pLock; m_p = p;}
	private:
		GINL GAIA::GVOID init(){m_pLock = GNIL; m_p = GNIL;}
	private:
		GAIA::SYNC::LockPure* m_pLock;
		GAIA::U32* m_p;
	};
	
	class ThdLockW : public GAIA::THREAD::Thread
	{
	public:
		GINL ThdLockW(){this->init();}
		virtual GAIA::GVOID Run()
		{
			for(GAIA::NUM x = 0; x < 10; ++x)
			{
				GAIA::SYNC::AutolockW al(*m_pLockRW);
				for(GAIA::NUM y = 0; y < 10; ++y)
				{
					GAIA::SYNC::AutolockW al(*m_pLockRW);
					for(GAIA::NUM z = 0; z < 10; ++z)
					{
						GAIA::SYNC::AutolockW al(*m_pLockRW);
						*m_p += 1;
					}
				}
			}
		}
		GINL GAIA::GVOID SetParam(GAIA::SYNC::LockRW* pLockRW, GAIA::U32* p){m_pLockRW = pLockRW; m_p = p;}
	private:
		GINL GAIA::GVOID init(){m_pLockRW = GNIL; m_p = GNIL;}
	private:
		GAIA::SYNC::LockRW* m_pLockRW;
		GAIA::U32* m_p;
	};
	
	class ThdLockR : public GAIA::THREAD::Thread
	{
	public:
		GINL ThdLockR(){this->init();}
		virtual GAIA::GVOID Run()
		{
			for(GAIA::NUM x = 0; x < 10; ++x)
			{
				GAIA::SYNC::AutolockR al(*m_pLockRW);
				for(GAIA::NUM y = 0; y < 10; ++y)
				{
					GAIA::SYNC::AutolockR al(*m_pLockRW);
					for(GAIA::NUM z = 0; z < 10; ++z)
						GAIA::SYNC::AutolockR al(*m_pLockRW);
				}
			}
		}
		GINL GAIA::GVOID SetParam(GAIA::SYNC::LockRW* pLockRW, GAIA::U32* p){m_pLockRW = pLockRW; m_p = p;}
	private:
		GINL GAIA::GVOID init(){m_pLockRW = GNIL; m_p = GNIL;}
	private:
		GAIA::SYNC::LockRW* m_pLockRW;
		GAIA::U32* m_p;
	};

	extern GAIA::GVOID t_sync_lock(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM THREAD_COUNT = 10;
		
		// Lock test.
		{
			GAIA::SYNC::Lock l;
			for(GAIA::NUM x = 0; x < 10; ++x)
				l.Enter();
			for(GAIA::NUM x = 0; x < 10; ++x)
				l.Leave();
			
			GAIA::U32 u = 0;
			ThdLock t[THREAD_COUNT];
			
			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
				t[x].SetParam(&l, &u);
			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
				t[x].Start();
			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
				t[x].Wait();
			if(u != THREAD_COUNT * 1000)
				TERROR;
		}

		// LockPure test.
		{
			GAIA::SYNC::LockPure l;
			l.Enter();
			l.Leave();

			GAIA::U32 u = 0;
			ThdLockPure t[THREAD_COUNT];

			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
				t[x].SetParam(&l, &u);
			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
				t[x].Start();
			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
				t[x].Wait();
			if(u != THREAD_COUNT * 1000)
				TERROR;
		}
		
		// LockRW test.
		{
			GAIA::SYNC::LockRW l;
			for(GAIA::NUM x = 0; x < 10; ++x)
				l.EnterWrite();
			for(GAIA::NUM x = 0; x < 10; ++x)
				l.LeaveWrite();
			
			for(GAIA::NUM x = 0; x < 10; ++x)
				l.EnterRead();
			for(GAIA::NUM x = 0; x < 10; ++x)
				l.LeaveRead();
			
			l.EnterRead();
			l.EnterWrite();
			l.LeaveWrite();
			l.LeaveRead();
			
			GAIA::U32 u = 0;
			ThdLockW tW[THREAD_COUNT];
			ThdLockR tR[THREAD_COUNT];
			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
			{
				tW[x].SetParam(&l, &u);
				tR[x].SetParam(&l, &u);
			}
			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
			{
				tW[x].Start();
				tR[x].Start();
			}
			for(GAIA::NUM x = 0; x < THREAD_COUNT; ++x)
			{
				tW[x].Wait();
				tR[x].Wait();
			}
			if(u != THREAD_COUNT * 1000)
				TERROR;
		}
	}
}
