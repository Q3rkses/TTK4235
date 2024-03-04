/**
 * @file timer.h
 * @brief Timer functions for measuring time
 * 
*/
#pragma once
#include <time.h>


/**
 * @brief Start the timer
 * @return time_t 
 */
time_t get_current_time();

time_t get_elapsed_time(time_t start_time);