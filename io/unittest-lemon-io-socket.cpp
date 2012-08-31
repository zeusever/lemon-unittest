#include <lemonxx/io/tcp.hpp>
#include <lemonxx/io/socket.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{
	
	class SocketUnittest{};

	typedef basic_socket<SOCK_STREAM,IPPROTO_TCP,net::endpoint> socket_type;

	LEMON_UNITTEST_CASE(SocketUnittest,SocketObjectAllcTest)
	{
		

		io_service service;

		socket_type v4(AF_INET,service);
	}


	LEMON_UNITTEST_CASE(SocketUnittest,SocketObjectAllcPerformanceTest)
	{

		io_service service;

		socket_type sockets[10000];

		for(size_t i = 0; i < sizeof(sockets)/sizeof(socket_type); ++ i)
		{
			sockets[i].open(AF_INET,service);
		}
	}

	LEMON_UNITTEST_CASE(SocketUnittest,SocketBindTest)
	{
		net::endpoint ep(net::resolver_iterator("127.0.0.1","15231")->ai_addr);

		io_service service;

		socket_type v4(AF_INET,service);

		v4.bind(ep);
	}

	LEMON_UNITTEST_CASE(SocketUnittest,SocketShutdownTest)
	{
		typedef basic_socket<SOCK_DGRAM,IPPROTO_UDP,net::endpoint> socket_type;

		io_service service;

		socket_type v4(AF_INET,service);

		v4.shutdown(SD_BOTH);
	}

	LEMON_UNITTEST_CASE(SocketUnittest,SockNameTest)
	{
		net::endpoint ep(net::resolver_iterator("127.0.0.1","15231")->ai_addr);

		io_service service;

		socket_type v4(AF_INET,service);

		v4.bind(ep);

		LEMON_CHECK(ep == v4.sockname());
	}
}}}