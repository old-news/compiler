#include "parser.h"
#include <memory>

NodePtr Parser::function_definition() {
	NodePtr n = make_unique<Node>(FUNCTION_DEFINITION);
	n->add(NAME, match(ID));
	match(OPEN_PARENTHESIS);
	if (CLOSE_PARENTHESIS != currtype()) {
		n->add(
			ARGS,
			list([this] {return var_ldecl();}, COMMA)
		);
	}
	match(CLOSE_PARENTHESIS);
	if (!tryMatch(COLON).is(INVALID)) {
		n->add(type());
	}
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
