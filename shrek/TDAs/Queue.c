//
//  Queue.c
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
	Node next;
};

struct strQueue{
	Data name;
	Node first,last;
	int size;
};

Queue queue_create(Data name){
	Queue newQueue = (Queue)malloc(sizeof(struct strQueue));
	newQueue->first = NULL;
	newQueue->last = NULL;
	newQueue->name = name;
	newQueue->size = 0;
	return newQueue;
}

void queue_print(Queue queue){
	Node auxNode = queue->first;
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
		auxNode = auxNode->next;
	}
	return;
}


Data queue_name(Queue queue){
	return queue ? queue->name : NULL;
}

void queue_destroy(Queue queue){
	tokenType type;
	Data auxData;
	while(!queue_isEmpty(queue)){
		type = queue->first->data->type;
		if(type == INT || type == FLOAT || type == STRING){
			auxData = queue_poll(queue);
			free(auxData->type);
			free(auxData->value);
			free(auxData);
		}
		else if(type == STACK){
			stack_destroy(queue_poll(queue));
		}
		else if(type == LIST){
			list_destroy(queue_poll(queue));
		}
		else if(type == QUEUE){
			queue_destroy(queue_poll(queue));
		}
		else if(type == SET){
			set_destroy(queue_poll(queue));
		}
		else if(type == MAP){
			map_destroy(queue_poll(queue));
		}
		else if(type == FUNCTION){
			block_destroy(queue_poll(queue));
		}
	}
	free(queue->name);
	free(queue);
	return;

}

int queue_size(Queue queue){
	return queue ? queue->size : NULL;
}

bool queue_isEmpty(Queue queue){
	return queue && queue->size != 0 ? true : false;
}

void queue_offer(Queue queue, Data data){
	if(!queue){
		return;
	}
	Node node = (Node)malloc(sizeof(struct strNode));
	node->data = data;
	node->next = NULL;
	if(queue_isEmpty(queue)){
		queue->first = node;
	}
	else{
		queue->last->next = node;
	}
	queue->last = node;
	queue->size++;
}
Data queue_peek(Queue queue){
	if(!queue || queue->size == 0){
		return NULL;
	}
	return queue->first->data;
}
Data queue_poll(Queue queue){
	if(!queue || queue->size ==0){
		return NULL;
	}
	Node auxNode = queue->first;
	Data auxData = queue->first->data;
	queue->first = queue->first->next;
	queue->size --;
	if(queue->size == 0){
		queue->last = NULL;
	}
	free(auxNode);
	return auxData;
}

