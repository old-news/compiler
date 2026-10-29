#include "tokenizer.cpp"
#include "token.cpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string read_prog() {
	fstream ifile("prog");
	if (!ifile) {
		return "BROKEN";
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
}
