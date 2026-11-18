#include "parser.h"

NodePtr Parser::loc() {
	NodePtr n = make_unique<Node>(LOC);
	switch (currtype()) {
		case RETURN: case BREAK: case CONTINUE:
			n->add(control_flow_interruption());
			break;
		case THROW:
			n->add(throw_statement());
			break;
		default:
			NodePtr c = tryParse([this] {return function_call();});
			if (c) {
				n->add(std::move(c));
				break;
			}
			c = tryParse([this] {return var_decl();});
			if (c) {
				n->add(std::move(c));
				break;
			}
			n->add(var_assignment());
	}
	loc_end();
	return n;
}

NodePtr Parser::control_flow_interruption() {
	NodePtr n = make_unique<Node>(CONTROL_FLOW_INTERRUPTION);
	TokenType t;
	switch ((t = currtype())) {
		case RETURN:
			n->add(match(RETURN));
			try {
				loc_end();
			} catch (const ParserException& e) {
				n->add(expression());
			}
			break;
		case BREAK: case CONTINUE:
			n->add(match(t));
			break;
		default:
			throw ParserException(UNEXPECTED_TOKEN, "Control flow interruption expected", getpos());
	}
	return n;
}

NodePtr Parser::throw_statement() {
	NodePtr n = make_unique<Node>(THROW_STATEMENT);
	match(THROW);
	n->add(expression());
	return n;
}

NodePtr Parser::var_decl() {
	NodePtr n = make_unique<Node>(VAR_DECL);
	if (COMMA == peek(1).getType()) {
		n = var_forward_decl();
		return n;
	}
	n->add(NAME, match(ID));
	if (ASSIGN_TYPED == currtype()) {
		n->add(match(ASSIGN_TYPED));
		n->add(expression());
	} else {
		match(COLON);
		NodePtr c = tryParse([this] {return class_access_level();});
		if (c) {
			n->add(std::move(c));
		}
		n->add(type());
		TokenType t = currtype();
		if (ASSIGN == t || ASSIGN_TYPED == t) {
			n->add(match(t));
			n->add(expression());
		}
	}
	return n;
}

NodePtr Parser::var_forward_decl() {
	NodePtr n = make_unique<Node>(VAR_FORWARD_DECL);
	n->add(list([this] {return make_unique<Node>(match(ID));}, COMMA));
	match(COLON);
	n->add(type());
	return n;
}

NodePtr Parser::var_assignment() {
	NodePtr n = make_unique<Node>(VAR_ASSIGNMENT);
	n->add(LVALUE, vvalue());
	TokenType t = currtype();
	switch (t) {
		case ASSIGN: case ASSIGN_PLUS: case ASSIGN_MINUS: case ASSIGN_STAR: case ASSIGN_SLASH: case ASSIGN_POWER: case ASSIGN_MODULO: case ASSIGN_OR: case ASSIGN_XOR: case ASSIGN_AND: case ASSIGN_SHIFT_LEFT: case ASSIGN_SHIFT_RIGHT:
			match(t);
			break;
		default:
			throw ParserException(UNEXPECTED_TOKEN, "Expected an assignment or modified assignment operator", getpos());
	}
	n->add(RVALUE, expression());
	return n;
}
