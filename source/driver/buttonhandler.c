#include "buttonhandler.h"


bool Get_Stop_Button_State(Buttonhandler buttonhandler){
    if (elevio_stopButton == 0){
        buttonhandler.StopBtnState = false;
    }
    else{
        buttonhandler.StopBtnState = true;
    }

    return buttonhandler.StopBtnState;
}

void Turn_On_Stop_Button_Lamp(Buttonhandler buttonhandler){
    elevio_setStopLamp(1);
}

bool Get_Obstruction_Button_State(Buttonhandler buttonhandler){
    if (elevio_obstruction == 0){
        buttonhandler.ObstructionBtnState = false;
    }
    else{
        buttonhandler.ObstructionBtnState = true;
    }

    return buttonhandler.ObstructionBtnState;
}

void Turn_On_Elevator_Button_Lamp(ButtonType type, int floor){
    elevio_setButtonLamp(floor, type, 1);
}

void Turn_Off_Elevator_Button_Lamp(ButtonType type, int floor){
    elevio_setButtonLamp(floor, type, 0);
}