//
//  Stack.c
//  shrek
//
//

#include "Stack.h"

typedef struct strNode * Node;

struct strNode{
	Data data;
	Node prior;
};
struct strStack{
	int size;
	Data name;
	Node top;
};

Stack stack_create(Data name){
	Stack newStack = (Stack)malloc(sizeof(struct strStack));
	newStack -> size = 0;
	newStack -> name = name;
	newStack -> top = NULL;
	return newStack;
}

void stack_print(Stack stack){
	Node auxNode = stack->top;
    printf("[");
	while(auxNode){
		data_print(auxNode->data);
		if(auxNode->prior)
			printf(", ");
		auxNode = auxNode->prior;
	}
	printf("]");
	return;
}

Data stack_name(Stack stack){
	return stack ? stack->name : NULL;
}

bool stack_isEmpty(Stack stack){
	return stack && stack->size != 0 ? true : false;
}

int size(Stack stack){
	return stack ? stack->size : 0;
}

Data stack_top(Stack stack){
	return stack ? stack->top->data : NULL;
}

Data stack_pop(Stack stack){
	if(!stack || !stack->name){
		return NULL;
	}
	Data auxData = stack->top->data;
	Node auxNode = stack->top;
	stack->top = stack->top->prior;
	free(auxNode);
	return auxData;
}

void stack_destroy(Stack stack){
    if (!stack) {
        return;
    }
	while(!stack_isEmpty(stack)){
		data_destroy(stack_pop(stack));
	}
	data_destroy(stack->name);
	free(stack);
	return;
}

void stack_push(Stack stack,Data data){
	if(!stack){
		return;
	}
	Node newNode = (Node) malloc (sizeof(struct strNode));
	newNode->data = data;
	if(stack->top){
		newNode->prior = stack->top;
	}
	else{
		newNode->prior = NULL;
	}
	stack->top = newNode;
}
