#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "queue.h"
#include "buttonhandler.h"  
#include "elevio.h"
#include "request.h"


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

void Move_To_Fulfill_Requests(Queue *mQueue, int *mDirection, int mTempFloor, int mCurrentFloor, bool *superstop, int *mBetweenCounter);