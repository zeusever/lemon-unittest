#include <lemon/io/basic_io_service.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	class IObjectUnittest{};

	class SocketImpl{};

	class IOServiceImpl 
	{
	public:
		
		typedef SocketImpl socket_service;


	};

	LEMON_UNITTEST_CASE(IObjectUnittest,CompilerTest)
	{
		impl::basic_io_service<IOServiceImpl>	ioservice;
	}
}}}