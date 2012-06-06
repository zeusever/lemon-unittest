#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/memory/ringbuffer.hpp>

namespace lemon{namespace memory{namespace test{
	struct RingBufferUnittest{};

	LEMON_UNITTEST_CASE(RingBufferUnittest,ContructorTest)
	{
		ringbuffer::allocator<sizeof(size_t),1024> alloc;

		size_t length = 0;

		while(alloc.capacity() != (length = alloc.length()))
		{
			LEMON_CHECK(alloc.push_front(&length,sizeof(size_t)) == 0);
		}

		for(size_t i = 0; i < alloc.capacity(); ++ i)
		{
			void * block  = alloc.push_front(&length,sizeof(size_t));

			LEMON_CHECK(0 != block);

			memcpy(&length,block,sizeof(length));

			LEMON_CHECK(i == length);
		}

		
	}
}}}