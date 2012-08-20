#include <limits>
#include <iomanip>
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
		size_t counter = 2500;

		lua_state L(luaL_newstate());

		for(size_t i = 0 ; i < counter; ++ i)
		{
			LEMON_CHECK(context(L) == &L);
		}
	}

	LEMON_UNITTEST_CASE(lstate_unittest,compare_test)
	{
		size_t counter = 2500;

		lua_state L(luaL_newstate());

		for(size_t i = 0 ; i < counter; ++ i)
		{
			LEMON_CHECK(&L == &L);
		}
	}

	LEMON_UNITTEST_CASE(lstate_unittest,call_test)
	{
		lua_state L;

		std::string file = UNITTEST_LEMON_LUA_SRC;

		file += "/unittest-lemon-lua-call.lua";

		dofile(L,file.c_str());

		call<void>(L,"hello");

		std::cout << call<std::string>(L,"hello") << std::endl;

		tuple<const char*,bool> result = call< tuple<const char*,bool> > (L,"hello_false");

		std::cout << get<0>(result) << std::endl;

		LEMON_CHECK(get<1>(result) == false);

		result = call< tuple<const char*,bool> > (L,"hello_true");

		std::cout << get<0>(result) << std::endl;

		LEMON_CHECK(get<1>(result) == true);

		result = call< tuple<const char*,bool> > (L,"say_hello","hello world",true);

		std::cout << get<0>(result) << std::endl;

		LEMON_CHECK(get<1>(result) == true);

		result = call< tuple<const char*,bool> > (L,"say_hello","hello world",false);

		std::cout << get<0>(result) << std::endl;

		LEMON_CHECK(get<1>(result) == false);

		LEMON_CHECK(call<int>(L,"echo_number",1) == 1);

		LEMON_CHECK(call<float>(L,"echo_number",1.0121) == 1.0121f);

		LEMON_CHECK(call<double>(L,"echo_number",1.0121f) == 1.0121f);

		LEMON_CHECK(call<byte_t>(L,"echo_number",1) == 1);

		LEMON_CHECK(call<uint16_t>(L,"echo_number",0xffff) == 0xffff);
	}

	void C_Hello()
	{
		std::cout << "luabind extension call" << std::endl;
	}

	void C_Hello_2()
	{
		std::cout << "luabind extension call 2" << std::endl;
	}

	void C_Hello_3(const char * message)
	{
		if(message)
		{
			std::cout << message << std::endl;
		}
	}

	std::string C_Hello_4(const std::string & message)
	{
		return message;
	}

	LEMON_UNITTEST_CASE(lstate_unittest,extensions_test)
	{
		lua_state L;

		std::string file = UNITTEST_LEMON_LUA_SRC;

		file += "/unittest-lemon-lua-call.lua";

		dofile(L,file.c_str());

		module(L,"extensions_test") 
			
			<= def("hello",&C_Hello)
			
			<= def("hello2",&C_Hello_2)

			<= def("hello3",&C_Hello_3)

			<= def("hello4",&C_Hello_4)
			;

		call<void>(L,"c_extension_hello");
	}

	class test
	{
	public:
		void hello()
		{
			int i = 0; 

			++ i;
		}

		void hello2()
		{
			int i = 0; 

			++ i;
		}
	};

	void C_Hello_5()
	{

	}

	struct lua_class_bind_unittest
	{
		lua_state L;

		size_t counter;

		lua_class_bind_unittest()
		{
			counter = 25000;

			std::string file = UNITTEST_LEMON_LUA_SRC;

			file += "/unittest-lemon-lua-call.lua";

			dofile(L,file.c_str());

			module(L,"extensions_test")

				<= class_<test>("test")
				.def("hello",&test::hello)
				.def("hello2",&test::hello2)

				<= def("hello",C_Hello_5)
				;
		}
	};

	

	LEMON_UNITTEST_CASE(lua_class_bind_unittest,lua_class_bind_call0_test)
	{
		test t;

		for(size_t i = 0 ; i < Context()->counter; ++ i)
		{
			call<void>(Context()->L,"cpp_test",&t);
		}

	}

	LEMON_UNITTEST_CASE(lua_class_bind_unittest,lua_class_bind_call0_compare1_test)
	{
		void * data = NULL;

		for(size_t i = 0 ; i < Context()->counter; ++ i)
		{
			call<void>(Context()->L,"cpp_test1",data);
		}

	}

	LEMON_UNITTEST_CASE(lua_class_bind_unittest,lua_class_bind_call0_compare_test)
	{
		test t;

		for(size_t i = 0 ; i < Context()->counter; ++ i)
		{
			t.hello();

			t.hello2();
		}
	}
}}}

