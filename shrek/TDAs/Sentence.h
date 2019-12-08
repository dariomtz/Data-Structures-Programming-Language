//
//  Sentence.h
//  shrek
//
//

#ifndef Sentence_h
#define Sentence_h

#include "stdhead.h"
#include "Lexer.h"

typedef struct strSentence * Sentence;

Sentence sentence_create(Lexer lexer, int a, int b);
void sentence_destroy(Sentence sentence);
Data sentence_getValue(Sentence sentence);
Sentence sentence_getLeftSubsentece(Sentence sentence);
Sentence sentence_getRightSubsentece(Sentence sentence);
error sentence_getErrorStatus(Sentence sentence);
Data sentence_getValue(Sentence sentence);


#endif /* Sentence_h */
