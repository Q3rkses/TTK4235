/**
 * @file
 * @brief queueNode module, used to define the queue struct and functionality.
*/

#pragma once
#define MAX_QUEUE_NODE_AMOUNT 10

int mQueueNodeCounter = 0;

/**
 * @brief QueueNode class, used to store information about the queue.
*/
// Queue can max have 10 nodes
typedef struct {
    int floor;
    QueueNode *pNextFloor;
} QueueNode;

QueueNode QueueNode_Init(void);

void Attach_QueueNode(int floor);

int Where_To_Attach_QueueNode();