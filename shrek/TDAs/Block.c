//
//  Block.c
//  shrek
//
//

#include "Block.h"

struct strBlock {
    Sentence * sentences;
    List params;
    int size, cap;
    error error;
    Data name;
};

Block block_create(Lexer lexer, int leftBrace, int rightBrace, Data name){
    Block newBlock = (Block) calloc(1, sizeof(struct strBlock));
    
    newBlock -> cap = 10;
    newBlock -> sentences = (Sentence *) malloc(sizeof(Sentence) * newBlock -> cap);
    newBlock -> error.type = NO_ERROR;
    newBlock -> error.message = (char *) malloc(100);
    
    int i = leftBrace + 1, j, openedBraces = 0;
    
    //get to the first token that is not a end of line, ather the initial '{'
    while (lexer_getToken(lexer, i) -> type == END_LINE){
        i++;
    }
    
    if (i >= rightBrace) {
        newBlock -> error.type = EMPTY_FUNCTION;
        strcpy(newBlock -> error.message, "This function is empty.\n");
        data_print(name);
        return newBlock;
    }
    
    j = i + 1;
    
    tokenType begining, current, next;
    
    while (true) {
        
        if (newBlock -> size == newBlock -> cap) {
            newBlock -> cap *= 2;
            newBlock -> sentences = (Sentence *) realloc(newBlock -> sentences, newBlock -> cap);
        }
        
        begining = lexer_getToken(lexer, i) -> type;
        current = lexer_getToken(lexer, j) -> type;
        next = lexer_getToken(lexer, j+1) -> type;
        
        if (current == LBRACE) {
            openedBraces++;
        }else if(current == RBRACE){
            openedBraces--;
        }
        
        if ((((begining == IF && next != ELSE) || begining == WHILE || begining == FOR) //if it is an if, for or while
            && current == RBRACE && openedBraces == 0)// and its braces have been closed
            //of if it is not and this token is an end of line
            || (current == END_LINE && begining != IF && begining != WHILE && begining != FOR)) {
            
            Sentence newSentence = sentence_create(lexer, i, j);
            newBlock -> sentences[newBlock -> size] = newSentence;
            error nsError = sentence_getErrorStatus(newSentence);
            if (nsError.type != NO_ERROR) {
                newBlock -> error.type = nsError.type;
                newBlock -> error.message = nsError.message;
                return newBlock;
            }
            
            newBlock -> size++;
            
            i = j;
            while ((void)i++,lexer_getToken(lexer, i) -> type == END_LINE){}
            
            j = i;
        }
        j++;
        
        if (i >= rightBrace) {
            return newBlock;
        }
    }
}

void block_destroy(Block block){
    if (!block) {
        return;
    }
    
    for (int i = 0; i < block -> size; i++) {
        sentence_destroy(block -> sentences[i]);
    }
    
    free(block -> sentences);
    
    if (block -> params) {
        list_destroy(block -> params);
    }
    
    free(block -> error.message);
    free(block);
}

int block_size(Block block){
    return block ? block -> size : 0;
}

error block_getErrorStatus(Block block){
    error e = {BLOCK_DOESNT_EXIST, NULL};
    return block ? block -> error : e;
}

Sentence block_getSentence(Block block, int pos){
    bool validPos = block && 0 <= pos && pos < block -> size;
    return validPos ? (block -> sentences[pos]) : NULL;
}

void block_print(Block block){
    if (!block || !block -> name) {
        return;
    }
    data_print(block -> name);
    printf("()");
}

List block_getParams(Block block){
    return block ? block -> params : NULL;
}

Data block_name(Block block){
    return block ?  block -> name : NULL;
}

void block_setParams(Block block, List params){
    if (!block) {
        return;
    }
    
    block -> params = params;
}


