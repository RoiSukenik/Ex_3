#include "Queue.h"

Queue* InitializeQueue(void) 
{
    Queue* que = NULL;
    que = (Queue*)malloc(sizeof(Queue));
    if (que == NULL) {
        printf("The pointer was not allocated correctly");
        return NULL;
    }
    que->count = 0;
    que->first = NULL;
    que->last = NULL;
    return que;
}

bool Empty(Queue* que) {
    return (que->count == 0);
}

int Push(Queue* que, int value) 
{
    q_node* tmp_node;
    tmp_node = (q_node*)malloc(sizeof(q_node));
    if (tmp_node == NULL) {
        printf("The pointer was not allocated correctly");
        return STATUS_CODE_FAILURE;
    }
    tmp_node->data = value;
    tmp_node->next = NULL;
    if (!Empty(que))
    {
        que->last->next = tmp_node;
        que->last = tmp_node;
    }
    else
    {
        que->first = tmp_node;
        que->last = tmp_node;
    }
    que->count++;
}

int Pop(Queue* que)
{
    q_node* tmp_node;
    int value = que->first->data;
    tmp_node = que->first;
    que->first = que->first->next;
    que->count--;
    free(tmp_node);
    return(value);
}
int Top(Queue* que) {
    return(que->first->data);
}
Queue* DestroyQueue(Queue* que) 
{
    q_node* temp_node;
    while (!Empty(que)) { int tmp_value = Pop(que); }
        free(que);
        return STATUS_CODE_SUCCESS;
}