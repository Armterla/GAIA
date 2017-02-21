#ifndef		__GAIA_NETWORK_SOCKET_IMPL_H__
#define		__GAIA_NETWORK_SOCKET_IMPL_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_memory.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
#include "gaia_network_socket.h"

#if GAIA_OS == GAIA_OS_WINDOWS
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	include <windows.h>
#else
#	include <unistd.h>
#	include <sys/errno.h>
#	if GAIA_OS == GAIA_OS_ANDROID
#		include <fcntl.h>
#	else
#		include <sys/fcntl.h>
#	endif
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <netinet/tcp.h>
#	include <netdb.h>
#endif

namespace GAIA
{
	namespace NETWORK
	{
		#define THROW_LASTERROR ThrowByLastError(__FILE__, __LINE__)

		GINL GAIA::GVOID ThrowByLastError(const GAIA::CH* pszFileName, GAIA::N32 nCodeLine)
		{
			GAST(!GAIA::ALGO::gstremp(pszFileName));
			GAST(nCodeLine >= 0);

		#if GAIA_OS == GAIA_OS_WINDOWS
		#	define THROW_ERR(error_number) case WSAE##error_number:{GAIA::ECT::EctNetwork enw(pszFileName, nCodeLine, GNIL, GAIA::ECT::GERROR_##error_number, nOSError); GTHROW_PURE(enw);}

			GAIA::N32 nOSError = WSAGetLastError();
			switch (nOSError)
			{
			THROW_ERR(NETDOWN)
			THROW_ERR(NETRESET)
			THROW_ERR(ACCES)
			THROW_ERR(AFNOSUPPORT)
			THROW_ERR(MFILE)
			THROW_ERR(NOBUFS)
			THROW_ERR(PROTONOSUPPORT)
			THROW_ERR(PROTOTYPE)
			THROW_ERR(ADDRINUSE)
			THROW_ERR(ADDRNOTAVAIL)
			THROW_ERR(BADF)
			THROW_ERR(DESTADDRREQ)
			THROW_ERR(FAULT)
			THROW_ERR(INVAL)
			THROW_ERR(NOTSOCK)
			THROW_ERR(OPNOTSUPP)
			THROW_ERR(LOOP)
			THROW_ERR(NAMETOOLONG)
			THROW_ERR(CONNABORTED)
			THROW_ERR(INTR)
			THROW_ERR(WOULDBLOCK)
			THROW_ERR(ALREADY)
			THROW_ERR(CONNREFUSED)
			THROW_ERR(HOSTUNREACH)
			THROW_ERR(INPROGRESS)
			THROW_ERR(ISCONN)
			THROW_ERR(NETUNREACH)
			THROW_ERR(TIMEDOUT)
			THROW_ERR(CONNRESET)
			THROW_ERR(NOPROTOOPT)
			THROW_ERR(MSGSIZE)
			THROW_ERR(NOTCONN)
			//// Not dispatched error code.
			////case WSAESOCKTNOSUPPORT:
			////case WSAEPFNOSUPPORT:
			////case WSAESHUTDOWN:
			////case WSAETOOMANYREFS:
			////case WSAEHOSTDOWN:
			////case WSAENOTEMPTY:
			////case WSAEPROCLIM:
			////case WSAEUSERS:
			////case WSAEDQUOT:
			////case WSAESTALE:
			////case WSAEREMOTE:
			////case WSAEDISCON:
			////case WSASYSNOTREADY:
			////case WSAVERNOTSUPPORTED:
			////case WSANOTINITIALISED:
			////case WSAHOST_NOT_FOUND:
			////case WSATRY_AGAIN:
			////case WSANO_RECOVERY:
			////case WSANO_DATA:
			////case WSA_SECURE_HOST_NOT_FOUND:
			////case WSA_IPSEC_NAME_POLICY_ERROR:
			default:
				GAIA::ECT::EctUnknown e(pszFileName, nCodeLine, GNIL);
				GTHROW_PURE(e);
			}
		#else
		#	define THROW_ERR(error_number) case E##error_number:{GAIA::ECT::EctNetwork enw(pszFileName, nCodeLine, GNIL, GAIA::ECT::GERROR_##error_number, nOSError); GTHROW_PURE(enw);}
			GAIA::N32 nOSError = errno;
			switch(nOSError)
			{
			THROW_ERR(NETDOWN)
			THROW_ERR(NETRESET)
			THROW_ERR(ACCES)
			THROW_ERR(AFNOSUPPORT)
			THROW_ERR(MFILE)
			THROW_ERR(NOBUFS)
			THROW_ERR(PROTONOSUPPORT)
			THROW_ERR(PROTOTYPE)
			THROW_ERR(ADDRINUSE)
			THROW_ERR(ADDRNOTAVAIL)
			THROW_ERR(BADF)
			THROW_ERR(DESTADDRREQ)
			THROW_ERR(FAULT)
			THROW_ERR(INVAL)
			THROW_ERR(NOTSOCK)
			THROW_ERR(OPNOTSUPP)
			THROW_ERR(LOOP)
			THROW_ERR(NAMETOOLONG)
			THROW_ERR(CONNABORTED)
			THROW_ERR(INTR)
			THROW_ERR(WOULDBLOCK)
			THROW_ERR(ALREADY)
			THROW_ERR(CONNREFUSED)
			THROW_ERR(HOSTUNREACH)
			THROW_ERR(INPROGRESS)
			THROW_ERR(ISCONN)
			THROW_ERR(NETUNREACH)
			THROW_ERR(TIMEDOUT)
			THROW_ERR(CONNRESET)
			THROW_ERR(NOPROTOOPT)
			THROW_ERR(MSGSIZE)
			THROW_ERR(NOTCONN)
			case ENFILE:
			case ENOMEM:
			case EIO:
			case EISDIR:
			case ENOENT:
			case ENOTDIR:
			case EROFS:
			case EDOM:
			case EPIPE:
			//// Not dispatched error code.
			////case EPERM:
			////case EAGAIN: // This error is the same to EWOULDBLOCK
			////case ESRCH:
			////case ENXIO:
			////case E2BIG:
			////case ENOEXEC:
			////case ECHILD:
			////case EBUSY:
			////case EEXIST:
			////case EXDEV:
			////case ENODEV:
			////case ENOTTY:
			////case EFBIG:
			////case ENOSPC:
			////case ESPIPE:
			////case EMLINK:
			////case EDEADLK:
			////case ENOLCK:
			////case ENOSYS:
			////case ENOTEMPTY:
			////case ERANGE:
			////case EILSEQ:
			////case STRUNCATE:
			////case EBADMSG:
			////case ECANCELED:
			////case EIDRM:
			////case ENODATA:
			////case ENOLINK:
			////case ENOMSG:
			////case ENOSR:
			////case ENOSTR:
			////case ENOTRECOVERABLE:
			////case ENOTSUP:
			////case EOTHER:
			////case EOVERFLOW:
			////case EOWNERDEAD:
			////case EPROTO:
			////case ETIME:
			////case ETXTBSY:
			default:
				GAIA::ECT::EctUnknown e(pszFileName, nCodeLine, GNIL);
				GTHROW_PURE(e);
			}
		#endif
		}

		GINL Socket::Socket()
		{
			this->init();
		}

		GINL Socket::~Socket()
		{
			GTRY
			{
				if(this->IsCreated())
					this->Close();
			}
			GCATCH(Network)
			{
				e.SetDispatched(GAIA::True);
			}
		}

		GINL GAIA::GVOID Socket::Create(GAIA::NETWORK::Socket::SOCKET_TYPE socktype)
		{
			if(this->IsCreated())
				GTHROW(Illegal);

			if(socktype == GAIA::NETWORK::Socket::SOCKET_TYPE_STREAM)
				m_nSocket = (GAIA::N32)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			else if(socktype == GAIA::NETWORK::Socket::SOCKET_TYPE_DATAGRAM)
				m_nSocket = (GAIA::N32)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			else
				GTHROW(InvalidParam);

			if(m_nSocket == GINVALID)
				THROW_LASTERROR;

		#if GAIA_OS == GAIA_OS_WINDOWS
			if(socktype == GAIA::NETWORK::Socket::SOCKET_TYPE_DATAGRAM)
			{
				#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
				BOOL bNewBehavior = 0;
				GAIA::UM uBytesReturn;
				GAIA::N32 nResult = ::WSAIoctl(m_nSocket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), GNIL, 0, &uBytesReturn, GNIL, GNIL);
				if(nResult == GINVALID)
				{
					GAIA::N32 nLastError = ::WSAGetLastError();
					if(nLastError == WSAEWOULDBLOCK)
					{
					}
					GASTFALSE;
				}
			}
		#endif

			m_SockType = socktype;
		}

		GINL GAIA::GVOID Socket::Close()
		{
			if(!this->IsCreated())
				GTHROW(Illegal);
		#if GAIA_OS == GAIA_OS_WINDOWS
			if(closesocket(m_nSocket) == GINVALID)
				THROW_LASTERROR;
		#else
			if(close(m_nSocket) == GINVALID)
				THROW_LASTERROR;
		#endif
			this->init();
		}

		GINL GAIA::GVOID Socket::Shutdown(GAIA::N32 nShutdownFlag)
		{
			if(!this->IsCreated())
				GTHROW(Illegal);
			GAIA::N32 nFlag = 0;
		#if GAIA_OS == GAIA_OS_WINDOWS
			if(nShutdownFlag & GAIA::NETWORK::Socket::SSDF_SEND && nShutdownFlag & GAIA::NETWORK::Socket::SSDF_RECV)
				nFlag = SD_BOTH;
			else
			{
				if(nShutdownFlag & GAIA::NETWORK::Socket::SSDF_SEND)
					nFlag |= SD_SEND;
				if(nShutdownFlag & GAIA::NETWORK::Socket::SSDF_RECV)
					nFlag |= SD_RECEIVE;
			}
		#else
			if(nShutdownFlag & GAIA::NETWORK::Socket::SSDF_SEND && nShutdownFlag & GAIA::NETWORK::Socket::SSDF_RECV)
				nFlag = SHUT_RDWR;
			else
			{
				if(nShutdownFlag & GAIA::NETWORK::Socket::SSDF_SEND)
					nFlag |= SHUT_WR;
				if(nShutdownFlag & GAIA::NETWORK::Socket::SSDF_RECV)
					nFlag |= SHUT_RD;
			}
		#endif
		
			if(shutdown(m_nSocket, nFlag) == GINVALID)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::N32 nOSError = WSAGetLastError();
				if(nOSError != WSAENOTCONN)
			#else
				GAIA::N32 nOSError = errno;
				if(nOSError != ENOTCONN)
			#endif
				THROW_LASTERROR;
			}
		}

		GINL GAIA::BL Socket::IsCreated() const
		{
			return m_nSocket != GINVALID;
		}

		GINL GAIA::NETWORK::Socket::SOCKET_TYPE Socket::GetType() const
		{
			if(!this->IsCreated())
				GTHROW(Illegal);
			return m_SockType;
		}

		GINL GAIA::GVOID Socket::Bind(const GAIA::NETWORK::Addr& addr)
		{
			if(!this->IsCreated())
				GTHROW(Illegal);
			if(m_bBinded)
				GTHROW(Illegal);

			sockaddr_in saddr;
			zeromem(&saddr);
			saddr.sin_family = AF_INET;

			// Construct network ip.
			if(!addr.ip.check())
				saddr.sin_addr.s_addr = 0;
			else
				addr2saddr(addr, &saddr);

			// Construct network port.
			if(addr.uPort == 0)
				saddr.sin_port = 0;
			else
				saddr.sin_port = port2sport((GAIA::U16)addr.uPort);

			//
			if(bind(m_nSocket, (sockaddr*)&saddr, sizeof(saddr)) == GINVALID)
				THROW_LASTERROR;

			//
			m_bBinded = true;
		}

		GINL GAIA::BL Socket::IsBinded() const
		{
			if(!this->IsCreated())
				GTHROW(Illegal);

			return m_bBinded;
		}

		GINL GAIA::GVOID Socket::SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, const GAIA::CTN::Vari& v)
		{
			if(!this->IsCreated())
				GTHROW(Illegal);

			switch(op)
			{
			case GAIA::NETWORK::Socket::SOCKET_OPTION_SENDBUFSIZE:
				{
					if(m_nSendBufferSize == (GAIA::N32)v)
						return;
					m_nSendBufferSize = v;
					if(setsockopt(m_nSocket, SOL_SOCKET, SO_SNDBUF, (GAIA::CH*)&m_nSendBufferSize, sizeof(m_nSendBufferSize)) != 0)
						THROW_LASTERROR;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_RECVBUFSIZE:
				{
					if(m_nRecvBufferSize == (GAIA::N32)v)
						return;
					m_nRecvBufferSize = v;
					if(setsockopt(m_nSocket, SOL_SOCKET, SO_RCVBUF, (GAIA::CH*)&m_nRecvBufferSize, sizeof(m_nRecvBufferSize)) != 0)
						THROW_LASTERROR;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_NOBLOCK:
				{
					if(m_bNotBlock == (GAIA::BL)v)
						return;
					m_bNotBlock = (GAIA::BL)v;
				#if GAIA_OS == GAIA_OS_WINDOWS
					GAIA::UM bNotBlockModeEnable = m_bNotBlock ? 1 : 0;
					ioctlsocket(m_nSocket, FIONBIO, &bNotBlockModeEnable);
				#else
					GAIA::N32 flags = fcntl(m_nSocket, F_GETFL, 0);
					if(m_bNotBlock)
						flags |= O_NONBLOCK;
					else
						flags &= (~O_NONBLOCK);
					fcntl(m_nSocket, F_SETFL, flags);
				#endif
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_TCPNODELAY:
				{
					if(m_bTCPNoDelay == (GAIA::BL)v)
						return;
					m_bTCPNoDelay = (GAIA::BL)v;
					GAIA::N32 nOption = m_bTCPNoDelay;
					if(setsockopt(m_nSocket, IPPROTO_TCP, TCP_NODELAY, (GAIA::CH*)&nOption, sizeof(nOption)) != 0)
						THROW_LASTERROR;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_KEEPALIVE:
				{
					if(m_bKeepAlive == (GAIA::BL)v)
						return;
					m_bKeepAlive = (GAIA::BL)v;
					GAIA::N32 nOption = m_bKeepAlive;
					if(setsockopt(m_nSocket, SOL_SOCKET, SO_KEEPALIVE, (GAIA::CH*)&nOption, sizeof(nOption)) != 0)
						THROW_LASTERROR;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_REUSEADDR:
				{
					if(m_bReuseAddr == (GAIA::BL)v)
						return;
					m_bReuseAddr = (GAIA::BL)v;
					GAIA::N32 nOption = m_bReuseAddr;
					if(setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEADDR, (GAIA::CH*)&nOption, sizeof(nOption)) != 0)
						THROW_LASTERROR;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_REUSEPORT:
				{
					if(m_bReusePort == (GAIA::BL)v)
						return;
				#if GAIA_OS == GAIA_OS_WINDOWS
					GTHROW(NotSupport);
				#else
					m_bReusePort = (GAIA::BL)v;
					GAIA::N32 nOption = m_bReusePort;
					if(setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEPORT, (GAIA::CH*)&nOption, sizeof(nOption)) != 0)
						THROW_LASTERROR;
				#endif
				}
				break;
			default:
				GTHROW(InvalidParam);
			}
		}

		GINL GAIA::GVOID Socket::GetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, GAIA::CTN::Vari& v)
		{
			if(!this->IsCreated())
				GTHROW(Illegal);

			switch(op)
			{
			case GAIA::NETWORK::Socket::SOCKET_OPTION_SENDBUFSIZE:
				{
					v = m_nSendBufferSize;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_RECVBUFSIZE:
				{
					v = m_nRecvBufferSize;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_NOBLOCK:
				{
					v = (GAIA::BL)m_bNotBlock;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_REUSEADDR:
				{
					v = (GAIA::BL)m_bReuseAddr;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_REUSEPORT:
				{
					v = (GAIA::BL)m_bReusePort;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_TCPNODELAY:
				{
					v = (GAIA::BL)m_bTCPNoDelay;
				}
				break;
			case GAIA::NETWORK::Socket::SOCKET_OPTION_KEEPALIVE:
				{
					v = (GAIA::BL)m_bKeepAlive;
				}
				break;
			default:
				GTHROW(InvalidParam);
			}
		}

		GINL GAIA::GVOID Socket::Accept(Socket& sock)
		{
			if(sock.IsCreated())
				GTHROW(InvalidParam);
			if(!this->IsCreated())
				GTHROW(Illegal);
		
			sockaddr_in newaddr;
			socklen_t newaddrlen = sizeof(newaddr);

			sock.m_nSocket = (GAIA::N32)accept(m_nSocket, (sockaddr*)&newaddr, &newaddrlen);
			if(sock.m_nSocket == GINVALID)
				THROW_LASTERROR;
			sock.m_bBinded = true;
			sock.m_bConnected = true;
		}

		GINL GAIA::GVOID Socket::Listen()
		{
			if(!this->IsCreated())
				GTHROW(Illegal);

			if(listen(m_nSocket, SOMAXCONN) == GINVALID)
				THROW_LASTERROR;
		}

		GINL GAIA::GVOID Socket::Connect(const GAIA::NETWORK::Addr& addr)
		{
			if(!addr.check())
				GTHROW(InvalidParam);

			if(!this->IsCreated())
				GTHROW(Illegal);
			if(m_bConnected)
				GTHROW(Illegal);

			// Construct network address.
			sockaddr_in saddr;
			zeromem(&saddr);
			saddr.sin_family = AF_INET;
			addr2saddr(addr, &saddr);

			if(connect(m_nSocket, (sockaddr*)&saddr, sizeof(saddr)) == GINVALID)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::N32 nOSError = WSAGetLastError();
				if(nOSError != WSAEWOULDBLOCK)
			#else
				GAIA::N32 nOSError = errno;
				if(nOSError != EWOULDBLOCK)
			#endif
				THROW_LASTERROR;
			}

			m_bConnected = true;
		}

		GINL GAIA::BL Socket::IsConnected() const
		{
			if(!this->IsCreated())
				GTHROW(Illegal);
			return m_bConnected;
		}

		GINL GAIA::N32 Socket::Send(const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32 nSendFlag)
		{
			if(p == GNIL)
				GTHROW(InvalidParam);
			if(nSize <= 0)
				GTHROW(InvalidParam);

			if(!this->IsCreated())
				GTHROW(Illegal);

			GAIA::N32 nFlag = 0;
			if(nSendFlag & GAIA::NETWORK::Socket::SSF_OOB)
				nFlag |= MSG_OOB;
			if(nSendFlag & GAIA::NETWORK::Socket::SSF_DONTROUTE)
				nFlag |= MSG_DONTROUTE;

		#if GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_UNIX
			GAIA::N32 nSended = (GAIA::N32)send(m_nSocket, (const GAIA::CH*)p, nSize, nFlag | MSG_NOSIGNAL);
		#else
			GAIA::N32 nSended = (GAIA::N32)send(m_nSocket, (const GAIA::CH*)p, nSize, nFlag);
		#endif
		
			if(nSended == GINVALID)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::N32 nOSError = WSAGetLastError();
				if(nOSError == WSAEWOULDBLOCK)
					nSended = 0;
				else
			#else
				GAIA::N32 nOSError = errno;
				if(nOSError == EWOULDBLOCK)
					nSended = 0;
				else
			#endif
				THROW_LASTERROR;
			}
			return nSended;
		}

		GINL GAIA::N32 Socket::Recv(GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32 nRecvFlag)
		{
			if(p == GNIL)
				GTHROW(InvalidParam);
			if(nSize <= 0)
				GTHROW(InvalidParam);

			if(!this->IsCreated())
				GTHROW(Illegal);

			GAIA::N32 nFlag = 0;
			if(nRecvFlag & GAIA::NETWORK::Socket::SRF_OOB)
				nFlag |= MSG_OOB;
			if(nRecvFlag & GAIA::NETWORK::Socket::SRF_PEEK)
				nFlag |= MSG_PEEK;

			GAIA::N32 nRecved = (GAIA::N32)recv(m_nSocket, (GAIA::CH*)p, nSize, nFlag);
			if(nRecved == GINVALID)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::N32 nOSError = WSAGetLastError();
				if(nOSError == WSAEWOULDBLOCK)
					nRecved = 0;
				else
			#else
				GAIA::N32 nOSError = errno;
				if(nOSError == EWOULDBLOCK)
					nRecved = 0;
				else
			#endif
				THROW_LASTERROR;
			}

			return nRecved;
		}

		GINL GAIA::N32 Socket::SendTo(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32 nSendFlag)
		{
			if(!addr.check())
				GTHROW(InvalidParam);
			if(p == GNIL)
				GTHROW(InvalidParam);
			if(nSize <= 0)
				GTHROW(InvalidParam);

			if(!this->IsCreated())
				GTHROW(Illegal);

			GAIA::N32 nFlag = 0;
			if(nSendFlag & GAIA::NETWORK::Socket::SSF_OOB)
				nFlag |= MSG_OOB;
			if(nSendFlag & GAIA::NETWORK::Socket::SSF_DONTROUTE)
				nFlag |= MSG_DONTROUTE;

			sockaddr_in saddr;
			zeromem(&saddr);
			saddr.sin_family = AF_INET;
			addr2saddr(addr, &saddr);

		#if GAIA_OS == GAIA_OS_LINUX || GAIA_OS == GAIA_OS_UNIX
			GAIA::N32 nSended = sendto(m_nSocket, (const GAIA::CH*)p, nSize, nFlag | MSG_NOSIGNAL, (sockaddr*)&saddr, sizeof(saddr));
		#else
			GAIA::N32 nSended = (GAIA::N32)sendto(m_nSocket, (const GAIA::CH*)p, nSize, nFlag, (sockaddr*)&saddr, sizeof(saddr));
		#endif
			if(nSended == GINVALID)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::N32 nOSError = WSAGetLastError();
				if(nOSError == WSAEWOULDBLOCK)
					nSended = 0;
				else
			#else
				GAIA::N32 nOSError = errno;
				if(nOSError == EWOULDBLOCK)
					nSended = 0;
				else
			#endif
				THROW_LASTERROR;
			}

			return nSended;
		}

		GINL GAIA::N32 Socket::RecvFrom(GAIA::NETWORK::Addr& addr, GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32 nRecvFlag)
		{
			if(p == GNIL)
				GTHROW(InvalidParam);
			if(nSize <= 0)
				GTHROW(InvalidParam);

			if(!this->IsCreated())
				GTHROW(Illegal);

			GAIA::N32 nFlag = 0;
			if(nRecvFlag & GAIA::NETWORK::Socket::SRF_OOB)
				nFlag |= MSG_OOB;
			if(nRecvFlag & GAIA::NETWORK::Socket::SRF_PEEK)
				nFlag |= MSG_PEEK;

			sockaddr_in saddr;
			zeromem(&saddr);
			saddr.sin_family = AF_INET;
			if(addr.check())
				addr2saddr(addr, &saddr);

			socklen_t recvfrom_addr_len = sizeof(saddr);
			GAIA::N32 nRecved = (GAIA::N32)recvfrom(m_nSocket, (GAIA::CH*)p, nSize, nFlag, (sockaddr*)&saddr, &recvfrom_addr_len);
			if(nRecved == GINVALID)
			{
			#if GAIA_OS == GAIA_OS_WINDOWS
				GAIA::N32 nOSError = WSAGetLastError();
				if(nOSError == WSAEWOULDBLOCK)
					nRecved = 0;
				else
			#else
				GAIA::N32 nOSError = errno;
				if(nOSError == EWOULDBLOCK)
					nRecved = 0;
				else
			#endif
				THROW_LASTERROR;
			}

			saddr2addr(&saddr, addr);

			return nRecved;
		}

		GINL GAIA::N32 Socket::GetFD() const
		{
			return m_nSocket;
		}

		GINL GAIA::BL Socket::GetBindedAddress(GAIA::NETWORK::Addr& addr)
		{
			if(!m_addrBinded.check())
				return GAIA::False;
			addr = m_addrBinded;
			return GAIA::True;
		}

		GINL GAIA::BL Socket::GetGlobalAddress(GAIA::NETWORK::Addr& addr)
		{
			if(m_nSocket == GINVALID)
				return GAIA::False;

			sockaddr_in saddr;
			zeromem(&saddr);
			socklen_t sock_addr_len = sizeof(saddr);
			if(getpeername(m_nSocket, (sockaddr*)&saddr, &sock_addr_len) != 0)
				return GAIA::False;

			saddr2addr(&saddr, addr);

			return GAIA::True;
		}

		GINL GAIA::BL Socket::GetLocalAddress(GAIA::NETWORK::Addr& addr)
		{
			if(m_nSocket == GINVALID)
				return GAIA::False;

			sockaddr_in saddr;
			zeromem(&saddr);
			socklen_t sock_addr_len = sizeof(saddr);
			if(getsockname(m_nSocket, (sockaddr*)&saddr, &sock_addr_len) != 0)
				return GAIA::False;

			saddr2addr(&saddr, addr);

			return GAIA::True;
		}

		GINL GAIA::BL GetPeerAddress(GAIA::NETWORK::Addr& addr)
		{
			if(!m_addrPeer.check())
				return GAIA::False;
			addr = m_addrPeer;
			return GAIA::True;
		}

		GINL GAIA::GVOID Socket::init()
		{
			m_nSocket = GINVALID;
			m_SockType = SOCKET_TYPE_INVALID;
			m_nSendBufferSize = 1024 * 2;
			m_nRecvBufferSize = 1024 * 2;
			m_addrBinded.reset();
			m_addrPeer.reset();
			m_bBinded = GAIA::False;
			m_bConnected = GAIA::False;
			m_bNotBlock = GAIA::False;
			m_bReuseAddr = GAIA::False;
			m_bReusePort = GAIA::False;
			m_bTCPNoDelay = GAIA::False;
			m_bKeepAlive = GAIA::False;
		}

		GINL GAIA::BL Socket::SetFD(GAIA::N32 nFD)
		{
			if(m_nSocket != GINVALID)
				return GAIA::False;
			m_nSocket = nFD;
			return GAIA::True;
		}

		GINL GAIA::GVOID Socket::SetPeerAddress(GAIA::NETWORK::Addr& addr)
		{
			GAST(addr.check());
			m_addrPeer = addr;
		}
	}
}

#endif
