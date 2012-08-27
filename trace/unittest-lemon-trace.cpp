#include <lemon/trace/abi.h>
#include <lemonxx/trace/object.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace trace{namespace test{

	class ABIUnittest{};

	LEMON_UNITTEST_CASE(ABIUnittest,LemonTraceDescriptionTest)
	{
		//LEMON_CHECK(sizeof(LemonTraceDescription) == 36);
	}

	class TraceUnittest{};

	void Trace(const message & msg)
	{
		description_t description;

		msg.description(description);


	}

	LEMON_UNITTEST_CASE(TraceUnittest,LocalTest)
	{
		lemon::uuid_t id;

		service s;

		provider p(s,&id);

		controller c(s);

		conumser cs(c,&Trace);

		LEMON_CHECK(!cs.empty());

		conumser cs1;

		LEMON_CHECK(cs1.empty());

		cs1.start(c,&Trace);

		LEMON_CHECK(!cs1.empty());
	}

}}}


