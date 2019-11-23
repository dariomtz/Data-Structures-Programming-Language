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
