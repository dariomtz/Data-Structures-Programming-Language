//
//  Block.h
//  shrek
//
//

#ifndef Block_h
#define Block_h

#include "stdhead.h"
#include "Sentence.h"
#include "Lexer.h"

typedef struct strBlock * Block;

Block block_create(Lexer lexer, int a, int b);
void block_destroy(Block block);
int block_size(Block block);
error block_getErrorStatus(Block block);
Sentence block_getSentence(Block block, int pos);

#endif /* Block_h */
