//
//  Map.c
//  shrek
//
//

#include "Map.h"

typedef struct strNode * Node;
struct strNode{
	Data key;
	Data value;
	Node next;
};

typedef struct strListMap * ListMap;
struct strListMap{
	int size;
	Node first;
	Node last;

};

struct strMap{
	int size;
	int cap;
	Data name;
	ListMap* table;

};

//internal functions

int hash(Data key,int cap){
    int n = 0;
    
    //hashing for different data types
    
    //for ints is module cap
    
    //for floats is module cap
    
    //for strings is sum all n where n = 256^i, where i is the position of the char from left to right,
    
    //for lists, maps, sets, stacks, and queues is the hashing of their name;
    
    return n;
}

void listMap_destroy(ListMap l) {
	Node n = l->first;
	Node next;
	while (n) {
		next = n->next;
		data_destroy(n->key);
		data_destroy(n->value);
		free((void*) n);
		n = next;
	}
	free((void *) l);
};

ListMap listMap_create() {
	return (ListMap) calloc(1, sizeof(struct strListMap));
};

void listMap_add(ListMap l, Data k, Data v) {
	if (!l || !k || !v)
		return;
	Node n = (Node) calloc(1, sizeof(struct strNode));
	n->key = k;
	n->value = v;
	if (!l->size) {
		l->first = n;
		l->last = n;
	} else {
		l->last->next = n;
		l->last = n;
	}
	l->size++;
}
;
Data listMap_remove(ListMap l, Data k) {
	if (!l || !k)
		return NULL;
	Node n = l->first;
	Node pn = NULL;
	Data value;
	while (n) {
		if (!data_cmp(k, n->key)) {
			value = n->value;
			if (n == l->first)
				l->first = n->next;
			else
				pn->next = n->next;
			if (n == l->last)
				l->last = pn;
			free(n);
			l->size--;
			return value;
		}
		pn = n;
		n = n->next;
	}
	return NULL;
}
;


//Externals Functions
Map map_create(Data name, int cap){
	if (!name || !cap )
		return NULL;
	Map map = (Map) malloc(sizeof(struct strMap));
	map->cap = cap;
	map->name = name;
	map->table = (ListMap*) calloc(cap, sizeof(ListMap));
	return map;
};
Data map_name(Map map){
	return (!map? NULL:map->name);
};
void map_destroy(Map map){
	if (!map)
		return;
	for (int i = 0; i < map->cap; i++)
		if (map->table[i])
			listMap_destroy(map->table[i]);
	free((void*) map->table);
	free((void*) map);
	return;
};
int  map_size(Map map){
	return (!map? 0:map->size);
};
void map_put(Map map, Data key, Data v){
	if (!map || !key || !v)
		return;
	int i = hash(key, map->cap);
	ListMap l = map->table[i];
	if (!l) {
		l = map->table[i] = listMap_create();
		listMap_add(l, key, v);
		map->size++;
		return;
	}
	Node n = l->first;
	while (n) {
		if (!data_cmp(n->key, key)) {
			n->value = v;
			return;
		}
		n = n->next;
	}
	listMap_add(l, key, v);
	map->size++;
	return;
};
Data map_get(Map map, Data key){
	if(!map||!key)
		return NULL;
	int i = hash(key, map->cap);
	ListMap l = map->table[i];
	if (!l)
		return NULL;
	Node n = l->first;
	while (n){
		if (!data_cmp(n->key, key))
			return (n->value);
		n = n->next;
	}
	return NULL;
};
void map_remove(Map map, Data key){
	if (!map || !key)
		return;
	int i = hash(key, map->cap);
	ListMap l = map->table[i];
	if (!l)
		return;
	Data value = listMap_remove(l, key);
	if (value)
		map->size--;
	data_destroy(value);
	return;
};
void map_print(Map map){
	if (!map)
		return;
	ListMap l;
	Node n;
	printf("[");
	for (int i = 0; i < map->cap; i++) {
		l = map->table[i];
		if (l && l->size) {
			n = l->first;
			while (n) {
				printf("<");
				data_print(n->key);
				printf(", ");
				data_print(n->value);
				printf(">");
				n = n->next;
				if (n)
					printf(", ");
			}
		}
	}
	printf("]");
};

