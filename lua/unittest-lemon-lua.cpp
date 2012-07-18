#include <lemonxx/luabind/luabind.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace luabind{namespace test{

	struct lstate_unittest{};

	LEMON_UNITTEST_CASE(lstate_unittest,create_lstate_test)
	{
		state L;

		LEMON_CHECK(NULL != (lua_State*)L);
	}


	LEMON_UNITTEST_CASE(lstate_unittest,lstate_alloc_test)
	{
		state L;

		size_t counter = 100000;

		for(size_t i = 0 ; i < counter; ++ i)
		{
			void * data = L.alloc();

			LEMON_CHECK(data != NULL);

			L.free(data);
		}
	}

}}}

