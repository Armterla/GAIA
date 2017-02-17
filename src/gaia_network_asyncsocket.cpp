#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_asyncsocket.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		AsyncSocket::AsyncSocket()
		{
			this->init();
		}

		AsyncSocket::~AsyncSocket()
		{

		}

		GAIA::GVOID AsyncSocket::Create()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Close()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Shutdown(GAIA::N32 nShutdownFlag)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Bind(const GAIA::NETWORK::Addr& addr)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::BL AsyncSocket::IsBind() const
		{
			return GAIA::False;
		}

		GAIA::GVOID AsyncSocket::Connect(const GAIA::NETWORK::Addr& addr)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Disconnect()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::BL AsyncSocket::IsConnected() const
		{
			return GAIA::False;
		}

		GAIA::GVOID AsyncSocket::Send(const GAIA::GVOID* pData, GAIA::NUM sSize)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Recv(GAIA::GVOID* pData, GAIA::NUM sSize)
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID

		#endif
		}

		GAIA::GVOID AsyncSocket::Flush()
		{
		#if GAIA_OS == GAIA_OS_WINDOWS

		#elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS || GAIA_OS == GAIA_OS_UNIX

		#elif GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_ANDROID
			
		#endif
		}

		GAIA::N32 AsyncSocket::GetFileDescriptor() const
		{
			return m_sock.GetFileDescriptor();
		}

		GAIA::BL AsyncSocket::GetGlobalAddress(GAIA::NETWORK::Addr& addr)
		{
			return m_sock.GetGlobalAddress(addr);
		}

		GAIA::BL AsyncSocket::GetLocalAddress(GAIA::NETWORK::Addr& addr)
		{
			return m_sock.GetLocalAddress(addr);
		}

		GAIA::GVOID AsyncSocket::init()
		{

		}

		GAIA::GVOID test()
		{
			
		}
	}
}
