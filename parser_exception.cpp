#include <string>

using namespace std;

enum ParserExceptionType  {
	SOOK_PAST_INPUT,
	UNEXPECTED_TOKEN, EMPTY_PROGRAM,
};

string to_string(ParserExceptionType t) {
	switch (t) {
		case SOOK_PAST_INPUT: return "SOOK_PAST_INPUT";
		case UNEXPECTED_TOKEN: return "UNEXPECTED_TOKEN";
		case EMPTY_PROGRAM: return "EMPTY_PROGRAM";
	}
	return "UNKNOWN";
}

class ParserException : public exception {
	private:
		ParserExceptionType  type;
		string message;
	public:
		ParserException (const string& msg) : message(msg) {};
		ParserException (const ParserExceptionType & ntype) : type(ntype) {};
		ParserException (const ParserExceptionType & ntype, const string& msg) : type(ntype), message(msg) {};

		string getMessage() const { return message; }
		ParserExceptionType getType() const { return type; }
		string toString() const {
			return "Tokenizer Exception (" + to_string(type) + "): " + message;
		}
		const char* what() const noexcept override {
			return message.c_str();
		}
};
