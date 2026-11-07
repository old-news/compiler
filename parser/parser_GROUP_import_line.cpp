#include "parser.h"
#include <memory>

NodePtr Parser::import_line() {
	NodePtr n = make_unique<Node>(IMPORT_LINE);
	n->add(make_unique<Node>(IMPORTEE, id_chain()));
	match(FROM);
	n->add(make_unique<Node>(LOCATION, id_chain()));
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
