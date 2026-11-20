#include "node.h"
#include "parser.h"
#include "parser_exception.h"
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
	size_t i = 0;
	while (NEWLINE == input[pos + i].getType()) {
		i++;
	}
	return input[pos + n + i];
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
		if (NEWLINE == currtype()) {
			advance();
		}
		return c;
	}
	throw ParserException(
		UNEXPECTED_TOKEN,
		"Failed token match of '" + to_string(type) + "'",
		getpos()
	);
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
	NodePtr n = tryParseStar(variable);
	if (n->children.empty()) {
		variable();
	}
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
	match(OPEN_CURLY);
	/*
	 * match(COLON);
	 * match(NEWLINE);
	 * match(INDENT);
	 */
}

void Parser::exit_scope() {
	match(CLOSE_CURLY);
	// match(DEDENT);
}

void Parser::loc_end() {
	if (SEMICOLON == currtype()) {
		advance();
	} else if (NEWLINE != unpeek(1).getType()) {
		// Throw an error when a loc_end is expected but isn't in the code
		match(SEMICOLON);
	}
}

bool Parser::atLoc_end() {
	return SEMICOLON == currtype() || NEWLINE == unpeek(1).getType();
}

NodePtr Parser::type() {
	NodePtr n = make_unique<Node>(TYPE);
	NodePtr c = tryParse([this] {return class_access_level();});
	if (c) {
		n->add(std::move(c));
	}
	if (CONST == currtype()) {
		n->add(match(CONST));
	}
	TokenType t = currtype();
	if (OPEN_CURLY == t && 0 != n->children.size() && CLASS_ACCESS == n->children[0]->type) {
		// Class function with no return type
		return n;
	}
	bool lastWasClass;
	do {
		lastWasClass = false;
		switch (t) {
			case AT_SYMBOL:
				n->add(match(AT_SYMBOL));
				break;
			case OPEN_BRACKET: {
				n->add(match(OPEN_BRACKET));
				if (CLOSE_BRACKET == currtype()) {
					n->add(match(CLOSE_BRACKET));
					break;
				}
				NodePtr c = tryParse([this] {return expression();});
				if (!c) {
					c = type();
				}
				n->add(match(CLOSE_BRACKET));
				n->add(std::move(c));
				break;
			}
			case ID:
				n->add(match(ID));
				lastWasClass = true;
				break;
			case INT: case BOOL: case STRING_TYPE: case CHAR: case FLOAT:
				n->add(match(t));
				lastWasClass = true;
				break;
			default:
				throw ParserException(UNEXPECTED_TOKEN, "Expected type", getpos());
		}
		if (atLoc_end()) {
			break;
		}
		t = currtype();
	} while (CLOSE_PARENTHESIS != t && OPEN_CURLY != t && ASSIGN != t && COMMA != t);
	if (!lastWasClass) {
		throw ParserException(UNEXPECTED_TOKEN, "Expected previous token to be a class", getpos());
	}
	return n;
}
