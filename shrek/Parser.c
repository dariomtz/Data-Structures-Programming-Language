//
//  Parser.c
//  shrek
//
/*
    Implementation of Parser struct and its functions
*/

#include "Parser.h"
#include "Map.h"

/*
 typedef enum {ERROR = -1,
 FALSE, TRUE, NAME, INT, FLOAT, STRING, END_LINE, LPAREN, RPAREN, LBRACE, RBRACE, 1
 LIST, STACK, QUEUE, SET, MAP, PLUS1, PLUS2,2
 MULTIPLICATION, DIVISION, MODULO,3
 SUM, SUBSTRACT,4
 GREATER, GREATER_EQUAL, SMALLER_EQUAL, SMALLER,5
 EQUAL, NOT_EQUAL,6
 BINARY_AND,7
 BINARY_XOR,8
 BINARY_OR,9
 LOGIC_AND,10
 LOGIC_OR, 11
 TERNARY, 12
 ASIGN, SUM_ASIGN, SUBSTRACT_ASIGN, MULTIPLICATION_ASIGN, DIVISION_ASIGN, MODULO_ASIGN, 13
 IF, ELSE, WHILE, FOR, BREAK, CONTINUE, BEGIN_MAIN, END_MAIN, FUNCTION 14
 //last in priority
 } tokenType;*/

int priority(tokenType type){
    if (0 <= type && type <= 10) {
        return 1;
    }else if(11 <= type && type <= 17){
        return 2;
    }else if(18 <= type && type <= 20){
        return 3;
    }else if(21 <= type && type <= 22){
        return 4;
    }else if(23 <= type && type <= 26){
        return 5;
    }else if(27 <= type && type <= 28){
        return 6;
    }else if(29 == type){
        return 7;
    }else if (30 == type){
        return 8;
    }else if (31 == type){
        return 9;
    }else if (32 == type){
        return 10;
    }else if (33 == type){
        return 11;
    }else if (34 == type){
        return 12;
    }else if (35 <= type && type <= 40){
        return 13;
    }else if (40 <= type && type <= 50){
        return 14;
    }else{
        return -1;
    }
}

typedef struct strNode * Node;

struct strNode{
    Data value;
    Node leftChild, rightChild;
};

struct strBlock {
    Node * sentences;
    int size, cap;
    error error;
};

typedef struct strBlock * Block;

struct strParser {
    Block main;
    Map functions;
    error error;
};

Block block_create(Lexer lexer, int a, int b){
    Block newBlock = (Block) calloc(1, sizeof(struct strBlock));
    
    newBlock -> cap = 10;
    newBlock -> sentences = (Node *) malloc(sizeof(Node) * newBlock -> cap);
    
    return newBlock;
}

Parser parser_create(Lexer lexer){
    Parser newParser = (Parser) calloc(1, sizeof(struct strParser));
    
    newParser -> error .type = NO_ERROR;
    newParser -> error.message = (char *) malloc(100);
    
    int numOfFunctions = 0, beginMain = -1, endMain = -1;
    
    for (int i = 0; i < lexer_numberOfTokens(lexer); i++) {
        if (lexer_getToken(lexer, i) -> type == FUNCTION) {
            numOfFunctions++;
        }else if (lexer_getToken(lexer, i) -> type == BEGIN_MAIN){
            beginMain = i;
        }else if (lexer_getToken(lexer, i) -> type == END_MAIN){
            endMain = i;
        }
    }
    
    if (beginMain == -1 || endMain == -1) {
        return NULL;
    }
    
    char * name = malloc(sizeof(char)*10);
    char name2[] = "functions";
    for (int i = 0; i < 10; i++) {
        name[i] = name2[i];
    }
    
    newParser -> functions = map_create(data_create(STRING, name), numOfFunctions * 2);

    for (int i = 0; i < lexer_numberOfTokens(lexer); i++) {
        
        if (lexer_getToken(lexer, i) -> type == FUNCTION) {
            
            Token nameToken = lexer_getToken(lexer, ++i);
            if(nameToken -> type != NAME){
                newParser -> error.type = NO_NAME_AFTER_FUNC;
                strcpy(newParser -> error.message, "\nPARSER ERROR: Expected NAME after FUNCTION\n");
                return newParser;
            }
            
            Data funcName = data_create(STRING, nameToken -> value);
            
            int leftBrace = i++;
            if (lexer_getToken(lexer, leftBrace) -> type != LBRACE) {
                newParser -> error.type = NO_LBRACE_AFTER_FUNC_NAME;
                strcpy(newParser -> error.message, "\nPARSER ERROR: Expected LEFT BRACE after FUNCTION NAME\n");
                return newParser;
            }
            
            int openedBraces = 0;
            while (lexer_getToken(lexer, i) -> type != RBRACE && openedBraces == 0) {
                if (lexer_getToken(lexer, i) -> type == LBRACE) {
                    openedBraces++;
                }else if (lexer_getToken(lexer, i) -> type == LBRACE){
                    
                }
                i++;
                
                if (!lexer_getToken(lexer, i)) {
                    newParser -> error.type = NO_RBRACE_FOR_FUNC;
                    strcpy(newParser -> error.message, "\nPARSER ERROR: Missing closing brace for function");
                    strcat(newParser -> error.message, funcName -> value);
                    strcat(newParser -> error.message, "\n");
                    return newParser;
                }
            }
            
            
            Block block = block_create(lexer, leftBrace, i);
            if (!block || block -> error.type != NO_ERROR ) {
                newParser -> error.type = block -> error.type;
                newParser -> error.message = block -> error.message;
                return newParser;
            }
            
            Data func = data_create(FUNCTION, block );
            
            
            map_put(newParser -> functions, funcName, func);
        }
    }
    
    newParser -> main = block_create(lexer, beginMain, endMain);
    
    
    return newParser;
}









