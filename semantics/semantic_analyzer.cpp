#include "semantic_analyzer.h"

SemanticAnalyzer::SemanticAnalyzer(NodePtr n) {
	st = {};
	tree = std::move(n);
}

SymbolTable SemanticAnalyzer::analyze() {
	return st;
}
