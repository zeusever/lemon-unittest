#include <lemon/ppc/package.hpp>
#include <lemonxx/unittest/unittest.hpp>
#include <unittest/lemon/ppc/configure.h>

namespace lemon{namespace ppc{namespace test{

	struct PackageUnittest{};

	LEMON_UNITTEST_CASE(PackageUnittest,StdOutTest)
	{
		core::Package package(UNITTEST_LEMON_PPC_ASSEMBLY_PATH);

		LemonUuid uuid = { 0x612c9e12, 0x2142, 0x4a7d, { 0x8a, 0x11, 0x3c, 0x2e, 0x8e, 0x58, 0xc8, 0x53 } };

		LEMON_CHECK(package.Guid() == uuid);

		LEMON_CHECK(package.GetName() == "unittest-lemon-ppc");

		LEMON_CHECK(package.TraceLogMacroName() == "UNITTEST_LEMON_PPC_TRACE");
	}

}}}
