#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

/**importing our own libraries*/
#include <driver/door.h>
#include <driver/queue.h>
#include <driver/request.h>
#include <driver/buttonhandler.h>



int main(){
    Buttonhandler buttonhandler;
    Elevatorpanel panel;

    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);

    while(1){
        /** Elevator position*/
        int floor = elevio_floorSensor();

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }


        /**------------------------- CHECK ELEVATOR PANEL BUTTONS -------------------------*/
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                
                /** TESTING IF MATRIX WORKS AS INTENTED*/
                if (btnPressed == 1 && panel.PanelButtonState[b][f] == 0){
                    panel.PanelButtonState[b][f] = 1;
                } else if (btnPressed == 1 && panel.PanelButtonState[b][f] == 1){
                    panel.PanelButtonState[b][f] = 0;
                }

            }
        }

        /**------------------------- STOP BUTTON FUNCTIONALITY -------------------------*/
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            buttonhandler.StopBtnState = true;
            Turn_On_Stop_Button_Lamp();

        /** Sleep for 1 second after button released then continue*/
        } else {
            nanosleep(&(struct timespec){0, 1000000000}, NULL);
            buttonhandler.StopBtnState = false;
            Turn_Off_Stop_Button_Lamp();

            /**implement continue here*/
            elevio_motorDirection(DIRN_DOWN);
        }
        
        /**------------------------- OBSTRUCTION BUTTON FUNCTIONALITY -------------------------*/
        if(elevio_obstruction()){
            elevio_motorDirection(DIRN_STOP);
            buttonhandler.ObstructionBtnState = true;
        } else {
            /**implement continue further requests here*/
            buttonhandler.ObstructionBtnState = false;
            elevio_motorDirection(DIRN_UP);
            
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}