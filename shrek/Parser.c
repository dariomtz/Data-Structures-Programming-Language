//
//  Parser.c
//  shrek
//
/*
    Implementation of Parser struct and its functions
*/

#include "Parser.h"

struct strParser {
    Block main;
    Map functions;
    error error;
};

Parser parser_create(Lexer lexer){
    Parser newParser = (Parser) calloc(1, sizeof(struct strParser));
    
    newParser -> error.message = (char *) malloc(100);
    
    if (lexer_numberOfTokens(lexer) == 0) {
        newParser -> error.type = EMPTY_FILE;
        strcpy(newParser -> error.message, "The given file is empty\n");
        return newParser;
    }
    
    Token lastToken = lexer_getToken(lexer, lexer_numberOfTokens(lexer) - 1);
    
    if (lastToken -> type == ERROR) {
        newParser -> error.type = UNREGONIZED_TOKEN;
        strcpy(newParser -> error.message, "Unrecognized symbol: ");
        strcat(newParser -> error.message, lastToken -> value);
        strcat(newParser -> error.message, "\n");
        return newParser;
    }
    
    newParser -> error .type = NO_ERROR;
    
    
    int numOfFunctions = 0, beginMain = -1, endMain = -1;
    
    for (int i = 0; i < lexer_numberOfTokens(lexer); i++) {
        if (lexer_getToken(lexer, i) -> type == FUNCTION) {
            numOfFunctions++;
        }else if (lexer_getToken(lexer, i) -> type == BEGIN_MAIN){
            beginMain = i;
        }else if (lexer_getToken(lexer, i) -> type == END_MAIN){
            endMain = i;
        }
    }
    
    if (beginMain == -1 || endMain == -1) {
        return NULL;
    }
    
    char * name = malloc(sizeof(char)*10);
    char name2[] = "functions";
    for (int i = 0; i < 10; i++) {
        name[i] = name2[i];
    }
    
    newParser -> functions = map_create(data_create(STRING, name), numOfFunctions * 2);

    for (int i = 0; i < lexer_numberOfTokens(lexer); i++) {
        
        if (lexer_getToken(lexer, i) -> type == FUNCTION) {
            
            Token nameToken = lexer_getToken(lexer, ++i);
            if(nameToken -> type != NAME){
                newParser -> error.type = NO_NAME_AFTER_FUNC;
                strcpy(newParser -> error.message, "Expected NAME after FUNCTION\n");
                return newParser;
            }
            
            Data funcName = data_create(STRING, nameToken -> value);
            
            int leftBrace = i++;
            if (lexer_getToken(lexer, leftBrace) -> type != LBRACE) {
                newParser -> error.type = NO_LBRACE_AFTER_FUNC_NAME;
                strcpy(newParser -> error.message, "Expected LEFT BRACE after FUNCTION NAME\n");
                return newParser;
            }
            
            int openedBraces = 0;
            while (true) {
                if (lexer_getToken(lexer, i) -> type == LBRACE) {
                    openedBraces++;
                }else if (lexer_getToken(lexer, i) -> type == RBRACE){
                    openedBraces--;
                    if (openedBraces == 0) {
                        break;
                    }
                }
                i++;
                
                if (!lexer_getToken(lexer, i)) {
                    newParser -> error.type = NO_RBRACE_FOR_FUNC;
                    strcpy(newParser -> error.message, "Missing closing brace for function");
                    strcat(newParser -> error.message, funcName -> value);
                    strcat(newParser -> error.message, "\n");
                    return newParser;
                }
            }
            
            
            Block block = block_create(lexer, leftBrace, i);
            error bError = block_getErrorStatus(block);
            if (bError.type != NO_ERROR) {
                newParser -> error.type = bError.type;
                newParser -> error.message = bError.message;
                return newParser;
            }
            
            Data func = data_create(FUNCTION, block);
            map_put(newParser -> functions, funcName, func);
        }
    }
    
    newParser -> main = block_create(lexer, beginMain, endMain);
    error mError = block_getErrorStatus(newParser -> main);
    if (mError.type != NO_ERROR) {
        newParser -> error.type = mError.type;
        newParser -> error.message = mError.message;
        return newParser;
    }
    
    return newParser;
}

void parser_destroy(Parser parser){
    if (!parser) {
        return;
    }
    
    map_destroy(parser -> functions);
    block_destroy(parser -> main);
    free(parser -> error.message);
    free(parser);
}

Block parser_getMain(Parser parser){
    return parser ? parser -> main : NULL;
}

Block parser_getFunction(Parser parser, Data name){
    return parser ? (map_get(parser -> functions, name) ? map_get(parser -> functions, name) -> value : NULL) : NULL;
}

error parser_getErrorStatus(Parser parser){
    error e = {PARSER_DOESNT_EXIST, NULL};
    return parser ? parser -> error : e;
}









