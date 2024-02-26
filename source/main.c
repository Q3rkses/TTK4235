#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

/**importing our own libraries*/
#include "driver/door.h"
#include "driver/queue.h"
#include "driver/request.h"
#include "driver/buttonhandler.h"



int main(){
    Buttonhandler buttonhandler;
    Elevatorpanel panel;

    elevio_init();
    Elevatorpanel_init(panel);
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);

    while(1){

        /**------------------------- FLOOR INDICATOR -------------------------*/
       
        /** The Elevator position given by sensor*/
        int floor = elevio_floorSensor();
        int direction;

        /**Elevator Light position (works mostly)*/
        if(floor == 0){
            elevio_floorIndicator(0);

        } else if(floor == 1){
            elevio_floorIndicator(1);

        } else if(floor == 2){
            elevio_floorIndicator(2);


        } else if(floor == 3){
            elevio_floorIndicator(3);
        }


        Update_Button_Press(panel);


        /**------------------------- STOP BUTTON FUNCTIONALITY -------------------------*/
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            buttonhandler.StopBtnState = true;
            Turn_On_Stop_Button_Lamp();
            break;

        /** Sleep for 1 second after button released then continue*/
        } else{
            buttonhandler.StopBtnState = false;
            Turn_Off_Stop_Button_Lamp();

            /**implement continue here*/
        
        }
        
        /**------------------------- OBSTRUCTION BUTTON FUNCTIONALITY -------------------------*/
        if(elevio_obstruction()){
            elevio_motorDirection(DIRN_STOP);
            buttonhandler.ObstructionBtnState = true;
        } else {
            /**implement continue further requests here*/
            buttonhandler.ObstructionBtnState = false;
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}