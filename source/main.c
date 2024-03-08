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
#include "driver/timer.h"



int main(){
    
    printf("------------------------- ELEVATOR START -------------------------\n");
    printf("Press the stop button on the elevator panel to exit\n");

    Buttonhandler buttonhandler;
    Elevatorpanel panel;
    Request *pRequest = NULL;
    Door door;
    
    ButtonType mButtonType = -1;
    int mFloor = -1;
    MotorDirection mDirection = DIRN_STOP;
    time_t mTime = get_current_time();
    int mTimerCounter = 0;

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
            elevio_floorIndicator(0);

        } else if(mCurrentFloor == 1){
            elevio_floorIndicator(1);

        } else if(mCurrentFloor == 2){
            elevio_floorIndicator(2);

        } else if(mCurrentFloor == 3){
            elevio_floorIndicator(3);
        }


        /**------------------------- CHECK ELEVATOR PANEL BUTTONS -------------------------*/
        pRequest = Update_Button_Press(&panel, &mFloor, &mButtonType);
        if(pRequest != NULL){
            Attach_Request_To_Queue(pRequest, &mQueue, mCurrentFloor);
        }

        /**------------------------- MOVE TO FULLFULL REQUESTS -------------------------*/
        if(mQueue.head->pNextRequest != mQueue.tail){
            Request_Elevator_Direction((mQueue.head->pNextRequest), mCurrentFloor, &mDirection);
            elevio_motorDirection(mDirection);
        }

        /**------------------------- REQUEST IS ON DESIRED FLOOR -------------------------*/
        if(mCurrentFloor == mQueue.head->pNextRequest->floor){
            elevio_motorDirection(DIRN_STOP);
            Door_Open(door);
            
            if (mTimerCounter == 0){
                mTime = get_current_time();
                mTimerCounter++;
            }

            if (get_elapsed_time(mTime) > 3){
                Door_Close(door);
                mTimerCounter = 0;
/**------------------------- REMOVE REQUEST FROM QUEUE HERE -------------------------*/
            }
        }



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
        while(elevio_obstruction()){
            buttonhandler.ObstructionBtnState = true;
            elevio_motorDirection(DIRN_STOP);
        }
        buttonhandler.ObstructionBtnState = false;
        
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}