#include "request.h"
#include <stdbool.h>

Request Request_Init(int floor, MotorDirection direction, bool off){
    Request req;
    req.floor = floor;
    req.direction = direction;
    req.off = off;

    printf("Request initialized with floor: %d, direction: %d, off: %d\n\n", req.floor, req.direction, req.off);
    return req;
};