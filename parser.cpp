#include <iostream> 
#include <string> 
#include <iomanip>
#include <fstream>
#include "parser.h"
#include "scanner.h"
#include "token.h" 
#include "testTree.h"
#include "testScanner.h"

using namespace std; 

static Token token;
/*
 *  program_node, vars_node, varList_node, exp_node, M_node, N_node, R_node, stats_node, mStat_node, stat_node, block_node, in_node, out_node, if_node, loop_node, assign_node,RO_node,

node *program(); node *vars(); node *varList(); node *exp(); node *M(); node *N(); node *R(); node *stats(); node *mStat(); node *stat(); node *block(); node *in(); node *out(); node *If(); node *loop(); node *assign(); node *RO();  

XOPEN_TOKEN, XCLOSE_TOKEN, XLOOP_TOKEN, XDATA_TOKEN, XEXIT_TOKEN, XIN_TOKEN, XOUT_TOKEN, XCOND_TOKEN, XTHEN_TOKEN, XLET_TOKEN, XFUNC_TOKEN, EOF_TOKEN, IDENTIFIER_TOKEN, INTEGER_TOKEN, EQUAL_TOKEN, LESS_TOKEN,GREATER_TOKEN, TILDE_TOKEN, COLON_TOKEN, SEMICOLON_TOKEN, PLUS_TOKEN, SUBTRACT_TOKEN, MULTIPLY_TOKEN, DIVIDE_TOKEN, PERCENT_TOKEN, PERIOD_TOKEN, COMMA_TOKEN, LEFT_PARENTHESIS_TOKEN, RIGHT_PARENTHESIS_TOKEN, LEFT_BRACE_TOKEN, RIGHT_BRACE_TOKEN, LEFT_BRACKET_TOKEN, RIGHT_BRACKET_TOKEN, GREATERGREATER_TOKEN, LESSLESS_TOKEN, COMMENT_TOKEN,

 */ 

//extern string token_types[];

node* parser(istream &file) { 
	node *root;

	cout << "Beginning parser" << endl;  

	lookahead_ch(file); 
	token = scanner(file); 
	root = program(file);
	

	if (token.tokenType != EOF_TOKEN) { 
		cout << "Parser error: extra token after EOF" << endl; 
	} 
	return root; 
} 

// <program>  -> <vars> xopen <stats> xclose
node *program(istream &file) { 
	//cout << "Entering program function" << endl;
	node *n = createNode(program_node);
	n-> c1 = vars(file); 
	if (token.tokenInstance == "xopen") {
		token = scanner(file); 
		n -> c2 = stats(file);
		
		//token = scanner(file); 
		if (token.tokenInstance == "xclose") { 
			token = scanner(file); 
			return n; 
		}
		cout << "<program>" << endl;
		error(); 
	}
	cout << "<program>" << endl; 	
	error(); 
	return NULL; 	 
} 
	
// <vars> -> empty | xdata <varList> 
node *vars(istream &file) { 
	if (token.tokenInstance == "xdata") {
		node *n = createNode(vars_node); 
		token = scanner(file); 
		n -> c1 = varList(file); 
		return n; 
	} else { 
		return NULL; // empty 
	}
} 

// <varList> -> identifier : integer ; | identifier : integer <varList> 
node *varList(istream &file) { 
	if (token.tokenType == IDENTIFIER_TOKEN) {
		node *n = createNode(varList_node); 
		n-> token1 = token; 
		token = scanner(file); 

		if (token.tokenType == COLON_TOKEN) { 
			token = scanner(file); 
			
			if (token.tokenType == INTEGER_TOKEN) { 
				n-> token2 = token; 
				token = scanner(file); 

				if (token.tokenType == SEMICOLON_TOKEN) { 
					token = scanner(file); 
					return n; 
				} else { 
					n-> c1 = varList(file); 
					return n; 
				}
			}
		}
	}
	cout << "<varList>" << endl; 
	error(); 
	return NULL;	
} 

// <exp> -> <M> / <exp> | <M> * <exp> | <M>
node *exp(istream &file) { 
	node *n = createNode(exp_node);
	n -> c1 = M(file);  

	if (token.tokenType == DIVIDE_TOKEN) { 
		n -> token1 = token; 
		token = scanner(file); 
		n -> c2 = exp(file); 
		return n; 
	} else if (token.tokenType == MULTIPLY_TOKEN) { 
		n -> token1 = token; 
		token = scanner(file); 
		n -> c2 = exp(file);
		return n;  
	} 
	return n; 
}

// <M> -> <N> + <M> | <N> 
node *M(istream &file) { 
	node *n = createNode(M_node); 
	n -> c1 = N(file); 

	if (token.tokenType == PLUS_TOKEN) { 
		n-> token1 = token; 
		token = scanner(file); 
		n-> c2 = M(file); 
		return n; 
	} 
	
	return n; 
} 

// <N> -> <R> - <N> | ~ <N> | <R>
node *N(istream &file) { 
	node *n = createNode(N_node); 

	if (token.tokenType == TILDE_TOKEN) { 
		n-> token1 = token; 
		token = scanner(file);
		n -> c1 = N(file);  
		return n; 
	} else { 
		n-> c1 = R(file); 
	
		if (token.tokenType == SUBTRACT_TOKEN) { 
			n-> token1 = token; 
			token = scanner(file); 
			n-> c2 = N(file); 
			return n; 
		} else { 
			return n; 
		} 
	}

	return NULL; 
} 


// <R> -> (<exp>) | identifier | integer
node *R(istream &file) { 
	node *n = createNode(R_node);
	 
	if (token.tokenInstance == "(") { 
		token = scanner(file); 
		n-> c1 = exp(file); 

		if (token.tokenInstance == ")") { 
			token = scanner(file);
		}
	} 
	 
	else if (token.tokenType == IDENTIFIER_TOKEN){ 
		n -> token1 = token; 
		token = scanner(file); 
	} 
	else if (token.tokenType == INTEGER_TOKEN) { 
		n -> token1 = token; 
		token = scanner(file); 
	} else { 
		cout << "<R>" << endl;
		error(); 
	} return n;  
} 


// <stats> -> <stat> <mStat> 
node *stats(istream &file) { 
	node *n = createNode(stats_node); 
	n -> c1 = stat(file); 
	n -> c2 = mStat(file); 
	return n; 
} 

// <mStat> -> empty | <stat> <mStat> 
node *mStat(istream &file) { 
	node *n = createNode(mStat_node);
	
	// check if current token can start a <stat> 
	if (token.tokenType == XIN_TOKEN || token.tokenType == XOUT_TOKEN || token.tokenType == LEFT_BRACE_TOKEN || token.tokenType == XCOND_TOKEN || token.tokenType == XLOOP_TOKEN || token.tokenType == XLET_TOKEN) {
		n -> c1 = stat(file); 
		n -> c2 = mStat(file);
		return n;
	} 

	return n; 
 
}
// <stat> -> <in>|<out>|<block>|<if>|<loop>|<assign>
node *stat(istream &file) { 
	node *n = createNode(stat_node);
	 
	if (token.tokenType == XIN_TOKEN) { 
		n -> c1 = in(file);
		return n;
	} else if (token.tokenType == XOUT_TOKEN) { 
		n-> c1 = out(file); 
		return n;
	} else if (token.tokenType == LEFT_BRACE_TOKEN) { 
		n -> c1 = block(file); 
		return n;
	} else if (token.tokenType == XCOND_TOKEN) { 
		n -> c1 = If(file); 
		return n;
	} else if (token.tokenType == XLOOP_TOKEN) { 
		n -> c1 = loop(file); 
		return n; 
	} else if (token.tokenType == XLET_TOKEN) { 
		n -> c1 = assign(file); 
		return n;
	} 
	cout << "<stat>" << endl;	
	error();  
	return NULL; 
} 


// <block> -> { <vars> <stats> } 
node *block(istream &file) { 
	if (token.tokenType == LEFT_BRACE_TOKEN) {
		node *n = createNode(block_node);
		token = scanner(file); 

		n -> c1 = vars(file); 
		n -> c2 = stats(file); 
		
		if (token.tokenType == RIGHT_BRACE_TOKEN) { 
			token = scanner(file); 
			return n; 
		}
		cout << "<block>" << endl;
		error(); 
	} 
	cout << "<block>" << endl;
	error(); 
	return NULL;
} 
	

// <in> -> xin >> identifier; 
node *in(istream &file) { 
	node *n = createNode(in_node); 
	
	if (token.tokenType == XIN_TOKEN) { 	
		token = scanner(file); 
	
		if (token.tokenInstance == ">>") { 
			token = scanner(file); 
		
			if (token.tokenType == IDENTIFIER_TOKEN) { 
				n-> token1 = token; 
				token = scanner(file); 
				
				if (token.tokenType == SEMICOLON_TOKEN) { 
					token = scanner(file); 
					return n; 
				}
			}
		}
	}
	cout << "<in>" << endl;
	error(); 
	return NULL;
} 

// <out> -> xout << <exp>; 
node *out(istream &file) { 
	node *n = createNode(out_node); 
	
	if (token.tokenType == XOUT_TOKEN) { 
		token = scanner(file); 

		if (token.tokenInstance == "<<") { 
			token = scanner(file); 
			n -> c1 = exp(file); 
		
			if (token.tokenType == SEMICOLON_TOKEN) { 
				token = scanner(file); 
				return n; 
			} 
		}
	} 
	cout << "<out>" << endl;
	error(); 
	return NULL; 
}

// <if> -> xcond [ <exp> <RO>  <exp> ] <stat> 
node *If(istream &file) { 
	node *n = createNode(if_node); 

	if (token.tokenType == XCOND_TOKEN) { 
		token = scanner(file); 

		if (token.tokenType == LEFT_BRACE_TOKEN) {
			token = scanner(file); 
			n -> c1 = exp(file); 
			n -> c2 = RO(file); 
			n -> c3 = exp(file); 
		
			if (token.tokenType == RIGHT_BRACE_TOKEN) { 
				token = scanner(file); 
				n -> c4 = stat(file); 
				return n; 
			} 
		}
	}
	cout << "<if>" << endl;
	error(); 
	return NULL; 
} 

// <loop> -> xloop [ <exp> <RO> <exp> ] <stat> 
node *loop(istream &file) { 
	node *n = createNode(loop_node); 

	if (token.tokenType == XLOOP_TOKEN) { 
		token = scanner(file); 
		
		if (token.tokenType == LEFT_BRACE_TOKEN) {
                        token = scanner(file);
                        n -> c1 = exp(file);
                        n -> c2 = RO(file);
                        n -> c3 = exp(file);

                        if (token.tokenType == RIGHT_BRACE_TOKEN) {
                                token = scanner(file);
                                n -> c4 = stat(file);
                                return n;
                        }
		}
        }
	cout << "<loop>" << endl;
	error(); 
        return NULL;
}


// <assign> -> xlet identifier <exp>; 
node *assign(istream &file) { 
	node *n = createNode(assign_node); 

	if (token.tokenType == XLET_TOKEN) { 
		n -> token1 = token; 
		token = scanner(file); 
		
		if (token.tokenType == IDENTIFIER_TOKEN) { 
			n -> token2 = token; 
			token = scanner(file); 
			n -> c1 = exp(file); 
		
			if (token.tokenType == SEMICOLON_TOKEN) { 
				token = scanner(file); 
				return n; 
			}
		}
	}
	cout << "<assign>" << endl;
	error(); 
	return NULL; 	
}
// <RO> -> <<(one token) | >> (one token) | < | > | = | %
node *RO(istream &file) { 
	node *n = createNode(RO_node);
	if (token.tokenInstance == "<<") { 
		n-> token1 = token; 
		token = scanner(file); 
		return n; 
	} else if (token.tokenInstance == ">>") { 
		n-> token1 = token;
                token = scanner(file);
                return n;
	} else if (token.tokenType == LESS_TOKEN) { 
		n-> token1 = token;
                token = scanner(file);
                return n;
	} else if (token.tokenType == GREATER_TOKEN) { 
		n-> token1 = token;
                token = scanner(file);
                return n;
	} else if (token.tokenType == EQUAL_TOKEN) { 
		n-> token1 = token;
                token = scanner(file);
                return n;
	} else if (token.tokenType == PERCENT_TOKEN) { 	
		n-> token1 = token;
                token = scanner(file);
                return n;
	} 
	cout << "<RO>" << endl;
	error(); 
	return NULL;
}

void error() {
   cout << "ERROR: Unexpected token - " << token_types[token.tokenType] << endl; 
   exit(token.tokenType);
}
