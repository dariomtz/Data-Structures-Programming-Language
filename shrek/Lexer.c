//
//  Lexer.c
//  shrek
//
/*
    Implementation of Lexer struct and its functions
*/

#include "Lexer.h"

#define caracter(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')|| (c>= '0' && c <= '9') || (c == '_'))

typedef enum{STRINING,CARACTER,OPERATIONAL,UNITARY, USELESS}action;

struct strLexer{
	int size;
	int capacity;
	Token * list;
};

void lexer_print(Lexer lexer){
	if(!lexer)
		return;
	int size = lexer_numberOfTokens(lexer);
	Token token;
	for(int i = 0; i<size; i++){
		token = lexer_getToken(lexer, i);
		printf("\t<Type: %d,\t",token->type);
		if(((char*)(token->value))[0] == '\n' || ((char*)(token->value))[0] == 13)
			printf("Value: \\n>\n");
        else{
		printf("Value: '%s'>\n",(char*)(token->value));
        }
	}
};

char * string_modification(char * c, int size){
	int removes = 0;
	for(int i = 0; i<size; i++){
		if(c[i] == '\\'){
			removes++;
			i++;
		}
	}
	char * word = (char *)malloc((size-2-removes)*sizeof(char));
	int mov = 0;
	for(int i = 0; i<(size-2-removes); i++){
		if(c[i+1+mov] == '\\'){
			if(c[i+2+mov] == 'n')
				word[i] = '\n';
			if(c[i+2+mov] == 't')
				word[i] = '\t';
			if(c[i+2+mov] == '\\')
				word[i] = '\\';
			if(c[i+2+mov] == '\"')
				word[i] = '\"';
			mov++;
			continue;
		}
		word[i] = c[i+1+mov];
	}
	word[size-2-removes] = '\0';
	free((void*)c);
	return word;
};

int evaluate_Action(char Char){
	if (Char == '"')
		return STRINING;
	if (Char == ' ' || Char == '\t')
		return USELESS;
	if (caracter(Char)||Char == '.')
		return CARACTER;
	if (Char == '\n' || Char == 13 || Char == '(' ||  Char == ')' || Char == '{' || Char == '}' || Char == '?' || Char == ':' || Char == ',' || Char == ';')
		return UNITARY;
	return OPERATIONAL;

}
;

tokenType searchType(char* word, int size){

	//First it look if the word is a string
	if(word[0] == '"' && word[size-1]=='"')
		return STRING;

	//It look the size of the word and search it in the switch
	switch(size){
	case 1:
		if (word[0] == '+')
			return SUM;
		if (word[0] == '-')
			return SUBSTRACT;
		if (word[0] == '*')
			return MULTIPLICATION;
		if (word[0] == '/')
			return DIVISION;
		if (word[0] == '%')
			return MODULO;
		if (word[0] == '&')
			return BINARY_AND;
		if (word[0] == '^')
			return BINARY_XOR;
		if (word[0] == '|')
			return BINARY_OR;
		if (word[0] == '=')
			return ASIGN;
		if (word[0] == '>')
			return GREATER;
		if (word[0] == '<')
			return SMALLER;
		if (word[0] == '(')
			return LPAREN;
		if (word[0] == ')')
			return RPAREN;
		if(word[0] == '\n' || word[0] == 13)
			return END_LINE;
		if (word[0] == ',')
			return COMMA;
		if (word[0] == ';')
			return SEMICOLON;
		if (word[0] == '.')
			return USE_METHOD;
		if (word[0] == '{')
			return LBRACE;
		if (word[0] == '}')
			return RBRACE;
		if (word[0] == '?')
            return TERNARY_QM;
        	if (word[0] == ':')
            return TERNARY_DOTS;
		if (word[0] == '!')
            return NEGATION;
		break;
	case 2:
		if (!(strcmp(word, "if")))
			return IF;
		if (!(strcmp(word, ">=")))
			return GREATER_EQUAL;
		if (!(strcmp(word, "<=")))
			return SMALLER_EQUAL;
		if(!(strcmp(word,"==")))
			return EQUAL;
		if (!(strcmp(word, "!=")))
			return NOT_EQUAL;
		if (!(strcmp(word, "+=")))
					return SUM_ASIGN;
		if (!(strcmp(word, "-=")))
			return SUBSTRACT_ASIGN;
		if (!(strcmp(word, "*=")))
			return MULTIPLICATION_ASIGN;
		if (!(strcmp(word, "/=")))
			return DIVISION_ASIGN;
		if (!(strcmp(word, "%=")))
			return MODULO_ASIGN;
		if (!(strcmp(word, "++")))
			return PLUS1;
		if (!(strcmp(word, "--")))
			return MINUS1;
		if (!(strcmp(word, "&&")))
			return LOGIC_AND;
		if (!(strcmp(word, "||")))
			return LOGIC_OR;
		break;
	case 3:
		if (!(strcmp(word, "for")))
			return FOR;
		if (!(strcmp(word, "set")))
			return SET;
		if (!(strcmp(word, "map")))
			return MAP;
		break;
	case 4:
		if (!(strcmp(word, "TRUE")))
			return TRUE;
		if (!(strcmp(word, "else")))
			return ELSE;
		if (!(strcmp(word, "list")))
			return LIST;
		break;
	case 5:
		if (!(strcmp(word, "FALSE")))
			return FALSE;
		if (!(strcmp(word, "while")))
			return WHILE;
		if (!(strcmp(word, "break")))
			return BREAK;
		if (!(strcmp(word, "stack")))
			return STACK;
		if (!(strcmp(word, "queue")))
			return QUEUE;
		break;
	case 8:
		if (!(strcmp(word, "continue")))
			return CONTINUE;
		
		if (!(strcmp(word, "FUNCTION")))
			return FUNCTION;
		break;
	}

	//If it is here, means that it can be a number o a name
	bool numeric = true;
	bool name = true;
	int points = 0;
	char letra;
	for(int i = 0; i<size; i++){
		letra = word[i];
		if(!(caracter(letra)))
			name = false;
		if(!(letra >='0' && letra <='9') && letra != '.')
			numeric = false;
		if(letra == '.')
			points++;
		if(!(numeric || name))
			return ERROR;
	}
	if(numeric){
		if(!points)
			return INT;
		if(points == 1)
			return FLOAT;
		return ERROR;

	}
	if(name)
		return NAME;

	//if it is here, its a ERROR
	return ERROR;
};

void token_add(Lexer lexer, Data data){
	if(lexer->size == lexer->capacity){
		lexer->list = (Token *)realloc(lexer->list,2*lexer->capacity*sizeof(Token));
		lexer->capacity *=2;
	}
	lexer->list[lexer->size] = data;
	lexer->size++;
	return;
};

char * word_elavoration(FILE * f, int size, fpos_t * pos){
	char * word = (char *)malloc(size*sizeof(char));
	fsetpos(f,pos);
	for(int i = 0; i<size; i++){
		word[i] = fgetc(f);
	}
	word[size]= '\0';
	return word;
};

int repairable(char *word, Lexer l){
	int points = 0;
	int car = 0;
	char c = word[0];
	int i = 0;
	int point_pos = 0;
	Token t;
	char * new_word;
	while(c != '\0'){
		c= word[i];
		if (c == '.'){
			points++;
			point_pos = i;
		}
		else if(caracter(c))
			car++;
		i++;
	}
	if(points != 1 || (car+points) != (i-1)){
		return 0;
	}
	if(point_pos){
	t = (Token) malloc(sizeof(struct strData));
	new_word = (char*)malloc(point_pos*sizeof(char));
	for(int j = 0; j<point_pos;j++)
		new_word[j]=word[j];
	new_word[point_pos]= '\0';
	t->type =searchType(new_word,point_pos);
	t->value = new_word;
	token_add(l,t);
	}

	t = (Token) malloc(sizeof(struct strData));
	new_word = (char*)malloc(2*sizeof(char));
	new_word[0]= '.';
	new_word[1]= '\0';
	t->type = USE_METHOD;
	t->value = new_word;
	token_add(l,t);

	if(point_pos == (i-2))
		return 1;
	t = (Token) malloc(sizeof(struct strData));
	new_word = (char*)malloc((i-(point_pos))*sizeof(char));
	for(int j = point_pos+1;j<=i;j++)
		new_word[j-(point_pos)-1]= word[j];
	new_word[i-(point_pos)+1]= '\0';
	t->type = searchType(new_word,i-(point_pos)-2);
	t->value = new_word;
	token_add(l,t);
	return 1;
};

Lexer lexer_create(FILE * f){
	if (!f)
		return NULL;
	Lexer lexer = (Lexer) calloc(1, sizeof(struct strLexer));
	lexer->capacity = 20;
	lexer->list = (Data*)malloc(20*sizeof(Data));
	Token token;

	int action;
	char Char;
	char * word;
	int i = 0;
	fpos_t pos;
	fgetpos(f,&pos);
	while (!feof(f)) {
		Char = fgetc(f);
		action = evaluate_Action(Char);
		if (action == STRINING) {
			while (action == STRINING && !feof(f)) {
				i++;
				if(Char == '\\'){
					i++;
					Char = fgetc(f);
				}

				Char = fgetc(f);
				if (Char == '"') {
					i++;
					break;
				}
			}
		} else if (evaluate_Action(Char) == UNITARY) {
			i++;
			Char = fgetc(f);
		} else if (evaluate_Action(Char) == USELESS) {
			fgetpos(f,&pos);
			i=0;
		} else if (evaluate_Action(Char) == action) {
			while (evaluate_Action(Char) == action && !feof(f)) {
				i++;
				Char = fgetc(f);
			}
		}
		if (i) {
			word = word_elavoration(f, i, &pos);
			fgetpos(f,&pos);

			token = (Token) malloc(sizeof(struct strData));
			token->type = searchType(word, i);
			if (token->type == STRING)
				word = string_modification(word, i);
			if (token->type == ERROR){
				if(repairable(word, lexer)){
					free(token);
					free(word);
					i=0;
					continue;
				}
			}
			token->value = word;
			token_add(lexer, token);
			i = 0;
			if (token->type == ERROR)
				break;
		}
	}
	return lexer;
};

void lexer_destroy(Lexer lexer){
	if(!lexer)
		return;
	Token token;
	for(int i = 0; i<lexer->size;i++){
		token = lexer->list[i];
		free((void*)token->value);
		free((void*)token);
	}
	free((void*)lexer->list);
	free((void*)lexer);
	return;
};

int lexer_numberOfTokens(Lexer lexer){
	return(!lexer?0:lexer->size);
};

Token lexer_getToken(Lexer lexer, int pos){
	if(!lexer || pos>=lexer->size || pos <0)
		return NULL;
	return (lexer->list[pos]);
};












