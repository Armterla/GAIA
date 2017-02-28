#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_httpserver.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		class HttpAsyncSocket : public GAIA::NETWORK::AsyncSocket
		{
		public:
			HttpAsyncSocket(HttpServer& svr, GAIA::NETWORK::AsyncDispatcher& disp, GAIA::NETWORK::ASYNC_SOCKET_TYPE socktype = GAIA::NETWORK::ASYNC_SOCKET_TYPE_CONNECTED)
				: GAIA::NETWORK::AsyncSocket(disp, socktype)
			{
				this->init();
				m_pSvr = &svr;
				m_method = GAIA::NETWORK::HTTP_METHOD_INVALID;
			}

			virtual ~HttpAsyncSocket()
			{
			}

			GINL GAIA::GVOID SetLink(GAIA::NETWORK::HttpServerLink* pLink){m_pLink = pLink;}
			GINL GAIA::NETWORK::HttpServerLink* GetLink() const{return m_pLink;}

		protected:
			virtual GAIA::GVOID OnCreated(GAIA::BL bResult){}
			virtual GAIA::GVOID OnClosed(GAIA::BL bResult){}
			virtual GAIA::GVOID OnBound(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}
			virtual GAIA::GVOID OnConnected(GAIA::BL bResult, const GAIA::NETWORK::Addr& addr){}
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
				GAST(m_pLink != GNIL);
				if(bResult)
					return;

				// Callback HttpServerLink::OnRecv.
				{
					GAIA::SYNC::AutolockR al(m_pSvr->m_rwCBS);
					for(GAIA::NUM x = 0 ; x < m_pSvr->m_cbs.size(); ++x)
					{
						GAIA::NETWORK::HttpServerCallBack* pCallBack = m_pSvr->m_cbs[x];
						GAST(pCallBack != GNIL);
						if(pCallBack->OnRecv(*m_pLink, pData, nSize))
							break;
					}
				}

				// Try to analyze http information.
				GAIA::SYNC::Autolock al(m_lr);
				m_recvbuf.write(pData, nSize);
				if(m_url.Empty())
				{
					GAIA::NUM sFindBegin = m_recvbuf.write_size() - nSize;
					if(sFindBegin >= 2)
						sFindBegin -= 2;
					else if(sFindBegin >= 1)
						sFindBegin -= 1;
					GAIA::NUM sMinimizeFindBegin = 3/*Method*/ + 1/*Space*/ + 1/*Url*/ + 1/*Space*/ + 1/*HttpVersion*/;
					if(sFindBegin < sMinimizeFindBegin)
					{
						sFindBegin = sMinimizeFindBegin;
						if(sFindBegin >= m_recvbuf.write_size())
							sFindBegin = GINVALID;
					}
					if(sFindBegin != GINVALID)
					{
						GAIA::U8* pBegin = m_recvbuf.fptr() + sFindBegin;
						GAIA::NUM sSize = m_recvbuf.write_size() - sFindBegin;
						GAIA::NUM sValidSize = sSize - 3;
						GAIA::U8* pHeadEnd = GNIL;
						for(GAIA::NUM x = 0; x < sValidSize; ++x)
						{
							if(pBegin[x] == '\r' && pBegin[x + 1] == '\n' && (pBegin[x + 2] == '\r' || pBegin[x + 2] == '\n'))
							{
								pHeadEnd = pBegin + 3;
								break;
							}
						}
						if(pHeadEnd != GNIL)
						{
							GAST(m_recvbuf.read_size() == 0);

							// Analyze.
							GAIA::BL bAnalyzeSuccess = GAIA::False;
							do
							{
								// Analyze method.
								GAIA::NUM sHeadBeginPos = m_recvbuf.find("\r\n", 2, 0);
								sHeadBeginPos += 2;
								GAIA::NUM sMethodEndPos = m_recvbuf.find(" ", 1, 0);
								if(sMethodEndPos > sHeadBeginPos)
									break;
								GAIA::U8* pMethodBegin = m_recvbuf.fptr();
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
								GAIA::NUM sUrlEndPos = m_recvbuf.find(" ", 1, sUrlBeginPos);
								if(sUrlEndPos > sHeadBeginPos)
									break;
								GAIA::NUM sUrlLen = sUrlEndPos - sUrlBeginPos;
								if(sUrlLen <= 0)
									break;
								if(!m_url.FromString((const GAIA::CH*)m_recvbuf.fptr() + sUrlBeginPos, &sUrlLen))
									break;

								// Analyze http version.
								GAIA::NUM sVerBeginPos = sUrlEndPos + 1;
								GAIA::NUM sVerEndPos = m_recvbuf.find(" ", 1, sVerBeginPos);
								if(sVerEndPos > sHeadBeginPos)
									break;
								GAIA::NUM sVerLen = sVerEndPos - sVerBeginPos;
								if(sVerLen <= 0 || sVerLen > m_ver.capacity())
									break;
								m_ver.assign(m_recvbuf.fptr() + sVerBeginPos, sVerLen);

								// Analyze head.
								GAIA::NUM sHeadLen = (pHeadEnd - m_recvbuf.fptr()) - 1 - sHeadBeginPos;
								if(sHeadLen <= 2)
									break;
								if(!m_head.FromString((const GAIA::CH*)m_recvbuf.fptr() + sHeadBeginPos, &sHeadLen))
									break;

								//
								bAnalyzeSuccess = GAIA::True;
							}
							while(GAIA::ALWAYSFALSE);
							if(bAnalyzeSuccess)
							{
								// Remove http information from buffer.
								m_recvbuf.keep(m_recvbuf.bptr() - pHeadEnd);

								// Notify callback.
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
			GINL GAIA::GVOID init(){m_pSvr = GNIL; m_pLink = GNIL;}

		private:
			GAIA::NETWORK::HttpServer* m_pSvr;
			GAIA::NETWORK::HttpServerLink* m_pLink;
			GAIA::SYNC::Lock m_lr;
			GAIA::CTN::Buffer m_recvbuf;
			GAIA::NETWORK::HTTP_METHOD m_method;
			GAIA::NETWORK::HttpURL m_url;
			GAIA::NETWORK::HttpHead m_head;
			GAIA::CTN::BasicChars<GAIA::CH, GAIA::NUM, 16> m_ver;
		};

		class HttpAsyncDispatcher : public GAIA::NETWORK::AsyncDispatcher
		{
		public:
			HttpAsyncDispatcher()
			{
				this->init();
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
				pLink->SetAsyncSocket(*(GAIA::NETWORK::HttpAsyncSocket*)&sock);
				pLink->SetPeerAddr(addrPeer);
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

		HttpServerLink::HttpServerLink(GAIA::NETWORK::HttpServer& svr)
		{
			this->init();
			m_pSvr = &svr;
		}

		HttpServerLink::~HttpServerLink()
		{
			if(m_pSock != GNIL)
				this->Close();
		}

		GAIA::BL HttpServerLink::Response(GAIA::NETWORK::HTTP_CODE httpcode, const GAIA::NETWORK::HttpHead& httphead, const GAIA::GVOID* p, GAIA::NUM sSize, const GAIA::U64& uCacheTime)
		{
			GPCHR_TRUE_RET(httpcode == GAIA::NETWORK::HTTP_CODE_INVALID, GAIA::False);
			GPCHR_TRUE_RET(httphead.Empty(), GAIA::False);
			GPCHR_NULL_RET(m_pSock, GAIA::False);

			GAIA::CTN::Buffer* pBuf = m_pSvr->RequestBuffer();
			pBuf->clear();

			// Write http version.
			const GAIA::NETWORK::HttpServerDesc& descSvr = m_pSvr->GetDesc();
			pBuf->write(descSvr.szHttpVer, descSvr.sHttpVerLen);
			pBuf->write(" ");

			// Write http code.
			pBuf->write(GAIA::NETWORK::HTTP_CODE_STRING[httpcode],
						sizeof(GAIA::NETWORK::HTTP_CODE_STRING[httpcode]) - sizeof(GAIA::CH));
			pBuf->write(" ");

			// Write http code descript text.
			pBuf->write(GAIA::NETWORK::HTTP_CODE_DESCRIPTION[httpcode],
						sizeof(GAIA::NETWORK::HTTP_CODE_DESCRIPTION[httpcode]) - sizeof(GAIA::CH));
			pBuf->write(" ");

			// Write head.
			if(!httphead.Empty())
			{

			}
			pBuf->write("\n", 1);
			m_pSvr->ReleaseBuffer(pBuf);

			// Send.
			m_pSock->Send(pBuf->fptr(), pBuf->write_size());

			// Write body.
			if(p != GNIL)
			{
				GAST(sSize > 0);
				m_pSock->Send(p, sSize);
			}
			else
				GAST(sSize == 0);

			// Cache for late use.
			if(uCacheTime == GINVALID)
			{

			}

			return GAIA::True;
		}

		GAIA::BL HttpServerLink::Close()
		{
			if(m_pSock == GNIL)
				return GAIA::False;
			m_pSock->drop_ref();
			m_pSock = GNIL;
			return GAIA::True;
		}

		HttpServerCallBack::HttpServerCallBack(GAIA::NETWORK::HttpServer& svr)
		{
			this->init();
		}

		HttpServerCallBack::~HttpServerCallBack()
		{
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

		GAIA::BL HttpServer::Create(const GAIA::NETWORK::HttpServerDesc& desc)
		{
			if(this->IsCreated())
				return GAIA::False;
			if(!desc.check())
				return GAIA::False;

			// Create async dispatcher.
			m_disp = gnew HttpAsyncDispatcher;
			GAIA::NETWORK::AsyncDispatcherDesc descDisp;
			descDisp.reset();
			descDisp.sThreadCount = desc.sThreadCount;
			descDisp.sMaxConnectionCount = desc.sMaxConnCount;
			if(!m_disp->Create(descDisp))
			{
				gdel m_disp;
				m_disp = GNIL;
				return GAIA::False;
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
				if(m_cache.empty())
					return GAIA::False;
				for(GAIA::CTN::Set<CacheNode>::it it = m_cache.frontit(); !it.empty(); )
				{
					CacheNode& cn = *it;
					gdel cn.buf;
				}
				m_cache.destroy();
				return GAIA::True;
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

			m_bBegin = GAIA::True;
			return GAIA::True;
		}

		GAIA::BL HttpServer::End()
		{
			GPCHR_FALSE_RET(this->IsCreated(), GAIA::False);
			if(!this->IsBegin())
				return GAIA::False;

			// Release all links.
			{
				GAIA::SYNC::AutolockW al(m_rwLinks);
				for(__LinkSetType::it it = m_links_bypeeraddr.frontit(); !it.empty(); )
				{
					__LinkSetType::_datatype& t = *it;
					GAIA::NETWORK::HttpServerLink* pLink = t;
					GAST(pLink != GNIL);
					it.erase();
					gdel pLink;
				}
			}

			// Release all request completed links.
			{
				GAIA::SYNC::AutolockW al(m_rwRCLinks);
				m_rclinks.clear();
			}

			// End async dispatcher.
			m_disp->End();

			m_bBegin = GAIA::False;
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

		const GAIA::NETWORK::Addr* HttpServer::GetOpennedAddr(GAIA::NUM sIndex) const
		{
			GPCHR_FALSE_RET(this->IsCreated(), GNIL);
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

		GAIA::BL HttpServer::RequestCache(const GAIA::CH* pszUrlAndHead, GAIA::NUM sUrlAndHeadLen, GAIA::GVOID** p, GAIA::NUM& sSize)
		{
			GPCHR_NULLSTR_RET(pszUrlAndHead, GAIA::False);
			GPCHR_NULL_RET(p, GAIA::False);
			m_rwCache.EnterRead();
			if(sUrlAndHeadLen == GINVALID)
				sUrlAndHeadLen = GAIA::ALGO::gstrlen(pszUrlAndHead);
			CacheNode n;
			n.strUrlAndHead.proxy(pszUrlAndHead, sUrlAndHeadLen, 0);
			CacheNode* pFinded = m_cache.find(n);
			n.strUrlAndHead.proxy(GNIL, 0, 0);
			if(pFinded == GNIL)
			{
				m_rwCache.LeaveRead();
				return GAIA::False;
			}
			pFinded->nRefCount++;
			*p = (GAIA::GVOID*)pFinded->buf->fptr();
			sSize = pFinded->buf->write_size();
			return GAIA::True;
		}

		GAIA::BL HttpServer::ReleaseCache(const GAIA::CH* pszUrlAndHead, GAIA::NUM sUrlAndHeadLen)
		{
			GPCHR_NULLSTR_RET(pszUrlAndHead, GAIA::False);
			if(sUrlAndHeadLen == GINVALID)
				sUrlAndHeadLen = GAIA::ALGO::gstrlen(pszUrlAndHead);
			CacheNode n;
			n.strUrlAndHead.proxy(pszUrlAndHead, sUrlAndHeadLen, 0);
			CacheNode* pFinded = m_cache.find(n);
			n.strUrlAndHead.proxy(GNIL, 0, 0);
			if(pFinded == GNIL)
				return GAIA::False;
			pFinded->nRefCount--;
			if(pFinded->nRefCount == 0)
				m_cache.erase(*pFinded);
			m_rwCache.LeaveRead();
			return GAIA::True;
		}

		GAIA::BL HttpServer::UpdateCache(const GAIA::CH* pszUrlAndHead, GAIA::NUM sUrlAndHeadLen, const GAIA::GVOID* p, GAIA::NUM sSize, GAIA::U64 uEffectTime)
		{
			GPCHR_NULLSTR_RET(pszUrlAndHead, GAIA::False);
			if(sUrlAndHeadLen == GINVALID)
				sUrlAndHeadLen = GAIA::ALGO::gstrlen(pszUrlAndHead);
			GAIA::SYNC::AutolockW al(m_rwCache);
			CacheNode n;
			n.strUrlAndHead.proxy(pszUrlAndHead, sUrlAndHeadLen, 0);
			CacheNode* pFinded = m_cache.find(n);
			n.strUrlAndHead.proxy(GNIL, 0, 0);
			if(pFinded == GNIL)
			{
				n.strUrlAndHead.assign(pszUrlAndHead, sUrlAndHeadLen);
				n.buf->assign(p, sSize);
				n.uRegistTime = GAIA::TIME::gmt_time();
				n.uEffectTime = uEffectTime;
				n.nRefCount = 0;
			}
			else
				pFinded->buf->assign(p, sSize);
			return GAIA::True;
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
				if(cn.nRefCount == 0 && cn.uRegistTime + cn.uEffectTime < uCurrentTime)
				{
					gdel cn.buf;
					it.erase();
				}
				else
					++it;
			}
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