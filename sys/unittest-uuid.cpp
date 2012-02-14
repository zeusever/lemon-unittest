#include <iostream>
#include <lemonxx/sys/uuid.hpp>
#include <lemonxx/unittest/dsel.hpp>

namespace lemon{namespace test{
	
	struct UuidUnittest{};

	LEMON_UNITTEST_CASE(UuidUnittest,CreateTest)
	{
		//generate uuid
		lemon::uuid_t u1;
		//uuid to string
		std::string u1string = u1.tostring();
		//generate uuid from source string
		lemon::uuid_t u2(u1string);

		LEMON_CHECK(u1 == u2);

		LEMON_CHECK(u2.tostring() == u1string);

		for (size_t i = 0;i < 100 ; ++ i)
		{
			lemon::uuid_t u3;

			LEMON_CHECK(u3 != u1);

			u1 = u3;
		}
	}

}}