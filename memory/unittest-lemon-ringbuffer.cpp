#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/memory/ringbuffer.hpp>

namespace lemon{namespace memory{namespace test{
	struct RingBufferUnittest{};

	LEMON_UNITTEST_CASE(RingBufferUnittest,ContructorTest)
	{
		ringbuffer::allocator<sizeof(size_t),3> alloc;

		size_t capacity = 0;

		while(0 != (capacity = alloc.capacity()))
		{
			LEMON_CHECK(alloc.push_front(&capacity,sizeof(size_t)) == 0);
		}

		void * block  = alloc.push_front(&capacity,sizeof(size_t));

		LEMON_CHECK(0 != block);

		memcpy(&capacity,block,sizeof(capacity));

		LEMON_CHECK(1 == capacity);
	}
}}}