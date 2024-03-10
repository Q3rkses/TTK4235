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
    if (Request_Already_Exists_In_Queue(request, queue)) {
        printf("Won't attach Request because it already exists in Queue.\n\n");
        return;
    }
    Request *pThis = Where_To_Attach_Request(request, queue, mCurrentFloor, &attachBefore);
    if (attachBefore) {
        Attach_Before_This(pThis, request, queue);
    } else {
        Attach_After_This(pThis, request, queue);
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
    MotorDirection elevatorDirn;
    if (request->floor - mCurrentFloor > 0) {
        elevatorDirn = DIRN_UP;
    } else if (request->floor - mCurrentFloor < 0) {
        elevatorDirn = DIRN_DOWN;
    }
    switch (request->direction)
    {
        // CABIN DONT WORK, OBST AND STOPTBN NOT FULLY IMPLEMENTED
        // WHEN DELETING REQ, DIRN MUST BE TAKEN INTO CONSIDERATION
    case BUTTON_CAB: // Request from inside the elevator. If there is a request from outside, which is in your path and is in the same direction add after that
        for (Request *iteratorNode = queue->head->pNextRequest; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
            if (iteratorNode == queue->tail) {
                *attachBefore = true;
                return iteratorNode;
            }
            bool requestInElevatorsWay = false;
            if ((request->floor < iteratorNode->floor && request->floor > mCurrentFloor) || (request->floor > iteratorNode->floor && request->floor < mCurrentFloor)) {
                requestInElevatorsWay = true;
            }
            if (requestInElevatorsWay) {
                *attachBefore = true;
                return iteratorNode;
            }
        }
        break;
    case BUTTON_HALL_UP: // Upwards request from outside the elevator. Oldest prioritized. Exeption: if the request from outside is in your path and is in the same direction
        for (Request *iteratorNode = queue->head->pNextRequest; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
            if (iteratorNode == queue->tail) {
                *attachBefore = true;
                return iteratorNode;
            }
            bool requestInElevatorsWay = false;
            if ((request->floor < iteratorNode->floor && request->floor > mCurrentFloor) || (request->floor > iteratorNode->floor && request->floor < mCurrentFloor)) {
                requestInElevatorsWay = true;
            }
            if (elevatorDirn == DIRN_UP && requestInElevatorsWay) {
                *attachBefore = true;
                return iteratorNode;
            }
        }
        break;
        // THIS DONT WORK
    case BUTTON_HALL_DOWN: // Downwards request from outside the elevator. Oldest prioritized. Exeption: if the request from outside is in your path and is in the same direction
        for (Request *iteratorNode = queue->head->pNextRequest; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
            if (iteratorNode == queue->tail) {
                *attachBefore = true;
                return iteratorNode;
            }
            bool requestInElevatorsWay = false;
            if ((request->floor < iteratorNode->floor && request->floor > mCurrentFloor) || (request->floor > iteratorNode->floor && request->floor < mCurrentFloor)) {
                requestInElevatorsWay = true;
            }
            if (elevatorDirn == DIRN_DOWN && requestInElevatorsWay) {
                *attachBefore = true;
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
        free(requestToAttach);
        return;
    }

    this->pPrevRequest->pNextRequest = requestToAttach;
    requestToAttach->pPrevRequest = this->pPrevRequest;
    this->pPrevRequest = requestToAttach;
    requestToAttach->pNextRequest = this;
    (queue->numberOfNodes)++;
    printf("Attached node adress %d to Queue before node adress %d\n\n", requestToAttach, this);
}

void Attach_After_This(Request *this, Request *requestToAttach, Queue *queue){
    if (this == queue->tail) {
        printf("Cannot attach after queue->tail!\n\n");
        free(requestToAttach);
        return;
    }

    this->pNextRequest->pPrevRequest = requestToAttach;
    requestToAttach->pNextRequest = this->pNextRequest;
    this->pNextRequest = requestToAttach;
    requestToAttach->pPrevRequest = this;
    (queue->numberOfNodes)++;
    printf("Attached node adress %d to Queue after node adress %d\n\n", requestToAttach, this);
}

void Delete_From_Queue(Request *request, Queue *queue){
    if (queue->head == NULL || queue->tail == NULL || queue->numberOfNodes < 2) {
        printf("Cannot apply Delete_From_Queue().\n\n");
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
            free(temp); // I think this is correct
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

// To test if this will work you should test the following scenario
// the elevator starts at floor 1, inside the person wants to go to floor 2
// at the same time someone at floor 2 will go down, but someone at floor 4 will
// also go down. The best behaviour here is to prioritize the cabin, then since you have arrived at 
// the floor go down for the person in floor 2, and then go up to 4
void Automatic_Deletion_From_Queue(Queue *queue, int mCurrentFloor, Door door, Elevatorpanel *panel){ // should this go on forever itself as well, because the while loop in main is going forever
    if (door.isOpen) {
        for (Request *iteratorNode = queue->head->pNextRequest; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
            if (iteratorNode->pPrevRequest == queue->head) {
                continue;
            }
            if (iteratorNode->pPrevRequest->floor == mCurrentFloor) {
                panel->PanelButtonState[iteratorNode->pPrevRequest->floor][iteratorNode->pPrevRequest->direction] = 0;
                Turn_Off_Elevator_Button_Lamp(iteratorNode->pPrevRequest->floor, iteratorNode->pPrevRequest->direction);
                Delete_From_Queue(iteratorNode->pPrevRequest, queue);
            }
        }
    }
}

void Empty_Queue(Queue *queue){
    for (Request *iteratorNode = queue->head->pNextRequest; iteratorNode != NULL; iteratorNode = iteratorNode->pNextRequest) {
        if (iteratorNode->pPrevRequest == queue->head) {
            continue;
        }
        Delete_From_Queue(iteratorNode->pPrevRequest, queue);
    }
}

void Queue_Print(Queue *pQueue){
    printf("QUEUE_PRINT BEGIN\n\n");
    int counter = 0;
    for (Request *i = pQueue->head; i != NULL; i = i->pNextRequest) {
        printf("%d. request:\nFloor: %d\nButtonType: %d\nOff: %d\nPrev node adress: %d\nCurrent node adress: %d\nNext node adress: %d\n\n", counter, i->floor, i->direction, i->off, i->pPrevRequest, i, i->pNextRequest);
        counter++;
    }
    printf("There are %d request(s) in Queue\n\n", pQueue->numberOfNodes);
    printf("QUEUE_PRINT END\n\n");
}