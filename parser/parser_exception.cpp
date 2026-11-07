#pragma once

#include "../utils.h"

#include <string>

using namespace std;

enum ParserExceptionType  {
	SOOK_PAST_END,
	UNEXPECTED_TOKEN, EMPTY_PROGRAM,
};

string to_string(ParserExceptionType t) {
	switch (t) {
		case SOOK_PAST_END: return "SOOK_PAST_END";
		case UNEXPECTED_TOKEN: return "UNEXPECTED_TOKEN";
		case EMPTY_PROGRAM: return "EMPTY_PROGRAM";
	}
	return "UNKNOWN";
}

class ParserException : public exception {
	private:
		ParserExceptionType  type;
		string message;
		Pos pos;
		string what_message;
	public:
		ParserException (const string& msg, const Pos& p) : message(msg), pos(p) {buildWhatMessage();};
		ParserException (const ParserExceptionType& ntype, const Pos& p) : type(ntype), pos(p) {buildWhatMessage();};
		ParserException (const ParserExceptionType& ntype, const string& msg, const Pos& p) : type(ntype), message(msg), pos(p) {buildWhatMessage();};

		string getMessage() const { return message; }
		ParserExceptionType getType() const { return type; }
		string toString() const {
			return "Tokenizer Exception (" + to_string(type) + "): " + what_message;
		}
		const char* what() const noexcept override {
			return what_message.c_str();
		}

		const void buildWhatMessage() {
			what_message = message + " at " + pos.toString();
		}
};
