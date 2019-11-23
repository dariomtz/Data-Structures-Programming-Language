//
//  Queue.h
//  shrek
//
//

#ifndef Queue_h
#define Queue_h

#include "stdhead.h"

typedef struct strQueue * Queue;

Queue queue_create(Data name);
void queue_destroy(Queue queue);
int queue_size(Queue queue);
bool queue_isEmpty(Queue queue);
void queue_offer(Queue queue, Data data);
Data queue_peek(Queue queue);
Data queue_poll(Queue queue);


#endif /* Queue_h */
