#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class AsyncDispatcherCollectCallBack : public GAIA::NETWORK::AsyncDispatcher::CallBack
	{
	public:
		virtual GAIA::BL OnCollectAsyncSocket(GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::AsyncSocket& sock)
		{
			listSocket.push_back(&sock);
			return GAIA::True;
		}
	public:
		GAIA::CTN::Vector<GAIA::NETWORK::AsyncSocket*> listSocket;
	};

	extern GAIA::GVOID t_network_asyncsocket(GAIA::LOG::Log& logobj)
	{
		// Simple AsyncDispatcher.
		{
			GAIA::NETWORK::AsyncDispatcher::Desc descDisp;
			descDisp.reset();
			if(!descDisp.check())
				TERROR;

			GAIA::NETWORK::AsyncDispatcher disp;
			if(disp.IsCreated())
				TERROR;
			if(!disp.Create(descDisp))
				TERROR;
			{
				if(disp.IsBegin())
					TERROR;
				if(!disp.Begin())
					TERROR;
				if(!disp.IsBegin())
					TERROR;
				{
					GAIA::NETWORK::Addr addrListen1; addrListen1.fromstring("127.0.0.1:4001");
					GAIA::NETWORK::Addr addrListen2; addrListen2.fromstring("127.0.0.1:4002");
					if(!disp.AddListenSocket(addrListen1))
						TERROR;
					if(!disp.AddListenSocket(addrListen2))
						TERROR;
					if(!disp.IsExistListenSocket(addrListen1))
						TERROR;
					if(!disp.IsExistListenSocket(addrListen2))
						TERROR;
					if(disp.GetListenSocketCount() != 2)
						TERROR;
					AsyncDispatcherCollectCallBack cb;
					if(!disp.CollectListenSocket(cb))
						TERROR;
					if(cb.listSocket.size() != 2)
						TERROR;
					if(!disp.RemoveListenSocket(addrListen1))
						TERROR;
					if(!disp.RemoveListenSocketAll())
						TERROR;
					if(disp.RemoveListenSocketAll())
						TERROR;
					if(disp.IsExistAcceptedSocket(addrListen1))
						TERROR;
					if(disp.GetAcceptedSocketCount() != 0)
						TERROR;
					cb.listSocket.clear();
					if(!disp.CollectAcceptedSocket(cb))
						TERROR;
					if(cb.listSocket.size() != 2)
						TERROR;
				}
				if(!disp.End())
					TERROR;
				if(disp.IsBegin())
					TERROR;
			}
			if(!disp.IsCreated())
				TERROR;
			if(!disp.Destroy())
				TERROR;
			if(disp.IsCreated())
				TERROR;
		}

		// Simple AsyncSocket.
		{
			GAIA::NETWORK::AsyncDispatcher disp;
			GAIA::NETWORK::AsyncSocket sock(disp);
			if(sock.GetAsyncSocketType() != GAIA::NETWORK::AsyncSocket::ASYNC_SOCKET_TYPE_CONNECTED)
				TERROR;
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