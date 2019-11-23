//
//  Parser.h
//  shrek
//
/*
    Definition of Parser and Parser functions.
*/

#ifndef Parser_h
#define Parser_h

#include <stdio.h>
#include "Lexer.h"

typedef struct strParser * Parser;

Parser parser_create(Lexer lexer);
Data parser_getFunction(Lexer lexer, Data name);

#endif /* Parser_h */
