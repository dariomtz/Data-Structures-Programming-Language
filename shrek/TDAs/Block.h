//
//  Block.h
//  shrek
//
//

#ifndef Block_h
#define Block_h

#include "stdhead.h"
#include "Sentence.h"
#include "List.h"
#include "Lexer.h"

typedef struct strBlock * Block;

Block block_create(Lexer lexer, int leftBrace, int rightBrace, Data name);
void block_print(Block block);
void block_destroy(Block block);
int block_size(Block block);
List block_getParams(Block block);
void block_setParams(Block block, List params);
error block_getErrorStatus(Block block);
Sentence block_getSentence(Block block, int pos);

#endif /* Block_h */
