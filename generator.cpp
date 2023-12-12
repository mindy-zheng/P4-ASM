#include <iostream>
#include <fstream> 
#include <cstdio> 
#include <cstdlib> 
#include <string> 
#include "token.h" 
#include "staticSemantics.h" 
#include "generator.h" 
#include "testTree.h" 

using namespace std; 

find_stack id_stack; 
static int temp_var = 0; 
static int label_count = 0; 
static char name[5]; 

enum nameType { 
	VAR, 
	LABEL
}; 

static char *newName(nameType type) { 
	if (type == VAR) { 
		sprintf(name, "V%d", temp_var++); 
	} else if (type == LABEL) { 
		sprintf(name, "L%d", label_count++); 
	} 
	return name;
}
 
void recGen(node *n, int *var_count, ofstream* output) { 
	int stack_dist; 
	string label; 
	string label2; 
	string arg; 
	stack_dist = verifyNode(n, var_count, output); 
	
	switch(n-> label) { 
		case exp_node: 
			if (n-> token1.tokenType != EMPTY_TOKEN) { 
				staticSemantics(n->c2, var_count, output); 
				arg = newName(VAR); 
				*output << "STORE " << arg << endl; 
				staticSemantics(n->c1, var_count, output); 
			
				if (n-> token1.tokenType == DIVIDE_TOKEN) { 
					*output << "DIV " << arg << endl; 
				} else if (n-> token1.tokenType == MULTIPLY_TOKEN) { 
					*output << "MULT " << arg << endl; 
				} 
			} 
			else { 
				staticSemantics(n-> c1, var_count, output); 
			} 
		break; 
	
		case M_node: 
			if (n-> token1.tokenType != EMPTY_TOKEN) { 
				staticSemantics(n->c1, var_count, output);
				arg = newName(VAR); 
				*output << "STORE " << arg << endl; 
		
				staticSemantics(n->c1, var_count, output); 
				
				if (n->token1.tokenType == PLUS_TOKEN) { 
					*output << "ADD " << arg << endl; 
				} 
			} else { 
				staticSemantics(n-> c1, var_count, output); 
			} 
			break; 
	
		case N_node: 
			if (n-> token1.tokenType != EMPTY_TOKEN) {
				staticSemantics(n-> c2, var_count, output); 
				arg = newName(VAR); 

				*output << "STORE " << arg << endl; 
				staticSemantics(n-> c1, var_count, output); 
			
				if (n->token1.tokenType == SUBTRACT_TOKEN) { 
					*output << "SUB " << arg << endl; 
				} else if (n-> c1-> label == N_node) { 
					*output << "MULT -1" << endl; 
				} 
			} else { 
				staticSemantics(n-> c1, var_count, output); 
			} 
			break; 
	
		case R_node: 
			if (n-> c1 == nullptr) { 
				if (n-> token1.tokenType == IDENTIFIER_TOKEN) { 
					*output << "STACKR " << stack_dist << endl; 
				} else if (n->token1.tokenType == INTEGER_TOKEN) { 
					*output << "LOAD " << n-> token1.tokenInstance << endl; 
				}
			} else { 
				staticSemantics(n-> c1, var_count, output); 
			} 
			break; 
		case in_node: 
			arg = newName(VAR); 
			*output << "READ " << arg << endl; 
			*output << "LOAD " << arg << endl; 
			*output << "STACKW " << id_stack.find(n-> token1, 2) << endl; 
			break; 
		case out_node: 
			staticSemantics(n-> c1, var_count, output); 
			arg = newName(VAR); 
			*output << "STORE " << arg << endl; 
			*output << "WRITE " << arg << endl; 
			break; 
		case if_node: 
			staticSemantics(n->c3, var_count, output); 
			arg = newName(VAR); 
			*output << "STORE " << arg << endl; 
			
			staticSemantics(n-> c1, var_count, output); 
			*output << "SUB " << arg << endl; 
			label = newName(LABEL); 
			
			switch(n-> c2-> token1.tokenType) {
				case GREATER_TOKEN: 
					*output << (n-> c2 -> token2.tokenType == EQUAL_TOKEN ? "BRNEG ": "BRZNEG ") << label << endl; 
					break; 
				case LESS_TOKEN:
			 		*output << (n-> c2 -> token2.tokenType == EQUAL_TOKEN ? "BRPOS": "BRZPOS ") << label << endl;
					break; 
				case EQUAL_TOKEN: 
					*output << (n-> c2 -> token2.tokenType == EQUAL_TOKEN ? "BRZERO ": "BRNEG " + string("BRPOS ")) << label << endl; 
					break; 
				default: 
					*output << (n-> c2 -> token2.tokenType == EQUAL_TOKEN ? "BRZERO " : "BRNEG " + string("BRPOS ")) << label << endl; 
			} 
			staticSemantics(n->c4, var_count, output); 
			*output << "BR " << label << endl; 
			*output << label2 << ": NOOP" << endl; 
			break; 

		case loop_node: 
			label = newName(LABEL); 
			*output << label << ": NOOP" << endl; 
			staticSemantics(n-> c3, var_count, output); 
			arg = newName(VAR); 
			*output << "STORE " << arg << endl; 
			
			staticSemantics(n-> c1, var_count, output); 
			*output << "SUB " << arg << endl; 
			label2 = newName(LABEL); 
			
		 	switch(n-> c2-> token1.tokenType) {
                                case GREATER_TOKEN:
                                        *output << (n-> c2 -> token2.tokenType == EQUAL_TOKEN ? "BRNEG ": "BRZNEG ") << label2 << endl;
                                        break;
                                case LESS_TOKEN:
                                        *output << (n-> c2 -> token2.tokenType == EQUAL_TOKEN ? "BRPOS": "BRZPOS ") << label2 << endl;
                                        break;
                                case EQUAL_TOKEN:
                                        *output << (n-> c2 -> token2.tokenType == EQUAL_TOKEN ? "BRZERO ": "BRNEG " + string("BRPOS ")) << label2
<< endl;
                                        break;
                                default:
                                        *output << (n-> c2 -> token2.tokenType == EQUAL_TOKEN ? "BRZERO " : "BRNEG " + string("BRPOS ")) << label2 << endl;
                        }	
			
			staticSemantics(n-> c4, var_count, output); 
			*output << "BR " << label << endl; 
			*output << label2 << ": NOOP" << endl; 
			break; 
		
		case assign_node: 
			staticSemantics(n-> c1, var_count, output); 
			*output << "STACKW " << stack_dist << endl; 
			break; 
		default: 
			staticSemantics(n-> c1, var_count, output);
			staticSemantics(n-> c2, var_count, output); 
			staticSemantics(n-> c3, var_count, output);
			staticSemantics(n-> c4, var_count, output);
			break; 
		}
}

			
void generator(node *program, ofstream* output) { 
	if (program == nullptr || program -> label != program_node) { 
		cerr << "Generator error: invalid parse tree..." << endl; 
		exit(program-> label); 
	}
	 
	staticSemantics(program, nullptr, output); 
	
	*output << "STOP" << endl; 
	for (int i = 0; i < temp_var; i++) { 
		*output << "V" << i << " 0" << endl;  
	} 
} 

void staticSemantics(node *n, int *var_count, ofstream* output) { 
	if (n != nullptr) {
		if (n-> label == program_node) {
			var_count = new int(0); 

			recGen(n, var_count, output); 				

			for (int i = 0; i < *var_count; i++) { 
				id_stack.pop(); 
				*output << "POP" << endl; 
			} 
			delete var_count; 
		} else {
			//int var_count = 0; 
			//preorder(n, var_count, id_stack);
			recGen(n, var_count, output);  
		}
	}
	//cout << "Exiting static semantics" << endl; 
}

int verifyNode(node *n,  int *var_count, ofstream* output) { 
	int stack_dist = -1; 
	if (n-> label == varList_node) { 
		if (n-> token1.tokenType == IDENTIFIER_TOKEN) { 
			stack_dist = id_stack.find(n-> token1, 1); // redefinition
			if (*var_count > 0) { 
				stack_dist = id_stack.find(n->token1, 1);
			} 
			 
			id_stack.push(n-> token1); 
			*output << "PUSH" << endl; 
			
			(*var_count)++; 
		
			return stack_dist; 	

		}
	} 
	else if (n-> label == R_node || n-> label == in_node || n-> label == assign_node || n-> label == assign_node || n-> label == exp_node || n->label == M_node || n-> label == N_node) {
		if (n-> token1.tokenType == IDENTIFIER_TOKEN) { 
			
			stack_dist = id_stack.find(n->token1, 2); 
			return stack_dist; 
		} 
	}
	return stack_dist; 
	//cout << "Exiting verifyNode" << endl; 
}




