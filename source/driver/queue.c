#include <stdio.h>
#include "queue.h"
#include "request.h"

Queue Queue_Init(Request *head, Request *tail){
    Queue queue;
    
    queue.head = head;
    queue.tail = tail;

    head->pNextRequest = (&queue)->tail;
    head->pPrevRequest = NULL;
    tail->pNextRequest = NULL;
    tail->pPrevRequest = (&queue)->head;

    queue.numberOfNodes = 2;

    printf("Queue initialized\n\n");
    return queue;
}

void Attach_Request_To_Queue(Request *request, Queue *queue, int mCurrentFloor){
    bool attachBefore = true;
    if (queue->numberOfNodes >= MAX_QUEUE_NODE_AMOUNT) {
        printf("Cannot attach Request because the Queue has %d elements.\n\n", queue->numberOfNodes);
        return;
    }
    if (Request_Already_Exists_In_Queue(&request, &queue)) {
        printf("Won't attach Request because it already exists in Queue.\n\n");
        return;
    }
    Request* pThis = Where_To_Attach_Request(&request, &queue, mCurrentFloor, &attachBefore);
    if (attachBefore) {
        Attach_Before_This(pThis, &request, &queue);
    } else {
        Attach_After_This(pThis, &request, &queue);
    }
}

bool Request_Already_Exists_In_Queue(Request *request, Queue *queue){
    for (Request *iteratorNode = queue->head; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
        if (iteratorNode->floor == request->floor && iteratorNode->direction == request->direction) {
            return true;
        }
    }
    return false;
}

Request* Where_To_Attach_Request(Request *request, Queue *queue, int mCurrentFloor, bool *attachBefore){
    /*int volatile *currentFloor = elevio_floorSensor(); // ? Can you constantly detect and change currentFloor? */
    MotorDirection elevatorDirn;
    if (request->floor - mCurrentFloor > 0) {
        elevatorDirn = DIRN_UP;
    } else if (request->floor - mCurrentFloor < 0) {
        elevatorDirn = DIRN_DOWN;
    } // possible to flag off here, actually no cuz elevatorDirn should either be up or down
    switch (request->direction)
    {
    case DIRN_STOP: // Request from inside the elevator. If there is a request from outside, which is in your path and is in the same direction add after that
        for (Request *iteratorNode = queue->head; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
            bool requestInElevatorsWay = false;
            if ((request->floor < iteratorNode->floor && request->floor > mCurrentFloor)
                || (request->floor > iteratorNode->floor && request->floor < mCurrentFloor)) {
                requestInElevatorsWay = true;
            } // check again
            if (request->direction == elevatorDirn && requestInElevatorsWay) {
                return iteratorNode;
            }
            if (iteratorNode->pNextRequest == NULL) {
                *attachBefore = false;
                return iteratorNode;
            }
        }
        break;
    case DIRN_UP: // Upwards request from outside the elevator. Oldest prioritized. Exeption: if the request from outside is in your path and is in the same direction
        for (Request *iteratorNode = queue->head; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
            bool requestInElevatorsWay = false;
            if ((request->floor < iteratorNode->floor && request->floor > mCurrentFloor)
                || (request->floor > iteratorNode->floor && request->floor < mCurrentFloor)) {
                requestInElevatorsWay = true;
            }
            if (elevatorDirn == DIRN_UP && request->direction == DIRN_UP && requestInElevatorsWay) {
                return iteratorNode;
            }
            if (iteratorNode->pNextRequest == NULL) {
                *attachBefore = false;
                return iteratorNode;
            }
        }
        break;
    case DIRN_DOWN: // Downwards request from outside the elevator. Oldest prioritized. Exeption: if the request from outside is in your path and is in the same direction
        for (Request *iteratorNode = queue->head; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
            bool requestInElevatorsWay = false;
            if ((request->floor < iteratorNode->floor && request->floor > mCurrentFloor)
                || (request->floor > iteratorNode->floor && request->floor < mCurrentFloor)) {
                requestInElevatorsWay = true;
            }
            if (elevatorDirn == DIRN_DOWN && request->direction == DIRN_DOWN && requestInElevatorsWay) {
                return iteratorNode;
            }
            if (iteratorNode->pNextRequest == NULL) {
                *attachBefore = false;
                return iteratorNode;
            }
        }
        break;
    default:
        break;
    }
}

void Attach_Before_This(Request *this, Request *requestToAttach, Queue *queue){
    if (this == queue->head) {
        printf("Cannot attach before queue->head!\n\n");
        return;
    }
    this->pPrevRequest->pNextRequest = requestToAttach;
    requestToAttach->pPrevRequest = this->pPrevRequest;
    this->pPrevRequest = requestToAttach;
    requestToAttach->pNextRequest = this;
    (queue->numberOfNodes)++;
}

void Attach_After_This(Request *this, Request *requestToAttach, Queue *queue){
    if (this == NULL) {
        printf("Cannot attach after NULL!\n\n");
        return;
    }
    this->pNextRequest->pPrevRequest = requestToAttach;
    requestToAttach->pNextRequest = this->pNextRequest;
    this->pNextRequest = requestToAttach;
    requestToAttach->pPrevRequest = this;
    (queue->numberOfNodes)++;
}

void Delete_From_Queue(Request *request, Queue *queue){
    if (queue->head == NULL || queue->tail == NULL || queue->numberOfNodes < 2) {
        printf("There are not enough elements in Queue. Cannot apply Delete_From_Queue().");
    }
    bool foundRequest = false;
    for (Request *iteratorNode = queue->head; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
        if (iteratorNode->floor == request->floor && iteratorNode->direction == request->direction) {
            Request *temp = iteratorNode;
            if (iteratorNode->pNextRequest != NULL) {
                Request *tempNext = iteratorNode->pNextRequest;
                iteratorNode->pPrevRequest->pNextRequest = iteratorNode->pNextRequest;
                tempNext = NULL;
            }
            if (iteratorNode->pPrevRequest != NULL) {
                Request *tempPrev = iteratorNode->pPrevRequest;
                iteratorNode->pNextRequest->pPrevRequest = iteratorNode->pPrevRequest;
                tempPrev = NULL;
            }
            foundRequest = true;
            break;
        }
    }
    if (foundRequest) {
        (queue->numberOfNodes)--;
        printf("Request deleted.\n\n");
    } else {
        printf("Request not in queue. Therefore cannot delete request!\n\n");
    }
}

void Queue_Print(Queue *pQueue){
    int counter = 0;
    for (Request *i = pQueue->head; i != NULL; i = i->pNextRequest) {
        printf("%d. request:\nFloor: %d\nDirection: %d\nOff: %d\nNext node adress: %d\nPrev node adress: %d\n\n", counter, i->floor, i->direction, i->off, i->pNextRequest, i->pPrevRequest);
        counter++;
    }
    printf("There are %d request(s) in Queue\n\n", pQueue->numberOfNodes);
}