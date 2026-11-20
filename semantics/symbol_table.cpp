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

string to_str(unordered_map<string, unordered_set<Location>> table) {
	string s = "";
	for (auto v : table) {
		s += v.first + "{\n";
		for (auto l : v.second) {
			s += "  " + l.toString() + "\n";
		}
		s += "}\n";
	}
	s += "\n";
	return s;
}

string SymbolTable::toString() {
	string f = "FUNCTIONS:\n" + to_str(functions);
	string c = "CLASSES:\n" + to_str(classes);
	string e = "ENUMS:\n" + to_str(enums);
	string v = "VARIABLES:\n" + to_str(variables);
	return f + c + e + v;
}
