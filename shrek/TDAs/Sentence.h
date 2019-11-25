//
//  Sentence.h
//  shrek
//
//  Created by Darío Martínez on 11/24/19.
//  Copyright © 2019 Darío Martínez. All rights reserved.
//

#ifndef Sentence_h
#define Sentence_h

#include "stdhead.h"
#include "Lexer.h"

typedef struct strSentence * Sentence;

Sentence sentence_create(Lexer lexer, int a, int b);
void sentence_destroy(Sentence sentence);
Sentence sentence_getLeftSubsentece(Sentence sentence);
Sentence sentence_getRightSubsentece(Sentence sentence);
error sentence_getErrorStatus(Sentence sentence);


#endif /* Sentence_h */
