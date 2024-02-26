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
 * @param direction, True if up, false if down.
 * @param off, True if someone is getting off at current floor.
*/
typedef struct {
    int floor;
    bool direction;
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

Request Request_Init(int floor, bool direction, bool off);

/**
 * @brief Request_Get_Floor function, used to get the desired floor of a request.
 * @param request, the request.
 * @return int, the desired floor of the request.
*/

int Request_Get_Floor(Request request);

/**
 * @brief Request_Get_Direction function, used to get the direction of a request.
 * @param request, the request.
 * @return int, the direction of the request.
*/

bool Request_Get_Direction(Request request);