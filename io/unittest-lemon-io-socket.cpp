#include <lemonxx/sys/sys.hpp>
#include <lemonxx/io/io_device.hpp>
#include <lemonxx/io/basic_socket.hpp>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/io/address.hpp>
#include <lemonxx/io/endpoint.hpp>
#include <lemonxx/io/resolver.hpp>
#include <lemonxx/io/tcp.hpp>
#include <lemonxx/io/udp.hpp>
#include <iostream>
namespace lemon{namespace io{namespace test{

	/*struct SocketUnittest{};

	void Client(ip::endpoint remote)
	{
		ip::endpoint local = remote;

		random_t rand;

		if(local.ipv4())
		{
			local = local.v4().port((unsigned short)rand.next());
		}
		else
		{
			local = local.v6().port((unsigned short)rand.next());
		}

		ip::tcp::client c(local);

		c.connect(remote);
	}

	LEMON_UNITTEST_CASE(SocketUnittest,basic_socketTest)
	{
		resolver_iterator end;

		for(resolver_iterator iter("localhost","1812");iter != end; ++ iter)
		{
			ip::tcp::server s(iter->ai_addr);

			s.listen(SOMAXCONN);

			lemon::thread_t t(lemon::bind(&Client,ip::endpoint(iter->ai_addr)));

			tuple<ip::tcp::connection::handle_type,ip::endpoint> result = s.accept();

			ip::tcp::connection conn(get<0>(result));

			ip::endpoint ep = get<1>(result);

			if(ep.ipv4())
			{
				std::cout << "accept connection from :" << ep.v4().address().string() << ":" << ep.v4().port()  << std::endl;
			}
			else
			{
				std::cout << "accept connection from :" << ep.v6().address().string() << ":" << ep.v6().port()  << std::endl;
			}
			
			conn.shutdown(SD_BOTH);

			t.join();
		}
	}

	void AsyncReceive(
		io_device &device,
		byte_t * buffer,
		size_t	numberOfBytesTransferred,
		const LemonErrorInfo &errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		std::cout << std::string(buffer,buffer + numberOfBytesTransferred) << std::endl;

		device.stop();
	}

	void AsyncAccept(
		io_device &device,
		ip::endpoint expect,
		ip::tcp::connection& conn,
		ip::endpoint &remote,
		size_t	numberOfBytesTransferred,
		const LemonErrorInfo &errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(0 == numberOfBytesTransferred);

		LEMON_CHECK(expect == remote);

		byte_t * buffer = new byte_t[256];

		conn.async_receive(buf(buffer,256),bind(&AsyncReceive,ref(device),buffer,_0,_1));
	}

	void AsyncSend(
		size_t required,
		size_t	numberOfBytesTransferred,
		const LemonErrorInfo &errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(required == numberOfBytesTransferred);
	}

	void AsyncConnect(
		ip::tcp::client& c,
		size_t	numberOfBytesTransferred,
		const LemonErrorInfo &errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(0 == numberOfBytesTransferred);

		const static char * message = "hello world!!!!";

		c.async_send(cbuf(message),bind(&AsyncSend,strlen(message),_0,_1));
	}

	LEMON_UNITTEST_CASE(SocketUnittest,asyncTcpTest)
	{
		resolver_iterator end;

		io_device device;

		for(resolver_iterator iter("localhost","1813");iter != end; ++ iter)
		{
			ip::tcp::server s(iter->ai_addr,device);

			ip::endpoint local(iter->ai_addr);

			random_t rand;

			if(local.ipv4())
			{
				local = local.v4().port((unsigned short)rand.next());
			}
			else
			{
				local = local.v6().port((unsigned short)rand.next());
			}

			s.listen(SOMAXCONN);

			ip::tcp::connection conn(iter->ai_family,device);

			ip::endpoint remote;

			s.async_accept(conn,remote,lemon::bind(&AsyncAccept,ref(device),local,ref(conn),ref(remote),_0,_1));

			ip::tcp::client c(local,device);

			c.async_connect(iter->ai_addr,bind(&AsyncConnect,ref(c),_0,_1));

			device.run();
		}
	}

	LEMON_UNITTEST_CASE(SocketUnittest,addressTest)
	{
		in_addr address = {0,0,0,0};

		char buffer[256];

		error_info errorCode;

		size_t length = LemonAddressToString(AF_INET,&address,sizeof(address),buffer,sizeof(buffer),&errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(length == sizeof("0.0.0.0") - 1);

		std::cout << buffer << std::endl;

		LemonAddressFromString(AF_INET,"127.0.0.1",sizeof("127.0.0.1"),&address,sizeof(address),&errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		in_addr empty = {127,0,0,1};

		LEMON_CHECK(memcmp(&empty,&address,sizeof(empty)) == 0);

		in_addr6 addressv6 = {0};

		addressv6.u.Byte[15] = 1;

		length = LemonAddressToString(AF_INET6,&addressv6,sizeof(addressv6),buffer,sizeof(buffer),&errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		std::cout << buffer << std::endl;

		addressv6.u.Byte[12] = 127;

		length = LemonAddressToString(AF_INET6,&addressv6,sizeof(addressv6),buffer,sizeof(buffer),&errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		std::cout << buffer << std::endl;

		addressv6.u.Word[5] = 0xffff;

		length = LemonAddressToString(AF_INET6,&addressv6,sizeof(addressv6),buffer,sizeof(buffer),&errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		std::cout << buffer << std::endl;

		LemonAddressFromString(AF_INET6,"::127.0.0.1",sizeof("::127.0.0.1"),&addressv6,sizeof(addressv6),&errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		byte_t local[16] = {0,0,0,0,0,0,0,0,0,0,0,0,127,0,0,1};

		LEMON_CHECK(memcmp(&local,&addressv6,sizeof(local)) == 0);

		LemonAddressFromString(AF_INET6,"fe80::91a3:79ef:d0a1:7e8f",sizeof("fe80::91a3:79ef:d0a1:7e8f"),&addressv6,sizeof(addressv6),&errorCode);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		byte_t local1[16] = {0xfe,0x80,0,0,0,0,0,0,0x91,0xa3,0x79,0xef,0xd0,0xa1,0x7e,0x8f};

		LEMON_CHECK(memcmp(&local1,&addressv6,sizeof(local1)) == 0);
	}

	LEMON_UNITTEST_CASE(SocketUnittest,cxxaddressTest)
	{
		ip::v4::address address = ip::v4::address::from("192.168.1.1");

		in_addr address1 = {192,168,1,1};

		LEMON_CHECK(address == address1);

		LEMON_CHECK(address.string() == "192.168.1.1");

		ip::v6::address addressv6 = ip::v6::address::from("fe80::91a3:79ef:d0a1:7e8f");

		byte_t local1[16] = {0xfe,0x80,0,0,0,0,0,0,0x91,0xa3,0x79,0xef,0xd0,0xa1,0x7e,0x8f};

		LEMON_CHECK((*(in_addr6*)local1) == addressv6);

		LEMON_CHECK(addressv6.string() == "fe80::91a3:79ef:d0a1:7e8f");

		LEMON_CHECK(ip::v6::address::from("::127.0.0.1").string() == "::127.0.0.1");

		LEMON_CHECK(ip::v6::address::from("::ffff:127.0.0.1").string() == "::ffff:127.0.0.1");

		ip::address address2 = addressv6;

		LEMON_CHECK(address2.ipv6());

		LEMON_CHECK(address2.v6() == addressv6);

		address2 = address;

		LEMON_CHECK(address2.ipv4());

		LEMON_CHECK(address2.v4() == address);
	}

	LEMON_UNITTEST_CASE(SocketUnittest,ResovlerTest)
	{
		resolver_iterator end;

		for(resolver_iterator iter("localhost","1812");iter != end; ++ iter)
		{
			ip::endpoint ep(iter->ai_addr);

			if(ep.ipv4())
			{
				std::cout << ep.v4().address().string() << ":" << ep.v4().port()  << std::endl;
			}
			else
			{
				std::cout << ep.v6().address().string() << ":" << ep.v6().port()  << std::endl;
			}
		}
	}


	LEMON_UNITTEST_CASE(SocketUnittest,udpTest)
	{
		resolver_iterator end;

		const char * message =  "hello world!!!";

		for(resolver_iterator iter("localhost","1812");iter != end; ++ iter)
		{
			ip::udp::socket s(iter->ai_addr);

			ip::endpoint local(iter->ai_addr);

			random_t rand;

			if(local.ipv4())
			{
				local = local.v4().port((unsigned short)rand.next());
			}
			else
			{
				local = local.v6().port((unsigned short)rand.next());
			}

			ip::udp::socket c(local);

			c.sendto(cbuf(message),iter->ai_addr);

			byte_t buffer[256];

			ip::endpoint remote;

			size_t length = s.receivefrom(buf(buffer),remote);

			LEMON_CHECK(length == strlen(message));

			LEMON_CHECK(memcmp(buffer,message,length) == 0);

			LEMON_CHECK(remote == local);
		}
	}

	void AsyncSendTo(
		size_t expect,
		size_t	numberOfBytesTransferred,
		const LemonErrorInfo &errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(numberOfBytesTransferred == expect);
	}

	void AsyncReceiveFrom(
		io_device &device,
		const char * message,
		byte_t * buffer,
		ip::endpoint expect,
		ip::endpoint & remote,
		size_t	numberOfBytesTransferred,
		const LemonErrorInfo &errorCode)
	{
		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		LEMON_CHECK(numberOfBytesTransferred == strlen(message));

		LEMON_CHECK(memcmp(buffer,message,numberOfBytesTransferred) == 0);

		LEMON_CHECK(remote == expect);

		device.stop();
	}

	LEMON_UNITTEST_CASE(SocketUnittest,async_udpTest)
	{
		resolver_iterator end;

		io_device device;

		const char * message =  "hello world!!!";

		for(resolver_iterator iter("localhost","1812");iter != end; ++ iter)
		{
			ip::udp::socket s(iter->ai_addr,device);

			ip::endpoint local(iter->ai_addr);

			random_t rand;

			if(local.ipv4())
			{
				local = local.v4().port((unsigned short)rand.next());
			}
			else
			{
				local = local.v6().port((unsigned short)rand.next());
			}

			ip::udp::socket c(local,device);

			c.async_sendto(cbuf(message),iter->ai_addr,bind(AsyncSendTo,strlen(message),_0,_1));

			byte_t buffer[256];

			ip::endpoint remote;

			s.async_receivefrom(buf(buffer),remote,bind(AsyncReceiveFrom,ref(device),message,buffer,local,ref(remote),_0,_1));

			device.run();
		}
	}*/

}}}