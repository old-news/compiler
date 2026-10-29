 #include <vector>
#include <queue>
#include "token.h"
#include "tokenizer_exceptions.h"

using namespace std;

class Validator {
	private:
		vector<Token> tokens;
		vector<TokenizerException> tokenizerExceptions;

		TokenizerException createUnmatchedParenthesisException(Token t) {
			string para_name("");
			string para_face("");
			switch (t.getType()) {
				case CLOSE_PARENTHESIS:
					para_name = "parenthesis";
					para_face = "closing";
					break;
				case OPEN_PARENTHESIS:
					para_name = "parenthesis";
					para_face = "opening";
					break;
				case CLOSE_BRACKET:
					para_name = "bracket";
					para_face = "closing";
					break;
				case OPEN_BRACKET:
					para_name = "bracket";
					para_face = "opening";
					break;
				case CLOSE_CURLY:
					para_name = "curly brace";
					para_face = "closing";
					break;
				case OPEN_CURLY:
					para_name = "curly brace";
					para_face = "opening";
					break;
				default:
					throw runtime_error("Tried to create an unmatched parenthesis exception on a non-parenthesis token: " + t.toString());
			}
			return TokenizerException(UNMATCHED_PARENTHESIS, "Unmatched " + para_face + " " + para_name + " on line " + to_string(t.getLine()));
		}

	public:
		Validator(const vector<Token>& input) : tokens(input) {};

		vector<TokenizerException> validate() {
			match_parentheses();
			return tokenizerExceptions;
		}

		void match_parentheses() {
			queue<Token> paras;
			for (Token t : tokens) {
				if (t.getType() == OPEN_BRACKET || t.getType() == OPEN_CURLY || t.getType() == OPEN_PARENTHESIS) {
					paras.push(t);
					continue;
				}

				TokenType ttype = t.getType();
				if (!(ttype == CLOSE_PARENTHESIS || ttype != CLOSE_BRACKET || ttype == CLOSE_CURLY)) continue;
				TokenType frontType = paras.front().getType();

				if (
					(ttype == CLOSE_PARENTHESIS && frontType == OPEN_PARENTHESIS) ||
					(ttype == CLOSE_BRACKET && frontType == OPEN_BRACKET) ||
					(ttype == CLOSE_CURLY && frontType == OPEN_CURLY)
				   ) {
					paras.pop();
					continue;
				}
				tokenizerExceptions.push_back(createUnmatchedParenthesisException(t));
			}
			while (!paras.empty()) {
				Token t = paras.front();
				paras.pop();
				tokenizerExceptions.push_back(createUnmatchedParenthesisException(t));
			}
		}
};

