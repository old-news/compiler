#include "parser.h"
#include "parser_exception.h"
#include <memory>

NodePtr Parser::program() {
	NodePtr n = make_unique<Node>(PROGRAM);
	n->add(compound_block());
	return n;
}

NodePtr Parser::compound_block() {
	NodePtr n = make_unique<Node>(COMPOUND_BLOCK);
	bool didAdd;
	TokenType t;
	do {
		t = currtype();
		if (END == t) {
			break;
		}
		didAdd = false;
		NodePtr c = block();
		if (!c->children.empty()) {
			n->add(std::move(c));
			didAdd = true;
		}
	} while (didAdd);
	return n;
}

NodePtr Parser::block() {
	NodePtr n = make_unique<Node>(BLOCK);
	switch (currtype()) {
		case WHILE: case FOR: case IF: case SWITCH:
			n->add(control_flow());
			break;
		case CLASS:
			n->add(class_definition());
			break;
		case ENUM:
			n->add(enum_definition());
			break;
		case IMPORT:
			n->add(import_line());
			break;
		case TRY:
			n->add(error_block());
			break;
		case CLOSE_CURLY:
			return n;
		default:
			if (COLON == peek(3).getType() && OPEN_PARENTHESIS == peek(1).getType()) {
				n->add(function_definition());
			} else {
				n->add(loc());
			}
			/*
			 * NodePtr c = tryParse([this] {return loc();});
			 * if (c) {
				 * n->add(std::move(c));
				 * break;
			 * }
			 * c = tryParse([this] {return function_definition();});
			 * if (c) {
				 * n->add(std::move(c));
				 * break;
			 * }
			 * throw ParserException(UNEXPECTED_TOKEN, "Expected something to be here", getpos());
	 		 */
	}
	return n;
}
