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
#include "Map.h"
#include "Block.h"

typedef struct strParser * Parser;

Parser parser_create(Lexer lexer);
void parser_destroy(Parser parser);
Block parser_getMain(Parser parser);
Block parser_getFunction(Parser parser, Data name);
error parser_getErrorStatus(Parser parser);

#endif /* Parser_h */
