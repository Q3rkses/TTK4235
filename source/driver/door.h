/**
 * @file door.h
 * @brief This file contains the door functionality.
*/

#pragma once
#include <stdbool.h>

/**
 * @brief This struct contains the door state.
*/
typedef struct Door{
    bool isOpen;
} Door;

/**
 * @brief This function turns on the door lamp
 * @return Door
*/
void Door_Open(Door *door);

/**
 * @brief This function turns off the door lamp
 * @return Door
*/
void Door_Close(Door *door);

/**
 * @brief This function initializes the door
 * @return Door
*/
void Door_init(Door *door);

void Floor_Light(int floor);