#pragma once

#include "../utils.h"
#include "parser_exception.h"

#include <string>

using namespace std;


string to_string(ParserExceptionType t) {
	switch (t) {
		case SOOK_PAST_END: return "SOOK_PAST_END";
		case UNEXPECTED_TOKEN: return "UNEXPECTED_TOKEN";
		case EMPTY_PROGRAM: return "EMPTY_PROGRAM";
	}
	return "UNKNOWN";
}

ParserException::ParserException (const string& msg, const Pos& p) : message(msg), pos(p) {buildWhatMessage();};
ParserException::ParserException (const ParserExceptionType& ntype, const Pos& p) : type(ntype), pos(p) {buildWhatMessage();};
ParserException::ParserException (const ParserExceptionType& ntype, const string& msg, const Pos& p) : type(ntype), message(msg), pos(p) {buildWhatMessage();};

		string ParserException::getMessage() const { return message; }
		ParserExceptionType ParserException::getType() const { return type; }
		string ParserException::toString() const {
			return "Tokenizer Exception (" + to_string(type) + "): " + what_message;
		}
		const char* ParserException::what() const noexcept {
			return what_message.c_str();
		}

		const void ParserException::buildWhatMessage() {
			what_message = message + " at " + pos.toString();
		}
