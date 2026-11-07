#include "parser.h"
#include <memory>

NodePtr Parser::function_definition() {
	NodePtr n = make_unique<Node>(FUNCTION_DEFINITION);
	n->add(type());
	n->add(NAME, match(ID));
	match(OPEN_PARENTHESIS);
	n->add(
		ARGS,
		list([this] {return type_name_pair();}, COMMA)
	);
	match(CLOSE_PARENTHESIS);
	enter_scope();
	n->add(compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::type_name_pair() {
	NodePtr n = make_unique<Node>(TYPE_NAME_PAIR);
	n->add(type());
	n->add(match(ID));
}
