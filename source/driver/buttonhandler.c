#include "buttonhandler.h"

void Elevatorpanel_init(Elevatorpanel panel){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            panel.PanelButtonState[i][j] = 0;
            Turn_Off_Elevator_Button_Lamp(i, j);
        }
    }
    printf("------------------------- Elevator panel initialized -------------------------\n");
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

void Turn_On_Elevator_Button_Lamp(int floor, ButtonType type){
    elevio_buttonLamp(floor, type, 1);
}

void Turn_Off_Elevator_Button_Lamp(int floor, ButtonType type){
    elevio_buttonLamp(floor, type, 0);
}

int Update_Button_Press(Elevatorpanel panel, int temp){
    /**------------------------- CHECK ELEVATOR PANEL BUTTONS -------------------------*/
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f, b);
            
    /**------------------------- ELEVATOR BUTTON MATRIX (FLOOR LIGHT SYS) -------------------------*/
            if (btnPressed == 1){
                if(panel.PanelButtonState[f][b] == 0 && temp != f+b){
                        Turn_On_Elevator_Button_Lamp(f, b);
                        panel.PanelButtonState[f][b] = 1;
                        printf(" Setting light %d, %d to high \n", f, b);
                
                return f+b;
                } else if (panel.PanelButtonState[f][b] == 1 && temp != f+b){
                    Turn_Off_Elevator_Button_Lamp(f, b);
                    panel.PanelButtonState[f][b] = 0;
                    printf(" Setting light %d, %d to low \n", f, b);
                
                    return f+b;
                    }
                }
            }
        }
    }