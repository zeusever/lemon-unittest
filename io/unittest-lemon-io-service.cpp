#include <lemonxx/io/io_service.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/function/bind.hpp>

namespace lemon{namespace io{namespace test{

	class IOServiceUnittest{};

	// void Callback(io_service & service, atomic_t & index, size_t counter, size_t bytes, const LemonErrorInfo & errorCode)
	// {
	// 	LEMON_CHECK(LEMON_SUCCESS(errorCode));

	// 	LEMON_CHECK(bytes == 0);

	// 	if( ++ index == counter)
	// 	{
	// 		service.stop();
	// 	}
	// }

	// LEMON_UNITTEST_CASE(IOServiceUnittest,IOServicePostTest)
	// {
	// 	atomic_t index = 0;

	// 	size_t	counter = 100000; 

	// 	io_service service;

	// 	service.start(1);

	// 	for(size_t i = 0; i < counter; ++ i)
	// 	{
	// 		service.post_one(lemon::bind(&Callback,ref(service),ref(index),counter,_0,_1));
	// 	}

	// 	service.join();
	// }

	// LEMON_UNITTEST_CASE(IOServiceUnittest,IOServiceRestartTest)
	// {
	// 	atomic_t index = 0;

	// 	size_t	counter = 100000; 

	// 	io_service service;

	// 	service.start(1);

	// 	for(size_t i = 0; i < counter; ++ i)
	// 	{
	// 		service.post_one(lemon::bind(&Callback,ref(service),ref(index),counter,_0,_1));
	// 	}

	// 	service.join();

	// 	service.reset();

	// 	service.start(4);

	// 	index = 0;

	// 	for(size_t i = 0; i < counter; ++ i)
	// 	{
	// 		service.post_one(lemon::bind(&Callback,ref(service),ref(index),counter,_0,_1));
	// 	}

	// 	service.join();
	// }

	// LEMON_UNITTEST_CASE(IOServiceUnittest,IOServicePost1Test)
	// {
	// 	atomic_t index = 0;

	// 	size_t	counter = 100000; 

	// 	io_service service;

	// 	service.start(2);

	// 	for(size_t i = 0; i < counter; ++ i)
	// 	{
	// 		service.post_one(lemon::bind(&Callback,ref(service),ref(index),counter,_0,_1));
	// 	}

	// 	service.join();
	// }

}}}