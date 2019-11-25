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
		type = auxNode->data->type;
		if(type == INT || type == FLOAT || type == STRING){
			print(auxNode->data);
		}
		else if(type == STACK){
			stack_print(auxNode->data->value);
		}
		else if(type == LIST){
			list_print(auxNode->data->value);
		}
		else if(type == QUEUE){
			queue_print(auxNode->data->value);
		}
		else if(type == SET){
			set_print(auxNode->data->value);
		}
		else if(type == MAP){
			set_print(auxNode->data->value);
		}
		else if(type == FUNCTION){
			block_print(auxNode->data->value);
		}
		auxNode = auxNode->prior;
	}
	return;
}

Data stack_name(Stack stack){
	return stack ? stack->name : NULL;
}

void stack_destroy(Stack stack){
	tokenType type;
	Data auxData;
	while(!stack_isEmpty(stack)){
		type = stack->top->data->type;
		if(type == INT || type == FLOAT || type == STRING){
			auxData = pop(stack);
			free(auxData->type);
			free(auxData->value);
			free(auxData);
		}
		else if(type == STACK){
			stack_destroy(pop(stack));
		}
		else if(type == LIST){
			list_destroy(pop(stack));
		}
		else if(type == QUEUE){
			queue_destroy(pop(stack));
		}
		else if(type == SET){
			set_destroy(pop(stack));
		}
		else if(type == MAP){
			map_destroy(pop(stack));
		}
		else if(type == FUNCTION){
			block_destroy(pop(stack));
		}
	}
	free(stack->name);
	free(stack);
	return;
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
