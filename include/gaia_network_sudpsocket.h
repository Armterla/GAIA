#ifndef		__GAIA_NETWORK_SUDPSOCKET_H__
#define		__GAIA_NETWORK_SUDPSOCKET_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_sync_lock.h"
#include "gaia_sync_autolock.h"
#include "gaia_ctn_ref.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_queue.h"
#include "gaia_ctn_buffer.h"
#include "gaia_ctn_pool.h"
#include "gaia_ctn_set.h"
#include "gaia_network_ip.h"
#include "gaia_network_addr.h"
#include "gaia_network_base.h"
#include "gaia_network_socket.h"

namespace GAIA
{
	namespace NETWORK
	{
		/*!
			@brief Safe UDP socket.

			@remarks
				Feature 1 : Use serial number solve udp datagram lose, datagram repeat, datagram order mistake problem.\n
				Feature 2 : Support safe udp datagram and unsafe udp datagram mix usage.\n
		*/
		class SUDPSocket : public GAIA::Base
		{
		public:
			static const GAIA::U64 DEFAULT_TIMEOUT_TIME = 10 * 1000 * 1000;
			static const GAIA::U64 DEFAULT_RESEND_TIME = 1 * 1000 * 1000;
			static const GAIA::U64 DEFAULT_RERECV_TIME = 10 * 1000;
			static const GAIA::U64 DEFAULT_RECYCLE_TIME = 10 * 1000 * 1000;

		public:
			class State : public GAIA::Base
			{
			public:
				GINL GAIA::GVOID reset()
				{
					uSendUnserialMsgCount = 0;
					uSendSerialMsgCount = 0;
					uSendUnserialMsgSize = 0;
					uSendSerialMsgSize = 0;

					uRecvUnserialMsgCount = 0;
					uRecvSerialMsgCount = 0;
					uRecvUnserialMsgSize = 0;
					uRecvSerialMsgSize = 0;

					uResendSerialMsgCount = 0;
					uResendSerialMsgSize = 0;

					uSendBackMsgCount = 0;
					uSendBackMsgSize = 0;
					uRecvBackMsgCount = 0;
					uRecvBackMsgSize = 0;
				}

			public:
				GAIA::U64 uSendUnserialMsgCount;
				GAIA::U64 uSendSerialMsgCount;
				GAIA::U64 uSendUnserialMsgSize;
				GAIA::U64 uSendSerialMsgSize;

				GAIA::U64 uRecvUnserialMsgCount;
				GAIA::U64 uRecvSerialMsgCount;
				GAIA::U64 uRecvUnserialMsgSize;
				GAIA::U64 uRecvSerialMsgSize;

				GAIA::U64 uResendSerialMsgCount;
				GAIA::U64 uResendSerialMsgSize;

				GAIA::U64 uSendBackMsgCount;
				GAIA::U64 uSendBackMsgSize;
				GAIA::U64 uRecvBackMsgCount;
				GAIA::U64 uRecvBackMsgSize;
			};

		public:
			/*!
				@brief Constructor.
			*/
			SUDPSocket();

			/*!
				@brief Destructor.

				@remarks
					If current socket is created, it will be close automatically.
			*/
			~SUDPSocket();

			/*!
				@brief Create socket.

				@exception
					GAIA::ECT::EctIllegal If socket is created.

				@exception
					GAIA::ECT::EctNetwork If there is not enough resource to create socket, throw it.

				@remarks
					This function will create a UDP datagram socket.
			*/
			GAIA::GVOID Create();

			/*!
				@brief Close socket.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If close socket failed.
			*/
			GAIA::GVOID Close();

			/*!
				@brief Shutdown socket.

				@param nShutdownFlag [in] Specify shutdown flags as a mask. see GAIA::NETWORK::Socket::SOCKET_SHUTDOWN_FLAG.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If shutdown socket failed.
			*/
			GAIA::GVOID Shutdown(GAIA::N32 nShutdownFlag = GAIA::NETWORK::Socket::SSDF_RECV | GAIA::NETWORK::Socket::SSDF_SEND);

			/*!
				@brief Check socket is create or not.
			*/
			GAIA::BL IsCreated() const;

			/*!
				@brief Bind address to socket.

				@param addr [in] Specify a network address to bind.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctIllegal If socket had binded.

				@exception
					GAIA::ECT::EctNetwork If bind socket failed.
			*/
			GAIA::GVOID Bind(const GAIA::NETWORK::Addr& addr);

			/*!
				@brief Check is bind address or not.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.
			*/
			GAIA::BL IsBinded() const;

			/*!
				@brief Set socket option.

				@param op [in] Specify the socket option type.

				@param v [in] Specify the socket option value.

				@exception
					GAIA::ECT::EctInvalidParam If parameter op is a invalid enum.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@exception
					GAIA::ECT::EctNetwork If set socket option failed.
			*/
			GAIA::GVOID SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, const GAIA::CTN::Vari& v);

			/*!
				@brief Get socket option.

				@param op [in] Specify the socket option type.

				@param v [out] Used for saving socket option value.

				@exception
					GAIA::ECT::EctInvalidParam If parameter op is a invalid enum.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.
			*/
			GAIA::GVOID GetOption(GAIA::NETWORK::Socket::SOCKET_OPTION op, GAIA::CTN::Vari& v);

			/*!
				@brief Set SUDPSocket timeout time in microseconds.

				@param uTimeout [in] Specify the timeout time in microseconds. Default value is 16 * 1000 * 1000(16 seconds).

				@remarks If parameter uTimeout equal zero, will reset timeout time to default.
			*/
			GAIA::GVOID SetTimeout(const GAIA::U64& uTimeout);

			/*!
				@brief Get SUDPSocket timeout time in microseconds.

				@return Return SUDPSocket timeout time in microseconds.
			*/
			const GAIA::U64& GetTimeout() const;

			/*!
				@brief Set SUDPSocket datagram resend time in microseconds.

				@param uResendTime [in] Specify the resend time in microseconds. Default value is 1 * 1000 * 1000(1 second).

				@remarks If parameter uResendTime equal zero, will reset resend time to default.
			*/
			GAIA::GVOID SetResendTime(const GAIA::U64& uResentTime);

			/*!
				@brief Get SUDPSocket datagram resend time in microseconds.

				@return Return SUDPSocket datagram resend time in microseconds.
			*/
			const GAIA::U64& GetResendTime() const;

			/*!
				@brief Set SUDPSocket datagram rerecv time in microseconds.

				@param uRerecvTime [in] Specify the rerecv time in microseconds. Default value is 10 * 1000(10 milliseconds).

				@remarks If parameter uRerecvTime equal zero, will reset rerecv time to default.
			*/
			GAIA::GVOID SetRerecvTime(const GAIA::U64& uRerecvTime);

			/*!
				@brief Get SUDPSocket datagram rerecv time in microseconds.

				@return Return SUDPSocket datagram rerecv time in microseconds.
			*/
			const GAIA::U64& GetRerecvTime() const;

			/*!
				@brief Set SUDPSocket link recycle time in microseconds.

				@param uRecycleTime [in] Specify the recycle time in microseconds. Default value is 16 * 1000 * 1000(16 seconds).

				@remarks If parameter uRecycleTime equal zero, will reset recycle time to default.
			*/
			GAIA::GVOID SetRecycleTime(const GAIA::U64& uRecycleTime);

			/*!
				@brief Get SUDPSocket datagram recycle time in microseconds.

				@return Return SUDPSocket datagram recycle time in microseconds.
			*/
			const GAIA::U64& GetRecycleTime() const;

			/*!
				@brief Send a udp datagram to peer.

				@param addr [in] Specify a network address to send.

				@param p [in] Specify a data buffer to send.

				@param nSize [in] Specify the data buffer's size in bytes.

				@return Return the data's size which had be sent.\n
					If failed, will return -1.\n

				@exception
					GAIA::ECT::EctInvalidParam If addr is invalid or p is GNIL or nSize below equal zero.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@remarks Actually, this function not send any data by socket.
					 The data will be sent when user call SUDPSocket::Execute.
			*/
			GAIA::N32 SendTo(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize);

			/*!
				@brief Recv a udp datagram from peer.

				@param addr [inout] As a input parameter, it specify a network address to receive data, as a output parameter, it used for saving the source network address of the received datagram.

				@param p [out] Used for saving the datagram.

				@param nSize [in] Specify the receive buffer's size in bytes.

				@return
					If there is not exist any datagram match parameter addr or the datagram can't continued to next serial number, return 0.\n
					If the buffer is not enough for save the datagram, return GINVALID.\n

				@exception
					GAIA::ECT::EctInvalidParam If p is GNIL or nSize below equal zero.

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@remarks Actually, this function not recv any data by socket.
					The data will be recv when user call SUDPSocket::Execute.
			*/
			GAIA::N32 RecvFrom(GAIA::NETWORK::Addr& addr, GAIA::GVOID* p, GAIA::N32 nSize);

			/*!
				@brief Get socket file descriptor.

				@return Return the socket file descriptor.
			*/
			GAIA::N32 GetFD() const;

			/*!
				@brief Get socket global address.

				@param addr [out] Used for saving the global address of current socket.

				@return
					If success return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL GetGlobalAddress(GAIA::NETWORK::Addr& addr);

			/*!
				@brief Get socket local address.

				@param addr [out] Used for saving the local address of current socket.

				@return
					If success return GAIA::True, or return GAIA::False.
			*/
			GAIA::BL GetLocalAddress(GAIA::NETWORK::Addr& addr);

			/*!
				@brief Execute the SUDPSocket.

				@param uDeltaTime [in] Delta time in microsecond between two SUDPSocket::Execute function called.

				@param bSend [in] Execute flush send buffer or not, it means datagram which be filled cache by SDUPSocket::SendTo will be socket-send.

				@param bRecv [in] Execute flush recv buffer or not, it means datagram which from peer socket will be cached to RAM container, and after that the moduler-user could call RecvFrom to get data only.

				@param bResend [in] Execute resend or not, then some safe datagram not be respondence, it will be resend by time-strategy.

				@param bRerecv [in] Execute receive notify back operation.

				@param bTimeout [in] Execute timeout link dispatch.

				@return
					If there are some datagram be sent or received

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@remarks
					The moduler-user could call this function in other threads for performance optimize.\n
					There is no effect for performance optimize when call this function in multi threads with same parameter(bSend and bRecv and bResend) at same time.
						But you can create three thread, one to send(with parameter bSend = GAIA::True), one to recv(with parameter bRecv = GAIA::True),
						and one to resend(with parameter bResend = GAIA::True), these three thread could improve the performance.\n
					This function could blocked when origin socket send and recv blocked(When origin socket is a block able socket).\n
			*/
			GAIA::BL Execute(GAIA::U64 uDeltaTime, GAIA::BL bSend = GAIA::True, GAIA::BL bRecv = GAIA::True, GAIA::BL bResend = GAIA::True, GAIA::BL bRerecv = GAIA::True, GAIA::BL bTimeout = GAIA::True);

			/*!
				@brief Reset the connection with a network address.

				@param pAddr [in] Specify the connection which will be reset, if GNIL means all connection will be reset.

				@param bSend [in] Specify reset connection's send context or not.

				@param bRecv [in] Specify reset connection's recv context or not.

				@return If sucecss, return GAIA::True, or will return GAIA::False.

				@exception
					GAIA::ECT::EctInvalidParam If pAddr not GNIL and it is a invalid network address.

				@exception
			*/
			GAIA::BL ResetConnection(const GAIA::NETWORK::Addr* pAddr = GNIL, GAIA::BL bSend = GAIA::True, GAIA::BL bRecv = GAIA::True);

			/*!
				@brief Get socket state.

				@return Return socket state.
			*/
			const GAIA::NETWORK::SUDPSocket::State& GetState() const;

		public:
			/*!
				@brief On data sent callback.

					When some datagram will be sent, this function will be called, then the moduler tell datagram's serial number via this function callback.

				@param addr [in] Specify which network address will be sended to.

				@param p [in] Specify the datagram.

				@param nSize [in] Specify the datagram's size in bytes.

				@param lSerial [out] Used for saving datagram's serial number.

				@return
					If datagram which specified by parameter is not a serial able udp datagram(unsafe datagram), the moduler-user will return GAIA::False.\n
					If datagram which specified by parameter is a serial able udp datagram(safe datagram), the moduler-user should analyze the serial number and save it to parameter lSerial, then return GAIA::True.\n

				@exception
					GAIA::ECT::EctIllegal If socket is not created.

				@remarks
					If you overwrite this virtual function, you must overwrite SUDPSocket::OnSent, SUDPSocket::OnRecv, SDUPSocket::OnBack, SUDPSocket::OnRecvBack together,
					And implement serial composite and decomposite work in these overwrite virtual funcitons.\n
					If you not overwrite any of SUDPSocket::OnSent or SUDPSocket::OnRecv or SUDPSocket::OnBack or SUDPSocket::OnRecvBack,
					current socket will like a normal(original) udp socket.\n
					This funciton will be called when SUDPSocket::SendTo be called.\n
			*/
			virtual GAIA::BL OnSend(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N64& lSerial){return GAIA::False;}

			/*!
				@brief On data recv callback.

					When SUDPSocket receive a datagram, this function will be called, the moduler-user should analyze the serial number to SUDPSocket.

				@param addr [in] Specify where the datagram come from.

				@param p [in] Specify the datagram.

				@param nSize [in] Specify the datagram's size in bytes.

				@param lSerial [out] Used for saving datagram's serial number.

				@return
					If this datagram is not a serial able udp datagram(unsafe datagram), the moduler-user will return GAIA::False.\n
					If this datagram is a serial able udp datagram(safe datagram), the moduler-user should analyze the serial number and save it to parameter lSerial, then return GAIA::True.\n

				@remarks
					If you overwrite this virtual function, you must overwrite SUDPSocket::OnSent, SUDPSocket::OnRecv, SDUPSocket::OnBack, SUDPSocket::OnRecvBack together,
					And implement serial composite and decomposite work in these overwrite virtual funcitons.\n
					If you not overwrite any of SUDPSocket::OnSent or SUDPSocket::OnRecv or SUDPSocket::OnBack or SUDPSocket::OnRecvBack,
					current socket will like a normal(original) udp socket.\n
					This funciton will be called when SUDPSocket::Execute be called.\n
			*/
			virtual GAIA::BL OnRecv(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N64& lSerial){return GAIA::False;}

			/*!
				@brief On notify data-received to other side callback.

					When SUDPSocket need notify a datagram to tell peer "Some datagram had received", this function will be called.\n
					And then the moduler-user will composite a respondence datagram to the buffer.

				@param addr [in] Specify which network address will be sended to.

				@param lBeginSerial [in] Specify the begin serial number.

				@param lEndSerial [in] Specify the end serial number.

				@param p [out] Used for saving datagram.

				@param nSize [in] Specify max size of datagram buffer.

				@param nResultSize [out] Used for saving the result datagram size.

				@return
					If buffer is enough, the moduler-user must save respondence datagram to the buffer, and return GAIA::True.
					If buffer is not enough, return GAIA::False.

				@remarks
					About index of parameter lEndSerial, it is included index. So it is [lBeginSerial, lEndSerial] but not [lBeginSerial, lEndSerial).
					If you overwrite this virtual function, you must overwrite SUDPSocket::OnSent, SUDPSocket::OnRecv, SDUPSocket::OnBack, SUDPSocket::OnRecvBack together,
					And implement serial composite and decomposite work in these overwrite virtual funcitons.\n
					If you not overwrite any of SUDPSocket::OnSent or SUDPSocket::OnRecv or SUDPSocket::OnBack or SUDPSocket::OnRecvBack,
					current socket will like a normal(original) udp socket.\n
					This funciton will be called when SUDPSocket::Execute be called.\n
			*/
			virtual GAIA::BL OnBack(const GAIA::NETWORK::Addr& addr, const GAIA::N64& lBeginSerial, const GAIA::N64& lEndSerial, GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N32& nResultSize){return GAIA::False;}

			/*!
				@brief On recv data-received from other side callback.

					When SUDPSocket receive a receive-respondence datagram, this function will be called, the moduler-user will analyze the datagram and tell SUDPSocket begin-serial-number and end-serial-number.

				@param addr [in] Specify where datagram come from.

				@param p [in] Specify the datagram.

				@param nSize [in] Specify the datagram's size.

				@param lBeginSerial [out] Used for saving datagram's begin serial number.

				@param lEndSerial [out] Used for saving datagram's end serial number.

				@return
					If analyze receive-respondence datagram successfully, return GAIA::True, or return GAIA::False.\n
					If analyze receive-respondence datagram failed, it means some udp peer sent a unsafe udp datagram or sent a illegal datagram to it.\n

				@remarks
					About index of parameter lEndSerial, it is included index. So it is [lBeginSerial, lEndSerial] but not [lBeginSerial, lEndSerial).
					If you overwrite this virtual function, you must overwrite SUDPSocket::OnSent, SUDPSocket::OnRecv, SDUPSocket::OnBack, SUDPSocket::OnRecvBack together,
					And implement serial composite and decomposite work in these overwrite virtual funcitons.\n
					If you not overwrite any of SUDPSocket::OnSent or SUDPSocket::OnRecv or SUDPSocket::OnBack or SUDPSocket::OnRecvBack,
					current socket will like a normal(original) udp socket.\n
					This funciton will be called when SUDPSocket::Execute be called.\n
			*/
			virtual GAIA::BL OnRecvBack(const GAIA::NETWORK::Addr& addr, const GAIA::GVOID* p, GAIA::N32 nSize, GAIA::N64& lBeginSerial, GAIA::N64& lEndSerial){return GAIA::False;}

		public:
			/*!
				@brief On SUDPSocket timeout time.

					When SUDPSocket had't receive any receive-respondence datagram from peer which should be send back from peer,
					and it cause moduler-user can't receive any datagram(By SUDPSocket::RecvFrom call) for timeout time this function will be called.

				@param addr [in] Specify which network address will timeout.

				@param bLostConnection [in] If GAIA::True means the link is timeout by safe-datagram checkup(Maybe network bad), If GAIA::False means the link is not used for a long time(Recycle time).
			*/
			virtual GAIA::GVOID OnTimeout(const GAIA::NETWORK::Addr& addr, GAIA::BL bLostConnection){}

		private:
			class Node : public GAIA::Base
			{
			public:
				GINL GAIA::N32 compare(const Node& src) const
				{
					GAIA::N32 nCmp = addr.compare(src.addr);
					if(nCmp != 0)
						return nCmp;
					if(lSerial == GINVALID && src.lSerial == GINVALID)
					{
						if(this < &src)
							return -1;
						else if(this > &src)
							return +1;
					}
					else if(lSerial == GINVALID && src.lSerial != GINVALID)
						return -1;
					else if(lSerial != GINVALID && src.lSerial == GINVALID)
						return +1;
					else
					{
						if(lSerial < src.lSerial)
							return -1;
						else if(lSerial > src.lSerial)
							return +1;
					}
					return 0;
				}
				GCLASS_COMPARE_BYCOMPARE(Node)
			public:
				GAIA::U64 uFirstTime; // The time when first sent or recv. In microseconds.
				GAIA::U64 uLastTime; // The time when last sent(include resend) or recv. In microseconds.
				GAIA::N64 lSerial; // GINVALID means it is not a serial able datagram.
				GAIA::NETWORK::Addr addr;
				GAIA::CTN::Buffer* pBuf;
			};
			typedef GAIA::CTN::Set<GAIA::CTN::Ref<Node> > __NodeSetType;
			class Link : public GAIA::Base
			{
			public:
				GINL GAIA::N32 compare(const Link& src) const
				{
					return addr.compare(src.addr);
				}
				GCLASS_COMPARE_BYCOMPARE(Link)
			public:
				GAIA::U64 uLastTime; // In microseconds.
				GAIA::NETWORK::Addr addr; // Peer address.
				__NodeSetType nodes;
				GAIA::N64 lNextSerial;
				GAIA::U64 lNeedBackSerialBegin;
				GAIA::U64 lNeedBackSerialFlags; // Must cache 64 difference serial.
			};

		private:
			GINL GAIA::GVOID init()
			{
				m_state.reset();
				m_uTimeout = DEFAULT_TIMEOUT_TIME;
				m_uResendTime = DEFAULT_RESEND_TIME;
				m_uRerecvTime = DEFAULT_RERECV_TIME;
				m_uRecycleTime = DEFAULT_RECYCLE_TIME;
				m_uLastExecuteTime = 0;
			}

		private:
			typedef GAIA::CTN::Queue<Node*> __NodeQueueType;
			typedef GAIA::CTN::Set<GAIA::CTN::Ref<Link> > __LinkSetType;
			typedef GAIA::CTN::Vector<Node*> __NodeVectorType;
			typedef GAIA::CTN::Vector<GAIA::NETWORK::Addr> __AddrVectorType;
			typedef GAIA::CTN::Vector<Link*> __LinkVectorType;
			typedef GAIA::CTN::Set<GAIA::NETWORK::Addr> __AddrSetType;

		private:

			/* State. */
			GAIA::NETWORK::SUDPSocket::State m_state;

			/* Socket. */
			GAIA::NETWORK::Socket m_sock;

			/* Work parameter. */
			GAIA::U64 m_uTimeout;
			GAIA::U64 m_uResendTime;
			GAIA::U64 m_uRerecvTime;
			GAIA::U64 m_uRecycleTime;
			GAIA::U64 m_uLastExecuteTime;

			/* Send cache. */
			GAIA::SYNC::Lock m_lrSendCache;
			__NodeVectorType m_sendcache;

			/* Send history. */
			GAIA::SYNC::Lock m_lrSent;
			__LinkSetType m_sentlinks_byaddr; // Ordered by network address(Node::addr).

			/* Recv cache. */
			GAIA::SYNC::Lock m_lrRecv;
			__LinkSetType m_recvlinks_byaddr; // Ordered by network address(Link::addr).
			__LinkSetType m_recvablelinks_byaddr; // Ordered by network address(Link::addr).

			/* Object pools. */
			GAIA::SYNC::Lock m_lrBufferPool;
			GAIA::CTN::Pool<GAIA::CTN::Buffer> m_bufpool;
			GAIA::SYNC::Lock m_lrNodePool;
			GAIA::CTN::Pool<Node> m_nodepool;
			GAIA::SYNC::Lock m_lrLinkPool;
			GAIA::CTN::Pool<Link> m_linkpool;

			/* Execute mutex. */
			GAIA::SYNC::Lock m_lrExecuteSend;
			GAIA::SYNC::Lock m_lrExecuteRecv;
			GAIA::SYNC::Lock m_lrExecuteResend;
			GAIA::SYNC::Lock m_lrExecuteRerecv;
			GAIA::SYNC::Lock m_lrExecuteTimeout;

			/* Temporary container. */
			__NodeVectorType m_listTempNodeForSend0;
			__LinkVectorType m_listTempLinkForSend1;
			__NodeVectorType m_listTempNodeForRecv0;
			__LinkVectorType m_listTempLinkForRecv1;
			__NodeVectorType m_listTempNodeForExecuteSend0;
			__NodeVectorType m_listTempNodeForExecuteSend1;
			__AddrVectorType m_listTempAddrForExecuteTimeout0;
			GAIA::CTN::Buffer m_recvbuf;

			/* Swap container. */
			GAIA::SYNC::Lock m_lrDisconnected;
			__AddrSetType m_listDisconnected;
		};
	}
}

#endif
