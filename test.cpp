#include "tokenizer.cpp"
#include "token.cpp"
#include "parser_expression.cpp"


#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string read_prog() {
	fstream ifile("prog");
	if (!ifile) {
		ifile.close();
		ifile.open("prog.py");
		if (!ifile) {
			return "BROKEN";
		}
	}
	ostringstream ss;
	ss << ifile.rdbuf();
	return ss.str();
}

int main() {
	string input = read_prog();
	Tokenizer t(input);
	vector<Token> toks = t.tokenize();
	for (Token tok : toks) {
		cout << tok.toString() << endl;
	}
	NodePtr tree = Parser(toks).parse();
	tree->print();
}
