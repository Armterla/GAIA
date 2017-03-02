#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_sync_base.h>
#include <gaia_fsys_file.h>
#include <gaia_fsys_dir.h>
#include <gaia_network_httpserver.h>

#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#include <gaia_network_socket_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		class HttpAsyncSocket : public GAIA::NETWORK::AsyncSocket
		{
			friend class HttpServerLink;
			friend class HttpServer;

		public:
			HttpAsyncSocket(GAIA::NETWORK::HttpServer& svr, GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::ASYNC_SOCKET_TYPE socktype = GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED)
				: GAIA::NETWORK::AsyncSocket(disp, socktype)
			{
				this->init();
				m_pSvr = &svr;
				m_method = GAIA::NETWORK::HTTP_METHOD_INVALID;
			}

			virtual ~HttpAsyncSocket()
			{
				if(m_pRecvBuf != GNIL)
					m_pSvr->ReleaseBuffer(m_pRecvBuf);
				if(m_pRecvBufSwap != GNIL)
					m_pSvr->ReleaseBuffer(m_pRecvBufSwap);
			}

			GINL GAIA::GVOID SetLink(GAIA::NETWORK::HttpServerLink* pLink)
			{
				GAIA::SYNC::Autolock al(m_lr);
				m_pLink = pLink;
			}
			GINL GAIA::NETWORK::HttpServerLink* GetLink() const{return m_pLink;}

			virtual GAIA::GVOID Create()
			{
				GAIA::NETWORK::AsyncSocket::Create();
				this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_NOBLOCK, GAIA::True);
				this->SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_REUSEADDR, GAIA::True);
			}

			virtual GAIA::N32 Send(const GAIA::GVOID* p, GAIA::N32 nSize)
			{
				GAIA::N32 nSent = GAIA::NETWORK::AsyncSocket::Send(p, nSize);
				m_needsendsize += nSent;
				return nSent;
			}

		protected:
			virtual GAIA::GVOID OnCreated(GAIA::BL bResult){}
			virtual GAIA::GVOID OnClosed(GAIA::BL bResult){}
			virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}
			virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}
			virtual GAIA::GVOID OnDisconnected(GAIA::BL bResult)
			{
				GAIA::SYNC::Autolock al(m_lr);
				if(m_pLink == GNIL)
					return;
				m_pSvr->RecycleLink(*m_pLink);
			}
			virtual GAIA::GVOID OnListened(GAIA::BL bResult){}
			virtual GAIA::GVOID OnAccepted(GAIA::BL bResult, const GAIA::NETWORK::Addr& addrListen){}
			virtual GAIA::GVOID OnSent(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nPracticeSize, GAIA::N32 nSize)
			{
				if(!bResult)
					return;
				GAIA::SYNC::Autolock al(m_lr);
				if(m_pLink == GNIL)
					return;
				GAIA::N64 uRemain = (m_needsendsize -= nPracticeSize);
				if(uRemain == 0 && m_bClosed)
					m_pSvr->RecycleLink(*m_pLink);
			}
			virtual GAIA::GVOID OnRecved(GAIA::BL bResult, const GAIA::GVOID* pData, GAIA::N32 nSize)
			{
				if(!bResult)
					return;

				// Try to analyze http information.
				GAIA::SYNC::Autolock al(m_lr);
				if(m_pLink == GNIL)
					return;
				if(m_pRecvBuf == GNIL)
				{
					m_pRecvBuf = m_pSvr->RequestBuffer();
					m_pRecvBuf->clear();
				}
				m_pRecvBuf->write(pData, nSize);
				if(m_url.Empty())
				{
					GAIA::NUM sFindBegin = m_pRecvBuf->write_size() - nSize;
					if(sFindBegin >= 2)
						sFindBegin -= 2;
					else if(sFindBegin >= 1)
						sFindBegin -= 1;
					GAIA::NUM sMinimizeFindBegin = 3/*Method*/ + 1/*Space*/ + 1/*Url*/ + 1/*Space*/ + 1/*HttpVersion*/;
					if(sFindBegin < sMinimizeFindBegin)
					{
						sFindBegin = sMinimizeFindBegin;
						if(sFindBegin >= m_pRecvBuf->write_size())
							sFindBegin = GINVALID;
					}
					if(sFindBegin != GINVALID)
					{
						GAIA::U8* pBegin = m_pRecvBuf->fptr() + sFindBegin;
						GAIA::NUM sSize = m_pRecvBuf->write_size() - sFindBegin;
						GAIA::NUM sValidSize = sSize - 3;
						GAIA::U8* pHeadEnd = GNIL;
						for(GAIA::NUM x = 0; x < sValidSize; ++x)
						{
							if(pBegin[x] == '\r' && pBegin[x + 1] == '\n' && (pBegin[x + 2] == '\r' || pBegin[x + 2] == '\n'))
							{
								pHeadEnd = pBegin + x + 3;
								break;
							}
						}
						if(pHeadEnd != GNIL)
						{
							GAST(m_pRecvBuf->read_size() == 0);

							// Analyze.
							GAIA::BL bAnalyzeSuccess = GAIA::False;
							do
							{
								// Analyze method.
								GAIA::NUM sHeadBeginPos = m_pRecvBuf->find("\r\n", 2, 0);
								sHeadBeginPos += 2;
								GAIA::NUM sMethodEndPos = m_pRecvBuf->find(" ", 1, 0);
								if(sMethodEndPos > sHeadBeginPos)
									break;
								GAIA::U8* pMethodBegin = m_pRecvBuf->fptr();
								for(GAIA::NUM x = 0; x < sizeofarray(GAIA::NETWORK::HTTP_METHOD_STRING); ++x)
								{
									if(GAIA::ALGO::gstrcmp(pMethodBegin, GAIA::NETWORK::HTTP_METHOD_STRING[x], sMethodEndPos) == 0)
									{
										m_method = (GAIA::NETWORK::HTTP_METHOD)x;
										break;
									}
								}
								if(m_method == GAIA::NETWORK::HTTP_METHOD_INVALID)
									break;

								// Analyze URL.
								GAIA::NUM sUrlBeginPos = sMethodEndPos + 1;
								GAIA::NUM sUrlEndPos = m_pRecvBuf->find(" ", 1, sUrlBeginPos);
								if(sUrlEndPos > sHeadBeginPos)
									break;
								GAIA::NUM sUrlLen = sUrlEndPos - sUrlBeginPos;
								if(sUrlLen <= 0)
									break;
								if(!m_url.FromString((const GAIA::CH*)m_pRecvBuf->fptr() + sUrlBeginPos, &sUrlLen))
									break;

								// Analyze http version.
								GAIA::NUM sVerBeginPos = sUrlEndPos + 1;
								GAIA::NUM sVerEndPos = sHeadBeginPos - 2;
								GAIA::NUM sVerLen = sVerEndPos - sVerBeginPos;
								if(sVerLen <= 0 || sVerLen > m_ver.capacity())
									break;
								m_ver.assign(m_pRecvBuf->fptr() + sVerBeginPos, sVerLen);

								// Analyze head.
								GAIA::NUM sHeadLen = (pHeadEnd - m_pRecvBuf->fptr()) - 1 - sHeadBeginPos;
								if(sHeadLen <= 2)
									break;
								if(!m_head.FromString((const GAIA::CH*)m_pRecvBuf->fptr() + sHeadBeginPos, &sHeadLen))
									break;

								//
								bAnalyzeSuccess = GAIA::True;
							}
							while(GAIA::ALWAYSFALSE);
							if(bAnalyzeSuccess)
							{
								// Remove http information from buffer.
								m_pRecvBuf->keep(m_pRecvBuf->bptr() - pHeadEnd);

								// Notify callback.
								m_pLink->rise_ref();
								GAIA::SYNC::AutolockW al(m_pSvr->m_rwRCLinks);
								m_pSvr->m_rclinks.push_back(m_pLink);
							}
							else
							{
								m_pSvr->GetStatus().uRequestAnalyzeFailedCount++;
							}
						}
					}
				}
			}
			virtual GAIA::GVOID OnShutdowned(GAIA::BL bResult, GAIA::N32 nShutdownFlag){}

		private:
			GINL GAIA::GVOID init()
			{
				m_pSvr = GNIL;
				m_pLink = GNIL;
				m_pRecvBuf = GNIL;
				m_pRecvBufSwap = GNIL;
				m_method = GAIA::NETWORK::HTTP_METHOD_INVALID;
				m_bClosed = GAIA::False;
			}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
			GAIA::NETWORK::HttpServerLink* m_pLink;
			GAIA::SYNC::Lock m_lr;
			GAIA::CTN::Buffer* m_pRecvBuf;
			GAIA::CTN::Buffer* m_pRecvBufSwap;
			GAIA::NETWORK::HTTP_METHOD m_method;
			GAIA::NETWORK::HttpURL m_url;
			GAIA::NETWORK::HttpHead m_head;
			GAIA::CTN::BasicChars<GAIA::CH, GAIA::NUM, 16> m_ver;
			GAIA::SYNC::Atomic m_needsendsize;
			GAIA::BL m_bClosed;
		};

		class HttpAsyncDispatcher : public GAIA::NETWORK::AsyncDispatcher
		{
		public:
			HttpAsyncDispatcher(GAIA::NETWORK::HttpServer& svr)
			{
				this->init();
				m_pSvr = &svr;
			}

			~HttpAsyncDispatcher()
			{
			}

		protected:
			virtual GAIA::NETWORK::AsyncSocket* OnCreateListenSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				HttpAsyncSocket* pListenSocket =
						gnew HttpAsyncSocket(*m_pSvr, *this, ASYNC_SOCKET_TYPE_LISTEN);
				return pListenSocket;
			}
			virtual GAIA::NETWORK::AsyncSocket* OnCreateAcceptingSocket(const GAIA::NETWORK::Addr& addrListen)
			{
				HttpAsyncSocket* pAcceptingSocket =
						gnew HttpAsyncSocket(*m_pSvr, *this, ASYNC_SOCKET_TYPE_ACCEPTING);
				return pAcceptingSocket;
			}
			virtual GAIA::BL OnAcceptSocket(GAIA::NETWORK::AsyncSocket& sock, const GAIA::NETWORK::Addr& addrListen)
			{
				GAIA::NETWORK::Addr addrPeer;
				GAIA::BL bGetPeerAddrResult = sock.GetPeerAddress(addrPeer);
				GAST(bGetPeerAddrResult && addrPeer.check());

				// If the IP address is in black list.
				if(m_pSvr->GetBlackWhiteMode() == GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE_BLACK)
				{
					if(m_pSvr->IsInBlackList(addrPeer.ip))
					{
						sock.drop_ref();
						m_pSvr->GetStatus().uRequestDenyByBWCount++;
						return GAIA::False;
					}
				}

				// If the IP address is in white list.
				else if(m_pSvr->GetBlackWhiteMode() == GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE_WHITE)
				{
					if(!m_pSvr->IsInWhiteList(addrPeer.ip))
					{
						sock.drop_ref();
						m_pSvr->GetStatus().uRequestDenyByBWCount++;
						return GAIA::False;
					}
				}

				// If connection count is too many, close it.
				if(m_pSvr->m_links_bypeeraddr.size() >= m_pSvr->GetDesc().sMaxConnCount)
				{
					sock.drop_ref();
					m_pSvr->GetStatus().uRequestDenyByMaxConnCount++;
					return GAIA::False;
				}

				//
				GAIA::NETWORK::HttpServerLink* pLink = gnew GAIA::NETWORK::HttpServerLink(*m_pSvr);
				pLink->m_pSock = (GAIA::NETWORK::HttpAsyncSocket*)&sock;
				pLink->SetPeerAddr(addrPeer);
				pLink->SetListenAddr(addrListen);
				pLink->SetAcceptTime(GAIA::TIME::gmt_time());
				((HttpAsyncSocket*)&sock)->SetLink(pLink);
				GAIA::SYNC::AutolockW al(m_pSvr->m_rwLinks);
				m_pSvr->m_links_bypeeraddr.insert(GAIA::CTN::Ref<GAIA::NETWORK::HttpServerLink>(pLink));
				return GAIA::True;
			}

		private:
			GINL GAIA::GVOID init(){m_pSvr = GNIL;}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
		};

		class HttpServerWorkThread : public GAIA::THREAD::Thread
		{
		public:
			GINL HttpServerWorkThread(GAIA::NETWORK::HttpServer& svr){this->init(); m_pSvr = &svr;}
			GINL ~HttpServerWorkThread(){}
			GAIA::GVOID SetStopCmd(GAIA::BL bStopCmd){m_bStopCmd = bStopCmd;}

		public:
			virtual GAIA::GVOID Run()
			{
				for(;;)
				{
					if(m_bStopCmd)
						break;
					GAIA::BL bExistExecutedTask = m_pSvr->Execute();
					if(!bExistExecutedTask)
						GAIA::SYNC::gsleep(1);
				}
			}

		private:
			GINL GAIA::GVOID init(){m_pSvr = GNIL; m_bStopCmd = GAIA::False;}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
			GAIA::BL m_bStopCmd;
		};

		HttpServerLink::HttpServerLink(GAIA::NETWORK::HttpServer& svr)
		{
			this->init();
			m_pSvr = &svr;
		}

		HttpServerLink::~HttpServerLink()
		{
			GAST(m_pSock != GNIL);
			if(m_pSock != GNIL)
			{
				if(m_pSock->IsCreated())
				{
					GAST(m_pSock->m_needsendsize == 0);
					m_pSock->Shutdown();
					m_pSock->Close();
				}
				m_pSock->drop_ref();
				m_pSock = GNIL;
			}
		}

		GAIA::BL HttpServerLink::Response(GAIA::NETWORK::HTTP_CODE httpcode, GAIA::NETWORK::HttpHead* pHttpHead, const GAIA::GVOID* p, GAIA::NUM sSize)
		{
			GPCHR_TRUE_RET(httpcode == GAIA::NETWORK::HTTP_CODE_INVALID, GAIA::False);
			GPCHR_NULL_RET(m_pSock, GAIA::False);

			GAIA::CTN::Buffer* pBuf = m_pSvr->RequestBuffer();
			pBuf->clear();

			// Write http version.
			const GAIA::NETWORK::HttpServerDesc& descSvr = m_pSvr->GetDesc();
			pBuf->write(descSvr.szHttpVer, descSvr.sHttpVerLen);
			pBuf->write(" ", 1);

			// Write http code.
			pBuf->write(GAIA::NETWORK::HTTP_CODE_STRING[httpcode],
						GAIA::NETWORK::HTTP_CODE_STRING_LENGTH[httpcode]);
			pBuf->write(" ", 1);

			// Write http code descript text.
			pBuf->write(GAIA::NETWORK::HTTP_CODE_DESCRIPTION[httpcode],
						GAIA::NETWORK::HTTP_CODE_DESCRIPTION_LENGTH[httpcode]);
			pBuf->write("\r\n", 2);

			// Write head.
			if(pHttpHead != GNIL && !pHttpHead->Empty())
			{
				GAIA::NUM sFirstLineSize = pBuf->write_size();
				pBuf->resize_keep(sFirstLineSize + pHttpHead->GetStringLength() + 1);
				GAIA::NUM sResultSize;
				GAIA::BL bSuccess;
				pHttpHead->ToString((GAIA::CH*)pBuf->fptr() + sFirstLineSize, pHttpHead->GetStringLength() + 1, &sResultSize, &bSuccess);
				GAST(sResultSize == pHttpHead->GetStringLength());
				GAST(bSuccess);
				pBuf->resize_keep(pBuf->write_size() - 1);
				GAST(pBuf->write_size() == sFirstLineSize + pHttpHead->GetStringLength());
			}
			pBuf->write("\n", 1);

			// Send.
			GAIA::NUM sSendedHeadSize = pBuf->write_size();
			m_pSock->Send(pBuf->fptr(), sSendedHeadSize);
			m_pSvr->ReleaseBuffer(pBuf);

			// Write body.
			if(p != GNIL)
			{
				GAST(sSize > 0);
				m_pSock->Send(p, sSize);
			}
			else
				GAST(sSize == 0);

			// Statistics.
			GAIA::NETWORK::HttpServerStatus& s = m_pSvr->GetStatus();
			if(m_sResponseTimes++ == 0)
			{
				s.uResponseCount++;
				s.uResponseSize += sSendedHeadSize;
				s.uResponseCountByMethod[m_pSock->m_method]++;
				s.uResponseSizeByMethod[m_pSock->m_method] += sSendedHeadSize;
			}
			s.uResponsePieceCount++;
			s.uResponseSize += sSize;
			s.uResponsePieceCountByMethod[m_pSock->m_method]++;
			s.uResponseSizeByMethod[m_pSock->m_method] += sSize;
			s.uResponseCountByCode[httpcode]++;

			return GAIA::True;
		}

		GAIA::BL HttpServerLink::Close()
		{
			m_pSock->m_bClosed = GAIA::True;
			return GAIA::True;
		}

		HttpServerCallBack::HttpServerCallBack(GAIA::NETWORK::HttpServer& svr)
		{
			this->init();
			m_pSvr = &svr;
		}

		HttpServerCallBack::~HttpServerCallBack()
		{
		}

		HttpServerCallBackForStaticResource::HttpServerCallBackForStaticResource(GAIA::NETWORK::HttpServer& svr)
			: HttpServerCallBack(svr)
		{
		}

		HttpServerCallBackForStaticResource::~HttpServerCallBackForStaticResource()
		{
		}

		BL HttpServerCallBackForStaticResource::OnRequest(GAIA::NETWORK::HttpServerLink& l, GAIA::NETWORK::HTTP_METHOD method, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize)
		{
			if(!url.IsPure())
				return GAIA::False;

			if(method != GAIA::NETWORK::HTTP_METHOD_GET &&
			   method != GAIA::NETWORK::HTTP_METHOD_HEAD)
				return GAIA::False;

			GAIA::NETWORK::HttpHead resphead;
			GAIA::CTN::AChars strResp;

			// Calculate file url.
			const GAIA::CH* pszURL = url.ToString();
			GAIA::CTN::TCharsString strFileURL = this->GetServer().GetDesc().pszRootPath;
			if(GAIA::ALGO::gstrcmpnil(pszURL, "/") == 0)
				strFileURL += "index.html";
			else
				strFileURL += pszURL;

			// Check file exist.
			GAIA::FSYS::Dir dir;
			if(!dir.ExistFile(strFileURL))
			{
				strResp = "GAIA HTTP SERVER CODE : ";
				strResp += GAIA::NETWORK::HTTP_CODE_STRING[GAIA::NETWORK::HTTP_CODE_NOTFOUND];
				strResp += " ";
				strResp += GAIA::NETWORK::HTTP_CODE_DESCRIPTION[GAIA::NETWORK::HTTP_CODE_NOTFOUND];
				strResp += " ";
				strResp += "[Not Exist This File]";

				GAIA::CH szContentLen[32];
				GAIA::ALGO::castv(strResp.size(), szContentLen, sizeof(szContentLen));
				resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, szContentLen);
				resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE, "text/plain; charset=UTF-8");

				l.Response(GAIA::NETWORK::HTTP_CODE_NOTFOUND, &resphead, strResp.fptr(), strResp.size());
				l.Close();
			}
			else if(method == GAIA::NETWORK::HTTP_METHOD_GET)
			{
				GAIA::FSYS::File f;
				if(f.Open(strFileURL, GAIA::FSYS::File::OPEN_TYPE_READ))
				{
					GAIA::N64 nFileSize = f.Size();
					if(nFileSize < 1024 * 128)
					{
						GAIA::CTN::Buffer buf;
						buf.resize(nFileSize);
						if(f.Read(buf.fptr(), buf.write_size()) == nFileSize)
						{
							GAIA::CH szContentLen[32];
							GAIA::ALGO::castv(nFileSize, szContentLen, sizeof(szContentLen));
							resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, szContentLen);
							resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE, "text/html; charset=UTF-8");

							l.Response(GAIA::NETWORK::HTTP_CODE_OK, &resphead, buf.fptr(), buf.write_size());
							l.Close();

							this->GetServer().UpdateCache(url, httphead, resphead, buf.fptr(), buf.write_size());
						}
						else
						{
							strResp = "GAIA HTTP SERVER CODE : ";
							strResp += GAIA::NETWORK::HTTP_CODE_STRING[GAIA::NETWORK::HTTP_CODE_SERVICEUNAVAILABLE];
							strResp += " ";
							strResp += GAIA::NETWORK::HTTP_CODE_DESCRIPTION[GAIA::NETWORK::HTTP_CODE_SERVICEUNAVAILABLE];
							strResp += " ";
							strResp += "[Read file failed]";

							GAIA::CH szContentLen[32];
							GAIA::ALGO::castv(strResp.size(), szContentLen, sizeof(szContentLen));
							resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, szContentLen);
							resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE, "text/plain; charset=UTF-8");

							l.Response(GAIA::NETWORK::HTTP_CODE_NOTFOUND, &resphead, strResp.fptr(), strResp.size());
							l.Close();
						}
					}
					else
					{
						strResp = "GAIA HTTP SERVER CODE : ";
						strResp += GAIA::NETWORK::HTTP_CODE_STRING[GAIA::NETWORK::HTTP_CODE_REQUESTENTITYTOOLARGE];
						strResp += " ";
						strResp += GAIA::NETWORK::HTTP_CODE_DESCRIPTION[GAIA::NETWORK::HTTP_CODE_REQUESTENTITYTOOLARGE];
						strResp += " ";
						strResp += "[File too large]";

						GAIA::CH szContentLen[32];
						GAIA::ALGO::castv(strResp.size(), szContentLen, sizeof(szContentLen));
						resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, szContentLen);
						resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE, "text/plain; charset=UTF-8");

						l.Response(GAIA::NETWORK::HTTP_CODE_NOTFOUND, &resphead, strResp.fptr(), strResp.size());
						l.Close();
					}
				}
				else
				{
					strResp = "GAIA HTTP SERVER CODE : ";
					strResp += GAIA::NETWORK::HTTP_CODE_STRING[GAIA::NETWORK::HTTP_CODE_NOTFOUND];
					strResp += " ";
					strResp += GAIA::NETWORK::HTTP_CODE_DESCRIPTION[GAIA::NETWORK::HTTP_CODE_NOTFOUND];
					strResp += " ";
					strResp += "[Open file failed]";

					GAIA::CH szContentLen[32];
					GAIA::ALGO::castv(strResp.size(), szContentLen, sizeof(szContentLen));
					resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, szContentLen);
					resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE, "text/plain; charset=UTF-8");

					l.Response(GAIA::NETWORK::HTTP_CODE_NOTFOUND, &resphead, strResp.fptr(), strResp.size());
					l.Close();
				}
			}

			return GAIA::True;
		}

		HttpServerCallBackForInfo::HttpServerCallBackForInfo(GAIA::NETWORK::HttpServer& svr)
			: HttpServerCallBack(svr)
		{
		}

		HttpServerCallBackForInfo::~HttpServerCallBackForInfo()
		{
		}

		GAIA::BL HttpServerCallBackForInfo::OnRequest(GAIA::NETWORK::HttpServerLink& l, GAIA::NETWORK::HTTP_METHOD method, const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize)
		{
			if(method != GAIA::NETWORK::HTTP_METHOD_GET)
				return GAIA::False;

			GAIA::NETWORK::HttpHead resphead;
			GAIA::CTN::AString strResp;
			strResp.reserve(1024 * 100);

			GAIA::CH szPath[32];
			url.GetPath(szPath, sizeof(szPath));
			if(GAIA::ALGO::gstrcmp(szPath, "/httpinfo") != 0)
				return GAIA::False;

			const GAIA::NETWORK::HttpServerStatus& s = this->GetServer().GetStatus();
			{
				strResp = "[GAIA HTTP SERVER STATUS]\n\n";

				strResp += "\tRequestAnalyzeFailedCount = "; strResp += s.uRequestAnalyzeFailedCount; strResp += "\n";
				strResp += "\tRequestDenyByBWCount = "; strResp += s.uRequestDenyByBWCount; strResp += "\n";
				strResp += "\tRequestDenyByMaxConnCount = "; strResp += s.uRequestDenyByMaxConnCount; strResp += "\n";
				strResp += "\tRequestDenyByMaxHalfConnCount = "; strResp += s.uRequestDenyByMaxHalfConnCount; strResp += "\n\n";

				strResp += "\tRequestCount = "; strResp += s.uRequestCount; strResp += "\n";
				for(GAIA::NUM x = 1; x < sizeofarray(s.uRequestCountByMethod); ++x)
				{
					strResp += "\t\t";
					strResp += GAIA::NETWORK::HTTP_METHOD_STRING[x];
					strResp += " = ";
					strResp += s.uRequestCountByMethod[x];
					strResp += "\n";
				}
				strResp += "\n";

				strResp += "\tResponseCount = "; strResp += s.uResponseCount; strResp += "\n";
				for(GAIA::NUM x = 1; x < sizeofarray(s.uResponseCountByMethod); ++x)
				{
					strResp += "\t\t";
					strResp += GAIA::NETWORK::HTTP_METHOD_STRING[x];
					strResp += " = ";
					strResp += s.uResponseCountByMethod[x];
					strResp += "\n";
				}
				strResp += "\n";

				strResp += "\tRequestPieceCount = "; strResp += s.uRequestPieceCount; strResp += "\n";
				for(GAIA::NUM x = 1; x < sizeofarray(s.uRequestPieceCountByMethod); ++x)
				{
					strResp += "\t\t";
					strResp += GAIA::NETWORK::HTTP_METHOD_STRING[x];
					strResp += " = ";
					strResp += s.uRequestPieceCountByMethod[x];
					strResp += "\n";
				}
				strResp += "\n";

				strResp += "\tResponsePieceCount = "; strResp += s.uResponsePieceCount; strResp += "\n";
				for(GAIA::NUM x = 1; x < sizeofarray(s.uResponsePieceCountByMethod); ++x)
				{
					strResp += "\t\t";
					strResp += GAIA::NETWORK::HTTP_METHOD_STRING[x];
					strResp += " = ";
					strResp += s.uResponsePieceCountByMethod[x];
					strResp += "\n";
				}
				strResp += "\n";

				strResp += "\tRequestSize = "; strResp += s.uRequestSize;
				strResp += ", Avg = "; strResp += s.uRequestSize / (s.uRequestCount == 0 ? 1 : s.uRequestCount);
				strResp += "\n";
				for(GAIA::NUM x = 1; x < sizeofarray(s.uRequestSizeByMethod); ++x)
				{
					strResp += "\t\t";
					strResp += GAIA::NETWORK::HTTP_METHOD_STRING[x];
					strResp += " = ";
					strResp += s.uRequestSizeByMethod[x];
					strResp += ", Avg = ";
					strResp += s.uRequestSizeByMethod[x] / (s.uRequestCountByMethod[x] == 0 ? 1 : s.uRequestCountByMethod[x]);
					strResp += "\n";
				}
				strResp += "\n";

				strResp += "\tResponseSize = "; strResp += s.uResponseSize;
				strResp += ", Avg = "; strResp += s.uResponseSize / (s.uResponseCount == 0 ? 1 : s.uResponseCount);
				strResp += "\n";
				for(GAIA::NUM x = 1; x < sizeofarray(s.uResponseSizeByMethod); ++x)
				{
					strResp += "\t\t";
					strResp += GAIA::NETWORK::HTTP_METHOD_STRING[x];
					strResp += " = ";
					strResp += s.uResponseSizeByMethod[x];
					strResp += ", Avg = ";
					strResp += s.uResponseSizeByMethod[x] / (s.uResponseCountByMethod[x] == 0 ? 1 : s.uResponseCountByMethod[x]);
					strResp += "\n";
				}
				strResp += "\n";

				for(GAIA::NUM x = 1; x < sizeofarray(s.uResponseCountByCode); ++x)
				{
					strResp += "\tCode ";
					strResp += GAIA::NETWORK::HTTP_CODE_STRING[x];
					strResp += " = ";
					strResp += s.uResponseCountByCode[x];
					strResp += "\n";
				}
				strResp += "\n";

				strResp += "\tHitResponseCacheCount = "; strResp += s.uHitResponseCacheCount; strResp += "\n";
				strResp += "\tHitResponseCacheSize = "; strResp += s.uHitResponseCacheSize; strResp += "\n";
				strResp += "\tNotHitResponseCacheCount = "; strResp += s.uNotHitResponseCacheCount; strResp += "\n";
				strResp += "\tNotResponseCount = "; strResp += s.uNotResponseCount; strResp += "\n";
				strResp += "\n";

				strResp += "[GAIA HTTP SERVER CALLBACK]\n\n";
				GAIA::CTN::Vector<GAIA::NETWORK::HttpServerCallBack*> listCallBack;
				this->GetServer().CollectCallBack(listCallBack);
				for(GAIA::NUM x = 0; x < listCallBack.size(); ++x)
				{
					GAIA::NETWORK::HttpServerCallBack* pCallBack = listCallBack[x];
					const GAIA::CH* pszCallBackName = pCallBack->GetName();
					strResp += "\t["; strResp += (x + 1); strResp += "] Name = "; strResp += pszCallBackName; strResp += "\n";
					pCallBack->drop_ref();
				}
				listCallBack.clear();
				strResp += "\n";

				strResp += "[GAIA HTTP SERVER WORK STATE]\n\n";
				strResp += "\tEnableDynamicResponseCache = "; strResp += this->GetServer().IsEnableDynamicResponseCache(); strResp += "\n";
				strResp += "\tEnableStaticResponseCache = "; strResp += this->GetServer().IsEnableStaticResponseCache(); strResp += "\n";
				strResp += "\tBlackWhiteMode = "; strResp += GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE_STRING[this->GetServer().GetBlackWhiteMode()]; strResp += "\n";
				strResp += "\n";

				strResp += "[GAIA HTTP SERVER OPEN ADDRESS]\n\n";
				GAIA::CTN::Vector<GAIA::NETWORK::Addr> listOpennedAddrs;
				this->GetServer().CollectOpennedAddr(listOpennedAddrs);
				for(GAIA::NUM x = 0; x < listOpennedAddrs.size(); ++x)
				{
					const GAIA::NETWORK::Addr& addr = listOpennedAddrs[x];
					GAIA::CH szTemp[64];
					addr.tostring(szTemp);
					strResp += "\t["; strResp += (x + 1); strResp += "] Address = "; strResp += szTemp; strResp += "\n";
				}
				strResp += "\n";

				const GAIA::NETWORK::HttpServerDesc& descSvr = this->GetServer().GetDesc();
				strResp += "[GAIA HTTP SERVER CREATE DESC]\n\n";
				strResp += "\tNetworkThreadCount = "; strResp += descSvr.sNetworkThreadCount; strResp += "\n";
				strResp += "\tWorkThreadCount = "; strResp += descSvr.sWorkThreadCount; strResp += "\n";
				strResp += "\tRootPath = "; strResp += descSvr.pszRootPath; strResp += "\n";
				strResp += "\tMaxConnCount = "; strResp += descSvr.sMaxConnCount; strResp += "\n";
				strResp += "\tMaxConnTime = "; strResp += descSvr.uMaxConnTime; strResp += "\n";
				strResp += "\tMaxHalfConnTime = "; strResp += descSvr.uMaxHarfConnTime; strResp += "\n";
				strResp += "\tMaxDynamicCacheSize = "; strResp += descSvr.uMaxDynamicCacheSize; strResp += "\n";
				strResp += "\tMaxDynamicCacheCount = "; strResp += descSvr.uMaxDynamicCacheCount; strResp += "\n";
				strResp += "\tMaxStaticCacheSize = "; strResp += descSvr.uMaxStaticCacheSize; strResp += "\n";
				strResp += "\tMaxStaticCacheCount = "; strResp += descSvr.uMaxStaticCacheCount; strResp += "\n";
				strResp += "\tMaxResponseCountPerMinute = "; strResp += descSvr.uMaxResponseCountPerMinute; strResp += "\n";
				strResp += "\tEnableAutoResponseStaticResource = "; strResp += descSvr.bEnableAutoResponseStaticResource; strResp += "\n";
				strResp += "\tHttpVersion = "; strResp += descSvr.sHttpVerLen; strResp += "\n";
				strResp += "\n";

				strResp += "[GAIA ASYNC NETWORK]\n\n";
				strResp += "\tUploadSpeed = "; strResp += "Not supported!"; strResp += "\n";
				strResp += "\tDownloadSpeed = "; strResp += "Not supported!"; strResp += "\n";
				strResp += "\tListenSocketCount = "; strResp += this->GetServer().GetAsyncDispatcher()->GetListenSocketCount(); strResp += "\n";
				strResp += "\tAcceptedSocketCount = "; strResp += this->GetServer().GetAsyncDispatcher()->GetAcceptedSocketCount(); strResp += "\n";
				strResp += "\tConnectedSocketCount = "; strResp += this->GetServer().GetAsyncDispatcher()->GetConnectedSocketCount(); strResp += "\n";
				strResp += "\n";

				strResp += "[GAIA INFO]\n\n";
				strResp += "\tGAIA Version = "; strResp += GAIA_VERSION_STRING; strResp += "\n";
				strResp += "\tGAIA Compile Time = "; strResp += GAIA_VERSION_COMPILEDATE; strResp += " "; strResp += GAIA_VERSION_COMPILETIME; strResp += "\n";
				strResp += "\tGAIA Last Modify Time = "; strResp += GAIA_VERSION_LASTMODIFYTIME; strResp += "\n";
				strResp += "\n";
			}

			GAIA::CH szContentLen[32];
			GAIA::ALGO::castv(strResp.size(), szContentLen, sizeof(szContentLen));
			resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTLENGTH, szContentLen);
			resphead.Set(GAIA::NETWORK::HTTP_HEADNAME_CONTENTTYPE, "text/plain; charset=UTF-8");

			l.Response(GAIA::NETWORK::HTTP_CODE_OK, &resphead, strResp.fptr(), strResp.size());
			l.Close();

			return GAIA::True;
		}

		HttpServer::HttpServer()
		{
			GAST(HTTP_METHOD_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_METHOD_STRING));
			GAST(HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_STRING));
			GAST(HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_LIST));
			GAST(HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_VALUE));
			GAST(HTTP_CODE_MAXENUMCOUNT == sizeofarray(GAIA::NETWORK::HTTP_CODE_DESCRIPTION));
			this->init();
		}

		HttpServer::~HttpServer()
		{
			if(this->IsCreated())
				this->Destroy();
		}

		GAIA::BL HttpServer::RegistCallBack(GAIA::NETWORK::HttpServerCallBack& cb)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::SYNC::AutolockW al(m_rwCBS);
			if(this->IsRegistedCallBack(cb))
				return GAIA::False;
			m_cbs.push_back(&cb);
			cb.rise_ref();
			return GAIA::True;
		}

		GAIA::BL HttpServer::UnregistCallBack(GAIA::NETWORK::HttpServerCallBack& cb)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::SYNC::AutolockW al(m_rwCBS);
			for(GAIA::NUM x = 0; x < m_cbs.size(); ++x)
			{
				if(m_cbs[x] == &cb)
				{
					m_cbs.erase(x);
					cb.drop_ref();
					return GAIA::True;
				}
			}
			return GAIA::False;
		}

		GAIA::BL HttpServer::UnregistCallBackAll()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::SYNC::AutolockW al(m_rwCBS);
			for(GAIA::NUM x = 0; x < m_cbs.size(); ++x)
			{
				GAIA::NETWORK::HttpServerCallBack* pCB = m_cbs[x];
				pCB->drop_ref();
			}
			m_cbs.clear();
		}

		GAIA::BL HttpServer::IsRegistedCallBack(GAIA::NETWORK::HttpServerCallBack& cb)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::SYNC::AutolockR al(m_rwCBS);
			for(GAIA::NUM x = 0; x < m_cbs.size(); ++x)
			{
				if(&cb == m_cbs[x])
					return GAIA::True;
			}
			return GAIA::False;
		}

		GAIA::BL HttpServer::CollectCallBack(GAIA::CTN::Vector<GAIA::NETWORK::HttpServerCallBack*>& listResult)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GAIA::BL bRet = GAIA::False;
			GAIA::SYNC::AutolockR al(m_rwCBS);
			for(GAIA::NUM x = 0; x < m_cbs.size(); ++x)
			{
				GAIA::NETWORK::HttpServerCallBack* pCallBack = m_cbs[x];
				pCallBack->rise_ref();
				listResult.push_back(pCallBack);
				bRet = GAIA::True;
			}
			return bRet;
		}

		GAIA::BL HttpServer::Create(const GAIA::NETWORK::HttpServerDesc& desc)
		{
			if(this->IsCreated())
				return GAIA::False;
			if(!desc.check())
				return GAIA::False;

			// Create async dispatcher.
			m_disp = gnew HttpAsyncDispatcher(*this);
			GAIA::NETWORK::AsyncDispatcherDesc descDisp;
			descDisp.reset();
			descDisp.sThreadCount = desc.sNetworkThreadCount;
			descDisp.sMaxConnectionCount = desc.sMaxConnCount;
			if(!m_disp->Create(descDisp))
			{
				gdel m_disp;
				m_disp = GNIL;
				return GAIA::False;
			}

			// Create work threads.
			GAST(m_listWorkThreads.empty());
			for(GAIA::NUM x = 0; x < desc.sWorkThreadCount; ++x)
			{
				HttpServerWorkThread* pThread = gnew HttpServerWorkThread(*this);
				m_listWorkThreads.push_back(pThread);
			}

			m_desc = desc;
			m_desc.pszRootPath = GAIA::ALGO::gstrnew(desc.pszRootPath);
			m_bCreated = GAIA::True;

			return GAIA::True;
		}

		GAIA::BL HttpServer::Destroy()
		{
			if(!this->IsCreated())
				return GAIA::False;

			//
			if(this->IsBegin())
				this->End();

			// Destroy work threads.
			for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
			{
				HttpServerWorkThread* pThread = m_listWorkThreads[x];
				GAST(pThread != GNIL);
				gdel pThread;
			}
			m_listWorkThreads.clear();

			// Destroy async dispatcher.
			m_disp->Destroy();
			gdel m_disp;
			m_disp = GNIL;

			//
			this->RemoveBlackListAll();
			this->RemoveWhiteListAll();

			// Destroy cache.
			{
				GAIA::SYNC::AutolockW al(m_rwCache);
				for(GAIA::CTN::Set<CacheNode>::it it = m_cache.frontit(); !it.empty(); )
				{
					CacheNode& cn = *it;
					if(cn.buf != GNIL)
						gdel cn.buf;
				}
			}

			// Destroy buffer pool.
			{
				GAIA::SYNC::Autolock al(m_lrBufPool);
				m_bufpool.destroy();
			}

			gdel[] m_desc.pszRootPath;
			m_desc.reset();
			m_bCreated = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL HttpServer::Begin()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			if(this->IsBegin())
				return GAIA::False;

			// Begin async dispatcher.
			m_disp->Begin();

			// Begin work thread.
			for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
			{
				HttpServerWorkThread* pThread = m_listWorkThreads[x];
				pThread->Start();
			}

			m_bBegin = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL HttpServer::End()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			if(!this->IsBegin())
				return GAIA::False;

			// Notify work thread end.
			for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
			{
				HttpServerWorkThread* pThread = m_listWorkThreads[x];
				pThread->SetStopCmd(GAIA::True);
				GAST(pThread != GNIL);
			}

			// End work thread.
			for(GAIA::NUM x = 0; x < m_listWorkThreads.size(); ++x)
			{
				HttpServerWorkThread* pThread = m_listWorkThreads[x];
				GAST(pThread != GNIL);
				pThread->Wait();
				pThread->SetStopCmd(GAIA::False);
			}

			// Release all links.
			{
				GAIA::SYNC::AutolockW al(m_rwLinks);
				for(__LinkSetType::it it = m_links_bypeeraddr.frontit(); !it.empty(); )
				{
					__LinkSetType::_datatype& t = *it;
					GAIA::NETWORK::HttpServerLink* pLink = t;
					GAST(pLink != GNIL);
					it.erase();
					pLink->drop_ref();
				}
				GAST(m_links_bypeeraddr.empty());
			}

			// Release all request completed links.
			{
				GAIA::SYNC::AutolockW al(m_rwRCLinks);
				while(!m_rclinks.empty())
				{
					GAIA::NETWORK::HttpServerLink* pLink = m_rclinks.front();
					m_rclinks.pop_front();
					GAST(pLink != GNIL);
					pLink->drop_ref();
				}
				GAST(m_rclinks.empty());
			}

			// End async dispatcher.
			m_disp->End();

			m_bBegin = GAIA::False;
			return GAIA::True;
		}

		GAIA::BL HttpServer::Execute()
		{
			GAIA::NETWORK::HttpServerLink* pLink;

			// Get a task.
			{
				GAIA::SYNC::AutolockW al(m_rwRCLinks);
				if(m_rclinks.empty())
					return GAIA::False;
				pLink = m_rclinks.front();
				m_rclinks.pop_front();
				GAST(pLink != GNIL);
			}

			// Execute the link.
			{
				HttpAsyncSocket* pSock = pLink->m_pSock;
				if(!pSock->m_bClosed)
				{
					// Swap.
					{
						GAIA::SYNC::Autolock al(pSock->m_lr);
						GAST(pSock->m_pRecvBufSwap == GNIL || pSock->m_pRecvBufSwap->empty());
						GAIA::ALGO::swap(pSock->m_pRecvBuf, pSock->m_pRecvBufSwap);
					}

					// Help.
					GAIA::GVOID* pData = GNIL;
					GAIA::NUM sDataSize = GINVALID;
					if(pSock->m_pRecvBufSwap != GNIL)
					{
						pData = pSock->m_pRecvBufSwap->fptr();
						sDataSize = pSock->m_pRecvBufSwap->write_size();
					}

					// Response by cache.
					GAIA::BL bResponsedByCache = GAIA::False;
					{
						if(pSock->m_method == GAIA::NETWORK::HTTP_METHOD_GET || pSock->m_method == GAIA::NETWORK::HTTP_METHOD_HEAD)
						{
							GAIA::GVOID* pCache;
							GAIA::NUM sCacheSize;
							GAIA::NETWORK::HttpHead resphead;
							if(this->RequestCache(pSock->m_url, pSock->m_head, resphead, &pCache, sCacheSize))
							{
								pLink->Response(GAIA::NETWORK::HTTP_CODE_OK, &resphead, pCache, sCacheSize);
								pLink->Close();
								this->ReleaseCache(pSock->m_url, pSock->m_head);
								bResponsedByCache = GAIA::True;

								m_status.uHitResponseCacheCount++;
								m_status.uHitResponseCacheSize += resphead.GetStringLength() + sCacheSize;
							}
							else
								m_status.uNotHitResponseCacheCount++;
						}
					}

					// Callback.
					if(!bResponsedByCache)
					{
						GAIA::SYNC::AutolockR al(m_rwCBS);
						GAIA::BL bResponsed = GAIA::False;
						for(GAIA::NUM x = 0; x < m_cbs.size(); ++x)
						{
							GAIA::NETWORK::HttpServerCallBack* cb = m_cbs[x];
							if(cb->OnRequest(*pLink, pSock->m_method, pSock->m_url, pSock->m_head, pData, sDataSize))
							{
								if(pLink->GetRequestTimes() == 0)
								{
									m_status.uRequestCount++;
									m_status.uRequestSize += pSock->m_url.Size();
									m_status.uRequestSize += pSock->m_head.GetStringLength();
									m_status.uRequestCountByMethod[pSock->m_method]++;
									m_status.uRequestSizeByMethod[pSock->m_method] += pSock->m_url.Size();
									m_status.uRequestSizeByMethod[pSock->m_method] += pSock->m_head.GetStringLength();
								}
								m_status.uRequestPieceCount++;
								m_status.uRequestSize += sDataSize;
								m_status.uRequestPieceCountByMethod[pSock->m_method]++;
								pLink->SetRequestTimes(pLink->GetRequestTimes() + 1);
								bResponsed = GAIA::True;
								break;
							}
						}
						if(!bResponsed)
						{
							m_status.uNotResponseCount++;
						}
					}

					// Clear buffer.
					pSock->m_pRecvBufSwap->clear();
				}

				// Try to recycle link.
				if(pSock->m_bClosed)
				{
					if(pSock->m_needsendsize == 0)
						this->RecycleLink(*pLink);
				}
			}

			// Drop.
			pLink->drop_ref();

			return GAIA::True;
		}

		GAIA::BL HttpServer::OpenAddr(const GAIA::NETWORK::Addr& addr)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GPCHR_FALSE_RET(addr.check(), GAIA::False);

			if(!m_disp->AddListenSocket(addr))
				return GAIA::False;

			return GAIA::True;
		}

		GAIA::BL HttpServer::CloseAddr(const GAIA::NETWORK::Addr& addr)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			GPCHR_FALSE_RET(addr.check(), GAIA::False);

			if(!m_disp->RemoveListenSocket(addr))
				return GAIA::False;

			return GAIA::True;
		}

		GAIA::BL HttpServer::CloseAddrAll()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);

			if(!m_disp->RemoveListenSocketAll())
				return GAIA::False;

			return GAIA::True;
		}

		GAIA::BL HttpServer::IsOpennedAddr(const GAIA::NETWORK::Addr& addr) const
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);

			if(!m_disp->IsExistListenSocket(addr))
				return GAIA::False;

			return GAIA::True;
		}

		GAIA::NUM HttpServer::GetOpennedAddrCount() const
		{
			GPCHR_FALSE_RET(this->IsCreated(), 0);
			return m_disp->GetListenSocketCount();
		}

		GAIA::BL HttpServer::CollectOpennedAddr(GAIA::CTN::Vector<GAIA::NETWORK::Addr>& listResult)
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			return m_disp->CollectListenSocket(listResult);
		}

		GAIA::GVOID HttpServer::EnableDynamicResponseCache(GAIA::BL bEnable)
		{
			m_bEnableDynamicResponseCache = bEnable;
		}

		GAIA::BL HttpServer::IsEnableDynamicResponseCache() const
		{
			return m_bEnableDynamicResponseCache;
		}

		GAIA::GVOID HttpServer::EnableStaticResponseCache(GAIA::BL bEnable)
		{
			m_bEnableStaticResponseCache = bEnable;
		}

		GAIA::BL HttpServer::IsEnableStaticResponseCache() const
		{
			return m_bEnableStaticResponseCache;
		}

		GAIA::GVOID HttpServer::SetBlackWhiteMode(GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE mode)
		{
			m_blackwhitemode = mode;
		}

		GAIA::NETWORK::HTTP_SERVER_BLACKWHITE_MODE HttpServer::GetBlackWhiteMode() const
		{
			return m_blackwhitemode;
		}

		GAIA::GVOID HttpServer::AddBlackList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uTime)
		{
			GPCHR_FALSE(ip.check());
			GAIA::SYNC::AutolockW al(m_rwBlackList);
			BWNode n;
			n.ip = ip;
			n.uRegistTime = GAIA::TIME::gmt_time();
			n.uEffectTime = uTime;
			m_BlackList.insert(n);
		}

		GAIA::GVOID HttpServer::RemoveBlackList(const GAIA::NETWORK::IP& ip)
		{
			GPCHR_FALSE(ip.check());
			GAIA::SYNC::AutolockW al(m_rwBlackList);
			BWNode n;
			n.ip = ip;
			m_BlackList.erase(n);
		}

		GAIA::GVOID HttpServer::RemoveBlackListAll()
		{
			GAIA::SYNC::AutolockW al(m_rwBlackList);
			m_BlackList.destroy();
		}

		GAIA::BL HttpServer::IsInBlackList(const GAIA::NETWORK::IP& ip) const
		{
			GPCHR_FALSE_RET(ip.check(), GAIA::False);
			GAIA::SYNC::AutolockR al(GCCAST(HttpServer*)(this)->m_rwBlackList);
			BWNode n;
			n.ip = ip;
			if(m_BlackList.find(n) == GNIL)
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::GVOID HttpServer::AddWhiteList(const GAIA::NETWORK::IP& ip, const GAIA::U64& uTime)
		{
			GPCHR_FALSE(ip.check());
			GAIA::SYNC::AutolockW al(m_rwWhiteList);
			BWNode n;
			n.ip = ip;
			n.uRegistTime = GAIA::TIME::gmt_time();
			n.uEffectTime = uTime;
			m_WhiteList.insert(n);
		}

		GAIA::GVOID HttpServer::RemoveWhiteList(const GAIA::NETWORK::IP& ip)
		{
			GPCHR_FALSE(ip.check());
			GAIA::SYNC::AutolockW al(m_rwWhiteList);
			BWNode n;
			n.ip = ip;
			m_WhiteList.erase(n);
		}

		GAIA::GVOID HttpServer::RemoveWhiteListAll()
		{
			GAIA::SYNC::AutolockW al(m_rwWhiteList);
			m_WhiteList.destroy();
		}

		GAIA::BL HttpServer::IsInWhiteList(const GAIA::NETWORK::IP& ip) const
		{
			GPCHR_FALSE_RET(ip.check(), GAIA::False);
			GAIA::SYNC::AutolockR al(GCCAST(HttpServer*)(this)->m_rwWhiteList);
			BWNode n;
			n.ip = ip;
			if(m_WhiteList.find(n) == GNIL)
				return GAIA::False;
			return GAIA::True;
		}

		GAIA::BL HttpServer::RequestCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen, GAIA::NETWORK::HttpHead& resphead, GAIA::GVOID** p, GAIA::NUM& sSize)
		{
			GPCHR_NULLSTR_RET(pszKey, GAIA::False);
			GPCHR_NULL_RET(p, GAIA::False);
			m_rwCache.EnterRead();
			if(sKeyLen == GINVALID)
				sKeyLen = GAIA::ALGO::gstrlen(pszKey);
			CacheNode n;
			n.strKey.proxy(pszKey, sKeyLen, 0);
			CacheNode* pFinded = m_cache.find(n);
			n.strKey.proxy(GNIL, 0, 0);
			if(pFinded == GNIL)
			{
				m_rwCache.LeaveRead();
				return GAIA::False;
			}
			if(pFinded->uEffectTime != GINVALID && pFinded->uRegistTime + pFinded->uEffectTime < GAIA::TIME::gmt_time())
			{
				m_rwCache.LeaveRead();
				return GAIA::False;
			}
			pFinded->nRefCount++;
			resphead = pFinded->resphead;
			if(pFinded->buf != GNIL)
			{
				*p = (GAIA::GVOID*)pFinded->buf->fptr();
				sSize = pFinded->buf->write_size();
			}
			else
			{
				*p = GNIL;
				sSize = 0;
			}
			return GAIA::True;
		}

		GAIA::BL HttpServer::RequestCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead, GAIA::NETWORK::HttpHead& resphead, GAIA::GVOID** p, GAIA::NUM& sSize)
		{
			GAST(!url.Empty());
			GAIA::NETWORK::HttpHead prachead = reqhead;
			prachead.Optimize();
			GAIA::NUM sUrlLength;
			const GAIA::CH* pszUrl = url.ToString(&sUrlLength);
			if(!prachead.Empty())
			{
				GAIA::NUM sHeadLength = prachead.GetStringLength();
				GAIA::CTN::ACharsString strTemp;
				strTemp.resize(sUrlLength + sHeadLength + 1);
				GAIA::ALGO::gmemcpy(strTemp.fptr(), pszUrl, sUrlLength);
				GAIA::NUM sHeadResultSize;
				GAIA::BL bHeadResult;
				prachead.ToString(strTemp.fptr() + sUrlLength, sHeadLength + 1, &sHeadResultSize, &bHeadResult);
				GAST(sHeadResultSize == sHeadLength);
				GAST(bHeadResult);
				strTemp.resize(strTemp.size() - 1);
				return this->RequestCache(strTemp.fptr(), strTemp.size(), resphead, p, sSize);
			}
			else
				return this->RequestCache(pszUrl, sUrlLength, resphead, p, sSize);
		}

		GAIA::BL HttpServer::ReleaseCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen)
		{
			GPCHR_NULLSTR_RET(pszKey, GAIA::False);
			if(sKeyLen == GINVALID)
				sKeyLen = GAIA::ALGO::gstrlen(pszKey);
			CacheNode n;
			n.strKey.proxy(pszKey, sKeyLen, 0);
			CacheNode* pFinded = m_cache.find(n);
			n.strKey.proxy(GNIL, 0, 0);
			if(pFinded == GNIL)
				return GAIA::False;
			pFinded->nRefCount--;
			GAST(pFinded->nRefCount >= 0);
			m_rwCache.LeaveRead();
			return GAIA::True;
		}

		GAIA::BL HttpServer::ReleaseCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead)
		{
			GAST(!url.Empty());
			GAIA::NETWORK::HttpHead prachead = reqhead;
			prachead.Optimize();
			GAIA::NUM sUrlLength;
			const GAIA::CH* pszUrl = url.ToString(&sUrlLength);
			if(!prachead.Empty())
			{
				GAIA::NUM sHeadLength = prachead.GetStringLength();
				GAIA::CTN::ACharsString strTemp;
				strTemp.resize(sUrlLength + sHeadLength + 1);
				GAIA::ALGO::gmemcpy(strTemp.fptr(), pszUrl, sUrlLength);
				GAIA::NUM sHeadResultSize;
				GAIA::BL bHeadResult;
				prachead.ToString(strTemp.fptr() + sUrlLength, sHeadLength + 1, &sHeadResultSize, &bHeadResult);
				GAST(sHeadResultSize == sHeadLength);
				GAST(bHeadResult);
				strTemp.resize(strTemp.size() - 1);
				return this->ReleaseCache(strTemp.fptr(), strTemp.size());
			}
			else
				return this->ReleaseCache(pszUrl, sUrlLength);
		}

		GAIA::BL HttpServer::UpdateCache(const GAIA::CH* pszKey, GAIA::NUM sKeyLen, const GAIA::NETWORK::HttpHead& resphead, const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::U64 uEffectTime)
		{
			GPCHR_NULLSTR_RET(pszKey, GAIA::False);
			if(sKeyLen == GINVALID)
				sKeyLen = GAIA::ALGO::gstrlen(pszKey);
			GAIA::SYNC::AutolockW al(m_rwCache);
			CacheNode n;
			n.strKey.proxy(pszKey, sKeyLen, 0);
			CacheNode* pFinded = m_cache.find(n);
			n.strKey.proxy(GNIL, 0, 0);
			if(pFinded == GNIL)
			{
				n.strKey.assign(pszKey, sKeyLen);
				n.resphead = resphead;
				if(p != GNIL)
				{
					GAST(sSize > 0);
					n.buf = gnew GAIA::CTN::Buffer;
					n.buf->assign(p, sSize);
				}
				else
				{
					GAST(sSize == 0);
					n.buf = GNIL;
				}
				n.uRegistTime = GAIA::TIME::gmt_time();
				n.uEffectTime = uEffectTime;
				n.nRefCount = 0;
				m_cache.insert(n);
			}
			else
			{
				pFinded->uRegistTime = GAIA::TIME::gmt_time();
				pFinded->uEffectTime = uEffectTime;
				pFinded->resphead = resphead;
				pFinded->buf->assign(p, sSize);
			}
			return GAIA::True;
		}

		GAIA::BL HttpServer::UpdateCache(const GAIA::NETWORK::HttpURL& url, const GAIA::NETWORK::HttpHead& reqhead, const GAIA::NETWORK::HttpHead& resphead, const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::U64 uEffectTime)
		{
			GAST(!url.Empty());
			GAIA::NETWORK::HttpHead prachead = reqhead;
			prachead.Optimize();
			GAIA::NUM sUrlLength;
			const GAIA::CH* pszUrl = url.ToString(&sUrlLength);
			if(!prachead.Empty())
			{
				GAIA::NUM sHeadLength = prachead.GetStringLength();
				GAIA::CTN::ACharsString strTemp;
				strTemp.resize(sUrlLength + sHeadLength + 1);
				GAIA::ALGO::gmemcpy(strTemp.fptr(), pszUrl, sUrlLength);
				GAIA::NUM sHeadResultSize;
				GAIA::BL bHeadResult;
				prachead.ToString(strTemp.fptr() + sUrlLength, sHeadLength + 1, &sHeadResultSize, &bHeadResult);
				GAST(sHeadResultSize == sHeadLength);
				GAST(bHeadResult);
				strTemp.resize(strTemp.size() - 1);
				return this->UpdateCache(strTemp.fptr(), strTemp.size(), resphead, p, sSize, uEffectTime);
			}
			else
				return this->UpdateCache(pszUrl, sUrlLength, resphead, p, sSize, uEffectTime);
		}

		GAIA::BL HttpServer::RecycleCache()
		{
			GAIA::U64 uCurrentTime = GAIA::TIME::gmt_time();
			GAIA::SYNC::AutolockW al(m_rwCache);
			if(m_cache.empty())
				return GAIA::False;
			for(GAIA::CTN::Set<CacheNode>::it it = m_cache.frontit(); !it.empty(); )
			{
				CacheNode& cn = *it;
				if(cn.nRefCount == 0 && cn.uEffectTime != GINVALID && cn.uRegistTime + cn.uEffectTime < uCurrentTime)
				{
					if(cn.buf != GNIL)
						gdel cn.buf;
					it.erase();
				}
				else
					++it;
			}
			return GAIA::True;
		}

		GAIA::BL HttpServer::RecycleLink(GAIA::NETWORK::HttpServerLink& l)
		{
			// Remove from link list.
			{
				GAIA::SYNC::AutolockW al(m_rwLinks);
				if(!m_links_bypeeraddr.erase(GAIA::CTN::Ref<GAIA::NETWORK::HttpServerLink>(&l)))
					return GAIA::False;
			}

			// Drop.
			l.drop_ref();

			return GAIA::True;
		}

		GAIA::CTN::Buffer* HttpServer::RequestBuffer()
		{
			GAIA::SYNC::Autolock al(m_lrBufPool);
			return m_bufpool.alloc();
		}

		GAIA::GVOID HttpServer::ReleaseBuffer(GAIA::CTN::Buffer* pBuf)
		{
			GAIA::SYNC::Autolock al(m_lrBufPool);
			m_bufpool.release(pBuf);
		}
	}
}