#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_asyncdispatcher.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

#if GAIA_OS == GAIA_OS_WINDOWS

#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
#	include <sys/types.h>
#	include <sys/event.h>
#	include <sys/time.h>
#	include <unistd.h>
#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

#endif

namespace GAIA
{
	namespace NETWORK
	{
		AsyncDispatcher::AsyncDispatcher()
		{
			this->init();
		}

		AsyncDispatcher::~AsyncDispatcher()
		{
			if(this->IsBegin())
				this->End();
		}

		GAIA::BL AsyncDispatcher::Begin()
		{
			if(this->IsBegin())
				return GAIA::False;

			// Create async controller in OS.
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			m_kqueue = kqueue();
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			m_epoll = epoll_create();
		#endif
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::End()
		{
			if(!this->IsBegin())
				return GAIA::False;

			// Close async controller in OS.
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			close(m_kqueue);
			m_kqueue = GINVALID;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			close(m_epoll);
			m_epoll = GINVALID;
		#endif
			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsBegin() const
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			if(m_kqueue != GINVALID)
				return GAIA::True;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			if(m_epoll != GINVALID)
				return GAIA::True;
		#endif
			return GAIA::False;
		}

		GAIA::BL AsyncDispatcher::AddAsyncSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwSockets);

			// 

			//
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveAsyncSocket(GAIA::NETWORK::AsyncSocket& sock)
		{
			GAIA::SYNC::AutolockW al(m_rwSockets);

			//
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

			//

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::RemoveAsyncSocketAll()
		{
			GAIA::SYNC::AutolockW al(m_rwSockets);

			//
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif

			//

			return GAIA::True;
		}

		GAIA::BL AsyncDispatcher::IsExistAsyncSocket(GAIA::NETWORK::AsyncSocket& sock) const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwSockets);

			//

			//

			return GAIA::True;
		}

		GAIA::NUM AsyncDispatcher::GetAsyncSocketCount() const
		{
			GAIA::SYNC::AutolockR al(GCCAST(AsyncDispatcher*)(this)->m_rwSockets);
			return m_nodes_byaddrptr.size();
		}

		GAIA::GVOID AsyncDispatcher::init()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS
			
		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX
			m_kqueue = GINVALID;
		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			m_epoll = GINVALID;
		#endif
		}
	}
}
