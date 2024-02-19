/**
 * @file
 * @brief Queue module, used to define the request struct and implement the queue logic.
*/

#pragma once
#include "elevio.h"
#include <stdbool.h>

/**
 * @brief Request class, used to store information about a request.
*/
typedef struct {
    int currentFloor;
    int desiredFloor;
    int direction;
    bool inside;
} Request;

/**
 * @brief Queue class, used to store information about the queue.
*/
typedef struct {
    
} Queue;

/**
 * @brief Request_Get_Direction function, used to get the direction of a request.
 * @return int, the direction of the request.
*/
int Request_Get_Direction(Request request);