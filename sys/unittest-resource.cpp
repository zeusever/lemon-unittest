#include <lemonxx/sys/resource.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace test{

	struct ResourceUnittes{};

	LEMON_UNITTEST_CASE(ResourceUnittes,trace_event_id_test)
	{
		LEMON_CHECK(trace_event_id(0x10,0x1000) == 0x00101000);

		LEMON_CHECK(trace_event_id(0x1002,0x1000) == 0x10021000);
	}

	LEMON_UNITTEST_CASE(ResourceUnittes,VerionTest)
	{
		resource rc;
	
		const LemonVersion * version;
		
		version = rc.version(LEMON_TEXT("1.0.*"));

		LEMON_CHECK(version->Major == 1);

		LEMON_CHECK(version->Minor == 0);

		version = rc.version(LEMON_TEXT("1.12131.123.5"));

		LEMON_CHECK(version->Major == 1);

		LEMON_CHECK(version->Minor == 12131);

		LEMON_CHECK(version->Build == 123);

		LEMON_CHECK(version->Reversion == 5);

		version = rc.version(LEMON_TEXT("1.12131.123.*"));

		LEMON_CHECK(version->Major == 1);

		LEMON_CHECK(version->Minor == 12131);

		LEMON_CHECK(version->Build == 123);

		LEMON_UNITTEST_EXPECT_EXCEPTION(rc.version(LEMON_TEXT("1.12131.123.*.")),error_info);

		LEMON_UNITTEST_EXPECT_EXCEPTION(rc.version(LEMON_TEXT("1.12131")),error_info);

		LEMON_UNITTEST_EXPECT_EXCEPTION(rc.version(LEMON_TEXT("1.12131.")),error_info);

		LEMON_UNITTEST_EXPECT_EXCEPTION(rc.version(LEMON_TEXT("1")),error_info);
	}

	LEMON_UNITTEST_CASE(ResourceUnittes,AddResourceItem)
	{
		resource rc;

		resource_errorinfo ec(1,LEMON_TEXT("LEMON_RESOURCE_ERRORINFO_TEST"),LEMON_TEXT("test"));

		rc.add(ec);

		resource_text text(LEMON_TEXT("LEMON_RESOURCE_ERRORINFO_TEST"),LEMON_TEXT("test"));

		rc.add(text);

		resource_trace_catalog catalog(1,LEMON_TEXT("LEMON_RESOURCE_ERRORINFO_TEST"),LEMON_TEXT("test"));

		rc.add(catalog);

		resource_trace_event event(1,LEMON_TEXT("LEMON_RESOURCE_ERRORINFO_TEST"));

		rc.add(event);

		LEMON_CHECK(ec == rc.errorinfo(1));

		LEMON_CHECK(text == rc.text(text.Global));

		LEMON_CHECK(catalog == rc.event_catalog(catalog.Value));

		LEMON_CHECK(event == rc.event(event.Sequence));

		
	}

	LEMON_UNITTEST_CASE(ResourceUnittes,IteratorTest)
	{
		resource rc;

		rc.add(resource_errorinfo(1,LEMON_TEXT("LEMON_RESOURCE_ERRORINFO_TEST"),LEMON_TEXT("test")));

		rc.add(resource_trace_catalog(1,LEMON_TEXT("LEMON_RESOURCE_ERRORINFO_TEST"),LEMON_TEXT("test")));

		rc.add(resource_errorinfo(2,LEMON_TEXT("LEMON_RESOURCE_ERRORINFO_TEST"),LEMON_TEXT("test")));

		rc.add(resource_errorinfo(3,LEMON_TEXT("LEMON_RESOURCE_ERRORINFO_TEST"),LEMON_TEXT("test")));

		{
			
			resource::const_resource_errorinfo_iterator iter = rc.begin(),end = rc.end();

			LEMON_CHECK(iter != end);

			LEMON_CHECK(++ iter != end);

			LEMON_CHECK(++ iter != end);

			LEMON_CHECK(++ iter == end);
		}

		{
			resource::const_resource_trace_catalog_iterator iter = rc.begin(),end = rc.end();

			LEMON_CHECK(iter != end);

			LEMON_CHECK(iter->Value == 1);

			LEMON_CHECK(++ iter == end);
		}
	}

}}

