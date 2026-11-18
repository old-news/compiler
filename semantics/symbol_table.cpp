#include "symbol_table.h"
#include <unordered_map>
#include <string>

SymbolTable::SymbolTable() {
	functions = {};
	classes = {};
	enums = {};
	packages = {};
	variables = {};
}

void SymbolTable::addFunc(string name, Location l) {
	functions[name].emplace(l);
}

void SymbolTable::addClass(string name, Location l) {
	classes[name].emplace(l);
}
void SymbolTable::addEnum(string name, Location l) {
	enums[name].emplace(l);
}

void SymbolTable::addVar(string name, Location l) {
	variables[name].emplace(l);
}

unordered_set<Location> SymbolTable::getFunc(string name) {
	return functions[name];
}

unordered_set<Location> SymbolTable::getClass(string name) {
	return classes[name];
}

unordered_set<Location> SymbolTable::getEnum(string name) {
	return enums[name];
}

unordered_set<Location> SymbolTable::getVar(string name) {
	return variables[name];
}
