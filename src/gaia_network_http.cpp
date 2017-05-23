#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_sync_base.h>
#include <gaia_time.h>
#include <gaia_network_http.h>
#include <gaia_type_impl.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#include <gaia_network_base_impl.h>
#include <gaia_network_socket_impl.h>

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
						GAIA::SYNC::gsleep(10);
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
				
				if(descHttp.nSocketSendTimeout != GINVALID)
					this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_SENDTIMEOUT, descHttp.nSocketSendTimeout);
				if(descHttp.nSocketRecvTimeout != GINVALID)
					this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_RECVTIMEOUT, descHttp.nSocketRecvTimeout);
			}

		protected:
			virtual GAIA::GVOID OnCreated(GAIA::BL bResult){}
			virtual GAIA::GVOID OnClosed(GAIA::BL bResult){}
			virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}
			virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr)
			{
				// Check state and parameter.
				if(!bResult)
				{
					//
					m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_CONNECT_FAILED;

					// Swap to complete list.
					this->SwapToCompleteList();

					return;
				}
				GAST(m_pRequest != GNIL);

				m_pRequest->m_uConnectedTime = GAIA::TIME::tick_time();
				
				if(m_pHttp->m_bLog)
				{
					GAIA::CH szTemp[32];
					addr.tostring(szTemp);
					GDEV << "[Http] HttpAsyncSocket::OnConnected: Connected to " << szTemp << GEND;
				}
				
				// Change state.
				{
					GAIA::SYNC::Autolock al(m_pRequest->m_lrState);

					// Remove from connecting list.
					{
						GAIA::SYNC::Autolock al(m_pHttp->m_lrConnectingList);
						GAIA::BL bEraseResult = m_pHttp->m_connectinglist.erase(m_pRequest);
						if(!bEraseResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::OnConnected: Erase request from connecting list failed!" << GEND;
					}

					// Insert to requesting list.
					{
						GAST(m_pRequest->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING);
						m_pRequest->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING);

						GAIA::SYNC::Autolock al(m_pHttp->m_lrRequestingList);
						m_pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING;
						GAIA::BL bInsertResult = m_pHttp->m_requestinglist.insert(m_pRequest);
						if(!bInsertResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::OnConnected: Insert request to requesting list failed!" << GEND;
					}
				}
			}
			virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult, GAIA::BL bByRemote)
			{
				// Check state and parameter.
				if(!bResult)
				{
					//
					m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_LOSTCONNECTION;

					// Swap to complete list.
					this->SwapToCompleteList();

					return;
				}
				GAST(m_pRequest != GNIL);

				if(m_pHttp->m_bLog)
				{
					GAIA::N64 lExpectSize = m_pRequest->m_lExpectRespBodySize;
					if(lExpectSize == GINVALID)
						lExpectSize = 0;
					GDEV << "[Http] HttpAsyncSocket::OnDisconnected: Disconnected progress " << m_pRequest->m_lRecvedSize << "/" << m_pRequest->m_sTotalRespHeadSize + lExpectSize << GEND;
				}

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
					//
					m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_LOSTCONNECTION;

					// Swap to complete list.
					this->SwapToCompleteList();

					return;
				}
				if(nPracticeSize == 0)
					return;
				GAST(m_pRequest != GNIL);

				if(m_pHttp->m_bLog)
					GDEV << "[Http] HttpAsyncSocket::OnSent: Sent " << nPracticeSize << " bytes, progress " << m_pRequest->m_lSentSize << "/" << m_pRequest->m_lSendingSize << GEND;

				// Callback.
				m_pRequest->OnSent(m_pRequest->m_lSentSize, pData, nPracticeSize);
				m_pRequest->m_lSentSize += nPracticeSize;
				m_pRequest->m_uLastSentTime = GAIA::TIME::tick_time();

				if(m_pRequest->m_lSentSize >= m_pRequest->m_sTotalReqHeadSize &&
				   m_pRequest->m_lSentSize - nPracticeSize < m_pRequest->m_sTotalReqHeadSize)
					m_pRequest->OnSentHead(m_pHttp->GetDesc().szHttpVer, m_pRequest->GetMethod(), m_pRequest->GetURL(), m_pRequest->GetHead());

				if(m_pRequest->m_lSentSize > m_pRequest->m_sTotalReqHeadSize)
				{
					GAIA::N64 lOffset = m_pRequest->m_lSentSize - nPracticeSize - m_pRequest->m_sTotalReqHeadSize;
					GAIA::N32 nSize = nPracticeSize;
					const GAIA::U8* pDataU8 = (const GAIA::U8*)pData;
					if(lOffset < 0)
					{
						pDataU8 -= lOffset;
						nSize += lOffset;
						lOffset = 0;
					}
					GAST(nSize > 0);
					m_pRequest->OnSentBody(lOffset, pDataU8, nSize);
				}

				// Change state.
				if(m_pRequest->m_lSentSize > m_pRequest->m_lSendingSize && m_pHttp->m_bLog)
					GERR << "[Http] HttpAsyncSocket::OnSent: SentSize above sending size, " << m_pRequest->m_lSentSize << ">" << m_pRequest->m_lSendingSize << GEND;
				if(m_pRequest->m_bHeadSendingComplete && m_pRequest->m_bBodySendingComplete &&
				   m_pRequest->m_lSentSize == m_pRequest->m_lSendingSize)
				{
					if(m_pRequest->GetState() == GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING)
					{
						GAIA::SYNC::Autolock al(m_pRequest->m_lrState);

						// Remove from requesting list.
						{
							GAIA::SYNC::Autolock al(m_pHttp->m_lrRequestingList);
							GAIA::BL bEraseResult = m_pHttp->m_requestinglist.erase(m_pRequest);
							if(!bEraseResult && m_pHttp->m_bLog)
								GERR << "[Http] HttpAsyncSocket::OnSent: Erase request from requesting list failed!" << GEND;
						}

						// Insert to waiting list.
						{
							GAST(m_pRequest->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING);
							m_pRequest->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING);

							GAIA::SYNC::Autolock al(m_pHttp->m_lrWaitingList);
							m_pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING;
							GAIA::BL bInsertResult = m_pHttp->m_waitinglist.insert(m_pRequest);
							if(!bInsertResult && m_pHttp->m_bLog)
								GERR << "[Http] HttpAsyncSocket::OnSent: Insert request to waiting list failed!" << GEND;
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
					if(m_pRequest->m_bHeadRecvedComplete && m_pRequest->m_lExpectRespBodySize != GINVALID &&
					   m_pRequest->m_lRecvedSize - m_pRequest->m_sTotalRespHeadSize >= m_pRequest->m_lExpectRespBodySize) // If body received complete.
						m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_OK;
					else
						m_pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_LOSTCONNECTION;

					// Swap to complete list.
					this->SwapToCompleteList();

					return;
				}
				GAST(m_pRequest != GNIL);

				if(m_pHttp->m_bLog)
				{
					if(m_pRequest->m_lExpectRespBodySize == GINVALID)
						GDEV << "[Http] HttpAsyncSocket::OnRecved: Received " << nSize << " bytes, progress " << m_pRequest->m_lRecvedSize + nSize << "/" << "?" << GEND;
					else
						GDEV << "[Http] HttpAsyncSocket::OnRecved: Received " << nSize << " bytes, progress " << m_pRequest->m_lRecvedSize + nSize << "/" << m_pRequest->m_sTotalRespHeadSize + m_pRequest->m_lExpectRespBodySize << GEND;
				}

				// Change state.
				if(m_pRequest->m_lRecvedSize == 0)
				{
					GAIA::SYNC::Autolock al(m_pRequest->m_lrState);

					if(m_pRequest->GetState() == GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING)
					{
						// Remove from requesting list.
						GAIA::SYNC::Autolock al(m_pHttp->m_lrRequestingList);
						GAIA::BL bEraseResult = m_pHttp->m_requestinglist.erase(m_pRequest);
						if(!bEraseResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::OnRecved: Erase request from requesting list failed!" << GEND;
					}
					else if(m_pRequest->GetState() == GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING)
					{
						// Remove from waiting list.
						GAIA::SYNC::Autolock al(m_pHttp->m_lrWaitingList);
						GAIA::BL bEraseResult = m_pHttp->m_waitinglist.erase(m_pRequest);
						if(!bEraseResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::OnRecved: Erase request from waiting list failed!" << GEND;
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
						if(!bInsertResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::OnRecved: Insert request to responsing list failed!" << GEND;
					}
				}

				// Callback.
				m_pRequest->OnRecved(m_pRequest->m_lRecvedSize, pData, nSize);
				m_pRequest->m_lRecvedSize += nSize;
				m_pRequest->m_uLastRecvedTime = GAIA::TIME::tick_time();

				if(m_pRequest->m_bHeadRecvedComplete)
				{
					GAIA::N64 lOffset = m_pRequest->m_lRecvedSize - nSize;
					GAST(lOffset >= m_pRequest->m_sTotalRespHeadSize);
					lOffset -= m_pRequest->m_sTotalRespHeadSize;
					m_pRequest->OnRecvedBody(lOffset, pData, nSize);
				}
				else
				{
					m_pRequest->m_respbuf.write(pData, nSize);
					GAIA::U8* pReader = m_pRequest->m_respbuf.read_ptr();
					GAIA::U8* pWriter = m_pRequest->m_respbuf.write_ptr();
					while(pReader < pWriter)
					{
						if(pReader[0] == '\r' &&
						   pReader + 1 < pWriter && pReader[1] == '\n' &&
						   pReader + 2 < pWriter && (pReader[2] == '\r' || pReader[2] == '\n') &&
						   pReader + 3 < pWriter && (pReader[3] == '\r' || pReader[3] == '\n'))
						{
							GAIA::NUM sHeadEndOffset = (GAIA::NUM)(pReader - m_pRequest->m_respbuf.fptr() + 4);
							m_pRequest->m_respbuf.seek_read(sHeadEndOffset);
							m_pRequest->m_bHeadRecvedComplete = GAIA::True;
							break;
						}
						++pReader;
					}
					if(m_pRequest->m_bHeadRecvedComplete)
					{
						//
						m_pRequest->m_sTotalRespHeadSize = m_pRequest->m_respbuf.read_size();

						// Analyze.
						GAIA::CH* pszHead = (GAIA::CH*)m_pRequest->m_respbuf.fptr();
						GAIA::CH* pszHttpVersion = pszHead;
						GAIA::CH* pszHttpCode = GNIL;
						GAIA::CH* pszHttpCodeDesc = GNIL;
						GAIA::NETWORK::HttpHead resphead;
						for(GAIA::NUM x = 0; x < m_pRequest->m_sTotalRespHeadSize; ++x)
						{
							if(pszHead[x] == ' ')
							{
								if(pszHttpCode == GNIL)
								{
									pszHttpCode = &pszHead[x + 1];
									pszHead[x] = '\0'; // End string.
								}
								else if(pszHttpCodeDesc == GNIL)
								{
									pszHttpCodeDesc = &pszHead[x + 1];
									pszHead[x] = '\0'; // End string.
								}
							}
							else if(pszHead[x] == '\r')
							{
								pszHead[x] = '\0'; // End string.

								// If analyze failed.
								if(x + 1 >= m_pRequest->m_sTotalRespHeadSize)
								{
									pszHttpVersion = GNIL;
									break;
								}
								if(pszHead[x + 1] != '\n')
								{
									pszHttpVersion = GNIL;
									break;
								}
								GAIA::NUM sHeadSize = m_pRequest->m_sTotalRespHeadSize - x - 4;
								if(sHeadSize > 0)
								{
									if(!resphead.FromString(&pszHead[x + 2], &sHeadSize))
									{
										pszHttpVersion = GNIL;
										break;
									}
								}
								break;
							}
						}
						const GAIA::CH* pszContentLength = resphead.GetValueByName(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH);
						if(pszContentLength != GNIL)
						{
							m_pRequest->m_lExpectRespBodySize = GAIA::ALGO::acasts(pszContentLength);
							if(m_pHttp->m_bLog)
								GDEV << "[Http] HttpAsyncSocket::OnRecved: Content length is " << pszContentLength << GEND;
						}

						// Callback head.
						if(pszHttpVersion != GNIL && pszHttpCode != GNIL && pszHttpCodeDesc != GNIL)
							m_pRequest->OnRecvedHead(pszHttpVersion, pszHttpCode, pszHttpCodeDesc, resphead);

						// Callback body.
						m_pRequest->m_respbuf.keep_remain();
						if(m_pRequest->m_respbuf.remain() > 0)
						{
							m_pRequest->OnRecvedBody(0, m_pRequest->m_respbuf.fptr(), m_pRequest->m_respbuf.write_size());
							m_pRequest->m_respbuf.destroy();
						}
					}
				}

				// Check complete, if complete, change state.
				{
					if(m_pRequest->m_lExpectRespBodySize != GINVALID &&
					   m_pRequest->m_lRecvedSize - m_pRequest->m_sTotalRespHeadSize >= m_pRequest->m_lExpectRespBodySize)
					{
						GAIA::SYNC::Autolock al(m_pRequest->m_lrState);

						// Remove from responsing list.
						{
							GAIA::SYNC::Autolock al(m_pHttp->m_lrResponsingList);
							GAIA::BL bEraseResult = m_pHttp->m_responsinglist.erase(m_pRequest);
							if(!bEraseResult && m_pHttp->m_bLog)
								GERR << "[Http] HttpAsyncSocket::OnRecved: Erase request from responsing list failed!" << GEND;
						}

						// Insert to complete list.
						{
							GAST(m_pRequest->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE);
							m_pRequest->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE);

							GAIA::SYNC::Autolock al(m_pHttp->m_lrCompleteList);
							m_pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE;
							GAIA::BL bInsertResult = m_pHttp->m_completelist.insert(m_pRequest);
							if(!bInsertResult && m_pHttp->m_bLog)
								GERR << "[Http] HttpAsyncSocket::OnRecved: Insert request to complete list failed!" << GEND;
						}
					}
				}
			}
			virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag){}

		private:
			GINL GAIA::GVOID init()
			{
				m_pHttp = GNIL;
				m_pRequest = GNIL;
			}
			GINL GAIA::GVOID SwapToCompleteList()
			{
				//
				GAIA::SYNC::Autolock al(m_pRequest->m_lrState);

				//
				m_pRequest->m_bSwapToCompleteList = GAIA::True;

				// Remove from old list.
				switch(m_pRequest->GetState())
				{
				case GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING:
					{
						GAIA::SYNC::Autolock al(m_pHttp->m_lrConnectingList);
						GAIA::BL bEraseResult = m_pHttp->m_connectinglist.erase(m_pRequest);
						if(!bEraseResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::SwapToCompleteList: Erase request from connecting list failed!" << GEND;
					}
					break;
				case GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING:
					{
						GAIA::SYNC::Autolock al(m_pHttp->m_lrRequestingList);
						GAIA::BL bEraseResult = m_pHttp->m_requestinglist.erase(m_pRequest);
						if(!bEraseResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::SwapToCompleteList: Erase request from requesting list failed!" << GEND;
					}
					break;
				case GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING:
					{
						GAIA::SYNC::Autolock al(m_pHttp->m_lrWaitingList);
						GAIA::BL bEraseResult = m_pHttp->m_waitinglist.erase(m_pRequest);
						if(!bEraseResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::SwapToCompleteList: Erase request from waiting list failed!" << GEND;
					}
					break;
				case GAIA::NETWORK::HTTP_REQUEST_STATE_RESPONSING:
					{
						GAIA::SYNC::Autolock al(m_pHttp->m_lrResponsingList);
						GAIA::BL bEraseResult = m_pHttp->m_responsinglist.erase(m_pRequest);
						if(!bEraseResult && m_pHttp->m_bLog)
							GERR << "[Http] HttpAsyncSocket::SwapToCompleteList: Erase request from responsing list failed!" << GEND;
					}
					break;
				default:
					GASTFALSE;
					break;
				}

				// Insert to complete list.
				{
					GAIA::SYNC::Autolock al(m_pHttp->m_lrCompleteList);
					GAIA::BL bInsertResult = m_pHttp->m_completelist.insert(m_pRequest);
					if(!bInsertResult && m_pHttp->m_bLog)
						GERR << "[Http] HttpAsyncSocket::SwapToCompleteList: Insert request to complete list failed!" << GEND;
				}
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
				GASTFALSE;
				HttpAsyncSocket* pListenSocket = gnew HttpAsyncSocket(*m_pHttp, *this, ASYNC_SOCKET_TYPE_LISTEN);
				return pListenSocket;
			}

			virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptingSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				GASTFALSE;
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
			if(m_bReqBufOwner)
				m_reqbuf.destroy();
			else
				m_reqbuf.proxy(GNIL, GNIL);
		}

		GAIA::GVOID HttpRequest::Reset()
		{
			if(m_bReqBufOwner)
				m_reqbuf.destroy();
			else
				m_reqbuf.proxy(GNIL, GNIL);
			this->init();
		}

		GAIA::BL HttpRequest::BindRequestBuffer(const GAIA::GVOID* p, GAIA::NUM sSize)
		{
			if(this->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_READY)
				return GAIA::False;

			if(p == GNIL)
			{
				if(sSize > 0)
				{
					m_bReqBufOwner = GAIA::True;
					m_reqbuf.resize(sSize);
				}
				else if(sSize == 0)
				{
					if(m_bReqBufOwner)
						m_reqbuf.destroy();
					else
						m_reqbuf.proxy(GNIL, GNIL);
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
					if(m_bReqBufOwner)
						m_reqbuf.destroy();
					else
						m_reqbuf.proxy(GNIL, GNIL);
					m_bReqBufOwner = GAIA::False;
					m_reqbuf.proxy((GAIA::U8*)p, (GAIA::U8*)p + sSize);
					m_reqbuf.seek_write(sSize);
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

			GAIA::NUM sRelativePartLength = m_url.GetRelativePartLength();
			if(sRelativePartLength == 0)
				sRelativePartLength = 1; // Default root path is "/".

			m_sTotalReqHeadSize =
					GAIA::NETWORK::HTTP_METHOD_STRING_LEN[m_method] + 1 +
					sRelativePartLength + 1 +
					m_pHttp->GetDesc().sHttpVerLen + 2 +
					m_head.GetStringSize() + 2;

			this->rise_ref("HttpRequest::Request:request rise ref");

			m_uRequestTime = GAIA::TIME::tick_time();

			{
				GAIA::SYNC::Autolock al(m_lrState);

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
					if(!bInsertResult && m_pHttp->m_bLog)
						GERR << "[Http] HttpRequest::Request: Insert request to pending list failed!" << GEND;
				}
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
			for(GAIA::NUM x = 0; x < desc.sWorkThreadCount; ++x)
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
				return GAIA::False;

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
				pThread->SetStopCmd(GAIA::False);
			}

			// Release all pending requests.
			{
				GAIA::SYNC::Autolock al(m_lrPendingList);
				for(__RequestSetType::it it = m_pendinglist.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpRequest* pRequest = *it;
					GAST(pRequest != GNIL);
					pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID;
					pRequest->drop_ref("Http::End:request drop ref for pending request");
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
					pRequest->drop_ref("Http::End:request drop ref for connecting request");
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
					pRequest->drop_ref("Http::End:request drop ref for requesting request");
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
					pRequest->drop_ref("Http::End:request drop ref for waiting request");
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
					pRequest->drop_ref("Http::End:request drop ref for responsing request");
				}
				m_responsinglist.clear();
			}

			// Release all complete requests.
			{
				GAIA::SYNC::Autolock al(m_lrCompleteList);
				for(__RequestSetType::it it = m_completelist.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpRequest* pRequest = *it;
					GAST(pRequest != GNIL);
					pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_INVALID;
					pRequest->drop_ref("Http::End:request drop ref for complete request");
				}
				m_completelist.clear();
			}

			// Release all sockets.
			{
				GAIA::SYNC::Autolock al(m_lrSocks);
				for(__SockSetType::it it = m_socks.frontit(); !it.empty(); ++it)
				{
					GAIA::NETWORK::HttpAsyncSocket* pSocket = *it;
					GAST(pSocket != GNIL);
					pSocket->drop_ref("Http::End:socket drop ref");
				}
				m_socks.clear();
			}

			// End async network.
			m_disp->End();

			m_bBegin = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL Http::Execute(GAIA::NETWORK::HttpWorkThread* pWorkThread)
		{
			GAIA::BL bRet = GAIA::False;
			GAIA::U64 uCurrentTime = GAIA::TIME::tick_time();

			// Allocate need close requests list.
			__RequestVectorType listTempNeedCloseRequests;
			__RequestVectorType* pNeedCloseRequests = &listTempNeedCloseRequests;
			if(pWorkThread != GNIL)
			{
				pNeedCloseRequests = &pWorkThread->listTempRequestListForClose;
				pNeedCloseRequests->clear();
			}

			// Connect.
			{
				GAIA::BL bConnectionIsFull = GAIA::False;
				{
					GAIA::SYNC::Autolock al(m_lrSocks);
					if(m_socks.size() >= m_desc.sMaxConnCount)
						bConnectionIsFull = GAIA::True;
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
						if(!addrHost.ip.fromstring(pszHostName))
						{
							if(!GAIA::NETWORK::GetHostIP(pszHostName, addrHost.ip))
								bHostAddressIsValid = GAIA::False;
						}

						//
						addrHost.uPort = url.GetPort();
						if(addrHost.uPort == 0)
							addrHost.uPort = GAIA::NETWORK::HTTP_DEFAULT_PORT;

						//
						if(bHostAddressIsValid)
						{
							//
							pRequest->m_uConnectingTime = uCurrentTime;
							
							// Allocate socket.
							HttpAsyncSocket* pSocket = gnew HttpAsyncSocket(*this, *m_disp);
							pRequest->m_pSock = pSocket;
							pSocket->m_pRequest = pRequest;
							pSocket->rise_ref("Http::Execute:socket rise ref for connect");

							// Insert to socket list.
							{
								GAIA::SYNC::Autolock al(m_lrSocks);
								GAIA::BL bInsertResult = m_socks.insert(pSocket);
								if(!bInsertResult && m_bLog)
									GERR << "[Http] Http::Execute: Insert socket to socket list failed!" << GEND;
							}

							// Create socket.
							pSocket->Create();

							// Insert to connecting list.
							{
								GAIA::SYNC::Autolock al(pRequest->m_lrState);

								GAST(pRequest->GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING);
								pRequest->OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING);

								{
									GAIA::SYNC::Autolock al(m_lrConnectingList);
									pRequest->m_state = GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING;
									GAIA::BL bInsertResult = m_connectinglist.insert(pRequest);
									if(!bInsertResult && m_bLog)
										GERR << "[Http] Http::Execute: Insert request to connecting list failed!" << GEND;
								}
							}

							// Connect to server.
							GTRY
							{
								pSocket->Connect(addrHost);
								if(m_bLog)
								{
									GAIA::CH szAddress[32];
									addrHost.tostring(szAddress);
									GDEV << "[Http] Http::Execute: Connecting to " << szAddress << GEND;
								}
								bRet = GAIA::True;
							}
							GCATCHBASE
							{
								pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_CONNECT_FAILED;
								pNeedCloseRequests->push_back(pRequest);
								
								// Erase from connection list.
								{
									GAIA::SYNC::Autolock al(m_lrConnectingList);
									GAIA::BL bEraseResult = m_connectinglist.erase(pRequest);
									if(!bEraseResult && m_bLog)
										GERR << "[Http] Http::Execute: Erase from connecting list failed when connect failed!" << GEND;
								}
							}
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
			{
				GAIA::SYNC::AutolockRW al(m_rwExecuteRequest, pWorkThread != GNIL);

				__RequestVectorType listTemp;
				__RequestVectorType* pTempList = &listTemp;

				// Collect request list.
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
							pRequest->rise_ref("Http::Execute:request rise ref for request");
							pRequest->m_pSock->rise_ref("Http::Execute:socket rise ref for request");
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
								const GAIA::CH* pszUrl = url.GetRelativePart();
								sUrlLen = url.GetRelativePartLength();
								static const GAIA::CH ROOT_PATH[] = "/";
								if(pszUrl == GNIL)
								{
									pszUrl = ROOT_PATH;
									sUrlLen = 1;
								}
								pReqBuf->write(pszUrl, sUrlLen);
								pReqBuf->write(" ", 1);

								// Http version.
								pReqBuf->write(m_desc.szHttpVer, m_desc.sHttpVerLen);
								pReqBuf->write("\r\n", 2);

								// Head.
								GAIA::NETWORK::HttpHead& head = pRequest->GetHead();
								head.Optimize();
								GAIA::NUM sHeadSize = head.GetStringSize();
								GAST(sHeadSize >= 0);
								if(sHeadSize > 0)
								{
									GAIA::U8* pHeadBuf = pReqBuf->alloc(sHeadSize + 1);
									GAIA::NUM sPracHeadSize;
									GAIA::BL bHeadToStringResult;
									head.ToString((GAIA::CH*)pHeadBuf, sHeadSize + 1, &sPracHeadSize, &bHeadToStringResult);
									GAST(bHeadToStringResult);
									GAST(sPracHeadSize == sHeadSize);
									pReqBuf->resize(pReqBuf->write_size() - 1);
								}

								// Blank line.
								pReqBuf->write("\r\n", 2);

								// Send.
								GTRY
								{
									GAIA::NUM sWriteSize = pReqBuf->write_size();
									GAST(sWriteSize == pRequest->m_sTotalReqHeadSize);
									pRequest->m_lSendingSize += sWriteSize;

									GAIA::N32 nSendSize = pRequest->m_pSock->Send(pReqBuf->fptr(), sWriteSize);
									GAST(nSendSize == sWriteSize);
									if(nSendSize != sWriteSize)
									{
										pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_UNKNOWN;
										pNeedCloseRequests->push_back(pRequest);
										break;
									}
									pReqBuf->clear();

									// Update info.
									pRequest->m_uLastSendingTime = uCurrentTime;
									pRequest->m_bHeadSendingComplete = GAIA::True;

									// Logout.
									if(m_bLog)
										GDEV << "[Http] Http::Execute: Send head " << sWriteSize << " bytes" << GEND;

									bRet = GAIA::True;
								}
								GCATCHALL
								{
									pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_UNKNOWN;
									pNeedCloseRequests->push_back(pRequest);
									break;
								}
							}

							// Send body.
							if(!pRequest->m_bBodySendingComplete)
							{
								// Collect data what need send.
								GAIA::BL bBodySendingComplete = GAIA::False;
								if(!pRequest->m_reqbuf.nill())
								{
									GAIA::NUM sRemain = pRequest->m_reqbuf.remain();
									if(sRemain > 0)
									{
										if(sRemain > 1024 * 100)
											sRemain = 1024 * 100;
										pReqBuf->resize(sRemain);
										pRequest->m_reqbuf.read(pReqBuf->fptr(), sRemain);
									}
									if(pRequest->m_reqbuf.remain() == 0)
										bBodySendingComplete = GAIA::True;
								}
								else
								{
									pReqBuf->resize(1024 * 100);
									GAIA::N64 lPracticeSize = pRequest->OnRequestBodyData(pRequest->m_lSendingSize - pRequest->m_sTotalReqHeadSize, pReqBuf->fptr(), pReqBuf->write_size());
									pReqBuf->resize((GAIA::NUM)lPracticeSize);
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
									GTRY
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

										// Logout.
										if(m_bLog)
											GDEV << "[Http] Http::Execute: Send body " << nSendSize << " bytes" << GEND;

										// Update info.
										pRequest->m_uLastSendingTime = uCurrentTime;

										bRet = GAIA::True;
									}
									GCATCHALL
									{
										pRequest->m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_UNKNOWN;
										pNeedCloseRequests->push_back(pRequest);
										break;
									}
								}

								// If body complete, record it.
								if(bBodySendingComplete)
								{
									pRequest->m_bBodySendingComplete = GAIA::True;
									bRet = GAIA::True;
								}
							}
						}
						while(GAIA::ALWAYSFALSE);
					}
				}

				// Drop request ref and socket ref.
				{
					for(GAIA::NUM x = 0; x < pTempList->size(); ++x)
					{
						GAIA::NETWORK::HttpRequest* pRequest = (*pTempList)[x];
						GAST(pRequest != GNIL);
						pRequest->m_pSock->drop_ref("Http::Execute:socket drop ref for request complete");
						pRequest->drop_ref("Http::Execute:request drop ref for request complete");
					}
				}
			}

			// Close the failed request.
			{
				for(GAIA::NUM x = 0; x < pNeedCloseRequests->size(); ++x)
				{
					GAIA::NETWORK::HttpRequest& req = *(*pNeedCloseRequests)[x];
					this->InternalCloseRequest(req);
					bRet = GAIA::True;
				}
				pNeedCloseRequests->clear();
			}

			// Close dispatch.
			{
				GAIA::SYNC::Autolock al(m_lrExecuteClose);

				// Collect completed request..
				{
					GAIA::SYNC::Autolock al(m_lrCompleteList);
					for(__RequestSetType::it it = m_completelist.frontit(); !it.empty(); ++it)
					{
						GAIA::NETWORK::HttpRequest* pRequest = *it;
						GAST(pRequest != GNIL);
						if(pWorkThread == GNIL || pRequest->m_uThreadMagicIndex % m_listWorkThreads.size() == pWorkThread->uThreadIndex)
							pNeedCloseRequests->push_back(pRequest);
					}
				}

				// Collect timeout request.
				{
					GAIA::SYNC::Autolock al(m_lrSocks);
					for(__SockSetType::it it = m_socks.frontit(); !it.empty(); ++it)
					{
						HttpAsyncSocket* pSock = *it;
						GAST(pSock != GNIL);
						GAST(pSock->m_pRequest != GNIL);
						HttpRequest* pRequest = pSock->m_pRequest;
						if(pWorkThread == GNIL || pRequest->m_uThreadMagicIndex % m_listWorkThreads.size() == pWorkThread->uThreadIndex)
						{
							// Check logic timeout.
							if(pSock->m_pRequest->m_uRequestTime != 0)
							{
								if(uCurrentTime > pSock->m_pRequest->m_uRequestTime)
								{
									GAIA::U64 uDeltaTime = uCurrentTime - pSock->m_pRequest->m_uRequestTime;
									if(uDeltaTime > pSock->m_pRequest->m_uLogicTimeout)
									{
										pNeedCloseRequests->push_back(pSock->m_pRequest);
										continue;
									}
								}
							}
							
							// Check network timeout.
							if(pSock->m_pRequest->m_uLastRecvedTime != 0)
							{
								if(uCurrentTime > pSock->m_pRequest->m_uLastRecvedTime)
								{
									GAIA::U64 uDeltaTime = uCurrentTime - pSock->m_pRequest->m_uLastRecvedTime;
									if(uDeltaTime > pSock->m_pRequest->m_uNetworkResponseTimeout)
									{
										pNeedCloseRequests->push_back(pSock->m_pRequest);
										continue;
									}
								}
							}
							else if(pSock->m_pRequest->m_uLastSentTime != 0)
							{
								if(uCurrentTime > pSock->m_pRequest->m_uLastSentTime)
								{
									GAIA::U64 uDeltaTime = uCurrentTime - pSock->m_pRequest->m_uLastSentTime;
									if(uDeltaTime > pSock->m_pRequest->m_uNetworkResponseTimeout)
									{
										pNeedCloseRequests->push_back(pSock->m_pRequest);
										continue;
									}
								}
							}
							else if(pSock->m_pRequest->m_uLastSendingTime != 0)
							{
								if(uCurrentTime > pSock->m_pRequest->m_uLastSendingTime)
								{
									GAIA::U64 uDeltaTime = uCurrentTime - pSock->m_pRequest->m_uLastSendingTime;
									if(uDeltaTime > pSock->m_pRequest->m_uNetworkResponseTimeout)
									{
										pNeedCloseRequests->push_back(pSock->m_pRequest);
										continue;
									}
								}
							}
							else if(pSock->m_pRequest->m_uConnectedTime != 0)
							{
								if(uCurrentTime > pSock->m_pRequest->m_uConnectedTime)
								{
									GAIA::U64 uDeltaTime = uCurrentTime - pSock->m_pRequest->m_uConnectedTime;
									if(uDeltaTime > pSock->m_pRequest->m_uNetworkResponseTimeout)
									{
										pNeedCloseRequests->push_back(pSock->m_pRequest);
										continue;
									}
								}
							}
							else if(pSock->m_pRequest->m_uConnectingTime != 0)
							{
								if(uCurrentTime > pSock->m_pRequest->m_uConnectingTime)
								{
									GAIA::U64 uDeltaTime = uCurrentTime - pSock->m_pRequest->m_uConnectingTime;
									if(uDeltaTime > pSock->m_pRequest->m_uNetworkResponseTimeout)
									{
										pNeedCloseRequests->push_back(pSock->m_pRequest);
										continue;
									}
								}
							}
						}
					}
				}

				// Make element in request list is unique.
				pNeedCloseRequests->sort();
				pNeedCloseRequests->unique();
				
				// Close the request.
				{
					for(GAIA::NUM x = 0; x < pNeedCloseRequests->size(); ++x)
					{
						GAIA::NETWORK::HttpRequest& req = *(*pNeedCloseRequests)[x];
						this->InternalCloseRequest(req);
						bRet = GAIA::True;
					}
					pNeedCloseRequests->clear();
				}
			}

			// Recycle cache.
			{
				GAIA::SYNC::Autolock al(m_lrExecuteRecycleCache);
			}

			return bRet;
		}

		GAIA::BL Http::CleanupCookic(GAIA::BL bRAM, GAIA::BL bFile, const GAIA::U64& uBeyondTime)
		{
			return GAIA::True;
		}

		GVOID Http::InternalCloseRequest(HttpRequest& req)
		{
			// Socket object.
			{
				if(req.m_pSock != GNIL)
				{
					// Remove from list.
					{
						GAIA::SYNC::Autolock al(m_lrSocks);
						GAIA::BL bEraseResult = m_socks.erase(req.m_pSock);
						if(!bEraseResult && m_bLog)
							GERR << "[Http] Http::InternalCloseSocket: Erase socket from socket list failed!" << GEND;
					}

					// Close socket.
					if(req.m_pSock->IsCreated())
					{
						GTRY
						{
							req.m_pSock->Shutdown();
							req.m_pSock->Close();
						}
						GCATCHALL{}
					}

					// Release.
					req.m_pSock->drop_ref("Http::InternalCloseRequest:socket drop ref front");
				}
			}

			// Request object.
			{
				// Call back state changement.
				if(req.m_NetworkError == GAIA::NETWORK::NETWORK_ERROR_INVALID)
					req.m_NetworkError = GAIA::NETWORK::NETWORK_ERROR_OK;
				if(req.m_NetworkError == GAIA::NETWORK::NETWORK_ERROR_OK)
				{
					if(req.GetState() != GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE)
						req.OnState(GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE);
				}

				// Callback End.
				req.OnEnd(GAIA::False);
				req.m_eventforcomplete.Fire();

				// Remove from list.
				{
					GAIA::SYNC::Autolock al(req.m_lrState);

					if(req.m_bSwapToCompleteList)
					{
						GAIA::SYNC::Autolock al(m_lrCompleteList);
						GAIA::BL bEraseResult = m_completelist.erase(&req);
						if(!bEraseResult && m_bLog)
							GERR << "[Http] Http::InternalCloseSocket: Erase request from complete list failed for complete flag set!" << GEND;
					}
					else
					{
						switch(req.GetState())
						{
						case GAIA::NETWORK::HTTP_REQUEST_STATE_PENDING:
							{
								GAIA::SYNC::Autolock al(m_lrPendingList);
								GAIA::BL bEraseResult = m_pendinglist.erase(&req);
								if(!bEraseResult && m_bLog)
									GERR << "[Http] Http::InternalCloseSocket: Erase request from pending list failed!" << GEND;
							}
							break;
						case GAIA::NETWORK::HTTP_REQUEST_STATE_CONNECTING:
							{
								GAIA::SYNC::Autolock al(m_lrConnectingList);
								GAIA::BL bEraseResult = m_connectinglist.erase(&req);
								if(!bEraseResult && m_bLog)
									GERR << "[Http] Http::InternalCloseSocket: Erase request from connecting list failed!" << GEND;
							}
							break;
						case GAIA::NETWORK::HTTP_REQUEST_STATE_REQUESTING:
							{
								GAIA::SYNC::Autolock al(m_lrRequestingList);
								GAIA::BL bEraseResult = m_requestinglist.erase(&req);
								if(!bEraseResult && m_bLog)
									GERR << "[Http] Http::InternalCloseSocket: Erase request from requesting list failed!" << GEND;
							}
							break;
						case GAIA::NETWORK::HTTP_REQUEST_STATE_WAITING:
							{
								GAIA::SYNC::Autolock al(m_lrWaitingList);
								GAIA::BL bEraseResult = m_waitinglist.erase(&req);
								if(!bEraseResult && m_bLog)
									GERR << "[Http] Http::InternalCloseSocket: Erase request from waiting list failed!" << GEND;
							}
							break;
						case GAIA::NETWORK::HTTP_REQUEST_STATE_RESPONSING:
							{
								GAIA::SYNC::Autolock al(m_lrResponsingList);
								GAIA::BL bEraseResult = m_responsinglist.erase(&req);
								if(!bEraseResult && m_bLog)
									GERR << "[Http] Http::InternalCloseSocket: Erase request from responsing list failed!" << GEND;
							}
							break;
						case GAIA::NETWORK::HTTP_REQUEST_STATE_COMPLETE:
							{
								GAIA::SYNC::Autolock al(m_lrCompleteList);
								GAIA::BL bEraseResult = m_completelist.erase(&req);
								if(!bEraseResult && m_bLog)
									GERR << "[Http] Http::InternalCloseSocket: Erase request from complete list failed!" << GEND;
							}
							break;
						default:
							GASTFALSE;
							break;
						}
					}
				}

				// Unreference socket.
				if(req.m_pSock != GNIL)
				{
					if(req.m_pSock->IsCreated())
					{
						GTRY
						{
							req.m_pSock->Shutdown();
							req.m_pSock->Close();
						}
						GCATCHALL{}
					}
					req.m_pSock->drop_ref("Http::InternalCloseRequest:socket drop ref back");
					req.m_pSock = GNIL;
				}

				// Release.
				req.drop_ref("Http::InternalCloseRequest:request drop ref");
			}
		}
	}
}
