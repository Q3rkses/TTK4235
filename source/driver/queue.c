#include <stdio.h>
#include "queue.h"

Queue Queue_Init(void){
    Queue queue;
    Request initializerReq = Request_Init(1, DIRN_STOP, false); // think about if off will delete the req
    queue.head = &initializerReq;
    queue.numberOfNodes = 1;

    printf("Queue initialized\n");
    return queue;
}

void Attach_Request_To_Queue(Request *request, Queue *queue, int mCurrentFloor){
    bool attachBefore = true;
    if (queue->numberOfNodes >= MAX_QUEUE_NODE_AMOUNT) {
        printf("Cannot attach Request because the Queue has %d elements.\n", queue->numberOfNodes);
        return;
    }
    if (Request_Already_Exists_In_Queue(&request, &queue)) {
        printf("Won't attach Request because it already exists in Queue.\n");
        return;
    }
    Request* pRequest = Where_To_Attach_Request(&request, &queue, mCurrentFloor, &attachBefore);
    if (attachBefore) {
        Attach_Before_Request(pRequest, &request, &queue);
    } else {
        Attach_After_Request(pRequest, &request, &queue);
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

// Not finished
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

void Attach_Before_Request(Request *request, Request *requestToAttach, Queue *queue){
    if (request == NULL) {
        printf("Cannot attach before NULL!\n");
        return;
    }
    
    (queue->numberOfNodes)++;
}

void Attach_After_Request(Request *request, Request *requestToAttach, Queue *queue){
    if (request == NULL) {
        printf("Cannot attach after NULL!\n");
        return;
    }
    requestToAttach->pNextRequest = request->pNextRequest;
    request->pNextRequest = requestToAttach;
    requestToAttach->pPrevRequest = request;
    (queue->numberOfNodes)++;
}

// It is enough to detach the request. The memory will be freed automatically since request is not dynamically allocated.
void Delete_From_Queue(Request *request, Queue *queue){ // remember to FREE deleted node and its nextPointer
    if (queue->head == NULL || queue->numberOfNodes == 0) {
        printf("There are no elements in Queue. Cannot apply Delete_From_Queue().");
    }
    bool foundRequest = false;
    if (queue->head->floor == request->floor && queue->head->direction == request->direction) {
        Request *temp = queue->head;
        queue->head = queue->head->pNextRequest;
        queue->head->pNextRequest = NULL;
        free(temp);
        foundRequest = true;
    }

    for (Request *iteratorNode = queue->head; iteratorNode->pNextRequest != NULL; iteratorNode = iteratorNode->pNextRequest) {
        if (iteratorNode->pNextRequest->floor == request->floor && iteratorNode->pNextRequest->direction == request->direction) {
            iteratorNode->pNextRequest = iteratorNode->pNextRequest->pNextRequest;
            foundRequest = true;
            break;
        }
    }
    for (Request *iteratorNode = queue->head; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
        if (iteratorNode->pNextRequest != NULL && iteratorNode->floor == request->floor && iteratorNode->direction == request->direction) {
            Request *temp = iteratorNode;
            iteratorNode->pNextRequest = iteratorNode->pNextRequest->pNextRequest;
            iteratorNode->pNextRequest->pPrevRequest = iteratorNode->pPrevRequest;
            temp->pNextRequest = NULL;
            temp->pPrevRequest = NULL;
            free(temp);
            foundRequest = true;
            break;
        }
    }
    if (foundRequest) {
        (queue->numberOfNodes)--;
        printf("Request detached.\n");
    } else {
        printf("Request not in queue. Therefore cannot delete request!\n");
    }
}