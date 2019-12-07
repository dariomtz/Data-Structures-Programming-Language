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

Data createArgumentList(Lexer lexer, int leftParen, int rightParen){
    List arguments = list_create(NULL);
    
    int i = leftParen, j = leftParen;
    Token current;
    
    while (true) {
        current = lexer_getToken(lexer, j);
        if (current ->type == COMMA || j == rightParen) {
            Sentence argument = sentence_create(lexer, i + 1, j-1);
            error e = sentence_getErrorStatus(argument);
            if (e.type) {
                return data_create(ERROR, NULL);
            }
            if (j == rightParen) {
                return data_create(LIST, arguments);
            }
            i = j;
        }
        j++;
    }
}

Sentence sentence_create(Lexer lexer, int a, int b){
    Sentence newSentece = (Sentence) malloc(sizeof(struct strSentence));
    newSentece -> error.message = (char *) malloc(100);

    //non-recursive cases
    if (a > b) {
        return NULL;
    }
    
    if (a == b) {
        newSentece -> value = data_makeCopy(lexer_getToken(lexer, a));
        newSentece -> rightSubsentence = NULL;
        newSentece -> leftSubsentence = NULL;
        return newSentece;
    }
    
    //Ignore the last character if it is a end of line character
    if (lexer_getToken(lexer, b) -> type == END_LINE) {
        b--;
    }
    
    //If both of the borders are parenthesis, they should be ignored
    if (lexer_getToken(lexer, a) -> type == LPAREN && lexer_getToken(lexer, b) -> type == RPAREN) {
        a++;
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
    int leastPriority = priority(leastPriorityData), parent = a;
    
    for (int i = a; i <= b; i++) {
        current = lexer_getToken(lexer, i);
        if (current -> type == LPAREN) {
            openedParentesis++;
            continue;
        }else if(current -> type == RPAREN){
            openedParentesis--;
            continue;
        }
        
        if (priority(current) < leastPriority && !openedParentesis) {
            leastPriorityData = current;
            leastPriority = priority(current);
            parent = i;
        }
    }
    
    newSentece -> value = leastPriorityData;
    
    
    //create a function sentence
    if (leastPriorityData -> type == NAME){
        if (a == parent && lexer_getToken(lexer, parent + 1) -> type == LPAREN && lexer_getToken(lexer, b) -> type == RPAREN) {
            Sentence argumets = (Sentence) malloc(sizeof(struct strSentence));
            argumets -> value = createArgumentList(lexer, parent + 1, b);
            argumets -> rightSubsentence = NULL;
            argumets -> leftSubsentence = NULL;
            newSentece -> rightSubsentence = argumets;
            newSentece -> leftSubsentence = NULL;
        }else{
            newSentece -> error.type = INVALID_OPERATION;
            strcpy(newSentece -> error.message, "Invalid operation.");
            return newSentece;
        }
    }
    
    newSentece -> leftSubsentence = sentence_create(lexer, a, parent - 1);
    
    if (newSentece -> leftSubsentence) {
        error leftChildError = sentence_getErrorStatus(newSentece -> leftSubsentence);
        if (leftChildError.type) {
            newSentece -> error.type = leftChildError.type;
            strcpy(newSentece -> error.message, leftChildError.message);
            return newSentece;
        }
    }
    
    newSentece -> rightSubsentence = sentence_create(lexer, parent + 1, b);
    
    if (newSentece -> rightSubsentence) {
        error rightChildError = sentence_getErrorStatus(newSentece -> rightSubsentence);
        if (rightChildError.type) {
            newSentece -> error.type = rightChildError.type;
            strcpy(newSentece -> error.message, rightChildError.message);
            return newSentece;
        }
    }
    
    return newSentece;
}

void sentence_destroy(Sentence sentence){
    if (!sentence) {
        return;
    }
    
    data_destroy(sentence -> value);
    free(sentence -> error.message);
    
    sentence_destroy(sentence -> leftSubsentence);
    sentence_destroy(sentence -> rightSubsentence);
    free(sentence);
}

Sentence sentence_getLeftSubsentece(Sentence sentence){
    return sentence ? sentence -> leftSubsentence : NULL;
}

Sentence sentence_getRightSubsentece(Sentence sentence){
    return sentence ? sentence -> rightSubsentence : NULL;
}

error sentence_getErrorStatus(Sentence sentence){
    error e = {SENTENCE_DOESNT_EXIST, NULL};
    return sentence ? sentence -> error : e;
}
