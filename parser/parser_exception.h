#pragma once

#include "../utils.h"

#include <string>

using namespace std;

enum ParserExceptionType  {
	SOOK_PAST_END,
	UNEXPECTED_TOKEN, EMPTY_PROGRAM,
};

string to_string(ParserExceptionType t);

class ParserException : public exception {
	private:
		ParserExceptionType  type;
		string message;
		Pos pos;
		string what_message;
	public:
		ParserException (const string& msg, const Pos& p);
		ParserException (const ParserExceptionType& ntype, const Pos& p);
		ParserException (const ParserExceptionType& ntype, const string& msg, const Pos& p);

		string getMessage() const;
		ParserExceptionType getType() const;
		string toString() const;
		const char* what() const noexcept override;

		const void buildWhatMessage();
};
