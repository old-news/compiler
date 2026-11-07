#include "../tokenizer/token.cpp"
#include "parser_exception.cpp"
#include "node.cpp"

#include <vector>
#include <functional>

using namespace std;

class Parser {
	private:
		vector<Token> input;
		size_t pos;
		Pos filepos;

		Token curr();
		TokenType currtype();
		Token peek(size_t);
		bool empty();
		Pos getpos();
		Token match(TokenType);
		Token tryMatch(TokenType);
		void advance();
		NodePtr tryParse(function<NodePtr()>);
		NodePtr tryParseStar(function<NodePtr()>);
		NodePtr list(function<NodePtr()>, TokenType);
		NodePtr list(function<Token()>, TokenType);
		void enter_scope();
		void exit_scope();
		void loc_end();
		NodePtr type();

		NodePtr program();
		NodePtr compound_block();
		NodePtr block();

		NodePtr expression();
		NodePtr ternary_operator();
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
		NodePtr vvalue();
		NodePtr v1();
		NodePtr v2();
		NodePtr v3();
		NodePtr array_expression();
		NodePtr function_call();

		NodePtr function_definition();
		NodePtr type_name_pair();

		NodePtr enum_definition();

		NodePtr import_line();
		NodePtr id_chain();

		NodePtr error_block();
		NodePtr try_block();
		NodePtr except_block();
		NodePtr finally_block();

		NodePtr class_definition();
		NodePtr class_def_body();
		NodePtr class_func_section();
		NodePtr class_function();
		NodePtr class_access_level();
		NodePtr class_var_section();
		NodePtr class_var_group();
	public:
		Parser(const vector<Token>& i);
		NodePtr parse();
};
