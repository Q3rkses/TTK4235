#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

/**importing our own libraries*/
#include <door.h>
#include <queue.h>
#include <request.h>
#include <buttonhandler.h>



int main(){
    Buttonhandler buttonhandler;

    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);

    while(1){
        int floor = elevio_floorSensor();

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }


        /**checks all the buttons on the panel*/
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        /**STOP BUTTON FUNCTIONALITY*/
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);

        /** Sleep for 1 second after button released then continue*/
        } else {
            nanosleep(&(struct timespec){0, 1000000000}, NULL);

            elevio_stopLamp(0);
            /**implement continue here*/
            elevio_motorDirection(DIRN_DOWN);
        }
        
        if(elevio_obstruction()){
            elevio_motorDirection(DIRN_STOP);
        } else {
            /**implement continue further requests here*/
            elevio_motorDirection(DIRN_UP);
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}