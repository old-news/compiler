#include "tokenizer.cpp"

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

enum FLAG {
	BAD, OUTPUT_NAME, INPUT_NAME,
};

unordered_map<FLAG, string> parse_flags(int argc, char** argv) {
	unordered_map<FLAG, string> map;
	for (int i = 1; i < argc; i++) {
		if ("-o" == (string) argv[i]) {
			if (map.find(OUTPUT_NAME) != map.end()) {
				throw runtime_error("Only one output '-o' flag allowed");
			}
			i++;
			map.emplace(OUTPUT_NAME, argv[i]);
		}
		if ('-' != (char) argv[i][0]) {
			if (map.find(INPUT_NAME) != map.end()) {
				throw runtime_error("You can only compile one file at a time");
			}
			map.emplace(INPUT_NAME, argv[i]);
		}
	}
	if (map.find(OUTPUT_NAME) == map.end()) {
		map.emplace(OUTPUT_NAME, "a.out");
	}
	return map;
}

string read_file(string filename) {
	fstream ifile(filename);
	if (!ifile) {
		return "BROKEN";
	}
	ostringstream ss;
	ss << ifile.rdbuf();
	return ss.str();
}

void write_file(string filename, string output) {
	ofstream ofile(filename);
	ofile << output;
}

int main(int argc, char** argv) {
	unordered_map<FLAG, string> flags = parse_flags(argc, argv);
	string source_code = read_file(flags.at(INPUT_NAME));
	string stdlib_code = read_file("stdlib.bv");
	string input_code = stdlib_code + '\n' + source_code;
	vector<Token> tokens = Tokenizer(input_code).tokenize();
	Tree parsed = Parser(tokens).parse();
	vector<exception> errors = Validator(parsed).validate();
	if (!errors.empty()) {
		for (auto error : errors) {
			cout << "Error: " << error.what() << endl;
		}
		return -1;
	}
	IR ir = InternalRepresentation(parsed).generate();
	IR optimized = Optimizer(ir).optimize();
	string assembly = Objectifier(optimized).objectify();
	vector<uint8_t> assembled = Assembler(assembly).assemble();
	write_file("a.out", assembled);
}
