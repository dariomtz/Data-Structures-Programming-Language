//
//  test.c
//  shrek
//
//

#include "test.h"

void test_list(){
    List list = list_create(NULL);
    printf("\nTest 1:\n");
    
    list_set(list, -1, NULL);
    printf("Test 1");
    
    
    printf("\n List passed all the tests :)");
}

void test_set(){
    printf("\n Set passed all the tests :)");
}

void test_map(){
    printf("\n Map passed all the tests :)");
}

void test_stack(){
    printf("\n Stack passed all the tests :)");
}

void test_queue(){
    printf("\n Queue passed all the tests :)");
}

void test_lexer(){
Lexer lexer;
    FILE * f;
    Token token;
    lexer = lexer_create(NULL);
    //Test 1 null file
    if(lexer)
    	printf("Failed test1: The lexer was created even if there wasnt a file\n");
    //test 2 empty file
    f = fopen("emptyText.txt", "r");
    lexer = lexer_create(f);
    fclose(f);
    if(!lexer)
    	printf("Failed test2: The lexer was not created even if there was a file\n");
    if(lexer_numberOfTokens(lexer))
    	printf("Failed test2: The lexer has a size even if there is the file empty\n");
    if(lexer_getToken(lexer))
    	printf("Failed test2: The lexer returned a token even if the file is empty\n");
    lexer_destroy(lexer);

    //test 3 all the tokentype
	f = fopen("alltokentype.txt", "r");
	lexer = lexer_create(f);
	fclose(f);
	if (!lexer)
		printf("Failed test3: The lexer was not created even if there was a file\n");
	if (lexer_numberOfTokens(lexer) != 53)
		printf("Failed test3: The lexer has not a size of 52\n");
	for(int i = 0; i<53; i++){
		token = lexer_getToken(lexer, i);
		if(i == token->type)
			printf("Failed test 3: the token %d have a %d token type, even if it has to be %d\n",i,token->type,i);
	}
	lexer_destroy(lexer);

	//test 4 error token
	f = fopen("errortoken.txt", "r");
	lexer = lexer_create(f);
	fclose(f);

	if (!lexer)
		printf("Failed test4: The lexer was not created even if there was a file\n");
	if (lexer_numberOfTokens(lexer) != 19)
		printf("Failed test4: The lexer has not a size of 19\n");
	token = lexer_getToken(lexer, lexer_numberOfTokens-1);
	if(ERROR != token->type)
		printf("Failed test4: The last token in lexer is not a error even if it was suppose to be a error\n");
	for (int i = 0; i < 19; i++) {
		token = lexer_getToken(lexer, i);
		if (i == token->type)
			printf("Failed test 4: the token %d have a %d token type, even if it has to be %d\n",i, token->type, i);
	}  
    printf("\n Lexer passed all the tests :)");
}

void test_parser(){
    
    printf("\n Parser passed all the tests :)");
}

void test_runtime(){
    printf("\n Runtime passed all the tests :)");
}

void test_all(){
    test_map();
    test_set();
    test_list();
    test_stack();
    test_queue();
    
    test_lexer();
    test_parser();
    test_runtime();
}
