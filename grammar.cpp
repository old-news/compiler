#include <regex>
#include <string>
#include <iostream>
#include <set>
#include <cmath>

using namespace std;

enum class TOKEN_TYPE {
	VARIABLE, TERMINAL
};

struct TOKEN {
	TOKEN_TYPE type;
	string value;

	bool operator==(const TOKEN& other) const {
		return type == other.type && value == other.value;
	}

	bool operator!=(const TOKEN& other) const {
		return !(*this == other);
	}

	bool operator<(const TOKEN& other) const {
		if (type != other.type) return type < other.type;
		return value < other.value;
	}
};

struct RULE {
	TOKEN lhs;
	vector<TOKEN> rhs;
};

struct RULE_EVALUATOR {
	TOKEN lhs;
	vector<TOKEN> rhs;
	size_t pos;
	vector<const RULE_EVALUATOR*> children;
	size_t origin;

	bool operator==(const RULE_EVALUATOR& other) const {
		return lhs == other.lhs && rhs == other.rhs && pos == other.pos && origin == other.origin;
	}

	bool operator!=(const RULE_EVALUATOR& other) const {
		return !(*this == other);
	}

	bool operator<(const RULE_EVALUATOR& other) const {
		if (lhs != other.lhs) return lhs < other.lhs;
		if (rhs != other.rhs) return rhs < other.rhs;
		if (pos != other.pos) return pos < other.pos;
		// if (parent != other.parent) return parent < other.parent;
		return origin < other.origin;
	}
};

template <typename T>
class Tree {
	private:
		T value;
		vector<Tree<T>> children;
	public:
		Tree() : children({}) {};

		Tree(T v) : value(v), children({}) {};

		Tree(vector<T> items) : children({}) {
			for (T item : items) {
				Tree<T> newTree = Tree();
				newTree.set_value(item);
				children.push_back(newTree);
			}
		}

		Tree(T v, vector<T> items) : Tree(items) {
			value = v;
		};

		T get_value() {
			return value;
		}

		void set_value(T v) {
			value = v;
		}

		void add(T item) {
			children.push_back(Tree(item));
		}

		void add(Tree<T> tree) {
			children.push_back(tree);
		}

		bool has_children() {
			return !children.empty();
		}

		vector<Tree<T>> get_children() {
			return children;
		}

		vector<T> find_path_rec(T target, vector<T> parent, vector<Tree<T>> children) {
			if (parent.back() == target) {
				return parent;
			}
			for (Tree<T> child : children) {
				parent.push_back(child.get_value());
				vector<T> result = find_path_rec(target, parent, child.get_children());
				parent.pop_back();
				if (!result.empty()) return result;
			}
			return {};
		}

		vector<T> find_path(T start_tok) {
			return find_path_rec(start_tok, {value}, children);
		}
};

string Tree_TOK_to_str(Tree<vector<TOKEN>> tree, int depth);
string REV_to_str(RULE_EVALUATOR rev);
string vREV_to_str(vector<RULE_EVALUATOR> vrev);

class Grammar {
	private:
		TOKEN start_tok;
		vector<RULE> rules;
		RULE_EVALUATOR final_rule;

		vector<TOKEN> tokenize(string s) {
			vector<TOKEN> o;
			for (char c : s) {
				o.push_back({TOKEN_TYPE::TERMINAL, {c}});
			}
			return o;
		}

		Tree<vector<TOKEN>> parse_rec(Tree<vector<TOKEN>> tree) {
			vector<TOKEN> t = tree.get_value();
			for (RULE rule : rules) {
				auto it = t.begin();
				while (it != t.end()) {
					it = search(it, t.end(), rule.rhs.begin(), rule.rhs.end());
					if (it == t.end()) break;
					vector<TOKEN> subbed(t);
					auto offset = it - t.begin();
					auto start = subbed.begin() + offset;
					subbed.erase(start, start + rule.rhs.size());
					subbed.insert(start, rule.lhs);
					Tree<vector<TOKEN>> sapling = Tree<vector<TOKEN>>(subbed);
					Tree<vector<TOKEN>> grown_sapling = parse_rec(sapling);
					tree.add(grown_sapling);
					it++; // so as to not repeatedly keep matching at start of string
					break;
				}
			}
			return tree;
		}

		Tree<vector<TOKEN>> build_AST(vector<set<RULE_EVALUATOR>> frames) {
			set<RULE_EVALUATOR>& lastFrame = frames[frames.size() - 1];
			RULE_EVALUATOR final_rule;
			Tree<vector<TOKEN>> tree;
			// size_t minLen = pow(2, 32);
			for (const RULE_EVALUATOR& rev : lastFrame) {
				if (rev.lhs == start_tok && rev.pos >= rev.rhs.size()) {
					bool isTerminalRHS = true;
					for (TOKEN tok : rev.rhs) {
						if (tok.type == TOKEN_TYPE::VARIABLE) {
							isTerminalRHS = false;
							break;
						}
					}
					if (!isTerminalRHS) continue;
					final_rule = rev;
					// minLen = rev.rhs.size();
				}
			}
			cout << REV_to_str(final_rule) << endl;
			// cout << REV_to_str(*(final_rule.parent)) << endl;
			tree.add(final_rule.rhs);
			// tree.set_value(final_rule.parent->rhs);
			return tree;
		}


		vector<RULE_EVALUATOR> build_derivation(vector<set<RULE_EVALUATOR>> frames) {
			vector<RULE_EVALUATOR> rule_order;
			RULE_EVALUATOR domino_rev;
			/*
			 * for (RULE_EVALUATOR rev : frames.back()) {
				 * if (rev.lhs == start_tok && rev.pos >= rev.rhs.size()) {
					 * domino_rev = rev;
				 * }
			 * }
			 * rule_order.push_back(domino_rev);
			 * while (rule_order.back().children.back() != nullptr) {
				 * rule_order.push_back(*(rule_order.back().children.back()));
			 * }
			 * reverse(rule_order.begin(), rule_order.end());
			 * vector<vector<TOKEN>> derivation;
			 * for (RULE_EVALUATOR rev : rule_order) {
				 * RULE rule = {rev.lhs, rev.rhs};
				 * derivation.push_back(rev.rhs);
			 * }
			 */
			vector<RULE_EVALUATOR> revs;
			revs.push_back(final_rule);
			cout << "FREV SIZE" << final_rule.children.size() << endl;
			for (const RULE_EVALUATOR* rev : final_rule.children) {
				revs.push_back(*rev);
			}
			reverse(revs.begin(), revs.end());
			return revs;
		}

		void print_tree(const RULE_EVALUATOR& r, int depth) {
			cout << string(depth, '\t') << REV_to_str(r) << '\n';
			for (const RULE_EVALUATOR* c : r.children)
				if (c) print_tree(*c, depth + 1);
		}

	public:
		Grammar() {
			rules = {};
		}

		Grammar(RULE rule) {
			rules = {rule};
			start_tok = rules[0].lhs;
		}

		Grammar(vector<RULE> start_rules) {
			for (RULE rule : start_rules) {
				rules.push_back(rule);
			}
			start_tok = rules[0].lhs;
		}

		Grammar(vector<RULE> start_rules, TOKEN final_token) : Grammar(start_rules) {
			start_tok = final_token;
		}

		Tree<vector<TOKEN>> parse(string s) {
			if (rules.empty()) return Tree<vector<TOKEN>>();
			vector<TOKEN> t = tokenize(s);
			vector<set<RULE_EVALUATOR>> frames(t.size()+2);
			RULE_EVALUATOR firstRule;
			firstRule.lhs = {TOKEN_TYPE::VARIABLE, "$"};
			firstRule.rhs = {start_tok};
			firstRule.pos = 0;
			firstRule.origin = 0;
			firstRule.children = {};
			frames[0].insert(firstRule);
			for (size_t i = 0; i <= t.size(); i++) {
				TOKEN scan = (i == t.size()) ? TOKEN({TOKEN_TYPE::TERMINAL, ""}) : t[i];
				set<RULE_EVALUATOR>& frame = frames[i];
				set<RULE_EVALUATOR>& nextframe = frames[i+1];
				set<RULE_EVALUATOR> matches = {};
				size_t beforeSize;
				do {
					beforeSize = frame.size();
					for (auto it = frame.begin(); it != frame.end(); it++) {
						const RULE_EVALUATOR& rev = *it;
						if (rev.pos >= rev.rhs.size()) continue;
						TOKEN pointed = rev.rhs[rev.pos];
						if (pointed.type == TOKEN_TYPE::VARIABLE) {
							for (const RULE& rule : rules) {
								if (rule.lhs != pointed) continue;
								RULE_EVALUATOR newRev = {rule.lhs, rule.rhs, 0, {}, i};
								frame.insert(newRev);
							}
						} else if (pointed == scan) {
							vector<const RULE_EVALUATOR*> newChildren = rev.children;
							newChildren.push_back(&rev);
							RULE_EVALUATOR newRev = {rev.lhs, rev.rhs, rev.pos+1, {}, rev.origin};
							nextframe.insert(newRev);
						}
					}
				} while (frame.size() > beforeSize);

				if (i == t.size()) break;


				do {
					beforeSize = nextframe.size();
					for (const RULE_EVALUATOR& rev : nextframe) {
						if (rev.pos < rev.rhs.size()) continue;
						for (const RULE_EVALUATOR& inrev : frames[rev.origin]) {
							if (inrev.pos >= inrev.rhs.size()) continue;
							TOKEN pointed = inrev.rhs[inrev.pos];
							if (pointed == rev.lhs) {
								// Previous rule had variable (rev.lhs) fulfilled, so we advance it
								vector<const RULE_EVALUATOR*> newChildren = inrev.children;
								newChildren.push_back(&rev);
								RULE_EVALUATOR newRev = {inrev.lhs, inrev.rhs, inrev.pos+1, newChildren, inrev.origin};
								nextframe.insert(newRev);
							}
						}
					}
				} while (nextframe.size() > beforeSize);
			}

			frames.pop_back();
			bool isMatched = false;
			set<TOKEN> completed;
			for (const RULE_EVALUATOR& rev : frames.back()) {
				if (rev.rhs == firstRule.rhs && rev.lhs == firstRule.lhs && rev.pos >= rev.rhs.size() && rev.origin == 0) {
					final_rule = rev;
					isMatched = true;
					break;
				}
			}
			if (!isMatched) {
				cout << "Match failed" << endl;
				return {};
			}

			cout << "Match succeeded" << endl;
			print_frames(frames);
			cout << vREV_to_str(build_derivation(frames)) << endl;
			print_tree(final_rule, 0);
			return build_AST(frames);
		}

		TOKEN get_start_tok() {
			return start_tok;
		}

		void print_frames(vector<set<RULE_EVALUATOR>> frames) {
			string s = "\n";
			string tab = "";
			for (auto frame : frames) {
				for (auto rev : frame) {
					s += tab + REV_to_str(rev) + '\n';
				}
				tab += '\t';
			}
			cout << s << endl;
		}
};

string TOK_to_str(TOKEN tok) {
	return (tok.type == TOKEN_TYPE::VARIABLE) ? "<" + tok.value + ">" : tok.value;
}

string vTOK_to_str(vector<TOKEN> vtok) {
	string s = "";
	for (TOKEN tok : vtok) {
		s += TOK_to_str(tok);
	}
	return s;
}

string vvTOK_to_str(vector<vector<TOKEN>> vvtok) {
	if (vvtok.empty()) return "";
	string s = "";
	for (vector<TOKEN> vtok : vvtok) {
		s += vTOK_to_str(vtok) + " → ";
	}
	s.pop_back();
	s.pop_back();
	s.pop_back();
	s.pop_back();
	return s;
}

string RULE_to_str(RULE rule) {
	return TOK_to_str(rule.lhs) + " -> " + vTOK_to_str(rule.rhs);
}

string vRULE_to_str(vector<RULE> rules) {
	string s = "";
	for (RULE rule : rules) {
		s += RULE_to_str(rule) + '\n';
	}
	s.pop_back();
	return s;
}

string REV_to_str(RULE_EVALUATOR rev) {
	string rhs = "";
	for (size_t i = 0; i < rev.rhs.size(); i++) {
		if (i == rev.pos) {
			rhs += '.';
		}
		rhs += TOK_to_str(rev.rhs[i]);
	}
	if (rev.pos >= rev.rhs.size()) {
		rhs += '.';
	}
	return TOK_to_str(rev.lhs) + " -> " + rhs;
}

string vREV_to_str(vector<RULE_EVALUATOR> vrev) {
	if (vrev.empty()) return "";
	string s = "";
	for (RULE_EVALUATOR rev : vrev) {
		s += REV_to_str(rev) + "; ";
	}
	s.pop_back();
	s.pop_back();
	return s;
}

string Tree_TOK_to_str(Tree<vector<TOKEN>> tree, int depth) {
	string tab = "";
	for (int i = 0; i < depth; i++) {
		tab += '\t';
	}
	string s = tab + vTOK_to_str(tree.get_value()) + '\n';
	for (Tree<vector<TOKEN>> sapling : tree.get_children()) {
		s += Tree_TOK_to_str(sapling, depth + 1);
	}
	return s;
}

