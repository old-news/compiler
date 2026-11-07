#include "parser.h"
#include "parser_exception.cpp"
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
}

NodePtr Parser::class_def_body() {
	NodePtr n = make_unique<Node>(BODY);
	NodePtr c = tryParse([this] {return class_var_section();});
	if (c) {
		n->add(std::move(c));
		n->add(class_func_section());
	} else {
		n->add(class_func_section());
		n->add(class_var_section());
	}
	return n;
}

NodePtr Parser::class_func_section() {
	NodePtr n = make_unique<Node>(CLASS_FUNC_SECTION);
	n->add(class_function());
	NodePtr c;
	while ((c = tryParse([this] {return class_function();}))) {
		n->add(std::move(c));
	}
	return n;
}

NodePtr Parser::class_function() {
	NodePtr n = make_unique<Node>(CLASS_FUNCTION);
	n->add(class_access_level());
	TokenType t = currtype();
	if (MONEY_SIGN == currtype()) {
		n->add(DESTRUCTOR, match(MONEY_SIGN));
	}
	n->add(function_definition());
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
	NodePtr c;
	while ((c = tryParse([this] {return class_var_group();}))) {
		n->add(std::move(c));
	}
	return n;
}

NodePtr Parser::class_var_group() {
	NodePtr n = make_unique<Node>(CLASS_VAR_GROUP);
	n->add(class_access_level());
	n->add(type());
	n->add(list([this] {return make_unique<Node>(match(ID));}, COMMA));
	loc_end();
	return n;
}
