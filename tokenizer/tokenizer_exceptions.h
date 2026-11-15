#pragma once
#include <string>

using namespace std;

enum TokenizerExceptionType  {
	UNTERMINATED_STRING,
	UNMATCHED_PARENTHESIS,
	INVALID_INDENT,
	SOOK_PAST_INPUT
};

string to_string(TokenizerExceptionType t);

class TokenizerException : public exception {
	private:
		TokenizerExceptionType  type;
		string message;
	public:
		TokenizerException (const string& msg);
		TokenizerException (const TokenizerExceptionType & ntype);
		TokenizerException (const TokenizerExceptionType & ntype, const string& msg);

		string getMessage() const;
		TokenizerExceptionType getType() const;
		string toString() const;
		const char* what() const noexcept override;
};
