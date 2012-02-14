#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/sys/random.hpp>

namespace lemon{namespace test{

	struct RandomUnittest{};

	LEMON_UNITTEST_CASE(RandomUnittest,LoopTest)
	{
		random_t rd;

		size_t buffer[5];

		memset(&buffer,0,sizeof(buffer));

		for(size_t i =0 ; i < 1000; ++i)
		{
			for(size_t j = 0; j < sizeof(buffer)/sizeof(size_t); ++ j)
			{
				size_t value =  rd;

				LEMON_CHECK(buffer[j] != value);

				buffer[j] = value;
			}		
		}
	}
}}