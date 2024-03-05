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

Request Update_Button_Press(Elevatorpanel *panel, int *floor, ButtonType *btntype){
    Request req;

    /**------------------------- CHECK ELEVATOR PANEL BUTTONS -------------------------*/
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f, b);
            
    /**------------------------- ELEVATOR BUTTON MATRIX (FLOOR LIGHT SYS) -------------------------*/
            if (btnPressed == 1){
                if (panel->PanelButtonState[f][b] == 0 && (b*5+f) != (*btntype*5 + *floor)){
                        Turn_On_Elevator_Button_Lamp(f, b);
                        panel->PanelButtonState[f][b] = 1;
                        printf(" Setting light %d, %d to high \n", f, b);

                        /**RETURNS*/
                        *btntype = b;
                        *floor = f;

                        req = Request_Init(f, b, false);
                        return req;
                    }
                } 

            /** if (btnPressed == 1){
                if (panel->PanelButtonState[f][b] == 1 && (5*b+f) != (*btntype*5 + *floor)){
                    Turn_Off_Elevator_Button_Lamp(f, b);
                    panel->PanelButtonState[f][b] = 0;
                    printf(" Setting light %d, %d to low \n", f, b);

                    /**RETURNS
                    *btntype = b;
                    *floor = f;

                    Request req = Request_Init(f, b, false);
                    }
                } */
            }
        }
    }