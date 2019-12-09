//
//  Map.c
//  shrek
//
//

#include "Map.h"
#include "Block.h"
#include "Set.h"
#include "List.h"
#include "Stack.h"
#include "Queue.h"


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
    if (!key) {
        return 0;
    }
    
    int h = 0;
    
    tokenType keyType = key -> type;
    
    if (keyType == INT) {
        int n = * ((int *) key -> value);
        return n % cap;
        
    }else if (keyType == FLOAT){
        float n = * ((float *) key -> value);
        return (int) n % cap;
    }
    
    char * stringKey;
    
    switch (keyType) {
        case NAME:
        case STRING:
            stringKey = (void *) key -> value;
            break;
        case STACK:
            return hash(stack_name(key -> value), cap);
        case QUEUE:
            return hash(queue_name(key -> value), cap);
        case LIST:
            return hash(list_name(key -> value), cap);
        case MAP:
            return hash(map_name(key -> value), cap);
        case SET:
            return hash(set_name(key -> value), cap);
        case FUNCTION:
            return hash(block_name(key -> value), cap);
        default:
            return 0;
            break;
    }
    
    int factor = 128;
    
    h = stringKey[0];
    
    for (int i = 1; stringKey[i] != '\0'; i++) {
        h +=  (( (h%cap) * (factor % cap) ) % cap + stringKey[i])%cap;
    }
    
    h %= cap;
    
    return h;
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
}

ListMap listMap_create() {
	return (ListMap) calloc(1, sizeof(struct strListMap));
}

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

//Externals Functions
Map map_create(Data name, int cap){
	if (!cap )
		return NULL;
	Map map = (Map) malloc(sizeof(struct strMap));
	map->cap = cap;
	map->name = name;
	map->table = (ListMap*) calloc(cap, sizeof(ListMap));
	return map;
}

Data map_name(Map map){
	return (!map? NULL:map->name);
}

void map_destroy(Map map){
	if (!map)
		return;
	for (int i = 0; i < map->cap; i++)
		if (map->table[i])
			listMap_destroy(map->table[i]);
	free((void*) map->table);
    data_destroy(map -> name);
	free((void*) map);
	return;
}

int  map_size(Map map){
	return (!map? 0:map->size);
}

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
            data_destroy(n -> value);
			n->value = v;
			return;
		}
		n = n->next;
	}
    
	listMap_add(l, key, v);
	map->size++;
	return;
}

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
}

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
}

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
}

