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

typedef enum {ERROR = -1,
    FALSE, TRUE, NAME, INT, FLOAT, STRING, LIST, STACK, QUEUE, SET, MAP,END_LINE, LPAREN, RPAREN, LBRACE, RBRACE,//first in priority
    PLUS1, PLUS2,
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
    IF, ELSE, WHILE, FOR, BREAK, CONTINUE, BEGIN_MAIN, END_MAIN,
    //last in priority
} tokenType;

typedef enum {false, true} bool;
typedef struct strData * Data;
typedef void * Type;

struct strData {
    tokenType type;
    Type value;
};

Data createData();

int cmp(Data a, Data b);
int print(Data data);
int destroy(Data data);

#endif /* stdhead_h */
