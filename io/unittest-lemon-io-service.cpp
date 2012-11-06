#include <lemonxx/function/bind.hpp>
#include <lemonxx/io/io_service.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	class IOServiceUnittest{};

	LEMON_UNITTEST_CASE(IOServiceUnittest,IOServiceBaseTest)
	{
		io_service service;

		for(size_t i = 0; i < 100; ++ i)
		{
			LEMON_CHECK(!service.stopped());

			service.stop();

			LEMON_CHECK(service.stopped());

			service.reset();
		}
	}

	void __stop(io_service & service)
	{
		lemon::sleep(4000);

		service.stop();
	}

	LEMON_UNITTEST_CASE(IOServiceUnittest,IOServiceDispatchTest)
	{
		io_service service;

		lemon::thread_t stop(lemon::bind(&__stop,lemon::ref(service)));

		service.dispatch();
	}

}}}