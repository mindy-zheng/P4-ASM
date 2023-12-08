#include "token.h"
#include "testTree.h"
#include "parser.h"  
#include "staticSemantics.h"
#include <iostream> 
#include <string> 

using namespace std; 


string label_names[] = {
        "PROGRAM", "VARS", "VARLIST", "EXP", "M", "N", "R", "STATS", "MSTAT", "STAT", "BLOCK", "IN", "OUT", "IF", "LOOP", "ASSIGN", "RO",
};

string token_types[] = {
        "XOPEN", "XCLOSED", "XLOOP", "XDATA", "XEXIT", "XIN", "XOUTD", "XCOND", "XTHEN", "XLET", "XFUNC",

        "EOF", "IDENTIFIER", "INTEGER",

        "EQUAL", "LESS", "GREATER", "TILDE", "COLON", "SEMICOLON", "PLUS", "SUBTRACT", "MULTIPLY", "DIVIDE", "PERCENT", "PERIOD", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "COMMA", "LEFT_BRACE", "RIGHT_BRACE", "LEFT_BRACKET", "RIGHT_BRACKET", "GREATERGREATER", "LESSLESS", "COMMENT", "EMPTY_TOKEN", // Empty token holder
};

node* createNode(node_t label) {
	node* new_node = new node;
	new_node -> label = label; 
	
	Token tk1 = {EMPTY_TOKEN, "", 0}; 
	Token tk2 = {EMPTY_TOKEN, "", 0}; 
	new_node -> token1 = tk1; 
	new_node -> token2 = tk2; 

	new_node -> c1 = nullptr;
	new_node -> c2 = nullptr;
	new_node -> c3 = nullptr;
	new_node -> c4 = nullptr;

	return new_node; 
} 

// Traverses parse tree in preorder; starts from root node and visits each node b4 its childrne 
void traversePreorder(node *n, int depth) {
	if (n != nullptr) {
		visitNode(n, depth); // visit current node if not null 
		
		// visit the children of the node recursively
		traversePreorder(n->c1, depth+1);
		traversePreorder(n->c2, depth+1);
		traversePreorder(n->c3, depth+1);
		traversePreorder(n->c4, depth+1);
	}
}

// Visits a node in the parse tree, prints label, indentation, and tokens associated w/ it
void visitNode(node *n, int depth) { 
	cout << string(depth * 2, '-') << "Node: " << label_names[n-> label]; 
	
	// if token 1 is not empty, print:
	if (n->token1.tokenType != EMPTY_TOKEN) {
   		 cout << " {" << token_types[n->token1.tokenType] << ", " << n->token1.tokenInstance << ", " << n->token1.lineNumber << "}";
	} 

	// if token 2 is not empty, print: 
	if (n->token2.tokenType != EMPTY_TOKEN) {
    		cout << " {" << token_types[n->token2.tokenType] << ", " << n->token2.tokenInstance << ", " << n->token2.lineNumber << "}";
	}
	
	cout << endl; 


} 

/*
typedef struct Token {
        tokenID tokenType;
        string tokenInstance;
        int lineNumber;
} Token;
*/

void staticSemantics(node *n, find_stack& id_stack) { 
	//cout << "Entering staticSemantics" << endl; 
	if (n != nullptr) {
		if (n-> label == block_node) {
			int var_count = 0; 
		
			preorder(n, var_count, id_stack);
		 
			for (int i = 0; i < var_count; i++) { 
				id_stack.pop(); 
			} 
		} else {
			int var_count = 0; 
			preorder(n, var_count, id_stack); 
		}
	}
	//cout << "Exiting static semantics" << endl; 
}

void preorder(node *n, int& var_count, find_stack& id_stack) { 
	//cout << "Entering preorder" << endl; 
	if (n != nullptr) {
		verifyNode(n, var_count, id_stack); 
	
		staticSemantics(n->c1, id_stack); 
		staticSemantics(n->c2, id_stack);
		staticSemantics(n->c3, id_stack);
 		staticSemantics(n->c4, id_stack);
	}
	//cout << "Exiting preorder" << endl; 
} 

void verifyNode(node *n, int &var_count, find_stack& id_stack) { 
	//cout << "Entering verifyNode" << endl; 
	if (n-> label == varList_node) { 
		if (n-> token1.tokenType == IDENTIFIER_TOKEN) { 
			if (var_count > 0) { 
				id_stack.find(n-> token1, 1); 
			} 

			id_stack.push(n->token1); 
			var_count++; 
		}
	} 
	else if (n-> label == R_node || n-> label == in_node || n-> label == assign_node || n-> label == assign_node || n-> label == exp_node || n->label == M_node || n-> label == N_node) {
		if (n-> token1.tokenType == IDENTIFIER_TOKEN) { 
			id_stack.find(n-> token1, 2); 
		} 
	}
	//cout << "Exiting verifyNode" << endl; 
}
