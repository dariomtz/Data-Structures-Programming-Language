//
//  Stack.h
//  shrek
//
//

#ifndef Stack_h
#define Stack_h

#include "stdhead.h"

typedef struct strStack * Stack;

Stack stack_create(Data name);
char * stack_name(Stack stack);
void stack_destroy(Stack stack);
bool stack_isEmpty(Stack stack);
int size(Stack stack);
Data stack_top(Stack stack);
Data pop(Stack stack);
void push(Stack stack,Data data);

#endif /* Stack_h */
