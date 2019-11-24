//
//  Set.h
//  shrek
//
//

#ifndef Set_h
#define Set_h

#include "stdhead.h"

typedef struct strSet * Set;

Set set_create(Data name);
void set_print(Set set);
char * set_name(Set set);
void set_destroy(Set set);
int set_size(Set set);
bool set_add(Set set, Data data);
bool set_contains(Set set, Data data);
bool set_remove(Set set, Data data);
void set_print(Set set);

#endif /* Set_h */
