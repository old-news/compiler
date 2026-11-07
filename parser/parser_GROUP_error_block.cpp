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
	return make_unique<Node>(TRY_BLOCK, compound_block());
}

NodePtr Parser::except_block() {
	NodePtr n = make_unique<Node>(EXCEPT_BLOCK);
	n->add(type_name_pair());
	n->add(BODY, compound_block());
	return n;
}

NodePtr Parser::finally_block() {
	return make_unique<Node>(FINALLY_BLOCK, compound_block());
}
