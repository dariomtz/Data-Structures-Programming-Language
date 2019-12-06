//
//  List.c
//  shrek
//
/*
    Implementation of List struct and its functions
*/

#include "List.h"
#include "Map.h"
#include "Set.h"
#include "Stack.h"
#include "Queue.h"

struct strNode{
	Data data;
	Node prior, next;
};

struct strList{
	Data name;
	Node first,last;
	int size;
}


List list_create(Data name){
	List newList = (List) malloc (sizeof(struct strList));
	newList->name = name;
	newList->first = NULL;
	newList->last = NULL;
	newList->size = 0;
	return newList;
}
void list_print(List list){
	if(!list)
		return;
	Node auxNode = list_begin(list);
	printf("(");
	while(auxNode){
		data_print(auxNode->data);
		if(auxNode->prior)
			printf(",");
		auxNode = auxNode->next;
	}
	printf(")");
}
Data list_name(List list){
	return list ? list->name : NULL;
}
void list_destroy(List list){
	if(!list)
		return;
	Node auxNode;
	Node auxNodetoDestroy;
	auxNode = list->first;

	while(auxNode){
		data_destroy(auxNode->data);
		auxNodetoDestroy = auxNode;
		auxNode = auxNode->next;
		free(auxNodetoDestroy);

	}
	free(list);
}
int list_size(List list){
	return list ? list->size : NULL;

}
void list_add(List list, Data data){
	Node newNode = (Node)malloc(sizeof(struct strNode));
	newNode -> data = data;
	if(list_exists(list_end(list))){
		newNode->prior = list->last;
		list->last->next = newNode;
		newNode -> next = NULL;
		list->last = newNode;
	}
	else{
		newNode->prior = NULL;
		newNode->next = NULL;
		list->first = newNode;
		list->last = newNode;
	}
	return;
}
Data list_get(List list, int pos){
	if(!list)
		return NULL;
	Node aux = list->first;
	int cont;
	while(aux && cont < pos){
		if(aux->next)
			aux = aux->next;
			cont++;
		else
			return NULL;
	}
	return NULL;

}
void list_set(List list, int pos, Data data){
	if(!list)
		return;
	Node aux = list->first;
	int cont;
	while(aux && cont < pos){
		if(aux->next)
			aux = aux->next;
			cont++;
		else
			return;
	}
	data_destroy(aux->data);
	aux->data = data;
	return;
}
void list_remove(List list, int pos){
	if(!list || !list->first){
		return;
	}
	Data auxData;
	int cont;
	while(aux && cont < pos){
		if(aux->next)
			aux = aux->next;
			cont++;
		else
			return;
	}
	if(aux->next){
		aux->next->last = aux->prior;
	}
	if(aux->prior){
		aux->prior = aux->next;
	}
	data_destroy(aux->data);
	free(aux);
	return;

}

Iterator list_begin(List list){
	return list && list->first ? list->first : NULL ;
}
Iterator list_end(List list){
	return list && list->last ? list->last : NULL;
}
bool list_exists(Iterator it){
	return it ? TRUE : FALSE; 
}
Iterator list_next(Iterator it){
	return it && it->next ? TRUE : FALSE ;
}
Iterator list_prior(Iterator it){
	return it && it->prior ? TRUE : FALSE;
}
Data list_data(Iterator it){
	return it && it->data ? TRUE : FALSE;
}

