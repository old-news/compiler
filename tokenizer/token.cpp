#pragma once

#include "../utils.h"
#include "token.h"

#include <string>

using namespace std;

string to_string(TokenType type) {
	switch (type) {
		// Special
		case INVALID: return "INVALID";
		case END: return "END";

		case INDENT: return "INDENT";
		case DEDENT: return "DEDENT";
		case NEWLINE: return "NEWLINE";

		// Identifiers and literals
		case ID: return "ID";
		case NUMBER: return "NUMBER";
		case STRING: return "STRING";
		case NULL_LITERAL: return "NULL_LITERAL";

		// Keywords
		case IF: return "IF";
		case ELIF: return "ELIF";
		case ELSE: return "ELSE";
		case SWITCH: return "SWITCH";
		case CASE: return "CASE";
		case DEFAULT: return "DEFAULT";
		case FOR: return "FOR";
		case TO: return "TO";
		case IN: return "IN";
		case WHILE: return "WHILE";
		case RETURN: return "RETURN";
		case BREAK: return "BREAK";
		case CONTINUE: return "CONTINUE";
		case THROW: return "THROW";
		case TRY: return "TRY";
		case EXCEPT: return "EXCEPT";
		case FINALLY: return "FINALLY";
		case ENUM: return "ENUM";
		case IMPORT: return "IMPORT";
		case FROM: return "FROM";
		case AS: return "AS";
		case CONST: return "CONST";
		case BOOL: return "BOOL";
		case INT: return "INT";
		case STRING_TYPE: return "STRING_TYPE";
		case CHAR: return "CHAR";
		case FLOAT: return "FLOAT";
		case TRUE: return "TRUE";
		case FALSE: return "FALSE";
		case CLASS: return "CLASS";
		case PRIVATE: return "PRIVATE";
		case PROTECTED: return "PROTECTED";
		case PUBLIC: return "PUBLIC";
		case CHILDOF: return "CHILDOF";
		case TEMPLATE: return "TEMPLATE";
		case ADHERESTO: return "ADHERESTO";

		// Delimiters
		case OPEN_PARENTHESIS: return "OPEN_PARENTHESIS";
		case CLOSE_PARENTHESIS: return "CLOSE_PARENTHESIS";
		case OPEN_CURLY: return "OPEN_CURLY";
		case CLOSE_CURLY: return "CLOSE_CURLY";
		case OPEN_BRACKET: return "OPEN_BRACKET";
		case CLOSE_BRACKET: return "CLOSE_BRACKET";
		case SEMICOLON: return "SEMICOLON";
		case COLON: return "COLON";
		case COMMA: return "COMMA";
		case PERIOD: return "PERIOD";
		case ARROW: return "ARROW";
		case QUESTION_MARK: return "QUESTION_MARK";
		case AT_SYMBOL: return "AT_SYMBOL";
		case MONEY_SIGN: return "MONEY_SIGN";

		// Logical / comparison operators
		case LOGICAL_OR: return "LOGICAL_OR";
		case LOGICAL_AND: return "LOGICAL_AND";
		case LOGICAL_NOT: return "LOGICAL_NOT";
		case EQUAL_DOUBLE: return "EQUAL_DOUBLE";
		case NOT_EQUAL: return "NOT_EQUAL";
		case LESS: return "LESS";
		case GREATER: return "GREATER";
		case LESS_EQUAL: return "LESS_EQUAL";
		case GREATER_EQUAL: return "GREATER_EQUAL";

		// Bitwise operators
		case BITWISE_OR: return "BITWISE_OR";
		case BITWISE_XOR: return "BITWISE_XOR";
		case BITWISE_AND: return "BITWISE_AND";
		case SHIFT_LEFT: return "SHIFT_LEFT";
		case SHIFT_RIGHT: return "SHIFT_RIGHT";
		case BITWISE_NOT: return "BITWISE_NOT";

		// Arithmetic operators
		case PLUS: return "PLUS";
		case MINUS: return "MINUS";
		case STAR: return "STAR";
		case SLASH: return "SLASH";
		case MODULO: return "MODULO";
		case POWER: return "POWER";
		case INTEGER_DIVIDE: return "INTEGER_DIVIDE";
		case INCREMENT: return "INCREMENT";
		case DECREMENT: return "DECREMENT";

		// Assignment operators
		case ASSIGN: return "ASSIGN";
		case ASSIGN_TYPED: return "ASSIGN_TYPED";
		case ASSIGN_PLUS: return "ASSIGN_PLUS";
		case ASSIGN_MINUS: return "ASSIGN_MINUS";
		case ASSIGN_STAR: return "ASSIGN_STAR";
		case ASSIGN_SLASH: return "ASSIGN_SLASH";
		case ASSIGN_POWER: return "ASSIGN_POWER";
		case ASSIGN_MODULO: return "ASSIGN_MODULO";
		case ASSIGN_INTEGER_DIVIDE: return "ASSIGN_INTEGER_DIVIDE";
		case ASSIGN_OR: return "ASSIGN_OR";
		case ASSIGN_XOR: return "ASSIGN_XOR";
		case ASSIGN_AND: return "ASSIGN_AND";
		case ASSIGN_SHIFT_LEFT: return "ASSIGN_SHIFT_LEFT";
		case ASSIGN_SHIFT_RIGHT: return "ASSIGN_SHIFT_RIGHT";
	}
	return "UNKNOWN";
}

Token::Token() : type(INVALID), value(""), pos({0, 0}) {}

		Token::Token(TokenType ty, const string& text = "", size_t ln = 0, size_t col = 0)
			: type(ty), value(text), pos({ln, col}) {}

		TokenType Token::getType() const {
			return type;
		}

		const string& Token::getValue() const {
			return value;
		}

		size_t Token::getLine() const {
			return pos.line;
		}

		size_t Token::getColumn() const {
			return pos.col;
		}

		Pos Token::getPos() const {
			return pos;
		}

		bool Token::is(TokenType ty) const {
			return type == ty;
		}

		string Token::toString() {
			return "(" + to_string(type) + ", '" + value + "', [" + std::to_string(pos.line) + ", " + std::to_string(pos.col) + "])";
		}

		bool Token::isLiteral() {
			return NULL_LITERAL == type || NUMBER == type || STRING == type || TRUE == type || FALSE == type;
		}

		bool Token::isValid() {
			return INVALID != type;
		}
