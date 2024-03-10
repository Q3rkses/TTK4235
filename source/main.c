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

    /**MAKE STRUCT OBJECTS*/
    Buttonhandler buttonhandler;
    Elevatorpanel panel;
    Request *pRequest = NULL;
    Door door;
    
    /**GLOBAL VARIABLES*/
    ButtonType mButtonType = -1;
    int mFloor = -1;
    MotorDirection mDirection = DIRN_STOP;
    time_t mTime = get_current_time();
    bool superstop = false;

    /**COUNTERS AND TEMP VALUES*/
    int mTimerCounter = 0;
    int mObstructionCounter = 0;
    int mTempDirection = 0;

    /**INITIALIZE STRUCTS*/
    elevio_init();
    Elevatorpanel_init(&panel);
    Request mHead = Request_Init(-1, DIRN_DOWN, false);
    Request mTail = Request_Init(-1, DIRN_DOWN, false);
    Queue mQueue = Queue_Init(&mHead, &mTail);

    printf("------------------------- GOING TO FIRST FLOOR TO BEFORE REQUESTS ARE ELIGEBLE -------------------------\n\n\n");

    while(elevio_floorSensor() != 0){
        elevio_motorDirection(DIRN_DOWN);
    } elevio_motorDirection(DIRN_STOP);

    for (int x = 0; x < 5; x++){
        printf("...\n");
    }

    printf("\n------------------------- ELEVATOR AT STARTING POSITION -------------------------\n\n\n");

    Queue_Print(&mQueue);

    while(1){
    
        /**------------------------- SENSOR INFORMATION -------------------------*/
        int mCurrentFloor = elevio_floorSensor();
        
        /**------------------------- FLOOR INDICATOR -------------------------*/
  
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
            Set_Elevator_Direction((mQueue.head->pNextRequest), mCurrentFloor, &mDirection);
            if (mCurrentFloor != -1 && superstop == false) {
                elevio_motorDirection(mDirection);
                }
        }

        /**------------------------- REQUEST IS ON DESIRED FLOOR -------------------------*/
        if(mCurrentFloor == mQueue.head->pNextRequest->floor){
            elevio_motorDirection(DIRN_STOP);
            Door_Open(&door);
            superstop = true;
            
            if (mTimerCounter == 0){
                mTime = get_current_time();
                mTimerCounter++;
            }

            /**------------------------- DELETING REQUEST AFTER 3 SECONDS HAVE PASSED -------------------------*/
            if (get_elapsed_time(mTime) > 2){
                for(int i = 0; i < 3; i++){
                    Automatic_Deletion_From_Queue(&mQueue, mCurrentFloor, door, &panel);
                }
                Door_Close(&door);
                mTimerCounter = 0;
                superstop = false;
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
            if (mObstructionCounter == 0){
                mTempDirection = mDirection;
                mObstructionCounter++;
            }
            elevio_motorDirection(DIRN_STOP);
            buttonhandler.ObstructionBtnState = true;
        }
        
        if (mObstructionCounter > 0 && !elevio_obstruction()){
            elevio_motorDirection(mTempDirection);
            mObstructionCounter = 0;
            buttonhandler.ObstructionBtnState = false;
        }
        
        nanosleep(&(struct timespec){0, 20*100*100}, NULL);
    }

    printf("------------------------- ELEVATOR STOP -------------------------\n");
    Empty_Queue(&mQueue);
    return 0;
}