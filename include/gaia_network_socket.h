#ifndef		__GAIA_NETWORK_SOCKET_H__
#define		__GAIA_NETWORK_SOCKET_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_vari.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"

namespace GAIA
{
	namespace NETWORK
	{
		/*!
			@brief Origin socket class.
		*/
		class Socket : public GAIA::Base
		{
			friend class AsyncSocket;

		public:
			/*!
				@brief Socket type.
			*/
			GAIA_ENUM_BEGIN(SOCKET_TYPE)
				SOCKET_TYPE_STREAM,
				SOCKET_TYPE_DATAGRAM,
			GAIA_ENUM_END(SOCKET_TYPE)

			/*!
				@brief Socket option.
			*/
			GAIA_ENUM_BEGIN(SOCKET_OPTION)
				SOCKET_OPTION_SENDBUFSIZE, 	/*!< N32 */
				SOCKET_OPTION_RECVBUFSIZE,	/*!< N32 */
				SOCKET_OPTION_NOBLOCK,		/*!< BL */
				SOCKET_OPTION_REUSEADDR,	/*!< BL */
				SOCKET_OPTION_REUSEPORT,	/*!< BL */
				SOCKET_OPTION_TCPNODELAY,	/*!< BL */
				SOCKET_OPTION_KEEPALIVE,	/*!< BL */
				SOCKET_OPTION_SENDTIMEOUT,	/*!< N32 */
				SOCKET_OPTION_RECVTIMEOUT,	/*!< N32 */
				SOCKET_OPTION_LINGER,		/*!< N32 */
				SOCKET_OPTION_BROADCAST,	/*!< BL */
				SOCKET_OPTION_DONTROUTE,	/*!< BL */
				SOCKET_OPTION_SENDLOWAT,	/*!< N32 */
				SOCKET_OPTION_RECVLOWAT,	/*!< N32 */
			GAIA_ENUM_END(SOCKET_OPTION)

			/*!
				@brief Socket send flag.
			*/
			GAIA_ENUM_BEGIN(SOCKET_SEND_FLAG)
				SSF_DEFAULT = 0x00000000,
				SSF_OOB = 0x00000001,
				SSF_DONTROUTE = 0x00000002,
			GAIA_ENUM_END(SOCKET_SEND_FLAG)

			/*!
				@brief Socket receive flag.
			*/
			GAIA_ENUM_BEGIN(SOCKET_RECV_FLAG)
				SRF_DEFAULT = 0x00000000,
				SRF_OOB = 0x00010000,
				SRF_PEEK = 0x00010000,
			GAIA_ENUM_END(SOCKET_RECV_FLAG)

			/*!
				@brief Socket shutdown flag.
			*/
			GAIA_ENUM_BEGIN(SOCKET_SHUTDOWN_FLAG)
				SSDF_DEFAULT = 0x00000000,
				SSDF_RECV = 0x10000000,
				SSDF_SEND = 0x20000000,
			GAIA_ENUM_END(SOCKET_SHUTDOWN_FLAG)
		public:

			/*!
				@brief Constructor.
			*/
			GINL Socket();

			/*!
				@brief Destructor.

				@remarks
					If current socket is created, it will be close automatically.
			*/
			GINL ~Socket();

			/*!
				@brief Create socket.

				@param socktype [in] Specify the socket type, could be SOCKET_TYPE_STREAM OR SOCKET_TYPE_DATAGRAM.

				@exception
					GAIA::ECT::EctInvalidParam If socktype is not a valid type.

				@exception
					GAIA::ECT::EctIllegal If socket is created.

				@exception
					GAIA::ECT::EctNetwork If there is not enough resource to create socket, throw it.
			*/
			GINL GAIA::GVOID Create(GAIA::NETWORK::Socket::SOCKET_TYPE socktype);

			/*!
				@brief Close socket.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If close socket failed.
			*/
			GINL GAIA::GVOID Close();

			/*!
				@brief Shutdown socket.

				@param nShutdownFlag [in] Specify shutdown flag. @see GAIA::NETWORK::Socket::SOCKET_SHUTDOWN_FLAG.
			 
				@exception
					GAIA::ECT::EctInvalidParam If nShutdownFlag is not a valid socket shutdown flag mask.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If shutdown socket failed.
			 
				@see GAIA::NETWORK::Socket::SOCKET_SHUTDOWN_FLAG.
			*/
			GINL GAIA::GVOID Shutdown(GAIA::N32 nShutdownFlag = GAIA::NETWORK::Socket::SSDF_RECV | GAIA::NETWORK::Socket::SSDF_SEND);

			/*!
				@brief Check current socket is create or not.
			 
				@return If the socket is created, return GAIA::True, or will return GAIA::False.
			*/
			GINL GAIA::BL IsCreated() const;

			/*!
				@brief Set socket option.
			 
				@param op [in] Specify the socket option name. @see GAIA::NETWORK::Socket::SOCKET_OPTION.
			 
				@param v [in] Specify a socket option value.
				
				@exception
					GAIA::ECT::EctInvalidParam If parameter op is a invalid enum.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If set socket option failed.
			*/
			GINL GAIA::GVOID SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, const GAIA::CTN::Vari& v);

			/*!
				@brief Get socket option.
			 
				@param op [in] Specify the socket option name. @see GAIA::NETWORK::Socket::SOCKET_OPTION.
			 
				@param v [out] Used for saving the socket option value.

				@exception
					GAIA::ECT::EctInvalidParam If parameter op is a invalid enum.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.
			*/
			GINL GAIA::GVOID GetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, GAIA::CTN::Vari& v);

			/*!
				@brief Get socket type.
			 
				@return Return the socket type. 

				@see GAIA::NETWORK::Socket::SOCKET_TYPE.
			*/
			GINL GAIA::NETWORK::Socket::SOCKET_TYPE GetType() const;

			/*!
				@brief Bind socket to a network address, include IP and port.

				@param addr [in] Specify the address where will be bound.
			 
				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If bind socket failed.
			*/
			GINL GAIA::GVOID Bind(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Check current socket is binded or not.

				@return If the socket is bound, return GAIA::True, or return GAIA::False.
			*/
			GINL GAIA::BL IsBinded() const;

			/*!
				@brief Accept a socket.(Stream socket only)
			 
				@param sock [in] Specify a GAIA::NETWORK::Socket to saving the accepted socket.

				@exception
					GAIA::ECT::EctInvalidParam If parameter sock is a created socket.
			 
				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If accept socket option failed.
			*/
			GINL GAIA::GVOID Accept(GAIA::NETWORK::Socket& sock);

			/*!
				@brief Listen socket.(Stream socket only)

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If listen socket failed.
			*/
			GINL GAIA::GVOID Listen();

			/*!
				@brief Connect to a network address.(Stream socket only)

				@param addr [in] Specify a address to connect to.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter addr is a invalid connect address.
			 
				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If socket connect to network address failed.
			*/
			GINL GAIA::GVOID Connect(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Check current socket is connect or not.(Stream socket only)
			 
				@return If the socket is a streamed socket(TCP), and it is connected by Socket::Connect or Socket::Accept, 
					it will return GAIA::True, or will return GAIA::False. 
			*/
			GINL GAIA::BL IsConnected() const;

			/*!
				@brief Send data to peer.(Stream socket only)

				@param p [in] Specify a buffer to send.
			 
				@param nSize [in] Specify the buffer's size in bytes.
			 
				@param nSendFlag [in] Specify the send flag. @see GAIA::NETWORK::Socket::SOCKET_SEND_FLAG.

				@return Return the practice sent size in bytes.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter p is GNIL.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter nSize below equal zero.

				@exception
					GAIA::ECT::EctInvalidParam If parameter nSendFlag is a invalid send flag.
			 
				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If send stream socket data failed.
			 
				@see GAIA::NETWORK::Socket::SOCKET_SEND_FLAG.
			*/
			GINL GAIA::N32 Send(const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32 nSendFlag = GAIA::NETWORK::Socket::SSF_DEFAULT);

			/*!
				@brief Receive data from peer.(Stream socket only)

				@param p [out] Used for saving the received data.
			 
				@param nSize [in] Specify the max size of parameter p in bytes.
			 
				@param nRecvFlag [in] Specify the receive flags. @see GAIA::NETWORK::Socket::SOCKET_RECV_FLAG.
			 
				@return Return the practice received data size in bytes.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter p is GNIL.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter nSize is below equal zero.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter nRecvFlag is a invalid recv flag.
			 
				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If recv stream socket data failed.
			 
				@see GAIA::NETWORK::Socket::SOCKET_RECV_FLAG.
			*/
			GINL GAIA::N32 Recv(GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32 nRecvFlag = GAIA::NETWORK::Socket::SRF_DEFAULT);

			/*!
				@brief Send data to peer.(Datagram socket only)

				@param addr [in] Specify a network address to send to.
					It must be a valid network address.
			 
				@param p [in] Specify a buffer to send to.
			 
				@param nSize [in] Specify the buffer's size in bytes.
			 
				@param nSendFlag [in] Specify the send flag. @see GAIA::NETWORK::Socket::SOCKET_SEND_FLAG.

				@return Return the practice sent size in bytes.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter addr is a invalid sendto address.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter p is GNIL.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter nSize below equal zero.

				@exception
					GAIA::ECT::EctInvalidParam If parameter nSendFlag is a invalid send flag.
			 
				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If send datagram socket data failed.
			 
				@see GAIA::NETWORK::Socket::SOCKET_SEND_FLAG.
			*/
			GINL GAIA::N32 SendTo(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32 nSendFlag = GAIA::NETWORK::Socket::SSF_DEFAULT);

			/*!
				@brief Receive data from peer.(Datagram socket only)

				@param addr [inout] If it is a valid network address, it specify a network address to receive from.
					If it is not a valid address, it used for saving the address where the datagram come from.

				@param p [out] Used for saving the received data.
			 
				@param nSize [in] Specify the max size of parameter p in bytes.
			 
				@param nRecvFlag [in] Specify the receive flags. @see GAIA::NETWORK::Socket::SOCKET_RECV_FLAG.
			 
				@return Return the practice received data size in bytes.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter p is GNIL.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter nSize is below equal zero.
			 
				@exception
					GAIA::ECT::EctInvalidParam If parameter nRecvFlag is a invalid recv flag.
			 
				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If recv datagram socket data failed.
			 
				@see GAIA::NETWORK::Socket::SOCKET_RECV_FLAG.
			*/
			GINL GAIA::N32 RecvFrom(GAIA::NETWORK::Addr& addr, GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32 nRecvFlag = GAIA::NETWORK::Socket::SRF_DEFAULT);

			/*!
				@brief Get socket file descriptor.
			 
				@return Return the socket's file descriptor.
			*/
			GINL GAIA::N32 GetFD() const;

			/*!
				@brief Get socket's bind address.
			 
				@param [out] addr Used for saving the binded address.

				@return If success return GAIA::True, or return GAIA::False;
			*/
			GINL GAIA::BL GetBindedAddress(GAIA::NETWORK::Addr& addr);

			/*!
				@brief Get socket's global address.

				@param [out] addr Used for saving the global address.
			 
				@return If success return GAIA::True, or return GAIA::False.
			*/
			GINL GAIA::BL GetGlobalAddress(GAIA::NETWORK::Addr& addr);

			/*!
				@brief Get socket's local address.
			 
				@param [out] addr Used for saving the local address.

				@return If success return GAIA::True, or return GAIA::False.
			*/
			GINL GAIA::BL GetLocalAddress(GAIA::NETWORK::Addr& addr);

			/*!
				@brief Get socket's peer address.
			 
				@param [out] addr Used for saving the peer address.

				@return If success return GAIA::True, or return GAIA::False.
			*/
			GINL GAIA::BL GetPeerAddress(GAIA::NETWORK::Addr& addr);

		private:
			GINL GAIA::GVOID init();
			GINL GAIA::BL SetFD(GAIA::N32 nFD);
			GINL GAIA::BL SetType(GAIA::NETWORK::Socket::SOCKET_TYPE type);
			GINL GAIA::BL SetBinded(GAIA::BL bBinded);
			GINL GAIA::BL SetConnected(GAIA::BL bConnected);
			GINL GAIA::GVOID SetPeerAddress(const GAIA::NETWORK::Addr& addr);
			GINL Socket(const Socket& src){}
			GINL Socket& operator = (const Socket& src){return *this;}

		private:
			GAIA::N32 m_nSocket;
			GAIA::NETWORK::Socket::SOCKET_TYPE m_SockType;
			GAIA::N32 m_nSendBufferSize;
			GAIA::N32 m_nRecvBufferSize;
			GAIA::N32 m_nSendTimeout;
			GAIA::N32 m_nRecvTimeout;
			GAIA::N32 m_nSendLoWat;
			GAIA::N32 m_nRecvLoWat;
			GAIA::N32 m_nLingerSecond; // GINVALID means close linger, 0 means recycle immediately, above zero means linger seconds.
			GAIA::NETWORK::Addr m_addrBinded;
			GAIA::NETWORK::Addr m_addrPeer;
			GAIA::BL m_bBinded : 1;
			GAIA::BL m_bConnected : 1;
			GAIA::BL m_bNotBlock : 1;
			GAIA::BL m_bReuseAddr : 1;
			GAIA::BL m_bReusePort : 1;
			GAIA::BL m_bTCPNoDelay : 1;
			GAIA::BL m_bKeepAlive : 1;
			GAIA::BL m_bBroadcast : 1;
			GAIA::BL m_bDontRoute : 1;
		};
	}
}

#endif
