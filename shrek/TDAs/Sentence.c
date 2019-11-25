//
//  Sentence.c
//  shrek
//
//

#include "Sentence.h"

struct strSentence {
    Sentence leftSubsentence, rightSubsentence;
    error error;
};

/*
 typedef enum {ERROR = -1,
 FALSE, TRUE, NAME, INT, FLOAT, STRING, END_LINE, LPAREN, RPAREN, LBRACE, RBRACE, 1
 LIST, STACK, QUEUE, SET, MAP, PLUS1, PLUS2,2
 MULTIPLICATION, DIVISION, MODULO,3
 SUM, SUBSTRACT,4
 GREATER, GREATER_EQUAL, SMALLER_EQUAL, SMALLER,5
 EQUAL, NOT_EQUAL,6
 BINARY_AND,7
 BINARY_XOR,8
 BINARY_OR,9
 LOGIC_AND,10
 LOGIC_OR, 11
 TERNARY, 12
 ASIGN, SUM_ASIGN, SUBSTRACT_ASIGN, MULTIPLICATION_ASIGN, DIVISION_ASIGN, MODULO_ASIGN, 13
 IF, ELSE, WHILE, FOR, BREAK, CONTINUE, BEGIN_MAIN, END_MAIN, FUNCTION 14
 //last in priority
 } tokenType;*/

int priority(tokenType type){
    if (0 <= type && type <= 10) {
        return 1;
    }else if(11 <= type && type <= 17){
        return 2;
    }else if(18 <= type && type <= 20){
        return 3;
    }else if(21 <= type && type <= 22){
        return 4;
    }else if(23 <= type && type <= 26){
        return 5;
    }else if(27 <= type && type <= 28){
        return 6;
    }else if(29 == type){
        return 7;
    }else if (30 == type){
        return 8;
    }else if (31 == type){
        return 9;
    }else if (32 == type){
        return 10;
    }else if (33 == type){
        return 11;
    }else if (34 == type){
        return 12;
    }else if (35 <= type && type <= 40){
        return 13;
    }else if (40 <= type && type <= 50){
        return 14;
    }else{
        return -1;
    }
}
