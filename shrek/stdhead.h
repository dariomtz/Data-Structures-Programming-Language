//
//  stdhead.h
//  shrek
//
/*
    Definition of structs that will be needed in more than one library
*/

#ifndef stdhead_h
#define stdhead_h

#include <stdio.h>
#include <stdlib.h>

typedef void * Type;
typedef enum {ERROR = -1} tokenType;
typedef enum {FALSE, TRUE} bool;
typedef struct strData * Data;

struct strData {
    tokenType type;
    Type value;
};

int cmp(Data a, Data b);
int print(Data data);
int destroy(Data data);

#endif /* stdhead_h */
