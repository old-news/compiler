#include "parser.h"
#include "parser_exception.cpp"
#include <memory>

NodePtr Parser::control_flow() {
	NodePtr n = make_unique<Node>(CONTROL_FLOW);
	switch (currtype()) {
		case WHILE: n->add(while_block()); break;
		case IF: n->add(if_block()); break;
		case SWITCH: n->add(switch_block()); break;
		case FOR: n->add(for_block()); break;
		default:
			throw ParserException(UNEXPECTED_TOKEN, "Control flow expected", getpos());
	}
	return n;
}

NodePtr Parser::for
