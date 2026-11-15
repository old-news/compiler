#pragma once

#include "parser.h"
#include <memory>

Parser::Parser(const vector<Token>& i) : input(i), pos(0) {};

Token Parser::curr() {
	return input[pos];
}

TokenType Parser::currtype() {
	return input[pos].getType();
}

Token Parser::unpeek(size_t n) {
	if (pos - n < 0) {
		throw ParserException(SOOK_PAST_END, "Unpeeked to before start", getpos());
	}
	return input[pos - n];
}

Token Parser::peek(size_t n) {
	return input[pos + n];
}

bool Parser::empty() {
	return pos >= input.size();
}

Pos Parser::getpos() {
	return curr().getPos();
}

Token Parser::match(TokenType type) {
	if (empty()) {
		throw ParserException(SOOK_PAST_END, "Matched beyond input end", getpos());
	}
	Token c = curr();
	if (type == c.getType()) {
		advance();
		return c;
	}
	throw ParserException(UNEXPECTED_TOKEN, "Failed token match", getpos());
}

Token Parser::tryMatch(TokenType type) {
	try {
		return match(type);
	} catch (ParserException e) {
		return Token();
	}
}

void Parser::advance() {
	pos++;
}

void Parser::retreat() {
	pos--;
}

NodePtr Parser::tryParse(function<NodePtr()> branch) {
	size_t save = pos;
	try {
		return branch();
	} catch (const ParserException& e) {
		pos = save;
		return nullptr;
	}
}

NodePtr Parser::tryParseStar(function<NodePtr()> branch) {
	NodePtr list = make_unique<Node>(LIST);
	NodePtr n;
	while (true) {
		n = tryParse(branch);
		if (!n) break;
		list->add(std::move(n));
	}
	return list;
}

NodePtr Parser::list(function<NodePtr()> variable) {
	NodePtr c = variable();
	NodePtr n = tryParseStar(variable);
	n->add(std::move(c));
	n->add(variable());
	return n;
}

NodePtr Parser::list(function<NodePtr()> variable, TokenType delim) {
	NodePtr n = make_unique<Node>(LIST);
	n->add(variable());
	while (delim == currtype()) {
		match(delim);
		n->add(variable());
	}
	return n;
}

NodePtr Parser::parse() {
	return Parser::program();
}

void Parser::enter_scope() {
	match(COLON);
	match(NEWLINE);
	match(INDENT);
}

void Parser::exit_scope() {
	match(DEDENT);
}

void Parser::loc_end() {
	match(NEWLINE);
}

NodePtr Parser::type() {
	NodePtr n = make_unique<Node>(TYPE);
	if (CONST == currtype()) {
		n->add(match(CONST));
	}
	TokenType t = currtype();
	switch (t) {
		case AT_SYMBOL:
			n->add(match(AT_SYMBOL));
			break;
		case OPEN_BRACKET: {
			match(OPEN_BRACKET);
			NodePtr c = tryParse([this] {return expression();});
			if (!c) {
				c = type();
			}
			match(CLOSE_BRACKET);
			n->add(std::move(c));
			break;
		}
		case ID:
			n->add(match(ID));
			break;
		case INT: case BOOL: case STRING_TYPE: case CHAR: case FLOAT:
			n->add(match(t));
			break;
		default:
			throw ParserException(UNEXPECTED_TOKEN, "Expected type", getpos());
	}
	if (ID != unpeek(1).getType()) {
		throw ParserException(UNEXPECTED_TOKEN, "Expected previous token to be a class", getpos());
	}
	return n;
}
