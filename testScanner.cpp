#include "testScanner.h"
#include "token.h"  
#include "scanner.h" 
#include <iostream>
#include <istream>
#include <string> 

using namespace std;

string token_types[] = {
	"XOPEN - KEYWORD",
	"XCLOSE - KEYWORD",
	"XLOOP - KEYWORD",
	"XDATA - KEYWORD",
	"XEXIT - KEYWORD",
	"XIN - KEYWORD",
	"XOUT - KEYWORD",
	"XCOND - KEYWORD",
	"XTHEN - KEYWORD",
	"XLET - KEYWORD", 
	"XFUNC - KEYWORD",
	
	"EOF", 	
	"IDENTIFIER",
	"INTEGER",
	
	"EQUAL - OP",
        "LESS - OP",
    	"GREATER - OP",
    	"TILDE - OP",
	"COLON - DELIMITER",
    	"SEMICOLON - DELIMITER",
  	"PLUS - OP",
    	"SUBTRACT - OP",
    	"MULTIPLY - OP",
    	"DIVIDE - OP",
    	"PERCENT - OP",
	"PERIOD - DELIMITER", 
	"LEFT_PARENTHESIS - DELIMITER",
    	"RIGHT_PARENTHESIS - DELIMITER",
	"COMMA - DELIMITER",
	"LEFT_BRACE - DELIMITER",
    	"RIGHT_BRACE - DELIMITER",
    	"LEFT_BRACKET - DELIMITER",
    	"RIGHT_BRACKET - DELIMITER",
	"GREATERGREATER - OP",
        "LESSLESS - OP",
	"COMMENT",
};

void testScanner(istream &FILE) {  
	Token tkn; 
	int end = 0; 

	lookahead_ch(FILE);

	while (!end) { 
		tkn = scanner(FILE); 
	if (tkn.tokenType == EOF_TOKEN) { 
		end = 1; 
	} 
	printToken(tkn); 
	}
}	

void printToken(Token tkn) {
        cout << "Token ID: " << token_types[tkn.tokenType] << "\n" << "Line Number: " << tkn.lineNumber << "\n" << "Token Instance: " << tkn.tokenInstance << "\n" << " ------------- " << endl;
}




