/**
 * @file
 * @brief This file contains the buttonhandler functionality.
*/

#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "queue.h"
#include "elevio.h"
#include "request.h"

/**
 * @brief This class is used to define the different types of buttons
*/

typedef struct Buttonhandler {
    bool StopBtnState;
    bool ObstructionBtnState;
} Buttonhandler;

/**
 * @brief This class is used to define the button matrix of the elevatorpanel interface
*/

typedef struct Elevatorpanel {
    int PanelButtonState[N_FLOORS][3];
} Elevatorpanel;

/**
 * @brief Function that initializes the elevator panel, will be used to keep track of which buttons have been pressed.
 * @return bool
*/
void Elevatorpanel_init(Elevatorpanel *panel);

/**
 * @brief This function returns the StopButton state
 * @return bool
*/

bool Get_Stop_Button_State(Buttonhandler buttonhandler);

/**
 * @brief This function turns on the StopButton lamp
 * @return void
*/
void Turn_On_Stop_Button_Lamp(void);

/**
 * @brief This function turns on the StopButton lamp
 * @return void
*/
void Turn_Off_Stop_Button_Lamp(void);


/**
 * @brief This function returns the ObstructionButton state
 * @return bool
*/

bool Get_Obstruction_Button_State(Buttonhandler buttonhandler);

/**
 * @brief This function turns on the ElevatorButton lamp
 * @return void
*/
void Turn_On_Elevator_Button_Lamp(int floor, ButtonType type);


/**
 * @brief This function turns off the ElevatorButton lamp
 * @return void
*/
void Turn_Off_Elevator_Button_Lamp(int floor, ButtonType type);


/**
 * @brief Checks for buttons presses on the elevator panel, and updates the panel state.
 * @return void
*/

Request* Update_Button_Press(Elevatorpanel *panel, int *floor, ButtonType *btntype);

void Buttonhandler_init(Buttonhandler *buttonhandler);

double Evaluate_Current_Floor(MotorDirection direction, int current_floor);

/**
 * @brief This function stops the elevator and opens the door when the stop button is pressed
 * @param mQueue the queue of requests
 * @param buttonhandler the buttonhandler
 * @param panel the elevator panel
 * @param door the door
 * @param superstop trigger such that no requests can be accepted and that elevator stops in place
 * @param mTimerCounter counts how many times the counter has starter counting
 * @param mStopCounter counts if the stop button has been pressed
 * @param mTime the current time
 * @param mCurrentFloor the current floor
 * @return void
*/
void Stop_Button(Queue *mQueue, Buttonhandler *buttonhandler, Elevatorpanel *panel, Door *door, bool *superstop, int *mTimerCounter, int *mStopCounter, time_t *mTime, int *mCurrentFloor);