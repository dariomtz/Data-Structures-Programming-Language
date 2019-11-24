//
//  Block.c
//  shrek
//
//

#include "Block.h"

struct strBlock {
    Sentence * sentences;
    int size, cap;
    error error;
};

Block block_create(Lexer lexer, int a, int b){
    Block newBlock = (Block) calloc(1, sizeof(struct strBlock));
    
    newBlock -> cap = 10;
    newBlock -> sentences = (Sentence *) malloc(sizeof(Sentence) * newBlock -> cap);
    newBlock -> error.type = NO_ERROR;
    newBlock -> error.message = (char *) malloc(100);
    
    int i = a+1, j= i + 1, openedBraces = 0; //first token is  { or BEGIN_MAIN so skip it;
    tokenType begining, current, next;
    
    while (true) {
        
        if (newBlock -> size == newBlock -> cap) {
            newBlock -> cap = newBlock -> cap *2;
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
        
        if (((begining == IF && next != ELSE) || begining == WHILE || begining == FOR) && current == RBRACE && openedBraces == 0) {
            
            Sentence newSentence = sentence_create(lexer, i, j);
            newBlock -> sentences[newBlock -> size] = newSentence;
            error nsError = sentence_getErrorStatus(newSentence);
            if (nsError.type != NO_ERROR) {
                newBlock -> error.type = nsError.type;
                newBlock -> error.message = nsError.message;
                return newBlock;
            }
            
            newBlock -> size++;
            
            i = j + 1;
            j = i;
            
        }else if (current == END_LINE && begining != IF && begining != WHILE && begining != FOR) {
            
            Sentence newSentence = sentence_create(lexer, i, j + 1);
            newBlock -> sentences[newBlock -> size] = newSentence;
            error nsError = sentence_getErrorStatus(newSentence);
            if (nsError.type != NO_ERROR) {
                newBlock -> error.type = nsError.type;
                newBlock -> error.message = nsError.message;
                return newBlock;
            }

            newBlock -> size++;
            
            i = j + 1;
            j = i;
        }
        
        j++;
        
        if (i == b) {
            if (j != i+1) {
                if(begining == IF){
                    newBlock -> error.type = NO_RBRACE_FOR_IF;
                    strcpy(newBlock -> error.message, "Missing '}' for IF statement\n");
                }else if (begining == WHILE){
                    newBlock -> error.type = NO_RBRACE_FOR_WHILE;
                    strcpy(newBlock -> error.message, "Missing '}' for WHILE loop\n");
                }else if (begining == FOR){
                    newBlock -> error.type = NO_RBRACE_FOR_FOR;
                    strcpy(newBlock -> error.message, "Missing '}' for FOR loop\n");
                }else{
                    newBlock -> error.type = NO_END_LINE_FOR_SENTENCE;
                    strcpy(newBlock -> error.message, "Missing '\n' for  statement\n");
                }
            }
            
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
    bool validPos = block && 0 <= pos && pos <= block -> size;
    return validPos ? (block -> sentences[pos]) : NULL;
}



