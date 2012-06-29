#include <lemon/ppc/parser.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace ppc{namespace test{

	struct ParserUnittest{};

	LEMON_UNITTEST_CASE(ParserUnittest,BaseTest)
	{
		const char * source = "LEMON_PPC_TEST(a,b,c,\"test %1,%2,%3\",1,a.test(),(double)3,4)";

		core::Lexer lexer;

		lexer.ReloadString(source);

		core::AST ast;

		core::Parser parser("LEMON_PPC_TEST");

		parser.Parse(lexer,ast);

		LEMON_CHECK(1 == ast.NodeList.size());

		LEMON_CHECK(ast.NodeList[0].formatter == "test %1,%2,%3");

		LEMON_CHECK(ast.NodeList[0].args == 8);

		LEMON_CHECK(ast.NodeList[0].lines == 1);
	}


	LEMON_UNITTEST_CASE(ParserUnittest,NoArgTest)
	{
		const char * source = "LEMON_PPC_TEST(a,b,c,\"test\")";

		core::Lexer lexer;

		lexer.ReloadString(source);

		core::AST ast;

		core::Parser parser("LEMON_PPC_TEST");

		parser.Parse(lexer,ast);

		LEMON_CHECK(1 == ast.NodeList.size());

		LEMON_CHECK(ast.NodeList[0].formatter == "test");

		LEMON_CHECK(ast.NodeList[0].args == 4);

		LEMON_CHECK(ast.NodeList[0].lines == 1);
	}

}}}
