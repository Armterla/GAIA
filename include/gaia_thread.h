#ifndef		__GAIA_THREAD_H__
#define		__GAIA_THREAD_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_string.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#else
#	include <pthread.h>
#	include <signal.h>
#endif

namespace GAIA
{
	namespace THREAD
	{
	#if GAIA_OS == GAIA_OS_WINDOWS // COME FROM MSDN https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
	#	pragma pack(push,8)
		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType; // Must be 0x1000.
			LPCSTR szName; // Pointer to name (in user addr space).
			DWORD dwThreadID; // Thread ID (-1=caller thread).
			DWORD dwFlags; // Reserved for future use, must be zero.
		}THREADNAME_INFO;
	#	pragma pack(pop)
	#endif

		class Thread;
	#if GAIA_OS == GAIA_OS_WINDOWS
		GINL static DWORD WINAPI gaia_native_thread_procedure(GAIA::GVOID* p);
	#else
		GINL static GAIA::GVOID* gaia_native_thread_procedure(GAIA::GVOID* p);
	#endif
		class Thread : public GAIA::Base
		{
		public:
			GAIA_ENUM_BEGIN(STATE)
				STATE_RUNNING,
			GAIA_ENUM_END(STATE)
		public:
			GINL Thread(){this->init();}
			GINL virtual ~Thread()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(m_hThread != GNIL)
				{
					::WaitForSingleObject(m_hThread, INFINITE);
					::CloseHandle(m_hThread);
				}
			#else
				if(m_bCreated)
					pthread_join(m_thread, GNIL);
			#endif
			}
			GINL GAIA::GVOID SetStackSize(GAIA::NUM size)
			{
			#ifdef GAIA_DEBUG_PLATFORM
				GAST(m_stacksize > 0);
				GAST(m_stacksize < 1024 * 1024 * 16);
				GAST(m_stacksize % 1024 == 0);
				GAST(m_stacksize / 1024 >= 10);
			#endif
				m_stacksize = size;
			}
			GINL GAIA::NUM GetStackSize() const{return m_stacksize;}
			GINL GAIA::GVOID SetName(const GAIA::CH* pszName){m_strName = pszName;}
			GINL const GAIA::CH* GetName(){return m_strName;}
			GINL STATE GetState() const{return m_state;}
			GINL GAIA::UM GetThreadID() const{return m_uThreadID;}
			GINL GAIA::BL Start()
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(m_hThread != GNIL)
					return GAIA::False;
				m_state = STATE_RUNNING;
				DWORD dwThreadID;
				m_hThread = ::CreateThread(GNIL, m_stacksize, gaia_native_thread_procedure, GSCAST(GAIA::GVOID*)(this), 0, &dwThreadID);
				if(m_hThread == GNIL)
					return GAIA::False;
				m_uThreadID = (GAIA::UM)dwThreadID;
				if(!m_strName.empty())
				{
					const DWORD MS_VC_EXCEPTION = 0x406D1388;
					THREADNAME_INFO info;
					info.dwType = 0x1000;
					info.szName = m_strName;
					info.dwThreadID = ::GetThreadId(m_hThread);
					info.dwFlags = 0;
					__try{RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);}
					__except(EXCEPTION_EXECUTE_HANDLER){}
				}
				return GAIA::True;
			#else
				if(m_bCreated)
					return GAIA::False;
				pthread_attr_t attr;
				if(pthread_attr_init(&attr) != 0)
					return GAIA::False;
				if(pthread_attr_setstacksize(&attr, m_stacksize) != 0)
					return GAIA::False;
				m_state = STATE_RUNNING;
				if(pthread_create(&m_thread, &attr, gaia_native_thread_procedure, GSCAST(GAIA::GVOID*)(this)) != 0)
					return GAIA::False;
				pthread_attr_destroy(&attr);
				m_uThreadID = (GAIA::UM)m_thread;
				m_bCreated = GAIA::True;
				return GAIA::True;
			#endif
			}
			GINL GAIA::BL Wait() const
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				if(m_hThread != GNIL)
				{
					if(::WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0)
					{
						::CloseHandle(m_hThread);
						(const_cast<Thread*>(this))->m_hThread = GNIL;
						(const_cast<Thread*>(this))->m_state = STATE_INVALID;
						(const_cast<Thread*>(this))->m_uThreadID = GINVALID;
						return GAIA::True;
					}
				}
			#else
				if(m_bCreated)
				{
					pthread_join(m_thread, GNIL);
					(const_cast<Thread*>(this))->m_bCreated = GAIA::False;
					(const_cast<Thread*>(this))->m_state = STATE_INVALID;
					(const_cast<Thread*>(this))->m_uThreadID = GINVALID;
					return GAIA::True;
				}
			#endif
				return GAIA::False;
			}
			virtual GAIA::GVOID Run() = 0;
		private:
			GINL GAIA::GVOID init()
			{
				m_stacksize = THREAD_STACK_SIZE;
				m_state = STATE_INVALID;
				m_uThreadID = GINVALID;
			#if GAIA_OS == GAIA_OS_WINDOWS
				m_hThread = GNIL;
			#else
				m_bCreated = GAIA::False;
			#endif
			}
		private:
			GAIA::NUM m_stacksize;
			STATE m_state;
			GAIA::UM m_uThreadID;
		#if GAIA_OS == GAIA_OS_WINDOWS
			HANDLE m_hThread;
		#else
			pthread_t m_thread;
			GAIA::U8 m_bCreated : 1;
		#endif
			GAIA::CTN::AString m_strName;
		};
	#if GAIA_OS == GAIA_OS_WINDOWS
		GINL static DWORD WINAPI gaia_native_thread_procedure(GAIA::GVOID* p)
		{
			Thread* pThread = GSCAST(Thread*)(p);
			GAST(GAIA::THREAD::threadid() == pThread->GetThreadID());
			pThread->Run();
			return 0;
		}
	#else
		GINL static GAIA::GVOID* gaia_native_thread_procedure(GAIA::GVOID* p)
		{
			signal(SIGPIPE, SIG_IGN);
			Thread* pThread = GSCAST(Thread*)(p);
			GAST(GAIA::THREAD::threadid() == pThread->GetThreadID());
			pThread->Run();
			pthread_exit(0);
		}
	#endif
	}
}

#endif
