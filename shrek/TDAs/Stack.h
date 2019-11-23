//
//  Stack.h
//  shrek
//
//

#ifndef Stack_h
#define Stack_h

#include "stdhead.h"

typedef struct strStack * STACK;

STACK stack_create(Data name);
void stack_destroy(STACK stack);
bool stack_isEmpty(STACK stack);
int size(STACK stack);
Data stack_top(STACK stack);
Data pop(STACK stack);
void push(STACK,Type data);

#endif /* Stack_h */
