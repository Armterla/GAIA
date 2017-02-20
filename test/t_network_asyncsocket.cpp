#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_network_asyncsocket(GAIA::LOG::Log& logobj)
	{
		// Simple AsyncDispatcher.
		{
			GAIA::NETWORK::AsyncDispatcher disp;
			if(disp.IsBegin())
				TERROR;
			if(!disp.Begin())
				TERROR;
			if(!disp.IsBegin())
				TERROR;
			if(!disp.End())
				TERROR;
		}

		// Simple AsyncSocket.
		{
			GAIA::NETWORK::AsyncDispatcher disp;
			GAIA::NETWORK::AsyncSocket sock(disp);
		}

		// Multi connect, send, receive, close.
		{
			static const GAIA::NUM SOCKET_COUNT = 1000;

			GAIA::NETWORK::AsyncDispatcher disp;
			GAIA::CTN::Vector<GAIA::NETWORK::AsyncSocket*> listSockets;

			for(GAIA::NUM x = 0; x < SOCKET_COUNT; ++x)
			{

			}
		}
	}
}