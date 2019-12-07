//
//  List.c
//  shrek
//
/*
    Implementation of List struct and its functions
*/

#include "List.h"

typedef struct strNode * Node;

struct strNode{
	Data data;
	Node prior, next;
};

struct strList{
	Data name;
	Node first,last;
	int size;
};


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
    Node auxNode = list -> first;
	printf("(");
	while(auxNode){
		data_print(auxNode->data);
		if(auxNode->next)
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
	return list ? list->size : 0;

}

void list_add(List list, Data data){
	Node newNode = (Node) malloc(sizeof(struct strNode));
	newNode -> data = data;
    newNode -> next = NULL;
    newNode -> prior = list -> last;
    
    if (list -> last) {
        list -> last -> next = newNode;
        list -> last = newNode;
    }
    
    if (!list -> first) {
        list -> first = newNode;
    }
	
    list -> size++;
	return;
}

Data list_get(List list, int pos){
    if(!list || !(0 <= pos || pos < list -> size))
		return NULL;
    
	Node aux = list->first;
	int cont = 0;
	while(cont < pos){
        aux = aux->next;
        cont++;
	}
	return aux -> data;
}

void list_set(List list, int pos, Data data){
	if(!list || !(0 <= pos || pos < list -> size))
		return;
    
    Node aux = list->first;
    int cont = 0;
    while(cont < pos){
        aux = aux->next;
        cont++;
    }
    
	data_destroy(aux->data);
	aux->data = data;
	return;
}

void list_remove(List list, int pos){
    if(!list || !(0 <= pos || pos < list -> size)){
		return;
	}
    
	Node aux = list -> first;
	int cont = 0;
    
    while(cont < pos){
        aux = aux -> next;
        cont++;
	}

    if (aux -> next) {
        aux -> next -> prior = aux -> prior;
    }else{
        list -> last = aux -> prior;
    }
    
    if (aux -> prior) {
        aux -> prior -> next = aux -> next;
    }else{
        list -> first = aux -> next;
    }
    
    list -> size--;
    
	free(aux);
	return;

}

Iterator list_begin(List list){
	return list ? list->first : NULL ;
}

Iterator list_end(List list){
	return list ? list->last : NULL;
}

bool list_exists(Iterator it){
	return it ? true : false;
}

Iterator list_next(Iterator it){
	return it ? it -> next : NULL;
}

Iterator list_prior(Iterator it){
	return it ? it -> prior : NULL;
}

Data list_data(Iterator it){
	return it ? it -> data : NULL;
}

