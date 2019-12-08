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

#define MEMORY_CAPACITY 100

typedef enum {ERROR = -1,
    FALSE, TRUE, NAME, INT, FLOAT, STRING, END_LINE, LPAREN, RPAREN, LBRACE, RBRACE,//first in priority
    USE_METHOD,
    LIST, STACK, QUEUE, SET, MAP, NEGATION, PLUS1, MINUS1,
    MULTIPLICATION, DIVISION, MODULO,
    SUM, SUBSTRACT,
    GREATER, GREATER_EQUAL, SMALLER_EQUAL, SMALLER,
    EQUAL, NOT_EQUAL,
    BINARY_AND,
    BINARY_XOR,
    BINARY_OR,
    LOGIC_AND,
    LOGIC_OR,
    TERNARY_QM, TERNARY_DOTS,
    ASIGN, SUM_ASIGN, SUBSTRACT_ASIGN, MULTIPLICATION_ASIGN, DIVISION_ASIGN, MODULO_ASIGN,
    COMMA, SEMICOLON,
    IF, ELSE, WHILE, FOR, BREAK, CONTINUE, SENTENCE, FUNCTION,
    //last in priority
} tokenType;

typedef enum {
    NO_ERROR,
    //lexer errors
    UNREGONIZED_TOKEN, EMPTY_FILE, FILE_DOESNT_EXIST,
    
    //Parser errors, from 100 to 200
    NO_NAME_AFTER_FUNC = 100, NO_LPAREN_FOR_PARAMS, NO_RPAREN_FOR_PARAMS, NO_LBRACE_FOR_FUNC, NO_RBRACE_FOR_FUNC,
    NO_RBRACE_FOR_IF, NO_RBRACE_FOR_WHILE, NO_RBRACE_FOR_FOR, NO_END_LINE_FOR_SENTENCE, BLOCK_DOESNT_EXIST, PARSER_DOESNT_EXIST, TOKEN_OUTSIDE_MAIN, REPEATED_NAME_FOR_FUNCTION, INVALID_PARAMS, SENTENCE_DOESNT_EXIST, INVALID_OPERATION, MISSING_SEMICOLON_FOR, EXTRA_SEMICOLON_FOR,
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
    bool dest;
};

Data data_create(tokenType type, Type value);
Data data_makeCopy(Data data);
Data data_copyResolvedData(Data data);
int data_cmp(Data a, Data b);
void data_print(Data data);
void data_destroy(Data data);

#endif /* stdhead_h */
