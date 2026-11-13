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

NodePtr Parser::for_block() {
	NodePtr n = make_unique<Node>(FOR_BLOCK);
	n->add(for_inner());
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::for_inner() {
	NodePtr n = make_unique<Node>(FOR_INNER);
	n->add(var_declaration());
	Token t = tryMatch(TO);
	if (t.is(INVALID)) {
		n->add(match(IN));
	} else {
		n->add(t);
	}
	n->add(expression());
	return n;
}

NodePtr Parser::while_block() {
	NodePtr n = make_unique<Node>(WHILE_BLOCK);
	n->add(CONDITION, expression());
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::if_block() {
	NodePtr n = make_unique<Node>(IF_BLOCK);
	n->add(CONDITION, expression());
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	NodePtr c;
	while ((c = tryParse([this] {return elif_block();}))) {
		n->add(std::move(c));
	}
	c = tryParse([this] {return else_block();});
	if (c) {
		n->add(std::move(c));
	}
	return n;
}

NodePtr Parser::elif_block() {
	NodePtr n = make_unique<Node>(ELIF_BLOCK);
	match(ELIF);
	n->add(CONDITION, expression());
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::else_block() {
	NodePtr n = make_unique<Node>(ELSE_BLOCK);
	match(ELSE);
	n->add(CONDITION, expression());
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::switch_block() {
	NodePtr n = make_unique<Node>(SWITCH_BLOCK);
	n->add(expression());
	enter_scope();
	n->add(switch_inner());
	exit_scope();
	return n;
}

NodePtr Parser::switch_inner() {
	NodePtr n = make_unique<Node>(SWITCH_INNER);
	bool didDefault = false;
	Token t = tryMatch(DEFAULT);
	if (!t.is(INVALID)) {
		retreat();
		n->add(switch_default());
		didDefault = true;
	}
	n->add(SWITCH_CASE_LIST, tryParseStar([this] {return switch_case();}));
	if (!didDefault) {
		t = tryMatch(DEFAULT);
		if (!t.is(INVALID)) {
			retreat();
			n->add(switch_default());
			didDefault = true;
		}
	}
	return n;
}

NodePtr Parser::switch_case() {
	NodePtr n = make_unique<Node>(SWITCH_CASE);
	n->add(CASE_VALUE_LIST, tryParseStar([this] {return expression();}));
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}

NodePtr Parser::switch_default() {
	NodePtr n = make_unique<Node>(SWITCH_DEFAULT);
	match(DEFAULT);
	enter_scope();
	n->add(BODY, compound_block());
	exit_scope();
	return n;
}
