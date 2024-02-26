#include <stdio.h>
#include "queue.h"

Queue Queue_Init(void){
    Queue queue;
    Request initializerReq = Request_Init(1, false, false);
    queue.head = &initializerReq;
    queue.numberOfNodes = 1;

    printf("Queue initialized\n");
    return queue;
}

void Attach_Request_To_Queue(Request *request, Queue *queue){
    if (queue->numberOfNodes >= MAX_QUEUE_NODE_AMOUNT) {
        printf("Cannot attach Request because the Queue has %d elements", queue->numberOfNodes);
        return;
    }
    Request prevRequest = After_Which_Request_To_Attach_Request(&request, &queue);
    Attach_After_PrevRequest(&prevRequest, &request, &queue);
}

Request After_Which_Request_To_Attach_Request(Request *request, Queue *queue){
    
}

void Attach_After_PrevRequest(Request *prevRequest, Request *requestToAttach, Queue *queue){
    if (prevRequest == NULL) {
        printf("prevRequest cannot be NULL");
        return;
    }
    requestToAttach->pNextRequest = prevRequest->pNextRequest;
    prevRequest->pNextRequest = requestToAttach;
    queue->numberOfNodes++;
}