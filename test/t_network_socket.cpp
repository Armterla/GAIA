#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	// Socket.
	class SocketAcceptThread : public GAIA::THREAD::Thread
	{
	public:
		virtual GAIA::GVOID Run()
		{
			GTRY
			{
				GAIA::NETWORK::Socket socknew;
				sock->Accept(socknew);
				socknew.Send("HelloKitty", 10);
			}
			GCATCH(Network)
			{
				GSTM << e.GetErrorText() << " Error=" << e.GetError() << " OSError=" << e.GetOSError() << " Exception!" << "\n";
			}
		}
	public:
		GAIA::NETWORK::Socket* sock;
	};

	extern GAIA::GVOID t_network_socket(GAIA::LOG::Log& logobj)
	{
		// Socket test.
		GTRY
		{
			GAIA::NETWORK::Addr addr1;
			addr1.fromstring("127.0.0.1:8012");
			char szTemp[1024];
			addr1.tostring(szTemp);
			if(GAIA::ALGO::gstrcmp(szTemp, "127.0.0.1:8012") != 0)
				TERROR;

			GAIA::NETWORK::Socket sock1;
			sock1.Create(GAIA::NETWORK::Socket::SOCKET_TYPE_STREAM);
			sock1.SetOption(GAIA::NETWORK::Socket::SOCKET_OPTION_REUSEADDR, GAIA::True);
			sock1.Bind(addr1);
			sock1.Listen();

			SocketAcceptThread thd;
			thd.sock = &sock1;
			thd.Start();

			GAIA::NETWORK::Socket sock2;
			sock2.Create(GAIA::NETWORK::Socket::SOCKET_TYPE_STREAM);
			sock2.Connect(addr1);
			sock2.Recv(szTemp, sizeofarray(szTemp));

			GAIA::SYNC::gsleep(100);

			GTRY
			{
				sock2.Shutdown();
			}
			GCATCH(Network)
			{
				e.SetDispatched(GAIA::True);
			}

			sock1.Close();
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
