#include "buttonhandler.h"

void Elevatorpanel_init(Elevatorpanel panel){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 4; j++){
            panel.PanelButtonState[i][j] = 0;
        }
    }
    printf("Elevator panel initialized\n");
}

bool Get_Stop_Button_State(Buttonhandler buttonhandler){
    if (elevio_stopButton() == 0){
        buttonhandler.StopBtnState = false;
    }
    else{
        buttonhandler.StopBtnState = true;
    }

    return buttonhandler.StopBtnState;
}

void Turn_On_Stop_Button_Lamp(){
    elevio_stopLamp(1);
}

void Turn_Off_Stop_Button_Lamp(){
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

void Turn_On_Elevator_Button_Lamp(ButtonType type, int floor){
    elevio_ButtonLamp(floor, type, 1);
}

void Turn_Off_Elevator_Button_Lamp(ButtonType type, int floor){
    elevio_ButtonLamp(floor, type, 0);
}