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
 * @param head, the begginning of the linked list
 * @param tail, the end of the linked list
 * @param numberOfNodes, number of requests in Queue
*/
// Queue can max have 10 nodes
// Queue is 0 indexed
typedef struct Queue {
    Request *head;
    Request *tail;
    int numberOfNodes;
} Queue;

/**
 * @brief Queue_Init function, used to initialize the queue.
 * @param head, the begginning of the linked list
 * @param tail, the end of the linked list
 * @return Queue, the initialized queue with a head- and tail-node.
*/
Queue Queue_Init(Request *head, Request *tail);

/**
 * @brief Attach_Request_To_Queue function, used to attach a request to the queue in the correct order.
 * @param request, the request to attach.
 * @param queue, the queue to attach the request to.
 * @param mCurrentFloor, the current floor of the elevator.
 * @param mDirection, the direction the elevator is moving in.
 * @param mElevMoving, true if the elevator is moving, false if not.
*/
void Attach_Request_To_Queue(Request *request, Queue *queue, double mCurrentFloor, MotorDirection mDirection, bool mElevMoving);

/**
 * @brief Request_Already_Exists_In_Queue function, used to check if a request already exists in the queue.
 * @param request, the request to check.
 * @param queue, the queue to check.
 * @return bool, true if the request already exists in the queue, false if not.
*/
bool Request_Already_Exists_In_Queue(Request *request, Queue *queue);

/** 
 * @brief Where_To_Attach_Request function
 * @param attachBefore, attachBefore can be true or false, it does not matter
 * @param request, request to attach
 *  @param queue, queue to attach request to
 * @param mCurrentFloor, current floor of elevator
 * @param mDirection, direction of elevator
 * @param mElevMoving, true if elevator is moving, false if not
 * @return Directly returns a request pointer and changes attachBefore pointer accordingly to ensure correct assertion of request to queue
*/
Request* Where_To_Attach_Request(Request *request, Queue *queue, double mCurrentFloor, bool *attachBefore, MotorDirection mDirection, bool mElevMoving);

/**
 * @brief Attach_Before_This function, used to attach a request before a specific request in the queue.
 * @param this, the request to attach before.
 * @param requestToAttach, the request to attach.
 * @param queue, the queue to attach the request to.
*/
void Attach_Before_This(Request *this, Request *requestToAttach, Queue *queue);

/**
 * @brief Attach_After_This function, used to attach a request after a specific request in the queue.
 * @param this, the request to attach after.
 * @param requestToAttach, the request to attach.
 * @param queue, the queue to attach the request to.
*/
void Attach_After_This(Request *this, Request *requestToAttach, Queue *queue);

/**
 * @brief Delete_From_Queue function, used to delete a specific request from the queue.
 * @param request, the request to delete.
 * @param queue, the queue to delete the request from.
*/
void Delete_From_Queue(Request *request, Queue *queue);

/**
 * @brief Automatic_Deletion_From_Queue function, used to delete a request from the queue when on the correct floor after 3 seconds have passed.
 * @param queue, the queue to delete the request from.
 * @param mCurrentFloor, the current floor of the elevator.
 * @param door, the door struct.
 * @param panel, the elevator panel.
*/
void Automatic_Deletion_From_Queue(Queue *queue, double mCurrentFloor, Door door, Elevatorpanel *panel);

/**
 * @brief Empty_Queue function, used to empty the queue and free memory, removes everything except head and tail.
 * @param queue, the queue to empty.
 * @param panel, the elevator panel.
*/
void Empty_Queue(Queue *queue, Elevatorpanel *panel);

/**
 * @brief Queue_Print function, used to print the queue.
 * @param pQueue, pointer to the queue which you want to print.
*/
void Queue_Print(Queue *pQueue);