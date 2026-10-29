#include <iostream>
#include <string>
#include "string_handler.cpp"
#include "grammar.cpp"
#include "tokenizer.cpp"
#include "validator.h"
#include "tokenizer_exceptions.h"

string read_input_pipe() {
	string line;
	string input;

	while(getline(cin, line)) {
		input += line;
	}
	return input;
}

int main() {
	vector<RULE> rules = {
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::TERMINAL, "0"}, {CFG_TYPE::VARIABLE, "S"}, {CFG_TYPE::TERMINAL, "1"}}
		},
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::TERMINAL, "0"}, {CFG_TYPE::TERMINAL, "1"}}
		},
	};
	
	rules = {
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::TERMINAL, "0"}, {CFG_TYPE::VARIABLE, "S"}, {CFG_TYPE::TERMINAL, "0"}}
		},
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::TERMINAL, "1"}, {CFG_TYPE::VARIABLE, "S"}, {CFG_TYPE::TERMINAL, "1"}}
		},
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::TERMINAL, "1"}}
		},
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::TERMINAL, "0"}}
		},
	};

	rules = {
		{
			{CFG_TYPE::VARIABLE, "B"},
			{{CFG_TYPE::TERMINAL, "0"}, {CFG_TYPE::VARIABLE, "A"}}
		},
		{
			{CFG_TYPE::VARIABLE, "A"},
			{{CFG_TYPE::TERMINAL, "1"}, {CFG_TYPE::VARIABLE, "B"}}
		},
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::VARIABLE, "B"}}
		},
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::VARIABLE, "A"}}
		},
		{
			{CFG_TYPE::VARIABLE, "A"},
			{{CFG_TYPE::TERMINAL, "1"}}
		},
		{
			{CFG_TYPE::VARIABLE, "B"},
			{{CFG_TYPE::TERMINAL, "0"}}
		},
	};

	rules = {
		{
			{CFG_TYPE::VARIABLE, "T"},
			{{CFG_TYPE::TERMINAL, "1"}}
		},
		{
			{CFG_TYPE::VARIABLE, "T"},
			{{CFG_TYPE::TERMINAL, "0"}}
		},
		{
			{CFG_TYPE::VARIABLE, "M"},
			{{CFG_TYPE::VARIABLE, "T"}}
		},
		{
			{CFG_TYPE::VARIABLE, "T"},
			{{CFG_TYPE::VARIABLE, "T"}, {CFG_TYPE::VARIABLE, "T"}}
		},
		{
			{CFG_TYPE::VARIABLE, "M"},
			{{CFG_TYPE::VARIABLE, "M"}, {CFG_TYPE::TERMINAL, "*"}, {CFG_TYPE::VARIABLE, "T"}}
		},
		{
			{CFG_TYPE::VARIABLE, "E"},
			{{CFG_TYPE::VARIABLE, "M"}}
		},
		{
			{CFG_TYPE::VARIABLE, "E"},
			{{CFG_TYPE::VARIABLE, "E"}, {CFG_TYPE::TERMINAL, "-"}, {CFG_TYPE::VARIABLE, "M"}}
		},
		{
			{CFG_TYPE::VARIABLE, "E"},
			{{CFG_TYPE::VARIABLE, "E"}, {CFG_TYPE::TERMINAL, "+"}, {CFG_TYPE::VARIABLE, "M"}}
		},
	};

	rules = {
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::TERMINAL, "0"}, {CFG_TYPE::VARIABLE, "S"}, {CFG_TYPE::TERMINAL, "1"}}
		},
		{
			{CFG_TYPE::VARIABLE, "S"},
			{{CFG_TYPE::TERMINAL, "0"}, {CFG_TYPE::TERMINAL, "1"}}
		},
	};

	// cout << "Rules: " << endl;
	// cout << vRULE_to_str(rules) << endl;
	// Grammar g(rules, {CFG_TYPE::VARIABLE, "S"});
	// cout << "pre parse" << endl;
	// cout << "start tok: " << TOK_to_str(g.get_start_tok()) << endl;
	// string match = read_input_pipe();
	// string match = "";
	/*
	 * for (int i = 0; i < 10000; i++) {
		 * match += "0";
	 * }
	 * for (int i = 0; i < 10000; i++) {
		 * match += "1";
	 * }
	 */
	// match.pop_back();
	// cout << "Matching `" << match << "` string" << endl;
	// auto matches = g.parse(match);
	// cout << "Maches: " << matches << endl;
	// vector<vector<TOKEN>> trimmed = g.parse("1+10*1011");
	// vector<vector<TOKEN>> trimmed = tree.find_path({g.get_start_tok()});
	// cout << vvTOK_to_str(trimmed) << endl;
	// cout << Tree_TOK_to_str(matches, 0) << endl;
	// cout << remove_strings("hi there 'bob' thing with a 'cool' hat and \"mighty \\\"cool\\\"\" glasses") << endl;
	string code = "# this is a comment\n  # this is comment2;\nhello there 'bob' and (dylan with 2**3 churr:s";
	vector<Token> tokens = tokenize(code);
	for (Token t : tokens) {
		cout << t.toString() << endl;
	}
	Validator v(tokens);
	vector<TokenizerException> te = v.validate();
	for (TokenizerException e : te) {
		cout << te.toString() << endl;
	}
}

