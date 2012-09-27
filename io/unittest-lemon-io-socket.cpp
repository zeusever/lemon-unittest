#include <lemon/io/basic_io_service.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	class SocketUnittest{};

	class test_socket_service
	{

	};

	struct test_io_servce_impl
	{
		typedef test_socket_service socket_service;
	};

	LEMON_UNITTEST_CASE(SocketUnittest,InterfaceTest)
	{
		typedef impl::basic_io_service<test_io_servce_impl> io_service;

		io_service::socket_type socket;
	}

}}}