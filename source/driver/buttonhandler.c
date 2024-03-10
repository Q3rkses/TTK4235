#include "buttonhandler.h"

void Elevatorpanel_init(Elevatorpanel *panel){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            panel->PanelButtonState[i][j] = 0;
            Turn_Off_Elevator_Button_Lamp(i, j);
        }
    }
    printf("------------------------- Elevator panel initialized -------------------------\n");
}

bool Get_Stop_Button_State(Buttonhandler buttonhandler){
    if (elevio_stopButton() == 1){
        buttonhandler.StopBtnState = true;
    }
    else{
        buttonhandler.StopBtnState = false;
    }

    return buttonhandler.StopBtnState;
}

void Turn_On_Stop_Button_Lamp(void){
    elevio_stopLamp(1);
}

void Turn_Off_Stop_Button_Lamp(void){
    elevio_stopLamp(0);
}

bool Get_Obstruction_Button_State(Buttonhandler buttonhandler){
    if (elevio_obstruction() == 0){
        buttonhandler.ObstructionBtnState = false;
    }
    else{
        buttonhandler.ObstructionBtnState = true;
    }

    return buttonhandler.ObstructionBtnState;
}

void Turn_On_Elevator_Button_Lamp(int floor, ButtonType type){
    elevio_buttonLamp(floor, type, 1);
}

void Turn_Off_Elevator_Button_Lamp(int floor, ButtonType type){
    elevio_buttonLamp(floor, type, 0);
}

Request* Update_Button_Press(Elevatorpanel *panel, int *floor, ButtonType *btntype){
    /**------------------------- CHECK ELEVATOR PANEL BUTTONS -------------------------*/
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f, b);
            
    /**------------------------- ELEVATOR BUTTON MATRIX (FLOOR LIGHT SYS) -------------------------*/
            if (btnPressed == 1){
                if (panel->PanelButtonState[f][b] == 0){
                        Turn_On_Elevator_Button_Lamp(f, b);
                        panel->PanelButtonState[f][b] = 1;
                        printf(" Setting light %d, %d to high \n", f, b);

                        /**RETURNS*/
                        *btntype = b;
                        *floor = f;

                        /**MAKING AND FILLING THE REQUEST*/
                        Request *pReq = (Request*)malloc(sizeof(Request));
                        pReq->floor = f;
                        pReq->direction = b;
                        pReq->off = false;
                        return pReq;
                    }
                }
            }
        }
        return NULL;
    }

void Buttonhandler_init(Buttonhandler *buttonhandler){
    Turn_Off_Stop_Button_Lamp();
    buttonhandler->StopBtnState = false;
    buttonhandler->ObstructionBtnState = false;
    printf("------------------------- Buttonhandler initialized -------------------------\n");
}

double Evaluate_Current_Floor(MotorDirection direction, int LastKnownFloor){
    
    if (direction == DIRN_UP){
        return LastKnownFloor + 0.5;
    }
    else if (direction == DIRN_DOWN){
        return LastKnownFloor - 0.5;
    }
    else{
        return LastKnownFloor;
    }
}

void Stop_Button(Queue *mQueue, Buttonhandler *buttonhandler, Elevatorpanel *panel, Door *door, bool *superstop, int *mTimerCounter, int *mStopCounter, time_t *mTime, int *mCurrentFloor){
    
    while(elevio_stopButton()){
        if (mCurrentFloor != -1){
                    Door_Open(&door);
                }

                if (mStopCounter == 0){
                    /**Set all the correct states for variables*/
                    Empty_Queue(&mQueue, &panel);
                    superstop = true;
                    mTimerCounter = 0;
                    mStopCounter++;
                }

                elevio_motorDirection(DIRN_STOP);
                buttonhandler->StopBtnState = true;
                Turn_On_Stop_Button_Lamp();
            }

        /** Sleep for 1 second after button released then continue*/
        if (mStopCounter > 0 && !elevio_stopButton()){
            if (mTimerCounter == 0){
                mTime = get_current_time();
                mTimerCounter++;
            }

            if (get_elapsed_time(mTime) > 2){
                /**Setting the correct states for variables*/
                superstop = false;
                buttonhandler->StopBtnState = false;
                Turn_Off_Stop_Button_Lamp();
                mTimerCounter = 0;
                mStopCounter = 0;

                if(mCurrentFloor != -1){
                    /**If the elevator is at a floor hold the door open and close after 3 seconds*/
                    Door_Close(&door);
                }
            }
        }
}