#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	static const GAIA::U8 SUDPSOCKET_UNSERIAL_MSGID = 1;
	static const GAIA::U8 SUDPSOCKET_SERIAL_MSGID = 2;
	static const GAIA::U8 SUDPSOCKET_BACK_MSGID = 3;

	class SUDPSocketUnserialMsg : public GAIA::Base
	{
	public:
		SUDPSocketUnserialMsg()
		{
			uMsgID = SUDPSOCKET_UNSERIAL_MSGID;
		}
	public:
		GAIA::U8 uMsgID;
		GAIA::U32 uData;
	};

	class SUDPSocketSerialMsg : public GAIA::Base
	{
	public:
		SUDPSocketSerialMsg()
		{
			uMsgID = SUDPSOCKET_SERIAL_MSGID;
		}
	public:
		GAIA::U8 uMsgID;
		GAIA::U16 uSerial;
		GAIA::U32 uData;
	};

	class SUDPSocketBackMsg : public GAIA::Base
	{
	public:
		SUDPSocketBackMsg()
		{
			uMsgID = SUDPSOCKET_BACK_MSGID;
		}
		GAIA::U8 uMsgID;
		GAIA::U32 uBeginSerial;
		GAIA::U32 uEndSerial;
	};

	class SUDPSocketImpl : public GAIA::NETWORK::SUDPSocket
	{
	public:
		GINL SUDPSocketImpl()
		{

		}
	public:
		virtual GAIA::BL OnSend(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N64& lSerial)
		{
			GAST(addr.check());
			GAST(p != GNIL);
			GAST(nSize > 0);
			GAIA::U8 uMsgID = *(GAIA::U8*)p;
			if(uMsgID != SUDPSOCKET_SERIAL_MSGID)
				return GAIA::False;
			SUDPSocketSerialMsg& msg = *(SUDPSocketSerialMsg*)p;
			lSerial = msg.uSerial;
			return GAIA::True;
		}
		virtual GAIA::BL OnRecv(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N64& lSerial)
		{
			GAST(addr.check());
			GAST(p != GNIL);
			GAST(nSize > 0);
			GAIA::U8 uMsgID = *(GAIA::U8*)p;
			if(uMsgID != SUDPSOCKET_SERIAL_MSGID)
				return GAIA::False;
			SUDPSocketSerialMsg& msg = *(SUDPSocketSerialMsg*)p;
			lSerial = msg.uSerial;
			return GAIA::True;
		}
		virtual GAIA::BL OnBack(const GAIA::NETWORK::Addr& addr, const GAIA::N64& lBeginSerial, const GAIA::N64& lEndSerial, GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32& nResultSize)
		{
			GAST(addr.check());
			GAST(p != GNIL);
			GAST(nSize > 0);
			GAST(lBeginSerial <= GAIA::U32MAX);
			GAST(lEndSerial <= GAIA::U32MAX);
			if(nSize < sizeof(SUDPSocketBackMsg))
				return GAIA::False;
			SUDPSocketBackMsg& msg = *(SUDPSocketBackMsg*)p;
			msg.uMsgID = SUDPSOCKET_BACK_MSGID;
			msg.uBeginSerial = (GAIA::U32)lBeginSerial;
			msg.uEndSerial = (GAIA::U32)lEndSerial;
			nResultSize = sizeof(SUDPSocketBackMsg);
			return GAIA::True;
		}
		virtual GAIA::BL OnRecvBack(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N64& lBeginSerial, GAIA::N64& lEndSerial)
		{
			GAST(addr.check());
			GAST(p != GNIL);
			GAST(nSize > 0);
			GAIA::U8 uMsgID = *(GAIA::U8*)p;
			if(uMsgID != SUDPSOCKET_BACK_MSGID)
				return GAIA::False;
			SUDPSocketBackMsg& msg = *(SUDPSocketBackMsg*)p;
			lBeginSerial = msg.uBeginSerial;
			lEndSerial = msg.uEndSerial;
			return GAIA::True;
		}
		virtual GAIA::GVOID OnTimeout(const GAIA::NETWORK::Addr& addr, GAIA::BL bLostConnection)
		{
			GAST(addr.check());
			if(bLostConnection)
			{

			}
			else
			{

			}
		}
	};

	class SUDPSocketThread : public GAIA::THREAD::Thread
	{
	public:
		SUDPSocketThread(const GAIA::CTN::Vector<SUDPSocketImpl*>& listSocks, GAIA::BL bSend, GAIA::BL bRecv, GAIA::BL bResend, GAIA::BL bRerecv, GAIA::BL bTimeout)
		{
			m_bStopCmd = GAIA::False;
			m_listSocks = listSocks;
			m_bSend = bSend;
			m_bRecv = bRecv;
			m_bResend = bResend;
			m_bRerecv = bRerecv;
			m_bTimeout = bTimeout;
		}

		GAIA::GVOID Run()
		{
			GAIA::U64 uLastTime = GAIA::TIME::tick_time();
			for(;;)
			{
				if(m_bStopCmd)
					break;

				GAIA::U64 uCurrentTime = GAIA::TIME::tick_time();
				GAIA::U64 uDeltaTime = uCurrentTime - uLastTime;
				uLastTime = uCurrentTime;
				if(!m_bResend)
					uDeltaTime = 0;
				for(GAIA::NUM x = 0; x < m_listSocks.size(); ++x)
				{
					SUDPSocketImpl* pSock = m_listSocks[x];
					GAST(pSock != GNIL);
					pSock->Execute(uDeltaTime, m_bSend, m_bRecv, m_bResend, m_bRerecv, m_bTimeout);
				}

				if(m_bSend)
					GAIA::SYNC::gsleep(10);
				else if(m_bRecv)
					GAIA::SYNC::gsleep(10);
				else if(m_bResend)
					GAIA::SYNC::gsleep(10);
				else if(m_bRerecv)
					GAIA::SYNC::gsleep(10);
				else if(m_bTimeout)
					GAIA::SYNC::gsleep(1000);
			}
		}

		GAIA::GVOID SetStopCmd(){m_bStopCmd = GAIA::True;}

	private:
		GAIA::BL m_bStopCmd;
		GAIA::CTN::Vector<SUDPSocketImpl*> m_listSocks;
		GAIA::BL m_bSend;
		GAIA::BL m_bRecv;
		GAIA::BL m_bResend;
		GAIA::BL m_bRerecv;
		GAIA::BL m_bTimeout;
	};

	class SUDPSocketRecv : public GAIA::Base
	{
	public:
		SUDPSocketRecv()
		{
			addr.reset();
			sUnserialMsgCount = 0;
			sSerialMsgCount = 0;
		}
	public:
		GAIA::NETWORK::Addr addr;
		GAIA::NUM sUnserialMsgCount;
		GAIA::NUM sSerialMsgCount;
		GAIA::CTN::Vector<GAIA::CTN::Pair<GAIA::NETWORK::Addr, GAIA::U16> > listLastSerial;
	};

	extern GAIA::GVOID t_network_sudpsocket(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM SUDPSOCKET_COUNT = 3;
		static const GAIA::NUM SUDPSOCKET_MSGCOUNT = 1000;

		GTRY
		{
			// Create socket.
			GAIA::CTN::Vector<GAIA::NETWORK::Addr> listAddrs;
			GAIA::CTN::Vector<SUDPSocketImpl*> listSocks;
			GAIA::CTN::Vector<SUDPSocketRecv> listRecv;
			for(GAIA::NUM x = 0; x < SUDPSOCKET_COUNT; ++x)
			{
				GAIA::NETWORK::Addr addr;
				addr.ip.fromstring("127.0.0.1");
				addr.uPort = 9010 + x;
				listAddrs.push_back(addr);
				SUDPSocketImpl* pSock = gnew SUDPSocketImpl;
				pSock->EnableLog(TMODULE_LOG_ENABLED);
				pSock->Create();
				pSock->Bind(addr);
				pSock->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_NOBLOCK, GAIA::True);
				pSock->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_SENDBUFSIZE, 1000 * 1024);
				pSock->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_RECVBUFSIZE, 1000 * 1024);
				listSocks.push_back(pSock);
				SUDPSocketRecv sockrecv;
				sockrecv.addr = addr;
				if(x == 0)
				{
					for(GAIA::NUM y = 0; y < SUDPSOCKET_COUNT; ++y)
					{
						GAIA::NETWORK::Addr addrRemote;
						addrRemote.ip.fromstring("127.0.0.1");
						addrRemote.uPort = 9010 + y;
						sockrecv.listLastSerial.push_back(GAIA::CTN::Pair<GAIA::NETWORK::Addr, GAIA::U16>(addrRemote, 0));
					}
				}
				else
					sockrecv.listLastSerial.push_back(GAIA::CTN::Pair<GAIA::NETWORK::Addr, GAIA::U16>(listAddrs[0], 0));
				listRecv.push_back(sockrecv);
			}

			GAIA::NETWORK::Addr addrServer = listAddrs[0];
			SUDPSocketImpl* pServerSocket = listSocks[0];
			GAIA::BL bExistError = GAIA::False;

			// New thread.
			GAIA::CTN::Vector<SUDPSocketThread*> listThreads;
			listThreads.push_back(gnew SUDPSocketThread(listSocks, GAIA::True, GAIA::False, GAIA::False, GAIA::False, GAIA::False));
			listThreads.push_back(gnew SUDPSocketThread(listSocks, GAIA::False, GAIA::True, GAIA::False, GAIA::False, GAIA::False));
			listThreads.push_back(gnew SUDPSocketThread(listSocks, GAIA::False, GAIA::False, GAIA::True, GAIA::False, GAIA::False));
			listThreads.push_back(gnew SUDPSocketThread(listSocks, GAIA::False, GAIA::False, GAIA::False, GAIA::True, GAIA::False));
			listThreads.push_back(gnew SUDPSocketThread(listSocks, GAIA::False, GAIA::False, GAIA::False, GAIA::False, GAIA::True));

			// Start thread.
			for(GAIA::NUM x = 0; x < listThreads.size(); ++x)
			{
				SUDPSocketThread* pThread = listThreads[x];
				pThread->Start();
			}

			// Unserial datagram test.
			{
				for(GAIA::NUM x = 0; x < SUDPSOCKET_MSGCOUNT; ++x)
				{
					SUDPSocketUnserialMsg msg;
					msg.uData = x;

					GAIA::BL bExistFailed = GAIA::False;
					for(GAIA::NUM y = 1; y < listSocks.size(); ++y)
					{
						SUDPSocketImpl* pSock = listSocks[y];
						GAST(pSock != GNIL);
						if(pSock->SendTo(addrServer, &msg, sizeof(msg)) != sizeof(msg))
						{
							TERROR;
							bExistFailed = GAIA::True;
							break;
						}

						const GAIA::NETWORK::Addr& addrClient = listAddrs[y];
						if(pServerSocket->SendTo(addrClient, &msg, sizeof(msg)) != sizeof(msg))
						{
							TERROR;
							bExistFailed = GAIA::True;
							break;
						}
					}
					if(bExistFailed)
						break;
				}
			}

			// Serial datagram test.
			{
				for(GAIA::NUM x = 0; x < SUDPSOCKET_MSGCOUNT; ++x)
				{
					SUDPSocketSerialMsg msg;
					msg.uSerial = x;
					msg.uData = x;

					GAIA::BL bExistFailed = GAIA::False;
					for(GAIA::NUM y = 1; y < listSocks.size(); ++y)
					{
						SUDPSocketImpl* pSock = listSocks[y];
						GAST(pSock != GNIL);
						if(pSock->SendTo(addrServer, &msg, sizeof(msg)) != sizeof(msg))
						{
							TERROR;
							bExistFailed = GAIA::True;
							break;
						}

						const GAIA::NETWORK::Addr& addrClient = listAddrs[y];
						if(pServerSocket->SendTo(addrClient, &msg, sizeof(msg)) != sizeof(msg))
						{
							TERROR;
							bExistFailed = GAIA::True;
							break;
						}
					}
					if(bExistFailed)
						break;
				}
			}

			// Recv test.
			{
				// Receive.
				GAIA::NUM sRecvCount = 0;
				GAIA::U64 uStartTime = GAIA::TIME::tick_time();
				for(;;)
				{
					GAIA::BL bRecved = GAIA::False;
					for(GAIA::NUM x = 0; x < listSocks.size(); ++x)
					{
						SUDPSocketImpl* pSock = listSocks[x];

						GAIA::NETWORK::Addr addrRecvFrom;
						addrRecvFrom.reset();
						GAIA::U8 msgbuf[GAIA::NETWORK::GAIA_NETWORK_MTU];
						GAIA::N32 nRecv = pSock->RecvFrom(addrRecvFrom, msgbuf, sizeof(msgbuf));
						if(nRecv <= 0)
							continue;

						if(sRecvCount % 10 == 0)
							logobj << "\tRecvMsg Count = " << sRecvCount << logobj.End();
						sRecvCount++;

						SUDPSocketRecv& sockrecv = listRecv[x];

						bRecved = GAIA::True;
						GAIA::U8 uMsgID = msgbuf[0];
						if(uMsgID == SUDPSOCKET_UNSERIAL_MSGID)
						{
							if(nRecv != sizeof(SUDPSocketUnserialMsg))
							{
								TERROR;
								bExistError = GAIA::True;
								break;
							}
							SUDPSocketUnserialMsg& msg = *(SUDPSocketUnserialMsg*)msgbuf;
							sockrecv.sUnserialMsgCount++;
						}
						else if(uMsgID == SUDPSOCKET_SERIAL_MSGID)
						{
							if(nRecv != sizeof(SUDPSocketSerialMsg))
							{
								TERROR;
								bExistError = GAIA::True;
								break;
							}
							SUDPSocketSerialMsg& msg = *(SUDPSocketSerialMsg*)msgbuf;
							sockrecv.sSerialMsgCount++;
							for(GAIA::NUM y = 0; y < sockrecv.listLastSerial.size(); ++y)
							{
								GAIA::CTN::Pair<GAIA::NETWORK::Addr, GAIA::U16>& pr = sockrecv.listLastSerial[y];
								if(pr.front() == addrRecvFrom)
								{
									if(pr.back() != msg.uSerial)
									{
										TERROR;
										bExistError = GAIA::True;
										break;
									}
									++pr.back();
									break;
								}
							}
						}
						else
						{
							TERROR;
							bExistError = GAIA::True;
							break;
						}
					}
					if(bExistError)
						break;
					if(!bRecved)
						GAIA::SYNC::gsleep(1);
					GAIA::U64 uCurrentTime = GAIA::TIME::tick_time();
					if(sRecvCount == (SUDPSOCKET_COUNT - 1) * 2 * 2 * SUDPSOCKET_MSGCOUNT)
					{
						logobj << "\tSUDPSocket timecost = " << (GAIA::F64)(uCurrentTime - uStartTime) / 1000 << "(ms)" << logobj.End();
						break;
					}
					if(uCurrentTime - uStartTime > (SUDPSOCKET_MSGCOUNT / 10000 + 1) * 1000 * 1000)
					{
						TERROR;
						break;
					}
				}

				// Check received content.
				if(!bExistError)
				{
					// Check server.
					{
						SUDPSocketRecv& sockrecv = listRecv[0];
						if(sockrecv.sUnserialMsgCount != SUDPSOCKET_MSGCOUNT * (listSocks.size() - 1))
						{
							bExistError = GAIA::True;
							TERROR;
						}
						if(sockrecv.sSerialMsgCount != SUDPSOCKET_MSGCOUNT * (listSocks.size() - 1))
						{
							bExistError = GAIA::True;
							TERROR;
						}
					}

					// Check client.
					for(GAIA::NUM x = 1; x < listRecv.size(); ++x)
					{
						SUDPSocketRecv& sockrecv = listRecv[x];
						if(sockrecv.sUnserialMsgCount != SUDPSOCKET_MSGCOUNT)
						{
							bExistError = GAIA::True;
							TERROR;
						}
						if(sockrecv.sSerialMsgCount != SUDPSOCKET_MSGCOUNT)
						{
							bExistError = GAIA::True;
							TERROR;
						}
					}
				}
			}

			// Timeout test.
			{

			}

			// Reset connection test.
			{
				for(GAIA::NUM x = 0; x < listSocks.size(); ++x)
				{
					SUDPSocketImpl* pSock = listSocks[x];
					if(x % 2 == 0)
						pSock->ResetConnection();
					else
						pSock->ResetConnection(&addrServer);
				}
			}

			// Stop thread.
			for(GAIA::NUM x = 0; x < listThreads.size(); ++x)
			{
				SUDPSocketThread* pThread = listThreads[x];
				pThread->SetStopCmd();
			}

			// Wait and delete thread.
			for(GAIA::NUM x = 0; x < listThreads.size(); ++x)
			{
				SUDPSocketThread* pThread = listThreads[x];
				pThread->Wait();
				gdel pThread;
			}
			listThreads.clear();

			if(bExistError)
			{
				for(GAIA::NUM x = 0; x < listSocks.size(); ++x)
				{
					SUDPSocketImpl* pSocket = listSocks[x];
					const SUDPSocketImpl::State& state = pSocket->GetState();
					logobj << "\t" << "SUDPSocket " << x << logobj.End();
					logobj << "\t" << "\t" << "SendUnserialMsgCount = " << state.uSendUnserialMsgCount << logobj.End();
					logobj << "\t" << "\t" << "SendSerialMsgCount = " << state.uSendSerialMsgCount << logobj.End();
					logobj << "\t" << "\t" << "SendUnserialMsgSize = " << state.uSendUnserialMsgSize << logobj.End();
					logobj << "\t" << "\t" << "SendSerialMsgSize = " << state.uSendSerialMsgSize << logobj.End();
					logobj << "\t" << "\t" << "RecvUnserialMsgCount = " << state.uRecvUnserialMsgCount << logobj.End();
					logobj << "\t" << "\t" << "RecvSerialMsgCount = " << state.uRecvSerialMsgCount << logobj.End();
					logobj << "\t" << "\t" << "RecvUnserialMsgSize = " << state.uRecvUnserialMsgSize << logobj.End();
					logobj << "\t" << "\t" << "RecvSerialMsgSize = " << state.uRecvSerialMsgSize << logobj.End();
					logobj << "\t" << "\t" << "ResendSerialMsgCount = " << state.uResendSerialMsgCount << logobj.End();
					logobj << "\t" << "\t" << "ResendSerialMsgSize = " << state.uResendSerialMsgSize << logobj.End();
					logobj << "\t" << "\t" << "SendBackMsgCount = " << state.uSendBackMsgCount << logobj.End();
					logobj << "\t" << "\t" << "SendBackMsgSize = " << state.uSendBackMsgSize << logobj.End();
					logobj << "\t" << "\t" << "RecvBackMsgCount = " << state.uRecvBackMsgCount << logobj.End();
					logobj << "\t" << "\t" << "RecvBackMsgSize = " << state.uRecvBackMsgSize << logobj.End();
				}
			}

			// Close and delete socket.
			for(GAIA::NUM x = 0; x < listSocks.size(); ++x)
			{
				SUDPSocketImpl* pSocket = listSocks[x];
				pSocket->Close();
				gdel pSocket;
			}
			listSocks.clear();
			listAddrs.clear();
		}
		GCATCH(Network)
		{
			GSTM << e.GetErrorText() << " Error=" << e.GetError() << " OSError=" << e.GetOSError() << " Exception!" << "\n";
			TERROR;
		}
		GCATCHBASE
		{
			e.SetDispatched(GAIA::True);
			TERROR;
		}
	}
}
