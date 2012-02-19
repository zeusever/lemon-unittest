#include <lemonxx/sys/sys.hpp>
#include <lemonxx/io/io_device.hpp>
#include <lemonxx/io/basic_socket.hpp>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/io/address.hpp>
#include <lemonxx/io/endpoint.hpp>
#include <lemonxx/io/resolver.hpp>

#include <iostream>
namespace lemon{namespace io{namespace test{

	struct SocketUnittest{};

	LEMON_UNITTEST_CASE(SocketUnittest,basic_socketTest)
	{
		basic_socket<SOCK_STREAM,IPPROTO_TCP> s;
	}

	LEMON_UNITTEST_CASE(SocketUnittest,addressTest)
	{
		in_addr address = {0,0,0,0};

		char buffer[256];

		LEMON_DECLARE_ERRORINFO(errorCode);

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

}}}