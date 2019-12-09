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
    //strcpy(fileName, "hola.shrek");
    
    printf("Write the name of your file: \n");
    scanf("%s",fileName);

    if(!validate(fileName)){
        printf("FILE ERROR: invalid type of file.\n");
        return 0;
    }
    FILE * f = fopen(fileName,"r");
    
    Lexer newLex = lexer_create(f);
    //lexer_print(newLex);
    
    fclose(f);
    
    Parser newParser = parser_create(newLex);

    RunTime(newParser);
    
    lexer_destroy(newLex);
    parser_destroy(newParser);
    

    return 0;


}
