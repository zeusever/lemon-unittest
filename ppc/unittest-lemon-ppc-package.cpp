#include <lemon/ppc/package.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <unittest/lemon/ppc/configure.h>
#include <unittest/lemon/ppc/g.unittest-lemon-ppc-package.cpp>


namespace lemon{namespace ppc{namespace test{

	struct PackageUnittest{};

	LEMON_UNITTEST_CASE(PackageUnittest,StdOutTest)
	{
		core::Package package(UNITTEST_LEMON_PPC_ASSEMBLY_PATH);

		LemonUuid uuid = { 0x612c9e12, 0x2142, 0x4a7d, { 0x8a, 0x11, 0x3c, 0x2e, 0x8e, 0x58, 0xc8, 0x53 } };

		LEMON_CHECK(package.Guid() == uuid);

		LEMON_CHECK(package.GetName() == LEMON_TEXT("unittest-lemon-ppc"));

		LEMON_CHECK(package.TraceLogMacroName() == "UNITTEST_LEMON_PPC_TRACE");
	}
	
	
	struct TraceLogUnittest
	{
		LemonTraceMessage * create_message(const LemonUuid * id,size_t level,size_t catalog,size_t length)
		{
			LemonTraceMessage * message = (LemonTraceMessage *)new lemon::byte_t[length];

			message->TimeStamp = lemon::time_t::now();

			message->Provider =  id;

			message->Catalog = catalog;

			message->Level = level;

			message->DataLength = length;

			return message;
		}

		void trace(LemonTraceMessage * message)
		{
			delete [] (lemon::byte_t*)message;
		}
	};

	LEMON_UNITTEST_CASE(TraceLogUnittest,T1)
	{
		std::cout << sizeof(LemonTraceMessage) << std::endl;

		UNITTEST_LEMON_PPC_TRACE(*Context(),LEMON_TRACELOG_DEBUG,TEST_A,"hello world [0]",1);
	}

}}}
