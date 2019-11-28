//
//  stdhead.c
//  shrek
//
/*
    Implementation of functions that will be needed in more than one library
*/

#include "stdhead.h"
#include "List.h"
#include "Set.h"
#include "Map.h"
#include "Stack.h"
#include "Queue.h"
#include "Block.h"
#include "Sentence.h"

Data data_create(tokenType type, Type value){
    Data newData = (Data) malloc(sizeof(struct strData));
    newData -> dest = true;
    newData -> type = type;
    newData -> value = value;
    return newData;
}

Data data_makeCopy(Data data){
    if (!data || !data -> value) {
        return NULL;
    }
    
    Data copy = (Data) malloc(sizeof(struct strData));
    copy -> dest = true;
    copy -> type = data -> type;
    copy -> value = (char *) malloc(strlen(data -> value));
    strcpy(copy -> value, data -> value);
    
    return copy;
}

Data data_copyResolvedData(Data data){
    if (!data) {
        return NULL;
    }
    
    Data copy = (Data) malloc(sizeof(struct strData));
    copy -> dest = true;
    copy -> type = data -> type;
    
    if (data -> type == INT) {
        int * i = (int *) malloc(sizeof(int));
        *i = *(int *)data -> value;
        copy -> value = i;
    }else if (data -> type == FLOAT){
        float * i = (float *) malloc(sizeof(float));
        *i = *(float *)data -> value;
        copy -> value = i;
    }else if (data -> type == STRING){
        copy -> value = (char *) malloc(strlen(data -> value));
        strcpy(copy -> value, data -> value);
    }else{
        free(copy);
        copy = NULL;
    }
    
    return copy;
}

Data data_makeReference(Data data){
    if (!data) {
        return NULL;
    }
    
    Data reference = (Data) malloc(sizeof(struct strData));
    reference -> dest = false;
    reference -> value = data -> value;
    reference -> type =  data -> type;
    
    return reference;
}

void data_destroy(Data data){
    if (!data) {
        return;
    }
    
    if (data -> dest) {
        if (data -> type == INT || data -> type == FLOAT || data -> type == STRING) {
            free(data->value);
        }
        
        else if (data -> type == MAP)
            map_destroy(data -> value);
        else if (data -> type == SET)
            set_destroy(data -> value);
        else if (data -> type == STACK)
            stack_destroy(data -> value);
        else if (data -> type == QUEUE)
            queue_destroy(data -> value);
        else if (data -> type == LIST)
            list_destroy(data -> value);
        else if (data -> type == FUNCTION)
            block_destroy(data -> value);
        
    }
    
    free(data);
}

int data_cmp(Data a, Data b){
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
        
        return strcmp(s1, s2);
    }
    return 0;
}

void print(Data data){
    if (data -> type == INT) {
        int n = * ((int *) data -> value);
        printf(" %d",n);
    }else if (data -> type == FLOAT){
        float f = * ((float *) data -> value);
        printf(" %f",f);
    }else if (data -> type == STRING){
        char * s = data -> value;
        printf(" %s", s);
    }
}

void data_print(Data data){
    if (!data) {
        return;
    }
    
    if (data -> type == INT || data -> type == FLOAT || data -> type == STRING) {
        print(data->value);
    }
    
    else if (data -> type == MAP)
        map_print(data -> value);
    else if (data -> type == SET)
        set_print(data -> value);
    else if (data -> type == STACK)
        stack_print(data -> value);
    else if (data -> type == QUEUE)
        queue_print(data -> value);
    else if (data -> type == LIST)
        list_print(data -> value);
    else if (data -> type == FUNCTION)
        block_print(data -> value);
}

