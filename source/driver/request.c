#include "request.h"
#include <stdbool.h>

Request Request_init(int floor, bool direction, bool off){
    Request req;
    req.floor = floor;
    req.direction = direction;
    req.off = off;
    return req;
};

int Request_Get_Floor(Request request){
    return request.floor;
};


bool Request_Get_Direction(Request request){ 
    return request.direction;
};

int Request_is_getting_off(Request request){ 
    return request.direction;
};