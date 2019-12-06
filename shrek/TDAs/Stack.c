//
//  Stack.c
//  shrek
//
//

#include "List.h"
#include "Map.h"
#include "Set.h"
#include "Stack.h"
#include "Queue.h"

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
	tokenType type;
	while(auxNode){
		data_print(auxNode->data);
		auxNode = auxNode->prior;
	}
	return;
}

Data stack_name(Stack stack){
	return stack ? stack->name : NULL;
}


bool stack_isEmpty(Stack stack){
	return stack && stack->size != 0 ? true : false;
}

int size(Stack stack){
	return stack ? stack->size : NULL;
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
	tokenType type;
	Data auxData;
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
