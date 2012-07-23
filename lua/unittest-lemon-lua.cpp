#include <lemonxx/luabind/luabind.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <unittest/lemon/lua/configure.h>

namespace lemon{namespace luabind{namespace test{

	struct lstate_unittest {};

	LEMON_UNITTEST_CASE(lstate_unittest,bind_test)
	{
		{
			lua_state L;

			LEMON_CHECK(context(L) == &L);
		}

		{
			lua_state L(luaL_newstate());

			LEMON_CHECK(context(L) == &L);


		}
	}

	LEMON_UNITTEST_CASE(lstate_unittest,bind_performance_test)
	{
		size_t counter = 2500000;

		lua_state L(luaL_newstate());

		for(size_t i = 0 ; i < counter; ++ i)
		{
			LEMON_CHECK(context(L) == &L);
		}
	}

	LEMON_UNITTEST_CASE(lstate_unittest,compare_test)
	{
		size_t counter = 2500000;

		lua_state L(luaL_newstate());

		for(size_t i = 0 ; i < counter; ++ i)
		{
			LEMON_CHECK(&L == &L);
		}
	}

}}}

