#include "request.h"
#include <stdbool.h>

Request Request_Init(int floor, MotorDirection direction, bool off){
    Request req;
    req.floor = floor;
    req.direction = direction;
    req.off = off;

    printf("Request initialized with floor: %d, direction: %d, off: %d\n", req.floor, req.direction, req.off);
    return req;
};

int Request_Get_Floor(Request request){
    return request.floor;
};

MotorDirection Request_Get_Direction(Request request){ 
    return request.direction;
};

bool Request_Is_Getting_Off(Request request){ 
    return request.off;
};