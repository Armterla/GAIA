#ifndef		__GAIA_SYNC_LOCK_H__
#define		__GAIA_SYNC_LOCK_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_thread_base.h"

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
		/*!
			@brief Sync recursive lock.
		*/
		class Lock : public GAIA::Base
		{
		public:
			GINL Lock()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::InitializeCriticalSection(&m_cs);
			#else
				pthread_mutexattr_t attr;
				pthread_mutexattr_init(&attr);
				pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
				pthread_mutex_init(&m_mutex, &attr);
				pthread_mutexattr_destroy(&attr);
			#endif
			#ifdef GAIA_DEBUG_SELFCHECK
				m_owner_tid = (GAIA::UM)GINVALID;
				m_owner_entertimes = 0;
			#endif
			}
			GINL ~Lock()
			{
			#ifdef GAIA_DEBUG_SELFCHECK
			#	if GAIA_OS == GAIA_OS_WINDOWS
			#		pragma warning(push)
			#		pragma warning(disable : 4297)
			#	endif
				GAIA::UM origin_owner_tid = m_owner_tid;
				GAST(origin_owner_tid == (GAIA::UM)GINVALID);
				GAST(m_owner_entertimes == 0);
			#	if GAIA_OS == GAIA_OS_WINDOWS
			#		pragma warning(pop)
			#	endif
			#endif
			#if GAIA_OS == GAIA_OS_WINDOWS
				::DeleteCriticalSection(&m_cs);
			#else
				pthread_mutex_destroy(&m_mutex);
			#endif
			}
			GINL GAIA::GVOID Enter()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				::EnterCriticalSection(&m_cs);
			#else
				pthread_mutex_lock(&m_mutex);
			#endif
			#ifdef GAIA_DEBUG_SELFCHECK
				if(m_owner_entertimes == 0)
				{
					GAST(m_owner_tid == (GAIA::UM)GINVALID);
					m_owner_tid = GAIA::THREAD::threadid();
				}
				else
					GAST(m_owner_tid == GAIA::THREAD::threadid());
				++m_owner_entertimes;
			#endif
			}
			GINL GAIA::GVOID Leave()
			{
			#ifdef GAIA_DEBUG_SELFCHECK
				GAST(m_owner_entertimes > 0);
				if(m_owner_entertimes > 0)
					GAST(m_owner_tid == GAIA::THREAD::threadid());
				--m_owner_entertimes;
				if(m_owner_entertimes == 0)
					m_owner_tid = (GAIA::UM)GINVALID;
			#endif
			#if GAIA_OS == GAIA_OS_WINDOWS
				::LeaveCriticalSection(&m_cs);
			#else
				pthread_mutex_unlock(&m_mutex);
			#endif
			}
		private:
			GINL Lock(const Lock& src){}
		private:
		#if GAIA_OS == GAIA_OS_WINDOWS
			CRITICAL_SECTION m_cs;
		#else
			pthread_mutex_t m_mutex;
		#endif
		#ifdef GAIA_DEBUG_SELFCHECK
			GAIA::UM m_owner_tid;
			GAIA::NM m_owner_entertimes;
		#endif
		};
	}
}

#endif
