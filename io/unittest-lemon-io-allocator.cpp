#include <lemon/io/allocator.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace io{namespace test{

	class LemonIOAllocatorUnittest{};

	LEMON_UNITTEST_CASE(LemonIOAllocatorUnittest,AllocTest)
	{
		impl::basic_allocator<mpl::make_list<int,char,std::string>::type> allocator;

		allocator.free<int>(allocator.alloc<int>());

		allocator.free<char>(allocator.alloc<char>());

		allocator.free<std::string>(allocator.alloc<std::string>());
	}

}}}
