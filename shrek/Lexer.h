//
//  Lexer.h
//  shrek
//
/*
    Definition of Lexer and Lexer functions.
*/

#ifndef Lexer_h
#define Lexer_h

#include "stdhead.h"

typedef struct strLexer * Lexer;
typedef Data Token;

Lexer lexer_create(FILE * f);
void lexer_destroy(Lexer lexer);
int lexer_numberOfTokens(Lexer lexer);
Token lexer_getToken(Lexer lexer, int pos);

#endif /* Lexer_h */
