#include <iostream>
#include <lemonxx/io/timer.hpp>
#include <lemonxx/function/bind.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	struct TimerUnittest{};

	void Callback(lemon::condition_variable & cv,lemon::mutex_t & mutex,size_t &counter)
	{
		if(counter < 4)
		{
			std::cout << "time out" << std::endl;

			++ counter;
		}
		else
		{
			lemon::mutex_t::scope_lock lock(mutex);

			cv.notify();
		}
	}

	LEMON_UNITTEST_CASE(TimerUnittest,TimerTest)
	{
		io_device device;

		timer t;

		lemon::mutex_t mutex;

		lemon::condition_variable cv;

		size_t counter = 0;

		t.start(device,1000,bind(&Callback,ref(cv),ref(mutex),ref(counter)));

		lemon::mutex_t::scope_lock lock(mutex);

		cv.wait(lock);
	}

}}}