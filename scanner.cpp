#include "token.h" 
#include "scanner.h"
#include "testTree.h" 
#include <iostream>
#include <fstream> 
#include <stdlib.h> 
#include <string> 

using namespace std;

/* After designing FSA, represent the 2-d array representation for the FSA as array of integers

This FSA should recognize repeat operator and delimiter characters and stay in the respective state until recognized as a token.
*/  

const int max_keywords = 11; 
const char *keywords[] = { 
	"xopen", "xclose", "xloop", "xdata", "xexit", "xin", "xout", "xcond", "xthen", "xlet", "xfunc"}; 
const int max_length = 8; // for int constants and keywords
int lineNum = 1; 
character next_ch; 
character filter(istream &FILE) { 
	character input; 
	char ch = FILE.get();
 
	int col = getFSAColumn(ch);
	//DEBUG: cout << "Current ch: " << ch << ", FSA Column: " << col << endl;

	while (col == COMMENT) { 
		do { 
			ch = FILE.get(); 
			if (ch == '\n') { 
				lineNum++; 
			}
		} while (ch != '$'); 
		
		ch = FILE.get(); 
		col = getFSAColumn(ch); 
	} 

	if (col >= LOWERCASE && col <= END) { 
		input.value = ch; 
		input.FSAColumn = col; 
		input.lineNum = lineNum; 

		if (ch == '\n') { 
			lineNum++; 
		} 
		
		return input; 
	} else {
		lexicalError(lineNum); 
		exit(col); 
	}
}


void lookahead_ch(istream &FILE) { 
	next_ch = filter(FILE); 
} 

Token new_token; 

Token scanner(istream &FILE) { 
	states curr_state = s1; // initial state 
	
	//states next_state; 
	states next_state; 

	//Token token; 
	// store instance of token: 
	string instance = "";
		
	while (curr_state != 1001) { 
		next_state = static_cast<states>(FSA_Table[curr_state][next_ch.FSAColumn]); 
		//cout << "Current State: " << curr_state << ", Next State: " << next_state << ", Next Character: " << next_ch.value << endl;
		if (next_state == -1) {
			lexicalError(next_ch.lineNum); 
		}
		
		// Multi-characters like: >> << 
		if (curr_state == 5 && next_ch.value == '>') {
            		next_ch = filter(FILE); // Move to the next character after '>'
            		instance = ">>"; 
            		next_state = FINAL; // Set the next state to the final state
        	}

		if (curr_state == 4 && next_ch.value == '<') {
            		next_ch = filter(FILE); // Move to the next character after '>'
            		instance = "<<"; 
            		next_state = FINAL; // Set the next state to the final state
        	}


		// Final state 
		if (next_state == FINAL) { 
			new_token.tokenType = static_cast<tokenID>(curr_state + max_keywords);
			new_token.tokenInstance = instance; 
			new_token.lineNumber = lineNum;

			//cout << "Token Instance: " << instance << ", Token ID: " << token.tokenType << endl;
			
			if (curr_state == s2) { 
				for (int i = 0; i < max_keywords; i++) { 
					if (keywords[i] == new_token.tokenInstance) { 
						new_token.tokenType = static_cast<tokenID>(i); 
						break;
					}
				}
			}
			return new_token; 
		} else { 
			if (next_ch.FSAColumn != WS) { 
				instance += next_ch.value;
			} 

			curr_state = next_state; 
			next_ch = filter(FILE); 
		
			if (instance.length() == max_length) { 
				//token.tokenType = static_cast<tokenID>(curr_state);
				new_token.tokenType = static_cast<tokenID>(curr_state + max_keywords);
				new_token.tokenInstance = instance; 
				new_token.lineNumber = next_ch.lineNum; 
				return new_token; 
			} 
		}
	}
	
	return new_token;

}

// a-z   A-Z   0-9    WS     =     <     >     ~     :     ;     +     -     *     /     %     .     (     )     ,     {     }     [     ]     $   EOF
/* 
enum token_names {
        LOWERCASE,
        UPPERCASE,
        INTEGER,
        WS,
        EQUAL,
        LESS,
        GREATER,
        TILDE,
        COLON,
        SEMICOLON,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVISION,
        PERCENT,
        PERIOD,
        LPARENTHESIS,
        RPARENTHESIS,
        COMMA,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        COMMENT,
        END
};
*/ 
int getFSAColumn(char ch) {
	if (islower(ch)) { 
		return LOWERCASE;
	}
	if (isupper(ch)) { 
		return UPPERCASE;
	} 
	if (isdigit(ch)) { 
		return INTEGER;
	}
	if (isspace(ch)) { 
		return WS; 
	} 
	if (!islower(ch) && !isupper(ch) && !isdigit(ch) && !isspace(ch)) { 
		switch(ch) { 
			case '=': 
				return EQUAL; 
			case '<': 
				return LESS;
			case '>': 
				return GREATER; 
			case '~':
				return TILDE;
			case ':': 
				return COLON; 
			case ';': 
				return SEMICOLON;
			case '+': 
				return PLUS;
			case '-': 
				return MINUS; 
			case '*': 
				return MULTIPLY;
			case '/': 
				return DIVISION;
			case '%': 
				return PERCENT; 
			case '.': 
				return PERIOD;
			case '(': 
				return LEFT_PARENTHESIS;
			case ')': 
				return RIGHT_PARENTHESIS;
			case ',':
				return COMMA;
			case '{': 
				return LEFT_BRACE;
			case '}': 
				return RIGHT_BRACE; 
			case '[': 
				return LEFT_BRACKET;
			case ']':
				return RIGHT_BRACKET;
			case EOF: 
				return END; 
			case '$': 
				return COMMENT;
			default: 
				return -1;
			}
	}
	return -1;
}

void lexicalError(int lineNum) { 
	cerr << "LEXICAL ERROR - line: " << lineNum << endl; 
} 

