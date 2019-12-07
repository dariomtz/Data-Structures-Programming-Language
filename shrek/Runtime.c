
//
//  Runtime.c
//  shrek
//
//  Created by DarÃ­o MartÃ­nez on 11/22/19.
//  Copyright Â© 2019 DarÃ­o MartÃ­nez. All rights reserved.
//

#include "Runtime.h"

Data resolve_sentence(Sentence s);

void RunTime(Parser parser){
	error error = parser_getErrorStatus(parser);
	if(error.type){
		if(error.type<=99)
			printf("LEXER ERROR: ");
		else
			printf("PARSER ERROR: ");
		printf("%s",error.message);
		return;
	}
	Map map_functions = parser_getMemoryMap(parser);
	Data main_data =(Data)malloc(sizeof(struct strData));
	main_data->type = NAME;
	
	//Here, the name of the main can change in the future
	strcpy(main_data->value, "main");
	//-------------------------------------------------------------------------------------//
	
	Block block = map_get(map_functions, main_data);
	int block_size = block_size(block);
	Sentence s;
	Data result_data;
	for(int i = 0; i<block_size; i++){
		s = block_getSentence(block,i);
		result_data = resolve_sentence(s);
		if(result_data->type == ERROR){
			error = result_data->value;
			return;
		}
	}
};
