#include "parser.h"
#include <memory>

NodePtr Parser::class_definition() {
	NodePtr n = make_unique<Node>(CLASS_DEFINITION);
	match(CLASS);
	n->add(NAME, match(ID));
	if (CHILDOF == currtype()) {
		match(CHILDOF);
		n->add(INHERITS_FROM, match(ID));
	}
	enter_scope();
	n->add(class_def_body());
	exit_scope();
	return n;
}

NodePtr Parser::class_def_body() {
	NodePtr n = make_unique<Node>(CLASS_DEF_BODY);
	if (OPEN_PARENTHESIS == peek(1).getType()) {
		n->add(class_func_section());
		n->add(class_var_section());
	} else {
		n->add(class_var_section());
		n->add(class_func_section());
	}
	return n;
	/*
	 * NodePtr c = tryParse([this] {return class_var_section();});
	 * if (c) {
		 * n->add(std::move(c));
		 * n->add(class_func_section());
	 * } else {
		 * n->add(class_func_section());
		 * n->add(class_var_section());
	 * }
	 * return n;
	 */
}

NodePtr Parser::class_func_section() {
	NodePtr n = list([this] {return function_definition();});
	n->type = CLASS_FUNC_SECTION;
	return n;
}

NodePtr Parser::class_access_level() {
	TokenType t = currtype();
	if (PRIVATE != t && PUBLIC != t && PROTECTED != t) {
		throw ParserException(UNEXPECTED_TOKEN, "Expected a class access level", getpos());
	}
	return make_unique<Node>(CLASS_ACCESS, match(t));
}

NodePtr Parser::class_var_section() {
	NodePtr n = make_unique<Node>(CLASS_VAR_SECTION);
	n->add(var_decl());
	loc_end();
	NodePtr c;
	while (OPEN_PARENTHESIS != peek(1).getType()) {
		c = var_decl();
		n->add(std::move(c));
		loc_end();
	}
	return n;
}
