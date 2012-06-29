#include <lemon/ppc/lexer.hpp>
#include <lemonxx/unittest/unittest.hpp>

namespace lemon{namespace ppc{namespace test{

	struct LexerUnittest{};

	LEMON_UNITTEST_CASE(LexerUnittest,PreProcessTest)
	{
		core::Lexer lexer;

		const char * preprocess1 = "#include <lemon/ppc/lexer.hpp>";

		const char * preprocess2 = "#include \"lemon/ppc/lexer.hpp\"";

		lexer.ReloadString(preprocess1);

		core::Token token;

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_PREPROCESS);

		LEMON_CHECK(token.Value == preprocess1);

		lexer.ReloadString(preprocess2);

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_PREPROCESS);

		LEMON_CHECK(token.Value == preprocess2);

		LEMON_CHECK(lexer.Line() == 1);
	}

	LEMON_UNITTEST_CASE(LexerUnittest,CommentTest)
	{
		const char * comment_cxx = "// test the cxx comment";

		const char * comment_c = "/* test the pure \n\n\n\n c comment */";

		core::Lexer lexer;

		lexer.ReloadString(comment_cxx);

		core::Token token;

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_COMMENT);

		LEMON_CHECK(token.Value == &comment_cxx[2]);

		lexer.ReloadString(comment_c);

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_COMMENT);

		LEMON_CHECK(token.Value == " test the pure \n\n\n\n c comment ");

		LEMON_CHECK(lexer.Line() == 5);
	}

	LEMON_UNITTEST_CASE(LexerUnittest,IDTest)
	{
		core::Lexer lexer;

		core::Token token;

		const char * source = "_test test_test 8888test test8888";

		lexer.ReloadString(source);

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_ID);

		LEMON_CHECK(token.Value == "_test");

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_ID);

		LEMON_CHECK(token.Value == "test_test");

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_LIT_NUMBER);

		LEMON_CHECK(token.Value == "8888");

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_ID);

		LEMON_CHECK(token.Value == "test");

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_ID);

		LEMON_CHECK(token.Value == "test8888");

		LEMON_CHECK(!lexer.Next(token));
	}

	LEMON_UNITTEST_CASE(LexerUnittest,LitTextTest)
	{
		core::Lexer lexer;

		core::Token token;

		const char * source = "  \"test test \\\" test \"  ";

		lexer.ReloadString(source);

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_LIT_TEXT);

		LEMON_CHECK(token.Value == "test test \\\" test ");
	}

	LEMON_UNITTEST_CASE(LexerUnittest,ComplexTest)
	{
		core::Lexer lexer;

		core::Token token;

		const char * source = "class A {}; //test \n struct B{};";

		lexer.ReloadString(source);

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_ID);

		LEMON_CHECK(token.Value == "class");


		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_ID);

		LEMON_CHECK(token.Value == "A");

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == '{');

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == '}');

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == ';');

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_COMMENT);

		LEMON_CHECK(token.Value == "test ");

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_ID);

		LEMON_CHECK(token.Value == "struct");

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == core::Lexer::TOKEN_ID);

		LEMON_CHECK(token.Value == "B");

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == '{');

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == '}');

		LEMON_CHECK(lexer.Next(token));

		LEMON_CHECK(token.Type == ';');
	}
}}}

