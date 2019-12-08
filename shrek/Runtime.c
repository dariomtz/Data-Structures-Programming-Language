
//
//  Runtime.c
//  shrek
//
//  Created by DarÃƒÂ­o MartÃƒÂ­nez on 11/22/19.
//  Copyright Ã‚Â© 2019 DarÃƒÂ­o MartÃƒÂ­nez. All rights reserved.
//

#include "Runtime.h"
#include "Block.h"
#include "Sentence.h"

#include "List.h"
#include "Queue.h"
#include "Set.h"
#include "Stack.h"
#include "Map.h"


typedef enum{NO_VALID = -1, M_ADD, M_GET, M_SET, M_REMOVE, M_PRINT, M_SIZE, M_PUT, M_ISEMPTY,
	M_OFFER, M_PEEK, M_POLL, M_TOP, M_POP, M_PUSH, M_CONTAINS}MethodType;

Data Runtime_evaluateMethod(Data container, Sentence sentence){
	Token token = container->type;
	List arguments = (sentence_getValue(sentence_getRightSubsentece(sentence)))->value;
	char * name = (char *)(sentence_getValue(sentence))->value;
	Data pos, data, answer;

	switch(token){
	case LIST:
		//PRINT
		if (!(strcmp(name, "print"))){
			if(list_size(arguments)){
				printf("RUNTIME ERROR: too many arguments for a print method\n");
				return data_create(ERROR, NULL);
			}
			list_print((List)container->value);
			return NULL;
		}
		//SIZE
		if (!(strcmp(name, "size"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a size method\n");
				return data_create(ERROR, NULL);
			}
			return data_create(INT, Runtime_createInt(list_size((List)container->value)));
		}
		//REMOVE
		if (!(strcmp(name, "remove"))){

			if(list_size(arguments)<1){
				printf("RUNTIME ERROR: too few arguments for a remove method\n");
				return data_create(ERROR, NULL);
			}
			if(list_size(arguments)>1){
				printf("RUNTIME ERROR: too many arguments for a remove method\n");
				return data_create(ERROR, NULL);
			}

			pos = resolve_sentence(list_get(arguments, 0));
			if(pos->type == ERROR)
				return pos;
			if(pos->type != INT){
				printf("RUNTIME ERROR: remove method only accept INT argument\n");
				data_destroy(pos);
				return data_create(ERROR, NULL);
			}
			list_remove((List)container->value,*((int*)pos->value));
			data_destroy(pos);
			return NULL;
		}
		//SET
		if (!(strcmp(name, "set"))){
			if(list_size(arguments)<2){
				printf("RUNTIME ERROR: too few arguments for a set method\n");
				return data_create(ERROR, NULL);
			}
			if(list_size(arguments)>2){
				printf("RUNTIME ERROR: too many arguments for a set method\n");
				return data_create(ERROR, NULL);
			}

			pos = resolve_sentence(list_get(arguments, 0));
			if (pos->type == ERROR)
				return pos;
			if (pos->type != INT) {
				printf("RUNTIME ERROR: set method only accept INT argument in first position\n");
				data_destroy(pos);
				return data_create(ERROR, NULL);
			}

			data = resolve_sentence(list_get(arguments, 1));
			if (data->type == ERROR)
				return data;

			list_set((List)container->value,*((int*)pos->value),data);
			data_destroy(pos);
			return NULL;
		}

		//ADD
		if (!(strcmp(name, "add"))) {
			if (list_size(arguments) < 1) {
				printf("RUNTIME ERROR: too few arguments for a add method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 1) {
				printf("RUNTIME ERROR: too many arguments for a add method\n");
				return data_create(ERROR, NULL);
			}

			data = resolve_sentence(list_get(arguments, 1));
			if (data->type == ERROR)
				return data;

			list_add((List)container->value,data);
			return NULL;
		}

		//GET
		if (!(strcmp(name, "get"))){
			if(list_size(arguments)<1){
				printf("RUNTIME ERROR: too few arguments for a get method\n");
				return data_create(ERROR, NULL);
			}
			if(list_size(arguments)>1){
				printf("RUNTIME ERROR: too many arguments for a get method\n");
				return data_create(ERROR, NULL);
			}

			pos = resolve_sentence(list_get(arguments, 0));
			if(pos->type == ERROR)
				return pos;
			if(pos->type != INT){
				printf("RUNTIME ERROR: get method only accept INT argument\n");
				data_destroy(pos);
				return data_create(ERROR, NULL);
			}
			data = list_get((List)container->value,*((int*)pos->value));
			data_destroy(pos);
			data = data_copyResolvedData(data);
			return data;
		}
		break;
	case MAP:
		//PRINT
		if (!(strcmp(name, "print"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a print method\n");
				return data_create(ERROR, NULL);
			}
			map_print((Map)container->value);
			return NULL;
		}
		//SIZE
		if (!(strcmp(name, "size"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a size method\n");
				return data_create(ERROR, NULL);
			}
			return data_create(INT, Runtime_createInt(map_size((Map)container->value)));
		}
		//REMOVE
		if (!(strcmp(name, "remove"))) {
			if (list_size(arguments) < 1) {
				printf("RUNTIME ERROR: too few arguments for a remove method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 1) {
				printf("RUNTIME ERROR: too many arguments for a remove method\n");
				return data_create(ERROR, NULL);
			}
			data = resolve_sentence(list_get(arguments, 0));
			if (data->type == ERROR)
				return data;
			map_remove((Map)container->value, data);
			data_destroy(data);
			return NULL;
		}
		//GET
		if (!(strcmp(name, "get"))){
			if (list_size(arguments) < 1) {
				printf("RUNTIME ERROR: too few arguments for a remove method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 1) {
				printf("RUNTIME ERROR: too many arguments for a remove method\n");
				return data_create(ERROR, NULL);
			}
			data = resolve_sentence(list_get(arguments, 0));
			if (data->type == ERROR)
				return data;
			answer = map_get((Map)container->value, data);
			data_destroy(data);
			answer = data_copyResolvedData;
			return answer;
		}
		//PUT o el que lo lea
		if (!(strcmp(name, "put"))) {
			if (list_size(arguments) < 2) {
				printf("RUNTIME ERROR: too few arguments for a put method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 2) {
				printf("RUNTIME ERROR: too many arguments for a put method\n");
				return data_create(ERROR, NULL);
			}
			pos = resolve_sentence(list_get(arguments, 0));
			if (data->type == ERROR)
				return pos;

			data = resolve_sentence(list_get(arguments, 1));
			if (data->type == ERROR)
				return data;
			map_put((Map)container->value,pos,data);
			return NULL;
		}
		break;

	case QUEUE:

		//Print
		if (!(strcmp(name, "print"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a print method\n");
				return data_create(ERROR, NULL);
			}
			queue_print((Queue)container->value);
			return NULL;
		}

		//SIZE
		if (!(strcmp(name, "size"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a size method\n");
				return data_create(ERROR, NULL);
			}
			return data_create(INT, Runtime_createInt(queue_size((Queue)container->value)));
		}

		//IS EMPTY
		if (!(strcmp(name, "isEmpty"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a isEmpty method\n");
				return data_create(ERROR, NULL);
			}
			return data_create(INT, Runtime_createInt(queue_isEmpty((Queue)container->value)));
		}

		//OFFER
		if (!(strcmp(name, "offer"))){
			if (list_size(arguments) < 1) {
				printf("RUNTIME ERROR: too few arguments for a offer method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 1) {
				printf("RUNTIME ERROR: too many arguments for a offer method\n");
				return data_create(ERROR, NULL);
			}
			data = resolve_sentence(list_get(arguments, 0));
			if (data->type == ERROR)
				return data;
			queue_offer(((Queue)container->value),data);
			return NULL;
		}

		//PEEK
		if (!(strcmp(name, "peek"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a peek method\n");
				return data_create(ERROR, NULL);
			}
			data = queue_peek(((Queue)container->value));
			return data_copyResolvedData(data);
		}

		//POLL
		if (!(strcmp(name, "poll"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a poll method\n");
				return data_create(ERROR, NULL);
			}
			return queue_poll(((Queue)container->value));
		}
		break;


	case SET:
		//PRINT
		if (!(strcmp(name, "print"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a print method\n");
				return data_create(ERROR, NULL);
			}
			set_print((Set)container->value);
			return NULL;
		}

		//SIZE
		if (!(strcmp(name, "size"))) {
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a size method\n");
				return data_create(ERROR, NULL);
			}
			return data_create(INT,Runtime_createInt(set_size((Set) container->value)));
		}

		//REMOVE
		if (!(strcmp(name, "remove"))){
			if (list_size(arguments) < 1) {
				printf("RUNTIME ERROR: too few arguments for a remove method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 1) {
				printf("RUNTIME ERROR: too many arguments for a remove method\n");
				return data_create(ERROR, NULL);
			}
			data = resolve_sentence(list_get(arguments, 0));
			if (data->type == ERROR)
				return data;
			set_remove((Set)container->value, data);
			data_destroy(data);
			return NULL;
		}

		//ADD
		if (!(strcmp(name, "add"))){
			if (list_size(arguments) < 1) {
				printf("RUNTIME ERROR: too few arguments for a add method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 1) {
				printf("RUNTIME ERROR: too many arguments for a add method\n");
				return data_create(ERROR, NULL);
			}
			data = resolve_sentence(list_get(arguments, 0));
			if (data->type == ERROR)
				return data;
			return data_create(INT,Runtime_createInt(set_add((Set)container->value, data)));
		}

		//CONTAINS
		if (!(strcmp(name, "contains"))){
			if (list_size(arguments) < 1) {
				printf("RUNTIME ERROR: too few arguments for a contains method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 1) {
				printf("RUNTIME ERROR: too many arguments for a contains method\n");
				return data_create(ERROR, NULL);
			}
			data = resolve_sentence(list_get(arguments, 0));
			if (data->type == ERROR)
				return data;

			answer = data_create(INT,Runtime_createInt(set_contains((Set)container->value, data)));
			data_destroy(data);
			return answer;
		}
		break;
	case STACK:

		//PRINT
		if (!(strcmp(name, "print"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a print method\n");
				return data_create(ERROR, NULL);
			}
			stack_print((Stack) container->value);
			return NULL;
		}

		//SIZE
		if (!(strcmp(name, "size"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a size method\n");
				return data_create(ERROR, NULL);
			}
			return data_create(INT,Runtime_createInt(stack_size((Stack) container->value)));
		}

		//isEmpty
		if (!(strcmp(name, "isEmpty"))) {
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a isEmpty method\n");
				return data_create(ERROR, NULL);
			}
			return data_create(INT,Runtime_createInt(stack_isEmpty((Stack) container->value)));
		}

		//TOP
		if (!(strcmp(name, "top"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a top method\n");
				return data_create(ERROR, NULL);
			}
			return data_copyResolvedData(stack_top((Stack) container->value));
		}

		//POP
		if (!(strcmp(name, "pop"))){
			if (list_size(arguments)) {
				printf("RUNTIME ERROR: too many arguments for a pop method\n");
				return data_create(ERROR, NULL);
			}
			return stack_pop((Stack) container->value);
		}

		//PUSH
		if (!(strcmp(name, "push"))){
			if (list_size(arguments) < 1) {
				printf("RUNTIME ERROR: too few arguments for a push method\n");
				return data_create(ERROR, NULL);
			}
			if (list_size(arguments) > 1) {
				printf("RUNTIME ERROR: too many arguments for a push method\n");
				return data_create(ERROR, NULL);
			}
			data = resolve_sentence(list_get(arguments, 0));
			if (data->type == ERROR)
				return data;
			stack_push((Stack) container->value,data);
			return NULL;
		}
		break;
	}
	printf("RUNTIME ERROR: '%s' is not a valid method\n",name);
	return data_create(ERROR, NULL);
}

Type Runtime_createInt(int num){
	int * p_num = malloc(sizeof(int));
	*p_num = num;
	return (Type)p_num;
 }

Type Runtime_createfloat(float num){
	float * p_num = malloc(sizeof(float));
	*p_num = num;
	return (Type)p_num;
 }

Data resolve_data(Data data) {
	switch(data->type)
	{
		case INT:
		return data_create(INT, Runtime_createInt(atoi(data->value)));
		case FLOAT:
		return data_create(FLOAT, Runtime_createfloat(atof(data->value)));
		case STRING:
		return data_makeCopy(data);
	}
}

Data resolve_sentence(Sentence sentence, Map map){
	Data midle_data, left_data, right_data;
	Sentence left_sentence, right_sentence;
	midle_data = sentence_getValue(sentence);
	Data answer;
	switch(midle_data->type){
	case FALSE:
		return data_create(INT, Runtime_createInt(0));
	case TRUE:
		return data_create(INT, Runtime_createInt(1));
	case NAME:
		answer =  map_get(map,midle_data);
		if(!answer){
			printf("RUNTIME ERROR: the %s name is not defined",midle_data->value);
			return data_create(ERROR, NULL);
		}
		return data_copyResolvedData(answer);
	case INT:
		return resolve_data(midle_data);
	case FLOAT:
		return resolve_data(midle_data);
	case STRING:
		return resolve_data(midle_data);
	case USE_METHOD:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence, map);
		if(left_data->type == ERROR)
			return left_data;
		if(left_data->type < LIST || left_data>MAP){
			data_destroy(left_data);
			left_data = sentence_getValue(left_sentence);
			printf("RUNTIME ERROR: the %s is not a abstract data type ",left_data->value);
			return data_create(ERROR, NULL);
		}
		right_sentence = sentence_getRightSubsentece(sentence);
		answer = Runtime_evaluateMethod(left_data,right_sentence);
		data_destroy(left_data);
		return answer;
	case LIST:
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = sentence_getValue(right_sentence);
		if (map_get(map, right_data)) {printf("RUNTIME ERROR: double declaration of '%s'\n",right_data->value);
			return data_create(ERROR, NULL);
		}
		map_put(map, right_data->value,data_create(LIST, list_create(data_copy(right_data->value))));
		return NULL;
	case STACK:
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = sentence_getValue(right_sentence);
		if (map_get(map, right_data)) {printf("RUNTIME ERROR: double declaration of '%s'\n",right_data->value);
			return data_create(ERROR, NULL);
		}
		map_put(map, right_data->value,data_create(STACK, stack_create(data_copy(right_data->value))));
		return NULL;
	case QUEUE:
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = sentence_getValue(right_sentence);
		if (map_get(map, right_data)) {printf("RUNTIME ERROR: double declaration of '%s'\n",right_data->value);
			return data_create(ERROR, NULL);
		}
		map_put(map, right_data->value,data_create(QUEUE, queue_create(data_copy(right_data->value))));
		return NULL;
	case SET:
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = sentence_getValue(right_sentence);
		if (map_get(map, right_data)) {printf("RUNTIME ERROR: double declaration of '%s'\n",right_data->value);
			return data_create(ERROR, NULL);
		}
		map_put(map, right_data->value,data_create(SET, set_create(data_copy(right_data->value))));
		return NULL;
	case MAP:
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = sentence_getValue(right_sentence);
		if (map_get(map, right_data)) {
			printf("RUNTIME ERROR: double declaration of '%s'\n",right_data->value);
			return data_create(ERROR, NULL);
		}
		map_put(map, right_data->value,data_create(MAP, map_create(data_copy(right_data->value))));
		return NULL;
	case PLUS1:
		left_sentence = sentence_getLeftSubsentece(sentence);

		if(left_sentence){
			left_data = resolve_sentence(left_sentence, map);
			if(left_data->type == ERROR)
				return left_data;
			if(left_data->type != INT || left_data->type != FLOAT){
				data_destroy(left_data);
				printf("RUNTIME ERROR: invalid operation\n");
				return data_create(ERROR, NULL);
			}
			answer = data_copyResolvedData(left_data);
			if(left_data->type == INT)
				(*((int*)left_data->value))++;
			else
				(*((float*)left_data->value))++;
			map_put(map,sentence_getValue(left_sentence),left_data);
			return answer;
		}
		else{
			right_sentence = sentence_getRightSubsentence(sentence);
			right_data = resolve_sentence(right_sentence,map);
			if(right_data->type == ERROR)
				return right_data;
			if(right_data->type != INT || right_data->type != FLOAT){
				data_destroy(right_data);
				printf("RUNTIME ERROR: invalid operation\n");
				return data_create(ERROR, NULL);
			}
			if(left_data->type == INT)
				(*((int*)left_data->value))++;
			else
				(*((float*)left_data->value))++;
			map_put(map,sentence_getValue(left_sentence),left_data);
			return data_copyResolvedData(left_data);
		}

	}
};

void RunTime(Parser parser){
	error error = parser_getErrorStatus(parser);
	if(error.type){
		if(error.type<=99)
			printf("LEXER ERROR: ");
		else
			printf("PARSER ERROR: ");
		printf("%s",error.message);
		return;
	}
	Map map_functions = parser_getMemoryMap(parser);
	Data main_data =(Data)malloc(sizeof(struct strData));
	main_data->type = NAME;
	
	//Here, the name of the main can change in the future
	strcpy(main_data->value, "main");
	//-------------------------------------------------------------------------------------//
	main_data = map_get(map_functions, main_data);
	Block block = (Block)main_data->type;
	int bl_size = block_size(block);
	Sentence sentence;
	Data result_data;
	for(int i = 0; i<bl_size; i++){
		sentence = block_getSentence(block,i);
		result_data = resolve_sentence(sentence, map_functions);
		if(result_data->type == ERROR){
			data_destroy(result_data);
			return;
		}
	}
};
