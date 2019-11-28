//
//  Sentence.c
//  shrek
//
//

#include "Sentence.h"
#include "List.h"

struct strSentence {
    Data value;
    Sentence leftSubsentence, rightSubsentence;
    error error;
};

/*
 typedef enum {ERROR = -1,
 FALSE, TRUE, NAME, INT, FLOAT, STRING, END_LINE, LPAREN, RPAREN, LBRACE, RBRACE,
 1: 0-10
 USE_METHOD,
 2: 11
 LIST, STACK, QUEUE, SET, MAP, PLUS1, MINUS1,
 3: 12-18
 MULTIPLICATION, DIVISION, MODULO,
 4: 19-21
 SUM, SUBSTRACT,
 5: 22-23
 GREATER, GREATER_EQUAL, SMALLER_EQUAL, SMALLER,
 6: 24-27
 EQUAL, NOT_EQUAL,
 7: 28-29
 BINARY_AND,
 8: 30
 BINARY_XOR,
 9: 31
 BINARY_OR,
 10: 32
 LOGIC_AND,
 11: 33
 LOGIC_OR,
 12: 34
 TERNARY_QM, TERNARY_DOTS,
 13: 35-36
 ASIGN, SUM_ASIGN, SUBSTRACT_ASIGN, MULTIPLICATION_ASIGN, DIVISION_ASIGN, MODULO_ASIGN,
 14: 37-42
 COMA, SEMICOLON,
 15: 43-44,
 IF, ELSE, WHILE, FOR, BREAK, CONTINUE, FUNCTION,
 16: 45-51
 //last in priority
 } tokenType;
 */

int priority(Token t){
    if (!t) {
        return -1;
    }
    tokenType type = t -> type;
    if (type <= 10) {
        return 16;
    }else if(type <= 11){
        return 15;
    }else if(type <= 18){
        return 14;
    }else if(type <= 21){
        return 13;
    }else if(type <= 23){
        return 12;
    }else if(type <= 27){
        return 11;
    }else if(type <= 29){
        return 10;
    }else if (type <= 30){
        return 9;
    }else if (type <= 31){
        return 8;
    }else if (type <= 32){
        return 7;
    }else if (type <= 33){
        return 6;
    }else if (type <= 34){
        return 5;
    }else if (type <= 36){
        return 4;
    }else if (type <= 42){
        return 3;
    }else if (type <= 44){
        return 2;
    }else if (type <= 51){
        return 1;
    }else{
        return -1;
    }
}

List createArgumentList(Lexer lexer, int leftParen, int rightParen){
    List arguments = list_create(NULL);
    int i = leftParen, j = leftParen;
    Token current;
    
    while (true) {
        current = lexer_getToken(lexer, j);
        
    }
    
    
    return arguments;
}

Sentence sentence_create(Lexer lexer, int a, int b){
    //reservar memoria para un sentence
    Sentence newSentece = (Sentence) malloc(sizeof(struct strSentence));
    
    //manejar el caso base a == b
    if (a == b) {
        newSentece -> value = data_makeCopy(lexer_getToken(lexer, a));
        newSentece -> rightSubsentence = NULL;
        newSentece -> leftSubsentence = NULL;
        return newSentece;
    }
    
    //manejar el caso en el que hay recursión
    if (lexer_getToken(lexer, b) -> type == END_LINE) {
        b--;
    }
    
    Token first = lexer_getToken(lexer, a);
    tokenType typeFirst = first -> type;
    if (typeFirst == IF) {
        newSentece -> value = data_makeCopy(first);
        //do all the validations to make an if-else statement and return errors
    }else if (typeFirst == FOR){
        newSentece -> value = data_makeCopy(first);
        //do all the validations to make an for statement and return errors
    }else if(typeFirst == WHILE){
        newSentece -> value = data_makeCopy(first);
        //do all the validations to make an while statement and return errors
    }
        
    
    int openedParentesis = 0;
    Token leastPriorityData = lexer_getToken(lexer, a), current;
    int leastPriority = priority(leastPriorityData);
    
    for (int i = a; i <= b; i++) {
        current = lexer_getToken(lexer, i);
        if (current -> type == LPAREN) {
            openedParentesis++;
            continue;
        }else if(current -> type == RPAREN){
            openedParentesis--;
            continue;
        }
        
        if (priority(current) < priority(leastPriorityData) && !openedParentesis) {
            leastPriorityData = current;
            leastPriority = priority(current);
        }
    }
    
    //si el de mayor prioridad es un nombre, entonces hay que revisar para ejecutar una función o método.
    
    
}

void sentence_destroy(Sentence sentence);
Sentence sentence_getLeftSubsentece(Sentence sentence);
Sentence sentence_getRightSubsentece(Sentence sentence);
error sentence_getErrorStatus(Sentence sentence);
