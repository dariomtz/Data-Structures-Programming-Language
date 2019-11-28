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
void stack_print(Stack stack);
Data stack_name(Stack stack);
void stack_destroy(Stack stack);
bool stack_isEmpty(Stack stack);
int size(Stack stack);
Data stack_top(Stack stack);
Data stack_pop(Stack stack);
void stack_push(Stack stack,Data data);

#endif /* Stack_h */
