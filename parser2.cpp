#include "token.cpp"
#include "parser_exception.cpp"

#include <exception>
#include <stdexcept>
#include <system_error>
#include <vector>
#include <memory>
#include <functional>

using namespace std;

enum NodeType {
	UNINITIALIZED,PROGRAM, COMPOUND_BLOCK, BLOCK, FUNCTION_DEFINITION,
	LIST,
	TOKEN
};

struct Node {
	NodeType type;
	Token token;
	vector<unique_ptr<Node>> children;

	explicit Node(NodeType t) : type(t) {};

	void add(unique_ptr<Node> child) {
		children.push_back(child);
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

		bool empty() {
			return pos >= input.size();
		}

		Token match(TokenType type) {
			if (empty()) {
				throw ParserException(SOOK_PAST_INPUT, "Matched beyond input end");
			}
			Token c = curr();
			if (type == c.getType()) {
				advance();
				return c;
			}
			throw ParserException(UNEXPECTED_TOKEN, "Failed token match");
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
			return program();
		}

		NodePtr program() {
			NodePtr n = make_unique<Node>(PROGRAM);
			n->add(compoundBlock());
			match(END);
			return n;
		}

		NodePtr compoundBlock() {
			NodePtr n = tryParseStar([this] { return block(); });
			if (!n) {
				throw ParserException(EMPTY_PROGRAM, "cannot compile empty program");
			}
			n->type = COMPOUND_BLOCK;
			return n;
		}

		NodePtr block() {
			NodePtr n = make_unique<Node>(BLOCK);
			switch (curr().getType()) {
				case CLASS:
					n->add(class_definition());
					break;
				case ENUM:
					n->add(enum_definition());
					break;
				case IMPORT:
					n->add(import());
					break;
				case TRY:
					n->add(error_handling());
					break;
				case FOR: case WHILE: case IF: case SWITCH:
					n->add(control_flow());
					break;
				default: {
					NodePtr c = tryParse([this] { return loc(); });
					if (c) {
						n->add(std::move(c));
						break;
					}
					c = function_definition();
					n->add(std::move(c));
					break;
				}
			}
			return n;
		}
};
