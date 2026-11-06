#include "token.cpp"
#include "parser_exception.cpp"

#include <exception>
#include <stdexcept>
#include <system_error>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <format>

using namespace std;

enum NodeType {
	UNINITIALIZED, PROGRAM, COMPOUND_BLOCK, BLOCK, FUNCTION_DEFINITION,
	LIST,
	TOKEN,
	EXPRESSION, E0, E1, E2, E2b, E3, E3b, E3c, E3d, E4, E5, E6, E7, E8, LITERAL, TERNARY_OPERATOR
};

string toString(NodeType type) {
	switch (type) {
		case UNINITIALIZED: return "UNINITIALIZED";
		case PROGRAM: return "PROGRAM";
		case COMPOUND_BLOCK: return "COMPOUND_BLOCK";
		case BLOCK: return "BLOCK";
		case FUNCTION_DEFINITION: return "FUNCTION_DEFINITION";
		case LIST: return "LIST";
		case TOKEN: return "TOKEN";
		case EXPRESSION: return "EXPRESSION";
		case E0: return "E0";
		case E1: return "E1";
		case E2: return "E2";
		case E2b: return "E2b";
		case E3: return "E3";
		case E3b: return "E3b";
		case E3c: return "E3c";
		case E3d: return "E3d";
		case E4: return "E4";
		case E5: return "E5";
		case E6: return "E6";
		case E7: return "E7";
		case E8: return "E8";
		case LITERAL: return "LITERAL";
		case TERNARY_OPERATOR: return "TERNARY_OPERATOR";
	}
	return "UNKNOWN";
}

struct Node {
	NodeType type;
	Token token;
	vector<unique_ptr<Node>> children;

	explicit Node(NodeType t) : type(t) {};

	explicit Node(Token t) : type(TOKEN), token(t) {};

	void add(unique_ptr<Node> child) {
		children.push_back(std::move(child));
	}

	void add(Token tchild) {
		children.push_back(make_unique<Node>(tchild));
	}

	void print() {
		cout << "DEPTH │ TREE" << endl << "──────┼─────" << endl;
		print(0);
	}

	void print(int depth) {
		string s = to_string(depth);
		if (s.size() < 5) {
			s.insert(0, 5 - s.size(), ' ');
		}
		cout << s << " │ ";
		for (int j = 0; j < depth; j++) {
			cout << ((INVALID == token.getType()) ? "\t" : "--------");
		}
		cout << ((INVALID == token.getType()) ? toString(type) : " " + token.getValue()) << endl;
		for (size_t i = 0; i < children.size(); i++) {
			children[i]->print(depth + 1);
		}
	}
};

using NodePtr = unique_ptr<Node>;

class Parser {
	private:
		vector<Token> input;
		size_t pos;
	public:
		Parser(const vector<Token>& i) : input(i) {
			pos = 0;
		};

		Token curr() {
			return input[pos];
		}

		TokenType currtype() {
			return input[pos].getType();
		}

		Token peek(size_t n) {
			return input[pos + n];
		}

		bool empty() {
			return pos >= input.size();
		}

		Token match(TokenType type) {
			if (empty()) {
				throw ParserException(SOOK_PAST_END, "Matched beyond input end");
			}
			Token c = curr();
			if (type == c.getType()) {
				advance();
				return c;
			}
			throw ParserException(UNEXPECTED_TOKEN, "Failed token match at " + to_string(pos));
		}

		void advance() {
			pos++;
		}

		NodePtr tryParse(function<NodePtr()> branch) {
			size_t save = pos;
			try {
				return branch();
			} catch (const ParserException& e) {
				pos = save;
				return nullptr;
			}
		}

		NodePtr tryParseStar(function<NodePtr()> branch) {
			NodePtr list = make_unique<Node>(LIST);
			NodePtr n;
			while (true) {
				n = tryParse(branch);
				if (!n) break;
				list->add(std::move(n));
			}
			return list;
		}

		NodePtr parse() {
			NodePtr n = expression();
			match(END);
			return n;
		}

		NodePtr expression() {
			NodePtr n = make_unique<Node>(EXPRESSION);
			NodePtr c = tryParse([this] {return ternary_operator();});
			if (c) {
				n->add(std::move(c));
				return n;
			} else {
				n->add(e0());
			}
			return n;
		}

		NodePtr e0() {
			NodePtr n = make_unique<Node>(E0);
			n->add(e1());
			while (LOGICAL_OR == currtype()) {
				NodePtr parent = make_unique<Node>(E0);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(LOGICAL_OR)));
				parent->add(e1());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e1() {
			NodePtr n = make_unique<Node>(E1);
			n->add(e2());
			while (LOGICAL_AND == currtype()) {
				NodePtr parent = make_unique<Node>(E1);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(LOGICAL_AND)));
				parent->add(e2());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e2() {
			NodePtr n = make_unique<Node>(E2);
			n->add(e2b());
			TokenType type;
			while (EQUAL_DOUBLE == (type = currtype()) || NOT_EQUAL == type) {
				NodePtr parent = make_unique<Node>(E2);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(type)));
				parent->add(e2b());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e2b() {
			NodePtr n = make_unique<Node>(E2b);
			n->add(e3());
			TokenType type;
			while (GREATER == (type = currtype()) || LESS == type || GREATER_EQUAL == type || LESS_EQUAL == type) {
				NodePtr parent = make_unique<Node>(E2b);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(type)));
				parent->add(e3());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e3() {
			NodePtr n = make_unique<Node>(E3);
			n->add(e3b());
			while (BITWISE_OR == currtype()) {
				NodePtr parent = make_unique<Node>(E3);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(BITWISE_OR)));
				parent->add(e3b());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e3b() {
			NodePtr n = make_unique<Node>(E3b);
			n->add(e3c());
			while (BITWISE_XOR == currtype()) {
				NodePtr parent = make_unique<Node>(E3b);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(BITWISE_XOR)));
				parent->add(e3c());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e3c() {
			NodePtr n = make_unique<Node>(E3c);
			n->add(e3d());
			while (BITWISE_AND == currtype()) {
				NodePtr parent = make_unique<Node>(E3c);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(BITWISE_AND)));
				parent->add(e3d());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e3d() {
			NodePtr n = make_unique<Node>(E3d);
			n->add(e4());
			TokenType type;
			while (SHIFT_RIGHT == (type = currtype()) || SHIFT_LEFT == type) {
				NodePtr parent = make_unique<Node>(E3d);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(type)));
				parent->add(e4());
				n = std::move(parent);
			}
			return n;
		}
		
		NodePtr e4() {
			NodePtr n = make_unique<Node>(E4);
			n->add(e5());
			TokenType type;
			while (PLUS == (type = currtype()) || MINUS == type) {
				NodePtr parent = make_unique<Node>(E4);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(type)));
				parent->add(e5());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e5() {
			NodePtr n = make_unique<Node>(E5);
			n->add(e6());
			TokenType type;
			while (STAR == (type = currtype()) || SLASH == type || MODULO == type) {
				NodePtr parent = make_unique<Node>(E5);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(type)));
				parent->add(e6());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e6() {
			NodePtr n = make_unique<Node>(E6);
			n->add(e7());
			TokenType type;
			while (LOGICAL_NOT == (type = currtype()) || MINUS == type || BITWISE_NOT == type) {
				NodePtr parent = make_unique<Node>(E6);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(type)));
				parent->add(e7());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e7() {
			NodePtr n = make_unique<Node>(E7);
			n->add(e8());
			while (POWER == currtype()) {
				NodePtr parent = make_unique<Node>(E7);
				parent->add(std::move(n));
				parent->add(make_unique<Node>(match(POWER)));
				parent->add(e8());
				n = std::move(parent);
			}
			return n;
		}

		NodePtr e8() {
			NodePtr n = make_unique<Node>(E8);
			n->add(literal());
			return n;
		}

		NodePtr literal() {
			NodePtr n = make_unique<Node>(LITERAL);
			switch (currtype()) {
				case NULL_LITERAL: n->add(make_unique<Node>(match(NULL_LITERAL))); break;
				case NUMBER: n->add(make_unique<Node>(match(NUMBER))); break;
				case STRING: n->add(make_unique<Node>(match(STRING))); break;
				case BOOLEAN: n->add(make_unique<Node>(match(BOOLEAN))); break;
				default: throw ParserException(UNEXPECTED_TOKEN, "Expected literal value");
			}
			return n;
		}

		NodePtr ternary_operator() {
			NodePtr n = make_unique<Node>(TERNARY_OPERATOR);
			n->add(e0());
			n->add(match(QUESTION_MARK));
			n->add(expression());
			n->add(match(COLON));
			n->add(expression());
			return n;
		}
};
