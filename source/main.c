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
#include "driver/request.h"
#include "driver/queue.h"



int main(){
    
    printf("------------------------- ELEVATOR START -------------------------\n");
    printf("Press the stop button on the elevator panel to exit\n");

    Buttonhandler buttonhandler;
    Elevatorpanel panel;
    
    ButtonType mButtonType = -1;
    int mFloor = -1;
    int mDirection = DIRN_STOP;

    elevio_init();
    Elevatorpanel_init(&panel);
    Request mHead = Request_Init(-1, DIRN_DOWN, false);
    Request mTail = Request_Init(-1, DIRN_DOWN, false);
    Queue mQueue = Queue_Init(&mHead, &mTail);

    /*---------------------Testing Queue System---------------------------*/
    Request req1 = Request_Init(4,DIRN_DOWN,false);
    Request req2 = Request_Init(3,DIRN_STOP,false);
    Request req3 = Request_Init(4,DIRN_DOWN,false);
    Request req4 = Request_Init(2,DIRN_DOWN,false);

    Queue_Print(&mQueue);
    /*Attach_After_This((&mQueue)->head, &req1, &mQueue);
    Queue_Print(&mQueue);
    Delete_From_Queue(&req1, &mQueue);
    Queue_Print(&mQueue);*/
    /*--------------------------------------------------------------------*/

    printf("------------------------- GOING TO FIRST FLOOR TO BEFORE REQUESTS ARE ELIGEBLE -------------------------\n\n\n");

    while(elevio_floorSensor() != 0){
        elevio_motorDirection(DIRN_DOWN);
    } elevio_motorDirection(DIRN_STOP);

    for (int x = 0; x < 5; x++){
        printf("...\n");
    }

    printf("\n------------------------- ELEVATOR AT STARTING POSITION -------------------------\n\n\n");

    while(1){
    
        /** The Elevator position given by sensor*/
        int mCurrentFloor = elevio_floorSensor();

        /*---------------------Testing Queue System---------------------------*/
        
        /**------------------------- FLOOR INDICATOR -------------------------*/
       

        /**Elevator Light position*/
        if(mCurrentFloor == 0){
            int mDirection = DIRN_STOP;
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
        Update_Button_Press(&panel, &mFloor, &mButtonType);

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