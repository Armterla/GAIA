#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	class AsyncSocketEx;
	class AsyncDispatcherEx;
	class AsyncCtx : public GAIA::Base
	{
	public:
		AsyncCtx()
		{
			pDispatcher = GNIL;
		}
	public:
		AsyncDispatcherEx* pDispatcher;
		GAIA::CTN::Map<GAIA::NETWORK::Addr, AsyncSocketEx*> listListenSockets;
		GAIA::CTN::Vector<AsyncSocketEx*> listConnectedSockets;
		GAIA::CTN::Vector<AsyncSocketEx*> listAcceptedSockets;
	};

	class AsyncDispatcherCollectCallBack : public GAIA::NETWORK::AsyncDispatcherCallBack
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

	class AsyncSocketEx : public GAIA::NETWORK::AsyncSocket
	{
	public:
		AsyncSocketEx(AsyncCtx& ctx, GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::ASYNC_SOCKET_TYPE socktype = GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED)
			: GAIA::NETWORK::AsyncSocket(disp, socktype)
		{
			m_ctx = &ctx;
		}

	protected:
		virtual GAIA::GVOID OnCreated(GAIA::BL bResult)
		{

		}
		virtual GAIA::GVOID OnClosed(GAIA::BL bResult)
		{

		}
		virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr)
		{

		}
		virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr)
		{

		}
		virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult)
		{

		}
		virtual GAIA::GVOID OnListened(GAIA::BL bResult)
		{

		}
		virtual GAIA::GVOID OnAccepted(GAIA::BL bResult, const GAIA::NETWORK::Addr& addrListen)
		{

		}
		virtual GAIA::GVOID OnSent(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::NUM sPracticeSize, GAIA::NUM sSize)
		{

		}
		virtual GAIA::GVOID OnRecved(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::NUM sSize)
		{

		}
		virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag)
		{

		}

	public:
		AsyncCtx* m_ctx;
	};

	class AsyncDispatcherEx : public GAIA::NETWORK::AsyncDispatcher
	{
	public:
		AsyncDispatcherEx(AsyncCtx& ctx){m_ctx = &ctx;}

	protected:
		virtual GAIA::NETWORK::AsyncSocket* OnCreateListenSocket()
		{
			AsyncSocketEx* pListenSocket = gnew AsyncSocketEx(*m_ctx, *this, GAIA::NETWORK::ASYNC_SOCKET_TYPE_LISTEN);
			return pListenSocket;
		}
		virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptedSocket()
		{
			AsyncSocketEx* pAcceptedSocket = gnew AsyncSocketEx(*m_ctx, *this, GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTED);
			return pAcceptedSocket;
		}
		virtual GAIA::BL OnAcceptSocket(GAIA::NETWORK::AsyncSocket& sock, const GAIA::NETWORK::Addr& addrListen)
		{
			return GAIA::False;
		}

	public:
		AsyncCtx* m_ctx;
	};

	extern GAIA::GVOID t_network_asyncsocket(GAIA::LOG::Log& logobj)
	{
		// Simple AsyncDispatcher.
		{
			GAIA::NETWORK::AsyncDispatcherDesc descDisp;
			descDisp.reset();
			TAST(descDisp.check());

			GAIA::NETWORK::AsyncDispatcher disp;
			TAST(!disp.IsCreated());

			TAST(disp.Create(descDisp));
			{
				TAST(disp.IsCreated());
				TAST(!disp.IsBegin());
				TAST(disp.Begin());
				{
					TAST(disp.IsBegin());
					GAIA::NETWORK::Addr addrListen1; addrListen1.fromstring("127.0.0.1:4001");
					GAIA::NETWORK::Addr addrListen2; addrListen2.fromstring("127.0.0.1:4002");
					TAST(disp.AddListenSocket(addrListen1));
					TAST(disp.AddListenSocket(addrListen2));
					TAST(disp.IsExistListenSocket(addrListen1));
					TAST(disp.IsExistListenSocket(addrListen2));
					TAST(disp.GetListenSocketCount() == 2);
					AsyncDispatcherCollectCallBack cb;
					TAST(disp.CollectListenSocket(cb));
					TAST(cb.listSocket.size() == 2);
					TAST(disp.RemoveListenSocket(addrListen1));
					TAST(disp.RemoveListenSocketAll());
					TAST(!disp.RemoveListenSocketAll());

					TAST(!disp.IsExistAcceptedSocket(addrListen1));
					TAST(disp.GetAcceptedSocketCount() == 0);
					cb.listSocket.clear();
					TAST(disp.CollectAcceptedSocket(cb));
					TAST(cb.listSocket.size() == 0);

					TAST(!disp.IsExistConnectedSocket(addrListen2));
					TAST(disp.GetConnectedSocketCount() == 0);
					cb.listSocket.clear();
					TAST(disp.CollectConnectedSocket(cb));
					TAST(cb.listSocket.size() == 0);
				}
				TAST(disp.End());
				TAST(!disp.IsBegin());
			}
			TAST(disp.Destroy());
			TAST(!disp.IsCreated());
		}

		// Simple AsyncSocket.
		{
			GAIA::NETWORK::AsyncDispatcher disp;
			GAIA::NETWORK::AsyncSocket sock1(disp);
			GAIA::NETWORK::AsyncSocket sock2(disp);
			TAST(sock1.GetAsyncSocketType() == GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED);

			GAIA::NETWORK::AsyncDispatcherDesc descDisp;
			descDisp.reset();
			TAST(disp.Create(descDisp));
			{
				TAST(disp.Begin());
				{
					GAIA::NETWORK::Addr addrListen1; addrListen1.fromstring("127.0.0.1:4003");
					GAIA::NETWORK::Addr addrListen2; addrListen2.fromstring("127.0.0.1:4004");
					TAST(disp.AddListenSocket(addrListen1));
					TAST(disp.AddListenSocket(addrListen2));

					sock1.Create();
					sock1.Connect(addrListen1);

					sock2.Create();
					sock2.Connect(addrListen1);

					sock1.Disconnect();
					sock2.Disconnect();
				}
				TAST(disp.End());
			}
			TAST(disp.Destroy());
		}

		// Multi connect, send, receive, close.
		{
			static const GAIA::NUM SOCKET_COUNT = 1000;

			AsyncCtx ctx;

			AsyncDispatcherEx disp(ctx);


			GAIA::NETWORK::AsyncDispatcherDesc descDisp;
			descDisp.reset();
			TAST(disp.Create(descDisp));
			{
				TAST(disp.Begin());
				{
					for(GAIA::NUM x = 0; x < SOCKET_COUNT; ++x)
					{

					}
				}
				TAST(disp.End());
			}
			TAST(disp.Destroy());
		}
	}
}