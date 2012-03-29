#include <lemonxx/sys/sys.hpp>
#include <lemonxx/io/io_device.hpp>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{
	
	struct IoDeviceUnittest{};

	/*LEMON_UNITTEST_CASE(IoDeviceUnittest,BaseTest)
	{
		io_device device;

		lemon::thread_t work(lemon::bind(&io_device::run,&device));

		for(;;)
		{
			if(1 == device.stop()) break;
		}

		work.join();
	}*/

	void ExitCall(size_t numberOfBytesTransferred,const LemonErrorInfo &errorCode,io_device & device)
	{
		LEMON_CHECK(0 == numberOfBytesTransferred);

		LEMON_CHECK(LEMON_SUCCESS(errorCode));

		device.stop();
	}

	LEMON_UNITTEST_CASE(IoDeviceUnittest,postTest)
	{
		io_device device;

		lemon::thread_t work(lemon::bind(&io_device::run,&device));

		device.post_done(lemon::bind(&ExitCall,_0,_1,ref(device)));

		work.join();
	}

}}}