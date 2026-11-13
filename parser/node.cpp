#include "../tokenizer/token.cpp"

#include <vector>
#include <memory>
#include <iostream>

using namespace std;

enum NodeType {
	UNINITIALIZED, PROGRAM, COMPOUND_BLOCK, BLOCK, TYPE,
	LIST, NAME, BODY, CONDITION,
	TOKEN,
	EXPRESSION,
		E0, E1, E2, E2b, E3, E3b, E3c, E3d, E4, E5, E6, E7, E8, LITERAL, TERNARY_OPERATOR,
		VVALUE, V1, V2, V3, VUNIT, ARRAY_INDEXING, ARRAY_EXPRESSION, FUNCTION_CALL,
	FUNCTION_DEFINITION,
		ARGS, VAR_LDECL,
	ENUM_DEFINITION,
	IMPORT_LINE,
		IMPORTEE, LOCATION,
	ERROR_BLOCK,
		TRY_BLOCK, EXCEPT_BLOCK, FINALLY_BLOCK,
	CLASS_DEFINITION,
		INHERITS_FROM, CLASS_FUNC_SECTION, CLASS_FUNCTION, CLASS_ACCESS, CLASS_VAR_SECTION, CLASS_VAR_GROUP, DESTRUCTOR,
	CONTROL_FLOW,
		FOR_BLOCK, FOR_INNER, WHILE_BLOCK, IF_BLOCK, ELIF_BLOCK, ELSE_BLOCK, SWITCH_BLOCK, SWITCH_CASE, SWITCH_INNER, SWITCH_CASE_LIST, SWITCH_DEFAULT, CASE_VALUE_LIST,
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
	explicit Node(NodeType type, Token tok) : type(type), token(tok) {};
	explicit Node(NodeType type, unique_ptr<Node> uniq) : type(type), children(uniq->children) {};

	void add(unique_ptr<Node> child) {
		children.push_back(std::move(child));
	}

	void add(NodeType ntype, unique_ptr<Node> child) {
		child->type = ntype;
		children.push_back(std::move(child));
	}

	void add(Token tchild) {
		add(make_unique<Node>(tchild));
	}

	void add(NodeType ntype, Token tchild) {
		add(make_unique<Node>(ntype, tchild));
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
