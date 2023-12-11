#ifndef STATICSEMANTICS_H
#define STATICSEMANTICS_H
#include "token.h"
#include "testTree.h" 
#include <fstream>
#pragma once 

using namespace std; 

class find_id {
public: 
	Token tk; 
	find_id* next; 
	find_id(const Token& token) : tk(token), next(nullptr) {} 
}; 

class find_stack { 
private: 
	static const int MAX_SIZE = 100; 
	int size; 
	find_id *top; 

public: 
	find_stack() : size(0), top(nullptr) {} 
	~find_stack(); 

	void push(const Token& tk); 
	void pop(); 
	void find(const Token &tk, int error); 
}; 

#endif

