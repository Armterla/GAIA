#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_sync_base.h>
#include <gaia_network_http.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		class HttpWorkThread : public GAIA::THREAD::Thread
		{
		public:
			HttpWorkThread(GAIA::NETWORK::Http& http)
			{
				this->init();
				m_pHttp = &http;
			}
			~HttpWorkThread(){}
			GAIA::GVOID SetStopCmd(GAIA::BL bStopCmd){m_bStopCmd = bStopCmd;}
			virtual GAIA::GVOID Run()
			{
				for(;;)
				{
					if(m_bStopCmd)
						break;
					GAIA::BL bExistExecutedTask = m_pHttp->Execute();
					if(!bExistExecutedTask)
						GAIA::SYNC::gsleep(1);
				}
			}

		private:
			GINL GAIA::GVOID init()
			{
				m_pHttp = GNIL;
				m_bStopCmd = GAIA::False;
			}

		private:
			GAIA::NETWORK::Http* m_pHttp;
			GAIA::BL m_bStopCmd;
		};

		class HttpAsyncSocket : public GAIA::NETWORK::AsyncSocket
		{
		public:
			HttpAsyncSocket(GAIA::NETWORK::Http& http, GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::ASYNC_SOCKET_TYPE socktype = GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED)
				: GAIA::NETWORK::AsyncSocket(disp, socktype)
			{
				this->init();
				m_pHttp = &http;
			}

			~HttpAsyncSocket(){}

			virtual GAIA::GVOID Create()
			{
				GAIA::NETWORK::AsyncSocket::Create();

				const GAIA::NETWORK::HttpDesc& descHttp = m_pHttp->GetDesc();
				if(descHttp.bEnableSocketTCPNoDelay)
					this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_TCPNODELAY, GAIA::True);
				if(descHttp.bEnableSocketNoBlock)
					this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_NOBLOCK, GAIA::True);
				if(descHttp.bEnableSocketReuseAddr)
					this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_REUSEADDR, GAIA::True);

				if(descHttp.nSocketSendBufferSize != GINVALID)
					this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_SENDBUFSIZE, descHttp.nSocketSendBufferSize);
				if(descHttp.nSocketRecvBufferSize != GINVALID)
					this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_RECVBUFSIZE, descHttp.nSocketRecvBufferSize);
			}

		protected:
			virtual GAIA::GVOID OnCreated(GAIA::BL bResult){}
			virtual GAIA::GVOID OnClosed(GAIA::BL bResult){}
			virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}
			virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr)
			{

			}
			virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult)
			{

			}
			virtual GAIA::GVOID OnListened(GAIA::BL bResult){}
			virtual GAIA::GVOID OnAccepted(GAIA::BL bResult, const GAIA::NETWORK::Addr& addrListen){}
			virtual GAIA::GVOID OnSent(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nPracticeSize, GAIA::N32 nSize)
			{

			}
			virtual GAIA::GVOID OnRecved(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nSize)
			{

			}
			virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag){}

		private:
			GINL GAIA::GVOID init()
			{
				m_pHttp = GNIL;
				m_pRequest = GNIL;
				m_sIndex = GINVALID;
			}

		private:
			GAIA::NETWORK::Http* m_pHttp;
			GAIA::NETWORK::HttpRequest* m_pRequest;
			GAIA::NUM m_sIndex; // Is the index in GAIA::NETWORK::Http's variable member m_socks.
		};

		class HttpAsyncDispatcher : public GAIA::NETWORK::AsyncDispatcher
		{
		public:
			HttpAsyncDispatcher(GAIA::NETWORK::Http& http)
			{
				m_pHttp = &http;
			}

			~HttpAsyncDispatcher(){}

		protected:
			virtual GAIA::NETWORK::AsyncSocket* OnCreateListenSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				HttpAsyncSocket* pListenSocket = gnew HttpAsyncSocket(*m_pHttp, *this, ASYNC_SOCKET_TYPE_LISTEN);
				return pListenSocket;
			}

			virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptingSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				HttpAsyncSocket* pAcceptingSocket = gnew HttpAsyncSocket(*m_pHttp, *this, ASYNC_SOCKET_TYPE_ACCEPTING);
				return pAcceptingSocket;
			}

		private:
			GINL GAIA::GVOID init()
			{
				m_pHttp = GNIL;
			}

		private:
			GAIA::NETWORK::Http* m_pHttp;
		};

		HttpRequest::HttpRequest(GAIA::NETWORK::Http& http)
		{
			m_pHttp = &http;
			this->init();
		}

		HttpRequest::~HttpRequest()
		{
			if(m_bBufOwner)
				m_buf.destroy();
			else
				m_buf.proxy(GNIL, GNIL);
		}

		Http& HttpRequest::GetHttp()
		{
			return *m_pHttp;
		}

		GAIA::GVOID HttpRequest::Reset()
		{
			if(m_bBufOwner)
				m_buf.destroy();
			else
				m_buf.proxy(GNIL, GNIL);
			this->init();
		}

		GAIA::BL HttpRequest::BindBuffer(const GAIA::GVOID* p, GAIA::NUM sSize)
		{
			if(m_bRequesting)
				return GAIA::False;

			if(p == GNIL)
			{
				if(sSize > 0)
				{
					m_bBufOwner = GAIA::True;
					m_buf.resize(sSize);
				}
				else if(sSize == 0)
				{
					if(m_bBufOwner)
						m_buf.destroy();
					else
						m_buf.proxy(GNIL, GNIL);
				}
				else
				{
					GASTFALSE;
					return GAIA::False;
				}
			}
			else
			{
				if(sSize > 0)
				{
					if(m_bBufOwner)
						m_buf.destroy();
					else
						m_buf.proxy(GNIL, GNIL);
					m_bBufOwner = GAIA::False;
					m_buf.proxy((GAIA::U8*)p, (GAIA::U8*)p + sSize - 1);
				}
				else
				{
					GASTFALSE;
					return GAIA::False;
				}
			}
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Request()
		{
			if(m_method == GAIA::NETWORK::HTTP_METHOD_INVALID)
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Cancel()
		{
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Pause()
		{
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Resume()
		{
			return GAIA::True;
		}

		GAIA::BL HttpRequest::Wait(GAIA::U32 uMilliSeconds)
		{
			return GAIA::True;
		}

		Http::Http()
		{
			GAST(GAIA::NETWORK::HTTP_METHOD_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_METHOD_STRING));
			GAST(GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_STRING));
			GAST(GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_LIST));
			GAST(GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_VALUE));
			GAST(GAIA::NETWORK::HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_DESCRIPTION));
			this->init();
		}

		Http::~Http()
		{
			if(this->IsCreated())
				this->Destroy();
		}

		GAIA::BL Http::Create(const GAIA::NETWORK::HttpDesc& desc)
		{
			if(!desc.check())
				return GAIA::False;

			if(this->IsCreated())
				return GAIA::False;

			// Create async network.
			m_disp = gnew GAIA::NETWORK::HttpAsyncDispatcher(*this);
			GAIA::NETWORK::AsyncDispatcherDesc descDisp;
			descDisp.reset();
			descDisp.sThreadCount = desc.sNetworkThreadCount;
			descDisp.sMaxConnectionCount = desc.sMaxConnCount;
			if(!m_disp->Create(descDisp))
				return GAIA::False;

			// Create work thread.
			for(GAIA::NUM x = 0; x < m_desc.sWorkThreadCount; ++x)
			{
				HttpWorkThread* pThread = gnew HttpWorkThread(*this);
				m_listWorkThreads.push_back(pThread);
			}

			m_desc = desc;
			m_bCreated = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL Http::Destroy()
		{
			if(!this->IsCreated())
				return GAIA::False;

			if(this->IsBegin())
				this->End();

			// Destroy async network.
			m_disp->Destroy();
			gdel m_disp;
			m_disp = GNIL;

			// Destroy work thread.
			for(GAIA::NUM x = 0; x < m_desc.sWorkThreadCount; ++x)
			{
				HttpWorkThread* pThread = m_listWorkThreads[x];
				GAST(pThread != GNIL);
				gdel pThread;
			}
			m_listWorkThreads.clear();

			// Cleanup cookic in RAM.

			m_bCreated = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL Http::Begin()
		{
			if(!this->IsCreated())
				return GAIA::False;

			if(this->IsBegin())
				return GAIA::False;

			// Begin async network.
			if(!m_disp->Begin())
			{
				for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
				{
					HttpWorkThread* pThread = m_listWorkThreads[x];
					pThread->SetStopCmd(GAIA::True);
				}
				for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
				{
					HttpWorkThread* pThread = m_listWorkThreads[x];
					pThread->Wait();
				}
				return GAIA::False;
			}

			// Start work thread.
			for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
			{
				HttpWorkThread* pThread = m_listWorkThreads[x];
				pThread->Start();
			}

			m_bBegin = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL Http::End()
		{
			if(!this->IsBegin())
				return GAIA::False;

			// Close work thread.
			for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
			{
				HttpWorkThread* pThread = m_listWorkThreads[x];
				GAST(pThread != GNIL);
				pThread->SetStopCmd(GAIA::True);
			}
			for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
			{
				HttpWorkThread* pThread = m_listWorkThreads[x];
				GAST(pThread != GNIL);
				pThread->Wait();
			}

			// Release all requests.
			{
				GAIA::SYNC::Autolock al(m_lrRequests);
				for(__RequestVectorType::it it = m_requests.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpRequest* pRequest = *it;
					GAST(pRequest != GNIL);
					pRequest->drop_ref();
				}
				m_requests.clear();
			}

			// Release all sockets.
			{
				GAIA::SYNC::Autolock al(m_lrSocks);
				for(__SockVectorType::it it = m_socks.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpAsyncSocket* pSocket = *it;
					GAST(pSocket != GNIL);
					pSocket->drop_ref();
				}
				m_socks.clear();
			}

			// End async network.
			m_disp->End();

			m_bBegin = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL Http::Execute()
		{
			GAIA::BL bRet = GAIA::False;

			// Request.
			{
				GAIA::BL bConnectionIsFull = GAIA::False;
				{
					GAIA::SYNC::Autolock al(m_lrSocks);
					if(m_socks.size() >= m_desc.sMaxConnCount)
						bConnectionIsFull = GAIA::False;
				}

				if(!bConnectionIsFull)
				{
					GAIA::NETWORK::HttpRequest* pRequest = GNIL;
					{
						GAIA::SYNC::Autolock al(m_lrRequests);
						if(!m_requests.empty())
						{
							pRequest = m_requests.back();
							m_requests.pop_back();
						}
					}

					if(pRequest != GNIL)
					{
						pRequest->OnBegin();

						const GAIA::NETWORK::HttpURL& url = pRequest->GetURL();
						GAIA::CH szHostName[64];
						GAIA::NUM sHostNameLen;
						const GAIA::CH* pszHostName = url.GetHostName(szHostName, sizeof(szHostName), &sHostNameLen);
						if(GAIA::ALGO::gstremp(pszHostName))
							pszHostName = GAIA::NETWORK::GAIA_LOCAL_HOST_IP;

						GAIA::BL bHostAddressIsValid = GAIA::True;
						GAIA::NETWORK::Addr addrHost;
						if(!addrHost.fromstring(pszHostName))
						{
							if(!GAIA::NETWORK::GetHostIP(pszHostName, addrHost.ip))
								bHostAddressIsValid = GAIA::False;
							else
							{
								if(!addrHost.fromstring(pszHostName))
									bHostAddressIsValid = GAIA::False;
							}
						}

						addrHost.uPort = url.GetPort();
						if(addrHost.uPort == 0)
							addrHost.uPort = GAIA::NETWORK::HTTP_DEFAULT_PORT;

						if(bHostAddressIsValid)
						{
							HttpAsyncSocket* pSocket = gnew HttpAsyncSocket(*this, *m_disp);
							pSocket->Create();
							pSocket->Connect(addrHost);
							bRet = GAIA::True;
						}
						else
						{
							pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_DOMAIN_ANALYSIS_FAILED;
							pRequest->OnEnd(GAIA::False);
						}
					}
				}
			}

			// Response.
			{

			}

			// Request timeout dispatch.
			{

			}

			// Recycle cache.
			{

			}

			return GAIA::True;
		}

		GAIA::BL Http::CleanupCookic(GAIA::BL bRAM, GAIA::BL bFile, const GAIA::U64& uBeyondTime)
		{
			return GAIA::True;
		}
	}
}