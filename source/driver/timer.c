#include "timer.h"

time_t get_current_time(){
    time_t seconds;
    seconds = time(NULL);
    return seconds;
}

time_t get_elapsed_time(time_t start_time){
    return(get_current_time() - start_time);
}