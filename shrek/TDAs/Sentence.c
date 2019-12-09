//
//  Sentence.c
//  shrek
//
//

#include "Sentence.h"
#include "Block.h"
#include "List.h"

struct strSentence {
    Data value;
    Sentence leftSubsentence, rightSubsentence;
    error error;
};

int priority(Token t){
    if (!t) {
        return -1;
    }
    tokenType type = t -> type;
    if (type <= RBRACE) {
        return 16;
    }else if(type <= USE_METHOD){
        return 15;
    }else if(type <= MINUS1){
        return 14;
    }else if(type <= MODULO){
        return 13;
    }else if(type <= SUBSTRACT){
        return 12;
    }else if(type <= SMALLER){
        return 11;
    }else if(type <= NOT_EQUAL){
        return 10;
    }else if (type <= BINARY_AND){
        return 9;
    }else if (type <= BINARY_XOR){
        return 8;
    }else if (type <= BINARY_OR){
        return 7;
    }else if (type <= LOGIC_AND){
        return 6;
    }else if (type <= LOGIC_OR){
        return 5;
    }else if (type <= TERNARY_DOTS){
        return 4;
    }else if (type <= MODULO_ASIGN){
        return 3;
    }else if (type <= SEMICOLON){
        return 2;
    }else if (type <= FUNCTION){
        return 1;
    }else{
        return -1;
    }
}

Data createArgumentList(Lexer lexer, int leftParen, int rightParen){
    List arguments = list_create(NULL);
    
    if (leftParen + 1 == rightParen) {
        return data_create(LIST, arguments);
    }
    
    int i = leftParen, j = leftParen;
    Token current;
    
    while (true) {
        current = lexer_getToken(lexer, j);
        if (current ->type == COMMA || j == rightParen) {
            
            Sentence argument = sentence_create(lexer, i + 1, j-1);
            error e = argument -> error;
            
            if (e.type) {
                Data d = data_create(ERROR, &e);
                d -> dest = false;
                return d;
            }
            
            list_add(arguments, data_create(SENTENCE, argument));
            
            if (j == rightParen) {
                return data_create(LIST, arguments);
            }
            i = j;
        }
        j++;
    }
}

Data createForList(Lexer lexer, int sc1, int sc2, int leftParen, int rightParen){
    List list = list_create(NULL);
    
    //asignation
    Sentence asignation = sentence_create(lexer, leftParen + 1, sc1 - 1);
    if (asignation) {
        error e = asignation -> error;
        if (e.type){
            Data d = data_create(ERROR, &e);
            d -> dest = false;
            return d;
        }
    }
    
    list_add(list, data_create(SENTENCE, asignation));
    //condition
    
    Sentence condition = sentence_create(lexer, sc1 + 1, sc2-1);
    if (condition) {
        error e = condition -> error;
        if (e.type){
            Data d = data_create(ERROR, &e);
            d -> dest = false;
            return d;
        }
    }
    
    list_add(list, data_create(SENTENCE, condition));

    //increment or decrement
    
    Sentence increment = sentence_create(lexer, sc2 + 1, LPAREN - 1);
    if (increment) {
        error e = increment -> error;
        if (e.type){
            Data d = data_create(ERROR, &e);
            d -> dest = false;
            return d;
        }
    }
    
    list_add(list, data_create(SENTENCE, increment));
    
    return data_create(LIST, list);
}

typedef struct{
    int lparen, rparen, rbrace, lbrace, elseRbrace, elseLbrace;
}codeBlock;

codeBlock getCodeBlock(Lexer lexer, int a, int b){
    codeBlock c = {0,0,0,0,1,0};
    
    //get left parenthesis from condition
    a++;
    if (lexer_getToken(lexer, a) -> type != LPAREN) {
        c.lparen = -1;
        return c;
    }else{
        c.lparen = a;
    }
    
    //get right parenthesis from condition
    int i = a+1;
    int openedParen = 0;
    Token current;
    while ((void)(current = lexer_getToken(lexer, i)), (openedParen || current -> type != RPAREN) && i < b) {
        if (current -> type == LPAREN) {
            openedParen++;
        }else if(current -> type == RPAREN){
            openedParen--;
        }
        i++;
    }
    
    c.rparen = i;
    
    //get left brace
    while ((void)(current = lexer_getToken(lexer, i)), current-> type != LBRACE) {
        i++;
    }
    
    c.lbrace = i;
    
    //get right brace
    i++;
    int openedBrace = 0;
    while ((void)(current = lexer_getToken(lexer, i)), (openedBrace || current -> type != RBRACE) && i < b) {
        if (current -> type == LBRACE) {
            openedBrace++;
        }else if(current -> type == RBRACE){
            openedBrace--;
        }
        i++;
    }
    
    c.rbrace = i;
    
    if (i == b) {
        return c;
    }
    
    //get left brace else
    while ((void)(current = lexer_getToken(lexer, i)), current-> type != LBRACE) {
        i++;
    }
    
    c.elseLbrace = i;
    
    //get right brace else
    i++;
    openedBrace = 0;
    while ((void)(current = lexer_getToken(lexer, i)), (openedBrace || current -> type != RBRACE) && i < b) {
        if (current -> type == LBRACE) {
            openedBrace++;
        }else if(current -> type == RBRACE){
            openedBrace--;
        }
        i++;
    }
    
    c.elseRbrace = i;
    
    return c;
}

Sentence sentence_create(Lexer lexer, int a, int b){
    Sentence newSentece = (Sentence) malloc(sizeof(struct strSentence));
    newSentece -> error.message = (char *) malloc(100);
    newSentece -> error.type = NO_ERROR;
    
    //Ignore the last character if it is a end of line character
    if (lexer_getToken(lexer, b) -> type == END_LINE) {
        b--;
    }
    
    //If both of the borders are parenthesis, they should be ignored
    if (lexer_getToken(lexer, a) -> type == LPAREN && lexer_getToken(lexer, b) -> type == RPAREN) {
        a++;
        b--;
    }

    //non-recursive cases
    
    if (a == b) {
        newSentece -> value = data_makeCopy(lexer_getToken(lexer, a));
        newSentece -> rightSubsentence = NULL;
        newSentece -> leftSubsentence = NULL;
        return newSentece;
    }
    
    if (a > b) {
        free(newSentece -> error.message);
        free(newSentece);
        
        return NULL;
    }
    
    Token first = lexer_getToken(lexer, a);
    tokenType typeFirst = first -> type;
    
    if (typeFirst == IF) {
        //do all the validations to make an if-else statement and return errors
        newSentece -> value = data_makeCopy(first);
        
        codeBlock c = getCodeBlock(lexer, a, b);
        
        newSentece -> leftSubsentence = sentence_create(lexer, c.lparen, c.rparen);
        
        newSentece -> rightSubsentence = (Sentence) malloc(sizeof(struct strSentence));
        
        Sentence rs = newSentece -> rightSubsentence;
        
        rs -> value = data_create(ELSE, NULL);
        
        
        rs -> leftSubsentence = (Sentence) malloc(sizeof(struct strSentence));
        rs -> leftSubsentence -> leftSubsentence = NULL;
        rs -> leftSubsentence -> rightSubsentence = NULL;
        rs -> leftSubsentence -> value = data_create(FUNCTION, block_create(lexer, c.lbrace, c.rbrace, NULL));
        
        rs -> rightSubsentence = (Sentence) malloc(sizeof(struct strSentence));
        rs -> rightSubsentence -> leftSubsentence = NULL;
        rs -> rightSubsentence -> rightSubsentence = NULL;
        rs -> rightSubsentence -> value = data_create(FUNCTION, block_create(lexer, c.elseLbrace, c.elseRbrace, NULL));
        
        return newSentece;
        
    }else if (typeFirst == FOR){
        //do all the validations to make an for statement and return errors
        newSentece -> value = data_makeCopy(first);
        
        codeBlock c = getCodeBlock(lexer, a, b);
        
        newSentece -> leftSubsentence = (Sentence) malloc(sizeof(struct strSentence));
        
        int sc1 = 0, sc2 = 0;
        Token current;
        for (int i = c.lparen; i < c.rparen; i++) {
            current = lexer_getToken(lexer, i);
            if (!sc1) {
                if (current -> type == SEMICOLON) {
                    sc1 = i;
                }
            }else if (!sc2) {
                if (current -> type == SEMICOLON) {
                    sc2 = i;
                }
            }else{
                if (current -> type == SEMICOLON) {
                    //error in for
                    newSentece -> error.type = EXTRA_SEMICOLON_FOR;
                    strcpy(newSentece -> error.message, "Extra ';' in 'for' statement.\n");
                    return newSentece;
                }
            }
        }
        
        if(!sc2){
            //error in for
            newSentece -> error.type = EXTRA_SEMICOLON_FOR;
            strcpy(newSentece -> error.message, "Expected ';' in 'for' statement.\n");
            return newSentece;
        }
        
        newSentece -> leftSubsentence -> value = createForList(lexer, sc1, sc2, c.lparen, c.rparen);
        newSentece -> leftSubsentence -> leftSubsentence = NULL;
        newSentece -> leftSubsentence -> rightSubsentence = NULL;
        
        newSentece -> rightSubsentence = (Sentence) malloc(sizeof(struct strSentence));
        
        newSentece -> rightSubsentence -> value = data_create(FUNCTION, block_create(lexer, c.lbrace, c.rbrace, NULL));
        newSentece -> rightSubsentence -> leftSubsentence = NULL;
        newSentece -> rightSubsentence -> rightSubsentence = NULL;
        
        return newSentece;
        
    }else if(typeFirst == WHILE){
        //do all the validations to make an while statement and return errors
        newSentece -> value = data_makeCopy(first);
        
        codeBlock c = getCodeBlock(lexer, a, b);
        
        newSentece -> leftSubsentence = (Sentence) malloc(sizeof(struct strSentence));
        
        newSentece -> leftSubsentence -> value = data_create(SENTENCE, sentence_create(lexer, c.lparen, c.rparen));
        newSentece -> leftSubsentence -> leftSubsentence = NULL;
        newSentece -> leftSubsentence -> rightSubsentence = NULL;
        
        newSentece -> rightSubsentence = (Sentence) malloc(sizeof(struct strSentence));
        
        newSentece -> rightSubsentence -> value = data_create(FUNCTION, block_create(lexer, c.lbrace, c.rbrace, NULL));
        newSentece -> rightSubsentence -> leftSubsentence = NULL;
        newSentece -> rightSubsentence -> rightSubsentence = NULL;
        
        return newSentece;
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
    
    newSentece -> value = data_makeCopy(leastPriorityData);
    
    //create a function sentence
    if (leastPriorityData -> type == NAME){
        if (a == parent && lexer_getToken(lexer, parent + 1) -> type == LPAREN && lexer_getToken(lexer, b) -> type == RPAREN) {
            Sentence argumets = (Sentence) malloc(sizeof(struct strSentence));
            
            argumets -> value = createArgumentList(lexer, parent + 1, b);
            if (argumets -> value -> type == ERROR) {
                error e = *(error*) argumets -> value -> value;
                newSentece -> error.type = e.type;
                strcpy(newSentece -> error.message, e.message);
                data_destroy(argumets -> value);
                return newSentece;
            }
            
            argumets -> rightSubsentence = NULL;
            argumets -> leftSubsentence = NULL;
            
            newSentece -> rightSubsentence = argumets;
            newSentece -> leftSubsentence = NULL;
            return newSentece;
        }else{
            newSentece -> error.type = INVALID_OPERATION;
            strcpy(newSentece -> error.message, "Invalid operation.\n");
            return newSentece;
        }
    }
    
    switch (leastPriorityData -> type) {
        case INT:
        case FLOAT:
        case FALSE:
        case TRUE:
        case STRING:
            newSentece -> error.type = INVALID_OPERATION;
            strcpy(newSentece -> error.message, "Invalid operation.\n");
            break;
        default:
            break;
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
    
    data_sentenceDataDestroy(sentence -> value);
    if (!sentence -> error.message) {
        free(sentence -> error.message);
        sentence -> error.message = NULL;
    }
    
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

Data sentence_getValue(Sentence sentence){
    return sentence ?  sentence -> value : NULL;
}

error sentence_getErrorStatus(Sentence sentence){
    error e = {SENTENCE_DOESNT_EXIST, NULL};
    return sentence ? sentence -> error : e;
}
