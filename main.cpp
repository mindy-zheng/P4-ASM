#include <iostream> 
#include <iomanip>
#include <fstream> 
#include "token.h"
#include "scanner.h"
#include "testTree.h" 
#include "parser.h" 
#include "generator.h"  
#include "staticSemantics.h" 

using namespace std; 

int main(int argc, char *argv[]) {
	ifstream inFile;
	ofstream outFile;  
	istream *input_stream;  
	string file; 
	
	if (argc > 2) { 
		cout << "Too many arguments passed." << endl; 
		return argc; 
	}

	if (argc == 2) {
		string input_extension = ".f23"; 
		file = argv[1];
		// Check if file is present with extension: 
		if (file.substr(file.size() - input_extension.size()) != input_extension) { 
			 file += input_extension;
		} 
	  
		inFile.open(file); 

		if (!inFile.is_open()) { 
			cout << "File was not opened." << endl; 
			return 1; 
		} 
		input_stream = &inFile; 
    	} else {
		const string output_name = "a"; 
		file = output_name; 
		input_stream = &std::cin; 
	}  

	//find_stack id_stack; 
	// Parser:
	node *root = parser(*input_stream); 
	if (inFile.is_open()) { 
		inFile.close(); 
	}
		
	// Prepare output file 
	file = file.substr(0, file.size() - 4) + ".asm"; 
	outFile.open(file); 
	
	if (!outFile.is_open()) { 
		cout << "Error opening asm file" << endl; 
		return 1; 
	} 
	
	// Call generator
	generator(root, &outFile); 
	outFile.close(); 

	cout << "Target file created: " << file << endl; 
	
	return 0; 
}

