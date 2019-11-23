//
//  List.h
//  shrek
//
/*
    Definition of List container and its functions
 */

#ifndef List_h
#define List_h

#include "stdhead.h"

typedef struct strList * List;
typedef struct strNode * Iterator;

List list_create(Data name);
void list_destroy(List list);
int list_size(List list);
void list_add(List list, Data);
Data list_get(List list, int pos);
void list_set(List list, int pos, Data data);
void list_remove(List list, int pos);

Iterator list_begin(List list);
Iterator list_end(List list);
bool list_exists(Iterator it);
Iterator list_next(Iterator it);
Iterator list_prior(Iterator it);
Data list_data(Iterator it);

#endif /* List_h */
