#ifndef		__GAIA_SYNC_EVENT_H__
#define		__GAIA_SYNC_EVENT_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
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
			#else
				pthread_mutex_destroy(&m_mutex);
				pthread_cond_destroy(&m_cond);
			#endif
			}
			GINL GAIA::GVOID Fire()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::ReleaseSemaphore(m_hSem, 1, GNIL);
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
		#else
			pthread_mutex_t m_mutex;
			pthread_cond_t m_cond;
			GAIA::NM m_waitcnt;
		#endif
		};
	}
}

#endif
