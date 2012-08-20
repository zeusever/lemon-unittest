#include <lemonxx/dtrace/dtrace.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace dtrace{namespace test{

	class DTraceUnittest{};

	LEMON_UNITTEST_CASE(DTraceUnittest,ServiceTest)
	{
		service local;

		service listener("listen://0.0.0.0:6543");
	}

}}}

