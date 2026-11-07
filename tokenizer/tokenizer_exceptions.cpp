#pragma once
#include <string>

using namespace std;

enum TokenizerExceptionType  {
	UNTERMINATED_STRING,
	UNMATCHED_PARENTHESIS,
	INVALID_INDENT,
	SOOK_PAST_INPUT
};

string to_string(TokenizerExceptionType t) {
	switch (t) {
		case UNTERMINATED_STRING: return "UNTERMINATED_STRING";
		case UNMATCHED_PARENTHESIS: return "UNMATCHED_PARENTHESIS";
		case INVALID_INDENT: return "INVALID_INDENT";
		case SOOK_PAST_INPUT: return "SOOK_PAST_INPUT";
	}
	return "UNKNOWN";
}

class TokenizerException : public exception {
	private:
		TokenizerExceptionType  type;
		string message;
	public:
		TokenizerException (const string& msg) : message(msg) {};
		TokenizerException (const TokenizerExceptionType & ntype) : type(ntype) {};
		TokenizerException (const TokenizerExceptionType & ntype, const string& msg) : type(ntype), message(msg) {};

		string getMessage() const { return message; }
		TokenizerExceptionType getType() const { return type; }
		string toString() const {
			return "Tokenizer Exception (" + to_string(type) + "): " + message;
		}
		const char* what() const noexcept override {
			return message.c_str();
		}
};
