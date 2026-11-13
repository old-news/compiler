#include "parser.h"
#include <memory>

NodePtr Parser::function_definition() {
	return function_definition(false);
}

NodePtr Parser::function_definition(bool is_class_function) {
	NodePtr n = make_unique<Node>(FUNCTION_DEFINITION);
	n->add(NAME, match(ID));
	match(OPEN_PARENTHESIS);
	n->add(
		ARGS,
		list([this] {return var_ldecl();}, COMMA)
	);
	match(CLOSE_PARENTHESIS);
	match(COLON);
	if (is_class_function) {
		n->add(class_access_level());
	}
	n->add(type());
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::var_ldecl() {
	NodePtr n = make_unique<Node>(VAR_LDECL);
	n->add(NAME, match(ID));
	match(COLON);
	n->add(type());
	return n;
}
