//
//  test.h
//  shrek
//
//

#ifndef test_h
#define test_h

#include "stdhead.h"

#include "List.h"
#include "Map.h"
#include "Set.h"
#include "Stack.h"
#include "Queue.h"

#include "Lexer.h"
#include "Parser.h"
#include "Runtime.h"

void test_all(void);

void test_list(void);
void test_set(void);
void test_map(void);
void test_stack(void);
void test_queue(void);

void test_lexer(void);
void test_parser(void);
void test_runtime(void);

#endif /* test_h */
