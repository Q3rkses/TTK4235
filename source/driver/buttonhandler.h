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

typedef struct Buttonhandler{
    bool StopBtnState;
    bool ObstructionBtnState;
}Buttonhandler;

/**
 * @brief This function returns the StopButton state
 * @return bool
*/

bool Get_Stop_Button_State(Buttonhandler buttonhandler);

/**
 * @brief This function turns on the StopButton lamp
 * @return void
*/
void Turn_On_Stop_Button_Lamp(Buttonhandler buttonhandler);

/**
 * @brief This function returns the ObstructionButton state
 * @return bool
*/

bool Get_Obstruction_Button_State(Buttonhandler buttonhandler);

/**
 * @brief This function turns on the ElevatorButton lamp
 * @return void
*/
void Turn_On_Elevator_Button_Lamp(ButtonType type, int floor);


/**
 * @brief This function turns off the ElevatorButton lamp
 * @return void
*/
void Turn_Off_Elevator_Button_Lamp(ButtonType type, int floor);