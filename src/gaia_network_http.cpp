#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_sync_base.h>
#include <gaia_time.h>
#include <gaia_network_http.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		class HttpWorkThread : public GAIA::THREAD::Thread
		{
			friend class HttpRequest;
			friend class Http;

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
					GAIA::BL bExistExecutedTask = m_pHttp->Execute(this);
					if(!bExistExecutedTask)
						GAIA::SYNC::gsleep(1);
				}
			}

		private:
			GINL GAIA::GVOID init()
			{
				m_pHttp = GNIL;
				m_bStopCmd = GAIA::False;
				uThreadIndex = GINVALID;
			}

		private:
			GAIA::NETWORK::Http* m_pHttp;
			GAIA::BL m_bStopCmd;

		private:
			GAIA::U32 uThreadIndex;
			GAIA::NETWORK::Http::__RequestVectorType listTempRequestListForRequest;
			GAIA::NETWORK::Http::__RequestVectorType listTempRequestListForClose;
			GAIA::CTN::Buffer reqbuf;
		};

		class HttpAsyncSocket : public GAIA::NETWORK::AsyncSocket
		{
			friend class HttpRequest;
			friend class Http;

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
			virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){GASTFALSE;}
			virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr)
			{
				// Check state and parameter.
				if(!bResult)
				{
					if(m_pRequest->m_NetworkError == GAIA::NETWORK::NETWORK_ERROR_INVALID)
						m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_CONNECT_FAILED;
					return;
				}
				GAST(m_pRequest != GNIL);

				// Change state.
				{
					// Remove from connecting list.
					{
						GAIA::SYNC::Autolock al(m_pHttp->m_lrConnectingList);
						GAIA::BL bEraseResult = m_pHttp->m_connectinglist.erase(m_pRequest);
						GAST(bEraseResult);
					}

					// Insert to requesting list.
					{
						GAST(m_pRequest->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING);
						m_pRequest->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING);

						GAIA::SYNC::Autolock al(m_pHttp->m_lrRequestingList);
						m_pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING;
						GAIA::BL bInsertResult = m_pHttp->m_requestinglist.insert(m_pRequest);
						GAST(bInsertResult);
					}
				}
			}
			virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult)
			{
				// Check state and parameter.
				if(!bResult)
				{
					if(m_pRequest->m_NetworkError == GAIA::NETWORK::NETWORK_ERROR_INVALID)
						m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_LOSTCONNECTION;
					return;
				}
				GAST(m_pRequest != GNIL);

				//
				m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_OK;
			}
			virtual GAIA::GVOID OnListened(GAIA::BL bResult){GASTFALSE;}
			virtual GAIA::GVOID OnAccepted(GAIA::BL bResult, const GAIA::NETWORK::Addr& addrListen){GASTFALSE;}
			virtual GAIA::GVOID OnSent(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nPracticeSize, GAIA::N32 nSize)
			{
				// Check state and parameter.
				if(!bResult)
				{
					if(m_pRequest->m_NetworkError == GAIA::NETWORK::NETWORK_ERROR_INVALID)
						m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_LOSTCONNECTION;
					return;
				}
				if(nPracticeSize == 0)
					return;
				GAST(m_pRequest != GNIL);

				// Callback.
				m_pRequest->OnSent(m_pRequest->m_lSentSize, pData, nPracticeSize);
				m_pRequest->m_lSentSize += nPracticeSize;
				m_pRequest->m_uLastSentTime = GAIA::TIME::tick_time();

				// Change state.
				GAST(m_pRequest->m_lSentSize <= m_pRequest->m_lSendingSize);
				if(m_pRequest->m_bHeadSendingComplete && m_pRequest->m_bBodySendingComplete &&
				   m_pRequest->m_lSentSize == m_pRequest->m_lSendingSize)
				{
					if(m_pRequest->GetState() == GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING)
					{
						// Remove from requesting list.
						{
							GAIA::SYNC::Autolock al(m_pHttp->m_lrRequestingList);
							GAIA::BL bEraseResult = m_pHttp->m_requestinglist.erase(m_pRequest);
							GAST(bEraseResult);
						}

						// Insert to waiting list.
						{
							GAST(m_pRequest->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING);
							m_pRequest->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING);

							GAIA::SYNC::Autolock al(m_pHttp->m_lrWaitingList);
							m_pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING;
							GAIA::BL bInsertResult = m_pHttp->m_waitinglist.insert(m_pRequest);
							GAST(bInsertResult);
						}
					}
					else
						GASTFALSE;
				}
			}
			virtual GAIA::GVOID OnRecved(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nSize)
			{
				// Check state and parameter.
				if(!bResult)
				{
					if(m_pRequest->m_NetworkError == GAIA::NETWORK::NETWORK_ERROR_INVALID)
						m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_OK;
					return;
				}
				GAST(m_pRequest != GNIL);

				// Change state.
				if(m_pRequest->m_lRecvedSize == 0)
				{
					if(m_pRequest->GetState() == GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING)
					{
						// Remove from requesting list.
						GAIA::SYNC::Autolock al(m_pHttp->m_lrRequestingList);
						GAIA::BL bEraseResult = m_pHttp->m_requestinglist.erase(m_pRequest);
						GAST(bEraseResult);
					}
					else if(m_pRequest->GetState() == GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING)
					{
						// Remove from waiting list.
						GAIA::SYNC::Autolock al(m_pHttp->m_lrWaitingList);
						GAIA::BL bEraseResult = m_pHttp->m_waitinglist.erase(m_pRequest);
						GAST(bEraseResult);
					}
					else
						GASTFALSE;

					// Insert to responsing list.
					{
						GAST(m_pRequest->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_RESPONSING);
						m_pRequest->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_RESPONSING);

						GAIA::SYNC::Autolock al(m_pHttp->m_lrResponsingList);
						m_pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_RESPONSING;
						GAIA::BL bInsertResult = m_pHttp->m_responsinglist.insert(m_pRequest);
						GAST(bInsertResult);
					}
				}

				// Callback.
				m_pRequest->OnRecved(m_pRequest->m_lRecvedSize, pData, nSize);
				m_pRequest->m_lRecvedSize += nSize;
				m_pRequest->m_uLastRecvedTime = GAIA::TIME::tick_time();

				// Check complete, if complete, change state.
				{

				}
			}
			virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag){}

		private:
			GINL GAIA::GVOID init()
			{
				m_pHttp = GNIL;
				m_pRequest = GNIL;
			}

		private:
			GAIA::NETWORK::Http* m_pHttp;
			GAIA::NETWORK::HttpRequest* m_pRequest;
		};

		class HttpAsyncDispatcher : public GAIA::NETWORK::AsyncDispatcher
		{
			friend class HttpRequest;
			friend class Http;

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
			if(this->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_READY)
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

			m_sTotalHeadSize =
					GAIA::NETWORK::HTTP_METHOD_STRING_LEN[m_method] + 1 +
					m_url.Size() + 1 +
					m_pHttp->GetDesc().sHttpVerLen + 2 +
					m_head.GetStringSize() + 1;

			this->rise_ref();

			m_uRequestTime = GAIA::TIME::tick_time();

			// Request thread magic index.
			{
				GAIA::SYNC::Autolock al(m_pHttp->m_lrCurrentThreadMagicIndex);
				m_uThreadMagicIndex = m_pHttp->m_uCurrentThreadMagicIndex++;
			}

			// Push to pending list.
			{
				GAST(this->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING);
				this->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING);

				GAIA::SYNC::Autolock al(m_pHttp->m_lrPendingList);
				m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING;
				GAIA::BL bInsertResult = m_pHttp->m_pendinglist.insert(this);
				GAST(bInsertResult);
			}

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
			return m_eventforcomplete.Wait(uMilliSeconds);
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
				pThread->uThreadIndex = (GAIA::U32)x;
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

			// Release all pending requests.
			{
				GAIA::SYNC::Autolock al(m_lrPendingList);
				for(__RequestSetType::it it = m_pendinglist.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpRequest* pRequest = *it;
					GAST(pRequest != GNIL);
					pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID;
					pRequest->drop_ref();
				}
				m_pendinglist.clear();
			}

			// Release all connecting requests.
			{
				GAIA::SYNC::Autolock al(m_lrConnectingList);
				for(__RequestSetType::it it = m_connectinglist.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpRequest* pRequest = *it;
					GAST(pRequest != GNIL);
					pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID;
					pRequest->drop_ref();
				}
				m_connectinglist.clear();
			}

			// Release all requesting requests.
			{
				GAIA::SYNC::Autolock al(m_lrRequestingList);
				for(__RequestSetType::it it = m_requestinglist.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpRequest* pRequest = *it;
					GAST(pRequest != GNIL);
					pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID;
					pRequest->drop_ref();
				}
				m_requestinglist.clear();
			}

			// Release all waiting requests.
			{
				GAIA::SYNC::Autolock al(m_lrWaitingList);
				for(__RequestSetType::it it = m_waitinglist.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpRequest* pRequest = *it;
					GAST(pRequest != GNIL);
					pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID;
					pRequest->drop_ref();
				}
				m_waitinglist.clear();
			}

			// Release all responsing requests.
			{
				GAIA::SYNC::Autolock al(m_lrResponsingList);
				for(__RequestSetType::it it = m_responsinglist.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpRequest* pRequest = *it;
					GAST(pRequest != GNIL);
					pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID;
					pRequest->drop_ref();
				}
				m_responsinglist.clear();
			}

			// Release all sockets.
			{
				GAIA::SYNC::Autolock al(m_lrSocks);
				for(__SockSetType::it it = m_socks.frontit(); !it.empty(); ++it)
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

		GAIA::BL Http::Execute(GAIA::NETWORK::HttpWorkThread* pWorkThread, GAIA::BL bConnect, GAIA::BL bRequest, GAIA::BL bTimeout, GAIA::BL bRecycleCache)
		{
			GAIA::BL bRet = GAIA::False;
			GAIA::U64 uCurrentTime = GAIA::TIME::tick_time();

			//
			__RequestVectorType listTempNeedCloseRequests;
			__RequestVectorType* pNeedCloseRequests = &listTempNeedCloseRequests;
			if(pWorkThread != GNIL)
			{
				pNeedCloseRequests = &pWorkThread->listTempRequestListForClose;
				pNeedCloseRequests->clear();
			}

			// Connect.
			if(bConnect)
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
						GAIA::SYNC::Autolock al(m_lrPendingList);
						if(!m_pendinglist.empty())
						{
							__RequestSetType::it itfront = m_pendinglist.frontit();
							GAST(!itfront.empty());
							pRequest = *itfront;
							itfront.erase();
						}
					}

					if(pRequest != GNIL)
					{
						//
						pRequest->OnBegin();

						//
						const GAIA::NETWORK::HttpURL& url = pRequest->GetURL();
						GAIA::CH szHostName[64];
						GAIA::NUM sHostNameLen;
						const GAIA::CH* pszHostName = url.GetHostName(szHostName, sizeof(szHostName), &sHostNameLen);
						if(GAIA::ALGO::gstremp(pszHostName))
							pszHostName = GAIA::NETWORK::GAIA_LOCAL_HOST_IP;

						//
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

						//
						addrHost.uPort = url.GetPort();
						if(addrHost.uPort == 0)
							addrHost.uPort = GAIA::NETWORK::HTTP_DEFAULT_PORT;

						//
						if(bHostAddressIsValid)
						{
							// Insert to connecting list.
							{
								GAST(pRequest->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING);
								pRequest->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING);

								GAIA::SYNC::Autolock al(m_lrConnectingList);
								pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING;
								GAIA::BL bInsertResult = m_connectinglist.insert(pRequest);
								GAST(bInsertResult);
							}

							// Allocate socket.
							HttpAsyncSocket* pSocket = gnew HttpAsyncSocket(*this, *m_disp);
							pRequest->m_pSock = pSocket;
							pSocket->m_pRequest = pRequest;
							pSocket->rise_ref();

							// Insert to socket list.
							{
								GAIA::SYNC::Autolock al(m_lrSocks);
								GAIA::BL bInsertResult = m_socks.insert(pSocket);
								GAST(bInsertResult);
							}

							// Create socket.
							pSocket->Create();

							// Connect to server.
							pSocket->Connect(addrHost);

							//
							bRet = GAIA::True;
						}
						else
						{
							pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_DOMAIN_ANALYSIS_FAILED;
							pNeedCloseRequests->push_back(pRequest);
						}
					}
				}
			}

			// Request.
			if(bRequest)
			{
				GAIA::SYNC::AutolockR al(m_rwExecuteRequest, pWorkThread != GNIL);

				__RequestVectorType listTemp;
				__RequestVectorType* pTempList = &listTemp;

				// Collect request list..
				{
					if(pWorkThread != GNIL)
					{
						pTempList = &pWorkThread->listTempRequestListForRequest;
						pTempList->clear();
					}
					GAIA::SYNC::Autolock al(m_lrRequestingList);
					for(__RequestSetType::it it = m_requestinglist.frontit(); !it.empty(); ++it)
					{
						GAIA::NETWORK::HttpRequest* pRequest = *it;
						if(pWorkThread == GNIL || pRequest->m_uThreadMagicIndex % m_listWorkThreads.size() == pWorkThread->uThreadIndex)
						{
							pTempList->push_back(pRequest);
							pRequest->rise_ref();
							pRequest->m_pSock->rise_ref();
						}
					}
				}

				// Request.
				{
					GAIA::CTN::Buffer reqbuf;
					GAIA::CTN::Buffer* pReqBuf = &reqbuf;
					if(pWorkThread != GNIL)
					{
						pReqBuf = &pWorkThread->reqbuf;
						pReqBuf->clear();
					}

					for(GAIA::NUM x = 0; x < pTempList->size(); ++x)
					{
						GAIA::NETWORK::HttpRequest* pRequest = (*pTempList)[x];
						GAST(pRequest != GNIL);

						do
						{
							// Send head.
							if(!pRequest->m_bHeadSendingComplete)
							{
								// Method.
								GAIA::NETWORK::HTTP_METHOD method = pRequest->GetMethod();
								const GAIA::CH* pszMethodString = GAIA::NETWORK::HTTP_METHOD_STRING[method];
								GAIA::NUM sMethodStringLen = GAIA::NETWORK::HTTP_METHOD_STRING_LEN[method];
								pReqBuf->write(pszMethodString, sMethodStringLen);
								pReqBuf->write(" ", 1);

								// URL.
								GAIA::NETWORK::HttpURL& url = pRequest->GetURL();
								GAIA::NUM sUrlLen;
								const GAIA::CH* pszUrl = url.ToString(&sUrlLen);
								pReqBuf->write(pszUrl, sUrlLen);
								pReqBuf->write(" ", 1);

								// Http version.
								pReqBuf->write(m_desc.szHttpVer, m_desc.sHttpVerLen);
								pReqBuf->write("\r\n", 2);

								// Head.
								GAIA::NETWORK::HttpHead& head = pRequest->GetHead();
								GAIA::NUM sHeadSize = head.GetStringSize();

								// Blank line.
								pReqBuf->write("\n", 1);

								// Send.
								GAST(pReqBuf->write_size() == pRequest->m_sTotalHeadSize);
								pRequest->m_lSendingSize += pReqBuf->write_size();
								GAIA::N32 nSendSize = pRequest->m_pSock->Send(pReqBuf->fptr(), pReqBuf->write_size());
								GAST(nSendSize == pReqBuf->write_size());
								if(nSendSize != pReqBuf->write_size())
								{
									pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_UNKNOWN;
									pNeedCloseRequests->push_back(pRequest);
									break;
								}
								pReqBuf->clear();

								// Update info.
								pRequest->m_uLastSendingTime = uCurrentTime;
								pRequest->m_bHeadSendingComplete = GAIA::True;
							}

							// Send body.
							if(!pRequest->m_bBodySendingComplete)
							{
								// Collect data what need send.
								GAIA::BL bBodySendingComplete = GAIA::False;
								if(!pRequest->m_buf.nill())
								{
									GAIA::NUM sRemain = pRequest->m_buf.remain();
									if(sRemain > 0)
									{
										if(sRemain > 1024 * 100)
											sRemain = 1024 * 100;
										pReqBuf->resize(sRemain);
										pRequest->m_buf.read(pReqBuf->fptr(), sRemain);
									}
									if(pRequest->m_buf.remain() == 0)
										bBodySendingComplete = GAIA::True;
								}
								else
								{
									pReqBuf->resize(1024 * 100);
									GAIA::N64 lPracticeSize = pRequest->OnRequestBodyData(pRequest->m_lSendingSize - pRequest->m_sTotalHeadSize, pReqBuf->fptr(), pReqBuf->write_size());
									pReqBuf->resize(lPracticeSize);
									if(lPracticeSize == 0)
										bBodySendingComplete = GAIA::True;
									else if(lPracticeSize == GINVALID)
									{
										pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_UNKNOWN;
										pNeedCloseRequests->push_back(pRequest);
										break;
									}
								}

								// If exist data need send, send it.
								if(pReqBuf->write_size() > 0)
								{
									// Send.
									pRequest->m_lSendingSize += pReqBuf->write_size();
									GAIA::N32 nSendSize = pRequest->m_pSock->Send(pReqBuf->fptr(), pReqBuf->write_size());
									GAST(nSendSize == pReqBuf->write_size());
									if(nSendSize != pReqBuf->write_size())
									{
										pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_UNKNOWN;
										pNeedCloseRequests->push_back(pRequest);
										break;
									}
									pReqBuf->clear();

									// Update info.
									pRequest->m_uLastSendingTime = uCurrentTime;
								}

								// If body complete, record it.
								if(bBodySendingComplete)
									pRequest->m_bBodySendingComplete = GAIA::True;
							}
						}
						while(GAIA::ALWAYSFALSE);
					}
				}

				// Recycle request list.
				{
					for(GAIA::NUM x = 0; x < pTempList->size(); ++x)
					{
						GAIA::NETWORK::HttpRequest* pRequest = (*pTempList)[x];
						GAST(pRequest != GNIL);
						pRequest->m_pSock->drop_ref();
						pRequest->drop_ref();
					}
				}
			}

			// Timeout dispatch.
			if(bTimeout)
			{
				GAIA::SYNC::Autolock al(m_lrExecuteTimeout);

			}

			// Recycle cache.
			if(bRecycleCache)
			{
				GAIA::SYNC::Autolock al(m_lrExecuteRecycleCache);

			}

			// Close the request.
			{
				for(GAIA::NUM x = 0; x < pNeedCloseRequests->size(); ++x)
				{
					GAIA::NETWORK::HttpRequest& req = *(*pNeedCloseRequests)[x];

					// Socket object.
					{
						GAIA::NETWORK::HttpAsyncSocket* pSocket = req.m_pSock;
						if(pSocket != GNIL)
						{
							// Remove from list.
							{
								GAIA::SYNC::Autolock al(m_lrSocks);
								GAIA::BL bEraseResult = m_socks.erase(pSocket);
								GAST(bEraseResult);
							}

							// Close socket.
							pSocket->Shutdown();
							pSocket->Close();

							// Release.
							pSocket->drop_ref();
						}
					}

					// Request object.
					{
						// Call back state changement.
						if(req.m_NetworkError == GAIA::NETWORK::NETWORK_ERROR_INVALID)
							req.m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_OK;
						if(req.m_NetworkError == GAIA::NETWORK::NETWORK_ERROR_OK)
						{

						}

						// Callback End.
						req.OnEnd(GAIA::False);
						req.m_eventforcomplete.Fire();

						// Remove from list.
						{
							switch(req.GetState())
							{
							case GAIA::NETWORK::HTTP_REQUEST_STATE_READY:
								break;
							case GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING:
								{
									GAIA::SYNC::Autolock al(m_lrPendingList);
									GAIA::BL bEraseResult = m_pendinglist.erase(&req);
									GAST(bEraseResult);
								}
								break;
							case GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING:
								{
									GAIA::SYNC::Autolock al(m_lrConnectingList);
									GAIA::BL bEraseResult = m_connectinglist.erase(&req);
									GAST(bEraseResult);
								}
								break;
							case GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING:
								{
									GAIA::SYNC::Autolock al(m_lrRequestingList);
									GAIA::BL bEraseResult = m_requestinglist.erase(&req);
									GAST(bEraseResult);
								}
								break;
							case GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING:
								{
									GAIA::SYNC::Autolock al(m_lrWaitingList);
									GAIA::BL bEraseResult = m_waitinglist.erase(&req);
									GAST(bEraseResult);
								}
								break;
							case GAIA::NETWORK::HTTP_REQUEST_STATE_RESPONSING:
								{
									GAIA::SYNC::Autolock al(m_lrResponsingList);
									GAIA::BL bEraseResult = m_responsinglist.erase(&req);
									GAST(bEraseResult);
								}
								break;
							default:
								GASTFALSE;
								break;
							}
						}

						// Unreference socket.
						if(req.m_pSock != GNIL)
						{
							req.m_pSock->Shutdown();
							req.m_pSock->Close();
							req.m_pSock->drop_ref();
						}

						// Release.
						req.drop_ref();
					}
				}
			}

			return GAIA::True;
		}

		GAIA::BL Http::CleanupCookic(GAIA::BL bRAM, GAIA::BL bFile, const GAIA::U64& uBeyondTime)
		{
			return GAIA::True;
		}
	}
}