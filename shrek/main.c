//
//  main.c
//  shrek
//
/*
    This code uses the other libraries to execute the code un a file with extension .shrek
*/

#include "Runtime.h"
int validate(char* fileName){
	int count=0;
	while(fileName[count]!= '.'){
		count++;
	}
	count++;
	int i=0;
	char shrek[]="shrek";
	while(shrek[i]==fileName[count]){
		i++;
		count++;
		if(shrek[i]=='\0' || fileName[count] == '\0'){
			break;
		}
	}
	if(strlen(shrek) != i || strlen(fileName) != count){
		return 0;
	}
	else{
		return 1;
	}
}
int main() {
   	char fileName[50];
    printf("Escribe el nombre de tu archivo: \n");
    scanf("%s",fileName);
    printf("WTF");
    system("pause");
    if(!validate(fileName)){
    	printf("FILE ERROR: invalid file");
    	return 0;
    }
    FILE * f = fopen(fileName,"r");
    printf("Se leyo");
    system("pause");
    Lexer newLex = lexer_create(f);
    printf("LEXER");
    Parser newParser = parser_create(newLex);
    printf("PARSER");
    RunTime(newParser);
    printf("RUNTIME");
    parser_destroy(newParser);
    lexer_destroy(newLex);
    system("pause");
    return 0;


}
