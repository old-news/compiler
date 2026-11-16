#include "../parser/node.h"

class SemanticAnalyzer {
	private:
		NodePtr tree;
		SymbolTable st;
	public:
		SemanticAnalyzer();

		SymbolTable analyze();
};
