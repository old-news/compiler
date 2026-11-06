#include "token.cpp"
#include "parser_exception.cpp"
#include "node.cpp"

#include <vector>
#include <functional>

using namespace std;

class Parser {
	private:
		vector<Token> input;
		size_t pos;

		Token curr();
		TokenType currtype();
		Token peek(size_t);
		bool empty();
		Token match(TokenType);
		Token tryMatch(TokenType);
		void advance();
		NodePtr tryParse(function<NodePtr()>);
		NodePtr tryParseStar(function<NodePtr()>);
		void enter_scope();
		void exit_scope();
		NodePtr type();

		NodePtr program();
		NodePtr compound_block();
		NodePtr block();

		NodePtr expression();
		NodePtr e0();
		NodePtr e1();
		NodePtr e2();
		NodePtr e2b();
		NodePtr e3();
		NodePtr e3b();
		NodePtr e3c();
		NodePtr e3d();
		NodePtr e4();
		NodePtr e5();
		NodePtr e6();
		NodePtr e7();
		NodePtr e8();
		NodePtr literal();
		NodePtr ternary_operator();

		NodePtr function_definition();
		NodePtr function_def_arg_list();
		NodePtr function_def_arg();
	public:
		Parser(const vector<Token>& i);
		NodePtr parse();
};
