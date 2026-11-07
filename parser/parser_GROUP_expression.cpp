#include "parser.h"

NodePtr Parser::expression() {
	NodePtr n = make_unique<Node>(EXPRESSION);
	NodePtr c = tryParse([this] {return ternary_operator();});
	if (c) {
		n->add(std::move(c));
		return n;
	} else {
		n->add(Parser::e0());
	}
	return n;
}

NodePtr Parser::e0() {
	NodePtr n = make_unique<Node>(E0);
	n->add(e1());
	while (LOGICAL_OR == currtype()) {
		NodePtr parent = make_unique<Node>(E0);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(LOGICAL_OR)));
		parent->add(e1());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e1() {
	NodePtr n = make_unique<Node>(E1);
	n->add(e2());
	while (LOGICAL_AND == currtype()) {
		NodePtr parent = make_unique<Node>(E1);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(LOGICAL_AND)));
		parent->add(e2());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e2() {
	NodePtr n = make_unique<Node>(E2);
	n->add(e2b());
	TokenType type;
	while (EQUAL_DOUBLE == (type = currtype()) || NOT_EQUAL == type) {
		NodePtr parent = make_unique<Node>(E2);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(type)));
		parent->add(e2b());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e2b() {
	NodePtr n = make_unique<Node>(E2b);
	n->add(e3());
	TokenType type;
	while (GREATER == (type = currtype()) || LESS == type || GREATER_EQUAL == type || LESS_EQUAL == type) {
		NodePtr parent = make_unique<Node>(E2b);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(type)));
		parent->add(e3());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e3() {
	NodePtr n = make_unique<Node>(E3);
	n->add(e3b());
	while (BITWISE_OR == currtype()) {
		NodePtr parent = make_unique<Node>(E3);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(BITWISE_OR)));
		parent->add(e3b());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e3b() {
	NodePtr n = make_unique<Node>(E3b);
	n->add(e3c());
	while (BITWISE_XOR == currtype()) {
		NodePtr parent = make_unique<Node>(E3b);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(BITWISE_XOR)));
		parent->add(e3c());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e3c() {
	NodePtr n = make_unique<Node>(E3c);
	n->add(e3d());
	while (BITWISE_AND == currtype()) {
		NodePtr parent = make_unique<Node>(E3c);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(BITWISE_AND)));
		parent->add(e3d());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e3d() {
	NodePtr n = make_unique<Node>(E3d);
	n->add(e4());
	TokenType type;
	while (SHIFT_RIGHT == (type = currtype()) || SHIFT_LEFT == type) {
		NodePtr parent = make_unique<Node>(E3d);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(type)));
		parent->add(e4());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e4() {
	NodePtr n = make_unique<Node>(E4);
	n->add(e5());
	TokenType type;
	while (PLUS == (type = currtype()) || MINUS == type) {
		NodePtr parent = make_unique<Node>(E4);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(type)));
		parent->add(e5());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e5() {
	NodePtr n = make_unique<Node>(E5);
	n->add(e6());
	TokenType type;
	while (STAR == (type = currtype()) || SLASH == type || MODULO == type) {
		NodePtr parent = make_unique<Node>(E5);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(type)));
		parent->add(e6());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e6() {
	NodePtr n = make_unique<Node>(E6);
	n->add(e7());
	TokenType type;
	while (LOGICAL_NOT == (type = currtype()) || MINUS == type || BITWISE_NOT == type) {
		NodePtr parent = make_unique<Node>(E6);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(type)));
		parent->add(e7());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e7() {
	NodePtr n = make_unique<Node>(E7);
	n->add(e8());
	while (POWER == currtype()) {
		NodePtr parent = make_unique<Node>(E7);
		parent->add(std::move(n));
		parent->add(make_unique<Node>(match(POWER)));
		parent->add(e8());
		n = std::move(parent);
	}
	return n;
}

NodePtr Parser::e8() {
	NodePtr n = make_unique<Node>(E8);
	n->add(literal());
	return n;
}

NodePtr Parser::literal() {
	NodePtr n = make_unique<Node>(LITERAL);
	switch (currtype()) {
		case NULL_LITERAL: n->add(make_unique<Node>(match(NULL_LITERAL))); break;
		case NUMBER: n->add(make_unique<Node>(match(NUMBER))); break;
		case STRING: n->add(make_unique<Node>(match(STRING))); break;
		case BOOLEAN: n->add(make_unique<Node>(match(BOOLEAN))); break;
		default: throw ParserException(UNEXPECTED_TOKEN, "Expected literal value");
	}
	return n;
}

NodePtr Parser::ternary_operator() {
	NodePtr n = make_unique<Node>(TERNARY_OPERATOR);
	n->add(e0());
	n->add(match(QUESTION_MARK));
	n->add(expression());
	n->add(match(COLON));
	n->add(expression());
	return n;
}
