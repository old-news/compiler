#include "node.h"
#include "parser.h"
#include "parser_exception.h"
#include <memory>

NodePtr Parser::import_line() {
	NodePtr n = make_unique<Node>(IMPORT_LINE);
	match(IMPORT);
	n->add(IMPORTEE, importee());
	match(FROM);
	n->add(LOCATION, id_chain());
	return n;
}

NodePtr Parser::importee() {
	NodePtr n = make_unique<Node>(IMPORTEE);
	while (FROM != peek(1).getType()) {
		n->add(match(ID));
		if (tryMatch(PERIOD).is(INVALID)) {
			break;
		}
	}
	if (PERIOD == unpeek(1).getType()) {
		TokenType t = currtype();
		if (ID != t && STAR != t) {
			// I think this never gets thrown actually
			throw ParserException(UNEXPECTED_TOKEN, "Expected something after period for import", getpos());
		}
		n->add(match(t));
	}
	return n;
}

NodePtr Parser::id_chain() {
	NodePtr n = make_unique<Node>(LIST);
	n->add(match(ID));
	while (PERIOD == currtype()) {
		match(PERIOD);
		n->add(match(ID));
	}
	return n;
}
