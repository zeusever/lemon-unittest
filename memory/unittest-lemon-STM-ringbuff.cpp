#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/memory/STM-ringbuffer.hpp>

namespace lemon{namespace memeory{namespace STM{namespace test{
	
	struct STMRingBufferUnittest{};

	LEMON_UNITTEST_CASE(STMRingBufferUnittest,SizeCapacityTest)
	{
		{
			ringbuffer<sizeof(size_t)> buffer(1024,1024);

			LEMON_CHECK(buffer.size() == 0);

			LEMON_CHECK(buffer.capacity() == 1024);
		}

		{
			ringbuffer<sizeof(size_t)> buffer(1024 ,1023);

			LEMON_CHECK(buffer.size() == 0);

			LEMON_CHECK(buffer.capacity() == 1023 * 2);
		}
	}
}}}}