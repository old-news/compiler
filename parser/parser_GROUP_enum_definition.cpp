#include "parser.h"
#include <memory>

NodePtr Parser::enum_definition() {
	NodePtr n = make_unique<Node>(ENUM_DEFINITION);
	match(ENUM);
	n->add(make_unique<Node>(NAME, match(ID)));
	enter_scope();
	n->add(id_list());
	exit_scope();
	return n;
}

NodePtr Parser::id_list() {
	NodePtr n = make_unique<Node>(LIST);
	n->add(match(ID));
	while (COMMA == currtype()) {
		match(COMMA);
		n->add(match(ID));
	}
	return n;
}
