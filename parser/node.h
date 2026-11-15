#pragma once

#include "../tokenizer/token.h"

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
	LOC,
		CONTROL_FLOW_INTERRUPTION, THROW_STATEMENT, VAR_DECL, VAR_FORWARD_DECL, VAR_ASSIGNMENT, LVALUE, RVALUE,
};

string toString(NodeType type);

struct Node {
	NodeType type;
	Token token;
	vector<unique_ptr<Node>> children;

	explicit Node(NodeType t);
	explicit Node(Token t);
	explicit Node(NodeType type, Token tok);
	explicit Node(NodeType type, unique_ptr<Node> uniq);

	void add(unique_ptr<Node> child);
	void add(NodeType ntype, unique_ptr<Node> child);
	void add(Token tchild);
	void add(NodeType ntype, Token tchild);
	void print();
	unique_ptr<Node> last();
	void print(int depth);
};

using NodePtr = unique_ptr<Node>;
