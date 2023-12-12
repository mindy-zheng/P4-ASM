#include <iostream> 
#include <cstdio> 
#include <cstdlib> 
#include <string> 
#include <fstream> 
#include <vector> 
#include "token.h" 
#include "testTree.h" 
#include "generator.h" 
#include "staticSemantics.h"  

using namespace std;

find_stack::~find_stack() { 
	while (size > 0) { 
		pop(); 
	}
}

void find_stack::push(const Token& tk) { 
	if (size == MAX_SIZE) { 
		std::cout << "STACK OVERFLOW" << std::endl;
        	std::exit(size);
	}

	find_id* new_id = new find_id(tk); 
	new_id -> next = top; 
	top = new_id; 
	size++;
} 

void find_stack::pop() { 
	//cout << "Popping" << endl;
	if (size > 0) { 
		find_id* temp = top; 
		top = temp-> next; 
		delete temp; 
		size--; 
		//cout << "Popped" << std::endl;
	} else {
		cout << "STACK EMPTY" << endl; 
		exit(size); 
	}
} 

int find_stack::find(const Token &tk, int error) { 
	find_id* current_id = top; 

	for(int distance = 0; distance < size; distance++) { 
		if (tk.tokenInstance == current_id-> tk.tokenInstance) { 
			if (error == 1) { 
				cout << "Static Semantics Error: Variable - '" << tk.tokenInstance << "' previously defined on line: " << current_id -> tk.lineNumber << "( " << tk.lineNumber << ")" << endl; 
				exit(current_id-> tk.lineNumber); 
			} else if (error == 2) { 
				return distance; 
			}
		} 
		current_id = current_id -> next; 
	} 

	if (error == 2) {
		cout << "Static Semantics Error: Variable - '" << tk.tokenInstance << "' previously defined on line: " << tk.lineNumber << endl; 
		std::exit(tk.lineNumber); 
	}
	return -1;
} 

void find_stack::printStack() const { 
	int distance;
	find_id* current_id = top; 
	
	cout << "Stack: " << endl; 
	for (distance = 0; distance < size; distance++) { 
		cout << current_id-> tk.tokenInstance << " " << distance << endl; 
		current_id = current_id -> next; 
	} 
}
