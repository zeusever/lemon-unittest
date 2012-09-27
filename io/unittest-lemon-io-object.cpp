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
		typedef impl::basic_io_service<IOServiceImpl>	io_service;

		typedef io_service::socket_type socket_type;

		io_service ioservice;

		socket_type  * socket = new (&ioservice) socket_type();

		delete socket;
	}
}}}