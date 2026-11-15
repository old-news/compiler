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
using NodePtr = unique_ptr<Node>;
