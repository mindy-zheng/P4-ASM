#ifndef TESTTREE_H
#define TESTTREE_H
#include "token.h"
#include "staticSemantics.h" 
#pragma once  


using namespace std; 

enum node_t { 
	program_node, 
	vars_node, 
	varList_node, 
	exp_node, 
	M_node, 
	N_node, 
	R_node, 
	stats_node, 
	mStat_node, 
	stat_node, 
	block_node, 
	in_node, 
	out_node, 
	if_node, 
	loop_node, 
	assign_node, 
	RO_node,
};

typedef struct node { 
	node_t label; // Corresponds to nonterminal name eg: <exp> 
	Token token1;		
	Token token2; 
	struct node *c1; 
	struct node *c2; 
	struct node *c3;
        struct node *c4;
} node; 

extern string token_types[]; 
node *createNode(node_t);
void visitNode(node *n, int depth);
//void createNode(node_t);
void traversePreorder(node *n, int depth);

void staticSemantics(node *n, find_stack& id_stack); 
void preorder(node *n, int& var_count, find_stack& id_stack); 
void verifyNode(node *n, int& var_count, find_stack& id_stack); 

#endif
