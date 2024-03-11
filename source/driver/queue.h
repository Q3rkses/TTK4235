/**
 * @file
 * @brief queueNode module, used to define the queue struct and functionality.
*/

#pragma once
#include "request.h"
#include "door.h"
#include "buttonhandler.h"
#define MAX_QUEUE_NODE_AMOUNT 12

/**
 * @brief Queue class, used to keep the request in order.
 * @param head, the begginning of the linked list class Queue
 * @param numberOfNodes, number of requests in Queue
*/
// Queue can max have 10 nodes
// Queue is 0 indexed
typedef struct Queue {
    Request *head;
    Request *tail;
    int numberOfNodes;
} Queue;

Queue Queue_Init(Request *head, Request *tail);

void Attach_Request_To_Queue(Request *request, Queue *queue, double mCurrentFloor, MotorDirection mDirection, int betweenFloors);

bool Request_Already_Exists_In_Queue(Request *request, Queue *queue);

/** 
 * @brief Where_To_Attach_Request function
 * @param attachBefore, attachBefore can be true or false, it does not matter
 * @return Directly returns a request pointer and changes attachBefore pointer accordingly to ensure correct assertion of request to queue
*/
Request* Where_To_Attach_Request(Request *request, Queue *queue, double mCurrentFloor, bool *attachBefore, MotorDirection mDirection, int betweenFloors);

void Attach_Before_This(Request *this, Request *requestToAttach, Queue *queue);

void Attach_After_This(Request *this, Request *requestToAttach, Queue *queue);

void Delete_From_Queue(Request *request, Queue *queue);

void Automatic_Deletion_From_Queue(Queue *queue, double mCurrentFloor, Door door, Elevatorpanel *panel);

void Empty_Queue(Queue *queue, Elevatorpanel *panel);

void Queue_Print(Queue *pQueue);