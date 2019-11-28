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
#include "List.h"
#include "Block.h"

typedef struct strParser * Parser;

Parser parser_create(Lexer lexer);
void parser_destroy(Parser parser);
Map parser_getMemoryMap(Parser parser);
error parser_getErrorStatus(Parser parser);

#endif /* Parser_h */
