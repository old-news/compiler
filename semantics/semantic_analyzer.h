#include "../parser/node.h"
#include "symbol_table.h"

class SemanticAnalyzer {
	private:
		NodePtr tree;
		SymbolTable st;
	public:
		SemanticAnalyzer(NodePtr);

		SymbolTable analyze();
		void analyze_rec(NodePtr node, Location loc);
};
