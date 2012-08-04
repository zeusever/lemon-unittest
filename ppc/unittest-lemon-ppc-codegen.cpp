#include <lemon/ppc/codegen.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace ppc{namespace test{

	struct CXXCodeGenUnittest{};

	LEMON_UNITTEST_CASE(CXXCodeGenUnittest,StdOutTest)
	{
		const char * source = "LEMON_PPC_TEST(a,b,c,\"test %1,%2,%3\",1,a.test(),(double)3,4)\nLEMON_PPC_TEST(a,b,c,\"test %1,%3\",1,a.test(),4)";

		core::Lexer lexer;

		lexer.ReloadString(source);

		core::AST ast;

		core::Parser parser("LEMON_PPC_TEST");

		parser.Parse(lexer,ast);

		core::CXXCodeGen codegen(lemon::uuid_t(),"LEMON_PPC_TEST","A;B;C",1);

		codegen.Generate(std::cout,ast);
	}


	

}}}
