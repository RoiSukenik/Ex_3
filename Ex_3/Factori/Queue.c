#include "Queue.h"

Queue* InitializeQueue(void) 
{
    Queue* que = NULL;
    Queue* que = (Queue*)malloc(sizeof(Queue));
    if (que == NULL) {
        printf("The pointer was not allocated correctly");
        exit(STATUS_CODE_FAILURE);
    }
    que->count = 0;
    que->front = NULL;
    que->rear = NULL;
}

bool Empty(Queue* que) {
    return (que->rear == NULL);
}

void Push(Queue* que, int value) 
{
    q_node* tmp_node;
    tmp_node = (q_node*)malloc(sizeof(q_node));
    tmp_node->data = value;
    tmp_node->next = NULL;
    if (!Empty(que))
    {
        que->rear->next = tmp_node;
        que->rear = tmp_node;
    }
    else
    {
        que->front = que->rear = tmp_node;
    }
    que->count++;
}

int Pop(Queue* que)
{
    q_node* tmp_node;
    int value = que->front->data;
    tmp_node = que->front;
    que->front = que->front->next;
    que->count--;
    free(tmp_node);
    return(value);
}
int Top(Queue* que) {
    return(que->front->data);
}
Queue* DestroyQueue(Queue* que) 
{
    q_node* temp_node;
        while (!Empty(que)) { Pop(que); }
        free(que);
        return STATUS_CODE_SUCCESS;
}