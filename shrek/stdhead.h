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
    FALSE, TRUE, NAME, INT, FLOAT, STRING, LIST, STACK, QUEUE, SET, MAP,END_LINE, LPAREN, RPAREN, LBRACE, RBRACE,//first in priority
    PLUS1, MINUS1,
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
    IF, ELSE, WHILE, FOR, BREAK, CONTINUE, BEGIN_MAIN, END_MAIN, FUNCTION,
    //last in priority
} tokenType;

typedef enum {
    NO_ERROR,
    //lexer errors
    UNREGONIZED_TOKEN,
    
    //Parser errors, from 100 to 200
    NO_NAME_AFTER_FUNC = 100, NO_LBRACE_AFTER_FUNC_NAME, NO_RBRACE_FOR_FUNC,
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

Data data_create(tokenType type, Type value);
int cmp(Data a, Data b);
int print(Data data);
int destroy(Data data);

#endif /* stdhead_h */
