//
//  stdhead.h
//  shrek
//
/*
    Definition of structs that will be needed in more than one library
*/

#ifndef stdhead_h
#define stdhead_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {ERROR = -1,
    FALSE, TRUE, NAME, INT, FLOAT, STRING, END_LINE, LPAREN, RPAREN, LBRACE, RBRACE,//first in priority
    USE_METHOD,
    LIST, STACK, QUEUE, SET, MAP, PLUS1, MINUS1,
    MULTIPLICATION, DIVISION, MODULO,
    SUM, SUBSTRACT,
    GREATER, GREATER_EQUAL, SMALLER_EQUAL, SMALLER,
    EQUAL, NOT_EQUAL,
    BINARY_AND,
    BINARY_XOR,
    BINARY_OR,
    LOGIC_AND,
    LOGIC_OR,
    TERNARY,
    ASIGN, SUM_ASIGN, SUBSTRACT_ASIGN, MULTIPLICATION_ASIGN, DIVISION_ASIGN, MODULO_ASIGN,
    COMA, SEMICOLON,
    IF, ELSE, WHILE, FOR, BREAK, CONTINUE, BEGIN_MAIN, END_MAIN, FUNCTION,
    //last in priority
} tokenType;

typedef enum {
    NO_ERROR,
    //lexer errors
    UNREGONIZED_TOKEN, EMPTY_FILE,
    
    //Parser errors, from 100 to 200
    NO_NAME_AFTER_FUNC = 100, NO_LBRACE_AFTER_FUNC_NAME, NO_RBRACE_FOR_FUNC, NO_RBRACE_FOR_IF, NO_RBRACE_FOR_WHILE, NO_RBRACE_FOR_FOR, NO_END_LINE_FOR_SENTENCE, BLOCK_DOESNT_EXIST, PARSER_DOESNT_EXIST,
}errorType;

typedef enum {false, true} bool;
typedef struct strData * Data;
typedef void * Type;

typedef struct {
    errorType type;
    char * message;
}error;

struct strData {
    tokenType type;
    Type value;
};

typedef struct strNode * Node;

struct strNode{
    Data value;
    Node leftChild, rightChild;
};

Data data_create(tokenType type, Type value);
int cmp(Data a, Data b);
void print(Data data);
void destroy(Data data);

#endif /* stdhead_h */
