#pragma once

#include <string>

using namespace std;

enum TokenType {
	// Special
	INVALID,            // default-constructed / unrecognized token
	END,                // end of input

	INDENT, DEDENT, NEWLINE,

	// Identifiers and literals
	ID,
	NUMBER,
	STRING,
	BOOLEAN,
	NULL_LITERAL,

	// Keywords
	IF, ELIF, ELSE,
	SWITCH, CASE, DEFAULT,
	FOR, WHILE,
	RETURN, BREAK, CONTINUE,
	THROW, TRY, EXCEPT, FINALLY,
	ENUM,
	IMPORT, FROM, AS,
	CONST, BOOL, INT, STRING_TYPE, CHAR, FLOAT,
	TRUE, FALSE,

	// Delimiters
	OPEN_PARENTHESIS, CLOSE_PARENTHESIS,   // ( )
	OPEN_CURLY, CLOSE_CURLY,               // { }
	OPEN_BRACKET, CLOSE_BRACKET,           // [ ]
	SEMICOLON,                             // ;
	COLON,                                 // :
	COMMA,                                 // ,
	PERIOD,                                // .
	ARROW,                                 // ->

	// Logical / comparison operators
	LOGICAL_OR, LOGICAL_AND,               // || &&
	LOGICAL_NOT,                           // !
	EQUAL_DOUBLE, NOT_EQUAL,               // == !=
	LESS, GREATER,                         // < >
	LESS_EQUAL, GREATER_EQUAL,             // <= >=

	// Bitwise operators
	BITWISE_OR, BITWISE_XOR, BITWISE_AND,  // | ^ &
	SHIFT_LEFT, SHIFT_RIGHT,               // << >>
	BITWISE_NOT,                           // ~

	// Arithmetic operators
	PLUS, MINUS,                           // + -
	STAR, SLASH, MODULO,                  // * / %
	POWER, INTEGER_DIVIDE,                 // ** //
	INCREMENT, DECREMENT,                  // ++ --

	// Assignment operators
	ASSIGN,					// =
	ASSIGN_TYPED,				// :=
	ASSIGN_PLUS, ASSIGN_MINUS,		// += -=
	ASSIGN_STAR, ASSIGN_SLASH,		// *= /=
	ASSIGN_POWER, ASSIGN_MODULO,		// **= %=
	ASSIGN_INTEGER_DIVIDE,			// //=
	ASSIGN_OR, ASSIGN_XOR, ASSIGN_AND,     // |= ^= &=
	ASSIGN_SHIFT_LEFT, ASSIGN_SHIFT_RIGHT  // <<= >>=
};

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
		case BOOLEAN: return "BOOLEAN";
		case NULL_LITERAL: return "NULL_LITERAL";

		// Keywords
		case IF: return "IF";
		case ELIF: return "ELIF";
		case ELSE: return "ELSE";
		case SWITCH: return "SWITCH";
		case CASE: return "CASE";
		case DEFAULT: return "DEFAULT";
		case FOR: return "FOR";
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

class Token {
	private:
		TokenType type;
		string value;
		size_t line;
		size_t column;
	public:
		Token() : type(INVALID), value(""), line(0), column(0) {}

		Token(TokenType ty, const string& text = "", size_t ln = 0, size_t col = 0)
			: type(ty), value(text), line(ln), column(col) {}

		TokenType getType() const {
			return type;
		}

		const string& getValue() const {
			return value;
		}

		size_t getLine() const {
			return line;
		}

		size_t getColumn() const {
			return column;
		}

		bool is(TokenType ty) const {
			return type == ty;
		}

		string toString() {
			return "(" + to_string(type) + ", '" + value + "', [" + std::to_string(line) + ", " + std::to_string(column) + "])";
		}
};
