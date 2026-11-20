#include "../parser/node.h"
#include "location.h"

#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

class SymbolTable {
	private:
		unordered_map<string, unordered_set<Location>> functions;
		unordered_map<string, unordered_set<Location>> classes;
		unordered_map<string, unordered_set<Location>> enums;
		unordered_set<string> packages;
		unordered_map<string, unordered_set<Location>> variables;
	public:
		SymbolTable();

		void addVar(string name, Location l);
		void addFunc(string name, Location l);
		void addClass(string name, Location l);
		void addEnum(string name, Location l);

		unordered_set<Location> getFunc(string name);
		unordered_set<Location> getClass(string name);
		unordered_set<Location> getEnum(string name);
		unordered_set<Location> getVar(string name);

		string toString();
};
