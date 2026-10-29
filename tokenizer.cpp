#include "token.cpp"
#include "tokenizer_exceptions.cpp"
#include "utils.h"

#include <iostream>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>

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
		Tokenizer(const string& in) {
			input = in;
			pos = 0;
			filepos = {1, 1};
			should_indent_increase = false;
			previous_indent = "";
			indent_gcd = 0;
			init_keywords();
		}

		void init_keywords() {
			keywords = {
				{"if", IF}, {"elif", ELIF}, {"else", ELSE},
				{"switch", SWITCH}, {"case", CASE}, {"default", DEFAULT},
				{"for", FOR}, {"while", WHILE},
				{"break", BREAK}, {"continue", CONTINUE},
				{"throw", THROW}, {"try", TRY}, {"except", EXCEPT}, {"finally", FINALLY},
				{"import", IMPORT}, {"from", FROM}, {"as", AS},
				{"const", CONST}, {"int", INT}, {"float", FLOAT}, {"char", CHAR}, {"str", STRING_TYPE}, {"bool", BOOL},
				{"return", RETURN}, {"enum", ENUM},

				{"null", NULL_LITERAL}, {"true", TRUE}, {"false", FALSE},
			};
		}

		vector<Token> tokenize() {
			while (pos < input.size()) {
				try {
					tokenize_next();
				} catch (TokenizerException e) {
					if (SOOK_PAST_INPUT != e.getType()) {
						throw e;
					}
					break;
				}
			}
			tokens.push_back(Token(END, "", filepos.line+1, 0));
			return tokens;
		}

		void tokenize_next() {
			char n = next();
			if ('\n' != n) {
				should_indent_increase = false;
			}
			switch (n) {
				case '\n':
					add(NEWLINE);
					handle_newline_indentation();
					break;
				case ' ': case '\t': case '\f':
					filepos.col++;
					pos++;
					break;
				case '(': add(OPEN_PARENTHESIS); break;
				case ')': add(CLOSE_PARENTHESIS); break;
				case '[': add(OPEN_BRACKET); break;
				case ']': add(CLOSE_BRACKET); break;
				case '{': add(OPEN_CURLY); break;
				case '}': add(CLOSE_CURLY); break;
				case ';': add(SEMICOLON); break;
				case ':':
					if ('=' == next(1)) {
						add(ASSIGN_TYPED, 2);
					} else {
						should_indent_increase = true;
						add(COLON);
					}
					break;
				case ',': add(COMMA); break;
				case '.': add(PERIOD); break;
				case '|':
					switch (next(1)) {
						case '|': add(LOGICAL_OR, 2); break;
						case '=': add(ASSIGN_OR, 2); break;
						default: add(BITWISE_OR);
					}
					break;
				case '&':
					switch (next(1)) {
						case '&': add(LOGICAL_AND, 2); break;
						case '=': add(ASSIGN_AND, 2); break;
						default: add(BITWISE_AND);
					}
					break;
				case '!':
					('=' == next(1)) ? add(NOT_EQUAL, 2) : add(LOGICAL_NOT);
					break;
				case '=':
					('=' == next(1)) ? add(EQUAL_DOUBLE, 2) : add(ASSIGN);
					break;
				case '<':
					switch (next(1)) {
						case '=': add(LESS_EQUAL, 2); break;
						case '<': add(SHIFT_LEFT, 2); break;
						default: add(LESS);
					}
					break;
				case '>':
					switch (next(1)) {
						case '=': add(GREATER_EQUAL, 2); break;
						case '>': add(SHIFT_RIGHT, 2); break;
						default: add(GREATER);
					}
					break;
				case '^':
					if ('=' == next(1)) {
						add(ASSIGN_XOR, 2);
					} else {
						add(BITWISE_XOR);
					}
					break;
				case '~': add(BITWISE_NOT); break;
				case '+':
					switch (next(1)) {
						case '=': add(ASSIGN_PLUS, 2); break;
						case '+': add(INCREMENT, 2); break;
						default: add(PLUS);
					}
					break;
				case '-':
					switch (next(1)) {
						case '=': add(ASSIGN_MINUS, 2); break;
						case '-': add(DECREMENT, 2); break;
						case '>': add(ARROW, 2); break;
						default: add(MINUS);
					}
					break;
				case '*':
					switch (next(1)) {
						case '=': add(ASSIGN_STAR, 2); break;
						case '*':
							  ('=' == next(3)) ? add(ASSIGN_POWER, 3) : add(POWER, 2);
							  break;
						default: add(STAR);
					}
					break;
				case '/':
					switch (next(1)) {
						case '=': add(ASSIGN_SLASH, 2); break;
						case '/':
							  ('=' == next(2)) ? add(ASSIGN_INTEGER_DIVIDE, 3) : add(INTEGER_DIVIDE, 2);
							  break;
						default: add(SLASH);
					}
					break;
				case '%':
					switch (next(1)) {
						case '=': add(ASSIGN_MODULO, 2); break;
						// case '%': add(??, 2); break;
						default: add(MODULO);
					}
					break;
				case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
					// NUMBER
					size_t seek = 1;
					char sook = next(seek);
					while (sook >= '0' && sook <= '9') {
						seek++;
						sook = next(seek);
					}
					size_t length = seek;
					add(NUMBER, length);
					break;
				}
				case '"': case '\'': {
					// STRING
					size_t seek = 1;
					char curr = next(seek);
					bool in_escape = false;
					while (curr != next() || in_escape) {
						in_escape = ('\\' == curr) ? !in_escape : false;
						seek++;
						curr = next(seek);
					}
					size_t length = seek + 1;
					add(STRING, length);
					break;
				}
				default: {
					// ID
					size_t seek = 1;
					char curr = next(seek);
					while (isidchar(curr)) {
						seek++;
						curr = next(seek);
					}
					size_t length = seek;
					string idstr = input.substr(pos, length);
					auto it = keywords.find(idstr);
					if (it == keywords.end()) {
						add(ID, length);
					} else {
						add(it->second, length);
					}
				}
			}
		}

		void add(TokenType type) {
			add(type, 1);
		}

		void add(TokenType type, size_t len) {
			add(type, input.substr(pos, len));
		}

		void add(TokenType type, string value) {
			tokens.push_back(Token(type, value, filepos.line, filepos.col));
			if ('\n' == input[pos + value.length() - 1]) {
				filepos.line++;
				filepos.col = 0;
			} else {
				filepos.col += value.length();
			}
			pos += value.length();
		}

		char next() {
			return next(0);
		}

		char next(size_t n) {
			if (pos + n >= input.size()) {
				throw TokenizerException(SOOK_PAST_INPUT, "sook past input length");
			}
			return input[pos + n];
		}

		bool iswspace(char c) {
			switch (c) {
				case ' ': case '\t': case '\n': case '\f': return true;
				default: return false;
			}
		}

		bool isidchar(char c) {
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
		}

		void handle_newline_indentation() {
			size_t seek = 0;
			char curr = next(seek);
			string indent = "";
			while (' ' == curr || '\t' == curr) {
				indent += curr;
				seek++;
				curr = next(seek);
			}
			if ('\n' == curr) {
				return;
			}
			if (previous_indent.size() < indent.size() && !should_indent_increase) {
				throw TokenizerException(INVALID_INDENT, "indent is too long");
			} else if (should_indent_increase && previous_indent.size() >= indent.size()) {
				throw TokenizerException(INVALID_INDENT, "indent is too short");
			}
			if (previous_indent.size() == indent.size()) {
				return;
			}
			if (0 == indent.size()) {
				for (int i = 0; i < previous_indent.size() / indent_gcd; i++) {
					add(DEDENT, "");
				}
				previous_indent = "";
				return;
			} 
			should_indent_increase = false;
			char indentChar = indent[0];
			if (count(indent.begin(), indent.end(), indentChar) != indent.size()) {
				throw TokenizerException(INVALID_INDENT, "indent has mixed tabs and spaces");
			}
			if (0 == previous_indent.size()) {
				add(INDENT, indent);
				previous_indent = indent;
				indent_gcd = indent.size();
				return;
			}
			if (indentChar != previous_indent[0]) {
				throw TokenizerException(INVALID_INDENT, "indent char is not consistent with previous indent");
			}
			int currGCD = gcd(previous_indent.size(), indent.size());
			if (indent_gcd != currGCD) {
				throw TokenizerException(INVALID_INDENT, "indent length scale is inconsistent");
			}
			if (previous_indent.size() < indent.size()) {
				if (indent.size() != previous_indent.size() + indent_gcd) {
					throw TokenizerException(INVALID_INDENT, "cannot have a double indent");
				}
				add(INDENT, indent);
				previous_indent = indent;
			} else {
				add(DEDENT, indent);
				for (size_t i = 1; i < (previous_indent.size() - indent.size()) / indent_gcd; i++) {
					add(DEDENT, "");
				}
				previous_indent = indent;
			}
		}
};
