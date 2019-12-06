//
//  Map.h
//  shrek
//
//

#ifndef Map_h
#define Map_h

#include "stdhead.h"

typedef struct strMap * Map;

Map map_create(Data name, int cap);
void map_print(Map map);
char * map_name(Map map);
void map_destroy(Map map);
int  map_size(Map map);
void map_put(Map map, Data key, Data v);
Data map_get(Map map, Data key);
void map_remove(Map map, Data key);

#endif /* Map_h */
