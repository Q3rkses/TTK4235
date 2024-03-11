#include "shortcuts.h"

void Stop_Button(Queue *mQueue, Buttonhandler *buttonhandler, Elevatorpanel *panel, Door *door, bool *superstop, int *mTimerCounter, int *mStopCounter, time_t *mTime, int *mCurrentFloor){
    
    while(elevio_stopButton()){
        if (*mCurrentFloor != -1){
                    Door_Open(door);
                }

                if (*mStopCounter == 0){
                    /**Set all the correct states for variables*/
                    Empty_Queue(mQueue, panel);
                    *superstop = true;
                    *mTimerCounter = 0;
                    *mStopCounter++;
                }

                elevio_motorDirection(DIRN_STOP);
                buttonhandler->StopBtnState = true;
                Turn_On_Stop_Button_Lamp();
            }

        /** Sleep for 1 second after button released then continue*/
        if (*mStopCounter > 0 && !elevio_stopButton()){
            if (*mTimerCounter == 0){
                *mTime = get_current_time();
                *mTimerCounter++;
            }

            if (get_elapsed_time(mTime) > 2){
                /**Setting the correct states for variables*/
                *superstop = false;
                buttonhandler->StopBtnState = false;
                Turn_Off_Stop_Button_Lamp();
                *mTimerCounter = 0;
                *mStopCounter = 0;

                if(*mCurrentFloor != -1){
                    /**If the elevator is at a floor hold the door open and close after 3 seconds*/
                    Door_Close(door);
                }
            }
        }
}

void Move_To_Fulfill_Requests(Queue *mQueue, int *mDirection, int mTempFloor, int mCurrentFloor, bool *superstop, int *mBetweenCounter){
    if(mQueue->head->pNextRequest != mQueue->tail){
        Set_Elevator_Direction((mQueue->head->pNextRequest), Evaluate_Current_Floor(mDirection, mTempFloor), mDirection);

        if(!(*superstop) && mCurrentFloor != -1){
                elevio_motorDirection(mDirection);
            }
        
        if (mBetweenCounter > 0 && !(*superstop)){
            if(mDirection != DIRN_STOP){
                        elevio_motorDirection(mDirection);
                        (*mBetweenCounter) = 0;
                        (*superstop) = false;
                    }
                }
            }
        }       
