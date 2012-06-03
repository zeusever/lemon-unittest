#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/memory/ringbuffer.hpp>

namespace lemon{namespace memory{namespace test{
	struct RingBufferUnittest{};

	LEMON_UNITTEST_CASE(RingBufferUnittest,ContructorTest)
	{
		ringbuffer::allocator<1024,1> alloc;
	}
}}}