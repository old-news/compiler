#include "parser.h"
#include <memory>

NodePtr Parser::error_block() {
	NodePtr n = make_unique<Node>(ERROR_BLOCK);
	n->add(try_block());
	while (EXCEPT == currtype()) {
		n->add(except_block());
	}
	if (FINALLY == currtype()) {
		n->add(finally_block());
	}
	return n;
}

NodePtr Parser::try_block() {
	NodePtr n = make_unique<Node>(TRY_BLOCK);
	match(TRY);
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::except_block() {
	NodePtr n = make_unique<Node>(EXCEPT_BLOCK);
	match(EXCEPT);
	n->add(var_ldecl());
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::finally_block() {
	NodePtr n = make_unique<Node>(FINALLY_BLOCK);
	match(FINALLY);
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}
