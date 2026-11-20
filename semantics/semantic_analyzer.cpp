#include "semantic_analyzer.h"

SemanticAnalyzer::SemanticAnalyzer(NodePtr n) {
	st = {};
	tree = std::move(n);
}

SymbolTable SemanticAnalyzer::analyze() {
	analyze_rec(std::move(tree), {});
	return st;
}

void SemanticAnalyzer::analyze_rec(NodePtr node, Location loc) {
	bool hasBlockChildren = COMPOUND_BLOCK == node->type || BODY == node->type;
	switch (node->type) {
		case FUNCTION_DEFINITION:
			st.addFunc(node->getName(), loc);
			break;
		case ENUM_DEFINITION:
			st.addEnum(node->getName(), loc);
			break;
		case CLASS_DEFINITION:
			st.addClass(node->getName(), loc);
			break;
		case VAR_DECL:
			st.addVar(node->getName(), loc);
			break;
		case VAR_FORWARD_DECL:
			for (int i = 0; i < node->children[0]->children.size(); i++) {
				st.addVar((*node->find(LIST))->children[i]->gettValue(), loc);
			}
			break;
		case IMPORT_LINE:
			NodePtr* thing = node->find(IMPORTEE);
			string name = (*thing)->concatChildren();
			st.addPackage(name);
			break;
	}
	if (hasBlockChildren) {
		loc.push(0);
	}
	for (int i = 0; i < node->children.size(); i++) {
		if (hasBlockChildren) {
			loc.set_back(i);
		}
		analyze_rec(std::move(node->children[i]), loc);
	}
}
