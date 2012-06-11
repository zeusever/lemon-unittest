#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/memory/ringbuffer.hpp>

namespace lemon{namespace memory{namespace test{
	struct RingBufferUnittest{};

	LEMON_UNITTEST_CASE(RingBufferUnittest,ContructorTest)
	{
		ringbuffer::allocator<sizeof(size_t)> alloc(1024);

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

		for(size_t i = 0; i < alloc.capacity(); ++ i)
		{
			void * block  = alloc.pop_back();

			LEMON_CHECK(0 != block);

			memcpy(&length,block,sizeof(length));

			LEMON_CHECK(i == length);
		}

		LEMON_CHECK(alloc.length() == 0);
	}

	LEMON_UNITTEST_CASE(RingBufferUnittest,IteratorTest)
	{
		typedef ringbuffer::allocator<sizeof(size_t),10> allocator_type;

		allocator_type alloc(3);

		//LEMON_CHECK(alloc.capacity() == 3);

		size_t length = alloc.length();

		alloc.push_back(&length,sizeof(length));

		LEMON_CHECK(alloc.length() == 1);

		length = alloc.length();

		alloc.push_back(&length,sizeof(length));

		LEMON_CHECK(alloc.length() == 2);

		length = alloc.length();

		alloc.push_back(&length,sizeof(length));

		LEMON_CHECK(alloc.length() == 3);

		memcpy(&length,*alloc.front(),sizeof(length));

		LEMON_CHECK(length == 0);

		memcpy(&length,*alloc.back(),sizeof(length));

		LEMON_CHECK(length == 2);

		LEMON_CHECK(++ alloc.front() == -- alloc.back());

		LEMON_CHECK(++ alloc.back() == alloc.end());
	}
}}}