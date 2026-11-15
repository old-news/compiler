#include "token.h"

#include <vector>
#include <unordered_map>

using namespace std;

class Tokenizer {
	private:
		string input;
		size_t pos;
		Pos filepos;
		vector<Token> tokens;
		unordered_map<string, TokenType> keywords;
		bool should_indent_increase;
		string previous_indent;
		int indent_gcd;
	
	public:
		Tokenizer(const string& in);

		void init_keywords();

		vector<Token> tokenize();

		void tokenize_next();

		void add(TokenType type);

		void add(TokenType type, size_t len);

		void add(TokenType type, string value);

		char next();

		char next(size_t n);

		bool iswspace(char c);

		bool isidchar(char c);

		void handle_newline_indentation();
};
