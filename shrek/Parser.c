//
//  Parser.c
//  shrek
//
/*
    Implementation of Parser struct and its functions
*/

#include "Parser.h"

struct strParser {
    Map memory;
    error error;
};

List createParamList(Lexer lexer, int leftParen, int rightParen){
    List params = list_create(NULL);
    
    bool coma = false;
    Token current;
    
    for (int i = leftParen + 1; i < rightParen; i++) {
        current = lexer_getToken(lexer, i);
        if (coma && current -> type != COMA) {
            list_destroy(params);
            return NULL;
        }else{
            if (current -> type != NAME) {
                list_destroy(params);
                return NULL;
            }
            
            list_add(params, data_makeCopy(current));
        }
        coma = !coma;
    }
    
    return params;
}

Parser parser_create(Lexer lexer){
    Parser newParser = (Parser) calloc(1, sizeof(struct strParser));
    
    newParser -> error.message = (char *) malloc(100);
    
    if (!lexer) {
        newParser -> error.type = FILE_DOESNT_EXIST;
        strcpy(newParser -> error.message, "The given file doesn't exist.\n");
        return newParser;
    }
    
    if (lexer_numberOfTokens(lexer) == 0) {
        newParser -> error.type = EMPTY_FILE;
        strcpy(newParser -> error.message, "The given file is empty.\n");
        return newParser;
    }
    
    Token lastToken = lexer_getToken(lexer, lexer_numberOfTokens(lexer) - 1);
    
    if (lastToken -> type == ERROR) {
        newParser -> error.type = UNREGONIZED_TOKEN;
        strcpy(newParser -> error.message, "Unrecognized symbol: ");
        strcat(newParser -> error.message, lastToken -> value);
        strcat(newParser -> error.message, ".\n");
        return newParser;
    }
    
    newParser -> error.type = NO_ERROR;
    
    newParser -> memory = map_create(NULL, MEMORY_CAPACITY);
    
    int i = 0, j = -1, opened;
    Token current, name;
    
    while (true) {
        current= lexer_getToken(lexer, ++j);
        if (current -> type == END_LINE) {
            continue;
        }else if (current -> type == FUNCTION){
            
            j++;
            name = lexer_getToken(lexer, j);
            if (name -> type != NAME) {
                //error, func declaration must be followed by a name
                newParser -> error.type = NO_NAME_AFTER_FUNC;
                strcpy(newParser -> error.message, "No name for function.\n");
                return newParser;
            }
            
            if (map_get(newParser -> memory, name)){
                //error, a function has already been defined with that name
                newParser -> error.type = REPEATED_NAME_FOR_FUNCTION;
                strcpy(newParser -> error.message, "A function has already been defined with that name.\n");
                return newParser;
            }
            
            j++;
            i = j;
            
            if (lexer_getToken(lexer, j) -> type != LPAREN) {
                //error, espected left parentesis after function name in declaration
                newParser -> error.type = NO_LPAREN_FOR_PARAMS;
                strcpy(newParser -> error.message, "Expected '(' after function name.\n");
                return newParser;
            }
            
            opened = 0;
            
            while (true) {
                current = lexer_getToken(lexer, ++j);
                if (!current) {
                    break;
                }else if (current -> type == LPAREN){
                    opened++;
                }else if (current -> type == RPAREN){
                    opened--;
                    if (opened == -1) {
                        break;
                    }
                }
            }
            
            if (!current) {
                //error, no closing parentesis for function params
                newParser -> error.type = NO_RPAREN_FOR_PARAMS;
                strcpy(newParser -> error.message, "Expected ')' to close params.\n");
                return newParser;
            }
            
            List paramList = createParamList(lexer, i, j);
            
            if (!paramList) {
                //error, invalid list of parameters for function:"name"
                newParser -> error.type = INVALID_PARAMS;
                strcpy(newParser -> error.message, "Invalid list of params. Each parameter must be a name and be separated with ','.\n");
                return newParser;
            }
            
            while ((void)(current = lexer_getToken(lexer, ++j)),current && current -> type == END_LINE) {}
            
            if (!current || current -> type != LBRACE) {
                //error, expected opening brace after closing param list
                newParser -> error.type = NO_LBRACE_FOR_FUNC;
                strcpy(newParser -> error.message, "Function declaration incomplete. Expected '{' after list of parameters.\n");
                return newParser;
            }
            
            i = j;
            opened = 0;
            
            while (true) {
                current = lexer_getToken(lexer, ++j);
                if (!current) {
                    break;
                }else if (current -> type == LBRACE){
                    opened++;
                }else if (current -> type == RBRACE){
                    opened--;
                    if (opened == -1) {
                        break;
                    }
                }
            }
            
            if (!current) {
                //error, expected closing brace for function definition.
                newParser -> error.type = NO_RBRACE_FOR_FUNC;
                strcpy(newParser -> error.message, "Function declaration incomplete. Expected '}' to close the function code.\n");
                return newParser;
            }
            
            Block func = block_create(lexer, i, j, data_makeCopy(name));
            
            error funcErrorStatus = block_getErrorStatus(func);
            if (funcErrorStatus.type) {
                //get the error in the block and put it in myself to return it
                newParser -> error.type = funcErrorStatus.type;
                strcpy(newParser -> error.message, funcErrorStatus.message);
                return newParser;
            }
            
            block_setParams(func, paramList);
            
            map_put(newParser -> memory, data_makeCopy(name), data_create(FUNCTION, func));
            
        }else{
            newParser -> error.type = TOKEN_OUTSIDE_MAIN;
            strcpy(newParser -> error.message, "Token outside function: every token must be inside a function or inside main.\n");
            return newParser;
        }
    }
    
    return newParser;
}

void parser_destroy(Parser parser){
    if (!parser) {
        return;
    }
    free(parser->error.message);
    map_destroy(parser -> memory);
    free(parser);
}

Map parser_getMemoryMap(Parser parser){
    return parser ? parser -> memory : NULL;
}

error parser_getErrorStatus(Parser parser){
    error e = {PARSER_DOESNT_EXIST, NULL};
    return parser ? parser -> error : e;
}









