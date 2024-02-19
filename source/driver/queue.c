#include "queue.h"
#include <stdbool.h>

int Request_Get_Direction(Request req){
    if (req.direction == 0) {
        if (req.currentFloor < req.desiredFloor){
            req.direction = 1;
        }
        else if (req.currentFloor > req.desiredFloor){
            req.direction = -1;
        }
    }

    return req.direction;
};