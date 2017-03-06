/*
	[Lock sequence]

	========================================================================================================================================
	Flow\Lock		m_lrExecute		m_rwSendCache	m_rwSent	m_rwRecv	m_lrLinkPool	m_lrNodePool	m_lrBufferPool	m_rwDisconnected

	SendTo

	RecvFrom													|-------------------------->|
																|------------------------------------------>|

	Execute			|-------------->|
					|------------------------------>|---------------------------------------------------------------------->|
					|------------------------------------------>|
					|------------------------------------------------------>|
					|---------------------------------------------------------------------->|
					|-------------------------------------------------------------------------------------->|
					|------------------------------------------>|---------->|
					|------------------------------------------>|-------------------------->|
					|------------------------------------------>|------------------------------------------>|
					|------------------------------>|---------------------->|
					|------------------------------>|-------------------------------------->|
					|------------------------------>|------------------------------------------------------>|
					|------------------------------------------------------------------------------------------------------>|

	ResetConnection	|-------------->|------------------------------------------------------>|
					|-------------->|---------------------------------------------------------------------->|
					|------------------------------>|-------------------------------------->|
					|------------------------------>|------------------------------------------------------>|
					|------------------------------------------>|---------->|
					|------------------------------------------>|-------------------------->|
					|------------------------------------------>|------------------------------------------>|

	========================================================================================================================================
	Flow\Lock		m_lrExecuteTimeout	m_lrExecuteSend		m_lrExecuteRecv		m_lrExecuteResend	m_lrExecuteRerecv

	Full-Execute	|------------------>|------------------>|------------------>|------------------>|
*/

#include <gaia_type.h>
#include <gaia_assert.h>
#include <gaia_network_sudpsocket.h>
#include <gaia_assert_impl.h>
#include <gaia_thread_base_impl.h>
#include <gaia_network_base_impl.h>
#include <gaia_network_socket_impl.h>

namespace GAIA
{
	namespace NETWORK
	{
		SUDPSocket::SUDPSocket()
		{
			this->init();
		}
		SUDPSocket::~SUDPSocket()
		{

		}
		GAIA::GVOID SUDPSocket::Create()
		{
			m_recvbuf.resize(GAIA::NETWORK::GAIA_NETWORK_MTU);
			m_sock.Create(GAIA::NETWORK::Socket::SOCKET_TYPE_DATAGRAM);
		}
		GAIA::GVOID SUDPSocket::Close()
		{
			m_sock.Close();
		}
		GAIA::GVOID SUDPSocket::Shutdown(GAIA::N32 nShutdownFlag)
		{
			m_sock.Shutdown(nShutdownFlag);
		}
		GAIA::BL SUDPSocket::IsCreated() const
		{
			return m_sock.IsCreated();
		}
		GAIA::GVOID SUDPSocket::Bind(const GAIA::NETWORK::Addr& addr)
		{
			m_sock.Bind(addr);
		}
		GAIA::BL SUDPSocket::IsBinded() const
		{
			return m_sock.IsBinded();
		}
		GAIA::GVOID SUDPSocket::SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, const GAIA::CTN::Vari& v)
		{
			m_sock.SetOption(op, v);
		}
		GAIA::GVOID SUDPSocket::GetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, GAIA::CTN::Vari& v)
		{
			m_sock.GetOption(op, v);
		}
		GAIA::GVOID SUDPSocket::SetTimeout(const GAIA::U64& uTimeout)
		{
			if(uTimeout == 0)
				m_uTimeout = DEFAULT_TIMEOUT_TIME;
			else
				m_uTimeout = uTimeout;
		}
		const GAIA::U64& SUDPSocket::GetTimeout() const
		{
			return m_uTimeout;
		}
		GAIA::GVOID SUDPSocket::SetResendTime(const GAIA::U64& uResendTime)
		{
			if(uResendTime == 0)
				m_uResendTime = DEFAULT_RESEND_TIME;
			else
				m_uResendTime = uResendTime;
		}
		const GAIA::U64& SUDPSocket::GetResendTime() const
		{
			return m_uResendTime;
		}
		GAIA::GVOID SUDPSocket::SetRerecvTime(const GAIA::U64& uRerecvTime)
		{
			if(uRerecvTime == 0)
				m_uRerecvTime = DEFAULT_RERECV_TIME;
			else
				m_uRerecvTime = uRerecvTime;
		}
		const GAIA::U64& SUDPSocket::GetRerecvTime() const
		{
			return m_uRerecvTime;
		}
		GAIA::GVOID SUDPSocket::SetRecycleTime(const GAIA::U64& uRecycleTime)
		{
			if(uRecycleTime == 0)
				m_uRecycleTime = DEFAULT_RECYCLE_TIME;
			else
				m_uRecycleTime = uRecycleTime;
		}
		const GAIA::U64& SUDPSocket::GetRecycleTime() const
		{
			return m_uRecycleTime;
		}
		GAIA::N32 SUDPSocket::SendTo(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize)
		{
			if(!addr.check())
				GTHROW(InvalidParam);
			if(p == GNIL)
				GTHROW(InvalidParam);
			if(nSize <= 0)
				GTHROW(InvalidParam);
			if(!this->IsCreated())
				GTHROW(Illegal);

			// Allocate node.
			Node* pNode;
			{
				GAIA::SYNC::Autolock al(m_lrNodePool);
				pNode = m_nodepool.alloc();
			}

			// Allocate buffer.
			{
				GAIA::SYNC::Autolock al(m_lrBufferPool);
				pNode->pBuf = m_bufpool.alloc();
			}

			// Copy data to node.
			pNode->uFirstTime = m_uLastExecuteTime;
			pNode->uLastTime = m_uLastExecuteTime;
			pNode->addr = addr;
			pNode->pBuf->assign(p, nSize);
			GAIA::N64 lSerial;
			GAIA::BL bSerial = this->OnSend(addr, p, nSize, lSerial);
			if(bSerial)
				pNode->lSerial = lSerial;
			else
				pNode->lSerial = GINVALID;

			// Push to send cache.
			{
				GAIA::SYNC::Autolock al(m_lrSendCache);
				m_sendcache.push_back(pNode);
			}

			return nSize;
		}
		GAIA::N32 SUDPSocket::RecvFrom(GAIA::NETWORK::Addr& addr, GAIA::GVOID* p, GAIA::N32 nSize)
		{
			if(p == GNIL)
				GTHROW(InvalidParam);
			if(nSize <= 0)
				GTHROW(InvalidParam);
			if(!this->IsCreated())
				GTHROW(Illegal);

			// Check empty.
			GAIA::SYNC::Autolock al(m_lrRecv);
			if(m_recvablelinks_byaddr.empty())
				return 0;

			// Get link.
			__LinkSetType::it itlink;
			if(addr.check())
			{
				Link linkfinder;
				linkfinder.addr = addr;
				__LinkSetType::it itfinded = m_recvablelinks_byaddr.lower_equal(GAIA::CTN::Ref<Link>(&linkfinder));
				if(itfinded.empty())
					return 0;
				GAIA::CTN::Ref<Link> findedlinkref = *itfinded;
				Link* pFindedLink = findedlinkref;
				GAST(pFindedLink != GNIL);
				if(*pFindedLink != linkfinder)
					return 0;
				itlink = itfinded;
			}
			else
				itlink = m_recvablelinks_byaddr.frontit();

			// Get node.
			GAIA::CTN::Ref<Link>& reflink = *itlink;
			Link* pLink = reflink;
			GAST(pLink != GNIL);
			__NodeSetType::it itnode = pLink->nodes.frontit();
			GAST(!itnode.empty());
			GAIA::CTN::Ref<Node>& refnode = *itnode;
			Node* pNode = refnode;
			GAST(pNode != GNIL);

			// Check serial.
			GAST(pNode->lSerial == GINVALID || pNode->lSerial == pLink->lNextSerial);

			// Fill result.
			GAST(pNode->pBuf != GNIL);
			if(nSize < pNode->pBuf->write_size())
				return 0;
			GAIA::CTN::Buffer& buf = *pNode->pBuf;
			GAIA::ALGO::gmemcpy(p, buf.fptr(), buf.write_size());
			if(pNode->lSerial != GINVALID)
				pLink->lNextSerial++;
			addr = pNode->addr;

			// Clear and recycle.
			{
				// Remove node from link's recv nodes container.
				itnode.erase();

				// Recycle buffer.
				{
					GAIA::SYNC::Autolock al(m_lrBufferPool);
					m_bufpool.release(pNode->pBuf);
				}

				// Recycle node.
				{
					GAIA::SYNC::Autolock al(m_lrNodePool);
					m_nodepool.release(pNode);
				}
			}

			//
			if(itnode.empty())
				itlink.erase();
			else
			{
				GAIA::CTN::Ref<Node>& refnodenext = *itnode;
				Node* pNodeNext = refnodenext;
				GAST(pNodeNext != GNIL);
				if(pNodeNext->lSerial != GINVALID && pNodeNext->lSerial != pLink->lNextSerial)
					itlink.erase();
			}

			return buf.write_size();
		}
		GAIA::N32 SUDPSocket::GetFD() const
		{
			return m_sock.GetFD();
		}
		GAIA::BL SUDPSocket::GetGlobalAddress(GAIA::NETWORK::Addr& addr)
		{
			return m_sock.GetGlobalAddress(addr);
		}
		GAIA::BL SUDPSocket::GetLocalAddress(GAIA::NETWORK::Addr& addr)
		{
			return m_sock.GetLocalAddress(addr);
		}
		GAIA::BL SUDPSocket::Execute(GAIA::U64 uDeltaTime, GAIA::BL bSend, GAIA::BL bRecv, GAIA::BL bResend, GAIA::BL bRerecv, GAIA::BL bTimeout)
		{
			if(!this->IsCreated())
				GTHROW(Illegal);

			//
			GAIA::BL bRet = GAIA::False;

			//
			m_uLastExecuteTime += uDeltaTime;
			GAIA::U64 uCurrentTime = m_uLastExecuteTime;

			// Resend dispatch.
			if(bResend)
			{
				//
				GAIA::SYNC::Autolock alexec(m_lrExecuteResend);

				// Resent.
				GAIA::SYNC::Autolock al(m_lrSent);
				for(__LinkSetType::it it = m_sentlinks_byaddr.frontit(); !it.empty(); ++it)
				{
					Link* pLink = *it;
					GAST(pLink != GNIL);
					if(pLink->nodes.empty())
						continue;

					GAIA::NUM sCurrentLinkResentCount = 0;
					GAIA::BL bExistFailed = GAIA::False;
					for(__NodeSetType::it itt = pLink->nodes.frontit(); !itt.empty(); ++itt)
					{
						Node* pNode = *itt;
						GAST(pNode != GNIL);

						if(uCurrentTime - pNode->uLastTime < m_uResendTime)
							break;
						if(uCurrentTime - pNode->uFirstTime >= m_uTimeout)
						{
							// Push to disconnected link list.
							{
								GAIA::SYNC::Autolock al(m_lrDisconnected);
								m_listDisconnected.insert(pNode->addr);
								break;
							}
						}
						else
						{
							GAIA::N32 nSent = m_sock.SendTo(pNode->addr, pNode->pBuf->fptr(), pNode->pBuf->write_size());
							if(nSent == pNode->pBuf->write_size())
							{
								pNode->uLastTime = uCurrentTime;
								bRet = GAIA::True;
								m_state.uResendSerialMsgCount++;
								m_state.uResendSerialMsgSize += pNode->pBuf->write_size();
								sCurrentLinkResentCount++;
								if(sCurrentLinkResentCount > 1000)
									break;
							}
							else
							{
								bExistFailed = GAIA::True;
								break;
							}
						}
					}
					if(bExistFailed)
						break;
				}
			}

			// Send dispatch.
			if(bSend)
			{
				//
				GAIA::SYNC::Autolock alexec(m_lrExecuteSend);

				// Collect.
				{
					GAIA::SYNC::Autolock al(m_lrSendCache);
					m_listTempNodeForExecuteSend0 = m_sendcache;
					m_sendcache.clear();
				}

				// Send.
				m_listTempNodeForExecuteSend1.clear();
				for(__NodeVectorType::it it = m_listTempNodeForExecuteSend0.frontit(); !it.empty(); ++it)
				{
					Node* pNode = *it;
					GAST(pNode != GNIL);

					GAIA::N32 nSent = m_sock.SendTo(pNode->addr, pNode->pBuf->fptr(), pNode->pBuf->write_size());
					if(nSent == pNode->pBuf->write_size())
					{
						if(pNode->lSerial == GINVALID)
						{
							m_state.uSendUnserialMsgCount++;
							m_state.uSendUnserialMsgSize += pNode->pBuf->write_size();
						}
						else
						{
							m_state.uSendSerialMsgCount++;
							m_state.uSendSerialMsgSize += pNode->pBuf->write_size();
						}

						pNode->uFirstTime = pNode->uLastTime = uCurrentTime;
						m_listTempNodeForExecuteSend1.push_back(pNode);
						*it = GNIL;
						bRet = GAIA::True;
					}
					else
						break;
				}

				// Push sent failed datagram back to the send cache.
				{
					GAIA::SYNC::Autolock al(m_lrSendCache);
					m_listTempNodeForSend0 = m_sendcache;
					m_sendcache.clear();
					for(__NodeVectorType::it it = m_listTempNodeForExecuteSend0.frontit(); !it.empty(); ++it)
					{
						Node* pNode = *it;
						if(pNode != GNIL)
							m_sendcache.push_back(pNode);
					}
					m_sendcache += m_listTempNodeForSend0;
				}

				// Recycle no-serial datagram.
				{
					// Recycle buffer.
					{
						GAIA::SYNC::Autolock al(m_lrBufferPool);
						for(__NodeVectorType::it it = m_listTempNodeForExecuteSend1.frontit(); !it.empty(); ++it)
						{
							Node* pNode = *it;
							GAST(pNode != GNIL);
							GAST(pNode->pBuf != GNIL);
							if(pNode->lSerial == GINVALID)
								m_bufpool.release(pNode->pBuf);
						}
					}

					// Recycle node.
					{
						GAIA::SYNC::Autolock al(m_lrNodePool);
						for(__NodeVectorType::it it = m_listTempNodeForExecuteSend1.frontit(); !it.empty(); ++it)
						{
							Node* pNode = *it;
							GAST(pNode != GNIL);
							if(pNode->lSerial == GINVALID)
							{
								m_nodepool.release(pNode);
								*it = GNIL;
							}
						}
					}
				}

				// Push serial datagram to sent container for later resend operation.
				{
					GAIA::SYNC::Autolock al(m_lrSent);
					for(__NodeVectorType::it it = m_listTempNodeForExecuteSend1.frontit(); !it.empty(); ++it)
					{
						Node* pNode = *it;
						if(pNode != GNIL)
						{
							GAST(pNode->lSerial != GINVALID);

							Link linkfinder;
							linkfinder.addr = pNode->addr;
							GAIA::CTN::Ref<Link> linkreffinder = &linkfinder;
							GAIA::CTN::Ref<Link>* pFindedLinkRef = m_sentlinks_byaddr.find(linkreffinder);
							if(pFindedLinkRef == GNIL)
							{
								GAIA::SYNC::Autolock al(m_lrLinkPool);
								Link* pNewLink = m_linkpool.alloc();
								pNewLink->uLastTime = uCurrentTime;
								pNewLink->addr = pNode->addr;
								pNewLink->nodes.clear();
								pNewLink->lNextSerial = 0;
								pNewLink->lNeedBackSerialBegin = GINVALID;
								pNewLink->lNeedBackSerialFlags = 0;
								linkreffinder = pNewLink;
								m_sentlinks_byaddr.insert(linkreffinder);
								pFindedLinkRef = &linkreffinder;
							}
							GAST(pFindedLinkRef != GNIL);
							Link* pFindedLink = *pFindedLinkRef;
							GAST(pFindedLink != GNIL);

							pFindedLink->nodes.insert(GAIA::CTN::Ref<Node>(pNode));
						}
					}
				}
			}

			// Receive dispatch.
			if(bRecv)
			{
				//
				GAIA::SYNC::Autolock alexec(m_lrExecuteRecv);

				//
				GAIA::NETWORK::Addr addrRecvFrom;
				for(;;)
				{
					addrRecvFrom.reset();
					GAIA::N32 nRecvSize = m_sock.RecvFrom(addrRecvFrom, m_recvbuf.fptr(), m_recvbuf.write_size());
					if(nRecvSize <= 0)
						break;
					else if(nRecvSize < m_recvbuf.write_size()) // Must below to MTU, because MTU size is a unknown size, maybe exist more data in this datagram.
					{
						GAIA::N64 lSerialBegin, lSerialEnd;
						GAIA::BL bOnRecvBack = this->OnRecvBack(addrRecvFrom, m_recvbuf.fptr(), nRecvSize, lSerialBegin, lSerialEnd);
						if(bOnRecvBack)
						{
							m_state.uRecvBackMsgCount++;
							m_state.uRecvBackMsgSize += nRecvSize;

							GAST(lSerialBegin <= lSerialEnd);
							GAIA::SYNC::Autolock al(m_lrSent);
							Link linkfinder;
							linkfinder.addr = addrRecvFrom;
							GAIA::CTN::Ref<Link>* pFindedLinkRef = m_sentlinks_byaddr.find(GAIA::CTN::Ref<Link>(&linkfinder));
							if(pFindedLinkRef != GNIL)
							{
								Link* pFindedLink = *pFindedLinkRef;

								Node nodefinder;
								nodefinder.addr = addrRecvFrom;
								nodefinder.lSerial = lSerialBegin;
								__NodeSetType::it itnodefront = pFindedLink->nodes.upper_equal(GAIA::CTN::Ref<Node>(&nodefinder));
								nodefinder.lSerial = lSerialEnd;
								__NodeSetType::it itnodeback = pFindedLink->nodes.lower_equal(GAIA::CTN::Ref<Node>(&nodefinder));
								if(!itnodefront.empty() && !itnodeback.empty())
								{
									GAIA::BL bBoundMatch = GAIA::True;
									{
										GAIA::CTN::Ref<Node>& refnodefront = *itnodefront;
										GAIA::CTN::Ref<Node>& refnodeback = *itnodeback;
										Node* pNodeFront = refnodefront;
										Node* pNodeBack = refnodeback;
										GAST(pNodeFront != GNIL);
										GAST(pNodeBack != GNIL);
										if((*pNodeFront) > (*pNodeBack))
											bBoundMatch = GAIA::False;
									}
									if(bBoundMatch)
									{
										while(!itnodefront.empty())
										{
											GAIA::CTN::Ref<Node>& refnode = *itnodefront;
											Node* pNode = refnode;
											GAST(pNode != GNIL);
											GAST(pNode->addr == addrRecvFrom && pNode->lSerial >= lSerialBegin && pNode->lSerial <= lSerialEnd);

											GAIA::BL bEnded;
											if(itnodefront == itnodeback)
												bEnded = GAIA::True;
											else
												bEnded = GAIA::False;
											itnodefront.erase();

											// Recycle buffer.
											{
												GAIA::SYNC::Autolock al(m_lrBufferPool);
												m_bufpool.release(pNode->pBuf);
											}

											// Recycle node.
											{
												GAIA::SYNC::Autolock al(m_lrNodePool);
												m_nodepool.release(pNode);
											}

											bRet = GAIA::True;
											if(bEnded)
												break;
										}
									}
								}
							}
						}
						else
						{
							//
							GAIA::N64 lSerial;
							GAIA::BL bOnRecv = this->OnRecv(addrRecvFrom, m_recvbuf.fptr(), nRecvSize, lSerial);
							GAIA::SYNC::Autolock al(m_lrRecv);
							if(bOnRecv)
							{
								m_state.uRecvSerialMsgCount++;
								m_state.uRecvSerialMsgSize += nRecvSize;
							}
							else
							{
								m_state.uRecvUnserialMsgCount++;
								m_state.uRecvUnserialMsgSize += nRecvSize;
							}

							// If link not exist, create link.
							Link linkfinder;
							linkfinder.addr = addrRecvFrom;
							GAIA::CTN::Ref<Link> linkreffinder = &linkfinder;
							GAIA::CTN::Ref<Link>* pFindedLinkRef = m_recvlinks_byaddr.find(linkreffinder);
							if(pFindedLinkRef == GNIL)
							{
								GAIA::SYNC::Autolock al(m_lrLinkPool);
								Link* pNewLink = m_linkpool.alloc();
								pNewLink->uLastTime = uCurrentTime;
								pNewLink->addr = addrRecvFrom;
								pNewLink->nodes.clear();
								pNewLink->lNextSerial = 0;
								pNewLink->lNeedBackSerialBegin = GINVALID;
								pNewLink->lNeedBackSerialFlags = 0;
								linkreffinder = pNewLink;
								m_recvlinks_byaddr.insert(linkreffinder);
								pFindedLinkRef = &linkreffinder;
							}
							GAST(pFindedLinkRef != GNIL);
							Link* pFindedLink = *pFindedLinkRef;
							GAST(pFindedLink != GNIL);

							//
							if(bOnRecv)
							{
								if(pFindedLink->lNeedBackSerialBegin == GINVALID)
								{
									pFindedLink->lNeedBackSerialBegin = lSerial;
									pFindedLink->lNeedBackSerialFlags = pFindedLink->lNeedBackSerialFlags | (GAIA::U64)(((GAIA::U64)1) << 0);
								}
								else
								{
									GAIA::N64 lSerialOffset = lSerial - pFindedLink->lNeedBackSerialBegin;
									if(lSerialOffset >= 0 && lSerialOffset < sizeof(pFindedLink->lNeedBackSerialFlags) * 8)
										pFindedLink->lNeedBackSerialFlags = pFindedLink->lNeedBackSerialFlags | (GAIA::U64)(((GAIA::U64)1) << lSerialOffset);
									else // Ignored because the moduler-user not call execute with bRerecv = GAIA::True at the same time.
									{
									}
								}
							}

							// Calculate bInsertAble.
							GAIA::BL bInsertAble;
							if(bOnRecv)
							{
								if(pFindedLink->lNextSerial <= lSerial)
								{
									Node nodefinder;
									nodefinder.addr = addrRecvFrom;
									nodefinder.lSerial = lSerial;
									if(pFindedLink->nodes.find(GAIA::CTN::Ref<Node>(&nodefinder)) == GNIL)
										bInsertAble = GAIA::True;
									else
										bInsertAble = GAIA::False;
								}
								else
									bInsertAble = GAIA::False;
							}
							else
								bInsertAble = GAIA::True;

							// Insert.
							if(bInsertAble)
							{
								// Alloc node and fill it.
								Node* pNode;
								{
									GAIA::SYNC::Autolock al(m_lrNodePool);
									pNode = m_nodepool.alloc();
									pNode->uFirstTime = pNode->uLastTime = uCurrentTime;
									if(bOnRecv)
										pNode->lSerial = lSerial;
									else
										pNode->lSerial = GINVALID;
									pNode->addr = addrRecvFrom;
									pNode->pBuf = GNIL;
								}

								// Alloc buffer and fill it.
								{
									GAIA::SYNC::Autolock al(m_lrBufferPool);
									pNode->pBuf = m_bufpool.alloc();
									pNode->pBuf->assign(m_recvbuf.fptr(), nRecvSize);
								}

								// Push node to link.
								pFindedLink->uLastTime = uCurrentTime;
								pFindedLink->nodes.insert(GAIA::CTN::Ref<Node>(pNode));

								// Ready for user RecvFrom.
								if(pNode->lSerial == GINVALID || pNode->lSerial == pFindedLink->lNextSerial)
									m_recvablelinks_byaddr.insert(GAIA::CTN::Ref<Link>(pFindedLink));

								//
								bRet = GAIA::True;
							}
						}
					}
					else // Ignored invalid datagram here. YOU CAN ADD SOME LOG OR OTHER CODE HERE FOR DEBUG.
					{
					}
				}
			}

			//
			if(bRerecv)
			{
				//
				GAIA::SYNC::Autolock alexec(m_lrExecuteRerecv);

				//
				GAIA::SYNC::Autolock al(m_lrRecv);
				for(__LinkSetType::it it = m_recvlinks_byaddr.frontit(); !it.empty(); ++it)
				{
					GAIA::CTN::Ref<Link> reflink = *it;
					Link* pLink = reflink;
					GAST(pLink != GNIL);
					if(uCurrentTime - pLink->uLastTime >= m_uRerecvTime)
					{
						GAIA::N64 lLastSerialOffset = GINVALID;
						if(pLink->lNeedBackSerialBegin != GINVALID)
						{
							GAIA::BL bSendBackSucces = GAIA::True;
							for(GAIA::NUM x = 0; x < sizeof(pLink->lNeedBackSerialFlags) * 8; ++x)
							{
								if(pLink->lNeedBackSerialFlags & (GAIA::U64)(((GAIA::U64)1) << x))
								{
									if(lLastSerialOffset == GINVALID)
										lLastSerialOffset = x;
								}
								else
								{
									if(lLastSerialOffset != GINVALID)
									{
										GAIA::U8 buf[GAIA::NETWORK::GAIA_NETWORK_MTU];
										GAIA::N32 nResultSize;
										GAIA::N64 lSerialBegin = pLink->lNeedBackSerialBegin + lLastSerialOffset;
										GAIA::N64 lSerialEnd = pLink->lNeedBackSerialBegin + x - 1;
										GAST(lSerialBegin <= lSerialEnd);
										if(this->OnBack(pLink->addr, lSerialBegin, lSerialEnd, buf, sizeof(buf), nResultSize))
										{
											GAST(nResultSize > 0);
											if(m_sock.SendTo(pLink->addr, buf, nResultSize) != nResultSize)
												bSendBackSucces = GAIA::False;
											else
											{
												m_state.uSendBackMsgCount++;
												m_state.uSendBackMsgSize += nResultSize;
											}
										}
										else
											bSendBackSucces = GAIA::False;
										lLastSerialOffset = GINVALID;
									}
								}
							}
							if(lLastSerialOffset != GINVALID)
							{
								GAIA::U8 buf[GAIA::NETWORK::GAIA_NETWORK_MTU];
								GAIA::N32 nResultSize;
								GAIA::N64 lSerialBegin = pLink->lNeedBackSerialBegin + lLastSerialOffset;
								GAIA::N64 lSerialEnd = pLink->lNeedBackSerialBegin + sizeof(pLink->lNeedBackSerialFlags) * 8 - 1;
								GAST(lSerialBegin <= lSerialEnd);
								if(this->OnBack(pLink->addr, lSerialBegin, lSerialEnd, buf, sizeof(buf), nResultSize))
								{
									GAST(nResultSize > 0);
									if(m_sock.SendTo(pLink->addr, buf, nResultSize) != nResultSize)
										bSendBackSucces = GAIA::False;
									else
									{
										m_state.uSendBackMsgCount++;
										m_state.uSendBackMsgSize += nResultSize;
									}
								}
								else
									bSendBackSucces = GAIA::False;
							}
							if(bSendBackSucces)
							{
								pLink->lNeedBackSerialBegin = GINVALID;
								pLink->lNeedBackSerialFlags = 0;
								bRet = GAIA::True;
							}
						}
					}
				}
			}

			//
			if(bTimeout)
			{
				//
				GAIA::SYNC::Autolock alexec(m_lrExecuteTimeout);

				// Recycle disconnect links.
				{
					m_listTempAddrForExecuteTimeout0.clear();

					// Collect.
					{
						GAIA::SYNC::Autolock al(m_lrDisconnected);
						for(__AddrSetType::it it = m_listDisconnected.frontit(); !it.empty(); ++it)
						{
							const GAIA::NETWORK::Addr& addr = *it;
							m_listTempAddrForExecuteTimeout0.push_back(addr);
						}
						m_listDisconnected.clear();
					}

					// Recycle.
					{
						for(__AddrVectorType::it it = m_listTempAddrForExecuteTimeout0.frontit(); !it.empty(); ++it)
						{
							const GAIA::NETWORK::Addr& addr = *it;
							this->OnTimeout(addr, GAIA::True);
							this->ResetConnection(&addr);
							bRet = GAIA::True;
						}
					}
				}

				// Recycle useless links.
				{
					m_listTempAddrForExecuteTimeout0.clear();

					// Collect.
					{
						GAIA::SYNC::Autolock al(m_lrRecv);
						for(__LinkSetType::it it = m_recvlinks_byaddr.frontit(); !it.empty(); ++it)
						{
							GAIA::CTN::Ref<Link> reflink = *it;
							Link* pLink = reflink;
							GAST(pLink != GNIL);
							if(uCurrentTime - pLink->uLastTime > m_uRecycleTime)
								m_listTempAddrForExecuteTimeout0.push_back(pLink->addr);
						}
					}

					// Recycle.
					{
						for(__AddrVectorType::it it = m_listTempAddrForExecuteTimeout0.frontit(); !it.empty(); ++it)
						{
							const GAIA::NETWORK::Addr& addr = *it;
							this->OnTimeout(addr, GAIA::False);
							this->ResetConnection(&addr);
							bRet = GAIA::True;
						}
					}
				}
			}

			return bRet;
		}
		GAIA::BL SUDPSocket::ResetConnection(const GAIA::NETWORK::Addr* pAddr, GAIA::BL bSend, GAIA::BL bRecv)
		{
			//
			if(pAddr != GNIL)
			{
				if(!pAddr->check())
					GTHROW(InvalidParam);
			}
			if(!bSend && !bRecv)
				GTHROW(InvalidParam);

			GAIA::SYNC::Autolock alexec1(m_lrExecuteTimeout);
			GAIA::SYNC::Autolock alexec2(m_lrExecuteSend);
			GAIA::SYNC::Autolock alexec3(m_lrExecuteRecv);
			GAIA::SYNC::Autolock alexec4(m_lrExecuteResend);
			GAIA::SYNC::Autolock alexec5(m_lrExecuteRerecv);

			//
			if(pAddr == GNIL)
			{
				// Reset send cache.
				if(bSend)
				{
					GAIA::SYNC::Autolock al(m_lrSendCache);
					for(__NodeVectorType::it it = m_sendcache.frontit(); !it.empty(); ++it)
					{
						Node* pNode = *it;
						GAST(pNode != GNIL);

						// Recycle buffer.
						{
							GAIA::SYNC::Autolock al(m_lrBufferPool);
							m_bufpool.release(pNode->pBuf);
						}

						// Recycle node.
						{
							GAIA::SYNC::Autolock al(m_lrNodePool);
							m_nodepool.release(pNode);
						}
					}
					m_sendcache.clear();
				}

				// Reset sent history.
				if(bSend)
				{
					GAIA::SYNC::Autolock al(m_lrSent);
					m_listTempLinkForSend1.clear();
					for(__LinkSetType::it it = m_sentlinks_byaddr.frontit(); !it.empty(); ++it)
					{
						Link* pLink = *it;
						GAST(pLink != GNIL);
						m_listTempLinkForSend1.push_back(pLink);

						m_listTempNodeForSend0.clear();
						for(__NodeSetType::it itt = pLink->nodes.frontit(); !itt.empty(); ++itt)
						{
							Node* pNode = *itt;
							GAST(pNode != GNIL);
							m_listTempNodeForSend0.push_back(pNode);
						}
						pLink->nodes.clear();
						for(__NodeVectorType::it itt = m_listTempNodeForSend0.frontit(); !itt.empty(); ++itt)
						{
							Node* pNode = *itt;

							// Recycle buffer.
							{
								GAIA::SYNC::Autolock al(m_lrBufferPool);
								m_bufpool.release(pNode->pBuf);
							}

							// Recycle node.
							{
								GAIA::SYNC::Autolock al(m_lrNodePool);
								m_nodepool.release(pNode);
							}
						}
					}
					m_sentlinks_byaddr.clear();
					for(__LinkVectorType::it it = m_listTempLinkForSend1.frontit(); !it.empty(); ++it)
					{
						Link* pLink = *it;

						// Recycle link.
						{
							GAIA::SYNC::Autolock al(m_lrLinkPool);
							m_linkpool.release(pLink);
						}
					}
				}

				// Reset recv context.
				if(bRecv)
				{
					GAIA::SYNC::Autolock al(m_lrRecv);
					m_listTempLinkForRecv1.clear();
					for(__LinkSetType::it it = m_recvlinks_byaddr.frontit(); !it.empty(); ++it)
					{
						GAIA::CTN::Ref<Link> reflink = *it;
						Link* pLink = reflink;
						GAST(pLink != GNIL);
						m_listTempLinkForRecv1.push_back(pLink);

						m_listTempNodeForRecv0.clear();
						for(__NodeSetType::it itt = pLink->nodes.frontit(); !itt.empty(); ++itt)
						{
							GAIA::CTN::Ref<Node>& refnode = *itt;
							Node* pNode = refnode;
							GAST(pNode != GNIL);
							m_listTempNodeForRecv0.push_back(pNode);
						}
						pLink->nodes.clear();
						for(__NodeVectorType::it itt = m_listTempNodeForRecv0.frontit(); !itt.empty(); ++itt)
						{
							Node* pNode = *itt;
							GAST(pNode != GNIL);

							// Recycle buffer.
							{
								GAIA::SYNC::Autolock al(m_lrBufferPool);
								m_bufpool.release(pNode->pBuf);
							}

							// Recycle node.
							{
								GAIA::SYNC::Autolock al(m_lrNodePool);
								m_nodepool.release(pNode);
							}
						}
					}
					m_recvlinks_byaddr.clear();
					m_recvablelinks_byaddr.clear();
					for(__LinkVectorType::it it = m_listTempLinkForRecv1.frontit(); !it.empty(); ++it)
					{
						Link* pLink = *it;
						GAST(pLink != GNIL);

						// Recycle link.
						{
							GAIA::SYNC::Autolock al(m_lrLinkPool);
							m_linkpool.release(pLink);
						}
					}
				}
			}
			else
			{
				// Reset send cache.
				if(bSend)
				{
					GAIA::SYNC::Autolock al(m_lrSendCache);
					m_listTempNodeForSend0 = m_sendcache;
					m_sendcache.clear();
					for(__NodeVectorType::it it = m_listTempNodeForSend0.frontit(); !it.empty(); ++it)
					{
						Node* pNode = *it;
						GAST(pNode != GNIL);
						if(pNode->addr == *pAddr)
						{
							// Recycle buffer.
							{
								GAIA::SYNC::Autolock al(m_lrBufferPool);
								m_bufpool.release(pNode->pBuf);
							}

							// Recycle node.
							{
								GAIA::SYNC::Autolock al(m_lrNodePool);
								m_nodepool.release(pNode);
							}
						}
						else
							m_sendcache.push_back(pNode);
					}
				}

				// Reset sent history.
				if(bSend)
				{
					GAIA::SYNC::Autolock al(m_lrSent);
					Link linkfinder;
					linkfinder.addr = *pAddr;
					GAIA::CTN::Ref<Link>* pFindedLinkRef = m_sentlinks_byaddr.find(GAIA::CTN::Ref<Link>(&linkfinder));
					if(pFindedLinkRef != GNIL)
					{
						Link* pFindedLink = *pFindedLinkRef;
						GAST(pFindedLink != GNIL);
						GAST(pFindedLink->addr == *pAddr);

						m_listTempNodeForSend0.clear();
						for(__NodeSetType::it itt = pFindedLink->nodes.frontit(); !itt.empty(); ++itt)
						{
							Node* pNode = *itt;
							GAST(pNode != GNIL);
							m_listTempNodeForSend0.push_back(pNode);
						}
						pFindedLink->nodes.clear();
						for(__NodeVectorType::it itt = m_listTempNodeForSend0.frontit(); !itt.empty(); ++itt)
						{
							Node* pNode = *itt;

							// Recycle buffer.
							{
								GAIA::SYNC::Autolock al(m_lrBufferPool);
								m_bufpool.release(pNode->pBuf);
							}

							// Recycle node.
							{
								GAIA::SYNC::Autolock al(m_lrNodePool);
								m_nodepool.release(pNode);
							}
						}

						m_sentlinks_byaddr.erase(GAIA::CTN::Ref<Link>(&linkfinder));

						// Recycle link.
						{
							GAIA::SYNC::Autolock al(m_lrLinkPool);
							m_linkpool.release(pFindedLink);
						}
					}
				}

				// Reset recv context.
				if(bRecv)
				{
					GAIA::SYNC::Autolock al(m_lrRecv);

					Link linkfinder;
					linkfinder.addr = *pAddr;
					GAIA::CTN::Ref<Link> linkreffinder = &linkfinder;
					GAIA::CTN::Ref<Link>* pFindedLinkRef = m_recvlinks_byaddr.find(linkreffinder);
					if(pFindedLinkRef != GNIL)
					{
						m_recvlinks_byaddr.erase(linkreffinder);
						m_recvablelinks_byaddr.erase(linkreffinder);

						Link* pFindedLink = *pFindedLinkRef;
						GAST(pFindedLink != GNIL);

						m_listTempNodeForRecv0.clear();
						for(__NodeSetType::it it = pFindedLink->nodes.frontit(); !it.empty(); ++it)
						{
							GAIA::CTN::Ref<Node>& refnode = *it;
							Node* pNode = refnode;
							GAST(pNode != GNIL);
							m_listTempNodeForRecv0.push_back(pNode);
						}
						pFindedLink->nodes.clear();
						for(__NodeVectorType::it it = m_listTempNodeForRecv0.frontit(); !it.empty(); ++it)
						{
							Node* pNode = *it;
							GAST(pNode != GNIL);

							// Recycle buffer.
							{
								GAIA::SYNC::Autolock al(m_lrBufferPool);
								m_bufpool.release(pNode->pBuf);
							}

							// Recycle node.
							{
								GAIA::SYNC::Autolock al(m_lrNodePool);
								m_nodepool.release(pNode);
							}
						}

						// Recycle link.
						{
							GAIA::SYNC::Autolock al(m_lrLinkPool);
							m_linkpool.release(pFindedLink);
						}
					}
				}
			}

			return GAIA::True;
		}

		const GAIA::NETWORK::SUDPSocket::State& SUDPSocket::GetState() const
		{
			return m_state;
		}
	}
}
