#include "tokenizer_exceptions.h"
#include <string>

using namespace std;

string to_string(TokenizerExceptionType t) {
	switch (t) {
		case UNTERMINATED_STRING: return "UNTERMINATED_STRING";
		case UNMATCHED_PARENTHESIS: return "UNMATCHED_PARENTHESIS";
		case INVALID_INDENT: return "INVALID_INDENT";
		case SOOK_PAST_INPUT: return "SOOK_PAST_INPUT";
	}
	return "UNKNOWN";
}

TokenizerException::TokenizerException (const string& msg) : message(msg) {};
TokenizerException::TokenizerException (const TokenizerExceptionType & ntype) : type(ntype) {};
TokenizerException::TokenizerException (const TokenizerExceptionType & ntype, const string& msg) : type(ntype), message(msg) {};

		string TokenizerException::getMessage() const { return message; }
		TokenizerExceptionType TokenizerException::getType() const { return type; }
		string TokenizerException::toString() const {
			return "Tokenizer Exception (" + to_string(type) + "): " + message;
		}
		const char* TokenizerException::what() const noexcept {
			return message.c_str();
		}
