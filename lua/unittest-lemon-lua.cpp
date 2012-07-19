#include <lemonxx/luabind/luabind.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <unittest/lemon/lua/configure.h>

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

		size_t counter = 1000;

		for(size_t i = 0 ; i < counter; ++ i)
		{
			void * data = L.alloc();

			LEMON_CHECK(data != NULL);

			L.free(data);
		}
	}

	void call_test(state &L)
	{
		/*luacall(L,"hello");

		luacall<std::string>(L,"hello");

		luacall<const char*>(L,"hello");

		tuple<const char*,bool> result = luacall< tuple<const char*,bool> >(L,"hello_false");

		LEMON_CHECK(!get<1>(result));

		tuple<std::string,bool> result1 = luacall< tuple<std::string,bool> >(L,"hello_true");

		LEMON_CHECK(get<1>(result1));

		char * message = "hello world";

		bool status = false;

		result1 = luacall< tuple<std::string,bool> >(L,"say_hello",message,status);

		LEMON_CHECK(get<0>(result1) == message);

		LEMON_CHECK(!get<1>(result1));*/

		//////////////////////////////////////////////////////////////////////////
		//number test
		// 

		LEMON_CHECK(1 == luacall<int>(L,"echo_number",1));

		LEMON_CHECK(1.1f == luacall<float>(L,"echo_number",1.1f));

		LEMON_CHECK(1.121213113f == luacall<double>(L,"echo_number",1.121213113f));

		{
			lemon::byte_t id = 1;

			LEMON_CHECK(id == luacall<lemon::byte_t>(L,"echo_number",id));
		}

		{
			long id = 1;

			LEMON_CHECK(id == luacall<long>(L,"echo_number",id));
		}

		{
			lemon::uint16_t id = 1;

			LEMON_CHECK(id == luacall<lemon::uint16_t>(L,"echo_number",id));
		}

		{
			lemon::uint32_t id = 1;

			LEMON_CHECK(id == luacall<lemon::uint32_t>(L,"echo_number",id));
		}

		{
			lemon::uint64_t id = 1;

			LEMON_CHECK(id == luacall<lemon::uint64_t>(L,"echo_number",id));
		}
	}

	struct lstate_pcall_unittest
	{
		lstate_pcall_unittest()
		{
			counter = 100000;

			std::string script = UNITTEST_LEMON_LUA_SRC;

			script += "/unittest-lemon-lua-call.lua";

			dofile(L,script);
		}

		size_t counter;

		state L;
	};

	LEMON_UNITTEST_CASE(lstate_pcall_unittest,luacall_test)
	{
		call_test(Context()->L);
	}

	template<typename T>
	T echo_number(T v)
	{
		return v;
	}

	void Compare()
	{
		LEMON_CHECK(1 == echo_number(1));

		LEMON_CHECK(1.1f == echo_number(1.1f));

		LEMON_CHECK(1.121213113f == echo_number(1.121213113f));

		{
			lemon::byte_t id = 1;

			LEMON_CHECK(id == echo_number(id));
		}

		{
			long id = 1;

			LEMON_CHECK(id == echo_number(id));
		}

		{
			lemon::uint16_t id = 1;

			LEMON_CHECK(id == echo_number(id));
		}

		{
			lemon::uint32_t id = 1;

			LEMON_CHECK(id == echo_number(id));
		}

		{
			lemon::uint64_t id = 1;

			LEMON_CHECK(id == echo_number(id));
		}
	}

	LEMON_UNITTEST_CASE(lstate_pcall_unittest,luacall_stack_test)
	{
		

		for(size_t i = 0; i < Context()->counter; ++ i) call_test(Context()->L);
		
	}

	LEMON_UNITTEST_CASE(lstate_pcall_unittest,compare_test)
	{
		for(size_t i = 0; i < Context()->counter; ++ i)
		{
			Compare();
		}
	}

}}}

