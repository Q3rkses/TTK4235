/**
 * @file
 * @brief request module, used to define the request struct and functionality.
*/

#pragma once
#include "elevio.h"
#include <stdbool.h>

/**
 * @brief Request class, used to store information about a request.
 * @param floor, the floor of the request.
 * @param direction, DIRN_UP if upward request, DIRN_DOWN if downward request, 0 or DIRN_STOP if a cabin request
 * @param off, True if someone is getting off at current floor.
 * @param pNextRequest, next request in queue
*/
typedef struct Request{
    struct Request *pNextRequest;
    struct Request *pPrevRequest;
    int floor;
    MotorDirection direction;
    bool off;
} Request;

/**
 * @brief initializing the request class, used to store information about a request.
 * @param floor, the floor of the request.
 * @param direction, True if up, false if down.
 * @param off, True if someone is getting off at current floor.
*/
Request Request_Init(int floor, MotorDirection direction, bool off);