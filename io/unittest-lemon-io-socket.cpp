#include <lemonxx/io/udp.hpp>
#include <lemonxx/io/tcp.hpp>
#include <lemon/io/assembly.h>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/unittest/unittest.hpp>

#define UNITTEST_TCP_MAX_CLIENT			100

namespace lemon{namespace io{namespace test{

	class SocketUnittest{};

	void RecvFrom(io_service & service,size_t numberOfBytesTransferred , const LemonErrorInfo & errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(numberOfBytesTransferred != 0);

		service.stop();
	}

	LEMON_UNITTEST_CASE(SocketUnittest,SocketAsyncRecvFromTest)
	{
		io_service service;

		lemon::net::endpoint ep(lemon::net::resolver_iterator("127.0.0.1","1812")->ai_addr);

		ip::udp::socket s(ep,service), c(ep.af(),service);

		char recvBuffer[1024] = {0};

		lemon::net::endpoint remote;

		std::string message = "hello the async io world!!!!";

		s.async_recvfrom(lemon::buf(recvBuffer),remote,lemon::bind(&RecvFrom,ref(service),_0,_1));

		c.sendto(lemon::cbuf(message),ep);

		service.dispatch();

		LEMON_CHECK(message == recvBuffer);
	}

	void Cancel(size_t /*numberOfBytesTransferred */, const LemonErrorInfo & errorCode)
	{
		LEMON_CHECK(lemon_true == LEMON_ERRORINOF_EQ(errorCode.Error,LEMON_IO_ASYNC_CANCEL));
	}

	LEMON_UNITTEST_CASE(SocketUnittest,SocketCancelTest)
	{
		io_service service;

		lemon::net::endpoint ep(lemon::net::resolver_iterator("127.0.0.1","1812")->ai_addr);

		char recvBuffer[1024] = {0};

		lemon::net::endpoint remote;

		for(size_t i = 0; i < 2; ++ i)
		{
			ip::udp::socket s(ep,service);

			s.async_recvfrom(lemon::buf(recvBuffer),remote,lemon::bind(&Cancel,_0,_1));

			service.stop();

			service.reset();
		}
	}

	void SocketShutDownTestClient(ip::tcp::client & c, lemon::net::endpoint & remote,const std::string & message)
	{
		lemon::sleep(1000);

		c.connect(remote);

		c.send(lemon::cbuf(message));

		c.shutdown(SD_SEND);

		LEMON_UNITTEST_EXPECT_EXCEPTION(c.send(lemon::cbuf(message)),lemon::error_info);
	}

	LEMON_UNITTEST_CASE(SocketUnittest,SocketShutDownTest)
	{
		io_service service;

		lemon::net::endpoint ep(lemon::net::resolver_iterator("127.0.0.1","1812")->ai_addr);

		lemon::net::endpoint remote;

		ip::tcp::server s(ep,service);

		s.listen(SOMAXCONN);
		
		ip::tcp::client c(ep.af(),service);

		char recvBuffer[1024] = {0};

		std::string message = "hello the async io world!!!!";

		lemon::thread_t t(lemon::bind(&SocketShutDownTestClient,lemon::ref(c),lemon::ref(ep),lemon::cref(message)));

		ip::tcp::connection cnn(s.accept(remote));

		cnn.receive(lemon::buf(recvBuffer));

		cnn.shutdown(SD_RECEIVE);

		//LEMON_UNITTEST_EXPECT_EXCEPTION(cnn.receive(buf(recvBuffer)),lemon::error_info);

		t.join();
	}

	void SyncClient(ip::tcp::client & c, lemon::net::endpoint & remote,const std::string & message)
	{
		lemon::sleep(100);

		c.connect(remote);

		c.send(cbuf(message));
	}

	LEMON_UNITTEST_CASE(SocketUnittest,SyncTcpTest)
	{
		io_service service;

		lemon::net::endpoint ep(lemon::net::resolver_iterator("127.0.0.1","1812")->ai_addr);

		ip::tcp::server s(ep,service);

		s.listen(SOMAXCONN);

		lemon::net::endpoint remote;

		std::string message = "hello the async io world!!!!";

		for(size_t i = 0; i < 10; ++ i)
		{
			ip::tcp::client c(ep.af(),service);

			lemon::thread_t t(lemon::bind(&SyncClient,lemon::ref(c),lemon::ref(ep),lemon::cref(message)));

			ip::tcp::connection cnn(s.accept(remote));

			char recvBuffer[1024] = {0};

			cnn.receive(lemon::buf(recvBuffer));

			LEMON_CHECK(message == recvBuffer);

			t.join();
		}
	}

	void AsyncAccept(io_service &service,size_t & counter,ip::tcp::connection::wrapper_type, const LemonErrorInfo & errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		++ counter;

		if(counter == 2) service.stop();
	}

	void AsyncConnect(io_service &service,size_t & counter,size_t numberOfBytesTransferred , const LemonErrorInfo & errorCode)
	{
		LEMON_CHECK(0 == numberOfBytesTransferred);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		++ counter;

		if(counter == 2) service.stop();
	}

	LEMON_UNITTEST_CASE(SocketUnittest,AsyncAcceptConnectTest)
	{
		io_service service;

		lemon::net::endpoint ep(lemon::net::resolver_iterator("127.0.0.1","18121")->ai_addr);

		ip::tcp::server s(ep,service);

		s.listen(SOMAXCONN);

		lemon::net::endpoint remote;

		for(size_t i = 0; i < 10; ++ i)
		{
			size_t counter = 0;

			s.async_accept(remote,lemon::bind(&AsyncAccept,lemon::ref(service),lemon::ref(counter),_0,_1));

			ip::tcp::client c(ep.af(),service);

			c.async_connect(ep,lemon::bind(&AsyncConnect,lemon::ref(service),lemon::ref(counter),_0,_1));

			service.dispatch();

			service.reset();
		}
	}

	class TcpServer
	{
	public:

		TcpServer(io_service &service,lemon::net::endpoint ep)
			:socket(ep,service)
		{
			socket.listen(SOMAXCONN);

			socket.async_accept(remote,lemon::bind(&TcpServer::Accept,this,_0,_1));
		}

		void Cancel()
		{
			socket.cancel();
		}

	private:

		void Accept(ip::tcp::connection::wrapper_type io,const LemonErrorInfo & errorCode)
		{
			if(LEMON_SUCCESS(errorCode))
			{
				ip::tcp::connection *cnn = new ip::tcp::connection(io);

				socket.async_accept(remote,lemon::bind(&TcpServer::Accept,this,_0,_1));

				cnn->async_receive(lemon::buf(buffer),lemon::bind(&TcpServer::Receive,this,cnn,_0,_1));
			}
		}

		void Receive(ip::tcp::connection *cnn,size_t /*length*/,const LemonErrorInfo & errorCode)
		{
			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			cnn->async_send(lemon::cbuf("hello world!"),lemon::bind(&TcpServer::Send,this,cnn,_0,_1));
		}


		void Send(ip::tcp::connection *cnn,size_t length,const LemonErrorInfo & errorCode)
		{
			delete cnn;

			LEMON_CHECK(length == strlen("hello world!"));

			LEMON_CHECK(LEMON_SUCCESS(errorCode));
		}

	private:
		
		char					buffer[1024];

		ip::tcp::server			socket;

		lemon::net::endpoint	remote;
	};

	class TcpClient
	{
	public:
		TcpClient(io_service &service,size_t & counter,lemon::net::endpoint remote)
			:_counter(counter),_service(service),client(remote.af(),service)
		{
			client.connect(remote);

			client.async_send(lemon::cbuf("hello world!!!!!"),lemon::bind(&TcpClient::Send,this,_0,_1));
		}

		void Send(size_t /*length*/,const LemonErrorInfo & errorCode)
		{
			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			client.async_receive(lemon::buf(buffer),lemon::bind(&TcpClient::Receive,this,_0,_1));
		}

		void Receive(size_t /*length*/,const LemonErrorInfo & errorCode)
		{
			LEMON_CHECK(LEMON_SUCCESS(errorCode));

			++ _counter;

			if(_counter == UNITTEST_TCP_MAX_CLIENT) _service.stop();
		}
	private:

		size_t				&_counter;

		io_service			&_service;

		ip::tcp::client		client;

		char				buffer[1024];
	};

	LEMON_UNITTEST_CASE(SocketUnittest,AsyncTcpTest)
	{
		io_service service;

		lemon::net::endpoint ep(lemon::net::resolver_iterator("127.0.0.1","18112")->ai_addr);

		TcpServer server(service,ep);

		size_t counter = 0;

		for(size_t i =0; i < 100000; ++i)
		{
			TcpClient * clients[UNITTEST_TCP_MAX_CLIENT];

			for(size_t i =0; i <  UNITTEST_TCP_MAX_CLIENT; ++ i)
			{
				try
				{
					clients[i] = new TcpClient(service,counter,ep);
				}
				catch(const lemon::error_info &e)
				{
					std::cout << e << std::endl;
				}

			}

			service.dispatch();

			for(size_t i =0; i <  UNITTEST_TCP_MAX_CLIENT; ++ i)
			{
				delete clients[i];
			}
		}
	}
}}}

