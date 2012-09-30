#include <lemonxx/io/tcp.hpp>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{
	
	class SocketUnittest{};

	void SyncServer(net::endpoint ep,io_service & service)
	{
		
		tcp::server server(ep,service);

		server.listen(SOMAXCONN);

		net::endpoint remote;

		tcp::connection cnn(server.accept(remote));

		cnn.send(lemon::cbuf("hello world"));
		
	}

	LEMON_UNITTEST_CASE(SocketUnittest,SyncAPITest)
	{
		io_service service;

		net::resolver_iterator iter("127.0.0.1","18120");

		tcp::client client(iter->ai_family,service);

		net::endpoint remote(iter->ai_addr);

		lemon::thread_t server(lemon::bind(&SyncServer,remote,ref(service)));

		lemon::sleep(4000);

		std::cout << "timeout ..." << std::endl;

		client.connect(remote);

		char buffer[255] = {0};

		client.receive(lemon::buf(buffer));

		server.join();
	}
}}}