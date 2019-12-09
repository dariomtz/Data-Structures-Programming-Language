
//
//  Runtime.c
//  shrek
//
//

#include "Runtime.h"
#include "Block.h"
#include "Sentence.h"

#include "List.h"
#include "Queue.h"
#include "Set.h"
#include "Stack.h"
#include "Map.h"

Data resolve_sentence(Sentence sentence, Map map);

typedef enum{NO_VALID = -1, M_ADD, M_GET, M_SET, M_REMOVE, M_PRINT, M_SIZE, M_PUT, M_ISEMPTY,
    M_OFFER, M_PEEK, M_POLL, M_TOP, M_POP, M_PUSH, M_CONTAINS}MethodType;

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
Data print_func(List arguments,Map memory){
	Iterator aux = list_begin(arguments);
	Data p;
	while(aux){
		p = resolve_sentence(list_data(aux)->value,memory);
		if(p->type == ERROR)
			return p;
		data_print(p);
		printf(" ");
		data_destroy(p);
		aux = list_next(aux);
	}
	return NULL;
}

Type Runtime_sumStrings(char * left_word, char * right_word){
	int i = 0;
	int j = 0;
	while(left_word[i] != '\0')
		i++;
	while(right_word[j] != '\0')
		j++;
	char * new_word = (char *)malloc((i+j+1)*sizeof(char));
	i = 0;
	j = 0;
	while(left_word[i] != '\0'){
		new_word[i] = left_word[i];
		i++;
	}
	while(right_word[j] != '\0'){
		new_word[i+j] = right_word[j];
		j++;
	}
	new_word[i+j+1]='\0';
	return new_word;
}

Data input_func(){
	char* string = (char*)malloc(100);
	scanf(" %[^\n]s",string);
	char* trueString = (char*)malloc(strlen(string));
	strcpy(trueString, string);
	free(string);
	Data s = data_create(STRING,trueString);
	return s;
}

Data int_cast(Sentence sentence,Map memory){
	Data r = resolve_sentence(sentence,memory);
	if( r->type == INT){
		return r;
	}

	int* i = (int*)malloc(sizeof(int));
	if(r->type == STRING){
		*i = atoi(r->value);
	}
	else if(r->type == FLOAT){
		*i = (int)*(float*)r->value;
	}
	else{
		free(i);
		printf("RUNTIME ERROR: invalid casting for int\n");
		return data_create(ERROR,NULL);
	}
	data_destroy(r);
	Data dataAux = data_create(INT,i);
	return dataAux;
}

Data float_cast(Sentence sentence,Map memory){
	Data r = resolve_sentence(sentence,memory);
	if(r->type == FLOAT){
		return r;
	}
	float* i = (float*)malloc(sizeof(float));
	if(r->type == INT)
		*i = (float)*(int*)r->value;
	else if(r->type == STRING){
		*i = atof(r->value);
	}
	else{
		free(i);
		printf("RUNTIME ERROR: invaid casting for float\n");
		return data_create(ERROR,NULL);
	}
	data_destroy(r);
	Data dataAux = data_create(FLOAT,i);
	return dataAux;
}

int int_len(int i){
	int count = 0;
	while(i/=10){
		count++;
	}
	return count;
}

int float_len(float i){
	int count = 0;
	while((int)i!=i){
		i *= 10;
	}
	int i2 = (int)i;
	while(i2/=10){
		count++;
	}
	return count;
}

Data string_cast(Sentence sentence,Map memory){
	Data r = resolve_sentence(sentence,memory);
	if(r->type == STRING){
		return r;
	}
	char* i;
	if(r->type == INT){
		i = (char*)malloc(sizeof(char)*int_len(*(int*)r->value)+1);
		gcvt(*(int*)r->value,float_len(*(int*)r->value)+1,i	);
	}
	else if(r->type == FLOAT){
		i = (char*)malloc(sizeof(char)*float_len(*(float*)r->value)+1);
		gcvt(*(float*)r->value,float_len(*(float*)r->value)+1,i	);
	}
	else{
		printf("RUNTIME ERROR: invalid casting for string\n");
		return data_create(ERROR,NULL);

	}
	data_destroy(r);
	Data dataAux = data_create(STRING,i);
	return dataAux;
}

Data Runtime_evaluateMethod(Data container, Sentence sentence, Map memory){
    tokenType token = container -> type;
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

            pos = resolve_sentence(list_get(arguments, 0) -> value, memory);
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

            pos = resolve_sentence(list_get(arguments, 0) -> value, memory);
            if (pos->type == ERROR)
                return pos;
            if (pos->type != INT) {
                printf("RUNTIME ERROR: set method only accept INT argument in first position\n");
                data_destroy(pos);
                return data_create(ERROR, NULL);
            }

            data = resolve_sentence(list_get(arguments, 1) -> value, memory);
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

            data = resolve_sentence(list_get(arguments, 0) -> value, memory);
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

            pos = resolve_sentence(list_get(arguments, 0) -> value, memory);
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
            data = resolve_sentence(list_get(arguments, 0) -> value, memory);
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
            data = resolve_sentence(list_get(arguments, 0) -> value, memory);
            if (data->type == ERROR)
                return data;
            answer = map_get((Map)container->value, data);
            data_destroy(data);
            answer = data_copyResolvedData(answer);
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
            pos = resolve_sentence(list_get(arguments, 0) -> value, memory);
            if (pos->type == ERROR)
                return pos;

            data = resolve_sentence(list_get(arguments, 1) -> value, memory);
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
            data = resolve_sentence(list_get(arguments, 0) -> value, memory);
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
            data = resolve_sentence(list_get(arguments, 0) -> value, memory);
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
            data = resolve_sentence(list_get(arguments, 0) -> value, memory);
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
            data = resolve_sentence(list_get(arguments, 0) -> value, memory);
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
            data = resolve_sentence(list_get(arguments, 0) -> value, memory);
            if (data->type == ERROR)
                return data;
            stack_push((Stack) container->value,data);
            return NULL;
        }
        break;
    default:
        printf("RUNTIME ERROR: '%s' is not a valid method\n",name);
        return data_create(ERROR, NULL);
            break;
    }
    printf("RUNTIME ERROR: '%s' is not a valid method\n",name);
    return data_create(ERROR, NULL);
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
        default:
            return data_create(ERROR, NULL);
            
    }
}

Data resolve_block(Map memory, Block b){
    if (!b) {
        return NULL;
    }
    int bl_size = block_size(b);
    Sentence sentence;
    Data result_data;
    for(int i = 0; i<bl_size; i++){
        sentence = block_getSentence(b,i);
        result_data = resolve_sentence(sentence, memory);
        if (result_data) {
            if (result_data -> type == BREAK || result_data -> type == ERROR || result_data -> type == CONTINUE) {
                return result_data;

            }else{
                data_destroy(result_data);
            }
        }
    }
    return NULL;
}

Data resolve_sentence(Sentence sentence, Map map){
    Data midle_data, left_data, right_data;
    Sentence left_sentence, right_sentence;
    midle_data = sentence_getValue(sentence);
	if (!midle_data) {
		return NULL;
	}
    Data answer;
    switch(midle_data->type){
    case FALSE:
        return data_create(INT, Runtime_createInt(0));
    case TRUE:
        return data_create(INT, Runtime_createInt(1));
    case NAME:

    	right_sentence = sentence_getRightSubsentece(sentence);
    	if(!strcmp(midle_data->value,"print")){
    		return print_func(sentence_getValue(right_sentence)->value,map);
    	} else if (!strcmp(midle_data->value, "input")) {
			//Check argument list for input
    		return input_func();
		} else if (!strcmp(midle_data->value, "int")) {
			
			List list = sentence_getValue(right_sentence)->value;
			if( list_size(list) < 1){
				printf("RUNTIME ERROR: too few arguments for a int cast\n");
				return data_create(ERROR, NULL);
			}
			if(list_size(list) > 1){
				printf("RUNTIME ERROR: too much arguments for a int cast\n");
				return data_create(ERROR, NULL);
			}
			return int_cast(list_get(list,0)->value,map);
			
		} else if (!strcmp(midle_data->value, "float")) {
			
			List list = sentence_getValue(right_sentence)->value;
			if( list_size(list) < 1){
				printf("RUNTIME ERROR: too few arguments for a float cast\n");
				return data_create(ERROR, NULL);
			}
			if(list_size(list) > 1){
				printf("RUNTIME ERROR: too much arguments for a float cast\n");
				return data_create(ERROR, NULL);
			}
			return float_cast(list_get(list,0)->value,map);

		} else if (!strcmp(midle_data->value, "str")) {
			
			List list = sentence_getValue(right_sentence)->value;
			if( list_size(list) < 1){
				printf("RUNTIME ERROR: too few arguments for a string cast\n");
				return data_create(ERROR, NULL);
			}
			if(list_size(list) > 1){
				printf("RUNTIME ERROR: too much arguments for a string cast\n");
				return data_create(ERROR, NULL);
			}
			return string_cast(list_get(list,0)->value,map);
		}

        answer =  map_get(map,midle_data);
        if(!answer){
            printf("RUNTIME ERROR: the '%s' name is not defined.\n",(char*)midle_data->value);
            return data_create(ERROR, NULL);
        }
        if(answer->type == FUNCTION){
        	

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
        if(left_data->type < LIST || left_data -> type > MAP){
            data_destroy(left_data);
            left_data = sentence_getValue(left_sentence);
            printf("RUNTIME ERROR: the %s is not a abstract data type.\n",(char*)left_data->value);
            return data_create(ERROR, NULL);
        }
        right_sentence = sentence_getRightSubsentece(sentence);
        answer = Runtime_evaluateMethod(left_data,right_sentence, map);
        data_destroy(left_data);
        return answer;
    case LIST:
        right_sentence = sentence_getRightSubsentece(sentence);
        right_data = sentence_getValue(right_sentence);
        if (map_get(map, right_data)) {
			printf("RUNTIME ERROR: double declaration of '%s'\n",(char*)right_data->value);
            return data_create(ERROR, NULL);
        }
        map_put(map, data_makeCopy(right_data),data_create(LIST, list_create(data_makeCopy(right_data))));
        return NULL;
    case STACK:
        right_sentence = sentence_getRightSubsentece(sentence);
        right_data = sentence_getValue(right_sentence);
        if (map_get(map, right_data)) {printf("RUNTIME ERROR: double declaration of '%s'\n",(char*)right_data->value);
            return data_create(ERROR, NULL);
        }
        map_put(map, data_makeCopy(right_data),data_create(STACK, stack_create(data_makeCopy(right_data))));
        return NULL;
    case QUEUE:
        right_sentence = sentence_getRightSubsentece(sentence);
        right_data = sentence_getValue(right_sentence);
        if (map_get(map, right_data)) {printf("RUNTIME ERROR: double declaration of '%s'\n",(char*)right_data->value);
            return data_create(ERROR, NULL);
        }
        map_put(map, data_makeCopy(right_data),data_create(QUEUE, queue_create(data_makeCopy(right_data))));
        return NULL;
    case SET:
        right_sentence = sentence_getRightSubsentece(sentence);
        right_data = sentence_getValue(right_sentence);
        if (map_get(map, right_data)) {printf("RUNTIME ERROR: double declaration of '%s'\n",(char*)right_data->value);
            return data_create(ERROR, NULL);
        }
        map_put(map, data_makeCopy(right_data),data_create(SET, set_create(data_makeCopy(right_data))));
        return NULL;
    case MAP:
        right_sentence = sentence_getRightSubsentece(sentence);
        right_data = sentence_getValue(right_sentence);
        if (map_get(map, right_data)) {
            printf("RUNTIME ERROR: double declaration of '%s'\n",(char*)right_data->value);
            return data_create(ERROR, NULL);
        }
        map_put(map, data_makeCopy(right_data),data_create(MAP, map_create(data_makeCopy(right_data), MEMORY_CAPACITY)));
        return NULL;
	case PLUS1:

		left_sentence = sentence_getLeftSubsentece(sentence);
		if(left_sentence){
			if(sentence_getValue(left_sentence)->type != NAME){
				printf("RUNTIME ERROR: increment is only accepted with variables\n");
				return data_create(ERROR, NULL);
			}
			left_data = resolve_sentence(left_sentence, map);
			if(left_data->type == ERROR)
				return left_data;
			if(left_data->type != INT && left_data->type != FLOAT){
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
			right_sentence = sentence_getRightSubsentece(sentence);
			if(sentence_getValue(right_sentence)->type != NAME){
				printf("RUNTIME ERROR: increment is only accepted with variables\n");
				return data_create(ERROR, NULL);
			}
			right_data = resolve_sentence(right_sentence,map);
			if(right_data->type == ERROR)
				return right_data;
			if(right_data->type != INT && right_data->type != FLOAT){
				data_destroy(right_data);
				printf("RUNTIME ERROR: invalid operation\n");
				return data_create(ERROR, NULL);
			}
			if(right_data->type == INT)
				(*((int*)right_data->value))++;
			else
				(*((float*)right_data->value))++;
			map_put(map,sentence_getValue(right_sentence),right_data);
			return data_copyResolvedData(right_data);
		}
			
	case MINUS1:
		left_sentence = sentence_getLeftSubsentece(sentence);

		if(left_sentence){
			if (sentence_getValue(left_sentence)->type != NAME) {
				printf("RUNTIME ERROR: decrement is only accepted with variables\n");
				return data_create(ERROR, NULL);
			}
			left_data = resolve_sentence(left_sentence, map);
			if(left_data->type == ERROR)
				return left_data;
			if(left_data->type != INT && left_data->type != FLOAT){
				data_destroy(left_data);
				printf("RUNTIME ERROR: invalid operation\n");
				return data_create(ERROR, NULL);
			}
			answer = data_copyResolvedData(left_data);
			if(left_data->type == INT)
				(*((int*)left_data->value))--;
			else
				(*((float*)left_data->value))--;
			map_put(map,sentence_getValue(left_sentence),left_data);
			return answer;
		}
		else{
			right_sentence = sentence_getRightSubsentece(sentence);
			if (sentence_getValue(right_sentence)->type != NAME) {
				printf("RUNTIME ERROR: decrement is only accepted with variables\n");
				return data_create(ERROR, NULL);
			}
			right_data = resolve_sentence(right_sentence,map);
			if(right_data->type == ERROR)
				return right_data;
			if(right_data->type != INT && right_data->type != FLOAT){
				data_destroy(right_data);
				printf("RUNTIME ERROR: invalid operation\n");
				return data_create(ERROR, NULL);
			}
			if(right_data->type == INT)
				(*((int*)right_data->value))--;
			else
				(*((float*)right_data->value))--;
			map_put(map,sentence_getValue(right_sentence),right_data);
			return data_copyResolvedData(right_data);
		}
		    
	case NEGATION:
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			printf("RUNTIME ERROR: no able to negate the sentence\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR)
			return right_data;
		if(right_data->type != INT && right_data->type != FLOAT){
			data_destroy(right_data);
			printf("RUNTIME ERROR: not acepted type to negate\n");
			return data_create(ERROR, NULL);
		}
		answer = data_copyResolvedData(right_data);
		if(answer->type == INT)
			(*((int*)(answer->value))) = !(*((int*)(answer->value)));
		return answer;
			
	case MULTIPLICATION:
			left_sentence = sentence_getLeftSubsentece(sentence);
			left_data = resolve_sentence(left_sentence,map);
			right_sentence = sentence_getRightSubsentece(sentence);
			right_data = resolve_sentence(right_sentence,map);
			if(!right_data){
				data_destroy(left_data);
				printf("RUNTIME ERROR: no right parameter in the multiplication\n");
				return data_create(ERROR, NULL);
			}
			if(right_data->type == ERROR){
				data_destroy(left_data);
				return right_data;
			}
			if(!left_data){
				data_destroy(right_data);
				printf("RUNTIME ERROR: no left parameter in the multiplication\n");
				return data_create(ERROR, NULL);
			}
			if(left_data->type == ERROR){
				data_destroy(right_data);
				return left_data;
			}
			if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
				data_destroy(right_data);
				data_destroy(left_data);
				printf("RUNTIME ERROR: not acepted type for a multiplication\n");
				return data_create(ERROR, NULL);
			}
			if(left_data->type == FLOAT){
				if(right_data->type == FLOAT)
					answer = data_create(FLOAT, Runtime_createfloat(((*((float *)left_data->value))*(*((float *)right_data->value)) )));
				else
					answer = data_create(FLOAT, Runtime_createfloat(((*((float *)left_data->value))*(*((int *)right_data->value)) )));
			}
			else{
				if(right_data->type == FLOAT)
					answer = data_create(FLOAT, Runtime_createfloat(((*((int *)left_data->value))*(*((float *)right_data->value)) )));
				else
					answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))*(*((int *)right_data->value)) )));
			}
			data_destroy(left_data);
			data_destroy(right_data);
			return answer;
			
	case DIVISION:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the division\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the division\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a division\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(FLOAT, Runtime_createfloat(((*((float *)left_data->value))/(*((float *)right_data->value)) )));
			else
				answer = data_create(FLOAT, Runtime_createfloat(((*((float *)left_data->value))/(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(FLOAT, Runtime_createfloat(((*((int *)left_data->value))/(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))/(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case MODULO:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the module\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the module\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if(left_data->type != INT || right_data->type != INT){
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: module operation only acept Int values\n");
			return data_create(ERROR, NULL);
		}
		answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))%(*((int *)right_data->value)) )));
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
	case SUM:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the summation\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the summation\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			if(left_data->type == STRING && right_data->type == STRING){
				answer = data_create(STRING,Runtime_sumStrings(left_data->value,right_data->value));
				data_destroy(right_data);
				data_destroy(left_data);
				return answer;
			}
			else{
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type to negate\n");
			return data_create(ERROR, NULL);
			}
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(FLOAT, Runtime_createfloat(((*((float *)left_data->value))+(*((float *)right_data->value)) )));
			else
				answer = data_create(FLOAT, Runtime_createfloat(((*((float *)left_data->value))+(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(FLOAT, Runtime_createfloat(((*((int *)left_data->value))+(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))+(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case SUBSTRACT:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the substraction\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the substraction\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a substraction\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(FLOAT, Runtime_createfloat(((*((float *)left_data->value))-(*((float *)right_data->value)) )));
			else
				answer = data_create(FLOAT, Runtime_createfloat(((*((float *)left_data->value))-(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(FLOAT, Runtime_createfloat(((*((int *)left_data->value))-(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))-(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case GREATER:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the greater operation\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the greater operation\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			if(left_data->type == STRING && right_data->type == STRING){
				answer = data_create(INT, Runtime_createInt(strcmp(left_data->value,right_data->value)>0));
				data_destroy(right_data);
				data_destroy(left_data);
				return answer;
			}
			else{
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a greater operation \n");
			return data_create(ERROR, NULL);
			}
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(INT,  Runtime_createInt(((*((float *)left_data->value))>(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((float *)left_data->value))>(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))>(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))>(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case GREATER_EQUAL:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the greater equal operation\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the greater equal operation\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			if(left_data->type == STRING && right_data->type == STRING){
				answer = data_create(INT, Runtime_createInt(strcmp(left_data->value,right_data->value)>=0));
				data_destroy(right_data);
				data_destroy(left_data);
				return answer;
			}
			else{
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a greater equal operation\n");
			return data_create(ERROR, NULL);
			}
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(INT,  Runtime_createInt(((*((float *)left_data->value))>=(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((float *)left_data->value))>=(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))>=(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))>=(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case SMALLER_EQUAL:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the smaller equal operation\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the smaller equal operation\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			if(left_data->type == STRING && right_data->type == STRING){
				answer = data_create(INT, Runtime_createInt(strcmp(left_data->value,right_data->value)<=0));
				data_destroy(right_data);
				data_destroy(left_data);
				return answer;
			}
			else{
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a smaller equal operation\n");
			return data_create(ERROR, NULL);
			}
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(INT,  Runtime_createInt(((*((float *)left_data->value))<=(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((float *)left_data->value))<=(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))<=(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))<=(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case SMALLER:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the smaller operation\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the smaller operation\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			if(left_data->type == STRING && right_data->type == STRING){
				answer = data_create(INT, Runtime_createInt(strcmp(left_data->value,right_data->value)<0));
				data_destroy(right_data);
				data_destroy(left_data);
				return answer;
			}
			else{
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a smaller operation\n");
			return data_create(ERROR, NULL);
			}
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(INT,  Runtime_createInt(((*((float *)left_data->value))<(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((float *)left_data->value))<(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))<(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))<(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case EQUAL:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the equal operation\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the equal operation\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			if(left_data->type == STRING && right_data->type == STRING){
				answer = data_create(INT, Runtime_createInt(strcmp(left_data->value,right_data->value) == 0));
				data_destroy(right_data);
				data_destroy(left_data);
				return answer;
			}
			else{
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for the equal\n");
			return data_create(ERROR, NULL);
			}
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(INT,  Runtime_createInt(((*((float *)left_data->value))==(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((float *)left_data->value))==(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))==(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))==(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case NOT_EQUAL:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the not equal operation\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the not equal operation\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			if(left_data->type == STRING && right_data->type == STRING){
				answer = data_create(INT, Runtime_createInt(strcmp(left_data->value,right_data->value)!=0));
				data_destroy(right_data);
				data_destroy(left_data);
				return answer;
			}
			else{
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a not equal operation\n");
			return data_create(ERROR, NULL);
			}
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(INT,  Runtime_createInt(((*((float *)left_data->value))!=(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((float *)left_data->value))!=(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))!=(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))!=(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case BINARY_AND:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the binary AND\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the binary AND\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a binary AND\n");
			return data_create(ERROR, NULL);
		}
		answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))&(*((int *)right_data->value)) )));
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case BINARY_XOR:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the binary XOR\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the binary XOR\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a binary XOR\n");
			return data_create(ERROR, NULL);
		}
		answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))^(*((int *)right_data->value)) )));
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case BINARY_OR:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the binary OR\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the binary OR\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a binary OR\n");
			return data_create(ERROR, NULL);
		}
		answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))|(*((int *)right_data->value)) )));
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case LOGIC_AND:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the logic AND\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the logic AND\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a logic AND\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(INT,  Runtime_createInt(((*((float *)left_data->value))&&(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((float *)left_data->value))&&(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))&&(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))&&(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
			
	case LOGIC_OR:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = resolve_sentence(left_sentence,map);
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			data_destroy(left_data);
			printf("RUNTIME ERROR: no right parameter in the logic AND\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			data_destroy(left_data);
			return right_data;
		}
		if(!left_data){
			data_destroy(right_data);
			printf("RUNTIME ERROR: no left parameter in the logic AND\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == ERROR){
			data_destroy(right_data);
			return left_data;
		}
		if((left_data->type != INT && left_data->type != FLOAT) || (right_data->type != INT && right_data->type != FLOAT)){
			data_destroy(right_data);
			data_destroy(left_data);
			printf("RUNTIME ERROR: not acepted type for a logic AND\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type == FLOAT){
			if(right_data->type == FLOAT)
				answer = data_create(INT,  Runtime_createInt(((*((float *)left_data->value))||(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((float *)left_data->value))||(*((int *)right_data->value)) )));
		}
		else{
			if(right_data->type == FLOAT)
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))||(*((float *)right_data->value)) )));
			else
				answer = data_create(INT, Runtime_createInt(((*((int *)left_data->value))||(*((int *)right_data->value)) )));
		}
		data_destroy(left_data);
		data_destroy(right_data);
		return answer;
            
    case TERNARY_QM:
            left_sentence = sentence_getLeftSubsentece(sentence);
            left_data = resolve_sentence(left_sentence, map);
            
            if (!left_data){
                printf("RUNTIME ERROR: Conditional sentece must return something for ternary operand.\n");
                return data_create(ERROR, NULL);
            }
            
            if(left_data -> type != INT && left_data -> type != FLOAT) {
                printf("RUNTIME ERROR: Conditional sentece must return a boolean value for ternary operand.\n");
                data_destroy(left_data);
                return data_create(ERROR, NULL);
            }
            
            right_sentence = sentence_getRightSubsentece(sentence);
            right_data = sentence_getValue(right_sentence);
            if (!right_data) {
                printf("RUNTIME ERROR: Ternary operand must specify a result using ':'.\n");
                data_destroy(left_data);
                return data_create(ERROR, NULL);
            }
            
            if (right_data -> type != TERNARY_DOTS) {
                printf("RUNTIME ERROR: Ternary operand must specify a result using ':'.\n");
                data_destroy(left_data);
                return data_create(ERROR, NULL);
            }
            
            if ((left_data -> type == INT && *(int*)left_data -> value) || (left_data -> type == FLOAT && *(float*)left_data -> value)) {
                answer = resolve_sentence(sentence_getLeftSubsentece(right_sentence), map);
            }else{
                answer = resolve_sentence(sentence_getRightSubsentece(right_sentence), map);
            }
            
            data_destroy(left_data);
            
            return answer;
            
   	case ASIGN:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = sentence_getValue(left_sentence);
		if(!left_data){
			printf("RUNTIME ERROR: assignation without variable name\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type != NAME){
			printf("RUNTIME ERROR: assignation to a no variable name\n");
			return data_create(ERROR, NULL);
		}

		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			printf("RUNTIME ERROR: assignation with not variable\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			return right_data;
		}
		map_put(map,data_makeCopy(left_data),right_data);
		return data_copyResolvedData(right_data);
			
	case SUM_ASIGN:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = sentence_getValue(left_sentence);
		if(!left_data){
			printf("RUNTIME ERROR: assignation without variable name\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type != NAME){
			printf("RUNTIME ERROR: assignation to a no variable name\n");
			return data_create(ERROR, NULL);
		}
		answer = map_get(map,left_data);
		if(!answer){
			printf("RUNTIME ERROR: sum assignation with a not declared name\n");
			return data_create(ERROR, NULL);
		}
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			printf("RUNTIME ERROR: assignation with not variable\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			return right_data;
		}
		answer = data_copyResolvedData(answer);
		if ((answer->type != INT && answer->type != FLOAT)|| (right_data->type != INT && right_data->type != FLOAT)) {
			if (answer->type == STRING && right_data->type == STRING) {
				map_put(map,left_data,data_create(STRING,Runtime_sumStrings(answer->value,right_data->value)));
				data_destroy(answer);
				data_destroy(right_data);
				return data_copyResolvedData(map_get(map,left_data));
			} else {
				data_destroy(right_data);
				data_destroy(answer);
				printf("RUNTIME ERROR: not compatible types for a sum asignation\n");
				return data_create(ERROR, NULL);
			}
		}
		if (answer->type == FLOAT) {
			if (right_data->type == FLOAT)
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((float *) answer->value))+ (*((float *) right_data->value))))));
			else
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((float *) answer->value))+ (*((int *) right_data->value))))));
		} else {
			if (right_data->type == FLOAT)
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((int *) answer->value))+ (*((float *) right_data->value))))));
			else
				map_put(map,left_data,data_create(INT,Runtime_createInt(((*((int *) answer->value))+ (*((int *) right_data->value))))));
		}
		data_destroy(answer);
		data_destroy(right_data);
		return data_copyResolvedData(map_get(map,left_data));
			
	case SUBSTRACT_ASIGN:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = sentence_getValue(left_sentence);
		if(!left_data){
			printf("RUNTIME ERROR: assignation without variable name\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type != NAME){
			printf("RUNTIME ERROR: assignation to a no variable name\n");
			return data_create(ERROR, NULL);
		}
		answer = map_get(map,left_data);
		if(!answer){
			printf("RUNTIME ERROR: substraction assignation with a not declared name\n");
			return data_create(ERROR, NULL);
		}
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			printf("RUNTIME ERROR: assignation with not variable\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			return right_data;
		}
		answer = data_copyResolvedData(answer);
		if ((answer->type != INT && answer->type != FLOAT)|| (right_data->type != INT && right_data->type != FLOAT)) {
				data_destroy(right_data);
				data_destroy(answer);
				printf("RUNTIME ERROR: not compatible types for a rest asignation\n");
				return data_create(ERROR, NULL);
		}
		if (answer->type == FLOAT) {
			if (right_data->type == FLOAT)
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((float *) answer->value))- (*((float *) right_data->value))))));
			else
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((float *) answer->value))- (*((int *) right_data->value))))));
		} else {
			if (right_data->type == FLOAT)
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((int *) answer->value))- (*((float *) right_data->value))))));
			else
				map_put(map,left_data,data_create(INT,Runtime_createInt(((*((int *) answer->value))- (*((int *) right_data->value))))));
		}
		data_destroy(answer);
		data_destroy(right_data);
		return data_copyResolvedData(map_get(map,left_data));
			
	case MULTIPLICATION_ASIGN:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = sentence_getValue(left_sentence);
		if(!left_data){
			printf("RUNTIME ERROR: assignation without variable name\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type != NAME){
			printf("RUNTIME ERROR: assignation to a no variable name\n");
			return data_create(ERROR, NULL);
		}
		answer = map_get(map,left_data);
		if(!answer){
			printf("RUNTIME ERROR: multiplication assignation with a not declared name\n");
			return data_create(ERROR, NULL);
		}
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			printf("RUNTIME ERROR: assignation with not variable\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			return right_data;
		}
		answer = data_copyResolvedData(answer);
		if ((answer->type != INT && answer->type != FLOAT)|| (right_data->type != INT && right_data->type != FLOAT)) {
				data_destroy(right_data);
				data_destroy(answer);
				printf("RUNTIME ERROR: not compatible types for a multiplication asignation\n");
				return data_create(ERROR, NULL);
		}
		if (answer->type == FLOAT) {
			if (right_data->type == FLOAT)
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((float *) answer->value))* (*((float *) right_data->value))))));
			else
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((float *) answer->value))* (*((int *) right_data->value))))));
		} else {
			if (right_data->type == FLOAT)
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((int *) answer->value))* (*((float *) right_data->value))))));
			else
				map_put(map,left_data,data_create(INT,Runtime_createInt(((*((int *) answer->value))* (*((int *) right_data->value))))));
		}
		data_destroy(answer);
		data_destroy(right_data);
		return data_copyResolvedData(map_get(map,left_data));
			
	case DIVISION_ASIGN:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = sentence_getValue(left_sentence);
		if(!left_data){
			printf("RUNTIME ERROR: assignation without variable name\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type != NAME){
			printf("RUNTIME ERROR: assignation to a no variable name\n");
			return data_create(ERROR, NULL);
		}
		answer = map_get(map,left_data);
		if(!answer){
			printf("RUNTIME ERROR: division assignation with a not declared name\n");
			return data_create(ERROR, NULL);
		}
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			printf("RUNTIME ERROR: assignation with not variable\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			return right_data;
		}
		answer = data_copyResolvedData(answer);
		if ((answer->type != INT && answer->type != FLOAT)|| (right_data->type != INT && right_data->type != FLOAT)) {
				data_destroy(right_data);
				data_destroy(answer);
				printf("RUNTIME ERROR: not compatible types for a division asignation\n");
				return data_create(ERROR, NULL);
		}
		if (answer->type == FLOAT) {
			if (right_data->type == FLOAT)
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((float *) answer->value))/ (*((float *) right_data->value))))));
			else
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((float *) answer->value))/ (*((int *) right_data->value))))));
		} else {
			if (right_data->type == FLOAT)
				map_put(map,left_data, data_create(FLOAT,Runtime_createfloat(((*((int *) answer->value))/ (*((float *) right_data->value))))));
			else
				map_put(map,left_data,data_create(INT,Runtime_createInt(((*((int *) answer->value))/ (*((int *) right_data->value))))));
		}
		data_destroy(answer);
		data_destroy(right_data);
		return data_copyResolvedData(map_get(map,left_data));
			
	case MODULO_ASIGN:
		left_sentence = sentence_getLeftSubsentece(sentence);
		left_data = sentence_getValue(left_sentence);
		if(!left_data){
			printf("RUNTIME ERROR: assignation without variable name\n");
			return data_create(ERROR, NULL);
		}
		if(left_data->type != NAME){
			printf("RUNTIME ERROR: assignation to a no variable name\n");
			return data_create(ERROR, NULL);
		}
		answer = map_get(map,left_data);
		if(!answer){
			printf("RUNTIME ERROR: module assignation with a not declared name\n");
			return data_create(ERROR, NULL);
		}
		right_sentence = sentence_getRightSubsentece(sentence);
		right_data = resolve_sentence(right_sentence,map);
		if(!right_data){
			printf("RUNTIME ERROR: assignation with not variable\n");
			return data_create(ERROR, NULL);
		}
		if(right_data->type == ERROR){
			return right_data;
		}
		answer = data_copyResolvedData(answer);
		if (answer->type != INT || right_data->type != INT ) {
				data_destroy(right_data);
				data_destroy(answer);
				printf("RUNTIME ERROR: not compatible types for a module asignation\n");
				return data_create(ERROR, NULL);
		}
		map_put(map,left_data,data_create(INT,Runtime_createInt(((*((int *) answer->value))% (*((int *) right_data->value))))));
		data_destroy(answer);
		data_destroy(right_data);
		return data_copyResolvedData(map_get(map,left_data));
            
    case IF:
            left_sentence = sentence_getLeftSubsentece(sentence);
            left_data = resolve_sentence(left_sentence, map);
            
            if (!left_data){
                printf("RUNTIME ERROR: There must be a conditional sentece for IF statement.\n");
                return data_create(ERROR, NULL);
            }
            
            if(left_data -> type != INT && left_data -> type != FLOAT) {
                printf("RUNTIME ERROR: Conditional sentece must return a boolean value for IF statement.\n");
                data_destroy(left_data);
                return data_create(ERROR, NULL);
            }
            
            right_sentence = sentence_getRightSubsentece(sentence);
            
            if ((left_data -> type == INT && *(int*)left_data -> value) || (left_data -> type == FLOAT && *(float*)left_data -> value)) {
                
                answer = resolve_block(map, sentence_getValue(sentence_getLeftSubsentece(right_sentence)) -> value);
            }else{
                answer = resolve_block(map, sentence_getValue(sentence_getRightSubsentece(right_sentence)) -> value);
            }
            data_destroy(left_data);
            
            return answer;
            
    case WHILE:
            left_sentence = sentence_getLeftSubsentece(sentence);
			
            left_data = resolve_sentence(sentence_getValue(left_sentence) -> value, map);
            
            if (!left_data){
                printf("RUNTIME ERROR: There must be a conditional sentece for WHILE statement.\n");
                return data_create(ERROR, NULL);
            }
            
            if(left_data -> type != INT && left_data -> type != FLOAT) {
                printf("RUNTIME ERROR: Conditional sentece must return a boolean value for WHILE statement.\n");
                data_destroy(left_data);
                return data_create(ERROR, NULL);
            }
            
            right_sentence = sentence_getRightSubsentece(sentence);
            right_data = sentence_getValue(right_sentence);
            
            while ((left_data -> type == INT && *(int*)left_data -> value) || (left_data -> type == FLOAT && *(float*)left_data -> value)) {
                answer = resolve_block(map, right_data -> value);
                if (answer) {
                    if (answer -> type == BREAK) {
                        data_destroy(left_data);
                        data_destroy(answer);
                        return NULL;
					
					}else if(answer -> type == ERROR){
                        data_destroy(left_data);
                        return answer;
						
					}else{
						data_destroy(answer);
					}
                }
                data_destroy(left_data);
				left_data = resolve_sentence(sentence_getValue(left_sentence) -> value, map);
            }
            data_destroy(left_data);
            
            return NULL;
            
    case FOR:
			left_sentence = sentence_getLeftSubsentece(sentence);
			left_data = sentence_getValue(left_sentence);
			List forInstructions = left_data -> value;
			
			Data initialization = list_get(forInstructions, 0);
			
			Data initializationResult = resolve_sentence(initialization -> value, map);
			
			if (initializationResult && initializationResult -> type == ERROR) {
				return initializationResult;
			}else{
				data_destroy(initializationResult);
			}
			
			Data condition = list_get(forInstructions, 1);
			
			Data conditionResolved = resolve_sentence(condition -> value, map);
			
			if (!conditionResolved){
				printf("RUNTIME ERROR: There must be a conditional sentece for FOR statement.\n");
				return data_create(ERROR, NULL);
			}
			
			if(conditionResolved -> type != INT && conditionResolved -> type != FLOAT) {
				printf("RUNTIME ERROR: Conditional sentece must return a boolean value for FOR statement.\n");
				return data_create(ERROR, NULL);
			}
			
			Sentence increment = list_get(forInstructions, 2) -> value;
			
			if (!increment) {
				printf("RUNTIME ERROR: Invalid types increment for FOR statement.\n");
				return NULL;
			}
			
			right_sentence = sentence_getRightSubsentece(sentence);
			right_data = sentence_getValue(right_sentence);
			Data incrementResult;
			
			while ((conditionResolved -> type == INT && *(int*)conditionResolved -> value) || (conditionResolved -> type == FLOAT && *(float*)conditionResolved -> value)) {
				
				answer = resolve_block(map, right_data -> value);
				if (answer) {
					if (answer -> type == BREAK) {
						
						data_destroy(conditionResolved);
						data_destroy(answer);
						return NULL;
						
					}else if(answer -> type == ERROR){
						data_destroy(conditionResolved);
						return answer;
					}else{
						data_destroy(answer);
					}
					
				}
				
				incrementResult = resolve_sentence(increment, map);
				if (incrementResult) {
					if (incrementResult -> type == ERROR) {
						return incrementResult;
					}
				}
				
				data_destroy(incrementResult);
				
				data_destroy(conditionResolved);
				conditionResolved = resolve_sentence(condition -> value, map);
			}
			
			data_destroy(conditionResolved);
			
			
			return NULL;
            
    case BREAK:
            return data_create(BREAK, NULL);
            break;
            
    case CONTINUE:
            return data_create(CONTINUE, NULL);
            break;
			
    default:
        return NULL;
    }
    return NULL;
}


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
    main_data -> value = (char *) calloc(5, sizeof(char));
	
    //Here, the name of the main can change in the future
    strcpy(main_data->value, "main");
    //-------------------------------------------------------------------------------------//
    Data main_func = map_get(map_functions, main_data);
	if (!main_func) {
		printf("RUNTIME ERROR: No function named 'main'.\n");
		return;
	}
    free(main_data);
    Block block = (Block) main_func -> value;
    int bl_size = block_size(block);
    Sentence sentence;
    Data result_data;
    for(int i = 0; i<bl_size; i++){
        sentence = block_getSentence(block,i);
        result_data = resolve_sentence(sentence, map_functions);
        if (result_data) {
            if(result_data->type == ERROR){
                data_destroy(result_data);
                return;
            }
        }
    }
	printf("\n");
};
