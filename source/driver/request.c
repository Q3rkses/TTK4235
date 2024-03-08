#include "request.h"
#include <stdbool.h>

Request Request_Init(int floor, MotorDirection direction, bool off) {
    Request req;
    req.floor = floor;
    req.direction = direction;
    req.off = off;

    printf("Request initialized with floor: %d, direction: %d, off: %d\n\n", req.floor, req.direction, req.off);
    return req;
}

int Request_Get_Floor(Request request) {
    return request.floor;
}

MotorDirection Request_Get_Direction(Request request) { 
    return request.direction;
}

bool Request_Is_Getting_Off(Request request) { 
    return request.off;
}

void Request_Elevator_Direction(Request *request, int currFloor, MotorDirection *direction) { 
    switch(request->direction){
        case BUTTON_CAB:
            if(request->floor > currFloor){
                direction = DIRN_UP;
            } else if(request->floor < currFloor){
                direction = DIRN_DOWN;
            } else {
                direction = DIRN_STOP;
            }

        case BUTTON_HALL_DOWN:
            if(request->floor > currFloor){
                direction = DIRN_UP;
            } else if(request->floor < currFloor){
                direction = DIRN_DOWN;
            } else {
                direction = DIRN_STOP;
            
            }

        case BUTTON_HALL_UP:
            if(request->floor > currFloor){
                direction = DIRN_UP;
            } else if(request->floor < currFloor){
                direction = DIRN_DOWN;
            } else {
                direction = DIRN_STOP;
            }
    }
}