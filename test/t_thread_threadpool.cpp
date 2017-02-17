#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class ThreadPoolTask : public GAIA::THREAD::ThreadPool::Task
	{
	public:
		GINL GAIA::GVOID SetValue(GAIA::NUM* pValue){m_pValue = pValue;}
		GINL GAIA::NUM* GetValue() const{return m_pValue;}
		GINL GAIA::GVOID SetLock(GAIA::SYNC::Lock* pLock){m_pLock = pLock;}
		GINL GAIA::SYNC::Lock* GetLock() const{return m_pLock;}
		virtual GAIA::GVOID Run()
		{
			GAST(m_pValue != GNIL);
			GAST(m_pLock != GNIL);
			m_pLock->Enter();
			{
				++(*m_pValue);
			}
			m_pLock->Leave();
		}
	private:
		GINL GAIA::GVOID init(){m_pValue = GNIL; m_pLock = GNIL;}
	private:
		GAIA::NUM* m_pValue;
		GAIA::SYNC::Lock* m_pLock;
	};
	extern GAIA::GVOID t_thread_threadpool(GAIA::LOG::Log& logobj)
	{
		GAIA::SYNC::Lock lr;
		GAIA::NUM sValue = 0;
		GAIA::CTN::Vector<ThreadPoolTask> listTask;
		listTask.resize(123);
		for(GAIA::NUM x = 0; x < listTask.size(); ++x)
		{
			listTask[x].SetValue(&sValue);
			listTask[x].SetLock(&lr);
		}

		GAIA::THREAD::ThreadPool tp;
		tp.SetThreadCount(7);
		tp.SetStackSize(1024 * 100);

		TAST(tp.Begin());
		TAST(tp.End());

		TAST(tp.Begin());
		{
			for(GAIA::NUM x = 0; x < listTask.size(); ++x)
				tp.RunTask(listTask[x]);
			for(GAIA::NUM x = 0; x < listTask.size(); ++x)
				tp.PushTask(listTask[x]);
			tp.FlushTask();
		}
		TAST(tp.End());
		if(sValue != listTask.size() * 2)
			TERROR;
	}
}
