#include <lemonxx/io/udp.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	class SocketUnittest{};

	LEMON_UNITTEST_CASE(SocketUnittest,CreateTest)
	{
		io_service service;

		ip::udp::socket s(AF_INET,service);
	}

}}}

