//
//  stdhead.c
//  shrek
//
/*
    Implementation of functions that will be needed in more than one library
*/

#include "stdhead.h"

Data data_create(tokenType type, Type value){
    Data newData = (Data) malloc(sizeof(struct strData));
    newData -> type = type;
    newData -> value = value;
    return newData;
}

void data_destroy(Data data){//just to destroy ints, floats and strings
    free(data->value);
    free(data);
}

int cmp(Data a, Data b){
    if((a -> type == FLOAT || a -> type == INT) && (b -> type == FLOAT || b -> type == INT)){
        int x, y;
        if(a -> type == INT) {
            x = *((int*)a -> value);
        }else{
            x = *((float*)a -> value);
        }
        if(b -> type == INT) {
            y = *((int*)a -> value);
        }else{
            y = *((float*)a -> value);
        }
        return x - y;
    }else if (a -> type == b -> type && a -> type == STRING){
        
        char * s1 = a -> value, * s2 = b -> value;
        int i;
        for (i= 0; s1[i] != '\0'; i++) {
            if (s1[i] > s2[i]) {
                return 1;
            }else if (s1[i] < s2[i]){
                return -1;
            }
        }
        
        if (s2[i] == '\0') {
            return 0;
        }else{
            return -1;
        }
    }
    return 0;
}

void print(Data data){
    if (data -> type == INT) {
        int n = * ((int *) data -> value);
        printf(" %d",n);
    }else if (data -> type == FLOAT){
        int f = * ((int *) data -> value);
        printf(" %d",f);
    }else if (data -> type == STRING){
        char * s = data -> value;
        printf(" %s", s);
    }
}

