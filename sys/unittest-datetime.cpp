#include <iostream>
#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/sys/datetime.hpp>

namespace lemon{namespace test{

	struct DateTimeUnittest{};

	LEMON_UNITTEST_CASE(DateTimeUnittest,time_tTest)
	{
		time_t t1;

		for(size_t i = 0; i < 100; ++ i) std::cout << "time duration..." << std::endl;

		time_t t2;

		time_t t3  = t2;

		LEMON_CHECK(t1 < t2);

		LEMON_CHECK(t1 != t2);

		LEMON_CHECK(t1 != t3);

		LEMON_CHECK(t2 == t3);

		LEMON_CHECK((t2 - t1) > 0);

		LEMON_CHECK((t1 - t2) < 0);

		time_duration du = t2 - t1;

		LEMON_CHECK(t1 + du == t2);

	}

	LEMON_UNITTEST_CASE(DateTimeUnittest,datetimeTest)
	{
		time_t t;

		localtime_t local(t);

		utctime_t utc(t);

		LEMON_CHECK(local.time() == utc.time());
	}
}}