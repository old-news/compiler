#include "parser.h"
#include <memory>

NodePtr Parser::function_definition() {
	NodePtr n = make_unique<Node>(FUNCTION_DEFINITION);
	n->add(type());
	n->add(match(ID));
	n->add(match(OPEN_PARENTHESIS));
	n->add(function_def_arg_list());
	enter_scope();
	n->add(compound_block());
	exit_scope();
}

NodePtr Parser::function_def_arg_list() {
	NodePtr n = make_unique<Node>(FUNCTION_DEF_ARG_LIST);
	n->add(function_def_arg());
	NodePtr c;
	while ((c = tryParse([this] {return function_def_arg();}))) {
		n->add(std::move(c));
		n->add(match(COMMA));
	}
	return n;
}

NodePtr Parser::function_def_arg() {
	NodePtr n = make_unique<Node>(FUNCTION_DEF_ARG);
	n->add(type());
	n->add(match(ID));
}
