#include <stdexcept>
#include <vector>
#include <functional>
#include <algorithm>
#include "token.cpp"

using namespace std;

enum SyntaxTreeNodeType {
	PROGRAM, COMPOUND_BLOCK, BLOCK, FUNCTION_DEFINITION,
	UNINITIALIZED,
	TOKEN
};

struct SyntaxTreeNode {
	SyntaxTreeNodeType type = UNINITIALIZED;
	vector<SyntaxTreeNode*> children;
	vector<Token> leaves;

	~SyntaxTreeNode() {
		for (SyntaxTreeNode* child : children) {
			delete child;
		}
	}
};

class SyntaxTree {
	private:
		SyntaxTreeNode* root;
	public:
		SyntaxTree();
		SyntaxTree(const SyntaxTreeNodeType& t) {
			root = new SyntaxTreeNode();
			root->type = t;
		}
		SyntaxTree(const SyntaxTreeNode& n) {
			root = new SyntaxTreeNode(n);
		}

		~SyntaxTree() {
			delete root;
		}

		void insert(const SyntaxTreeNodeType& t) {
			SyntaxTreeNode node = SyntaxTreeNode(t, {});
			insert(&node);
		}

		void insert(SyntaxTreeNode* node) {
			root->children.push_back(node);
		}

		void insert(const SyntaxTree& tree) {
			root->children.push_back(tree.getRoot());
		}
		
		void insert(const SyntaxTree* tree) {
			root->children.push_back(tree->getRoot());
		}

		void addLeaf(const Token& t) {
			root->leaves.push_back(t);
		}
		
		SyntaxTreeNode* getRoot() const {
			return root;
		}

		SyntaxTreeNodeType getType() const {
			return root->type;
		}

		vector<Token> getLeaves() const {
			return root->leaves;
		}

		size_t size() {
			return root->children.size();
		}

		bool empty() {
			return root->children.empty();
		}

		bool failed() {
			return empty() && UNINITIALIZED == root->type
		}
};

class Parser {
	private:
		vector<Token> tokens;
		SyntaxTree root;
		size_t pos;
	public:
		Parser(const vector<Token>& intokens) {
			tokens = intokens;
			pos = 0;
		}

		Token currTok() {
			return tokens[pos];
		}

		TokenType currType() {
			return currTok().getType();
		}

		bool empty() {
			return pos >= tokens.size();
		}

		SyntaxTree parse() {
			return program();
		}

		Token match(TokenType ty) {
			if (empty()) return Token();
			Token t = currTok();
			if (ty == t.getType()) {
				advanceToken();
				return t;
			} else {
				throw_error();
			}
			return Token();
		}

		Token matchFirst(vector<TokenType> tys) {
			if (0 == tokens.size()) return Token();
			Token t = currTok();
			for (TokenType ty : tys) {
				if (ty == t.getType()) {
					advanceToken();
					return t;
				}
			}
			throw_error();
			return Token();
		}

		void throw_error() {
			throw std::runtime_error("Invalid syntax");
		}

		void advanceToken() {
			pos++;
		}

		SyntaxTree* goBranch(vector<function<SyntaxTree*()>> branches) {
			for (function<SyntaxTree*()> branch : branches) {
				SyntaxTree* tree = tryParse(branch);
				if (nullptr != tree) return tree;
			}
			return nullptr;
		}

		SyntaxTree* tryParse(function<SyntaxTree*()> branch) {
			size_t save = pos;
			SyntaxTree* tree = branch();
			if (tree->failed()) {
				delete tree;
				return nullptr;
			}
			return tree;
		}




		SyntaxTree program() {
			SyntaxTree tree(PROGRAM);
			SyntaxTree* child;
			while (true) {
				child = tryParse([this] {return compoundBlock();});
				if (nullptr == child) break;
				tree.insert(child);
			}
			match(END);
			if (tree.failed()) throw runtime_error("Program is empty");
			return tree;
		}

		SyntaxTree* compoundBlock() {
			SyntaxTree* tree = new SyntaxTree(COMPOUND_BLOCK);
			SyntaxTree* child;
			while (true) {
				child = tryParse([this] {return block();});
				if (nullptr == child) break;
				tree->insert(child);
			}
			return tree;
		}

		SyntaxTree* compoundNotTLBlock() {
			SyntaxTree* tree = new SyntaxTree(COMPOUND_NOT_TL_BLOCK);
			SyntaxTree* child;
			vector<function<SyntaxTree*()>> branches = {
				[this] {return LOC();},
				[this] {return controlFlow();},
				[this] {return errorHandling();}
			};
			child = goBranch(branches);
			if (nullptr == child) return nullptr;
			tree->insert(child);
			return tree;
		}

		SyntaxTree* block() {
			SyntaxTree* tree = new SyntaxTree(BLOCK);
			SyntaxTree* child;
			child = controlFlow() || LOC() || functionDefinition() || classDefinition || enumDefinition() || Import() || errorHandling();
			tree->insert(child);
			return tree;
		}

		SyntaxTree* controlFlow() {
			SyntaxTree* tree = new SyntaxTree(CONTROL_FLOW);
			SyntaxTree* child;
			child = forBlock() || whileBlock() || ifBlock() || switchBlock() || inlineControlFlow();
			tree->insert(child);
			return tree;
		}

		SyntaxTree* forBlock() {
			SyntaxTree* tree = new SyntaxTree(FOR_BLOCK);
			match(FOR);
			match(OPEN_PARENTHESIS);
			tree->insert(forInner());
			match(CLOSE_PARENTHESIS);
			match(OPEN_CURLY);
			tree->insert(compoundNotTLBlock());
			match(CLOSE_CURLY);
			return tree;
		}

		SyntaxTree* forInner() {
			SyntaxTree* tree = new SyntaxTree(FOR_INNER);
			Token forVar = match(ID);
			match(IN);
			tree->insert(expression());
			return tree;
		}

		SyntaxTree* whileBlock() {
			SyntaxTree* tree = new SyntaxTree(WHILE_BLOCK);
			match(WHILE);
			match(OPEN_PARENTHESIS);
			tree->insert(expression());
			match(CLOSE_PARENTHESIS);
			match(OPEN_CURLY);
			tree->insert(compoundNotTLBlock());
			match(CLOSE_CURLY);
			return tree;
		}

		SyntaxTree* ifBlock() {
			SyntaxTree* tree = new SyntaxTree(IF_BLOCK);
			match(IF);
			match(OPEN_PARENTHESIS);
			tree->insert(expression());
			match(CLOSE_PARENTHESIS);
			match(OPEN_CURLY);
			tree->insert(compoundNotTLBlock());
			match(CLOSE_CURLY);
			SyntaxTree* ElifChain = elifChain();
			if (nullptr != ElifChain) tree->insert(ElifChain);
			SyntaxTree* endElse = Else();
			if (nullptr != endElse) tree->insert(endElse);
			return tree;
		}

		SyntaxTree* elifChain() {
			SyntaxTree* tree = new SyntaxTree(ELIF_CHAIN);
			SyntaxTree* child;
			while (true) {
				child = elifBlock();
				if (nullptr == child) break;
				tree->insert(child);
			}
			return (0 == tree.size()) ? nullptr : tree;
		}

		SyntaxTree* elifBlock() {
			SyntaxTree* tree = new SyntaxTree(ELIF_BLOCK);
			match(ELIF);
			match(OPEN_PARENTHESIS);
			tree->insert(expression());
			match(CLOSE_PARENTHESIS);
			match(OPEN_CURLY);
			tree->insert(compoundNotTLBlock());
			match(CLOSE_CURLY);
			return tree;
		}

		SyntaxTree* Else() {
			SyntaxTree* tree = new SyntaxTree(ELSE);
			match(ELSE);
			match(OPEN_CURLY);
			tree->insert(compoundNotTLBlock());
			match(CLOSE_CURLY);
			return tree;
		}

		SyntaxTree* switchBlock() {
			SyntaxTree* tree = new SyntaxTree(SWITCH_BLOCK);
			SyntaxTree* child;
			match(SWITCH);
			match(OPEN_PARENTHESIS);
			tree->insert(expression());
			match(CLOSE_PARENTHESIS);
			match(OPEN_CURLY);
			tree->insert(switchBody());
			match(CLOSE_CURLY);
			return tree;
		}

		SyntaxTree* switchBody() {
			SyntaxTree* tree = new SyntaxTree(SWITCH_BODY);
			SyntaxTree* child;
			while (true) {
				child = switchCase();
				if (nullptr == child) break;
				tree->insert(child);
			}
			return (0 == tree->size()) ? nullptr : tree;
		}

		SyntaxTree* switchCase() {
			SyntaxTree* tree = new SyntaxTree(SWITCH_CASE);
			tree->insert(switchCaseBegin());
			match(COLON);
			match(OPEN_CURLY);
			tree->insert(compoundNotTLBlock());
			match(CLOSE_CURLY);
			return tree;
		}

		SyntaxTree* switchCaseBegin() {
			SyntaxTree* tree = new SyntaxTree(SWITCH_CASE_BEGIN);
			SyntaxTree* child;
			try {
				match(DEFAULT);
			} catch {
				match(CASE);
				while (true) {
					child = switchCaseValue();
					if (nullptr == child) break;
					tree->insert(child);
				}
			}
		}

		SyntaxTree* switchCaseValue() {
			SyntaxTree* tree = new SyntaxTree(SWITCH_CASE_VALUE);
			SyntaxTree* child;
			SyntaxTree* lit = literal();
			if (nullptr != lit) {
				tree->insert(lit);
				return tree;
			}
			while (true) {
				Token id = match(ID);
				try {
					Token period = match(PERIOD);
					tree->addLeaf(id);
					tree->addLeaf(period);
				} catch (runtime_error e) {
					tree->addLeaf(id);
					break;
				}
			}
			return tree;
		}

		SyntaxTree* literal() {
			SyntaxTree* tree = new SyntaxTree(LITERAL);
			Token child = matchFirst({null, NUMBER, STRING, BOOLEAN});
			tree->addLeaf(child);
			return tree;
		}

		SyntaxTree* inlineControlFlow() {
			SyntaxTree* tree = new SyntaxTree(INLINE_CONTROL_FLOW);
			SyntaxTree* child = inlineIf() || inlineWhile() || inlineFor();
			tree->insert(child);
			return (0 == tree->size()) ? nullptr : tree;
		}

		SyntaxTree* inlineIf() {
			SyntaxTree* tree = new SyntaxTree(INLINE_IF);
			match(IF);
			match(OPEN_PARENTHESIS);
			tree->insert(expression());
			match(CLOSE_PARENTHESIS);
			tree->insert(LOC());
			return (0 == tree->size()) ? nullptr : tree;
		}

		SyntaxTree* inlineWhile() {
			SyntaxTree* tree = new SyntaxTree(INLINE_WHILE);
			match(WHILE);
			match(OPEN_PARENTHESIS);
			tree->insert(expression());
			match(CLOSE_PARENTHESIS);
			tree->insert(LOC());
			return (0 == tree->size()) ? nullptr : tree;
		}

		SyntaxTree* inlineFor() {
			SyntaxTree* tree = new SyntaxTree(INLINE_FOR);
			match(WHILE);
			match(OPEN_PARENTHESIS);
			tree->insert(forInner());
			match(CLOSE_PARENTHESIS);
			tree->insert(LOC());
			return (0 == tree->size()) ? nullptr : tree;
		}

		SyntaxTree* LOC() {
			SyntaxTree* tree = new SyntaxTree(LOC);
			SyntaxTree* child = variableDeclaration() || variableAssignment() || vvalue() || branchStatement() || throwStatement();
			tree->insert(child);
			match(SEMICOLON);
			return tree;
		}

		SyntaxTree* variableDeclaration() {
			SyntaxTree* tree = new SyntaxTree(VARIABLE_DECLARATION);
		}
};

