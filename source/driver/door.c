#include "door.h"
#include "elevio.h"


void Door_Open(Door door){
    door.isOpen = true;
    elevio_doorOpenLamp(1);
}

void Door_Close(Door door){
    door.isOpen = false;
    elevio_doorOpenLamp(0);
}

bool Get_Door_State(Door door){
    return door.isOpen;
}