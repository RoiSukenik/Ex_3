#ifndef		QUEUE_DOT_H
#define		QUEUE_DOT_H

#include "includes.h"

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCCESS 0


// inspired by codesdope blog
typedef struct _q_node
{
	int data;
	struct q_node* next;
}q_node;


typedef struct _Queue
{
    int count;
    q_node* first;
    q_node* last;
}Queue;


/*
* Description - Initialize Queue, Based on Queue File
* Parameters - None
* Returns - Pointer to a Queue struct
*/
Queue *InitializeQueue(void);

/*
* Description - Returns the top value of the queue
* Parameters - Pointer to a Queue type object
* Returns - int which is at the top the given queue
*/
int Top(Queue* que);

/*
* Description - Pop the first object in a queue
* Parameters - Pointer to a Queue type object
* Returns - int of the poped item, poped node is freed , next elemnt is now top of queue
*/
int Pop(Queue* que);

/*
* Description - Push an element to the end of the queue
* Parameters - Pointer to a Queue type object and an integer data object
* Returns - void, nothing
*/
int Push(Queue* que, int value);


/*
* Description - Checks if queue is empty
* Parameters - Pointer to a Queue type object
* Returns - True if queue is empty | False if queue is not empty
*/
bool Empty(Queue* que);

/*
* Description - Destroys Queue
* Parameters - Pointer to a Queue type object
* Returns - Free all queue memory resources, set pointer to NULL
*/
Queue *DestroyQueue(Queue* que);

#endif // !QUEUE_DOT_H



