#include "parser.h"
#include <memory>

NodePtr Parser::enum_definition() {
	NodePtr n = make_unique<Node>(ENUM_DEFINITION);
	match(ENUM);
	n->add(make_unique<Node>(NAME, match(ID)));
	enter_scope();
	n->add(BODY, list([this] {return make_unique<Node>(match(ID));}, COMMA));
	exit_scope();
	return n;
}
