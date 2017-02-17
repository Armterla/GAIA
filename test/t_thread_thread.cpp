#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class Thd : public GAIA::THREAD::Thread
	{
	public:
		GINL Thd(){this->init();}
		virtual GAIA::GVOID Run()
		{
			if(this->GetState() == GAIA::THREAD::Thread::STATE_RUNNING)
				m_sData = 1;
		}
		GINL GAIA::NUM GetData() const{return m_sData;}
	private:
		GINL GAIA::GVOID init(){m_sData = 0;}
	private:
		GAIA::NUM m_sData;
	};

	extern GAIA::GVOID t_thread_thread(GAIA::LOG::Log& logobj)
	{
		Thd t;
		t.SetName("GAIA THREAD TEST : Thd");
		t.SetStackSize(1024 * 128);
		if(t.GetStackSize() != 1024 * 128)
			TERROR;
		if(t.GetState() != GAIA::THREAD::Thread::STATE_INVALID)
			TERROR;
		if(!t.Start())
			TERROR;
		if(!t.Wait())
			TERROR;
		if(t.GetState() != GAIA::THREAD::Thread::STATE_INVALID)
			TERROR;
		if(t.GetData() != 1)
			TERROR;
	}
}
