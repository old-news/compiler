#include "../parser/node.h"

#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

class SymbolTable {
	private:
		unordered_map<string, NodePtr> functions;
		unordered_map<string, NodePtr> classes;
		unordered_map<string, NodePtr> enums;
		unordered_set<string> packages;
		unordered_map<string, int> variables;
	public:
		SymbolTable();
};
