/**
 * @file
 * @brief queueNode module, used to define the queue struct and functionality.
*/

#pragma once
#include "request.h"
#define MAX_QUEUE_NODE_AMOUNT 10

/**
 * @brief Queue class, used to keep the request in order.
 * @param head, the begginning of the linked list class Queue
 * @param numberOfNodes, number of requests in Queue
*/
// Queue can max have 10 nodes
// Queue is 0 indexed
typedef struct {
    Request *head;
    int numberOfNodes;
} Queue;

Queue Queue_Init(void);

void Attach_Request_To_Queue(Request *request, Queue *queue);

int After_Which_Request_To_Attach_Request(Request *request, Queue *queue);

void Attach_After_PrevRequest(Request *prevRequest, Request *requestToAttach, Queue *queue);

void Delete_From_Queue(Request *request, Queue *queue);