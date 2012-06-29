#include <lemon/ppc/codegen.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace ppc{namespace test{

	struct CXXCodeGenUnittest{};

	LEMON_UNITTEST_CASE(CXXCodeGenUnittest,StdOutTest)
	{
		const char * source = "LEMON_PPC_TEST(a,b,c,\"test %1,%2,%3\",1,a.test(),(double)3,4)";

		core::Lexer lexer;

		lexer.ReloadString(source);

		core::AST ast;

		core::Parser parser("LEMON_PPC_TEST");

		parser.Parse(lexer,ast);

		core::CxxCodeGen codegen;

		codegen.Generate("LEMON_PPC_TEST","__lemonppc_unittest_lemon_pcc_codegegen",std::cout,ast);
	}


	

}}}
