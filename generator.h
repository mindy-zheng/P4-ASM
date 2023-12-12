#ifndef GENERATOR_H
#define GENERATOR_H
#include <iostream>
#include <fstream> 
#include "testTree.h" 
#include "staticSemantics.h" 
#include "token.h" 

using namespace std; 

void recGen(node *n, int* var_count, ofstream* output); 
void generator(node *program, ofstream* output); 
void staticSemantics(node *n, int* var_count, ofstream* output); 
int verifyNode(node *n, int* var_count, ofstream* output); 

#endif 
