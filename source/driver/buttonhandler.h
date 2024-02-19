/**
 * @file
 * @brief This file contains the buttonhandler functionality.
*/

#pragma once
#include <stdbool.h>
#include <elevio.h>

/**
 * @brief This enum is used to define the different types of buttons
*/

typedef struct buttonhandler{
    bool StopBtnState;
    bool ObstructionBtnState;
    
    ButtonType ElevatorButtonType;
    int floor;
    int ElevatorButtonState;
    
}buttonhandler;

/**
 * @brief This function returns the StopButton state
 * @return bool
*/

bool Get_Stop_Button_State(buttonhandler buttonhandler);

/**
 * @brief This function turns on the StopButton lamp
 * @return void
*/
void Turn_On_Stop_Button_Lamp(buttonhandler buttonhandler);

/**
 * @brief This function returns the ObstructionButton state
 * @return bool
*/

bool Get_Obstruction_Button_State(buttonhandler buttonhandler);

/**
 * @brief This function returns the ElevatorButton state
 * @return int
*/

int Get_Elevator_Button_State(buttonhandler buttonhandler);

/**
 * @brief This function turns on the ElevatorButton lamp
 * @return void
*/
void Turn_On_Elevator_Button_Lamp(buttonhandler buttonhandler);