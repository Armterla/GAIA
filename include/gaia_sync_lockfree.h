#ifndef		__GAIA_SYNC_LOCKFREE_H__
#define		__GAIA_SYNC_LOCKFREE_H__

#include "gaia_type.h"
#include "gaia_assert.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#else
#	include <pthread.h>
#endif

namespace GAIA
{
	namespace SYNC
	{
		class LockFree : public GAIA::Base
		{
		public:
			GINL LockFree()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::InitializeCriticalSection(&m_cs);
			#else
				pthread_mutex_init(&m_mutex, GNIL);
				pthread_cond_init(&m_cond, GNIL);
				m_reqcount = -1;
				m_owner_tid = GINVALID;
				m_owner_entertimes = 0;
			#endif
			}
			GINL ~LockFree()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::DeleteCriticalSection(&m_cs);
			#else
				pthread_mutex_destroy(&m_mutex);
				pthread_cond_destroy(&m_cond);
				GAST(m_reqcount == -1);
				GAST(m_owner_tid == GINVALID);
			#endif
			}
			GINL GAIA::GVOID Enter()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::EnterCriticalSection(&m_cs);
			#else
				pthread_mutex_lock(&m_mutex);
				{
					++m_reqcount;
					if(m_reqcount == 0)
					{
						GAST(m_owner_tid == GINVALID);
						m_owner_tid = GAIA::THREAD::threadid();
					}
					else // m_reqcount > 0
					{
						GAST(m_reqcount > 0);
						if(m_owner_tid != GAIA::THREAD::threadid())
						{
							pthread_cond_wait(&m_cond, &m_mutex);
							m_owner_tid = GAIA::THREAD::threadid();
						}
					}
					++m_owner_entertimes;
					GAST(m_reqcount >= 0);
				}
				pthread_mutex_unlock(&m_mutex);
			#endif
			}
			GINL GAIA::GVOID Leave()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::LeaveCriticalSection(&m_cs);
			#else
				pthread_mutex_lock(&m_mutex);
				{
					GAST(m_reqcount >= 0);
					GAST(m_owner_entertimes > 0);
					GAST(m_owner_tid != GINVALID);
					--m_owner_entertimes;
					if(m_owner_entertimes == 0)
					{
						m_owner_tid = GINVALID;
						if(m_reqcount > 0)
							pthread_cond_signal(&m_cond);
					}
					--m_reqcount;
				}
				pthread_mutex_unlock(&m_mutex);
			#endif
			}
		private:
			GINL LockFree(const LockFree& src){}
		private:
		#if GAIA_OS == GAIA_OS_WINDOWS
			CRITICAL_SECTION m_cs;
		#else
			pthread_mutex_t m_mutex;
			pthread_cond_t m_cond;
			GAIA::NM m_reqcount;
			GAIA::UM m_owner_tid;
			GAIA::NM m_owner_entertimes;
		#endif
		};
	}
}

#endif
