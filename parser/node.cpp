#pragma once

#include "../tokenizer/token.h"
#include "node.h"

#include <vector>
#include <memory>
#include <iostream>

using namespace std;

string toString(NodeType type) {
	switch (type) {
		case UNINITIALIZED: return "UNINITIALIZED";
		case PROGRAM: return "PROGRAM";
	        case COMPOUND_BLOCK: return "COMPOUND_BLOCK";
	        case BLOCK: return "BLOCK";
	        case TYPE: return "TYPE";
	        case LIST: return "LIST";
	        case NAME: return "NAME";
	        case BODY: return "BODY";
	        case CONDITION: return "CONDITION";
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
	        case VVALUE: return "VVALUE";
	        case V1: return "V1";
	        case V2: return "V2";
	        case V3: return "V3";
	        case VUNIT: return "VUNIT";
	        case ARRAY_INDEXING: return "ARRAY_INDEXING";
	        case ARRAY_EXPRESSION: return "ARRAY_EXPRESSION";
	        case FUNCTION_CALL: return "FUNCTION_CALL";
	        case FUNCTION_DEFINITION: return "FUNCTION_DEFINITION";
	        case ARGS: return "ARGS";
	        case VAR_LDECL: return "VAR_LDECL";
	        case ENUM_DEFINITION: return "ENUM_DEFINITION";
	        case IMPORT_LINE: return "IMPORT_LINE";
	        case IMPORTEE: return "IMPORTEE";
	        case LOCATION: return "LOCATION";
	        case ERROR_BLOCK: return "ERROR_BLOCK";
	        case TRY_BLOCK: return "TRY_BLOCK";
	        case EXCEPT_BLOCK: return "EXCEPT_BLOCK";
	        case FINALLY_BLOCK: return "FINALLY_BLOCK";
	        case CLASS_DEFINITION: return "CLASS_DEFINITION";
	        case INHERITS_FROM: return "INHERITS_FROM";
	        case CLASS_FUNC_SECTION: return "CLASS_FUNC_SECTION";
	        case CLASS_FUNCTION: return "CLASS_FUNCTION";
	        case CLASS_ACCESS: return "CLASS_ACCESS";
	        case CLASS_VAR_SECTION: return "CLASS_VAR_SECTION";
	        case CLASS_VAR_GROUP: return "CLASS_VAR_GROUP";
	        case DESTRUCTOR: return "DESTRUCTOR";
	        case CONTROL_FLOW: return "CONTROL_FLOW";
	        case FOR_BLOCK: return "FOR_BLOCK";
	        case FOR_INNER: return "FOR_INNER";
	        case WHILE_BLOCK: return "WHILE_BLOCK";
	        case IF_BLOCK: return "IF_BLOCK";
	        case ELIF_BLOCK: return "ELIF_BLOCK";
	        case ELSE_BLOCK: return "ELSE_BLOCK";
	        case SWITCH_BLOCK: return "SWITCH_BLOCK";
	        case SWITCH_CASE: return "SWITCH_CASE";
	        case SWITCH_INNER: return "SWITCH_INNER";
	        case SWITCH_CASE_LIST: return "SWITCH_CASE_LIST";
	        case SWITCH_DEFAULT: return "SWITCH_DEFAULT";
	        case CASE_VALUE_LIST: return "CASE_VALUE_LIST";
	        case LOC: return "LOC";
	        case CONTROL_FLOW_INTERRUPTION: return "CONTROL_FLOW_INTERRUPTION";
	        case THROW_STATEMENT: return "THROW_STATEMENT";
	        case VAR_DECL: return "VAR_DECL";
	        case VAR_FORWARD_DECL: return "VAR_FORWARD_DECL";
	        case VAR_ASSIGNMENT: return "VAR_ASSIGNMENT";
	        case LVALUE: return "LVALUE";
	        case RVALUE: return "RVALUE";
	}
	return "UNKNOWN";
}

Node::Node(NodeType t) : type(t) {};
Node::Node(Token t) : type(TOKEN), token(t) {};
Node::Node(NodeType type, Token tok) : type(type), token(tok) {};
Node::Node(NodeType type, unique_ptr<Node> uniq) {
		type = type;
		children = std::move(uniq->children);
	}

	void Node::add(unique_ptr<Node> child) {
		children.push_back(std::move(child));
	}

	void Node::add(NodeType ntype, unique_ptr<Node> child) {
		child->type = ntype;
		children.push_back(std::move(child));
	}

	void Node::add(Token tchild) {
		add(make_unique<Node>(tchild));
	}

	void Node::add(NodeType ntype, Token tchild) {
		add(make_unique<Node>(ntype, tchild));
	}

	void Node::print() {
		cout << "DEPTH │ TREE" << endl << "──────┼─────" << endl;
		print(0);
	}

	unique_ptr<Node> Node::last() {
		return std::move(children.back());
	}

	void Node::print(int depth) {
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
