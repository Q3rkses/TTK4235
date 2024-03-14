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
    bool mElevMoving = false;
    int nothingVarToInit = 0;
    int *mFloorLastCompletedRequest = &nothingVarToInit;

    /**COUNTERS AND TEMP VALUES*/
    int mTimerCounter = 0;
    int mObstructionCounter = 0;
    int mStopCounter = 0;
    int mTempDirection = 0;
    int mBetweenCounter = 0;
    int mTempFloor = 0;

    /**INITIALIZE STRUCTS*/
    elevio_init();
    Elevatorpanel_init(&panel);
    Door_init(&door);
    Buttonhandler_init(&buttonhandler);
    Request mHead = Request_Init(-2, DIRN_DOWN, false);
    Request mTail = Request_Init(-2, DIRN_DOWN, false);
    Queue mQueue = Queue_Init(&mHead, &mTail);

    printf("------------------------- GOING TO FIRST FLOOR TO BEFORE REQUESTS ARE ELIGEBLE -------------------------\n\n\n");

    while(elevio_floorSensor() != 0){
        elevio_motorDirection(DIRN_DOWN);
    } elevio_motorDirection(DIRN_STOP);

    for (int x = 0; x < 5; x++){
        printf("...\n");
    }

    printf("\n------------------------- ELEVATOR AT STARTING POSITION -------------------------\n\n\n");

    for (int x = 0; x < 5; x++){
        printf("...\n");
    }

    Queue_Print(&mQueue);

    while(1){
    
        /**------------------------- SENSOR INFORMATION -------------------------*/
        int mCurrentFloor = elevio_floorSensor();
        
        if (mCurrentFloor != -1){
            mTempFloor = mCurrentFloor;
        }
        
        /**------------------------- FLOOR INDICATOR -------------------------*/
        Floor_Light(mCurrentFloor);


        /**------------------------- CHECK ELEVATOR PANEL BUTTONS -------------------------*/
        pRequest = Update_Button_Press(&panel, &mFloor, &mButtonType);
        if(pRequest != NULL){
            Attach_Request_To_Queue(pRequest, &mQueue, Evaluate_Current_Floor(mDirection, mTempFloor), mDirection, mElevMoving, mFloorLastCompletedRequest, mTempFloor);
        }

        /**------------------------- REQUEST IS ON DESIRED FLOOR -------------------------*/
        if(mCurrentFloor == mQueue.head->pNextRequest->floor){
            superstop = true;
            mElevMoving = false;
            elevio_motorDirection(DIRN_STOP);
            Door_Open(&door);

            
            if (mTimerCounter == 0){
                mTime = get_current_time();
                mTimerCounter++;
            }
        }

            /**------------------------- DELETING REQUEST AFTER 3 SECONDS HAVE PASSED -------------------------*/
        
        if (get_elapsed_time(mTime) > 2 && door.isOpen == true){

            if(panel.PanelButtonState[mCurrentFloor][0] == true || panel.PanelButtonState[mCurrentFloor][1] == true || panel.PanelButtonState[mCurrentFloor][2] == true){

                for(int i = 0; i < 3; i++){
                    Automatic_Deletion_From_Queue(&mQueue, mCurrentFloor, door, &panel, mFloorLastCompletedRequest);
                }
                if (!elevio_obstruction()){
                    Door_Close(&door);
                    superstop = false;
                }
                mTimerCounter = 0;
            }
            
            if (!elevio_obstruction()){
                Door_Close(&door);
                superstop = false;
            }
            mTimerCounter = 0;
        }

        /**------------------------- MOVE TO FULLFULL REQUESTS -------------------------*/
        
        /**Move_To_Fulfill_Requests(&mQueue, &mDirection, mTempFloor, mCurrentFloor, &superstop, &mBetweenCounter);*/
        
        if(mQueue.head->pNextRequest != mQueue.tail){
            Set_Elevator_Direction((mQueue.head->pNextRequest), Evaluate_Current_Floor(mDirection, mTempFloor), &mDirection);

            if(!superstop && mCurrentFloor != -1){
                    elevio_motorDirection(mDirection);
                    mElevMoving = true;
                }
        }

        /**------------------------- STOP BUTTON FUNCTIONALITY -------------------------*/
        while(elevio_stopButton()){
            
            if(mQueue.numberOfNodes > 2){
                Empty_Queue(&mQueue, &panel);
            }

            if (mCurrentFloor != -1){
                Door_Open(&door);
            }

            if (mStopCounter == 0){
                /**Set all the correct states for variables*/
                superstop = true;
                mTimerCounter = 0;
                mStopCounter++;
            }

            elevio_motorDirection(DIRN_STOP);
            buttonhandler.StopBtnState = true;
            Turn_On_Stop_Button_Lamp();

        }

        /** Sleep for 3 second after button released then continue*/
        if (mStopCounter > 0 && !elevio_stopButton()){
            if (mTimerCounter == 0){
                Turn_Off_Stop_Button_Lamp();
                mTime = get_current_time();
                mTimerCounter++;
            }

            if (get_elapsed_time(mTime) > 2){
                /**Setting the correct states for variables*/
                superstop = false;
                buttonhandler.StopBtnState = false;
                mTimerCounter = 0;
                mStopCounter = 0;
                mBetweenCounter = 1;

                if(mCurrentFloor != -1){
                    /**If the elevator is at a floor hold the door open and close after 3 seconds*/
                    if(!elevio_obstruction()){
                    Door_Close(&door);
                    }
                }
            }
        }

        /** Start going in the supposed direction even when between floors after a stop */
        
        if (mBetweenCounter > 0 && !superstop){
            if(mQueue.head->pNextRequest != mQueue.tail){
                Set_Elevator_Direction((mQueue.head->pNextRequest), Evaluate_Current_Floor(mDirection, mTempFloor), &mDirection);
                
                if(mDirection != DIRN_STOP){
                    elevio_motorDirection(mDirection);
                    mBetweenCounter = 0;
                }
            }  
        }

        /**------------------------- OBSTRUCTION BUTTON FUNCTIONALITY -------------------------*/
        
        if (door.isOpen == true){
            if(elevio_obstruction()){
                superstop = true;
                if (mObstructionCounter == 0){
                    /**Set all the correct states for variables*/
                    mTimerCounter = 0;
                    mTempDirection = mDirection;
                    mObstructionCounter++;
                    }

                elevio_motorDirection(DIRN_STOP);
                buttonhandler.ObstructionBtnState = true;
                }

                if (mObstructionCounter > 0 && !elevio_obstruction()){
                    if (mTimerCounter == 0){
                        mTime = get_current_time();
                        mTimerCounter++;
                    }


                    if (get_elapsed_time(mTime) > 2){
                        /**Setting the correct states for variables*/
                        superstop = false;
                        buttonhandler.ObstructionBtnState = false;
                        mTimerCounter = 0;
                        mObstructionCounter = 0;
                        Door_Close(&door);
                    }
                }  
        }
        
        nanosleep(&(struct timespec){0, 50*100*100}, NULL);

    }

    printf("------------------------- ELEVATOR STOP -------------------------\n");
    Empty_Queue(&mQueue, &panel);
    return 0;
}