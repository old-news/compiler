#include "parser.h"
#include <memory>

NodePtr Parser::program() {
	NodePtr n = make_unique<Node>(PROGRAM);
	n->add(compound_block());
	return n;
}

NodePtr Parser::compound_block() {
	NodePtr n = make_unique<Node>(COMPOUND_BLOCK);
	NodePtr c;
	do {
		c = tryParse([this] {return block();});
		if (c) n->add(std::move(c));
	} while (c);
	return n;
}

NodePtr Parser::block() {
	NodePtr n = make_unique<Node>(BLOCK);
	switch (currtype()) {
		case WHILE: case FOR: case IF: case SWITCH:
			n->add(control_flow());
			break;
		case CLASS:
			n->add(class_definitionn());
			break;
		case ENUM:
			n->add(enum_definition());
			break;
		case IMPORT:
			n->add(import_line());
			break;
		case TRY:
			n->add(error_handling());
			break;
		default:
			NodePtr c = tryParse([this] {return loc();});
			if (c) {
				n->add(std::move(c));
				break;
			}
			n->add(function_definition());
			break;
	}
	return n;
}
