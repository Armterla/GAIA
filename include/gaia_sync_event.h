#ifndef		__GAIA_SYNC_EVENT_H__
#define		__GAIA_SYNC_EVENT_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
#	include <dispatch/dispatch.h>
#else
#	include <pthread.h>
#	include <sys/time.h>
#	include <sys/errno.h>
#endif

namespace GAIA
{
	namespace SYNC
	{
		class Event : public GAIA::Base
		{
		public:
			GINL Event()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				m_hSem = ::CreateSemaphore(GNIL, 0, 0x7FFFFFFF, GNIL);
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				m_sem = dispatch_semaphore_create(0);
			#else
				pthread_mutex_init(&m_mutex, GNIL);
				pthread_cond_init(&m_cond, GNIL);
				m_waitcnt = 0;
			#endif
			}
			GINL ~Event()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::CloseHandle(m_hSem);
			#elif GAIA_OS == GAIA_OS_OSX
			#	if !__has_feature(objc_arc)
					dispatch_release(m_sem);
			#	endif
			#elif GAIA_OS == GAIA_OS_IOS
			#	if !__has_feature(objc_arc)
					dispatch_release(m_sem);
			#	endif
			#else
				pthread_mutex_destroy(&m_mutex);
				pthread_cond_destroy(&m_cond);
			#endif
			}
			GINL GAIA::GVOID Fire()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::ReleaseSemaphore(m_hSem, 1, GNIL);
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				dispatch_semaphore_signal(m_sem);
			#else
				pthread_mutex_lock(&m_mutex);
				{
					if(m_waitcnt > 0)
						pthread_cond_signal(&m_cond);
					--m_waitcnt;
				}
				pthread_mutex_unlock(&m_mutex);
			#endif
			}
			GINL GAIA::BL Wait(GAIA::U32 uMilliSeconds = GINVALID)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				return ::WaitForSingleObject(m_hSem, uMilliSeconds) == WAIT_OBJECT_0;
			#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
				if(uMilliSeconds == GINVALID)
					return dispatch_semaphore_wait(m_sem, DISPATCH_TIME_FOREVER) == 0;
				else
				{
					dispatch_time_t waittime = dispatch_time(DISPATCH_TIME_NOW, (GAIA::U64)uMilliSeconds * 1000L * 1000L);
					return dispatch_semaphore_wait(m_sem, waittime) == 0;
				}
			#else
				GAIA::BL ret = GAIA::False;
				pthread_mutex_lock(&m_mutex);
				{
					++m_waitcnt;
					if(m_waitcnt > 0)
					{
						if(uMilliSeconds == 0xFFFFFFFF)
						{
							if(pthread_cond_wait(&m_cond, &m_mutex) == 0)
								ret = GAIA::True;
						}
						else
						{
							timeval now;
							gettimeofday(&now, GNIL);
							timespec abstime;
							abstime.tv_nsec = now.tv_usec * 1000 + (uMilliSeconds % 1000) * 1000 * 1000;
							abstime.tv_sec = now.tv_sec + uMilliSeconds / 1000;
							abstime.tv_sec += abstime.tv_nsec / (1000 * 1000 * 1000);
							abstime.tv_nsec = abstime.tv_nsec % (1000 * 1000 * 1000);
							GAIA::N32 nWaitResult = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime);
							if(nWaitResult == ETIMEDOUT)
								--m_waitcnt;
							else if(nWaitResult == 0)
								ret = GAIA::True;
						}
					}
					else
						ret = GAIA::True;
				}
				pthread_mutex_unlock(&m_mutex);
				return ret;
			#endif
			}
		private:
			GINL Event(const Event& src){}
		private:
		#if GAIA_OS == GAIA_OS_WINDOWS
			HANDLE m_hSem;
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
			dispatch_semaphore_t m_sem;
		#else
			pthread_mutex_t m_mutex;
			pthread_cond_t m_cond;
			GAIA::NM m_waitcnt;
		#endif
		};
	}
}

#endif
