#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	static const GAIA::NUM LISTEN_SOCKET_COUNT = 10;
	static const GAIA::NUM DATA_SOCKET_COUNT = 100;
	static const GAIA::NUM DATA_TRANSFER_COUNT = 1000;
	static const GAIA::NUM START_PORT = 10000;

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
		GAIA::SYNC::Lock lrListenSocket;
		GAIA::SYNC::Lock lrConnectedSocket;
		GAIA::SYNC::Lock lrAcceptedSocket;
		GAIA::CTN::Vector<GAIA::CTN::Pair<GAIA::NETWORK::Addr, AsyncSocketEx*> > listListenSockets;
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
			m_bNoMoreCallBack = GAIA::False;
		}

		virtual GAIA::GVOID Create()
		{
			AsyncSocket::Create();

		#if GAIA_OS != GAIA_OS_WINDOWS
			this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_SENDBUFSIZE, 1024 * 1024);
			this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_RECVBUFSIZE, 1024 * 1024);
			this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_NOBLOCK, GAIA::True);
		#endif
		}

	protected:
		virtual GAIA::GVOID OnCreated(GAIA::BL bResult)
		{
			m_nCreatedCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nCreatedCount[2]++;
		}
		virtual GAIA::GVOID OnClosed(GAIA::BL bResult)
		{
			m_nClosedCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nClosedCount[2]++;
		}
		virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr)
		{
			m_nBoundCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nBoundCount[2]++;
		}
		virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr)
		{
			m_nConnectedCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nConnectedCount[2]++;
		}
		virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult)
		{
			m_nDisconnectedCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nDisconnectedCount[2]++;
		}
		virtual GAIA::GVOID OnListened(GAIA::BL bResult)
		{
			m_nListenCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nListenCount[2]++;
		}
		virtual GAIA::GVOID OnAccepted(GAIA::BL bResult, const GAIA::NETWORK::Addr& addrListen)
		{
			m_nAcceptedCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nAcceptedCount[2]++;
		}
		virtual GAIA::GVOID OnSent(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nPracticeSize, GAIA::N32 nSize)
		{
			if(bResult)
			{
				if(nPracticeSize > 0)
				{
					GAIA::SYNC::Autolock al(m_lrSendBuf);
					m_sendbuf.write(pData, nPracticeSize);
				}
			}
			m_nSentCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nSentCount[2]++;
		}
		virtual GAIA::GVOID OnRecved(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nSize)
		{
			if(bResult)
			{
				if(nSize > 0)
				{
					GAIA::SYNC::Autolock al(m_lrRecvBuf);
					m_recvbuf.write(pData, nSize);
				}
			}
			m_nRecvCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nRecvCount[2]++;
		}
		virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag)
		{
			m_nShutCount[bResult]++;
			if(m_bNoMoreCallBack)
				m_nShutCount[2]++;
		}

	public:
		AsyncCtx* m_ctx;
		GAIA::SYNC::Lock m_lrSendBuf;
		GAIA::SYNC::Lock m_lrRecvBuf;
		GAIA::CTN::BufferRW m_sendbuf;
		GAIA::CTN::BufferRW m_recvbuf;
		GAIA::SYNC::Atomic m_nCreatedCount[3];
		GAIA::SYNC::Atomic m_nClosedCount[3];
		GAIA::SYNC::Atomic m_nBoundCount[3];
		GAIA::SYNC::Atomic m_nConnectedCount[3];
		GAIA::SYNC::Atomic m_nDisconnectedCount[3];
		GAIA::SYNC::Atomic m_nListenCount[3];
		GAIA::SYNC::Atomic m_nAcceptedCount[3];
		GAIA::SYNC::Atomic m_nSentCount[3];
		GAIA::SYNC::Atomic m_nRecvCount[3];
		GAIA::SYNC::Atomic m_nShutCount[3];
		GAIA::BL m_bNoMoreCallBack;
	};

	class AsyncDispatcherEx : public GAIA::NETWORK::AsyncDispatcher
	{
	public:
		AsyncDispatcherEx(AsyncCtx& ctx){m_ctx = &ctx;}

	protected:
		virtual GAIA::NETWORK::AsyncSocket* OnCreateListenSocket(const GAIA::NETWORK::Addr& addrListen)
		{
			AsyncSocketEx* pListenSocket = gnew AsyncSocketEx(*m_ctx, *this, GAIA::NETWORK::ASYNC_SOCKET_TYPE_LISTEN);
			GAIA::SYNC::Autolock al(m_ctx->lrListenSocket);
			GAIA::CTN::Pair<GAIA::NETWORK::Addr, AsyncSocketEx*> pr;
			GAIA::NUM sIndex = addrListen.uPort - START_PORT;
			pr.front(addrListen);
			pr.back(pListenSocket);
			m_ctx->listListenSockets[sIndex] = pr;
			return pListenSocket;
		}
		virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptingSocket(const GAIA::NETWORK::Addr& addrListen)
		{
			AsyncSocketEx* pAcceptingSocket = gnew AsyncSocketEx(*m_ctx, *this, GAIA::NETWORK::ASYNC_SOCKET_TYPE_ACCEPTING);
			return pAcceptingSocket;
		}
		virtual GAIA::BL OnAcceptSocket(GAIA::NETWORK::AsyncSocket& sock, const GAIA::NETWORK::Addr& addrListen)
		{
			GAIA::SYNC::Autolock al(m_ctx->lrAcceptedSocket);
			m_ctx->listAcceptedSockets.push_back((AsyncSocketEx*)&sock);
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

					TAST(disp.GetAcceptedSocketCount() == 0);
					cb.listSocket.clear();
					TAST(disp.CollectAcceptedSocket(cb));
					TAST(cb.listSocket.size() == 0);

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
			AsyncCtx ctx;
			ctx.listListenSockets.reserve(LISTEN_SOCKET_COUNT);
			ctx.listConnectedSockets.reserve(DATA_SOCKET_COUNT);
			ctx.listAcceptedSockets.reserve(DATA_SOCKET_COUNT);

			AsyncDispatcherEx disp(ctx);
			ctx.pDispatcher = &disp;

			GAIA::NETWORK::AsyncDispatcherDesc descDisp;
			descDisp.reset();
			TAST(disp.Create(descDisp));
			{
				TAST(disp.Begin());
				{
					// Start listen.
					GAIA::NETWORK::Addr addrListen;
					addrListen.ip.fromstring("127.0.0.1");
					addrListen.uPort = START_PORT;
					for(GAIA::NUM x = 0; x < LISTEN_SOCKET_COUNT; ++x)
						ctx.listListenSockets.push_back(GAIA::CTN::Pair<GAIA::NETWORK::Addr, AsyncSocketEx*>(addrListen, GNIL));

					for(GAIA::NUM x = 0; x < LISTEN_SOCKET_COUNT; ++x)
					{
						disp.AddListenSocket(addrListen);
						addrListen.uPort++;
					}

					// Create connected socket.
					for(GAIA::NUM x = 0; x < DATA_SOCKET_COUNT; ++x)
					{
						AsyncSocketEx* pSocket = gnew AsyncSocketEx(ctx, disp);
						pSocket->Create();
						ctx.listConnectedSockets.push_back(pSocket);
					}

					// Bind.
					{

					}

					// Connect.
					for(GAIA::NUM x = 0; x < ctx.listConnectedSockets.size(); ++x)
					{
						AsyncSocketEx* pSocket = ctx.listConnectedSockets[x];
						GAIA::NUM sRandIndex = GAIA::MATH::xrandom() % ctx.listListenSockets.size();
						GAIA::NETWORK::Addr addrConnectTo = ctx.listListenSockets[sRandIndex].front();
						pSocket->Connect(addrConnectTo);
					}

					// Send.
					for(GAIA::NUM x = 0; x < ctx.listConnectedSockets.size(); ++x)
					{
						AsyncSocketEx* pSocket = ctx.listConnectedSockets[x];
						for(GAIA::NUM y = 0; y < DATA_TRANSFER_COUNT; ++y)
						{
							GAIA::NUM sSent = pSocket->Send(&y, sizeof(y));
							TAST(sSent == sizeof(y));
						}
					}

					// Wait complete.
					{
						GAIA::SYNC::gsleep(5000);
					}

					// Sign no more back.
					{
						// Listen.
						{
							GAIA::SYNC::Autolock al(ctx.lrListenSocket);
							for(GAIA::NUM x = 0; x < ctx.listListenSockets.size(); ++x)
								ctx.listListenSockets[x].back()->m_bNoMoreCallBack = GAIA::True;
						}

						// Connected.
						{
							GAIA::SYNC::Autolock al(ctx.lrConnectedSocket);
							for(GAIA::NUM x = 0; x < ctx.listConnectedSockets.size(); ++x)
								ctx.listConnectedSockets[x]->m_bNoMoreCallBack = GAIA::True;
						}

						// Accepted.
						{
							GAIA::SYNC::Autolock al(ctx.lrAcceptedSocket);
							for(GAIA::NUM x = 0; x < ctx.listAcceptedSockets.size(); ++x)
								ctx.listAcceptedSockets[x]->m_bNoMoreCallBack = GAIA::True;
						}
					}

					// Wait no more callback.
					{
						GAIA::SYNC::gsleep(1000);
					}

					// Check receive.
					for(GAIA::NUM x = 0; x < ctx.listAcceptedSockets.size(); ++x)
					{
					}

					// Check callback workflow result.
					{
						// Listen.
						{
							GAIA::SYNC::Autolock al(ctx.lrListenSocket);
							for(GAIA::NUM x = 0; x < ctx.listListenSockets.size(); ++x)
							{
								AsyncSocketEx* pSock = ctx.listListenSockets[x].back();
							}
						}

						// Connected.
						{
							GAIA::SYNC::Autolock al(ctx.lrConnectedSocket);
							for(GAIA::NUM x = 0; x < ctx.listConnectedSockets.size(); ++x)
							{
								AsyncSocketEx* pSock = ctx.listConnectedSockets[x];
							}
						}

						// Accepted.
						{
							GAIA::SYNC::Autolock al(ctx.lrAcceptedSocket);
							for(GAIA::NUM x = 0; x < ctx.listAcceptedSockets.size(); ++x)
							{
								AsyncSocketEx* pSock = ctx.listAcceptedSockets[x];
								TAST((GAIA::N64)pSock->m_nRecvCount[1] > 0);
							}
						}
					}

					// Shutdown.
					for(GAIA::NUM x = 0; x < DATA_SOCKET_COUNT; ++x)
					{
						AsyncSocketEx* pSocket = ctx.listConnectedSockets[x];
						pSocket->Shutdown();
					}

					// Close.
					for(GAIA::NUM x = 0; x < DATA_SOCKET_COUNT; ++x)
					{
						AsyncSocketEx* pSocket = ctx.listConnectedSockets[x];
						pSocket->Close();
					}

					// Release.
					for(GAIA::NUM x = 0; x < ctx.listConnectedSockets.size(); ++x)
						ctx.listConnectedSockets[x]->drop_ref();
					ctx.listConnectedSockets.clear();
					for(GAIA::NUM x = 0; x < ctx.listAcceptedSockets.size(); ++x)
						ctx.listAcceptedSockets[x]->drop_ref();
					ctx.listAcceptedSockets.clear();

					// End listen.
					addrListen.ip.fromstring("127.0.0.1");
					addrListen.uPort = START_PORT;
					for(GAIA::NUM x = 0; x < ctx.listListenSockets.size(); ++x)
					{
						disp.RemoveListenSocket(addrListen);
						addrListen.uPort++;
					}
					ctx.listListenSockets.clear();
				}
				TAST(disp.End());
			}
			TAST(disp.Destroy());
		}
	}
}
