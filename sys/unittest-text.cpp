#include <string>
#include <iostream> 
#include <lemonxx/unittest/unittest.hpp>
#include <lemonxx/sys/text.hpp>

namespace lemon{namespace test{
	struct LemonTextUnittest{};

	LEMON_UNITTEST_CASE(LemonTextUnittest,UTF8LocalTest)
	{
		//const std::string helloworld = "hello the world";

		//char buffer[] = "hello the world";

		//LEMON_DECLARE_ERRORINFO(errorinfo);

		//size_t length = LemonLocaleToUTF8(helloworld.c_str(),helloworld.size(),buffer,sizeof(buffer),&errorinfo);

		//LEMON_CHECK(LEMON_SUCCESS(errorinfo));

		//std::cout << helloworld.length() << "--" << length << std::endl;

		//std::cout << buffer << std::endl;

		//LEMON_CHECK(length == helloworld.length());

		///*length = LemonLocaleToUTF8(helloworld.c_str(),helloworld.size(),buffer,0,&errorinfo);

		//LEMON_CHECK(LEMON_FAILED(errorinfo));

		//std::cout << helloworld.length() << "--" << length << std::endl
		//
		//LEMON_CHECK(length == helloworld.length());
		//*/

		//LEMON_CHECK(to_utf8(helloworld) == helloworld);
	}

}}
