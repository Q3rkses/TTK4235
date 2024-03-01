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
    
    printf("------------------------- ELEVATOR START -------------------------\n");
    printf("Press the stop button on the elevator panel to exit\n");

    Buttonhandler buttonhandler;
    Elevatorpanel panel;

    elevio_init();
    Elevatorpanel_init(panel);
    elevio_motorDirection(DIRN_UP);
    
    printf("------------------------- GOING TO FIRST FLOOR TO BEFORE REQUESTS ARE ELIGEBLE -------------------------\n\n\n");
    while(elevio_floorSensor() != 0){
        elevio_motorDirection(DIRN_DOWN);
    } elevio_motorDirection(DIRN_STOP);
    printf("------------------------- ELEVATOR AT STARTING POSITION -------------------------\n\n\n");

    while(1){

        /**------------------------- FLOOR INDICATOR -------------------------*/
       
        /** The Elevator position given by sensor*/
        int mCurrentFloor = elevio_floorSensor();
        int mDirection;

        /**Elevator Light position*/
        if(mCurrentFloor == 0){
            int mDirection = DIRN_UP;
            elevio_floorIndicator(0);
            elevio_motorDirection(mDirection);

        } else if(mCurrentFloor == 1){
            elevio_floorIndicator(1);
            elevio_motorDirection(mDirection);


        } else if(mCurrentFloor == 2){
            elevio_floorIndicator(2);
            elevio_motorDirection(mDirection);

        } else if(mCurrentFloor == 3){
            elevio_floorIndicator(3);
            int mDirection = DIRN_DOWN;
        }


        /**------------------------- CHECK ELEVATOR PANEL BUTTONS -------------------------*/
        Update_Button_Press(panel);

        /** DEBUGGING
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int mBtnPressed = elevio_callButton(f, b);


                if (elevio_callButton(f, b)){
                    printf("Button pressed: %d, %d\n", f, );
                }
                

                if (mBtnPressed == 1 && panel.PanelButtonState[b][f] == 0){
                    panel.PanelButtonState[b][f] = 1;
                    printf("Button press registered as 1: %d, %d\n", b, f);
                    
                } else if (mBtnPressed == 1 && panel.PanelButtonState[b][f] == 1){
                    panel.PanelButtonState[b][f] = 0;
                    printf("Button press registered as 0: %d, %d\n", b, f);
                }
            }
        }
        
        */


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