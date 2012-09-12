#include <lemonxx/io/tcp.hpp>
#include <lemonxx/io/udp.hpp>
#include <lemonxx/io/socket.hpp>
#include <lemonxx/function/bind.hpp>
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

	class TcpUnittest{};

	void Client(net::endpoint remote)
	{
		net::endpoint ep(net::resolver_iterator("127.0.0.1","15237")->ai_addr);

		io_service service;

		ep.port(random_t().next() % 65535);

		tcp::client client(ep,service);

		client.connect(remote);

		client.send(cbuf("hello !!!!"));

		char buffer[512] = {0};

		client.receive(buf(buffer));

		std::cout << "echo :" << buffer << std::endl;
	}

	LEMON_UNITTEST_CASE(TcpUnittest,AcceptTest)
	{
		net::endpoint ep(net::resolver_iterator("127.0.0.1","15231")->ai_addr);

		io_service service;

		tcp::server server(ep,service);

		server.listen(SOMAXCONN);

		thread_t  client(lemon::bind(&Client,ep));

		net::endpoint remote;

		tcp::connection cnn(server.accept(remote));

		char buffer[512] = {0};

		cnn.receive(buf(buffer));

		std::cout << "recv :" << buffer << std::endl;

		cnn.send(cbuf("world !!!"));

		client.join();
	}

	struct Connection
	{
		tcp::connection conn; 
		
		net::endpoint remote; 

		char buffer[255];
	};

	void AsyncSend(Connection & conn,size_t /*numberOfBytesTransferred*/ , const LemonErrorInfo & errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		conn.conn.close();
	}

	void AsyncReceive(Connection & conn,size_t /*numberOfBytesTransferred*/ , const LemonErrorInfo & errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));
		
		conn.conn.async_send(cbuf("hello world"),lemon::bind(&AsyncSend,ref(conn),_0,_1));

	}

	void AsyncAccept(Connection & conn,tcp::connection::wrapper_type io,size_t numberOfBytesTransferred , const LemonErrorInfo & errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(numberOfBytesTransferred == 0);

		conn.conn.reset(io);

		conn.conn.async_receive(buf(conn.buffer),lemon::bind(&AsyncReceive,ref(conn),_0,_1));
	}

	LEMON_UNITTEST_CASE(TcpUnittest,OverlappedIOTest)
	{
		const size_t counter = 10000;

		atomic_t index = 0;

		net::endpoint ep(net::resolver_iterator("127.0.0.1","15237")->ai_addr);

		io_service service;

		service.start(1);

		tcp::server server(ep,service);

		Connection connections[counter];

		tcp::client clients[counter];

		server.listen(SOMAXCONN);

		for(size_t i = 0; i < counter ; ++ i)
		{
			server.async_accept(connections[i].remote,lemon::bind(&AsyncAccept,ref(connections[i]),_0,_1,_2));
		}

		char buffer[512] = {0};

		for(size_t i = 0; i < counter ; ++ i)
		{
			clients[i].open(ep.af(),service);

			clients[i].connect(ep);

			clients[i].send(cbuf("hello world"));

			clients[i].receive(buf(buffer));

			clients[i].close();
		}

		service.stop();

		service.join();
	}
}}}