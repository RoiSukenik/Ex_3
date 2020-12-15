#ifndef		QUEUE_DOT_H
#define		QUEUE_DOT_H

#include	<stdbool.h>
#include	<string.h>
static const int STATUS_CODE_SUCCESS = 0;
static const int STATUS_CODE_FAILURE = -1;


// inspired by codesdope blog
struct q_node
{
	int data;
	struct q_node* next;
};
typedef struct q_node q_node;

struct Queue
{
    int count;
    q_node* front;
    q_node* rear;
};
typedef struct Queue Queue;

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
int Top(Queue* queue);

/*
* Description - Pop the first object in a queue
* Parameters - Pointer to a Queue type object
* Returns - int of the poped item, poped node is freed , next elemnt is now top of queue
*/
int Pop(Queue* queue);

/*
* Description - Push an element to the end of the queue
* Parameters - Pointer to a Queue type object and an integer data object
* Returns - void, nothing
*/
void Push(Queue* queue, int value);


/*
* Description - Checks if queue is empty
* Parameters - Pointer to a Queue type object
* Returns - True if queue is empty | False if queue is not empty
*/
bool Empty(Queue* queue);

/*
* Description - Destroys Queue
* Parameters - Pointer to a Queue type object
* Returns - Free all queue memory resources, set pointer to NULL
*/
Queue *DestroyQueue(Queue* queue);

#endif // !QUEUE_DOT_H



