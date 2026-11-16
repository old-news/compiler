#pragma once

#include "../utils.h"

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
	NULL_LITERAL,

	// Keywords
	IF, ELIF, ELSE,
	SWITCH, CASE, DEFAULT,
	FOR, TO, IN, WHILE,
	RETURN, BREAK, CONTINUE,
	THROW, TRY, EXCEPT, FINALLY,
	ENUM,
	IMPORT, FROM, AS,
	CONST, BOOL, INT, STRING_TYPE, CHAR, FLOAT,
	TRUE, FALSE,
	CLASS, PRIVATE, PROTECTED, PUBLIC, CHILDOF, TEMPLATE, ADHERESTO,

	// Delimiters
	OPEN_PARENTHESIS, CLOSE_PARENTHESIS,   // ( )
	OPEN_CURLY, CLOSE_CURLY,               // { }
	OPEN_BRACKET, CLOSE_BRACKET,           // [ ]
	SEMICOLON,                             // ;
	COLON,                                 // :
	COMMA,                                 // ,
	PERIOD,                                // .
	ARROW,                                 // ->
	QUESTION_MARK,
	AT_SYMBOL,				// @
	MONEY_SIGN,				// $

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

string to_string(TokenType type);

class Token {
	private:
		TokenType type;
		string value;
		Pos pos;
	public:
		Token();
		explicit Token(TokenType ty, const string& text, size_t ln, size_t col);

		TokenType getType() const;
		const string& getValue() const;
		size_t getLine() const;
		size_t getColumn() const;
		Pos getPos() const;

		bool is(TokenType ty) const;
		string toString();
		bool isLiteral();
		bool isValid();
};
