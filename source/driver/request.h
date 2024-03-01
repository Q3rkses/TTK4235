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
    Request *pNextRequest;
    Request *pPrevRequest;
    int floor;
    MotorDirection direction;
    bool off;
} Request;


/**
 * @brief Request_Get_Direction function, used to get the direction of a request.
 * @return int, the direction of the request.
*/

/**
 * @brief initializing the request class, used to store information about a request.
 * @param floor, the floor of the request.
 * @param direction, True if up, false if down.
 * @param off, True if someone is getting off at current floor.
*/

Request Request_Init(int floor, MotorDirection direction, bool off);

/**
 * @brief Request_Get_Floor function, used to get the desired floor of a request.
 * @param request, the request.
 * @return int, the desired floor of the request.
*/

int Request_Get_Floor(Request request);

/**
 * @brief Request_Get_Direction function, used to get the direction of a request.
 * @param request, the request.
 * @return bool, the direction of the request 1 is up, 0 is down.
*/

MotorDirection Request_Get_Direction(Request request);

/**
 * @brief Request_Is_Getting_Off function, used to check if someone is getting off at the current floor.
 * @param request, the request.
 * @return bool, 1 if someone is getting off, 0 if not.
*/
bool Request_Is_Getting_Off(Request request);