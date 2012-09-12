#include <lemonxx/sys/sys.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace test{
	
	class AddressUnittest{};

	LEMON_UNITTEST_CASE(AddressUnittest,Address_V4Test)
	{
		net::resolver_iterator iter("127.0.0.1","1812");

		LEMON_CHECK(iter != net::resolver_iterator());

		net::address_v4 v4(127,0,0,1);

		LEMON_CHECK(iter->ai_family == AF_INET);

		LEMON_CHECK(v4 == net::address_v4(((sockaddr_in*)iter->ai_addr)->sin_addr));

		LEMON_CHECK(v4.length() == sizeof(in_addr));

		LEMON_CHECK(v4 == net::address_v4("127.0.0.1"));

		LEMON_CHECK(v4.tostring() == "127.0.0.1");
	}
#ifdef LEMON_SUPPORT_IPV6

	LEMON_UNITTEST_CASE(AddressUnittest,Address_V6Test)
	{
		net::resolver_iterator iter("2002:7671:bfd4:a:fdec:30ff:85ce:3b43","1812");

		net::address_v6  v6("2002:7671:bfd4:a:fdec:30ff:85ce:3b43");

		LEMON_CHECK(iter != net::resolver_iterator());

		LEMON_CHECK(iter->ai_family == AF_INET6);

		LEMON_CHECK(v6 == net::address_v6(((sockaddr_in6*)iter->ai_addr)->sin6_addr));

		LEMON_CHECK(v6.length() == sizeof(in6_addr));

		LEMON_CHECK(v6.tostring() == "2002:7671:bfd4:a:fdec:30ff:85ce:3b43");
	}
#endif // LEMON_SUPPORT_IPV6
}}
