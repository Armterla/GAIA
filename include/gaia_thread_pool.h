#ifndef 	__GAIA_THREAD_POOL_H__
#define 	__GAIA_THREAD_POOL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_atomic.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_sync_event.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_queue.h"
#include "gaia_thread_base.h"
#include "gaia_thread.h"

namespace GAIA
{
	namespace THREAD
	{
		class ThreadPool : public GAIA::Base
		{
		public:
			class Task : public GAIA::RefObject
			{
				friend class ThreadPool;
			public:
				GINL Task(){m_uGroupIndex = GINVALID;}
				virtual GAIA::GVOID Run() = 0;
				GINL GAIA::GVOID SetGroupIndex(GAIA::U32 uGroupIndex){m_uGroupIndex = uGroupIndex;}
				GINL GAIA::U32 GetGroupIndex() const{return m_uGroupIndex;}
				GINL GAIA::GVOID Wait(){m_event.Wait((GAIA::U32)GINVALID);}
			private:
				GINL GAIA::GVOID FireComplete(){m_event.Fire();}
			private:
				GAIA::U32 m_uGroupIndex;
				GAIA::SYNC::Event m_event;
			};
			class WorkThread : public GAIA::THREAD::Thread
			{
				friend class ThreadPool;
			private:
				GAIA_ENUM_BEGIN(STOP_TYPE)
					STOP_TYPE_IMMEDIATELY,
					STOP_TYPE_WAITCOMPLETE,
				GAIA_ENUM_END(STOP_TYPE)
			private:
				WorkThread(){this->init();}
				~WorkThread(){this->destruct();}
				GINL GAIA::GVOID SetStopCmd(STOP_TYPE stoptype){m_stoptype = stoptype;}
				GINL STOP_TYPE GetStopCmd() const{return m_stoptype;}
				GINL GAIA::GVOID BeginPushTask(){m_lrTaskQueue.Enter();}
				GINL GAIA::GVOID EndPushTask(){m_lrTaskQueue.Leave();}
				GINL GAIA::GVOID FireTask(){m_event.Fire();}
				GINL GAIA::GVOID PushTask(GAIA::THREAD::ThreadPool::Task& task)
				{
					task.rise_ref();
					m_taskqueue.push_back(&task);
				}
				virtual GAIA::GVOID Run()
				{
					for(;;)
					{
						// Wait for work event.
						m_event.Wait((GAIA::U32)GINVALID);

						// If the ThreadPool want stop, stop it.
						STOP_TYPE stoptype = this->GetStopCmd();
						if(stoptype == STOP_TYPE_IMMEDIATELY)
							break;

						// Swap from queue to swapqueue.
						{
							GAIA::SYNC::Autolock al(m_lrTaskQueue);
							for(; !m_taskqueue.empty(); m_taskqueue.pop_front())
								m_taskqueueswap.push_back(m_taskqueue.front());
						}

						// Do swapqueue.
						for(; !m_taskqueueswap.empty(); m_taskqueueswap.pop_front())
						{
							GAIA::THREAD::ThreadPool::Task* pTask = m_taskqueueswap.front();
							GAST(pTask != GNIL);
							pTask->Run();
							pTask->FireComplete();
							pTask->drop_ref();
						}

						if(stoptype == STOP_TYPE_WAITCOMPLETE)
							break;
					}
				}
			private:
				GINL GAIA::GVOID init(){m_stoptype = STOP_TYPE_INVALID;}
				GINL GAIA::GVOID destruct()
				{
					GAIA::SYNC::Autolock al(m_lrTaskQueue);
					for(; !m_taskqueue.empty(); m_taskqueue.pop_front())
					{
						GAIA::THREAD::ThreadPool::Task* pTask = m_taskqueue.front();
						GAST(pTask != GNIL);
						pTask->drop_ref();
					}
					for(; !m_taskqueueswap.empty(); m_taskqueueswap.pop_front())
					{
						GAIA::THREAD::ThreadPool::Task* pTask = m_taskqueueswap.front();
						GAST(pTask != GNIL);
						pTask->drop_ref();
					}
				}
			private:
				typedef GAIA::CTN::Queue<GAIA::THREAD::ThreadPool::Task*> __TaskQueueType;
			private:
				GAIA::SYNC::Lock m_lrTaskQueue;
				GAIA::SYNC::Event m_event;
				__TaskQueueType m_taskqueue;
				__TaskQueueType m_taskqueueswap;
				STOP_TYPE m_stoptype;
			};
		public:
			GINL ThreadPool()
			{
				this->init();
			}
			GINL ~ThreadPool()
			{
				if(this->IsBegin())
					this->End();
				this->destruct();
			}
			GINL GAIA::BL SetThreadCount(GAIA::NUM sCount)
			{
				if(this->IsBegin())
					return GAIA::False;
				m_threads.resize(sCount);
				m_threads.reset(GNIL);
				return GAIA::True;
			}
			GINL GAIA::NUM GetThreadCount() const
			{
				return m_threads.size();
			}
			GINL GAIA::GVOID SetStackSize(GAIA::NUM size){m_uStackSize = size;}
			GINL GAIA::NUM GetStackSize() const{return m_uStackSize;}
			GINL GAIA::BL Begin()
			{
				if(this->IsBegin())
					return GAIA::False;
				GAST(this->GetThreadCount() != 0);
				if(this->GetThreadCount() == 0)
					return GAIA::False;
				for(__ThreadListType::it it = m_threads.frontit(); !it.empty(); ++it)
				{
					GAIA::THREAD::ThreadPool::WorkThread* pThread = gnew GAIA::THREAD::ThreadPool::WorkThread;
					*it = pThread;
					pThread->SetStackSize(m_uStackSize);
					pThread->Start();
				}
				m_bBegin = GAIA::True;
				return GAIA::True;
			}
			GINL GAIA::BL End(GAIA::BL bWaitComplete = GAIA::True)
			{
				if(!this->IsBegin())
					return GAIA::False;
				if(bWaitComplete)
					this->FlushTask();
				for(__ThreadListType::it it = m_threads.frontit(); !it.empty(); ++it)
				{
					GAIA::THREAD::ThreadPool::WorkThread* pThread = *it;
					GAST(pThread != GNIL);
					if(bWaitComplete)
						pThread->SetStopCmd(GAIA::THREAD::ThreadPool::WorkThread::STOP_TYPE_WAITCOMPLETE);
					else
						pThread->SetStopCmd(GAIA::THREAD::ThreadPool::WorkThread::STOP_TYPE_IMMEDIATELY);
					pThread->FireTask();
				}
				for(__ThreadListType::it it = m_threads.frontit(); !it.empty(); ++it)
				{
					GAIA::THREAD::ThreadPool::WorkThread* pThread = *it;
					GAST(pThread != GNIL);
					pThread->Wait();
					gdel pThread;
					*it = GNIL;
				}
				m_uLastGroupIndex = 0;
				m_bBegin = GAIA::False;
				return GAIA::True;
			}
			GINL GAIA::BL IsBegin() const{return m_bBegin;}
			GINL GAIA::U32 ReserveGroupIndex()
			{
				GAIA::SYNC::Autolock al(m_lrTaskQueue);
				GAIA::U32 uGroupIndex = m_uLastGroupIndex++;
				if(uGroupIndex == GINVALID)
					uGroupIndex = 0;
				return uGroupIndex;
			}
			/*!
				@brief Push a async task to thread pool.

				@remarks
					The task will not be executed until you call ThreadPool::FlushTask.
			*/
			GINL GAIA::U32 PushTask(GAIA::THREAD::ThreadPool::Task& task)
			{
				task.rise_ref();
				GAIA::U32 uGroupIndex = task.GetGroupIndex();
				GAIA::SYNC::Autolock al(m_lrTaskQueue);
				if(uGroupIndex == GINVALID)
				{
					uGroupIndex = m_uLastGroupIndex++;
					if(uGroupIndex == GINVALID)
						uGroupIndex = 0;
					task.SetGroupIndex(uGroupIndex);
				}
				m_taskqueue.push_back(&task);
				return uGroupIndex;
			}
			GINL GAIA::BL FlushTask()
			{
				if(!this->IsBegin())
					return GAIA::False;
				if(m_taskqueue.empty())
					return GAIA::False;
				GAST(!m_threads.empty());
				GAIA::SYNC::Autolock al(m_lrTaskQueue);
				while(!m_taskqueue.empty())
				{
					GAIA::THREAD::ThreadPool::Task* pTask = m_taskqueue.front();
					GAST(pTask != GNIL);
					m_taskqueue.pop_front();
					GAIA::U32 uThreadIndex = pTask->GetGroupIndex() % m_threads.size();
					GAIA::THREAD::ThreadPool::WorkThread* pThread = m_threads[uThreadIndex];
					GAST(pThread != GNIL);
					pThread->BeginPushTask();
					{
						pThread->PushTask(*pTask);
						pTask->drop_ref();
					}
					pThread->EndPushTask();
					pThread->FireTask();
				}
				return GAIA::True;
			}
			/*!
				@brief Run a async task.
			*/
			GINL GAIA::U32 RunTask(GAIA::THREAD::ThreadPool::Task& task)
			{
				GAIA::U32 uGroupIndex = task.GetGroupIndex();
				if(uGroupIndex == GINVALID)
				{
					GAIA::SYNC::Autolock al(m_lrTaskQueue);
					uGroupIndex = m_uLastGroupIndex++;
					if(uGroupIndex == GINVALID)
						uGroupIndex = 0;
					task.SetGroupIndex(uGroupIndex);
				}

				GAIA::U32 uThreadIndex = uGroupIndex % m_threads.size();
				GAIA::THREAD::ThreadPool::WorkThread* pThread = m_threads[uThreadIndex];
				GAST(pThread != GNIL);
				pThread->BeginPushTask();
				{
					pThread->PushTask(task);
				}
				pThread->EndPushTask();
				pThread->FireTask();
				return uGroupIndex;
			}
		private:
			GINL GAIA::GVOID init()
			{
				m_uStackSize = GAIA::THREAD_STACK_SIZE;
				m_uLastGroupIndex = 0;
				m_bBegin = GAIA::False;
			}
			GINL GAIA::GVOID destruct()
			{
				for(; !m_taskqueue.empty(); m_taskqueue.pop_front())
				{
					GAIA::THREAD::ThreadPool::Task* pTask = m_taskqueue.front();
					GAST(pTask != GNIL);
					pTask->drop_ref();
				}
			}
		private:
			typedef GAIA::CTN::Vector<GAIA::THREAD::ThreadPool::WorkThread*> __ThreadListType;
			typedef GAIA::CTN::Queue<GAIA::THREAD::ThreadPool::Task*> __TaskQueueType;
		private:
			__ThreadListType m_threads;
			GAIA::SYNC::Lock m_lrTaskQueue;
			__TaskQueueType m_taskqueue;
			GAIA::NUM m_uStackSize;
			GAIA::U32 m_uLastGroupIndex;
			GAIA::U8 m_bBegin : 1;
		};
	}
}

#endif
