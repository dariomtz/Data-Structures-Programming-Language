//
//  Set.c
//  shrek
//
//

#include "Set.h"

typedef struct strNode * node;

struct strNode{
	node left;
	node right;
	Data data;
};

struct strSet{
	int size;
	Data name;
	node sets[3]; //int float strings
};

//Internals Functions
void print(node n) {
	if (!n)
		return;
	print(n->left);
	data_print(n->data);
	if(n->right != NULL)
		printf(", ");
	print(n->right);
};

void destroy(node n){
	if(!n)
		return;
	destroy(n->left);
	destroy(n->right);
	data_destroy(n->data);
	free((void*)n);
	return;
}

void addLink(node p, node c, bool isLeftChild) {
	if (!p)
		return;
	if (isLeftChild == true)
		p->left = c;
	else
		p->right = c;

	return;
}

node almostBigger(node n){
	node p = NULL;
	while(n->right){
		p = n;
		n = n->right;
	}
	return p;
}

void removeBranch(Set s, node p, bool isLeftChild, int a){
	if(!s || !s->sets[a])
		return;
	node n;
	n = (!p)?s->sets[a]:(isLeftChild)?p->left:p->right;

	if(!n->left && !n->right){
		if(!p) // caso 1 con raiz
			s->sets[a] = NULL;
		else // caso 1
			addLink(p,NULL,isLeftChild);
		data_destroy(n->data);
		free((void*)n);
		s->size--;
		return;
	}
	if (!n->left || !n->right) {
		if (!p) {
			if (n->left) // caso 2a raiz
				s->sets[a] = n->left;
			else // caso 2b raiz
				s->sets[a] = n->right;
		}
		else {
			if (n->left) //caso 2a
				addLink(p, n->left, isLeftChild);
			else // caso 2b
				addLink(p, n->right, isLeftChild);
		}
		data_destroy(n->data);
		free((void*) n);
		s->size--;
		return;
	}

	node pm = almostBigger(n->left);
	Type t;
	if (pm) {// caso 3b
		t = (pm->right)->data;
		removeBranch(s, pm, false, a);
		n->data = t;
	}
	else {// caso 3a
		t = (n->left)->data;
		removeBranch(s,n,true, a);
		n->data = t;
	}
	return;
};

//External functions

Set set_create(Data name){
	Set s = (Set)calloc(1,sizeof(struct strSet));
	s->name = name;
	return s;
};
Data set_name(Set set){
	return (!set? NULL:set->name);
};
void set_destroy(Set set){
	if (!set)
		return;
	for(int i = 0; i<3;i++)
		destroy(set->sets[i]);
	free((void*) set);
	return;
}

int set_size(Set set){
	return (!set? -1:set->size);
}

bool set_add(Set set, Data data){
	if (!set || !data)
		return false;
	node nt = (node) calloc(1, sizeof(struct strNode));
	nt->data = data;
	int a =(data->type == INT || data->type == FLOAT)? 0:(data->type == STRING)?1:2;
	if (!set->sets[a]) {
		set->sets[a] = nt;
		set->size++;
		return true;
	}
	node n;
	int r;
	n = set->sets[a];
	while (n) {
		r = data_cmp(data, n->data);
		if (r == 0) {
			free((void*) nt);
			return false;
		}
		if (r > 0) {
			if (n->right == NULL) {
				n->right = nt;
				set->size++;
				return true;
			} else {
				n = n->right;
			}
		} else {
			if (n->left == NULL) {
				n->left = nt;
				set->size++;
				return true;
			} else {
				n = n->left;
			}
		}
	}
	free((void*) nt);
	return false;
}

bool set_contains(Set set, Data data){
	if (!set || !data)
		return false;
	int a =(data->type == INT || data->type == FLOAT)? 0:(data->type == STRING)?1:2;
	node n = set->sets[a];
	int r;
	while (n) {
		r = data_cmp(data, n->data);
		if (r == 0)
			return true;
		if (r > 0)
			n = n->right;
		else
			n = n->left;
	}
	return false;
};

void set_remove(Set set, Data data){
	if (!set || !data)
		return;
	int a =(data->type == INT || data->type == FLOAT)? 0:(data->type == STRING)?1:2;
	node n = set->sets[a];
	node p = NULL;
	int r;
	bool isLeftChild;
	while (n) {
		r = data_cmp(data, n->data);
		if (r == 0) {
			break;
		}
		if (r > 0) {
			p = n;
			n = n->right;
			isLeftChild = false;
		} else {
			p = n;
			n = n->left;
			isLeftChild = true;
		}
	}
	if (!n)
		return;
	removeBranch(set, p, isLeftChild, a);
	return;
};

void set_print(Set set) {
	if (!set)
		return;
	printf("{");
	for (int i = 0; i < 3; i++)
		print(set->sets[i]);
	printf("}");
	return;
}






